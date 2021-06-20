
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the MODUL1_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// MODUL1_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.

#ifdef MODUL1_EXPORTS
#define MODUL1_API __declspec(dllexport)
#else
#define MODUL1_API __declspec(dllimport)
#endif



extern "C" {
	MODUL1_API void SendString(const char*);
	MODUL1_API const char* RecvString();
}


