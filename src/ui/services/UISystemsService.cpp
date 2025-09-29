module UI.SystemsService;

import UI.UIInputSystemService;
import UI.UIRenderSystemService;

namespace UI {

	void initUISubSystems(UISystems&, kgr::container& container) {
		container.emplace<UIInputSystemService>();
		container.emplace<UIRenderSystemService>();
	}

} // namespace Audio
