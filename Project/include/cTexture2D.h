#pragma once
#include "../include/utiliy/Grafics_libs.h"
/*! \brief takes care of the representation of textures */
struct sTextureDescriptor
{
  float texWidth{0.0f};
  float texHeight{0.f};
  int texFormat{0};//! used to know how to interpret the data 
  int Usage{0};
  int BindFlags{0};
  int CpuAccess{0};
  // this is in case the texture comes in an array format 
  int arraySize{0};
};

/*! manages 2d textures */
class cTexture2D
{
public:
  cTexture2D();
  ~cTexture2D();
public:
  ID3D11Texture2D * getTexture();
  ID3D11Texture2D ** getTextureRef();
private:
  ID3D11Texture2D * mptr_texture;
};

