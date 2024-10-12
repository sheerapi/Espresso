#include "Input.h"
#include "SDL_events.h"
#include "core/EventManager.h"

namespace Espresso
{
	void Input::Update()
	{
		scrollDelta.X = 0;
		scrollDelta.Y = 0;

		previousKeys = keys;
		keys.reset();

		previousMouse = mouse;
		mouse.reset();
	}

	void Input::Handle(void* data)
	{
		auto* e = (SDL_Event*)data;

		switch (e->type)
		{
		case SDL_KEYDOWN:
			if (e->key.repeat == 0U)
			{
				keys.set((int)e->key.keysym.scancode);
				EventManager::TriggerEvent("key.pressed", &e->key.keysym.scancode);
			}
			break;

		case SDL_KEYUP:
			keys.reset((int)e->key.keysym.scancode);
			EventManager::TriggerEvent("key.released", &e->key.keysym.scancode);
			break;

		case SDL_MOUSEMOTION:
			mousePos = Vector2((float)e->motion.x, (float)e->motion.y);
			break;

		case SDL_MOUSEBUTTONDOWN:
			mouse.set((int)e->button.button);
			EventManager::TriggerEvent("mouse.pressed", &e->button.button);
			break;

		case SDL_MOUSEBUTTONUP:
			mouse.reset((int)e->button.button);
			EventManager::TriggerEvent("mouse.released", &e->button.button);
			break;

		case SDL_MOUSEWHEEL:
			scrollDelta = Vector2((float)e->wheel.x, (float)e->wheel.y);
			break;

		case SDL_DROPFILE:
			EventManager::TriggerEvent("filedropped", e->drop.file);
			SDL_free(e->drop.file);
			break;

		case SDL_CONTROLLERDEVICEADDED:
			EventManager::TriggerEvent("controller.new", &e->cdevice.which);
			break;

		case SDL_CONTROLLERAXISMOTION:
		case SDL_CONTROLLERBUTTONDOWN:
		case SDL_CONTROLLERBUTTONUP:
		case SDL_CONTROLLERTOUCHPADMOTION:
		case SDL_CONTROLLERTOUCHPADDOWN:
		case SDL_CONTROLLERTOUCHPADUP:
		case SDL_CONTROLLERSENSORUPDATE:
		case SDL_CONTROLLERDEVICEREMOVED:
		case SDL_JOYDEVICEADDED:
		case SDL_JOYDEVICEREMOVED:
		case SDL_JOYBALLMOTION:
		case SDL_JOYBUTTONDOWN:
		case SDL_JOYBUTTONUP:
		case SDL_JOYHATMOTION:
		case SDL_JOYBATTERYUPDATED:
			break;

		default:
			break;
		}

		singleFrameKeys = keys & ~previousKeys;
		singleFrameMouse = mouse & ~previousMouse;
	}

	auto Input::GetKeyPressed(Keycode key) -> bool
	{
		std::lock_guard<std::mutex> lock(inputMutex);

		if (singleFrameKeys.test((int)key))
		{
			singleFrameKeys.reset((int)key);
			return true;
		}

		return false;
	}

	auto Input::IsKeyPressed(Keycode key) -> bool
	{
		return keys.test((int)key);
	}

	auto Input::IsKeyReleased(Keycode key) -> bool
	{
		return !keys.test((int)key);
	}

	auto Input::GetButtonPressed(MouseButton button) -> bool
	{
		std::lock_guard<std::mutex> lock(inputMutex);

		if (singleFrameMouse.test((int)button))
		{
			singleFrameMouse.reset((int)button);
			return true;
		}

		return false;
	}

	auto Input::IsButtonPressed(MouseButton button) -> bool
	{
		return mouse.test((int)button);
	}

	auto Input::IsButtonReleased(MouseButton button) -> bool
	{
		return !mouse.test((int)button);
	}

	auto Input::GetAnyKeyPressed() -> bool
	{
		std::lock_guard<std::mutex> lock(inputMutex);

		if (singleFrameKeys.any())
		{
			singleFrameKeys.reset();
			return true;
		}

		return false;
	}

	auto Input::IsAnyKeyPressed() -> bool
	{
		return keys.any();
	}

	auto Input::IsAnyKeyReleased() -> bool
	{
		return keys.none();
	}
}