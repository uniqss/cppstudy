#ifndef _DLLPARSER_HPP_
#define _DLLPARSER_HPP_

#include <Windows.h>
#include <string>
#include <map>
#include <functional>
#include <iostream>

class DllParser {
public:
	DllParser() {}

	~DllParser() { UnLoad(); }

	bool Load(const std::string& dllPath) {
		m_hMod = LoadLibrary(dllPath.data());
		if (nullptr == m_hMod) {
			std::cout << "LoadLibrary failed\n";
			return false;
		}

		return true;
	}

	template <typename T, typename... Args>
#if ((defined(_MSVC_LANG) && _MSVC_LANG >= 201703L) || __cplusplus >= 201703L)
	// auto is ok.
	//auto ExcecuteFunc(const std::string& funcName, Args&&... args) {

	typename std::invoke_result<T, Args&&...>::type ExcecuteFunc(const std::string& funcName, Args&&... args) {
		// 下面两种都不行
		//typename std::invoke_result<std::function<T>(Args...)>::type ExcecuteFunc(const std::string& funcName, Args&&... args) {
		//typename std::invoke_result<std::function<T>(Args...), Args&&...>::type ExcecuteFunc(const std::string& funcName, Args&&... args) {
#else
	typename std::result_of<std::function<T>(Args...)>::type ExcecuteFunc(const std::string & funcName, Args&&... args) {
#endif
		T* f = GetFunction<T>(funcName);

		if (f == nullptr) {
			std::string s = "can not find this function " + funcName;
			throw std::exception(s.c_str());
		}

		return f(std::forward<Args>(args)...);
	}

private:
	bool UnLoad() {
		if (m_hMod == nullptr) return true;

		auto b = FreeLibrary(m_hMod);
		if (!b) return false;

		m_hMod = nullptr;
		return true;
	}

	template <typename T>
	T* GetFunction(const std::string & funcName) {
		//auto it = m_map.find(funcName);
		//if (it != m_map.end()) return it->second;
		auto addr = GetProcAddress(m_hMod, funcName.c_str());
		return (T*)(addr);
	}

private:
	HMODULE m_hMod;
	std::map<std::string, FARPROC> m_map;
	};

#endif  //_DLLPARSER_HPP_