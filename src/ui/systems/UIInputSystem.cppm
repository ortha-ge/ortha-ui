export module UI.UIInputSystem;

import Core.Scheduler;
import entt;

export namespace UI {

	class UIInputSystem {
	public:

		UIInputSystem(entt::registry&, Core::Scheduler&);
		~UIInputSystem();

		void tickSystem(entt::registry&);

	private:

		entt::registry& mRegistry;
		Core::Scheduler& mScheduler;
		Core::TaskHandle mTickHandle;

	};

} // namespace UI
