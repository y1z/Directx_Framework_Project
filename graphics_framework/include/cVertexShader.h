#pragma once
#include "../include/utility/Grafics_libs.h"
#include "cShaderBase.h"
class cVertexShader : public cShaderBase
{
public:
  cVertexShader();

  ~cVertexShader();
#if DIRECTX
  //! for argument's that require 1 pointer
  ID3D11VertexShader*  getVertexShader();
  //! for argument's that require 2 pointers 
  ID3D11VertexShader**  getVertexShaderRef();

#endif // DIRECTX

private:
  ID3D11VertexShader *mptr_vertexShader;
};

