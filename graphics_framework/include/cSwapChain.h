#pragma once
#include "utility/enGraphics.h"
#include "../include/cDepthStencilView.h"
#include "../include/cRenderTargetView.h"
#include "cRenderTarget.h"
#include "utility/enDefs.h"
#include "enum_headers/enFormatEnums.h"
#include <cstdint>
//forward declarations
class cRenderTargetView;
class cTexture2D;
class cDevice;
class cWindow;

/*! controls the settings for the swap-chain*/
struct sSwapDesc
{
  uint32_t buffWidth = 1;
  uint32_t buffHeight = 1;
  int buffUsage = 0;
  int buffFormat = 0;
#if DIRECTX
  HWND outputWindow; //! used only in directX 
#elif OPEN_GL
  GLFWwindow * outputWindow;//!< used only in open_gl 
#else
  void* outputWindow = nullptr;
#endif // DIRECTX
//! controls the refresh rate  buffNumaretor / buffDenominator
  uint16_t buffRefershNumaretor = 60;
  uint16_t buffRefershDenominator = 1;
  uint8_t buffCount{ 0 };
  uint8_t sampCount{ 0 };
  uint8_t sampQuality{ 0 };
  bool isWindowd = true;
};

/*! take care of presenting the image to the window*/
class cSwapChain
{
public:
  cSwapChain();

  ~cSwapChain();
public:// functions 
#if DIRECTX
  IDXGISwapChain
    *getSwapChain();

  IDXGISwapChain
    **getSwapChainRef();
#endif // DIRECTX

#if DIRECTX
  DXGI_SWAP_CHAIN_DESC
    getSwapChainDesc();

#endif // DIRECTX
  /*! for the description of the swap-chain
         \param width [in] the width of the buffer
         \param height [in] the height of the buffer
         \param format [in] what format to use for the buffer
         \param usage [in] for what is the buffer going to used
         \param handle [in] this is to get the window where the image is going to
         \param bufferCount [in] how many buffer there are going to be
         \param RefreshNumerator [in] this is a representation of a rational number that controls the refresh-rate
         \param RefreshDenominator [in] this is a representation of a rational number that controls the refresh-rate
         \param SmapCount [in] the number of samples per pixel of a resource
         \param SampQuality [in] the quality of a sample
         \param isWindowed [in] tell the swap-chain if it has a window to display images */
  void
    setSwapChain(uint32_t width, uint32_t height,
                 int format, int usage,
                 cWindow &handle, uint8_t bufferCount = 1,
                 uint16_t RefreshNumerator = 60, uint16_t RefreshDenominator = 1,
                 uint8_t SampCount = 1, uint8_t SampQuality = 0,
                 bool isWindowed = true);
  /*! just passes the argument to the render-target*/
  void
    setRenderTarget(uint32 width, uint32 height, enFormats format);
  
#if OPEN_GL
          /*! set the window for the glfwSwapBuffers function */
  void
    setGlWindow(GLFWwindow *newWindow);

#endif // OPEN_GL

  /*!returns the swap chain render target*/
  cRenderTarget&
    getRenderTarget();

  /*!returns the swap chain depth-stencil-view*/
  cDepthStencilView&
    getDepthStencilView();

  /*!returns the swap chain depth-stencil*/
  cTexture2D&
    getDepthStencil();

  /*!*/
  cRenderTargetView&
    getRenderTargetView();

  /*!*/
  void
    setDepthStencilView(enFormats format, int Dimension = 3, int Mip = 0);

  /*! the swap-chain gets a buffer for swapping
  \param backBuffer [out] the resulting back buffer
  \param bufferIndex [in] determines which index the buffer belongs to */
  bool
    GetBuffer(cTexture2D &backBuffer, uint32_t bufferIndex);
  /*! used to attach the render-target of the class to the swapChain*/
  bool
    InitBuffer();
  bool//! this is the function that switches the back buffer with front buffer. 
    Present(uint32_t SycroOption, uint32_t PresentationOption = 0,
            unsigned int Program = 0);
  //! resizes the window 
  bool
    Resize(cDevice &device,
           uint32_t width, uint32_t height);
private:
#if DIRECTX
  IDXGISwapChain *mptr_swapChain;
#elif OPEN_GL
  /*! this is so the swap-chain can use
  the glSwapBuffer functions */
  GLFWwindow *mptr_window = nullptr;
  uint32 *mptr_backBuffer{nullptr};
  uint32 *mptr_depthStencilView{ nullptr };
#endif // DIRECTX
  sSwapDesc m_desc;
  cDepthStencilView m_depthStencilView;
  cRenderTarget m_renderTarget;
  cRenderTargetView m_renderTargetView;
};

