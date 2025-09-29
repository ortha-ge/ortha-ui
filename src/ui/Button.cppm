module;

#include <cstdint>
#include <string>


export module UI.Button;

import glm;

export namespace UI {

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

} // namespace UI
