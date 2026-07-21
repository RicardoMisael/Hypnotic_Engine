/**
 * @file Mesh.h
 * @brief Define las estructuras Mesh y Submesh utilizadas para representar geometría renderizable.
 * @ingroup rendering
 */
#pragma once

#include "Prerequisites.h"
#include "Buffer.h"
#include "MeshComponent.h"

class Device;

/**
 * @struct Submesh
 * @brief Unidad de geometría renderizable con buffers y material propios.
 *
 * Un Mesh puede estar compuesto por múltiples submallas, permitiendo
 * dividir un modelo en secciones independientes con diferentes materiales
 * sin duplicar la información geométrica original.
 */
struct Submesh {

    /** Vertex Buffer que almacena los vértices de la submalla en GPU. */
    Buffer vertexBuffer;

    /** Index Buffer que define la conectividad de la geometría. */
    Buffer indexBuffer;

    /** Número total de índices utilizados para el renderizado. */
    unsigned int indexCount = 0;

    /** Índice inicial dentro del Index Buffer. */
    unsigned int startIndex = 0;

    /** Identificador del slot de material asociado. */
    unsigned int materialSlot = 0;
};

/**
 * @class Mesh
 * @brief Representa una colección de submallas listas para renderizado.
 *
 * Un Mesh encapsula la geometría de un modelo mediante una colección
 * de Submesh, cada una con sus propios buffers de GPU y asociación de
 * materiales. Esta estructura constituye la representación utilizada
 * por el pipeline de renderizado.
 */
class Mesh {
public:

    /**
     * @brief Obtiene la colección de submallas.
     *
     * @return Referencia modificable a las submallas del Mesh.
     */
    std::vector<Submesh>& getSubmeshes() { return m_submeshes; }

    /**
     * @brief Obtiene la colección de submallas en modo lectura.
     *
     * @return Referencia constante a las submallas del Mesh.
     */
    const std::vector<Submesh>& getSubmeshes() const { return m_submeshes; }

    /**
     * @brief Construye un Mesh a partir de datos geométricos cargados en CPU.
     *
     * Cada MeshComponent se convierte en una Submesh independiente,
     * generando los recursos de GPU necesarios para su renderizado.
     *
     * Los slots de material se asignan siguiendo el orden de los
     * componentes proporcionados durante la construcción.
     *
     * Si la creación de una submalla falla, ésta es descartada sin
     * interrumpir el procesamiento del resto de componentes.
     *
     * @param device Dispositivo gráfico utilizado para crear los buffers.
     * @param components Componentes geométricos de entrada.
     * @return Mesh preparado para ser utilizado por el renderer.
     */
    static Mesh buildFrom(
        Device& device,
        const std::vector<MeshComponent>& components);

    /**
     * @brief Libera los recursos gráficos asociados al Mesh.
     *
     * Destruye los buffers de cada submalla y elimina todas las
     * referencias internas a la geometría almacenada.
     */
    void destroy() {
        for (Submesh& sm : m_submeshes) {
            sm.vertexBuffer.destroy();
            sm.indexBuffer.destroy();
        }
        m_submeshes.clear();
    }

private:

    /** Colección de submallas que conforman la geometría del Mesh. */
    std::vector<Submesh> m_submeshes;
};