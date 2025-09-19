export module UI.ImageButton;

import UI.Button;

import Core.ResourceHandle;

export namespace UI {

	struct ImageButton : Button {
		Core::ResourceHandlePtr image;
	};

} // namespace UI
