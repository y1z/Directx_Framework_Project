#pragma once
#include <cinttypes>
#include <cassert>
#include <iostream>
/**
 * @brief This file contains all  typedef, #define and all other data
 * that can be check at compile time
 *
 */

 /*++++++++++++++++++++++++++++++++++++*/
 /* TYPE's */
 /*++++++++++++++++++++++++++++++++++++*/
  // signed int type's
using int8 = int8_t; /*!<this is a 8-bit integer */
using int16 = int16_t;//<! this is a 16-bit integer 
using int32 = int32_t;//<! this is a 32-bit integer 
using int64 = int64_t;//<! this is a 64-bit integer 

// unsigned int type's 
using uint8 = uint8_t;//<! this is a 8-bit unsigned integer 
using uint16 = uint16_t;//<! this is a 16-bit unsigned integer 
using uint32 = uint32_t;//<! this is a 32-bit unsigned integer 
using uint64 = uint64_t;//<! this is a 64-bit unsigned integer 

// signed int type's that tell you how much memory they use 
using Byte = int_least8_t;//<! a single byte 
using Byte2 = int_least16_t;//<! 2 bytes per instance
using Byte4 = int_least32_t;//<! 4 bytes per instance 
using Byte8 = int_least64_t;//<! 8 bytes per instance 

// unsigned int type's that tell you how much memory they use 
using uByte = uint_least8_t;//<! a single byte 
using uByte2 = uint_least16_t;//<! 2 bytes per instance
using uByte4 = uint_least32_t;//<! 4 bytes per instance 
using uByte8 = uint_least64_t;//<! 8 bytes per instance 

/*++++++++++++++++++++++++++++++++++++*/
/* Error codes */
/*++++++++++++++++++++++++++++++++++++*/
enum class enErrorCode :Byte4
{
  Sucessful = 0b0000'0000'0000'0000'0000'0000'0000'0000'0000, //<! indicates that no error occurred  
  UnClassified = 0b0000'0000'0000'0000'0000'0000'0000'0000'0001,//<! indicates that there is no-know name for the error 
  NotReady = 0b0000'0000'0000'0000'0000'0000'0000'0000'0010, //<! means that the function/class/object needed more set up before it's ready to be used  

};
/*++++++++++++++++++++++++++++++++++++*/
/* Logger functions */
/*++++++++++++++++++++++++++++++++++++*/
namespace enError
{
  /*! Present error message to help with debugging */
  static void ENTROPY_log_error(const char* FunctionName,
                                          std::size_t LineNumber,
                                          const char* FileName)
  {
    std::cerr << '\t' << "Error occurred in : \"" << FunctionName << "\"\n"
      << "On line " << LineNumber << '\n'
      << "In File \"" << FileName << "\n\n";
  }

  static void ENTROPY_log_error_code(enErrorCode errorCode = enErrorCode::UnClassified,
                                               const char* FunctionName = nullptr)
  {
    //this is the format all messages will follow 
    auto messageFormat = [](const char* FuncName, const char* ErrorReport) {
      std::clog << '\t' << ErrorReport << '\n' << '\t' << "in Function : '" << FuncName << "'\n\n";
    };

    switch (errorCode)
    {
      case enErrorCode::Sucessful:
        messageFormat(FunctionName, "executed successfully");
        break;
      case enErrorCode::UnClassified:
        messageFormat(FunctionName, "Some UN Classified Error occurred");
        break;
      case enErrorCode::NotReady:
        messageFormat(FunctionName, "Class needs more setup before it can be used");
        break;
      default:
        messageFormat(FunctionName, "No error massage set");
        break;
    }

  }


}

/*++++++++++++++++++++++++++++++++++++*/
/* #define's  */
/*++++++++++++++++++++++++++++++++++++*/

#ifndef NDEBUG
/*! output that an error occurred
in some function in some file in some line
does not indicate the error only where it possibly
happen */
#define EN_LOG_ERROR enError::ENTROPY_log_error( __func__ ,__LINE__,__FILE__);
#else
// does nothing 
#define EN_LOG_ERROR
#endif // !NDEBUG
