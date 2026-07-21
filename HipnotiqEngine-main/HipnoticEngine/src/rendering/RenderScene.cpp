/**
 * @file RenderScene.cpp
 * @brief Implementa la funcionalidad de RenderScene.
 * @ingroup rendering
 *
 * Este módulo proporciona las operaciones necesarias para administrar
 * el contenido de una escena de renderizado durante la generación
 * de cada frame.
 */

#include "Rendering/RenderScene.h"

 /**
  * @brief Restablece el contenido de la escena de renderizado.
  *
  * Elimina todos los objetos registrados, descarta las luces activas
  * y libera la referencia al skybox asociado.
  *
  * Esta operación suele ejecutarse al comienzo de cada frame antes
  * de recopilar nuevamente los elementos visibles que serán procesados
  * por el renderer.
  */
void
RenderScene::clear() {
    opaqueObjects.clear();
    transparentObjects.clear();
    directionalLights.clear();
    skybox = nullptr;
}