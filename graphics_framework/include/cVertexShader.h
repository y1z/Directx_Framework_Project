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
#elif OPEN_GL 
#endif // DIRECTX
private:
#if DIRECTX
  ID3D11VertexShader *mptr_vertexShader;
#elif OPEN_GL
#endif // DIRECTX

};

