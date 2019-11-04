#pragma once
#include <string>
#include "utility/enGraphics.h"
#include "utility/enDefs.h"
#include "../Resource.h"
class cDeviceContext;
class cViewport;

class cWindow
{
public:
  cWindow();
  ~cWindow();

public:
  /*! this starts the window
  \param ptr_proc [in] a pointer to the wndProc function
  \param instance [in] a variable used to get an HWND
  \param windowName [in] give the window a name
  \param className [in] this is just an identifier for the window */
  bool
    init(LRESULT(CALLBACK*ptr_proc)(HWND, UINT, WPARAM, LPARAM),
         const HMODULE *Instance,
         const char *windowName = nullptr,
         const char *className = nullptr);
#ifndef OPEN_GL
  /*! returns  the window handle */
  HWND
    getHandle()const;
#elif OPEN_GL

  /*! returns the window handle */
  GLFWwindow
    *getHandle();
#else
  void*
    getHandle();
#endif // DIRECTX

   /*! returns the current HMODULE of the window */
  HMODULE
    getInstance() const;

  uint32
    getWidth() const;

  uint32
    getHeight() const;

  void
    update();

private:
#if UNICODE 
  std::wstring m_name;
  std::wstring m_className;
#else
  std::string m_name;
  std::string m_className;
#endif // UNICODE 
  /*! a pointer for the wndProc function */
  WNDCLASSEX m_descriptor;
  LRESULT(CALLBACK *mptr_proc) (HWND, UINT, WPARAM, LPARAM);
#ifndef OPEN_GL 
  HWND m_handle;
#elif OPEN_GL
  GLFWwindow* m_handle;
#else 
  void* m_handle;
#endif // DIRECTX
  HINSTANCE m_instance;

  uint32 m_width;
  uint32 m_height;
};

