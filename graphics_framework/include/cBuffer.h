#pragma once
#include "utility/Grafics_libs.h"
#include "enum_headers/enumBufferType.h"
#include <cstdint>
#include <cstddef>

//! this will represent the data my buffer has 
struct sBufferDesc
{
  uint32_t sizeOfBuffer{0};
  int usage{0};
  uint32_t bindFlags{0};
  uint32_t cpuAccess{0};
  uint32_t miscFlags{0};
  uint32_t structured;
};

/*! this is the base class for all buffer types */
class cBuffer
{
public:
  cBuffer();

  virtual ~cBuffer();
#if DIRECTX
  //! for functions that require a single pointer 
  ID3D11Buffer 
    *getBuffer();

  //! for functions that require a 2 pointers 
  ID3D11Buffer 
    **getBufferRef();
#endif // DIRECTX

public: // functions 
  //! for setting the values in the buffer 
  virtual void
    setDescription(uint32_t singleElementSize,
                   uint32_t totalElements,
                   uint32_t  cpuAccess,
                   uint32_t miscFlags = 0,
                   uint32_t structured = 0) = 0;

//! returns the number of elements in the buffer 
  std::size_t 
    getElementCount() const;

#if DIRECTX
  //! this is for interfacing with the directX api.
  virtual D3D11_BUFFER_DESC
    getDirectXDesc() = 0;

#endif // DIRECTX
  //!return the buffer descriptor 
  sBufferDesc
    getDescriptor() const;
  //!return the type the buffer is (vertex ,index , const)
  BufferType
    getBufferType() const;
  //! return the stride of the buffer 
  std::size_t
    getStride() const;
protected:
#if DIRECTX
  //! directX 11 buffer implementation
  ID3D11Buffer *mptr_buffer;
#endif 
  //! keeps track of how many element's a buffer contains
  std::size_t m_elementCount;
  //! the size of a single element in the buffer 
  std::size_t m_stride;
  //! to keep track of the stats of the buffer 
  sBufferDesc m_Desc;
  //! to know which type of buffer it being used (vertex, index ,const etc...)
  BufferType m_Type;
};

