
#include "../include/cTexture2D.h"
#include <memory>
cTexture2D::cTexture2D()
#if DIRECTX
  :mptr_texture(nullptr)
#endif // DIRECTX
{
  std::memset(&m_desc, 0, sizeof(m_desc));
}


cTexture2D::~cTexture2D()
{

#if DIRECTX
  if (mptr_texture != nullptr) 
  { mptr_texture->Release(); }
  
#endif // DIRECTX
}
#if DIRECTX

ID3D11Texture2D
* cTexture2D::getTexture()
{
  return mptr_texture;
}

ID3D11Texture2D
** cTexture2D::getTextureRef()
{
  return &mptr_texture;
}

ID3D11ShaderResourceView 
* cTexture2D::getResource()
{
  return m_resourceView.getShaderResource();
}

ID3D11ShaderResourceView
** cTexture2D::getResourceRef()
{
  return m_resourceView.getShaderResourceRef();
}

#endif // DIRECTX

void 
cTexture2D::setDescriptor(float width, float height, int format, int usage, int bindFlags, int CpuAccess, int arraySize)
{
#if DIRECTX
  m_desc.texWidth = width; m_desc.texHeight = height;
  m_desc.texFormat = format; m_desc.Usage = usage;
  m_desc.BindFlags = bindFlags; m_desc.CpuAccess;
  m_desc.arraySize = arraySize;
#endif // DIRECTX
}

sTextureDescriptor 
cTexture2D::getDescriptor()
{
  return  m_desc;
}

void cTexture2D::Release()
{
#if DIRECTX
  mptr_texture->Release();
  mptr_texture = nullptr;
#endif // DIRECTX
}

