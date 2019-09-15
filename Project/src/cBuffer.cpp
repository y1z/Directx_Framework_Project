#include "../include/cBuffer.h"
#include <memory>
cBuffer::cBuffer()
#if DIRECTX
  :mptr_buffer(nullptr) ,
#endif // DIRECTX
  m_stride(0)
{
  std::memset(&m_Desc, 0, sizeof(m_Desc));
}

cBuffer::~cBuffer()
{
#if DIRECTX
  if (mptr_buffer)
  {
    mptr_buffer->Release();
  }

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

sBufferDesc
cBuffer::getDescriptor() const
{
  return  this->m_Desc;
}

BufferType
cBuffer::getBufferType() const
{
  return m_Type;
}

std::size_t 
cBuffer::getStride() const
{
  return m_stride;
}
