module;

#include <cstdint>
#include <utility>

#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

module UI.UIRenderSystem;

import Core.Any;
import Core.JsonTypeLoaderAdapter;
import Core.ResourceLoadRequest;
import Core.ResourceHandleUtils;
import Core.GlobalSpatial;
import Core.TypeLoader;
import Gfx.Image;
import Gfx.IndexBuffer;
import Gfx.Material;
import Gfx.RenderCommand;
import Gfx.ShaderProgram;
import Gfx.VertexBuffer;
import Gfx.Viewport;
import Gfx.Reflection.MaterialDescriptor;
import UI.Button;
import UI.ImageButton;

namespace UI::UIRenderSystemInternal {

	struct UIColourVertex {
		glm::vec3 position;
		glm::vec3 colour0;
	};

	struct UIImageVertex {
		glm::vec3 position;
		glm::vec2 texCoord0;
		glm::vec3 colour0;
	};

	Gfx::RenderCommand createUIColourElementRenderCommand(
		entt::registry& registry, const Button& element, const Core::GlobalSpatial& spatial, const entt::entity viewportEntity,
		const entt::entity shaderProgramEntity) {

		using namespace Gfx;

		const float halfQuadWidth = spatial.scale.x * 0.5f;
		const float halfQuadHeight = spatial.scale.y * 0.5f;

		VertexBuffer vertexBuffer;
		vertexBuffer.data.resize(sizeof(UIColourVertex) * 4);
		auto* vertexHead = reinterpret_cast<UIColourVertex*>(vertexBuffer.data.data());

		const glm::vec3& buttonColour = element.state == ButtonState::Hovered   ? element.hoverColor
										: element.state == ButtonState::Pressed ? element.pressedColor
																			   : element.color;

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
		return renderCommand;
	}

	Gfx::RenderCommand createUIImageElementRenderCommand(
		entt::registry& registry, const ImageButton& element, const Core::GlobalSpatial& spatial, const entt::entity viewportEntity,
		const entt::entity shaderProgramEntity, const entt::entity imageEntity) {
		using namespace Core;
		using namespace Gfx;

		const float halfQuadWidth = spatial.scale.x * 0.5f;
		const float halfQuadHeight = spatial.scale.y * 0.5f;

		VertexBuffer vertexBuffer;
		vertexBuffer.data.resize(sizeof(UIImageVertex) * 4);
		auto* vertexHead = reinterpret_cast<UIImageVertex*>(vertexBuffer.data.data());

		const glm::vec3& buttonColour = element.state == ButtonState::Hovered   ? element.hoverColor
										: element.state == ButtonState::Pressed ? element.pressedColor
																			   : element.color;

		vertexHead[0] = { spatial.position + glm::vec3{ -halfQuadWidth, -halfQuadHeight, 0.0f}, { 0.0f, 0.0f}, buttonColour };
		vertexHead[1] = { spatial.position + glm::vec3{ -halfQuadWidth, halfQuadHeight, 0.0f}, { 0.0f, 1.0f}, buttonColour };
		vertexHead[2] = { spatial.position + glm::vec3{ halfQuadWidth, halfQuadHeight, 0.0f}, { 1.0f, 1.0f}, buttonColour };
		vertexHead[3] = { spatial.position + glm::vec3{ halfQuadWidth, -halfQuadHeight, 0.0f}, { 1.0f, 0.0f}, buttonColour };

		IndexBuffer indexBuffer;
		indexBuffer.data.resize(sizeof(uint16_t) * 6);
		auto* indexHead = reinterpret_cast<uint16_t*>(indexBuffer.data.data());

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

		renderCommand.uniformData["s_texColour"] = Any(entt::entity{ imageEntity });
		renderCommand.uniformData["u_alphaColour"] = Any(glm::vec4{ 0.0f, 0.0f, 0.0f, 0.0f });

		return renderCommand;
	}
}

namespace UI {

	UIRenderSystem::UIRenderSystem(Core::EnTTRegistry& registry, Core::Scheduler& scheduler)
		: mRegistry(registry)
		, mScheduler(scheduler) {

		auto typeLoaderAdapter = std::make_shared<Core::JsonTypeLoaderAdapter<Gfx::MaterialDescriptor>>();
		mUIColourMaterial = Core::ResourceLoadRequest::create<Core::TypeLoader>(registry, "assets/materials/ui_colour.json", typeLoaderAdapter);
		mUIImageMaterial = Core::ResourceLoadRequest::create<Core::TypeLoader>(registry, "assets/materials/ui_image.json", typeLoaderAdapter);

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

		const auto* colourMaterial = getResource<Material>(registry, mUIColourMaterial);
		if (!colourMaterial) {
			return;
		}

		const auto* imageMaterial = getResource<Material>(registry, mUIImageMaterial);
		if (!imageMaterial) {
			return;
		}

		const auto&& [colourShaderProgramEntity, colourShaderProgram] = Core::getResourceAndEntity<ShaderProgram>(registry, colourMaterial->shaderProgram);
		if (!colourShaderProgram) {
			return;
		}

		const auto&& [imageShaderProgramEntity, imageShaderProgram] = Core::getResourceAndEntity<ShaderProgram>(registry, imageMaterial->shaderProgram);
		if (!imageShaderProgram) {
			return;
		}

		auto viewportView = registry.view<Viewport>();
		if (viewportView.empty()) {
			return;
		}

		const entt::entity viewportEntity = viewportView.front();

		registry.view<Button, GlobalSpatial>()
			.each([this, &registry, colourShaderProgramEntity, viewportEntity](const Button& button, const GlobalSpatial& spatial) {
				using namespace UIRenderSystemInternal;
				RenderCommand renderCommand = createUIColourElementRenderCommand(registry, button, spatial, viewportEntity, colourShaderProgramEntity);

				entt::entity renderCommandEntity = registry.create();
				registry.emplace<RenderCommand>(renderCommandEntity, std::move(renderCommand));
			});

		registry.view<ImageButton, GlobalSpatial>()
			.each([this, &registry, imageShaderProgramEntity, viewportEntity](const ImageButton& button, const GlobalSpatial& spatial) {
				using namespace UIRenderSystemInternal;
				auto&& [imageEntity, image] = getResourceAndEntity<Image>(registry, button.image);
				if (!image) {
					return;
				}

				RenderCommand renderCommand = createUIImageElementRenderCommand(registry, button, spatial, viewportEntity, imageShaderProgramEntity, imageEntity);

				entt::entity renderCommandEntity = registry.create();
				registry.emplace<RenderCommand>(renderCommandEntity, std::move(renderCommand));
			});
	}

} // namespace UI
