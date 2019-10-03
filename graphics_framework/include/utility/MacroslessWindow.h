#pragma once
// make sure i can use function with the name 'min' and 'max' 
#define NOMINMAX 1
// remove modem configuration extentions 
#define NOMCX 1
// remove communication related stuff https://docs.microsoft.com/en-us/windows/win32/winprog/using-the-windows-headers
#define NOCOMM 1
// don't include encryption related headers 
#define NOCRYPT 1
// remove shit i don't need https://docs.microsoft.com/en-us/windows/win32/winprog/using-the-windows-headers
#define WIN32_LEAN_AND_MEAN  1
 

#include <Windows.h>
