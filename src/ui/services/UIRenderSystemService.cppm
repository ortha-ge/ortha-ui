
export module UI.UIRenderSystemService;

export import UI.UIRenderSystem;

import kangaru;
import Core.EnTTRegistryService;
import Core.SchedulerService;

export namespace UI {

	struct UIRenderSystemService
		: kgr::single_service<UIRenderSystem, kgr::dependency<Core::EnTTRegistryService, Core::SchedulerService>>{};

} // namespace UI
