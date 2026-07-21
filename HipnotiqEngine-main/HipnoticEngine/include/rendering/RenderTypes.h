/**
 * @file RenderTypes.h
 * @brief Define los tipos fundamentales, enumeraciones y estructuras compartidas por el sistema de renderizado.
 * @ingroup rendering
 */
#pragma once

#include "Prerequisites.h"

class Mesh;
class MaterialInstance;

/**
 * @enum MaterialDomain
 * @brief Clasifica el comportamiento de renderizado de un material dentro del pipeline.
 */
enum class MaterialDomain {

    /** Geometría completamente opaca. */
    Opaque = 0,

    /** Geometría con descarte por alfa (alpha test). */
    Masked,

    /** Geometría transparente con blending. */
    Transparent
};

/**
 * @enum BlendMode
 * @brief Modos de composición de color soportados por el renderer.
 */
enum class BlendMode {

    /** Sin mezcla; sobrescribe el color destino. */
    Opaque = 0,

    /** Mezcla alfa tradicional. */
    Alpha,

    /** Mezcla aditiva. */
    Additive,

    /** Mezcla con alfa premultiplicado. */
    PremultipliedAlpha
};

/**
 * @enum RenderPassType
 * @brief Identifica los distintos pases ejecutados por el pipeline de renderizado.
 */
enum class RenderPassType {

    /** Pase de generación de sombras. */
    Shadow = 0,

    /** Pase principal de geometría opaca. */
    Opaque,

    /** Pase de renderizado del skybox. */
    Skybox,

    /** Pase de geometría transparente. */
    Transparent,

    /** Pase de visualización para herramientas de editor. */
    Editor
};

/**
 * @enum LightType
 * @brief Tipos de fuentes de iluminación soportadas por el sistema.
 */
enum class LightType {

    /** Luz direccional infinita. */
    Directional = 0,

    /** Luz puntual omnidireccional. */
    Point,

    /** Luz tipo reflector (spotlight). */
    Spot
};

/**
 * @struct LightData
 * @brief Describe las propiedades de una fuente de luz utilizada durante el renderizado.
 *
 * Contiene los parámetros necesarios para evaluar la contribución
 * lumínica de una luz dentro de los shaders.
 */
struct LightData {

    /** Tipo de luz. */
    LightType type = LightType::Directional;

    /** Color emitido por la luz. */
    EU::Vector3 color = EU::Vector3(1.0f, 1.0f, 1.0f);

    /** Intensidad luminosa. */
    float intensity = 1.0f;

    /** Dirección de emisión para luces direccionales y spot. */
    EU::Vector3 direction = EU::Vector3(0.0f, -1.0f, 0.0f);

    /** Alcance máximo para luces puntuales y spot. */
    float range = 0.0f;

    /** Posición de la fuente de luz. */
    EU::Vector3 position = EU::Vector3(0.0f, 0.0f, 0.0f);

    /** Ángulo de apertura para luces tipo spot. */
    float spotAngle = 0.0f;
};

/**
 * @struct MaterialParams
 * @brief Conjunto de parámetros PBR configurables por instancia de material.
 *
 * Estos valores complementan la información proveniente de las texturas
 * y son enviados a GPU durante el proceso de renderizado.
 */
struct MaterialParams {

    /** Color base del material. */
    XMFLOAT4 baseColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

    /** Nivel de metalicidad de la superficie. */
    float metallic = 1.0f;

    /** Rugosidad de la superficie. */
    float roughness = 1.0f;

    /** Intensidad de oclusión ambiental. */
    float ao = 1.0f;

    /** Escala aplicada al normal map. */
    float normalScale = 1.0f;

    /** Intensidad de emisión luminosa. */
    float emissiveStrength = 1.0f;

    /** Umbral de descarte alfa para materiales Masked. */
    float alphaCutoff = 0.5f;
};

/**
 * @struct CBPerFrame
 * @brief Constant Buffer actualizado una vez por frame.
 *
 * Almacena información global compartida por todos los objetos
 * procesados durante la generación de una imagen.
 */
struct CBPerFrame {

    /** Matriz de vista. */
    XMFLOAT4X4 View{};

    /** Matriz de proyección. */
    XMFLOAT4X4 Projection{};

    /** Posición de la cámara activa. */
    EU::Vector3 CameraPos{};

    float pad0 = 0.0f;

    /** Dirección de la fuente de luz principal. */
    EU::Vector3 LightDir = EU::Vector3(0.0f, -1.0f, 0.0f);

    float pad1 = 0.0f;

    /** Color de la fuente de luz principal. */
    EU::Vector3 LightColor = EU::Vector3(1.0f, 1.0f, 1.0f);

    float pad2 = 0.0f;
};

/**
 * @struct CBPerObject
 * @brief Constant Buffer actualizado para cada instancia renderizada.
 *
 * Contiene la información de transformación requerida por los shaders
 * para procesar un objeto individual.
 */
struct CBPerObject {

    /** Transformación global del objeto en espacio de mundo. */
    XMFLOAT4X4 World{};
};

/**
 * @struct CBPerMaterial
 * @brief Constant Buffer actualizado para cada material activo.
 *
 * Agrupa los parámetros de sombreado enviados al shader durante
 * el renderizado de una superficie.
 */
struct CBPerMaterial {

    /** Color base del material. */
    XMFLOAT4 BaseColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

    /** Valor de metalicidad. */
    float Metallic = 1.0f;

    /** Valor de rugosidad. */
    float Roughness = 1.0f;

    /** Intensidad de oclusión ambiental. */
    float AO = 1.0f;

    /** Escala aplicada al normal map. */
    float NormalScale = 1.0f;

    /** Intensidad de emisión luminosa. */
    float EmissiveStrength = 1.0f;

    /** Umbral utilizado para descarte alfa. */
    float AlphaCutoff = 0.0f;

    /** Padding utilizado para cumplir los requisitos de alineación de GPU. */
    float pad0 = 0.0f;
    float pad1 = 0.0f;
    float pad2 = 0.0f;
    float pad3 = 0.0f;
    float pad4 = 0.0f;
    float pad5 = 0.0f;
};

/**
 * @struct RenderObject
 * @brief Describe una instancia de geometría procesable por el renderer.
 *
 * Agrupa la malla, los materiales y la información de transformación
 * necesaria para que el pipeline ejecute los distintos pases de renderizado.
 */
struct RenderObject {

    /** Malla asociada al objeto. */
    Mesh* mesh = nullptr;

    /** Instancia de material principal. */
    MaterialInstance* materialInstance = nullptr;

    /** Instancias de material utilizadas por cada Submesh. */
    std::vector<MaterialInstance*> materialInstances;

    /** Transformación global del objeto en el espacio de mundo. */
    XMMATRIX world = XMMatrixIdentity();

    /** Indica si el objeto participa en la generación de sombras. */
    bool castShadow = true;

    /** Indica si requiere renderizado transparente. */
    bool transparent = false;

    /** Distancia a la cámara utilizada para ordenamiento de renderizado. */
    float distanceToCamera = 0.0f;
};