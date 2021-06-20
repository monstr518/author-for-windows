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

typedef map<int,UGraf*>	M_IUG;
typedef vector<string>	V_S;


class MenagerUG{
	int Nfree,pos;
public:
	string buf;
	M_IUG ALLUG;

	V_S Result;

	MenagerUG();
	~MenagerUG();

	void clear();
	void NewUG();
	void getPos();
	void getNewUnitNumber();

	void NET(string);
	void MARKER(string);

	void GETNET(string);
	void GETMARKER(string);
	void GETWAY(string);
};



//	head.h :-|