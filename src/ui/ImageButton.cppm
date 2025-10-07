export module Ortha.UI.ImageButton;

import Ortha.UI.Button;

import Ortha.Core.ResourceHandle;

export namespace Ortha::UI {

	struct ImageButton : Button {
		Core::ResourceHandlePtr image;
	};

} // namespace Ortha::UI
