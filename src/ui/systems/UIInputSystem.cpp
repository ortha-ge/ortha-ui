module;

#include <utility>

module Ortha.UI.UIInputSystem;

import Ortha.Core.GlobalSpatial;
import Ortha.Gfx.Camera;
import Ortha.Input.MouseState;
import Ortha.UI.Button;
import Ortha.UI.ImageButton;
import entt;
import glm;

namespace Ortha::UI::UIInputSystemInternal {

	template <typename UIElement>
	void processUIElementMouseState(
		UIElement& element, const glm::vec3& cameraPosition, const Core::GlobalSpatial& spatial,
		const Input::MouseState& mouseState) {
		using namespace Input;

		const float halfQuadWidth = spatial.scale.x * 0.5f;
		const float halfQuadHeight = spatial.scale.y * 0.5f;

		const glm::vec3 localPosition = spatial.position - cameraPosition;

		const bool isPressed = isMouseButtonPressed(mouseState, MouseButton::Left);
		const bool isHoveredOver =
			mouseState.x > localPosition.x - halfQuadWidth && mouseState.x < localPosition.x + halfQuadWidth &&
			mouseState.y > localPosition.y - halfQuadHeight && mouseState.y < localPosition.y + halfQuadHeight;

		if (isHoveredOver) {
			element.state = isPressed ? ButtonState::Pressed : ButtonState::Hovered;
		} else {
			element.state = ButtonState::Default;
		}
	}

}

namespace Ortha::UI {

	UIInputSystem::UIInputSystem(entt::registry& registry, Core::Scheduler& scheduler)
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
		using namespace Ortha::Core;
		using namespace Gfx;
		using namespace Input;
		using namespace UIInputSystemInternal;

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
				processUIElementMouseState(button, cameraPosition, spatial, mouseState);
			});

		registry.view<ImageButton, GlobalSpatial>()
			.each([&mouseState, cameraPosition](ImageButton& button, const GlobalSpatial& spatial) {
				processUIElementMouseState(button, cameraPosition, spatial, mouseState);
			});
	}

} // namespace Ortha::UI
