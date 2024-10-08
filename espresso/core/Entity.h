#pragma once
#include "components/Component.h"
#include "components/Transform.h"
#include "utils/Demangle.h"
#include "utils/threading/TickSystem.h"
#include <memory>
#include <string>
#include <vector>

namespace Espresso
{
	class Entity
	{
	public:
		inline static auto GlobalEntityCount() -> unsigned long
		{
			return globalEntCount;
		}

		struct Transform Transform;

		Entity(const std::string& name = "Entity") // NOLINT
			: _id(++globalEntCount), _name(name) {
			Transform._entity = this;
			  };

		template <typename T> auto AddComponent() -> std::shared_ptr<T>
		{
			if (!Internals::typeCheck<Component, T>())
			{
				return nullptr;
			}

			auto comp = std::make_shared<T>();
			comp->Entity = this;
			comp->Transform = &Transform;
			_components.push_back(comp);
			es_tickSystem->Enqueue([comp]() { comp->Start(); });

			return comp;
		}

		auto GetFirstChild() -> std::shared_ptr<Entity>;
		auto GetChildByName(const std::string& name) -> std::shared_ptr<Entity>;
		auto GetChildById(unsigned long id) -> std::shared_ptr<Entity>;
		auto GetLastChild() -> std::shared_ptr<Entity>;

		void RemoveChild(const std::shared_ptr<Entity>& child);
		void RemoveChildren();
		void NotifyChange();

		template <typename T> auto GetComponent() -> std::shared_ptr<T>
		{
			auto itr = std::find_if(_components,
									[=](auto itr) { return typeid(*itr) == typeid(T); });

			if (itr == _components.end())
			{
				es_coreWarn("Component {} not found!", Internals::demangle<T>());
				return nullptr;
			}

			return *itr;
		}
		void RemoveComponent(const std::shared_ptr<Component>& comp);

		template <typename T> void RemoveComponent()
		{
			std::erase_if(_components,
						  [=](auto itr) { return typeid(*itr) == typeid(T); });
		}

		inline void RemoveComponents()
		{
			_components.clear();
		}

		auto AddChild(const std::string& name = "Entity") -> std::shared_ptr<Entity>;

		auto Parent() -> Entity*
		{
			return _parent;
		}

		inline auto GetName() -> std::string
		{
			return _name;
		}

		[[nodiscard]] inline auto GetId() const -> unsigned long
		{
			return _id;
		}

		[[nodiscard]] inline auto IsEnabled() const -> bool
		{
			return _enabled;
		}

		[[nodiscard]] inline auto IsOrphan() const -> bool
		{
			return _parent == nullptr;
		}

	private:
		Entity* _parent{nullptr};
		unsigned long _id;
		std::string _name;
		bool _enabled{true};
		std::vector<std::shared_ptr<Component>> _components;
		std::vector<std::shared_ptr<Entity>> _children;

		inline static unsigned long globalEntCount;

		void _tick();

		friend class Scene;
	};
}