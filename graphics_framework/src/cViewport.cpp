
#include "../include/cViewport.h"
//std includes
#include <memory>
cViewport::cViewport()
{
#if DIRECTX
  std::memset(&m_viewport, 0, sizeof(m_viewport));
#endif // DIRECTX
  std::memset(&m_viewportDesc, 0, sizeof(m_viewportDesc));
}

cViewport::~cViewport()
{}

void
cViewport::setViewport(uint32 width, uint32 height,
                       float minDepth, float maxDepth,
                       float TopLeftX, float TopLeftY)
{
  m_viewportDesc.width = width;
  m_viewportDesc.height = height;
  m_viewportDesc.minDepth = minDepth;
  m_viewportDesc.maxDepth = maxDepth;
  m_viewportDesc.TopLeftX = TopLeftX;
  m_viewportDesc.TopLeftY = TopLeftY;
}

#if DIRECTX
D3D11_VIEWPORT
cViewport::getViewport()
{
  SecureZeroMemory(&m_viewport,sizeof(m_viewport));
  m_viewport.Height = m_viewportDesc.height;
  m_viewport.Width = m_viewportDesc.width;
  m_viewport.MinDepth = m_viewportDesc.minDepth;
  m_viewport.MaxDepth = m_viewportDesc.maxDepth;
  m_viewport.TopLeftX = m_viewportDesc.TopLeftX;
  m_viewport.TopLeftY = m_viewportDesc.TopLeftY;
  return  m_viewport;
}

#elif OPEN_GL
sViewportDesc 
cViewport::getViewport() const
{
  return m_viewportDesc;
}

int32 
cViewport::getWidth() const
{
  return m_viewportDesc.width;
}

int32 
cViewport::getHeight() const
{
  return  m_viewportDesc.height;
}

#endif // 0

