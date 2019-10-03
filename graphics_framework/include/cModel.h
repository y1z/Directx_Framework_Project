#pragma once
#include"../include/cMesh.h"
#include <vector>

class aiScene;
class aiNode;
class aiMesh;
class cDevice;
class cDeviceContext;
class cConstBuffer;

/*! take care of storing and drawing a 3d model on screen*/
class cModel
{
public:
  cModel() = default;
  ~cModel() = default;
public:
//! this function it where we load the file from
  bool 
    LoadModelFromFile(const char* filePath,cDevice &device, const char *HardCodedPath = nullptr);
/*! this functions draws all the meshes to the back buffer */
  void
    DrawMeshes(cDeviceContext &deviceContext,std::vector<cConstBuffer *> &buffers);
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
};

