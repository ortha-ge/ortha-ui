module;

#include <cstdint>
#include <memory>
#include <utility>
#include <vector>

module Ortha.UI.UIRenderSystem;

import Ortha.RTTI.Any;
import Ortha.Core.JsonTypeLoaderAdapter;
import Ortha.Core.ResourceLoadRequest;
import Ortha.Core.ResourceHandleUtils;
import Ortha.Core.GlobalSpatial;
import Ortha.Core.Spatial;
import Ortha.RTTI.TypeId;
import Ortha.Core.TypeLoader;
import Ortha.Gfx.Camera;
import Ortha.Gfx.Image;
import Ortha.Gfx.IndexBuffer;
import Ortha.Gfx.Material;
import Ortha.Gfx.RenderCandidates;
import Ortha.Gfx.RenderCommand;
import Ortha.Gfx.ShaderProgram;
import Ortha.Gfx.VertexBuffer;
import Ortha.Gfx.Viewport;
import Ortha.Gfx.Reflection.MaterialDescriptor;
import Ortha.UI.Button;
import Ortha.UI.ImageButton;
import entt;
import glm;

namespace Ortha::UI::UIRenderSystemInternal {

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
		entt::registry& registry, const Button& element, const Core::GlobalSpatial& spatial, const Gfx::Camera& camera, const glm::mat4& viewMatrix,
		const entt::entity shaderProgramEntity) {

		using namespace Ortha::Gfx;

		const float halfQuadWidth = spatial.scale.x * 0.5f;
		const float halfQuadHeight = spatial.scale.y * 0.5f;

		VertexBuffer vertexBuffer;
		vertexBuffer.vertexLayout = shaderProgramEntity;
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
		renderCommand.viewportEntity = camera.viewport;
		renderCommand.shaderProgram = shaderProgramEntity;

		renderCommand.vertexBuffer = registry.create();
		renderCommand.vertexCount = 4;
		registry.emplace<VertexBuffer>(renderCommand.vertexBuffer, vertexBuffer);

		renderCommand.indexBuffer = registry.create();
		renderCommand.indexCount = 6;
		registry.emplace<IndexBuffer>(renderCommand.indexBuffer, indexBuffer);

		renderCommand.renderPass = 10;
		renderCommand.viewMatrix = viewMatrix;
		return renderCommand;
	}

	Gfx::RenderCommand createUIImageElementRenderCommand(
		entt::registry& registry, const ImageButton& element, const Core::GlobalSpatial& spatial, const Gfx::Camera& camera, const glm::mat4& viewMatrix,
		const entt::entity shaderProgramEntity, const entt::entity imageEntity) {
		using namespace Core;
		using namespace Ortha::Gfx;

		const float halfQuadWidth = spatial.scale.x * 0.5f;
		const float halfQuadHeight = spatial.scale.y * 0.5f;

		VertexBuffer vertexBuffer;
		vertexBuffer.vertexLayout = shaderProgramEntity;
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
		renderCommand.viewportEntity = camera.viewport;
		renderCommand.shaderProgram = shaderProgramEntity;

		renderCommand.vertexBuffer = registry.create();
		renderCommand.vertexCount = 4;
		registry.emplace<VertexBuffer>(renderCommand.vertexBuffer, vertexBuffer);

		renderCommand.indexBuffer = registry.create();
		renderCommand.indexCount = 6;
		registry.emplace<IndexBuffer>(renderCommand.indexBuffer, indexBuffer);

		renderCommand.renderPass = 10;

		renderCommand.uniformData["s_texColour"] = Ortha::RTTI::Any(entt::entity{ imageEntity });
		renderCommand.uniformData["u_alphaColour"] = Ortha::RTTI::Any(glm::vec4{ 0.0f, 0.0f, 0.0f, 0.0f });

		renderCommand.viewMatrix = viewMatrix;

		return renderCommand;
	}
}

namespace Ortha::UI {

	UIRenderSystem::UIRenderSystem(entt::registry& registry, Core::Scheduler& scheduler)
		: mRegistry(registry)
		, mScheduler(scheduler) {

		auto typeLoaderAdapter = std::make_shared<Core::JsonTypeLoaderAdapter<Ortha::Gfx::MaterialDescriptor>>();
		mUIColourMaterial = Core::ResourceLoadRequest::create<Core::TypeLoader>(registry, "assets/materials/ui_colour.json", typeLoaderAdapter);
		mUIImageMaterial = Core::ResourceLoadRequest::create<Core::TypeLoader>(registry, "assets/materials/ui_image.json", typeLoaderAdapter);

		mTickHandle = mScheduler.schedule([this] {
			tickSystem(mRegistry);
		}, 100);
	}

