#include "Entity.h"
#include "core/Scene.h"
#include <algorithm>

namespace Espresso
{
	auto Entity::AddChild(const std::string& name) -> std::shared_ptr<Entity>
	{
		auto ent = Scene::main->AddEntity(name);
		_children.push_back(ent);
		return ent;
	}

	void Entity::RemoveComponent(const std::shared_ptr<Component>& comp)
	{
		auto itr = std::find(_components.begin(), _components.end(), comp);

		if (itr == _components.end())
		{
			es_coreWarn("Component {} not found",
						Internals::demangle(typeid(comp.get()).name()));
			return;
		}

		_components.erase(itr);
	}

	auto Entity::GetFirstChild() -> std::shared_ptr<Entity>
	{
		return _children.size() != 0 ? _children[0] : nullptr;
	}

	auto Entity::GetChildByName(const std::string& name) -> std::shared_ptr<Entity>
	{
		auto itr = std::find_if(_children.begin(), _children.end(),
								[=](auto itr) { return itr->GetName() == name; });

		if (itr == _children.end())
		{
			es_coreWarn("Entity {} not found!", name);
			return nullptr;
		}

		return *itr;
	}

	auto Entity::GetChildById(unsigned long id) -> std::shared_ptr<Entity>
	{
		auto itr = std::find_if(_children.begin(), _children.end(),
								[=](auto itr) { return itr->GetId() == id; });

		if (itr == _children.end())
		{
			es_coreWarn("Entity {} not found!", id);
			return nullptr;
		}

		return *itr;
	}

	auto Entity::GetLastChild() -> std::shared_ptr<Entity>
	{
		return _children.size() != 0 ? _children[_children.size() - 1] : nullptr;
	}

	void Entity::NotifyChange()
	{
		Transform._hasChanged = true;

		for (auto& child : _children)
		{
			child->NotifyChange();
		}
	}

	void Entity::RemoveChild(const std::shared_ptr<Entity>& child)
	{
		auto itr = std::find(_children.begin(), _children.end(), child);

		if (itr == _children.end())
		{
			es_coreWarn("Entity not found!");
			return;
		}

		_children.erase(itr);
	}

	void Entity::RemoveChildren()
	{
		_children.clear();
	}

	void Entity::_tick()
	{
		for (auto& comp : _components)
		{
			if (comp->IsEnabled())
			{
				comp->Update();
			}
		}

		Transform._tick();
	}
}