#pragma once
#include "cVertexBuffer.h"
#include "cIndexBuffer.h"
#include "../include/directx_structs.h"
#include <vector>

class cDevice;
class cDeviceContext;

class cMesh
{
public:
  cMesh();
  // I demand move construction
  cMesh(const cMesh &mesh) = delete;
  cMesh(cMesh &&mesh);
  ~cMesh();
public:// operators 
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
  cMesh& operator =(cMesh && mesh);
public: // functions 
  cVertexBuffer &getVertexBuffer();
  cIndexBuffer &getIndexBuffer();
private:
  cVertexBuffer *mptr_vertexBuffer;
  cIndexBuffer *mptr_indexBuffer;

};


