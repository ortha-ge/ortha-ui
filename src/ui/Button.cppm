module;

#include <cstdint>
#include <string>

#include <glm/vec3.hpp>

export module UI.Button;

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
