#pragma once
#include "cBuffer.h"
/**
 *@brief This class contains the indices for a 
 *3D models.
*/

class cIndexBuffer : public cBuffer
{
public:
  cIndexBuffer();

  ~cIndexBuffer();
public:

  void //! set the buffer 
    init(uint32_t singleElementSize,
                   uint32_t TotalElements,
                   uint32_t  cpuAccess,
                   uint32_t miscFlags = 0,
                   uint32_t structured = 0) override;

  //! set a pointer to the vertex data  
  void
    setData(void* Data);

  const void *
    getData() const;

#if DIRECTX
  //! for interfacing with directX 
  D3D11_BUFFER_DESC
    getDirectXDesc() override;
  //! for interfacing with directX 
  D3D11_SUBRESOURCE_DATA
    getSubresource();
#elif OPEN_GL 
#endif // DIRECTX
private:

#if DIRECTX
  D3D11_SUBRESOURCE_DATA m_desc;
#elif OPEN_GL
  const void* mptr_data = nullptr;
#endif // DIRECTX
};

