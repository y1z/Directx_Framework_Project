#pragma once
#include "../include/utiliy/Grafics_libs.h"
#include <cstdint>
#include <algorithm>

struct sSamplerDesc
{
  uint32_t filter{0};//<! how to filter the data 
  // how to address the x , y ,z coordinates of a sampled texture  
  uint32_t addressU{0};//<! how to address the x axis 
  uint32_t addressV{0};//<! how to address the y axis 
  uint32_t addressW{0};//<! how to address the z axis 

  int comparingFunc{0};//<! how to compare 
  uint32_t AnisotropicLevel{1};//<! controls the how anisotropic the texture is 
  float boderColor[4]{0.0f,0.0f,0.0f,0.0f}; //<! is only used in certain modes 
  float minLod{0.0f};//<! lowest level of detail 
  float maxLod{0.0f};//<! highest level of detail
};

/*! this will control the data from a texture is taken in */
class cSampler
{
public:
  cSampler();

  ~cSampler();
#if DIRECTX
  //! for interfacing with directX 
  ID3D11SamplerState *getSampler();
  //! for arguments that require 2 pointers 
  ID3D11SamplerState **getSamplerRef();
#endif // DIRECTX

  void
    setDescirption(uint32_t filter,
                   uint32_t addressU,
                   uint32_t addressV,
                   uint32_t addressW,
                   int comparasionFunc,
                   uint32_t maxAnisotropy = 1,
                   float minLod = 0.0f,
                   float maxLod = std::numeric_limits<float>::max());

  void
    setBorderColor(float red,
                   float green,
                   float blue,
                   float alpha = 1.0f);

#if DIRECTX
  D3D11_SAMPLER_DESC
    getDirectxSamplerDesc();

#endif // DIRECTX

private:

#if DIRECTX
  //! directX sampler implementation
  ID3D11SamplerState *mptr_sampler;
#endif // DIRECTX
  sSamplerDesc m_Description;

};

