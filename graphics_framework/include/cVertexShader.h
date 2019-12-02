#pragma once
#include "utility/enGraphics.h"
#include "cShaderBase.h"


class cVertexShader : public cShaderBase
{
public:
  cVertexShader();

  cVertexShader(cVertexShader &&other);

  cVertexShader(const cVertexShader &other) = delete;

  ~cVertexShader();
public:
#if DIRECTX
  //! for argument's that require 1 pointer
  ID3D11VertexShader*  getVertexShader();
  //! for argument's that require 2 pointers 
  ID3D11VertexShader**  getVertexShaderRef();
#elif OPEN_GL 
#endif // DIRECTX

public:
private:

#if DIRECTX
  ID3D11VertexShader *mptr_vertexShader;
#elif OPEN_GL
#endif // DIRECTX

};

