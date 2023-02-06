#include "boost/asio/signal_set.hpp"

#include <iostream>
using std::cout;
using std::endl;

void handler(const boost::system::error_code& error, int signal_number) {
	cout << "handling signal:" << signal_number << " error:" << error.message() << endl;
}

int main() {
	boost::asio::io_context ctx;
	boost::asio::signal_set signals(ctx, SIGINT, SIGTERM);

	signals.async_wait(handler);

	ctx.run();

	system("pause");

	return 0;
}
