
#include "..//include/cMesh.h"
#include "..//include/cDevice.h"
#include "../include/utility/CustomStructs.h"

cMesh::cMesh()
 :m_topology(Topology::TriList)
{
  mptr_indexBuffer = new cIndexBuffer();
  mptr_vertexBuffer = new cVertexBuffer();
  mptr_Texture = std::make_shared<cTexture2D>();

  glm::vec4 DefaultPosition(0.1f, 0.1f, 0.1f, 1.0f);

  m_transform *= DefaultPosition;
#if DIRECTX
 //dx::XMVECTOR defualtPos = dx::XMVectorSet(0.1f, 0.1f, 0.1f, 1.0f);
 // m_transform.matrix = dx::XMMatrixTranslationFromVector(defualtPos);
#else
#endif // DIRECTX
}

cMesh::cMesh(cMesh && mesh)
  :mptr_indexBuffer(mesh.mptr_indexBuffer),
  mptr_vertexBuffer(mesh.mptr_vertexBuffer),
  m_topology(mesh.m_topology) 
{
  mesh.mptr_indexBuffer = nullptr;
  mesh.mptr_vertexBuffer = nullptr;
// transferring  ownership
  this->mptr_indiceData = std::move(mesh.mptr_indiceData);
  this->mptr_vertexData = std::move(mesh.mptr_vertexData);

  mptr_Texture = mesh.mptr_Texture;
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

cMesh & cMesh::operator=(cMesh && mesh)
{
  this->mptr_indexBuffer = mesh.mptr_indexBuffer;
  this->mptr_vertexBuffer = mesh.mptr_vertexBuffer;
  this->mptr_Texture = mesh.mptr_Texture;

// transferring  ownership
  this->mptr_indiceData = std::move(mesh.mptr_indiceData);
  this->mptr_vertexData = std::move(mesh.mptr_vertexData);

  mesh.mptr_indexBuffer = nullptr;
  mesh.mptr_vertexBuffer = nullptr;
  return *this;
}

void cMesh::initIndexBuffer(std::unique_ptr<std::vector<uint16>> & indeces)
{
  mptr_indiceData = std::move(indeces);
  mptr_indexBuffer->setDescription(sizeof(uint16),mptr_indiceData->size(),0);
  mptr_indexBuffer->setData( &mptr_indiceData->front());
}

void cMesh::initVertexBuffer(std::unique_ptr<std::vector<sVertexPosTex>> & vertexes)
{
  mptr_vertexData = std::move(vertexes);
  mptr_vertexBuffer->setDescription(sizeof(sVertexPosTex), mptr_vertexData->size(), 0);
  mptr_vertexBuffer->setData(&mptr_vertexData->front());
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

void cMesh::setTexture(const std::shared_ptr<cTexture2D> &newTexture)
{
  mptr_Texture = newTexture;
}

void cMesh::setTransform(const sMatrix4x4 & newTransform)
{
#if DIRECTX
//  m_transform = newTransform;
#else
#endif // DIRECTX

}

void cMesh::setTopology(Topology topology)
{
  m_topology = topology;
}

Topology 
cMesh::getTopology() const
{
  return  m_topology;
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

const std::vector<sVertexPosTex>* 
cMesh::getVertexData() const
{
  return mptr_vertexData.get();
}

const std::vector<uint16>* 
cMesh::getIndiceData() const
{
  return mptr_indiceData.get();
}

void cMesh::setTransform(glm::mat4 & transform)
{
  this->m_transform = transform;
}

glm::mat4 cMesh::getTransform() const
{
  return  this->m_transform;
}

#if DIRECTX
ID3D11ShaderResourceView *
cMesh::getResource()
{
  return mptr_Texture->getResource();
}

ID3D11ShaderResourceView **
cMesh::getResourceRef()
{
  return mptr_Texture->getResourceRef();
}

#endif // DIRECTX
