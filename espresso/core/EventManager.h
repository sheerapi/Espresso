#pragma once
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace Espresso
{
	using EventListener = std::function<void(void*)>;

	class EventManager
	{
	public:
		void TriggerEvent(const std::string& name, void* data = nullptr);

		void AddListener(const std::string& name, EventListener callback);
		void RemoveListener(const std::string& name, EventListener callback);
		void RemoveListener(const std::string& name);

		void RemoveAll();

	private:
		std::unordered_map<std::string, std::vector<std::shared_ptr<EventListener>>>
			_listeners;
	};
}