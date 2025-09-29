export module UI.UIRenderSystem;

import Core.ResourceHandle;
import Core.Scheduler;
import entt;

export namespace UI {

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

} // namespace UI
