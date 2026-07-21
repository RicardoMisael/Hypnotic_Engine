/**
 * @file DeferredRenderer.h
 * @author Proyecto Engine
 * @brief Renderer basado en Deferred Shading para la etapa gr�fica principal.
 */

#pragma once

#include "Buffer.h"
#include "DepthStencilState.h"
#include "DepthStencilView.h"
#include "RasterizerState.h"
#include "Rendering/ISceneRenderer.h"
#include "Rendering/RenderScene.h"
#include "Rendering/RenderTypes.h"
#include "SamplerState.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "EngineUtilities/Utilities/EditorViewportPass.h"

class Device;
class DeviceContext;
class Camera;
class Material;

/**
 * @class DeferredRenderer
 * @brief Gestiona la renderizaci�n diferida utilizando m�ltiples buffers
 *        intermedios para almacenar informaci�n geom�trica y de iluminaci�n.
 *
 * Este renderer divide el proceso en distintas etapas:
 * - Shadow Pass
 * - Geometry Pass (GBuffer)
 * - Lighting Pass
 * - Forward Pass para elementos transparentes
 * - Skybox Pass
 */
class DeferredRenderer : public ISceneRenderer
{
public:

	/*==============================
	=          Lifecycle           =
	==============================*/

	HRESULT init(Device& device) override;

	void resize(Device& device,
		unsigned int width,
		unsigned int height) override;

	void render(DeviceContext& deviceContext,
		const Camera& camera,
		RenderScene& scene,
		EditorViewportPass& viewportPass) override;

	void destroy() override;

	/*==============================
	=         Debug Views          =
	==============================*/

	void setShadowFactorDebugEnabled(bool enabled) override
	{
		m_shadowFactorDebugEnabled = enabled;
	}

	void setDeferredDebugViewMode(int mode) override
	{
		m_deferredDebugViewMode = mode;
	}

	const char* getDebugName() const override
	{
		return "DeferredRenderer";
	}

	/*==============================
	=      Resource Accessors      =
	==============================*/

	ID3D11ShaderResourceView* getShadowMapSRV() const override
	{
		return m_shadowDepthSRV.m_textureFromImg;
	}

	ID3D11ShaderResourceView* getPreShadowSRV() const override
	{
		return m_preShadowDebugPass.getSRV();
	}

	ID3D11ShaderResourceView* getGBufferAlbedoMetallicSRV() const override
	{
		return m_gBufferAlbedoMetallicSRV.m_textureFromImg;
	}

	ID3D11ShaderResourceView* getGBufferNormalRoughnessSRV() const override
	{
		return m_gBufferNormalRoughnessSRV.m_textureFromImg;
	}

	ID3D11ShaderResourceView* getGBufferWorldAoSRV() const override
	{
		return m_gBufferWorldAoSRV.m_textureFromImg;
	}

	ID3D11ShaderResourceView* getGBufferEmissiveAlphaSRV() const override
	{
		return m_gBufferEmissiveAlphaSRV.m_textureFromImg;
	}

private:

	/*==============================
	=      Internal Pipeline       =
	==============================*/

	void buildQueues(RenderScene& scene,
		const Camera& camera);

	void updatePerFrame(const Camera& camera,
		const RenderScene& scene,
		DeviceContext& deviceContext);

	void updateLightMatrices(const Camera& camera,
		const RenderScene& scene);

	void renderSceneToTarget(DeviceContext& deviceContext,
		RenderScene& scene,
		EditorViewportPass& targetPass,
		bool applyShadows);

	/*==============================
	=      Render Stages           =
	==============================*/

	void renderShadowPass(DeviceContext& deviceContext);
	void renderGeometryPass(DeviceContext& deviceContext);
	void renderLightingPass(DeviceContext& deviceContext);
	void renderTransparentPass(DeviceContext& deviceContext);
	void renderSkyboxPass(DeviceContext& deviceContext,
		RenderScene& scene);

	/*==============================
	=     Per Object Drawing       =
	==============================*/

	void renderGeometryObject(DeviceContext& deviceContext,
		const RenderObject& object);

	void renderForwardObject(DeviceContext& deviceContext,
		const RenderObject& object,
		RenderPassType passType);

