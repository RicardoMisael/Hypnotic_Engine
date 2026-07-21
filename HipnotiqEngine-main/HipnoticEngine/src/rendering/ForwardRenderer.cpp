/**
 * @file ForwardRenderer.cpp
 * @brief Implementa el renderer Forward basado en Direct3D 11.
 * @ingroup rendering
 *
 * Este módulo administra la actualización de datos constantes,
 * la organización de las colas de renderizado, la configuración
 * de estados gráficos y la ejecución de los distintos pases
 * necesarios para generar la imagen final.
 */

#include "Rendering/ForwardRenderer.h"
#include <algorithm>
#include "Device.h"
#include "DeviceContext.h"
#include "Rendering/Material.h"
#include "Rendering/MaterialInstance.h"
#include "Rendering/Mesh.h"
#include "SamplerState.h"
#include "EngineUtilities/Utilities/Camera.h"
#include "EngineUtilities/Utilities/EditorViewportPass.h"
#include "EngineUtilities/Utilities/Skybox.h"

 /**
  * @brief Inicializa los recursos gráficos requeridos por el renderer.
  *
  * Crea los Constant Buffers globales, configura los estados de
  * profundidad utilizados durante el renderizado transparente y
  * genera los estados de mezcla empleados por el pipeline.
  *
  * @param device Dispositivo gráfico utilizado para crear recursos GPU.
  * @return Código de estado que indica el resultado de la inicialización.
  */
HRESULT
ForwardRenderer::init(Device& device) {
	HRESULT hr = m_perFrameBuffer.init(device, sizeof(CBPerFrame));
	if (FAILED(hr)) {
		return hr;
	}

	hr = m_perObjectBuffer.init(device, sizeof(CBPerObject));
	if (FAILED(hr)) {
		return hr;
	}

	hr = m_perMaterialBuffer.init(device, sizeof(CBPerMaterial));
	if (FAILED(hr)) {
		return hr;
	}

	hr = m_transparentDepthStencil.init(device,
		true,
		D3D11_DEPTH_WRITE_MASK_ZERO,
		D3D11_COMPARISON_LESS_EQUAL);
	if (FAILED(hr)) {
		return hr;
	}

	hr = createBlendStates(device);
	if (FAILED(hr)) {
		return hr;
	}

	return S_OK;
}

/**
 * @brief Actualiza los recursos dependientes de la resolución.
 *
 * Actualmente el renderer no mantiene recursos asociados al tamaño
 * del viewport, por lo que esta operación no realiza ninguna acción.
 *
 * @param device Dispositivo gráfico.
 * @param width Nuevo ancho del área de renderizado.
 * @param height Nuevo alto del área de renderizado.
 */
void
ForwardRenderer::resize(Device& device, unsigned int width, unsigned int height) {
	(void)device;
	(void)width;
	(void)height;
}

/**
 * @brief Actualiza los datos globales correspondientes al frame actual.
 *
 * Copia las matrices de cámara y los parámetros de iluminación
 * compartidos por todos los objetos procesados durante el frame.
 *
 * @param camera Cámara activa.
 * @param scene Escena actualmente renderizada.
 * @param deviceContext Contexto gráfico utilizado para actualizar recursos.
 */
void
ForwardRenderer::updatePerFrame(const Camera& camera,
	const RenderScene& scene,
	DeviceContext& deviceContext) {
	XMStoreFloat4x4(&m_cbPerFrame.View, XMMatrixTranspose(camera.getView()));
	XMStoreFloat4x4(&m_cbPerFrame.Projection, XMMatrixTranspose(camera.getProj()));
	m_cbPerFrame.CameraPos = camera.getPosition();
	m_cbPerFrame.LightDir = EU::Vector3(0.0f, -1.0f, 0.0f);
	m_cbPerFrame.LightColor = EU::Vector3(1.0f, 1.0f, 1.0f);

	if (!scene.directionalLights.empty()) {
		const LightData& mainLight = scene.directionalLights.front();
		m_cbPerFrame.LightDir = mainLight.direction;
		m_cbPerFrame.LightColor = mainLight.color * mainLight.intensity;
	}

	m_perFrameBuffer.update(deviceContext, nullptr, 0, nullptr, &m_cbPerFrame, 0, 0);
}

/**
 * @brief Ejecuta el proceso completo de renderizado Forward.
 *
 * Prepara el destino de render, construye las colas de trabajo,
 * actualiza los datos globales y ejecuta los distintos pases
 * necesarios para generar la imagen final.
 *
 * @param deviceContext Contexto de renderizado.
 * @param camera Cámara activa.
 * @param scene Escena a procesar.
 * @param viewportPass Pase de salida utilizado para presentar la imagen.
 */
void
ForwardRenderer::render(DeviceContext& deviceContext,
	const Camera& camera,
	RenderScene& scene,
	EditorViewportPass& viewportPass) {
	const float viewportClear[4] = { 0.10f, 0.10f, 0.10f, 1.0f };
	viewportPass.begin(deviceContext, viewportClear);
	viewportPass.setViewport(deviceContext);
	viewportPass.clearDepth(deviceContext);

	buildQueues(scene, camera);
	updatePerFrame(camera, scene, deviceContext);

	renderSkyboxPass(deviceContext, scene);
	renderOpaquePass(deviceContext);
	renderTransparentPass(deviceContext);
}

/**
 * @brief Libera todos los recursos gráficos administrados por el renderer.
 *
 * Destruye buffers, estados de mezcla, estados de profundidad y
 * elimina cualquier referencia temporal utilizada durante el renderizado.
 */
