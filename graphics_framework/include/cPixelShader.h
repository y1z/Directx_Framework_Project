#pragma once
#include "utility/enGraphics.h"
#include "cShaderBase.h"

/**
*@brief holds all data necessary for the pixel shader 
*/
class cPixelShader : public cShaderBase
{
public:
  cPixelShader();
  cPixelShader(const cPixelShader &other) = delete;

  cPixelShader(cPixelShader &&other);

  ~cPixelShader();

#if DIRECTX
  ID3D11PixelShader *getPixelShader();
  ID3D11PixelShader **getPixelShaderRef();
  
private:
  ID3D11PixelShader *mptr_pixelShader;
#endif // DIRECTX
};

