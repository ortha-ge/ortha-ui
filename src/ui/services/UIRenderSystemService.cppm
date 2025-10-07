
export module Ortha.UI.UIRenderSystemService;

export import Ortha.UI.UIRenderSystem;

import kangaru;
import Ortha.Core.EnTTRegistryService;
import Ortha.Core.SchedulerService;

export namespace Ortha::UI {

	struct UIRenderSystemService
		: kgr::single_service<UIRenderSystem, kgr::dependency<Core::EnTTRegistryService, Core::SchedulerService>>{};

} // namespace Ortha::UI
