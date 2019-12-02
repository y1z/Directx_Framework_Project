
#include "../include/cVertexShader.h"

cVertexShader::cVertexShader()
#if DIRECTX
  :mptr_vertexShader(nullptr)
#endif // DIRECTX 
{
  m_shaderType = enShaderTypes::vertex;
}

cVertexShader::cVertexShader(cVertexShader && other)
  :cShaderBase(std::forward< cVertexShader >(other))
{

#if DIRECTX
  this->mptr_vertexShader = other.mptr_vertexShader;
  other.mptr_vertexShader = nullptr;
#elif OPEN_GL

#endif // DIRECTX


}

cVertexShader::~cVertexShader()
{
#if DIRECTX
  if (mptr_vertexShader != nullptr)
  {
    mptr_vertexShader->Release();
    mptr_vertexShader = nullptr;
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
