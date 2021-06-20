//	head.h

#define MODUL_EXPORTS

#ifdef MODUL_EXPORTS
#define MODUL_API __declspec(dllexport)
#else
#define MODUL_API __declspec(dllimport)
#endif


extern "C" {
	MODUL_API void SendString(char*);
	MODUL_API char* RecvString();
}