	void renderShadowObject(DeviceContext& deviceContext,
		const RenderObject& object);

	/*==============================
	=       Target Binding         =
	==============================*/

	void bindGBufferTargets(DeviceContext& deviceContext,
		ID3D11DepthStencilView* depthStencilView);

	void bindFinalTarget(DeviceContext& deviceContext,
		ID3D11RenderTargetView* renderTargetView,
		ID3D11DepthStencilView* depthStencilView);

	void clearDeferredSRVs(DeviceContext& deviceContext);

	/*==============================
	=      Resource Creation       =
	==============================*/

	HRESULT createShadowResources(Device& device);

	HRESULT createGBufferResources(Device& device,
		unsigned int width,
		unsigned int height);

	HRESULT createGBufferTarget(Device& device,
		unsigned int width,
		unsigned int height,
		DXGI_FORMAT format,
		Texture& texture,
		Texture& srv,
		RenderTargetView& rtv);

	HRESULT createLightingResources(Device& device);
	HRESULT createFullScreenQuad(Device& device);
	HRESULT createBlendStates(Device& device);

	ID3D11BlendState* resolveBlendState(const Material* material) const;

private:

	/* Constant Buffers */
	Buffer m_perFrameBuffer;
	Buffer m_perObjectBuffer;
	Buffer m_perMaterialBuffer;
	Buffer m_lightingDebugBuffer;

	/* Fullscreen Geometry */
	Buffer m_fullscreenVertexBuffer;
	Buffer m_fullscreenIndexBuffer;

	/* Depth States */
	DepthStencilState m_transparentDepthStencil;
	DepthStencilState m_disabledDepthStencil;
	DepthStencilState m_shadowDepthStencil;

	/* Blend States */
	ID3D11BlendState* m_alphaBlendState = nullptr;
	ID3D11BlendState* m_opaqueBlendState = nullptr;
	ID3D11BlendState* m_additiveBlendState = nullptr;
	ID3D11BlendState* m_premultipliedBlendState = nullptr;

	float m_blendFactor[4] = { 0.f, 0.f, 0.f, 0.f };

	/* Shadow Resources */
	Texture m_shadowDepthTexture;
	Texture m_shadowDepthSRV;
	DepthStencilView m_shadowDSV;
	ShaderProgram m_shadowShader;
	RasterizerState m_shadowRasterizer;

	unsigned int m_shadowMapSize = 2048;

	/* Deferred Shading Resources */
	ShaderProgram m_gBufferShader;
	ShaderProgram m_deferredLightingShader;
	SamplerState m_lightingSampler;
	RasterizerState m_fullscreenRasterizer;

	/* GBuffer */
	Texture m_gBufferAlbedoMetallicTexture;
	Texture m_gBufferAlbedoMetallicSRV;
	RenderTargetView m_gBufferAlbedoMetallicRTV;

	Texture m_gBufferNormalRoughnessTexture;
	Texture m_gBufferNormalRoughnessSRV;
	RenderTargetView m_gBufferNormalRoughnessRTV;

	Texture m_gBufferWorldAoTexture;
	Texture m_gBufferWorldAoSRV;
	RenderTargetView m_gBufferWorldAoRTV;

	Texture m_gBufferEmissiveAlphaTexture;
	Texture m_gBufferEmissiveAlphaSRV;
	RenderTargetView m_gBufferEmissiveAlphaRTV;

	EditorViewportPass m_preShadowDebugPass;

	bool m_applyShadows = true;

	unsigned int m_renderWidth = 1280;
	unsigned int m_renderHeight = 720;

	CBPerFrame m_cbPerFrame{};
	CBPerObject m_cbPerObject{};
	CBPerMaterial m_cbPerMaterial{};

	struct DeferredLightingDebugData
	{
		int DebugViewMode = 0;
		float ShadowStrength = 1.0f;
		float pad0 = 0.0f;
		float pad1 = 0.0f;
	} m_lightingDebugData{};

	bool m_shadowFactorDebugEnabled = false;
	int m_deferredDebugViewMode = 0;

	std::vector<const RenderObject*> m_opaqueQueue;
	std::vector<const RenderObject*> m_transparentQueue;
};