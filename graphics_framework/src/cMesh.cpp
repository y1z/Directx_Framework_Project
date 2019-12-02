
#include "..//include/cMesh.h"
#include "..//include/cDevice.h"
#include "../include/utility/CustomStructs.h"

cMesh::cMesh()
  :m_topology(enTopology::TriList)
{
  mptr_indexBuffer = new cIndexBuffer();
  mptr_vertexBuffer = new cVertexBuffer();
  mptr_Texture = std::make_shared<cTexture2D>();

  glm::vec4 DefaultPosition(0.1f, 0.1f, 0.1f, 1.0f);

  m_transform *= DefaultPosition;
}

cMesh::cMesh(cMesh && mesh)
  :mptr_indexBuffer(mesh.mptr_indexBuffer),
  mptr_vertexBuffer(mesh.mptr_vertexBuffer),
  m_topology(mesh.m_topology)
{
  mesh.mptr_indexBuffer = nullptr;
  mesh.mptr_vertexBuffer = nullptr;

  //transfer all data related with the vertex
  this->mptr_vertexDataContainer = std::move(mesh.mptr_vertexDataContainer);

  //transfer all data related with the indices 
  this->mptr_indexDataContainer = std::move(mesh.mptr_indexDataContainer);

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
  this->mptr_indexDataContainer = std::move(mesh.mptr_indexDataContainer);
  this->mptr_vertexDataContainer = std::move(mesh.mptr_vertexDataContainer);

  mesh.mptr_indexBuffer = nullptr;
  mesh.mptr_vertexBuffer = nullptr;
  return *this;
}

void cMesh::initIndexBuffer(std::unique_ptr<std::vector<uint16>> & indeces)
{
  mptr_indexDataContainer = std::move(indeces);
  mptr_indexBuffer->init(sizeof(uint16), mptr_indexDataContainer->size(), 0);
  mptr_indexBuffer->setData(mptr_indexDataContainer->data());
}

void cMesh::initVertexBuffer(std::unique_ptr<std::vector<VERTEX_T>> & vertexes)
{
  mptr_vertexDataContainer = std::move(vertexes);
  if ( mptr_vertexDataContainer->empty() == false)
  {
    uint32 singleElementSize = sizeof(mptr_vertexDataContainer->at(0));
    mptr_vertexBuffer->init(sizeof(VERTEX_T), mptr_vertexDataContainer->size(), 0);
  }

  mptr_vertexBuffer->setData(mptr_vertexDataContainer->data());
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
  m_transform = newTransform.matrix;
}

void cMesh::setTopology(enTopology topology)
{
  m_topology = topology;
}

enTopology
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

const std::vector< VERTEX_T>*
cMesh::getVertexData() const
{
  return mptr_vertexDataContainer.get();
}

const std::vector<uint16>*
cMesh::getIndiceData() const
{
  return mptr_indexDataContainer.get();
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
