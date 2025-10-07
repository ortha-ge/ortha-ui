
export module Ortha.UI.UIInputSystemService;

export import Ortha.UI.UIInputSystem;

import kangaru;
import Ortha.Core.EnTTRegistryService;
import Ortha.Core.SchedulerService;

export namespace Ortha::UI {

	struct UIInputSystemService
		: kgr::single_service<UIInputSystem, kgr::dependency<Core::EnTTRegistryService, Core::SchedulerService>>{};

} // namespace Ortha::UI
