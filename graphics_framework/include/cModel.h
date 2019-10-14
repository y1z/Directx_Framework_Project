#pragma once
#include"../include/cMesh.h"
#include "actor/baseComponent.h"
#include "glm/matrix.hpp"
#include "glm/vec4.hpp"
#include "utility/enDefs.h"
/*******************/
#include <vector>
#include <string>
#include <filesystem>// TODO : implement this library in this class 

struct aiScene;
struct aiNode;
struct aiMesh;
struct sColorf;
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
    DrawMeshes(cDeviceContext &deviceContext, std::vector<cConstBuffer *> &buffers, const sColorf &color = {0.0f,0.0f,0.7f,1.0f});

  //! this is to set the path to a  
  void 
    setModelPath(const std::string_view modelPath);

  //! this is to set a path to a material 
  void
    setMaterialPath(const std::string_view MaterialPath);

  /*! returns the total number of mesh the model has */
  std::size_t 
    getMeshCount() const;
  /*! returns the number of vertex the model is composed of. */
  std::size_t 
    getVertexCount()const;
  /*! returns a pointer to a that the model contains */
  [[nodiscard]]const cMesh*
    getMesh(std::size_t index) const ;

public:// baseComponent functions 
  //! make sure the component is ready 
  bool 
    isReady()const override;

  void 
    Init(cDevice &device, [[maybe_unused]]cDeviceContext &deviceContext)override;

  void 
    Draw(cDeviceContext &devContext, std::vector<cConstBuffer *>&buffers)override;

  void
    update(cDeviceContext &deviceContext, const sMatrix4x4 &Transform ) override;

  void 
    Destroy() override;

  void
    setTransform(glm::mat4 &matrix);

  glm::mat4
    getTransform()const;
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
  //! path to model (TODO: Replace with std::filesystem::path)
  std::string m_modelPath;
  //! paths to materials (TODO: Replace with std::filesystem::path)
  std::vector<std::string> m_materialPaths;
  //! this keeps track 
  sMatrix4x4 m_transform;
};

