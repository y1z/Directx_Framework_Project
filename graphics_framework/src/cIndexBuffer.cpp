
#include "../include/cIndexBuffer.h"
#include <memory>
cIndexBuffer::cIndexBuffer()
  :cBuffer()
{
  m_Type = BufferType::Index;
#if OPEN_GL
  mptr_data = nullptr;
#endif // OPEN_GL
}

cIndexBuffer::~cIndexBuffer()
{}

void
cIndexBuffer::init(uint32_t singleElementSize,
                   uint32_t TotalElements,
                   uint32_t cpuAccess,
                   uint32_t miscFlags,
                   uint32_t structured)
{
  m_stride = singleElementSize;
  m_elementCount = TotalElements;
  m_Desc.sizeOfBuffer = singleElementSize * TotalElements;
  m_Desc.bindFlags = static_cast< int >(m_Type);
  m_Desc.cpuAccess = cpuAccess;
  m_Desc.miscFlags = miscFlags;
  m_Desc.structured = structured;
  m_Desc.usage = 0;// equivalent to D3D11_USAGE_DEFAULT
#if OPEN_GL

  glGenBuffers(1, &m_bufferID);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferID);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

#endif // OPEN_GL

}

void
cIndexBuffer::setData(void * dataSource)
{
#if DIRECTX
  m_desc.pSysMem = dataSource;
#elif OPEN_GL
  mptr_data = dataSource;
#endif // DIRECTX
}

const void *
cIndexBuffer::getData() const
{
#if DIRECTX
  return m_desc.pSysMem;
#else
  return  mptr_data;
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
  Result.Usage = static_cast< D3D11_USAGE >(m_Desc.usage);
  Result.MiscFlags = m_Desc.miscFlags;
  Result.StructureByteStride = m_Desc.structured;
  return  Result;
}

D3D11_SUBRESOURCE_DATA
cIndexBuffer::getSubresource()
{
  return  m_desc;
}
#endif // DIRECTX