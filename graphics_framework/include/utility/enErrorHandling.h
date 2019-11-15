#pragma once
#include "utility/enGraphics.h"
#include <iostream>
#include <sstream>

#if OPEN_GL
static void GlRemoveAllErrors()
{

  while (glGetError() != GL_NO_ERROR)
  {}
}

static bool GlCheckForError()
{
  bool IsErrorResult = false;
  while (GLenum Error = glGetError())
  {
    IsErrorResult = true;
    std::stringstream ConvertToHex;

    ConvertToHex << std::hex << ( unsigned int )Error;

    std::string Message = "\n ---- ERROR HAS OCURRED :< ";

    Message += ConvertToHex.str();
    Message += "> \n";

    std::cout << Message.c_str();
    if (Error == GL_INVALID_ENUM)
    {
      std::cout << "invalid Enum \n";
    }
    if (Error == GL_INVALID_OPERATION)
    {

      std::cout << "invalid operation \n";
    }


  }
  return IsErrorResult;
}

#endif // OPEN_GL
