#include "..//include/cMesh.h"
#include "..//include/cDevice.h"

cMesh::cMesh()
{
  mptr_indexBuffer = new cIndexBuffer();
  mptr_vertexBuffer = new cVertexBuffer();
}

cMesh::cMesh(cMesh && mesh)
  :mptr_indexBuffer(mesh.mptr_indexBuffer),
  mptr_vertexBuffer(mesh.mptr_vertexBuffer)
{
  mesh.mptr_indexBuffer = nullptr;
  mesh.mptr_vertexBuffer = nullptr;
}

cMesh::~cMesh()
{
  if (mptr_indexBuffer != nullptr)
  {
    delete mptr_indexBuffer;
  }
  if (mptr_vertexBuffer != nullptr)
  {
    delete mptr_vertexBuffer;
  }
}

void cMesh::initIndexBuffer(std::vector<WORD>& indeces)
{
  mptr_indexBuffer->setDescription(sizeof(WORD),indeces.size(),0);
  mptr_indexBuffer->setData( &indeces.front());
}

void cMesh::initVertexBuffer(std::vector<SimpleVertex>& vertexes)
{
  mptr_vertexBuffer->setDescription(sizeof(SimpleVertex), vertexes.size(), 0);
  mptr_vertexBuffer->setData(&vertexes.front());
}

bool cMesh::createVertexBuffer(cDevice & device)
{
  bool isSuccessful = device.CreateVertexBuffer(*mptr_vertexBuffer);
  return isSuccessful;
}

bool cMesh::createIndexBuffer(cDevice & device)
{
  bool isSuccessful = device.CreateIndexBuffer(*mptr_indexBuffer);
  return isSuccessful;
}

cMesh & cMesh::operator=(cMesh && mesh)
{
  this->mptr_indexBuffer = mesh.mptr_indexBuffer;
  this->mptr_vertexBuffer = mesh.mptr_vertexBuffer;

  mesh.mptr_indexBuffer = nullptr;
  mesh.mptr_vertexBuffer = nullptr;
  return *this;
}

cVertexBuffer &
cMesh::getVertexBuffer()
{
  return *this->mptr_vertexBuffer;
}

cIndexBuffer &
cMesh::getIndexBuffer()
{
  return  *this->mptr_indexBuffer;
}
