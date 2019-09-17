#include "..\include\cVertexBuffer.h"
#include <memory>
cVertexBuffer::cVertexBuffer()
  :cBuffer()
{
  m_Type = BufferType::Vertex;

#if DIRECTX
  std::memset(&m_data, 0, sizeof(m_data));
#endif // DIRECTX
}

cVertexBuffer::~cVertexBuffer()
{
#if DIRECTX
  m_data.pSysMem = nullptr;
#endif // DIRECTX
}

void
cVertexBuffer::setDescription(uint32_t singleElementSize,
                              uint32_t TotalElements,
                              uint32_t cpuAccess,
                              uint32_t miscFlags,
                              uint32_t structured)
{
  m_elementCount = TotalElements;
  m_stride = singleElementSize;
  m_Desc.sizeOfBuffer = singleElementSize * TotalElements ;
  m_Desc.cpuAccess = cpuAccess;
  m_Desc.miscFlags = miscFlags;
  m_Desc.structured = structured;
  // equivalent to D3D11_BIND_VERTEX_BUFFER
  m_Desc.bindFlags = static_cast<uint32_t>(BufferType::Vertex);
  m_Desc.usage = 0;//equivalent to D3D11_USAGE_DEFAULT 
}

void
cVertexBuffer::setData(void * ptr)
{ m_data.pSysMem = ptr; }


#if DIRECTX

D3D11_BUFFER_DESC
cVertexBuffer::getDirectXDesc()
{
  D3D11_BUFFER_DESC Result;
  Result.BindFlags = m_Desc.bindFlags;
  Result.ByteWidth = m_Desc.sizeOfBuffer;
  Result.CPUAccessFlags = m_Desc.cpuAccess;
  Result.Usage = static_cast<D3D11_USAGE>(m_Desc.usage);
  Result.MiscFlags = m_Desc.miscFlags;
  Result.StructureByteStride = m_Desc.structured;
  return  Result;
}

D3D11_SUBRESOURCE_DATA
cVertexBuffer::getSubresource()
{
  return m_data;
}

#endif // DIRECTX

