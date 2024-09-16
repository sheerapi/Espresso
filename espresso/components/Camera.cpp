#include "Camera.h"
#include "core/Application.h"

namespace Espresso::Components
{
	auto Camera::GetProjectionMatrix() -> Matrix4 // NOLINT
	{
		return (Type == CameraType::Perspective)
				   ? Matrix4::Perspective(
						 FOV, Application::main->GetWindow()->GetAspectRatio(), Near, Far)
				   : Matrix4::Ortho(-FOV, FOV, -FOV, FOV, Near, Far);
	}
}