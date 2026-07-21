/**
 * @file MaterialInstance.cpp
 * @brief Implementa la funcionalidad de MaterialInstance.
 * @ingroup rendering
 *
 * Este módulo administra el enlace de las texturas asociadas a una
 * instancia de material para su utilización durante el proceso
 * de renderizado.
 */

#include "Rendering/MaterialInstance.h"
#include "DeviceContext.h"
#include "Texture.h"

 /**
  * @brief Enlaza las texturas configuradas en la instancia de material.
  *
  * Antes de realizar nuevos enlaces, los slots utilizados por el flujo
  * PBR son limpiados para evitar referencias persistentes provenientes
  * de draw calls anteriores.
  *
  * Cada textura válida se enlaza al registro correspondiente del
  * Pixel Shader siguiendo la convención establecida por el renderer:
  *
  * - t0 : Albedo
  * - t1 : Normal
  * - t2 : Metallic
  * - t3 : Roughness
  * - t4 : Ambient Occlusion
  * - t5 : Emissive
  *
  * Los registros que no disponen de una textura asociada permanecen
  * enlazados a nullptr.
  *
  * @param deviceContext Contexto de renderizado utilizado para realizar
  *        los enlaces de recursos GPU.
  */
void
MaterialInstance::bindTextures(DeviceContext& deviceContext) const {
    ID3D11ShaderResourceView* nullTextures[6] = {
        nullptr, nullptr, nullptr,
        nullptr, nullptr, nullptr
    };

    deviceContext.PSSetShaderResources(0, 6, nullTextures);

    if (m_albedo) {
        m_albedo->render(deviceContext, 0, 1);
    }

    if (m_normal) {
        m_normal->render(deviceContext, 1, 1);
    }

    if (m_metallic) {
        m_metallic->render(deviceContext, 2, 1);
    }

    if (m_roughness) {
        m_roughness->render(deviceContext, 3, 1);
    }

    if (m_ao) {
        m_ao->render(deviceContext, 4, 1);
    }

    if (m_emissive) {
        m_emissive->render(deviceContext, 5, 1);
    }
}