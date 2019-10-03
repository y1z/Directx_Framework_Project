#pragma once
#include "utility/Grafics_libs.h"

enum class TextureAddress : int
{
#if DIRECTX
  Wrap = D3D11_TEXTURE_ADDRESS_WRAP,
  Mirror = D3D11_TEXTURE_ADDRESS_MIRROR,
  Clamp = D3D11_TEXTURE_ADDRESS_CLAMP,
  Border = D3D11_TEXTURE_ADDRESS_BORDER,
  Mirror_once = D3D11_TEXTURE_ADDRESS_MIRROR_ONCE
#else
  Wrap,
  Mirror,
  Clamp,
  Border,
  Mirror_once
#endif // DIRECTX
};

