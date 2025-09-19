module;

#include <utility>

#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

module UI.UIRenderSystem;

import Core.JsonTypeLoaderAdapter;
import Core.ResourceLoadRequest;
import Core.ResourceHandleUtils;
import Core.GlobalSpatial;
import Core.TypeLoader;
import Gfx.IndexBuffer;
import Gfx.Material;
import Gfx.RenderCommand;
import Gfx.ShaderProgram;
import Gfx.VertexBuffer;
import Gfx.Viewport;
import Gfx.Reflection.MaterialDescriptor;
import UI.Button;

namespace UI {

	UIRenderSystem::UIRenderSystem(Core::EnTTRegistry& registry, Core::Scheduler& scheduler)
		: mRegistry(registry)
		, mScheduler(scheduler) {

		auto typeLoaderAdapter = std::make_shared<Core::JsonTypeLoaderAdapter<Gfx::MaterialDescriptor>>();
		mUIMaterial = Core::ResourceLoadRequest::create<Core::TypeLoader>(registry, "assets/materials/ui.json", typeLoaderAdapter);

		mTickHandle = mScheduler.schedule([this] {
			tickSystem(mRegistry);
		});
	}

	UIRenderSystem::~UIRenderSystem() {
		mScheduler.unschedule(std::move(mTickHandle));
	}

	void UIRenderSystem::tickSystem(entt::registry& registry) {
		using namespace Core;
		using namespace Gfx;

		const auto* material = getResource<Gfx::Material>(registry, mUIMaterial);
		if (!material) {
			return;
		}

		const auto&& [shaderProgramEntity, shaderProgram] = Core::getResourceAndEntity<Gfx::ShaderProgram>(registry, material->shaderProgram);
		if (!shaderProgram) {
			return;
		}

		auto viewportView = registry.view<Viewport>();
		if (viewportView.empty()) {
			return;
		}

		const entt::entity viewportEntity = viewportView.front();

		struct UIVertex {
			glm::vec3 position;
			glm::vec3 color;
		};

		registry.view<Button, GlobalSpatial>()
			.each([this, &registry, shaderProgramEntity, viewportEntity](const Button& button, const GlobalSpatial& spatial) {
				const float halfQuadWidth = spatial.scale.x * 0.5f;
				const float halfQuadHeight = spatial.scale.y * 0.5f;

				VertexBuffer vertexBuffer;
				vertexBuffer.data.resize(sizeof(UIVertex) * 4);
				UIVertex* vertexHead = reinterpret_cast<UIVertex*>(vertexBuffer.data.data());

				const glm::vec3& buttonColour = button.state == ButtonState::Hovered   ? button.hoverColor
												: button.state == ButtonState::Pressed ? button.pressedColor
																					   : button.color;

				vertexHead[0] = { spatial.position + glm::vec3{ -halfQuadWidth, -halfQuadHeight, 0.0f}, buttonColour };
				vertexHead[1] = { spatial.position + glm::vec3{ -halfQuadWidth, halfQuadHeight, 0.0f}, buttonColour };
				vertexHead[2] = { spatial.position + glm::vec3{ halfQuadWidth, halfQuadHeight, 0.0f}, buttonColour };
				vertexHead[3] = { spatial.position + glm::vec3{ halfQuadWidth, -halfQuadHeight, 0.0f}, buttonColour };

				IndexBuffer indexBuffer;
				indexBuffer.data.resize(sizeof(uint16_t) * 6);
				uint16_t* indexHead = reinterpret_cast<uint16_t*>(indexBuffer.data.data());

				// 0, 1, 3,
				indexHead[0] = 0;
				indexHead[1] = 1;
				indexHead[2] = 3;
				// 3, 1, 2
				indexHead[3] = 3;
				indexHead[4] = 1;
				indexHead[5] = 2;

				RenderCommand renderCommand;
				renderCommand.viewportEntity = viewportEntity;
				renderCommand.shaderProgram = shaderProgramEntity;

				renderCommand.vertexBuffer = registry.create();
				renderCommand.vertexCount = 4;
				registry.emplace<VertexBuffer>(renderCommand.vertexBuffer, vertexBuffer);

				renderCommand.indexBuffer = registry.create();
				renderCommand.indexCount = 6;
				registry.emplace<IndexBuffer>(renderCommand.indexBuffer, indexBuffer);

				renderCommand.renderPass = 10;

				entt::entity renderCommandEntity = registry.create();
				registry.emplace<RenderCommand>(renderCommandEntity, renderCommand);
			});
	}

} // namespace UI
