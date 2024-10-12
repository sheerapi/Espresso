#pragma once
#include "Keycode.h"
#include "input/MouseButton.h"
#include "utils/math/Vector2.h"
#include <bitset>
#include <mutex>

namespace Espresso
{
	class Input
	{
	public:
		static auto IsKeyPressed(Keycode key) -> bool;
		static auto GetKeyPressed(Keycode key) -> bool;
		static auto IsKeyReleased(Keycode key) -> bool;

		static auto IsButtonPressed(MouseButton button) -> bool;
		static auto GetButtonPressed(MouseButton button) -> bool;
		static auto IsButtonReleased(MouseButton button) -> bool;

		static auto IsAnyKeyPressed() -> bool;
		static auto GetAnyKeyPressed() -> bool;
		static auto IsAnyKeyReleased() -> bool;

		static void Handle(void* data);
		static void Update();

		static inline auto GetMousePosition() -> Vector2
		{
			return mousePos;
		}

		static inline auto GetMouseDelta() -> Vector2
		{
			return scrollDelta;
		}

	private:
		inline static Vector2 mousePos;
		inline static Vector2 scrollDelta;

		inline static std::bitset<300> keys;
		inline static std::bitset<300> previousKeys;

		inline static std::bitset<300> singleFrameKeys;

		inline static std::bitset<5> mouse;
		inline static std::bitset<5> previousMouse;

		inline static std::bitset<5> singleFrameMouse;

		inline static std::mutex inputMutex;
	};
}