
#include "../include/cShaderResourceView.h"

cShaderResourceView::cShaderResourceView()
#if DIRECTX
  :mptr_shaderResource(nullptr)
#endif // DIRECTX
{}

cShaderResourceView::~cShaderResourceView()
{
#if DIRECTX
  if (mptr_shaderResource)
  {
    mptr_shaderResource->Release();
  }
#endif // DIRECTX
}

#if DIRECTX
ID3D11ShaderResourceView *
cShaderResourceView::getShaderResource()
{
  return mptr_shaderResource;
}

ID3D11ShaderResourceView ** cShaderResourceView::getShaderResourceRef()
{
  return &mptr_shaderResource;
}
#endif // DIRECTX

