#pragma once
#include  "Prerequisites.h"
#include  "MeshComponent.h"
#include "fbxsdk.h"

struct LoadDataOBJ {
  std::string name;
  std::vector<SimpleVertex> vertex;
  std::vector<unsigned int> index;
  int numVertex = 0;
  int numIndex = 0;
};

class
ModelLoader {
public:
  /**
   * @brief Contructor y destructor
   */
  ModelLoader() = default;
  ~ModelLoader() = default;

  /**
   * @brief Crea el FBX Manager, configura las opciones de entrada/salida
   * @return true si la inicialización fue exitosa; false en caso contrario.
   */
  bool
  InitializeFBXManager();

  /**
   * @brief Realiza el parsing del archivo, carga los nodos, extrae la información de mallas
   * @param filePath Ruta completa al archivo .fbx que se desea cargar.
   * @return true si se cargó y procesó correctamente.
   */
  bool
  LoadFBXModel(const std::string& filePath);

  /**
   * @brief Recorre cada nodo del árbol jerárquico del archivo FBX
   * @param node Nodo raíz o intermedio a analizar.
   */
  void
  ProcessFBXNode(FbxNode* node);

  /**
   * @brief Extrae información de vértices, índices y coordenadas UV desde una malla.
   * @param node Nodo con malla dentro de la escena FBX.
   */
  void
  ProcessFBXMesh(FbxNode* node);

  /**
   * @brief  Procesa los materiales asociados a las mallas
   * @param material Material de la malla que contiene texturas.
   */
  void
  ProcessFBXMaterials(FbxSurfaceMaterial* material);

  /**
 * @brief Devuelve todos los nombres de archivos de texturas encontrados en el modelo FBX.
 * @param Vector con los nombres o rutas relativas de los archivos de textura.
 */
  std::vector<std::string>
  GetTextureFileNames() const { return textureFileNames; }

  /**
  * @brief Convierte el contenido del archivo OBJ en una estructura interna
  * @param filePath Ruta del archivo .obj.
  * @return true si el archivo fue cargado correctamente.
  */
  bool 
  LoadOBJ_model(const std::string& filePath);

  /**
   * @brief Loads an OBJ model and returns the data as a LoadDataOBJ structure.
   * @param objFileName Path to the .obj file to load.
   */
  LoadDataOBJ
  LoadOBJ(std::string objFileName);

private:
  FbxManager* lSdkManager;
  FbxScene* lScene;
  std::vector<std::string> textureFileNames;
public:
  std::vector<MeshComponent> meshes;
};