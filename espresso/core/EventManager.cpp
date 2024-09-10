#include "EventManager.h"
#include "Logger.h"
#include "utils/StringUtils.h"

namespace Espresso
{
	void EventManager::TriggerEvent(const std::string& name, void* data)
	{
		if (_listeners.contains(stringToLower(name)))
		{
			for (auto& callback : _listeners[stringToLower(name)])
			{
				(*callback)(data);
			}
		}
	}

	void EventManager::AddListener(const std::string& name, EventListener callback)
	{
		_listeners[stringToLower(name)].push_back(
			std::shared_ptr<EventListener>(&callback));
	}

	void EventManager::RemoveListener(const std::string& name, EventListener callback)
	{
		if (_listeners.contains(stringToLower(name)))
		{
			auto vec = _listeners[stringToLower(name)];
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
}