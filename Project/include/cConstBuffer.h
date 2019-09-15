#pragma once
#include "include/cBuffer.h"
class cConstBuffer : public cBuffer
{
public:
  cConstBuffer();

  ~cConstBuffer();
public:
//! set up the const buffer 
  void
    setDescription(uint32_t singleElementSize,
                   uint32_t totalElements,
                   uint32_t cpuAccess,
                   uint32_t miscFlags = 0,
                   uint32_t structured = 0);

#if DIRECTX
  //!this is to interface with the directX API
  D3D11_BUFFER_DESC
    getDirectXDesc() override;

#endif // DIRECTX
private:

};
