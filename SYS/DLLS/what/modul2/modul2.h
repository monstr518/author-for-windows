
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the MODUL2_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// MODUL2_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef MODUL2_EXPORTS
#define MODUL2_API __declspec(dllexport)
#else
#define MODUL2_API __declspec(dllimport)
#endif


extern "C" {
	MODUL2_API void SendString(const char*);
	MODUL2_API const char* RecvString();
}

