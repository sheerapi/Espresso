#pragma once
#include "utils/Demangle.h"
#include <format>
#include <functional>
#include <string>

namespace Espresso
{
	class Object
	{
	public:
		virtual ~Object() = default;

		[[nodiscard]] inline auto GetName() const -> std::string
		{
			return Internals::demangle(typeid(*this).name());
		}

		[[nodiscard]] inline auto GetHash() const -> unsigned long
		{
			return std::hash<const Object*>()(this);
		}

		[[nodiscard]] virtual auto ToString() const -> std::string
		{
			return std::format("[{} ({})]", GetName(), static_cast<const void*>(this));
		}
	};
}