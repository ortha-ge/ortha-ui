module;

#include <utility>

#include <entt/entt.hpp>
#include <glm/vec3.hpp>

module UI.UIInputSystem;

import Core.GlobalSpatial;
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
		using namespace Input;

		MouseState mouseState;
		registry.view<MouseState>()
			.each([&mouseState](const MouseState& inputMouseState) {
				mouseState = inputMouseState;
			});

		registry.view<Button, GlobalSpatial>()
			.each([&mouseState](Button& button, const GlobalSpatial& spatial) {
				const float halfQuadWidth = spatial.scale.x * 0.5f;
				const float halfQuadHeight = spatial.scale.y * 0.5f;

				// TODO: get screen space position of spatial
				if (mouseState.x > spatial.position.x - halfQuadWidth &&
					mouseState.x < spatial.position.x + halfQuadWidth &&
					mouseState.y > spatial.position.y - halfQuadHeight &&
					mouseState.y < spatial.position.y + halfQuadHeight) {

					const bool isPressed = isMouseButtonPressed(mouseState, MouseButton::Left);
					button.state = isPressed ? ButtonState::Pressed : ButtonState::Hovered;
				} else {
					button.state = ButtonState::Default;
				}
			});
	}

} // namespace UI
