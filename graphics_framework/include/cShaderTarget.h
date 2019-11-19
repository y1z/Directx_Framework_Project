#pragma once
#include "cDepthStencilView.h"
#include "cRenderTarget.h"
#include "cRenderTargetView.h"
#include "cTexture2D.h"
#include "utility/CustomStructs.h"
#include "cShaderResourceView.h"

class cDevice;

/**
*@brief creates a self contained instance of a render-target and 
* everything needed to make it work.
*
*/

class
  cShaderTarget
{
public:
  cShaderTarget();

  ~cShaderTarget();

  bool
    init(const sWindowSize &screenSize, cDevice &device);

  cRenderTargetView&
    getRenderTargetView();

  cShaderResourceView&
  getShaderResourceView();

  cShaderResourceView *
  getShaderResourceViewPtr();

private:
  cDepthStencilView m_depthStencilView;
  cRenderTargetView m_renderTargetView;
  cShaderResourceView m_ResourceView;
  cRenderTarget m_renderTarget;
  cTexture2D m_texture;
};

