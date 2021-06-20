
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the MODUL5_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// MODUL5_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef MODUL5_EXPORTS
#define MODUL5_API __declspec(dllexport)
#else
#define MODUL5_API __declspec(dllimport)
#endif


extern "C" {
	MODUL5_API void SendString(const char*);
	MODUL5_API const char* RecvString();
}
