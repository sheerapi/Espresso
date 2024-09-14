#pragma once
#include <string>
#include <typeinfo>

#ifdef __GNUG__
#	include <cstdlib>
#	include <cxxabi.h>
#	include <memory>
#endif

namespace Espresso::Internals
{
	template <typename T> auto demangle() -> std::string
	{
		const auto* name = typeid(T).name();
#ifdef __GNUG__
		int status;

		std::unique_ptr<char, void (*)(void*)> res{
			abi::__cxa_demangle(name, nullptr, nullptr, &status), std::free};

		return (status == 0) ? res.get() : name;
#else
		return name;
#endif
	}

	inline auto demangle(const char* name) -> std::string
	{
#ifdef __GNUG__
		int status;

		std::unique_ptr<char, void (*)(void*)> res{
			abi::__cxa_demangle(name, nullptr, nullptr, &status), std::free};

		return (status == 0) ? res.get() : name;
#else
		return name;
#endif
	}

	template <typename T1, typename T2> auto typeCheck() -> bool
	{
		if (!std::is_base_of<T1, T2>())
		{
			es_coreError("Expected {}, got {}", demangle<T1>(), demangle<T2>());
			return false;
		}

		return true;
	}
}