
#include "../include/cIndexBuffer.h"
#include <memory>
cIndexBuffer::cIndexBuffer()
  :cBuffer()
{
  std::memset(&m_data, 0, sizeof(m_data));
  m_Type = BufferType::Index;
}

cIndexBuffer::~cIndexBuffer()
{
}

void
cIndexBuffer::setDescription(uint32_t singleElementSize,
                             uint32_t TotalElements,
                             uint32_t cpuAccess,
                             uint32_t miscFlags,
                             uint32_t structured)
{
  m_stride = singleElementSize;
  m_elementCount = TotalElements;
  m_Desc.sizeOfBuffer = singleElementSize * TotalElements;
  m_Desc.bindFlags = static_cast<int>(m_Type);
  m_Desc.cpuAccess = cpuAccess;
  m_Desc.miscFlags = miscFlags;
  m_Desc.structured = structured;
  m_Desc.usage = 0;// equivalent to D3D11_USAGE_DEFAULT
}

void
cIndexBuffer::setData(void * ptr)
{
#if DIRECTX
  m_data.pSysMem = ptr;
#endif // DIRECTX
}

#if DIRECTX 
D3D11_BUFFER_DESC
cIndexBuffer::getDirectXDesc()
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
cIndexBuffer::getSubresource()
{
  return  m_data;
}
#endif // DIRECTX