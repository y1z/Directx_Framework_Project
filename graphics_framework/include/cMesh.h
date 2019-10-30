#pragma once
#include "cVertexBuffer.h"
#include "cIndexBuffer.h"
#include "cTexture2D.h"
#include "../include/directx_structs.h"
#include "utility/CustomStructs.h"
#include "enum_headers/enumTopology.h"
#include "utility/enDefs.h"
// std includes 
#include <vector>
#include <memory>

class cDevice;
class cDeviceContext;
struct sMatrix4x4;


class cMesh
{
public:
  cMesh();
  // I demand move construction
  cMesh(const cMesh &mesh) = delete;
  cMesh(cMesh &&mesh);
  ~cMesh();

public:// operators 
  cMesh& operator =(cMesh && mesh);

public:// functions 
  //! set up the index buffer for creation
  void
    initIndexBuffer(std::unique_ptr<std::vector<uint16>> & indeces);
  //! set up the vertex buffer for creation
  void
    initVertexBuffer(std::unique_ptr<std::vector<sVertexPosTex>> & vertexes);
  //! creates the vertex buffer 
  bool
    createVertexBuffer(cDevice &device);
  //! creates the index buffer 
  bool
    createIndexBuffer(cDevice &device);

  //! sets a texture to the current mesh
  void
    setTexture(const std::shared_ptr<cTexture2D> &newTexture);
  //! give a mesh a different transform
  void
    setTransform(const sMatrix4x4 & newTransform);
  //! sets the topology for the individual mesh 
  void
    setTopology(Topology topology);
  //! returns the type of topology the mesh contians 
  Topology
    getTopology() const;

public: // functions 
  cVertexBuffer &getVertexBuffer();
  cIndexBuffer &getIndexBuffer();
/*! returns a vector that contains all data related with vertexes of the mesh */
  const std::vector<sVertexPosTex>* getVertexData() const;
/*! returns a vector that contains all data related with indices of the mesh */
  const std::vector<uint16>* getIndiceData() const;

  void setTransform(glm::mat4 &transform);
  glm::mat4 getTransform()const;
#if DIRECTX

  ID3D11ShaderResourceView
    *getResource();

  ID3D11ShaderResourceView
    **getResourceRef();
#endif // DIRECTX
private:
  cVertexBuffer *mptr_vertexBuffer;

  cIndexBuffer *mptr_indexBuffer;
  //! this is so a mesh can share a texture with a another mesh 
  std::shared_ptr<cTexture2D> mptr_Texture;
  /*! this contains the values that consist of the vertex data */
  std::unique_ptr<std::vector<sVertexPosTex>> mptr_vertexData;
  /*! this contains the values that represent the indices */
  std::unique_ptr<std::vector<uint16>> mptr_indiceData;
  //! controls which topology it used be each mesh.
  Topology m_topology;
  glm::mat4 m_transform;
};


