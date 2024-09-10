#include "EventManager.h"
#include "Logger.h"
#include "utils/StringUtils.h"

namespace Espresso
{
	void EventManager::TriggerEvent(const std::string& name, void* data)
	{
		if (listeners.contains(stringToLower(name)))
		{
			for (auto& callback : listeners[stringToLower(name)])
			{
				(*callback)(data);
			}
		}
	}

	void EventManager::AddListener(const std::string& name, EventListener callback)
	{
		listeners[stringToLower(name)].push_back(
			std::shared_ptr<EventListener>(&callback));
	}

	void EventManager::RemoveListener(const std::string& name, EventListener callback)
	{
		if (listeners.contains(stringToLower(name)))
		{
			auto vec = listeners[stringToLower(name)];
			auto itr = std::find(vec.begin(), vec.end(),
								 std::shared_ptr<EventListener>(&callback));

			if (itr != vec.end())
			{
				vec.erase(itr);
			}
			else
			{
				es_coreError("Specified listener for event {} not found!", name);
			}
		}
		else
		{
			es_coreError("Event {} not registered!", name);
		}
	}

	void EventManager::RemoveListener(const std::string& name)
	{
		if (listeners.contains(stringToLower(name)))
		{
			listeners[stringToLower(name)].clear();
		}
		else
		{
			es_coreError("Event {} not registered!", name);
		}
	}

	void EventManager::RemoveAll()
	{
		listeners.clear();
	}
}