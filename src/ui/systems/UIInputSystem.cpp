module;

#include <utility>

#include <entt/entt.hpp>
#include <glm/vec3.hpp>

module UI.UIInputSystem;

import Core.GlobalSpatial;
import Gfx.Camera;
import Input.MouseState;
import UI.Button;

namespace UI {

	UIInputSystem::UIInputSystem(Core::EnTTRegistry& registry, Core::Scheduler& scheduler)
		: mRegistry(registry)
		, mScheduler(scheduler) {

		mTickHandle = mScheduler.schedule([this] {
			tickSystem(mRegistry);
		});
	}

	UIInputSystem::~UIInputSystem() {
		mScheduler.unschedule(std::move(mTickHandle));
	}

	void UIInputSystem::tickSystem(entt::registry& registry) {
		using namespace Core;
		using namespace Gfx;
		using namespace Input;

		MouseState mouseState;
		registry.view<MouseState>()
			.each([&mouseState](const MouseState& inputMouseState) {
				mouseState = inputMouseState;
			});

		glm::vec3 cameraPosition;
		registry.view<Camera, GlobalSpatial>()
			.each([&cameraPosition](const Camera&, GlobalSpatial& globalSpatial) {
				cameraPosition = globalSpatial.position;
			});

		registry.view<Button, GlobalSpatial>()
			.each([&mouseState, cameraPosition](Button& button, const GlobalSpatial& spatial) {
				const float halfQuadWidth = spatial.scale.x * 0.5f;
				const float halfQuadHeight = spatial.scale.y * 0.5f;

				const glm::vec3 localPosition = spatial.position - cameraPosition;

				// TODO: get screen space position of spatial
				if (mouseState.x > localPosition.x - halfQuadWidth &&
					mouseState.x < localPosition.x + halfQuadWidth &&
					mouseState.y > localPosition.y - halfQuadHeight &&
					mouseState.y < localPosition.y + halfQuadHeight) {

					const bool isPressed = isMouseButtonPressed(mouseState, MouseButton::Left);
					button.state = isPressed ? ButtonState::Pressed : ButtonState::Hovered;
				} else {
					button.state = ButtonState::Default;
				}
			});
	}

} // namespace UI
