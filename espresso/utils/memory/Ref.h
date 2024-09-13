#pragma once
#include "core/Logger.h"
#include <atomic>

namespace Espresso
{
	template <typename T> class Ref
	{
	public:
		explicit Ref(T* data)
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

				es_coreDebug("Copied {} bytes at {} ({} refs)", sizeof(T), (void*)_data,
							 _refCount->load());
			}
		}

		template <typename... Args> explicit Ref(Args&&... args)
		{
			_data = new T(args...);
			es_coreDebug("Allocated {} bytes at {}", sizeof(T), (void*)_data);
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

				es_coreDebug("Deallocated {} bytes at {}", sizeof(T), (void*)_data);
				delete _data;
				delete _refCount;
			}
			else if (_data)
			{
				es_coreDebug("Deallocated reference to {} ({} references remain)",
							 (void*)_data, _refCount->load());
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

		operator T() const
		{
			return *_data;
		}

		operator T&()
		{
			return *_data;
		}

	private:
		T* _data;
		std::atomic<int>* _refCount{new std::atomic<int>(1)};
	};
}

template <typename T, typename... Args> auto makeRef(Args&&... args) -> Espresso::Ref<T>
{
	return Espresso::Ref<T>(args...);
}