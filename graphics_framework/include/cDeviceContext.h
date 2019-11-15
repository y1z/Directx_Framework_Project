#pragma once 
#include "utility/enGraphics.h"
#include "../include/utility/enDefs.h"
// std includes 
#include <cstdint>
// Forward decelerations 
class cTexture2D;
class cRenderTargetView;
class cDepthStencilView;
class cVertexShader;
class cPixelShader;
class cInputLayout;
class cVertexBuffer;
class cIndexBuffer;
class cConstBuffer;
class cSampler;
class cViewport;
class cBuffer;
class cShaderResourceView;
class cMesh;
/***************************/
struct sSamplerDesc;
struct sTextureDescriptor;
struct sDepthStencilDescriptor;
struct sInputDescriptor;
struct sColorf;

        /*!Contains all the necessary  information for drawing to
the buffer */
struct sDrawData
{
#if DIRECTX
#elif OPEN_GL
  /*! used for knowing which of the index-buffers are set */
  uint32 currentIndexBuffer = 0u;
  /*! used for knowing which of the vertex-buffers are set */
  uint32 currentVertexBuffer = 0u;
  /*! used for knowing which type of topology is set */
  uint32 currentTopology = 0u;
  /*! used for knowing how to interpret the indices of the index buffer */
  uint32 currentFormat = 0u;
#endif // DIRECTX
};



/*!takes care of preparing/setting all variables
that are necessary for drawing to the screen*/
class cDeviceContext
{
public:
  cDeviceContext();

  ~cDeviceContext();

public:// functions 

#if DIRECTX
  //! for interfacing with directX 
  ID3D11DeviceContext *
    getDeviceContext();
  //! for function that need 2 pointers to pointers 
  ID3D11DeviceContext **
    getDeviceContextRef();
#endif // DIRECTX

  //! resets all things set by device context to default settings 
  void
    ClearState();

  /*! set render-targets and can set multiple render-targets
      \param renderTragetsViews [out] the render-targets to be set
      \param  depthStencilView [out] the set depth-stencil view(note there can only be 1)
      \param numRenderTargets [in] to tell how many render target there are */
  void
    OMSetRenderTargets(cRenderTargetView renderTragetsViews[],
                       cDepthStencilView &depthStencilView,
                       uint32_t numRenderTragets = 1);

  /*!set the view port/s */
  void
    RSSetViewports(cViewport viewports[],
                   uint8_t numViewports = 1);

  void //! set's how to interpret the data from the shaders 
    IASetInputLayout(cInputLayout &inputLayout);

  void //! set's the vertex-buffers (can set multiple at a time )
    IASetVertexBuffers(cVertexBuffer vertexBuffer[], uint32_t numBuffers,
                       uint32_t Slot = 0);

  void/*! set's the index buffer
      \param indexBuffer [in] the index-buffer that going to be set
      \param Format [in] how to interpret indices from the index-buffer*/
    IASetIndexBuffer(cIndexBuffer &indexBuffer, int Format, int offSet = 0);

  void/*! set's the topology */
    IASetPrimitiveTopology(int enTopology);

  void/*! updates some resource on the graphic card
      \param Buffer [in] the buffer that contains the data that going to the graphics card
      \param originOfData [in] this is the data the graphics card is going to receive*/
    UpdateSubresource(cBuffer *Buffer, const void *originOfData);

  void
    ClearRenderTargetView(cRenderTargetView &renderTargetView, sColorf* color = nullptr);

  void //! clear the depthStencil with the choice of only clearing the depth/stencil portion or both
    ClearDepthStencilView(cDepthStencilView &depthStencilView, bool ClearStencil = false,
                          bool ClearDepth = true, uint8_t StencilClearValues = 0,
                          float DepthClearValue = 1.0f);

  void /*!sets the vertex shader */
    VSSetShader(cVertexShader &vertexShader);

  void /*! set's the constant buffers */
    VSSetConstantBuffers(cConstBuffer &Buffer, uint8_t Slot);

  void//! set's the pixel shader 
    PSSetShader(cPixelShader &pixelShader);

  void/*! set's the resources for the pixel shader (can set multiple)*/
    PSSetShaderResources(cShaderResourceView  ShaderResources[], uint32_t numResources = 1, uint32_t Slots = 0);

  void/*! set's the resources for the pixel shader (can only set one) */
    PSSetShaderResources(cMesh &ShaderResources, uint32_t Slot = 0);

  void/*! setting the const-buffers for the pixel shader (can only set one at a time)
      \param Slots the individual slots where the constant buffer goes to.*/
    PSSetConstantBuffers(cConstBuffer &Buffer, uint32_t Slots);

  void/*!set's the samplers (can set multiple) */
    PSSetSamplers(cSampler samplers[], uint32_t numSamplers = 1, uint32_t slot = 0);

  void/*! this is the function that draws to the window
      \param IndexCount [in] how many indexes to drawi
      \param indexOffset [in] if for some reason you what an off set ???*/
    DrawIndexed(uint32_t indexCount, uint32_t indexOffset);

  /**/
  bool
    SetShaders(cVertexShader &vertexShader, cPixelShader &pixelShader);
private:
#if DIRECTX
  ID3D11DeviceContext* mptr_deviceContext = nullptr;
#elif OPEN_GL
   /*!where all information relevant to drawing to the screen is keep-ed */
  sDrawData m_drawingData;

#endif // DIRECTX
};

