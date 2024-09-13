#pragma once
#include "SDL_timer.h"

namespace Espresso::Threading
{
	class ThreadTime
	{
	public:
		inline ThreadTime()
		{
			_last = 0;
			_now = SDL_GetPerformanceCounter();
		}

		inline void StartMeasure()
		{
			_last = _now;
		}

		inline void EndMeasure()
		{
			_now = SDL_GetPerformanceCounter();
			_delta = ((static_cast<double>(_now) - static_cast<double>(_last)) * 1000 /
					  (double)SDL_GetPerformanceFrequency()) *
					 0.001F;
			_elapsed += _delta;
		}

		[[nodiscard]] inline auto GetDelta() const -> double
		{
			return _delta;
		}

		[[nodiscard]] inline auto GetTPS() const -> double
		{
			double safeTPS = 0;

			if (GetDelta() > 0)
			{
				if (GetDelta() < 1e-6F)
				{
					safeTPS = 1.0F / 1e-6F;
				}
				else
				{
					safeTPS = 1.0F / GetDelta();
				}
			}
			else
			{
				safeTPS = 0;
			}

			return safeTPS;
		}

	private:
		unsigned long _last;
		unsigned long _now;
		double _delta;
		double _elapsed;
	};
}