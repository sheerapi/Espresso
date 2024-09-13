#pragma once
#include "core/Logger.h"
#include <atomic>
#include <functional>
#include <thread>

#define es_metaMask 0x7
#define es_addrMask ~es_metaMask

namespace Espresso
{
	template <typename T> class Ref
	{
	public:
		Ref(T* data)
		{
			es_coreDebug("Ref contained {} bytes at {}", sizeof(T), (void*)data);

			_data = data;
		}

		Ref(const Ref& other)
		{
			_refCount = other._refCount;

			_data = other._data;

			if (_data)
			{
				++(*_refCount);
				unsigned long threadId =
					std::hash<std::thread::id>()(std::this_thread::get_id());

				es_coreDebug("Thread {} copied {} bytes at {} ({} refs)", threadId,
							 sizeof(T), (void*)_data, _refCount->load());
			}
		}

		template <typename... Args> Ref(Args&&... args)
		{
			_data = new T(args...);
			unsigned long threadId =
				std::hash<std::thread::id>()(std::this_thread::get_id());

			es_coreDebug("Thread {} allocated {} bytes at {}", threadId, sizeof(T),
						 (void*)_data);
		}

		~Ref()
		{
			Release();
		}

		auto operator=(const Ref& other) -> Ref&
		{
			if (this != &other)
			{
				Release();
				_data = other._data;
				_refCount = other._refCount;

				if (_data)
				{
					++(*_refCount);
				}
			}
		}

		void Release()
		{
			if (_data && --(*_refCount) == 0)
			{
				unsigned long threadId =
					std::hash<std::thread::id>()(std::this_thread::get_id());

				es_coreDebug("Thread {} deallocated {} bytes at {}", threadId, sizeof(T),
							 (void*)_data);
				delete _data;
				delete _refCount;
			}
		}

		inline auto Get() const -> T*
		{
			return _data;
		}

		auto operator->() const -> T*
		{
			return _data;
		}

		auto operator*() const -> T&
		{
			return *_data;
		}

	private:
		T* _data;
		std::atomic<int>* _refCount{new std::atomic<int>(1)};
	};
}