#include "../include/cTexture2D.h"
#include <memory>

cTexture2D::cTexture2D()
#if DIRECTX
  :mptr_texture(nullptr)
#elif OPEN_GL
  :m_textureID(0)
#endif // DIRECTX
{
  std::memset(&m_desc, 0, sizeof(m_desc));
}


cTexture2D::~cTexture2D()
{

#if DIRECTX
  if (mptr_texture != nullptr)
  {
    mptr_texture->Release();
    mptr_texture = nullptr;
  }
#elif OPEN_GL

  if (m_textureID != 0)
  {
    // this is done because a texture can be used for many things 
    if (glIsBuffer(m_textureID) == GL_TRUE)
    {
      glDeleteBuffers(1, &m_textureID);
    }

    if (glIsTexture(m_textureID) == GL_TRUE)
    {
      glDeleteTextures(1, &m_textureID);
    }

    if (glIsRenderbuffer(m_textureID) == GL_TRUE)
    {
      glDeleteRenderbuffers(1, &m_textureID);
    }

    if (glIsFramebuffer(m_textureID) == GL_TRUE)
    {
      glDeleteFramebuffers(1, &m_textureID);
    }

    m_textureID = 0;
  }

#endif // DIRECTX
}

#if DIRECTX
ID3D11Texture2D
* cTexture2D::getTexture()
{
  return mptr_texture;
}

ID3D11Texture2D
** cTexture2D::getTextureRef()
{
  return &mptr_texture;
}

ID3D11ShaderResourceView
* cTexture2D::getResource()
{
  return m_resourceView.getShaderResource();
}

ID3D11ShaderResourceView
** cTexture2D::getResourceRef()
{
  return m_resourceView.getShaderResourceRef();
}

#endif // DIRECTX

void
cTexture2D::init(uint32 width, uint32 height,
                 int format, int usage,
                 int bindFlags, int CpuAccess,
                 int arraySize)
{
  m_desc.texWidth = width;
  m_desc.texHeight = height;
  m_desc.texFormat = format;
  m_desc.Usage = usage;
  m_desc.BindFlags = bindFlags;
  m_desc.CpuAccess = CpuAccess;
  m_desc.arraySize = arraySize;
#if OPEN_GL
  glGenTextures(1, &m_textureID);

#endif // OPEN_GL

}

sTextureDescriptor
cTexture2D::getDescriptor()
{
  return  m_desc;
}

uint32 
cTexture2D::getWidth() const
{
  return  m_desc.texWidth;
}

uint32 
cTexture2D::getHeight() const
{
  return  m_desc.texHeight;
}



#if OPEN_GL

uint32
cTexture2D::getID() const
{
  return m_textureID;
}

uint32 *
cTexture2D::getIDPtr()
{
  return &m_textureID;
}

void
cTexture2D::setID(uint32 newID)
{
  m_textureID = newID;
  }


#endif // OPEN_GL

void
cTexture2D::Release()
{
#if DIRECTX
  mptr_texture->Release();
  mptr_texture = nullptr;
#elif OPEN_GL
  if (m_textureID != 0)
  { glDeleteTextures(1, &m_textureID); }
#endif // DIRECTX
}


