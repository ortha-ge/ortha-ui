module Ortha.UI.SystemsService;

import Ortha.UI.UIInputSystemService;
import Ortha.UI.UIRenderSystemService;

namespace Ortha::UI {

	void initUISubSystems(UISystems&, kgr::container& container) {
		container.emplace<UIInputSystemService>();
		container.emplace<UIRenderSystemService>();
	}

} // namespace Ortha::Audio
