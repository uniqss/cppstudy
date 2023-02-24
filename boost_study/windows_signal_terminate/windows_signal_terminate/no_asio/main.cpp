
#include <iostream>
using std::cout;
using std::endl;

#include <string>
#include <algorithm>
#include <thread>
#include <chrono>
#include <atomic>
#include <queue>
#include <mutex>

#include "ctrl-c.h"


std::atomic_bool g_terminating = false;
std::queue<std::shared_ptr<std::string>> g_msgq;

std::queue<std::shared_ptr<std::string>> g_inputs;
std::mutex g_inputs_mtx;
std::atomic_int g_wait_group = 0;

bool handler(enum CtrlCLibrary::CtrlSignal event) {
	cout << "handling event:" << event << endl;
	// if return false, the main process will exit directly, no matter system("pause") or std::this_thread::sleep_for called or not.
	// if return true, the main thread will not exit, but you can set something for the main thread to exit. but if it is a thread waiting for user input, it will not work.
	g_terminating = true;
	cout << "g_terminating set to true" << endl;

	return true;
}

void msg_thread() {
	++g_wait_group;
	cout << "msg_thread starting to work..." << endl;
	while (!g_terminating)
	{
		bool haveMsg = !g_msgq.empty();
		for (size_t i = 0; i < 100; ++i)
		{
			if (g_msgq.empty()) break;
			auto msg = g_msgq.front();
			g_msgq.pop();
			std::cout << *msg << endl;
		}

		if (!haveMsg)
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	cout << "msg_thread terminating..." << endl;
	--g_wait_group;
}

void input_handle_thread() {
	++g_wait_group;
	cout << "input_handle_thread starting to work..." << endl;
	while (!g_terminating)
	{
		bool empty = false;
		do {
			std::lock_guard<std::mutex> _(g_inputs_mtx);
			empty = g_inputs.empty();
			if (empty) break;
			auto input = g_inputs.front();
			g_inputs.pop();
			if (*input == "e" || *input == "exit" || *input == "q" || *input == "quit") {
				g_terminating = true;
				break;
			}
		} while (false);

		if (empty)
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
	cout << "input_handle_thread terminating..." << endl;
	--g_wait_group;
}

void input_thread() {
	++g_wait_group;
	cout << "input_thread starting to work..." << endl;
	while (!g_terminating)
	{
		std::string input;
		std::cin >> input;
		std::for_each(input.begin(), input.end(), std::tolower);
		do
		{
			std::lock_guard<std::mutex> _(g_inputs_mtx);
			auto input_op = std::make_shared<std::string>(input);
			g_inputs.push(input_op);
		} while (false);
	}
	cout << "input_thread terminating..." << endl;
	--g_wait_group;
}

int main()
{
	unsigned int handler_id = CtrlCLibrary::SetCtrlCHandler(handler);
	if (handler_id == CtrlCLibrary::kErrorID) {
		std::cerr << "Can't set ctrl+c handler" << std::endl;
		return -1;
	}

	std::thread t_msg_thread(msg_thread);
	t_msg_thread.detach();
	std::thread t_input_handle_thread(input_handle_thread);
	t_input_handle_thread.detach();
	std::thread t_input_thread(input_thread);
	t_input_thread.detach();

	while (!g_terminating) {
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	cout << "setting t_inpupt_thread  to nullptr" << endl;
	t_input_thread.~thread();

	cout << "main terminating..." << endl;

	while (g_wait_group > 0)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}


	return 0;
}