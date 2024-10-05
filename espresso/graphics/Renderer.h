#pragma once

namespace Espresso::Graphics
{
	class Renderer
	{
	public:
		inline static Renderer* main{nullptr};

		virtual auto GetInternalBackendFlag() -> unsigned long = 0;

		static void ChooseBackend();
	};
}