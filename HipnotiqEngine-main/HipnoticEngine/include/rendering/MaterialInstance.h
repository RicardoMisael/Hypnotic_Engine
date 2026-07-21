/**
 * @file MaterialInstance.h
 * @brief Define la clase MaterialInstance utilizada para personalizar materiales por objeto.
 * @ingroup rendering
 */
#pragma once

#include "Prerequisites.h"
#include "Rendering/RenderTypes.h"

class Material;
class DeviceContext;
class Texture;

/**
 * @class MaterialInstance
 * @brief Representa una instancia configurable de un Material.
 *
 * Una instancia de material permite reutilizar un mismo material base
 * compartiendo shaders y estados de renderizado, mientras mantiene
 * recursos de textura y parámetros PBR específicos para cada objeto.
 *
 * Convención de slots de textura utilizados por el Pixel Shader:
 * - t0 : Albedo
 * - t1 : Normal
 * - t2 : Metallic
 * - t3 : Roughness
 * - t4 : Ambient Occlusion
 * - t5 : Emissive
 * - t6 : Shadow Map (asignado durante el renderizado)
 */
class MaterialInstance {
public:

    /**
     * @brief Establece el material base asociado a la instancia.
     *
     * @param material Material compartido por la instancia.
     */
    void setMaterial(Material* material) { m_material = material; }

    /**
     * @brief Establece la textura de color base.
     *
     * @param texture Textura de albedo.
     */
    void setAlbedo(Texture* texture) { m_albedo = texture; }

    /**
     * @brief Establece la textura de normales.
     *
     * @param texture Normal map asociado.
     */
    void setNormal(Texture* texture) { m_normal = texture; }

    /**
     * @brief Establece la textura de metalicidad.
     *
     * @param texture Mapa de metallic.
     */
    void setMetallic(Texture* texture) { m_metallic = texture; }

    /**
     * @brief Establece la textura de rugosidad.
     *
     * @param texture Mapa de roughness.
     */
    void setRoughness(Texture* texture) { m_roughness = texture; }

    /**
     * @brief Establece la textura de oclusión ambiental.
     *
     * @param texture Mapa de ambient occlusion.
     */
    void setAO(Texture* texture) { m_ao = texture; }

    /**
     * @brief Establece la textura emisiva.
     *
     * @param texture Mapa emissive.
     */
    void setEmissive(Texture* texture) { m_emissive = texture; }

    /**
     * @brief Obtiene el material base asociado.
     *
     * @return Material utilizado por la instancia.
     */
    Material* getMaterial() const { return m_material; }

    /**
     * @brief Obtiene la textura de color base.
     *
     * @return Textura de albedo.
     */
    Texture* getAlbedo() const { return m_albedo; }

    /**
     * @brief Obtiene la textura de normales.
     *
     * @return Normal map configurado.
     */
    Texture* getNormal() const { return m_normal; }

    /**
     * @brief Obtiene la textura de metalicidad.
     *
     * @return Mapa de metallic.
     */
    Texture* getMetallic() const { return m_metallic; }

    /**
     * @brief Obtiene la textura de rugosidad.
     *
     * @return Mapa de roughness.
     */
    Texture* getRoughness() const { return m_roughness; }

    /**
     * @brief Obtiene la textura de oclusión ambiental.
     *
     * @return Mapa AO.
     */
    Texture* getAO() const { return m_ao; }

    /**
     * @brief Obtiene la textura emisiva.
     *
     * @return Mapa emissive.
     */
    Texture* getEmissive() const { return m_emissive; }

    /**
     * @brief Accede a los parámetros PBR de la instancia.
     *
     * @return Referencia modificable a los parámetros del material.
     */
    MaterialParams& getParams() { return m_params; }

    /**
     * @brief Accede a los parámetros PBR en modo lectura.
     *
     * @return Referencia constante a los parámetros del material.
     */
    const MaterialParams& getParams() const { return m_params; }

    /**
     * @brief Enlaza las texturas configuradas a la etapa Pixel Shader.
     *
     * Cada textura válida se vincula al slot correspondiente según la
     * convención definida por el motor de renderizado.
     *
     * @param deviceContext Contexto gráfico utilizado para realizar el enlace.
     */
    void bindTextures(DeviceContext& deviceContext) const;

private:

    /** Material base compartido entre múltiples instancias. */
    Material* m_material = nullptr;

    /** Textura de color base (Albedo). */
    Texture* m_albedo = nullptr;

    /** Textura de normales utilizada para iluminación. */
    Texture* m_normal = nullptr;

    /** Mapa de metalicidad. */
    Texture* m_metallic = nullptr;

    /** Mapa de rugosidad superficial. */
    Texture* m_roughness = nullptr;

    /** Mapa de oclusión ambiental. */
    Texture* m_ao = nullptr;

    /** Mapa de emisión luminosa. */
    Texture* m_emissive = nullptr;

    /** Parámetros PBR específicos de la instancia. */
    MaterialParams m_params;
};