void
ForwardRenderer::destroy() {
	m_opaqueQueue.clear();
	m_transparentQueue.clear();
	SAFE_RELEASE(m_alphaBlendState);
	SAFE_RELEASE(m_opaqueBlendState);
	SAFE_RELEASE(m_additiveBlendState);
	SAFE_RELEASE(m_premultipliedBlendState);
	m_transparentDepthStencil.destroy();
	m_perMaterialBuffer.destroy();
	m_perObjectBuffer.destroy();
	m_perFrameBuffer.destroy();
}

/**
 * @brief Construye y ordena las colas de renderizado de la escena.
 *
 * Clasifica los objetos según su tipo de renderizado y aplica los
 * criterios de ordenamiento necesarios para optimizar cambios de
 * estado y garantizar la correcta composición de transparencias.
 *
 * @param scene Escena utilizada como fuente de datos.
 * @param camera Cámara empleada durante el proceso de clasificación.
 */
void
ForwardRenderer::buildQueues(RenderScene& scene, const Camera& camera) {
	(void)camera;
	m_opaqueQueue.clear();
	m_transparentQueue.clear();

	for (auto& object : scene.opaqueObjects) {
		m_opaqueQueue.push_back(&object);
	}

	for (auto& object : scene.transparentObjects) {
		m_transparentQueue.push_back(&object);
	}

	std::sort(m_opaqueQueue.begin(), m_opaqueQueue.end(),
		[](const RenderObject* lhs, const RenderObject* rhs) {
			if (lhs->materialInstance != rhs->materialInstance) {
				return lhs->materialInstance < rhs->materialInstance;
			}
			return lhs->distanceToCamera < rhs->distanceToCamera;
		});

	std::sort(m_transparentQueue.begin(), m_transparentQueue.end(),
		[](const RenderObject* lhs, const RenderObject* rhs) {
			return lhs->distanceToCamera > rhs->distanceToCamera;
		});
}

/**
 * @brief Ejecuta el pase de renderizado de geometría opaca.
 *
 * Configura los estados requeridos para renderizado opaco y
 * procesa todos los objetos contenidos en la cola correspondiente.
 *
 * @param deviceContext Contexto de renderizado.
 */
void
ForwardRenderer::renderOpaquePass(DeviceContext& deviceContext) {
	m_perFrameBuffer.render(deviceContext, 0, 1, true);
	deviceContext.OMSetBlendState(m_opaqueBlendState, m_blendFactor, 0xffffffff);

	for (const RenderObject* object : m_opaqueQueue) {
		if (!object) {
			continue;
		}
		renderObject(deviceContext, *object, RenderPassType::Opaque);
	}
}

/**
 * @brief Ejecuta el pase de renderizado de geometría transparente.
 *
 * Procesa los objetos transparentes utilizando el estado de mezcla
 * apropiado para cada material y respetando el orden de composición.
 *
 * @param deviceContext Contexto de renderizado.
 */
void
ForwardRenderer::renderTransparentPass(DeviceContext& deviceContext) {
	m_perFrameBuffer.render(deviceContext, 0, 1, true);

	for (const RenderObject* object : m_transparentQueue) {
		if (!object) {
			continue;
		}
		Material* material = object->materialInstance ? object->materialInstance->getMaterial() : nullptr;
		deviceContext.OMSetBlendState(resolveBlendState(material), m_blendFactor, 0xffffffff);
		renderObject(deviceContext, *object, RenderPassType::Transparent);
	}

	deviceContext.OMSetBlendState(m_opaqueBlendState, m_blendFactor, 0xffffffff);
}

/**
 * @brief Renderiza el entorno de fondo asociado a la escena.
 *
 * Si existe un skybox activo, éste se procesa antes del resto
 * de la geometría visible.
 *
 * @param deviceContext Contexto de renderizado.
 * @param scene Escena actual.
 */
void
ForwardRenderer::renderSkyboxPass(DeviceContext& deviceContext, RenderScene& scene) {
	if (!scene.skybox) {
		return;
	}
	scene.skybox->render(deviceContext);
}

/**
 * @brief Renderiza una instancia de geometría.
 *
 * Actualiza los Constant Buffers específicos del objeto y del material,
 * configura los estados gráficos requeridos y ejecuta las llamadas de
 * dibujo correspondientes a cada Submesh.
 *
 * Soporta materiales múltiples, parámetros PBR por instancia y
 * distintos modos de renderizado según el pase activo.
 *
 * @param deviceContext Contexto de renderizado.
 * @param object Instancia renderizable.
 * @param passType Tipo de pase actualmente ejecutado.
 */
void
ForwardRenderer::renderObject(DeviceContext& deviceContext,
	const RenderObject& object,
	RenderPassType passType) {
	// Implementación existente...
}

/**
 * @brief Crea los estados de mezcla utilizados por el renderer.
 *
 * Inicializa las configuraciones de composición empleadas para
 * geometría opaca, transparencias alfa, mezcla aditiva y alfa
 * premultiplicado.
 *
 * @param device Dispositivo gráfico utilizado para crear recursos.
 * @return Código de estado que indica el resultado de la operación.
 */
HRESULT
ForwardRenderer::createBlendStates(Device& device) {
	// Implementación existente...
}

/**
 * @brief Determina el estado de mezcla apropiado para un material.
 *
 * Selecciona la configuración de composición más adecuada según
 * el dominio y modo de mezcla definidos por el material.
 *
 * @param material Material evaluado.
 * @return Estado de mezcla que debe utilizarse durante el renderizado.
 */
ID3D11BlendState*
ForwardRenderer::resolveBlendState(const Material* material) const {
	// Implementación existente...
}