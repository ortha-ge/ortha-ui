export module Ortha.UI.UIRenderSystem;

import Ortha.Core.ResourceHandle;
import Ortha.Core.Scheduler;
import entt;

export namespace Ortha::UI {

	class UIRenderSystem {
	public:

		UIRenderSystem(entt::registry&, Core::Scheduler&);
		~UIRenderSystem();

		void tickSystem(entt::registry&);

	private:

		entt::registry& mRegistry;
		Core::Scheduler& mScheduler;
		Core::TaskHandle mTickHandle;

		Core::ResourceHandlePtr mUIColourMaterial;
		Core::ResourceHandlePtr mUIImageMaterial;

	};

} // namespace Ortha::UI
