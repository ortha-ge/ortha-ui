module;

#include <entt/fwd.hpp>

export module UI.UIInputSystem;

import Core.EnTTRegistry;
import Core.Scheduler;

export namespace UI {

	class UIInputSystem {
	public:

		UIInputSystem(Core::EnTTRegistry&, Core::Scheduler&);
		~UIInputSystem();

		void tickSystem(entt::registry&);

	private:

		Core::EnTTRegistry& mRegistry;
		Core::Scheduler& mScheduler;
		Core::TaskHandle mTickHandle;

	};

} // namespace UI
