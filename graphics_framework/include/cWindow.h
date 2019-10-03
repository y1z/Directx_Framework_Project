#pragma once
#include <string>
#include "utility/Grafics_libs.h"
#include "utility/enDefs.h"
#include "../Resource.h"

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
    init(LRESULT(__stdcall*ptr_proc)(HWND, UINT, WPARAM, LPARAM),
         const HINSTANCE &instance,
         const char *windowName = nullptr,
         const char *className = nullptr);
#if DIRECTX
  /*! returns  the window handle */
  HWND
    getHandle()const;
  /*! returns the current HINSTANCE of the window */
  HINSTANCE
    getInstance() const;
#else
#endif // DIRECTX

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
  HWND m_handle;
  HINSTANCE m_instance;
  uint32 m_width;
  uint32 m_height;
};

