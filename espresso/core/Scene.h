#pragma once
#include "Entity.h"
#include "components/Camera.h"
#include <memory>
#include <vector>

namespace Espresso
{
	class Scene
	{
	public:
		inline static Scene* main{nullptr};

		Scene(const std::string& name = "Scene") : _name(name) // NOLINT
		{
			AddEntity("Main Camera")->AddComponent<Components::Camera>();
		};

		inline ~Scene()
		{
			RemoveEntities();
		}

		inline static void ChangeScene(Scene* newScene)
		{
			delete main;
			main = newScene;
		}

		inline auto AddEntity(const std::string& name = "Entity")
			-> std::shared_ptr<Entity>
		{
			auto ent = std::make_shared<Entity>(name);
			_entities.push_back(std::shared_ptr<Entity>(ent));
			return ent;
		}

		inline auto GetFirstEntity() -> std::shared_ptr<Entity>
		{
			return _entities.size() != 0 ? _entities[0] : nullptr;
		}

		inline auto GetEntityByName(const std::string& name) -> std::shared_ptr<Entity>
		{
			auto itr = std::find_if(_entities.begin(), _entities.end(),
									[=](auto itr) { return itr->GetName() == name; });

			if (itr == _entities.end())
			{
				es_coreWarn("Entity {} not found!", name);
				return nullptr;
			}

			return *itr;
		}

		inline auto GetEntityById(unsigned long id) -> std::shared_ptr<Entity>
		{
			auto itr = std::find_if(_entities.begin(), _entities.end(),
									[=](auto itr) { return itr->GetId() == id; });

			if (itr == _entities.end())
			{
				es_coreWarn("Entity {} not found!", id);
				return nullptr;
			}

			return *itr;
		}

		inline auto GetLastEntity() -> std::shared_ptr<Entity>
		{
			return _entities.size() != 0 ? _entities[_entities.size() - 1] : nullptr;
		}

		inline void RemoveEntity(const std::shared_ptr<Entity>& child)
		{
			auto itr = std::find(_entities.begin(), _entities.end(), child);

			if (itr == _entities.end())
			{
				es_coreWarn("Entity not found!");
				return;
			}

			_entities.erase(itr);
		}

		inline auto GetCurrentEntities() -> unsigned long
		{
			return _entities.size();
		}

		inline void RemoveEntities()
		{
			_entities.clear();
		}

		inline auto GetName() -> std::string
		{
			return _name;
		}

		inline void Tick()
		{
			for (auto& entity : _entities)
			{
				if (entity->IsEnabled())
				{
					entity->_tick();
				}
			}
		}

	private:
		std::string _name;
		std::vector<std::shared_ptr<Entity>> _entities;
	};
}