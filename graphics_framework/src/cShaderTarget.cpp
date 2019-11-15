#include "cShaderTarget.h"
#include "cDevice.h"
#include "utility/HelperFuncs.h"
#include <cassert>

cShaderTarget::cShaderTarget()
{}

cShaderTarget::~cShaderTarget()
{}

bool
cShaderTarget::init(const sWindowSize & screenSize, cDevice &device)
{
#if DIRECTX

  bool isSucceful = false;

  sTextureDescriptor  textureDesc = helper::createDepthStencilDesc(screenSize.width, screenSize.height);
  textureDesc.texFormat = enFormats::fR32G32B32A32;
  textureDesc.Usage = 0;
  textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

  isSucceful = device.CreateTexture2D(textureDesc, m_renderTarget.getTexture());
  if (isSucceful == false)
  {
    EN_LOG_ERROR;
    return false;
  }

  m_renderTarget.init(screenSize.width, screenSize.height, textureDesc.texFormat);

  isSucceful = device.CreateRenderTargetView(m_renderTarget.getTexture(), m_renderTargetView);

  if (isSucceful == false)
  {
    EN_LOG_ERROR;
    return false;
  }
  //D3D11_RESOURCE_DIMENSION_TEXTURE2D
  // D3D_SRV_DIMENSION 
  // D3D11_SRV_DIMENSION_TEXTURE2D
  //D3D11_RESOURCE_DIMENSION_TEXTURE2D
  m_ResourceView.init(static_cast< enFormats >(textureDesc.texFormat),
                      1, 0
                      , D3D11_SRV_DIMENSION_TEXTURE2D);


  auto ResourceViewDesc = m_ResourceView.getDxDescriptor();
 // ResourceViewDesc.Texture2D.MipLevels = 1;

  HRESULT hr = device.getDevice()->CreateShaderResourceView(m_renderTarget.getTexture().getTexture(),
                                                            &ResourceViewDesc,
                                                            m_ResourceView.getShaderResourceRef());

  if (FAILED(hr))
  {
    EN_LOG_ERROR;
    return false;
  }


  return true;
#elif OPEN_GL
  GlRemoveAllErrors();

  if (m_depthStencilView.getDepthStencil().getID() == 0 &&
      m_ResourceView.getResourceID() == 0 &&
      m_texture.getID() == 0)
  {
    m_depthStencilView.init(enFormats::depthStencil_format);

    m_ResourceView.init(enFormats::fR32G32B32A32, 1, 0, 3);

    m_texture.init(screenSize.width, screenSize.height,
                   enFormats::fR32G32B32A32, 0,
                   ( int )enBufferUse::renderTragetOut);

  }
  else
  {
    EN_LOG_ERROR_WITH_CODE(enErrorCode::NotReady);
    return false;
  }

  if (GlCheckForError())
  {
    EN_LOG_ERROR;
    return false;

  }

  device.CreateRenderTargetView(m_renderTarget, m_renderTargetView);

#endif // DIRECTX

  return false;
}

cRenderTargetView &
cShaderTarget::getRenderTargetView()
{
  return m_renderTargetView;
}

cShaderResourceView &
cShaderTarget::getShaderResourceView()
{
  return m_ResourceView;
}
