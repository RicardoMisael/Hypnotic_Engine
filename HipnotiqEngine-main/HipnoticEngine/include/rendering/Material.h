/**
 * @file Material.h
 * @brief Define la clase Material utilizada para encapsular la configuración de renderizado de una superficie.
 * @ingroup rendering
 */
#pragma once

#include "Prerequisites.h"
#include "Rendering/RenderTypes.h"

class ShaderProgram;
class RasterizerState;
class DepthStencilState;
class SamplerState;

/**
 * @class Material
 * @brief Encapsula los recursos y estados gráficos necesarios para renderizar una superficie.
 *
 * Un material describe cómo debe procesarse y visualizarse una superficie
 * durante el renderizado, agrupando programas de shaders, estados de GPU y
 * parámetros relacionados con la mezcla y clasificación de renderizado.
 */
class Material {
public:

    /**
     * @brief Establece el programa de shaders utilizado por el material.
     *
     * @param shader Programa de shaders asociado.
     */
    void setShader(ShaderProgram* shader) { m_shader = shader; }

    /**
     * @brief Establece el estado de rasterización.
     *
     * @param state Estado de rasterización a utilizar.
     */
    void setRasterizerState(RasterizerState* state) { m_rasterizerState = state; }

    /**
     * @brief Establece el estado de profundidad y stencil.
     *
     * @param state Estado Depth-Stencil asociado.
     */
    void setDepthStencilState(DepthStencilState* state) { m_depthStencilState = state; }

    /**
     * @brief Establece el estado de muestreo de texturas.
     *
     * @param state Estado de sampler asociado.
     */
    void setSamplerState(SamplerState* state) { m_samplerState = state; }

    /**
     * @brief Define el dominio de renderizado del material.
     *
     * @param domain Dominio al que pertenece el material.
     */
    void setDomain(MaterialDomain domain) { m_domain = domain; }

    /**
     * @brief Define el modo de mezcla empleado durante el renderizado.
     *
     * @param blendMode Configuración de blending.
     */
    void setBlendMode(BlendMode blendMode) { m_blendMode = blendMode; }

    /**
     * @brief Obtiene el programa de shaders asociado al material.
     *
     * @return Puntero al programa de shaders actual.
     */
    ShaderProgram* getShader() const { return m_shader; }

    /**
     * @brief Obtiene el estado de rasterización configurado.
     *
     * @return Estado de rasterización activo.
     */
    RasterizerState* getRasterizerState() const { return m_rasterizerState; }

    /**
     * @brief Obtiene el estado de profundidad y stencil configurado.
     *
     * @return Estado Depth-Stencil activo.
     */
    DepthStencilState* getDepthStencilState() const { return m_depthStencilState; }

    /**
     * @brief Obtiene el estado de muestreo utilizado por el material.
     *
     * @return Estado de sampler activo.
     */
    SamplerState* getSamplerState() const { return m_samplerState; }

    /**
     * @brief Obtiene el dominio de renderizado del material.
     *
     * @return Dominio actualmente configurado.
     */
    MaterialDomain getDomain() const { return m_domain; }

    /**
     * @brief Obtiene el modo de mezcla configurado.
     *
     * @return Modo de blending activo.
     */
    BlendMode getBlendMode() const { return m_blendMode; }

private:

    /** Programa de shaders responsable del procesamiento del material. */
    ShaderProgram* m_shader = nullptr;

    /** Estado de rasterización aplicado durante el renderizado. */
    RasterizerState* m_rasterizerState = nullptr;

    /** Estado de profundidad y stencil asociado al material. */
    DepthStencilState* m_depthStencilState = nullptr;

    /** Estado de muestreo utilizado para acceder a recursos de textura. */
    SamplerState* m_samplerState = nullptr;

    /** Dominio que determina el comportamiento general del material. */
    MaterialDomain m_domain = MaterialDomain::Opaque;

    /** Configuración de mezcla aplicada durante la composición final. */
    BlendMode m_blendMode = BlendMode::Opaque;
};