	UIRenderSystem::~UIRenderSystem() {
		mScheduler.unschedule(std::move(mTickHandle));
	}

	void UIRenderSystem::tickSystem(entt::registry& registry) {
		using namespace Core;
		using namespace Ortha::Gfx;

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

		registry.view<Camera>()
			.each([&registry](const entt::entity entity, const Camera&) {
				RenderCandidates& renderCandidates = registry.get_or_emplace<RenderCandidates>(entity);

				if (!renderCandidates.candidateBuckets.contains(Ortha::RTTI::TypeId::get<Button>())) {
					auto renderCandidateVisitor = [&registry](RenderCandidateBucket::EntityList& entityList, const entt::entity entity) {
						if (!registry.all_of<Button>(entity)) {
							return;
						}

						entityList.emplace_back(entity);
					};

					renderCandidates.candidateBuckets.emplace(Ortha::RTTI::TypeId::get<Button>(), renderCandidateVisitor);
				}

				if (!renderCandidates.candidateBuckets.contains(Ortha::RTTI::TypeId::get<ImageButton>())) {
					auto renderCandidateVisitor = [&registry](RenderCandidateBucket::EntityList& entityList, const entt::entity entity) {
						if (!registry.all_of<ImageButton>(entity)) {
							return;
						}

						entityList.emplace_back(entity);
					};

					renderCandidates.candidateBuckets.emplace(Ortha::RTTI::TypeId::get<ImageButton>(), renderCandidateVisitor);
				}
			});

		registry.view<Camera, Spatial, RenderCandidates>()
			.each([&registry, colourShaderProgramEntity, imageShaderProgramEntity](const entt::entity, const Camera& camera, const Spatial& cameraSpatial, const RenderCandidates& renderCandidates) {
				using namespace UIRenderSystemInternal;

				glm::mat4 translation = glm::translate(glm::mat4(1.0f), cameraSpatial.position);
				glm::mat4 rotation = glm::mat4_cast(cameraSpatial.rotation);
				glm::mat4 scale = glm::scale(glm::mat4(1.0f), cameraSpatial.scale);
				glm::mat4 viewMatrix{ glm::inverse(translation * rotation * scale) };

				if (renderCandidates.candidateBuckets.contains(Ortha::RTTI::TypeId::get<Button>())) {
					const auto& buttonRenderCandidates{ renderCandidates.candidateBuckets.at(Ortha::RTTI::TypeId::get<Button>()) };
					for (auto&& buttonEntity : buttonRenderCandidates.entityList) {
						if (!registry.all_of<Button, GlobalSpatial>(buttonEntity)) {
							continue;
						}

						const auto& [button, spatial] = registry.get<Button, GlobalSpatial>(buttonEntity);
						RenderCommand renderCommand = createUIColourElementRenderCommand(registry, button, spatial, camera, viewMatrix, colourShaderProgramEntity);
						entt::entity renderCommandEntity = registry.create();
						registry.emplace<RenderCommand>(renderCommandEntity, std::move(renderCommand));
					}
				}

				if (renderCandidates.candidateBuckets.contains(Ortha::RTTI::TypeId::get<ImageButton>())) {
					const auto& buttonRenderCandidates{ renderCandidates.candidateBuckets.at(Ortha::RTTI::TypeId::get<ImageButton>()) };
					for (auto&& buttonEntity : buttonRenderCandidates.entityList) {
						if (!registry.all_of<ImageButton, GlobalSpatial>(buttonEntity)) {
							continue;
						}

						const auto& [imageButton, spatial] = registry.get<ImageButton, GlobalSpatial>(buttonEntity);

						auto&& [imageEntity, image] = getResourceAndEntity<Image>(registry, imageButton.image);
						if (!image) {
							return;
						}

						RenderCommand renderCommand = createUIImageElementRenderCommand(registry, imageButton, spatial, camera, viewMatrix, imageShaderProgramEntity, imageEntity);
						entt::entity renderCommandEntity = registry.create();
						registry.emplace<RenderCommand>(renderCommandEntity, std::move(renderCommand));
					}
				}
			});
	}

} // namespace Ortha::UI
