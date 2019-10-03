#pragma once
#include "../include/utility/Grafics_libs.h"
#include "cShaderBase.h"
class cPixelShader : public cShaderBase
{
public:
  cPixelShader();

  ~cPixelShader();

#if DIRECTX
  ID3D11PixelShader *getPixelShader();
  ID3D11PixelShader **getPixelShaderRef();
  
private:
  ID3D11PixelShader *mptr_pixelShader;
#endif // DIRECTX
};

