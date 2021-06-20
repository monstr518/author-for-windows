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


class OneLink{
public:
	int port;//8958
	void* thread;
	SOCKET SO;
	string S;
	string wayexe;

	OneLink();

	DWORD WINAPI Poisc_client(void*param);
	static DWORD WINAPI run_exe(void*param);

};


typedef vector<OneLink*>	V_OL;


class Serv{
public:
	int active;
	V_OL List;
	
	void Send(const char*);
	void Recv();
};


//	head.h :-|