#include "../include/cPixelShader.h"

cPixelShader::cPixelShader()
#if DIRECTX
  :mptr_pixelShader(nullptr)
#endif // DIRECTX
{
  m_shaderType = enShaderTypes::pixel;
}

cPixelShader::cPixelShader(cPixelShader && other)
  :cShaderBase(std::forward< cPixelShader >(other))
{
#if DIRECTX
  this->mptr_pixelShader = other.mptr_pixelShader;
  other.mptr_pixelShader = nullptr;
#endif // DIRECTX
}

cPixelShader::~cPixelShader()
{
#if DIRECTX
  if (mptr_pixelShader)
  {
    mptr_pixelShader->Release();
    mptr_pixelShader = nullptr;
  }

#endif // DIRECTX
}

#if DIRECTX
ID3D11PixelShader *
cPixelShader::getPixelShader()
{
  return mptr_pixelShader;
}

ID3D11PixelShader **
cPixelShader::getPixelShaderRef()
{
  return &mptr_pixelShader;
}

#endif // DIRECTX
