
#include "..\include\cVertexBuffer.h"
#include <memory>
cVertexBuffer::cVertexBuffer()
  :cBuffer()
{
  m_Type = enBufferType::Vertex;

#if DIRECTX
  std::memset(&m_desc, 0, sizeof(m_desc));
#else 

#endif // DIRECTX
}

cVertexBuffer::~cVertexBuffer()
{
#if DIRECTX
  m_desc.pSysMem = nullptr;
#endif // DIRECTX
}

void
cVertexBuffer::init(uint32_t singleElementSize,
                              uint32_t TotalElements,
                              uint32_t cpuAccess,
                              uint32_t miscFlags,
                              uint32_t structured)
{
  m_elementCount = TotalElements;
  m_stride = singleElementSize;
  m_Desc.sizeOfBuffer = singleElementSize * TotalElements;
  m_Desc.cpuAccess = cpuAccess;
  m_Desc.miscFlags = miscFlags;
  m_Desc.structured = structured;
  // equivalent to D3D11_BIND_VERTEX_BUFFER
  m_Desc.bindFlags = static_cast< uint32_t >(enBufferType::Vertex);
  m_Desc.usage = 0;//equivalent to D3D11_USAGE_DEFAULT 
#if OPEN_GL
  glGenBuffers(1, &this->m_bufferID);
  glBindBuffer(GL_ARRAY_BUFFER, m_bufferID);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
#endif // OPEN_GL

}

void
cVertexBuffer::setData(void * ptrToData)
{
#if DIRECTX
  m_desc.pSysMem = ptrToData;
#elif OPEN_GL
  m_Desc.ptr_data = ptrToData;
#endif // DIRECTX
}

const void *
cVertexBuffer::getData() const
{
#if DIRECTX
  return m_desc.pSysMem;
#else 
  return m_Desc.ptr_data;
#endif // DIRECTX
}


#if DIRECTX

D3D11_BUFFER_DESC
cVertexBuffer::getDirectXDesc()
{
  D3D11_BUFFER_DESC Result;
  Result.BindFlags = m_Desc.bindFlags;
  Result.ByteWidth = m_Desc.sizeOfBuffer;
  Result.CPUAccessFlags = m_Desc.cpuAccess;
  Result.Usage = static_cast< D3D11_USAGE >(m_Desc.usage);
  Result.MiscFlags = m_Desc.miscFlags;
  Result.StructureByteStride = m_Desc.structured;
  return  Result;
}

D3D11_SUBRESOURCE_DATA
cVertexBuffer::getSubresource()
{
  return m_desc;
}

#endif // DIRECTX

