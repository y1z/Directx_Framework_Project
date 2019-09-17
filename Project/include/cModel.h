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
    LoadModelFromFile(const char* filePath,cDevice &device);
/*! this functions draws all the meshes to the back buffer */
  void
    DrawMeshes(cDeviceContext &deviceContext,std::vector<cConstBuffer *> &buffers);
private:

  //! traverses the tree to get the data from the meshes 
  void 
    TraversTree(const aiScene*scene,aiNode *node,cDevice &device);

//! this where we keep 
  void
    ExtractMesh(const aiMesh *assimpMesh,cDevice &device);
private:
  std::vector<cMesh> m_meshes;
};

