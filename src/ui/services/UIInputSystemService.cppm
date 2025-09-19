
export module UI.UIInputSystemService;

export import UI.UIInputSystem;

import kangaru;
import Core.EnTTRegistryService;
import Core.SchedulerService;

export namespace UI {

	struct UIInputSystemService
		: kgr::single_service<UIInputSystem, kgr::dependency<Core::EnTTRegistryService, Core::SchedulerService>>{};

} // namespace UI
