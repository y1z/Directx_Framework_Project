
#include "../include/cVertexShader.h"

cVertexShader::cVertexShader()
#if DIRECTX
  :mptr_vertexShader(nullptr)
#endif // DIRECTX 
{}

cVertexShader::~cVertexShader()
{
#if DIRECTX
  if (mptr_vertexShader != nullptr)
  {
    mptr_vertexShader->Release();
  }
#endif // DIRECTX
}

#if DIRECTX

ID3D11VertexShader * cVertexShader::getVertexShader()
{
  return mptr_vertexShader;
}

ID3D11VertexShader ** cVertexShader::getVertexShaderRef()
{
  return &mptr_vertexShader;
}

#endif // DIRECTX
