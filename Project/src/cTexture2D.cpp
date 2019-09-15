#include "include/cTexture2D.h"

cTexture2D::cTexture2D()
#if DIRECTX
  :mptr_texture(nullptr)
#endif // DIRECTX
{}


cTexture2D::~cTexture2D()
{
#if DIRECTX
  if (mptr_texture != nullptr) 
  { mptr_texture->Release(); }
  
#endif // DIRECTX
}
#if DIRECTX

ID3D11Texture2D * cTexture2D::getTexture()
{
  return mptr_texture;
}

ID3D11Texture2D ** cTexture2D::getTextureRef()
{
  return &mptr_texture;
}

#endif // DIRECTX
