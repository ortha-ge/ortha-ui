module;

#include <entt/fwd.hpp>

export module UI.UIRenderSystem;

import Core.EnTTRegistry;
import Core.ResourceHandle;
import Core.Scheduler;

export namespace UI {

	class UIRenderSystem {
	public:

		UIRenderSystem(Core::EnTTRegistry&, Core::Scheduler&);
		~UIRenderSystem();

		void tickSystem(entt::registry&);

	private:

		Core::EnTTRegistry& mRegistry;
		Core::Scheduler& mScheduler;
		Core::TaskHandle mTickHandle;

		Core::ResourceHandlePtr mUIMaterial;

	};

} // namespace UI
