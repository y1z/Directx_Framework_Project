#include "..\include\cConstBuffer.h"


cConstBuffer::cConstBuffer()
  :cBuffer()
{
  m_Type = BufferType::Const;
  m_bufferIndexID = 0u;
#if OPEN_GL
  m_glUniformBlockID = 0u;
#endif // OPEN_GL
}

cConstBuffer::~cConstBuffer()
{
#if OPEN_GL
  if (m_glUniformBlockID != 0) { glDeleteBuffers(1, &m_glUniformBlockID); }
#endif // OPEN_GL
}

void
cConstBuffer::init(uint32_t singleElementSize,
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
  m_Desc.bindFlags = static_cast< uint32_t >(BufferType::Const);
  m_Desc.usage = 0;//equivalent to D3D11_USAGE_DEFAULT 
#if OPEN_GL


#endif // OPEN_GL
}

void
cConstBuffer::setIndex(const uint32_t newIndex)
{
  m_bufferIndexID = newIndex;
}

uint32_t
cConstBuffer::getIndex() const
{
  return  this->m_bufferIndexID;
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
  Result.Usage = static_cast< D3D11_USAGE >(m_Desc.usage);
  Result.StructureByteStride = m_Desc.structured;
  return Result;
}
#elif OPEN_GL

uint32_t
cConstBuffer::getGlUniformBlockID() const
{
  return  m_glUniformBlockID;
}

uint32_t *
cConstBuffer::getGlUniformBlockIDPtr()
{
  return &m_glUniformBlockID;
}

void
cConstBuffer::setGlUniformBlockID(const uint32_t uniformBlockID)
{
  m_glUniformBlockID = uniformBlockID;
}
#endif // DIRECTX
