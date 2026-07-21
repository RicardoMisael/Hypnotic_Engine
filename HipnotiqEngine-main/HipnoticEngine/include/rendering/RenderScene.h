/**
 * @file RenderScene.h
 * @brief Define la clase RenderScene utilizada para almacenar los elementos visibles de una escena.
 * @ingroup rendering
 */
#pragma once

#include "Prerequisites.h"
#include "Rendering/RenderTypes.h"

class Skybox;

/**
 * @class RenderScene
 * @brief Contenedor de datos utilizado por el pipeline de renderizado.
 *
 * Esta estructura agrupa los objetos renderizables, las fuentes de luz
 * activas y los recursos de entorno necesarios para generar un frame.
 *
 * Su contenido se construye durante la fase de recopilación de escena y
 * posteriormente es consumido por el renderer para ejecutar los distintos
 * pases de renderizado.
 */
class RenderScene {
public:

    /**
     * @brief Restablece el contenido de la escena de renderizado.
     *
     * Elimina todos los objetos registrados, descarta las luces activas
     * y libera la referencia al skybox asociado.
     */
    void clear();

public:

    /**
     * @brief Colección de objetos clasificados como opacos.
     *
     * Estos objetos suelen procesarse primero para maximizar la eficacia
     * del depth buffer y reducir operaciones de overdraw.
     */
    std::vector<RenderObject> opaqueObjects;

    /**
     * @brief Colección de objetos clasificados como transparentes.
     *
     * Normalmente se renderizan después de la geometría opaca y pueden
     * requerir ordenamiento en función de la distancia a la cámara.
     */
    std::vector<RenderObject> transparentObjects;

    /**
     * @brief Conjunto de luces direccionales activas para el frame actual.
     */
    std::vector<LightData> directionalLights;

    /**
     * @brief Skybox utilizado como fondo y entorno de la escena.
     */
    Skybox* skybox = nullptr;
};