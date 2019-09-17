#pragma once 
#include "../include/utiliy/Grafics_libs.h"

class cShaderResourceView
{
public:
  cShaderResourceView();

  ~cShaderResourceView();
public:
#if DIRECTX
  //! get the shader Resource 
  ID3D11ShaderResourceView 
    *getShaderResource();
//! for function that ask for pointers to arrays 
  ID3D11ShaderResourceView 
    **getShaderResourceRef();

#endif // DIRECTX
private:
#if DIRECTX
  ID3D11ShaderResourceView *mptr_shaderResource;
#endif // DIRECTX
};
