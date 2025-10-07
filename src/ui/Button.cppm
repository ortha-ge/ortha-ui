module;

#include <cstdint>
#include <string>


export module Ortha.UI.Button;

import glm;

export namespace Ortha::UI {

	enum class ButtonState : uint8_t {
		Default,
		Hovered,
		Pressed
	};

	struct Button {
		std::string text;
		glm::vec3 color;
		glm::vec3 hoverColor;
		glm::vec3 pressedColor;
		ButtonState state{ ButtonState::Default };
	};

} // namespace Ortha::UI
