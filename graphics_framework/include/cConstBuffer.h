#pragma once
#include "../include/cBuffer.h"

/**
*@brief a buffer that contains the information that the G.P.U will read
*
*
* 
*/

class cConstBuffer : public cBuffer
{
public:
  cConstBuffer();

  ~cConstBuffer();
public:
  //! set up the const buffer 
  void
    init(uint32_t singleElementSize,
                   uint32_t totalElements,
                   uint32_t cpuAccess,
                   uint32_t miscFlags = 0,
                   uint32_t structured = 0) override;

  /*! sets the index that's going
  to be used to identify which 
  buffer is going to used.*/
  void
    setIndex(const uint32_t newIndex);


  uint32_t
    getIndex()const;

#if DIRECTX
  //!this is to interface with the directX API
  D3D11_BUFFER_DESC
    getDirectXDesc() override;
#elif OPEN_GL
  
  uint32_t
    getGlUniformBlockID() const;

  uint32_t *
    getGlUniformBlockIDPtr();

  void
    setGlUniformBlockID(const uint32_t uniformBlockID);

#endif // DIRECTX
private:
  uint32_t m_bufferIndexID;
  /*! intended for usage with uniform block 
  * https://hub.packtpub.com/opengl-40-using-uniform-blocks-and-uniform-buffer-objects/
  */
#if OPEN_GL
  uint32_t m_glUniformBlockID;
#endif // OPEN_GL
};
