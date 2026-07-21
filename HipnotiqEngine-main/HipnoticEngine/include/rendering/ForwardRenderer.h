/**
 * @file ForwardRenderer.h
 * @brief Define la clase ForwardRenderer responsable del pipeline de renderizado Forward.
 * @ingroup rendering
 */
#pragma once

#include "Prerequisites.h"
#include "Buffer.h"
#include "DepthStencilState.h"
#include "Rendering/RenderScene.h"
#include "Rendering/RenderTypes.h"

class Device;
class DeviceContext;
class Camera;
class EditorViewportPass;
class Material;

/**
 * @class ForwardRenderer
 * @brief Gestiona el renderizado de escenas mediante la técnica Forward Rendering.
 *
 * Esta clase se encarga de actualizar los datos constantes de GPU,
 * organizar los objetos de la escena en colas de renderizado y ejecutar
 * los distintos pases necesarios para generar la imagen final.
 */
class ForwardRenderer {
public:

    /**
     * @brief Inicializa los recursos gráficos requeridos por el renderer.
     *
     * @param device Dispositivo gráfico utilizado para la creación de recursos.
     * @return Código de estado que indica si la inicialización fue exitosa.
     */
    HRESULT init(Device& device);

    /**
     * @brief Actualiza los recursos dependientes de la resolución actual.
     *
     * @param device Dispositivo gráfico.
     * @param width Nuevo ancho del área de renderizado.
     * @param height Nuevo alto del área de renderizado.
     */
    void resize(Device& device, unsigned int width, unsigned int height);

    /**
     * @brief Actualiza la información constante utilizada durante el frame.
     *
     * @param camera Cámara activa.
     * @param scene Escena que será renderizada.
     * @param deviceContext Contexto de ejecución de comandos gráficos.
     */
    void updatePerFrame(const Camera& camera,
        const RenderScene& scene,
        DeviceContext& deviceContext);

    /**
     * @brief Ejecuta el proceso completo de renderizado de la escena.
     *
     * @param deviceContext Contexto de renderizado.
     * @param camera Cámara activa.
     * @param scene Escena a procesar.
     * @param viewportPass Pase final de presentación en el viewport.
     */
    void render(DeviceContext& deviceContext,
        const Camera& camera,
        RenderScene& scene,
        EditorViewportPass& viewportPass);

    /**
     * @brief Libera todos los recursos gráficos administrados por la clase.
     */
    void destroy();

private:

    /**
     * @brief Genera las colas de renderizado para objetos opacos y transparentes.
     *
     * @param scene Escena fuente.
     * @param camera Cámara utilizada para criterios de ordenamiento.
     */
    void buildQueues(RenderScene& scene, const Camera& camera);

    /**
     * @brief Ejecuta el pase de renderizado de geometría opaca.
     *
     * @param deviceContext Contexto de renderizado.
     */
    void renderOpaquePass(DeviceContext& deviceContext);

    /**
     * @brief Ejecuta el pase de renderizado de geometría transparente.
     *
     * @param deviceContext Contexto de renderizado.
     */
    void renderTransparentPass(DeviceContext& deviceContext);

    /**
     * @brief Renderiza el skybox asociado a la escena actual.
     *
     * @param deviceContext Contexto de renderizado.
     * @param scene Escena que contiene el skybox.
     */
    void renderSkyboxPass(DeviceContext& deviceContext,
        RenderScene& scene);

    /**
     * @brief Renderiza un objeto utilizando el pase especificado.
     *
     * @param deviceContext Contexto de renderizado.
     * @param object Objeto a dibujar.
     * @param passType Tipo de pase de renderizado.
     */
    void renderObject(DeviceContext& deviceContext,
        const RenderObject& object,
        RenderPassType passType);

    /**
     * @brief Crea los estados de mezcla utilizados durante el renderizado.
     *
     * @param device Dispositivo gráfico.
     * @return Código de estado de la operación.
     */
    HRESULT createBlendStates(Device& device);

    /**
     * @brief Determina el estado de mezcla adecuado para un material.
     *
     * @param material Material a evaluar.
     * @return Puntero al estado de mezcla correspondiente.
     */
    ID3D11BlendState* resolveBlendState(const Material* material) const;

private:

    /** Buffer constante con datos globales actualizados cada frame. */
    Buffer m_perFrameBuffer;

    /** Buffer constante con datos específicos de cada objeto renderizado. */
    Buffer m_perObjectBuffer;

    /** Buffer constante con parámetros asociados a materiales. */
    Buffer m_perMaterialBuffer;

    /** Estado Depth-Stencil empleado durante el renderizado transparente. */
    DepthStencilState m_transparentDepthStencil;

    /** Estado de mezcla para transparencia alfa tradicional. */
    ID3D11BlendState* m_alphaBlendState = nullptr;

    /** Estado de mezcla utilizado para geometría opaca. */
    ID3D11BlendState* m_opaqueBlendState = nullptr;

    /** Estado de mezcla aditiva para efectos luminosos. */
    ID3D11BlendState* m_additiveBlendState = nullptr;

    /** Estado de mezcla basado en alfa premultiplicado. */
    ID3D11BlendState* m_premultipliedBlendState = nullptr;

    /** Factor de mezcla suministrado a la etapa Output Merger. */
    float m_blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

    /** Información constante compartida por todo el frame. */
    CBPerFrame m_cbPerFrame{};

    /** Información constante asociada al objeto actual. */
    CBPerObject m_cbPerObject{};

    /** Información constante asociada al material actual. */
    CBPerMaterial m_cbPerMaterial{};

    /** Colección de objetos clasificados como opacos. */
    std::vector<const RenderObject> m_opaqueQueue;

    /** Colección de objetos clasificados como transparentes. */
    std::vector<const RenderObject> m_transparentQueue;
};