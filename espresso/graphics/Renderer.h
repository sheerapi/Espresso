#pragma once

namespace Espresso::Graphics
{
	class Renderer
	{
	public:
		inline static Renderer* main{nullptr};

		virtual void Init() {};
		virtual void Shutdown() {};

		virtual void Swap() {};

		virtual auto GetInternalBackendFlag() -> unsigned long = 0;

		static void ChooseBackend();

	protected:
		void* ContextHandle;
	};
}