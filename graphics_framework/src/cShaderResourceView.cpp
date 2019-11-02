#include "../include/cShaderResourceView.h"
#include "cDevice.h"
#include "cDeviceContext.h"
#include "utility/enErrorHandling.h"

//#ifndef  STB_IMAGE_IMPLEMENTATION 
//#define STB_IMAGE_IMPLEMENTATION 
#include "utility/stb_image.h"

//#endif // STB_IMAGE_IMPLEMENTATION
//#endif // STBI_INCLUDE_STB_IMAGE_H   

#if OPEN_GL
static uint32 GlTextureTacker = GL_TEXTURE0;
#elif DIRECTX

#endif // OPEN_GL

cShaderResourceView::cShaderResourceView()
#if DIRECTX
  :mptr_shaderResource(nullptr)
#endif // DIRECTX
{}

cShaderResourceView::~cShaderResourceView()
{
#if DIRECTX
  if (mptr_shaderResource)
  {
    mptr_shaderResource->Release();
  }
#endif // DIRECTX
}

#if DIRECTX
ID3D11ShaderResourceView *
cShaderResourceView::getShaderResource()
{
  return mptr_shaderResource;
}

ID3D11ShaderResourceView **
cShaderResourceView::getShaderResourceRef()
{
  return &mptr_shaderResource;
}

D3D11_SHADER_RESOURCE_VIEW_DESC
cShaderResourceView::getDxDescriptor()
{
  D3D11_SHADER_RESOURCE_VIEW_DESC result;
  std::memset(&result, 0, sizeof(result));
  result.Format = static_cast< DXGI_FORMAT >(m_desc.format);
  result.ViewDimension = static_cast< D3D11_SRV_DIMENSION >(m_desc.viewDim);
  return  result;
}
#endif // DIRECTX

bool
cShaderResourceView::createShaderResourceFromFile(std::string_view file,
                                                  cDevice & device,
                                                  cDeviceContext & deviceContext)
{
#if OPEN_GL
  mptr_resourceData = stbi_load(file.data(), &m_witdth, &m_height, &m_channalCount, 4);
  if (mptr_resourceData == nullptr)
  {
    return false;
  }
  glGenTextures(1, &m_resourceID);
  glBindTexture(GL_TEXTURE_2D, m_resourceID);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  float color[] = { 1.00f, 1.0f, 0.0f, 1.0f };
  glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);

  if (mptr_resourceData != nullptr)
  {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_witdth, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, mptr_resourceData);
    glGenerateMipmap(GL_TEXTURE_2D);
    m_imageID = GlTextureTacker;
    GlTextureTacker++;
  }

  glBindTexture(GL_TEXTURE_2D, 0);

  stbi_image_free(mptr_resourceData);
  mptr_resourceData = nullptr;
  if (!GlCheckForError())
  {
    return true;
  }
  else
  {
    return false;
  }

#endif // OPEN_GL
  // TODO : implement for DIRECTX
  return true;
}

uint8 *
cShaderResourceView::getResourceData() const
{
  return mptr_resourceData;
}

#if OPEN_GL
uint32_t
cShaderResourceView::getResourceID() const
{
  return  m_resourceID;
}

uint32_t
cShaderResourceView::getTextureID() const
{
  return m_imageID;
}

#endif // OPEN_GL

int32_t
cShaderResourceView::getWidth() const
{
  return m_witdth;
}

int32_t
cShaderResourceView::getHeight() const
{
  return  m_height;
}

int32_t
cShaderResourceView::getChannelCount() const
{
  return m_channalCount;
}

void
cShaderResourceView::setDescriptor(Formats format, int newViewDim)
{
  this->m_desc.format = format;
  this->m_desc.viewDim = newViewDim;
}

