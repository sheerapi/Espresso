#pragma once

namespace Espresso
{
	class Entity;

	class Component
	{
	public:
		inline static auto GlobalComponentCount() -> unsigned long
		{
			return globalCompCount;
		}

		class Entity* Entity;

		Component() : _id(++globalCompCount) {};

		[[nodiscard]] inline auto IsEnabled() const -> bool
		{
			return _enabled;
		}

		[[nodiscard]] inline auto GetID() const -> unsigned long
		{
			return _id;
		}

		virtual ~Component() = default;

		virtual void Start() {};
		virtual void Update() {};
		virtual void Destroy() {};

	private:
		bool _enabled{true};
		unsigned long _id;

		inline static unsigned long globalCompCount;
	};
}