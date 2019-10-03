
#include "cSampler.h"
#include <memory>
cSampler::cSampler()
#if DIRECTX
  :mptr_sampler(nullptr)
#else
#endif // DIRECTX
{
  std::memset(&m_Description, 0, sizeof(m_Description));
}

cSampler::~cSampler()
{
#if DIRECTX
  if (mptr_sampler)
  {
    mptr_sampler->Release();
  }
#endif // DIRECTX
}

#if DIRECTX
ID3D11SamplerState *
cSampler::getSampler()
{
  return mptr_sampler;
}

ID3D11SamplerState **
cSampler::getSamplerRef()
{
  return &mptr_sampler;
}

#endif // DIRECTX

void cSampler::setDescirption(uint32_t filter, uint32_t addressU,
                              uint32_t addressV, uint32_t addressW,
                              int comparasionFunc, uint32_t maxAnisotropy,
                              float minLod, float maxLod)
{
  m_Description.filter = filter;
  m_Description.addressU = addressU;
  m_Description.addressV = addressV;
  m_Description.addressW = addressW;
  m_Description.comparingFunc = comparasionFunc;
  m_Description.AnisotropicLevel = maxAnisotropy;
  m_Description.minLod = minLod;
  m_Description.maxLod = maxLod;
}

void cSampler::setBorderColor(float red, float green,
                              float blue, float alpha)
{
  m_Description.boderColor[0] = red;
  m_Description.boderColor[1] = green;
  m_Description.boderColor[2] = blue;
  m_Description.boderColor[3] = alpha;
}

#if DIRECTX
D3D11_SAMPLER_DESC
cSampler::getDirectxSamplerDesc()
{
  D3D11_SAMPLER_DESC Result;
  std::memset(&Result, 0, sizeof(Result));
  Result.AddressU = static_cast<D3D11_TEXTURE_ADDRESS_MODE>(m_Description.addressU);
  Result.AddressV = static_cast<D3D11_TEXTURE_ADDRESS_MODE>(m_Description.addressV);
  Result.AddressW = static_cast<D3D11_TEXTURE_ADDRESS_MODE>(m_Description.addressW);
  Result.ComparisonFunc = static_cast<D3D11_COMPARISON_FUNC>(m_Description.comparingFunc);
  Result.Filter = static_cast<D3D11_FILTER>(m_Description.filter);
  Result.MaxAnisotropy = m_Description.AnisotropicLevel;
  Result.MaxLOD = m_Description.maxLod;
  Result.MinLOD = m_Description.minLod;
  for (uint8_t i = 0; i < 4; ++i)
  {
    Result.BorderColor[i] = m_Description.boderColor[i];
  }
  return Result;
}


#endif // DIRECTX

