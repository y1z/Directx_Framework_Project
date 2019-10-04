#pragma once
#include "../include/utility/Grafics_libs.h"
#include "../include/cShaderResourceView.h"
class cMesh;
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
#if DIRECTX
  ID3D11Texture2D * getTexture();
  ID3D11Texture2D ** getTextureRef();

  ID3D11ShaderResourceView  *getResource();
  ID3D11ShaderResourceView  **getResourceRef();
#endif // DIRECTX

  void
    setDescriptor(float width, float height,
                  int format, int usage,
                  int bindFlags, int CpuAccess = 0,
                  int arraySize = 1);

  sTextureDescriptor
    getDescriptor();

  void Release();
private:
#if DIRECTX
  ID3D11Texture2D * mptr_texture;
#endif // DIRECTX
public://make private
  sTextureDescriptor m_desc;
private:
  cShaderResourceView m_resourceView;
};

