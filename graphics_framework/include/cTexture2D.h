#pragma once
#include "utility/enGraphics.h"
#include "../include/cShaderResourceView.h"
#include "utility/enDefs.h"
class cMesh;
/*! \brief takes care of the representation of textures */
struct sTextureDescriptor
{
  uint32 texWidth{ 0 };
  uint32 texHeight{ 0 };
  int texFormat{ 0 };//! used to know how to interpret the data 
  int Usage{ 0 };
  int BindFlags{ 0 };
  int CpuAccess{ 0 };
  // this is in case the texture comes in an array format 
  int arraySize{ 0 };
};

/*! manages 2d textures */
class cTexture2D
{
public:
  cTexture2D();
  ~cTexture2D();
public:
#if DIRECTX
  ID3D11Texture2D * getTexture();
  ID3D11Texture2D ** getTextureRef();

  ID3D11ShaderResourceView  *getResource();
  ID3D11ShaderResourceView  **getResourceRef();
#endif // DIRECTX

  void
    init(uint32 width, uint32 height,
         int format, int usage,
         int bindFlags, int CpuAccess = 0,
         int arraySize = 1);


  sTextureDescriptor
    getDescriptor();

  uint32
    getWidth() const;

  uint32
    getHeight() const;

#if OPEN_GL

  uint32
    getID() const;

  uint32*
    getIDPtr();

  void
    setID(uint32 newID);

#endif // OPEN_GL

  void
    Release();

private:
#if DIRECTX
  ID3D11Texture2D * mptr_texture;
#elif  OPEN_GL
  uint32 m_textureID;
#endif // DIRECTX
public://make private
  sTextureDescriptor m_desc;
private:
  cShaderResourceView m_resourceView;
};

