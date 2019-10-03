 #include "..\include\cConstBuffer.h"


cConstBuffer::cConstBuffer()
  :cBuffer()
{
  m_Type = BufferType::Const;
}

cConstBuffer::~cConstBuffer()
{}

void 
cConstBuffer::setDescription(uint32_t singleElementSize,
                             uint32_t totalElements,
                             uint32_t cpuAccess,
                             uint32_t miscFlags,
                             uint32_t structured)
{
  m_stride = singleElementSize;
  m_elementCount = totalElements;
  m_Desc.sizeOfBuffer = singleElementSize * totalElements;
  m_Desc.cpuAccess = cpuAccess;
  m_Desc.miscFlags = miscFlags;
  m_Desc.structured = structured;
  // equivalent to D3D11_BIND_CONSTANT_BUFFER
  m_Desc.bindFlags = static_cast<uint32_t>(BufferType::Const);
  m_Desc.usage = 0;//equivalent to D3D11_USAGE_DEFAULT 
}

#if DIRECTX
D3D11_BUFFER_DESC
cConstBuffer::getDirectXDesc()
{
  D3D11_BUFFER_DESC  Result;
  Result.ByteWidth = m_Desc.sizeOfBuffer;
  Result.BindFlags = m_Desc.bindFlags;
  Result.MiscFlags = m_Desc.miscFlags;
  Result.CPUAccessFlags = m_Desc.cpuAccess;
  Result.Usage = static_cast<D3D11_USAGE>(m_Desc.usage);
  Result.StructureByteStride = m_Desc.structured;
  return Result;
}
#endif // DIRECTX
