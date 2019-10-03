#pragma once
#include "cBuffer.h"

class cIndexBuffer : public cBuffer 
{
public:
  cIndexBuffer();

  ~cIndexBuffer();
public:

  void //! set the buffer 
    setDescription(uint32_t singleElementSize,
                   uint32_t TotalElements,
                   uint32_t  cpuAccess,
                   uint32_t miscFlags = 0,
                   uint32_t structured = 0) override;

  //! set a pointer to the vertex data  
  void
    setData(void* ptr);

#if DIRECTX
  //! for interfacing with directX 
  D3D11_BUFFER_DESC
    getDirectXDesc() override;
  //! for interfacing with directX 
  D3D11_SUBRESOURCE_DATA
    getSubresource();
#else 
#endif // DIRECTX
private:

#if DIRECTX
  D3D11_SUBRESOURCE_DATA m_data;
#else
#endif // DIRECTX
};

