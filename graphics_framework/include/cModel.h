#pragma once
#include"../include/cMesh.h"
#include "actor/baseComponent.h"
#include <vector>
#include <string>

class aiScene;
class aiNode;
class aiMesh;
class cDevice;
class cDeviceContext;
class cConstBuffer;

/*! take care of storing and drawing a 3d model on screen*/
class cModel : public baseComponent 
{
public:
  cModel();
  cModel(std::string_view  strView);
  ~cModel() = default;
public:
  //! this function it where we load the file from
  bool 
    LoadModelFromFile(cDevice &device);
  /*! this functions draws all the meshes to the back buffer */
  void
    DrawMeshes(cDeviceContext &deviceContext,std::vector<cConstBuffer *> &buffers);

  // material 
  void setModelPath(const std::string_view modelPath );
public:// baseComponent functions 
  //
  bool 
    isReady()const override;

  void 
    Init(cDevice &device, [[maybe_unused]]cDeviceContext &deviceContext)override;

  void 
    Draw(cDeviceContext &devContext, std::vector<cConstBuffer *>&buffers)override;

  void 
    Destroy() override;
private:

  //! traverses the tree to get the data from the meshes 
  void 
    TraversTree(const aiScene*scene,aiNode *node,cDevice &device,std::vector<std::string> &texturePaths);

  //! this where we keep 
  void
    ExtractMesh(const aiMesh *assimpMesh,cDevice &device, const aiScene*scene,std::vector<std::string> &texturePaths);
  //! makes sure that a path from a mesh is valid 
  void
    CheckTexturePaths(const aiScene *assimpScene, const aiMesh *assimpMesh,std::vector<std::string> &texturePaths);
  //! just extract a texture from a given path 
  void 
    ExtractTexture(const char *texturePath,cMesh &AfectedMesh ,cDevice &device);
private:
  std::vector<cMesh> m_meshes;
  std::string m_modelPath;
  std::string m_materialPath;
};

