
export module Ortha.UI.SystemsService;

export import Ortha.UI.Systems;

import kangaru;

export namespace Ortha::UI {

	void initUISubSystems(UISystems&, kgr::container&);

	struct UISystemsService
		: kgr::single_service<UISystems>
		, kgr::autocall<kgr::invoke_method<initUISubSystems>>{};

} // namespace Ortha::UI
