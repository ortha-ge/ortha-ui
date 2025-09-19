
export module UI.SystemsService;

export import UI.Systems;

import kangaru;

export namespace UI {

	void initUISubSystems(UISystems&, kgr::container&);

	struct UISystemsService
		: kgr::single_service<UISystems>
		, kgr::autocall<kgr::invoke_method<initUISubSystems>>{};

} // namespace UI
