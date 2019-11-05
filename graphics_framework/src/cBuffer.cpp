
#include "../include/cBuffer.h"
#include <memory>
cBuffer::cBuffer()
#if DIRECTX
  :mptr_buffer(nullptr) 
#elif   OPEN_GL
  :m_bufferID(0) 
#endif // DIRECTX
{
  m_stride = 0;
  m_elementCount = 0;
  std::memset(&m_Desc, 0, sizeof(m_Desc));
}

cBuffer::~cBuffer()
{
#if DIRECTX
  if (mptr_buffer)
  {
    mptr_buffer->Release();
  }
#elif OPEN_GL
  if (m_bufferID != 0) { glDeleteBuffers(1, &m_bufferID); }
#endif // DIRECTX
}

#if DIRECTX
ID3D11Buffer *
cBuffer::getBuffer()
{
  return mptr_buffer;
}

ID3D11Buffer **
cBuffer::getBufferRef()
{
  return &mptr_buffer;
}
#endif // DIRECTX

std::size_t 
cBuffer::getElementCount() const
{
  return m_elementCount;
}
#if OPEN_GL

uint32_t
cBuffer::getID() const
{
  return m_bufferID;
}

uint32_t *
cBuffer::getIDPtr()
{
  return &m_bufferID;
}

#endif // OPEN_GL

sBufferDesc
cBuffer::getDescriptor() const
{
  return  this->m_Desc;
}

enBufferType
cBuffer::getBufferType() const
{
  return m_Type;
}

std::size_t
cBuffer::getStride() const
{
  return m_stride;
}

uint32_t
cBuffer::getBufferSize() const
{
  return m_Desc.sizeOfBuffer;
}
