#pragma once
#include "cVertexBuffer.h"
#include "cIndexBuffer.h"
#include "cTexture2D.h"
#include "../include/directx_structs.h"
#include "utility/CustomStructs.h"
#include "enum_headers/enumTopology.h"
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
    initIndexBuffer(std::vector<WORD> &indeces);
  //! set up the vertex buffer for creation
  void
    initVertexBuffer(std::vector<SimpleVertex> &vertexes);
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
  //! controls which topology it used be each mesh.
  Topology m_topology;
#if DIRECTX
 sMatrix4x4 m_transform;
#endif // DIRECTX
};


