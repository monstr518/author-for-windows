
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the MODUL4_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// MODUL4_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef MODUL4_EXPORTS
#define MODUL4_API __declspec(dllexport)
#else
#define MODUL4_API __declspec(dllimport)
#endif


extern "C" {
	MODUL4_API void SendString(const char*);
	MODUL4_API const char* RecvString();
}
