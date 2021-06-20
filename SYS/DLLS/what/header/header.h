//	header.h
#pragma warning( disable:4786)
#pragma warning( disable:4005)


#include <windows.h>
#include <string>
#include <vector>
#include <iostream.h>
#include <algorithm>


#define EXPORT __declspec(dllexport)


extern "C" {
	EXPORT void SendString(char*);
	EXPORT char* RecvString();
}



using namespace std;

//---------------------------------------------------------------------------------------
class Xdll{
	HINSTANCE dll;
public:
	void (*SendString)(const char*);
	const char* (*RecvString)();
	const char* way;//Путь к библиотеке.
	bool isUnderstend;//Вопрос понял.
	string name;
	bool Questions;//Задайот вопросы.
	bool Paralele; //Приймет новый вопрос не решив старый.
	bool Recurse;  //Может задать вопрос самому себе.

	Xdll(const char*);
	~Xdll();
	bool isCorectLoadLibrary();

	void Send(const char*);
	string Recv();

};



typedef vector<string> V_S;
//---------------------------------------------------------------------------------------
class Otvet{
public:
	Xdll*kto;
	V_S	 hto;
};

typedef vector<Otvet> V_Otvet;

typedef vector<Xdll*> V_pXdll;

//---------------------------------------------------------------------------------------
class ControlerDLL{
	static char*names[];
	int LimitHistory;
	V_Otvet otvets;
	V_pXdll history;
public:
	string way;
	V_pXdll	dlls;
	int iterator,nmax;
	int Gotovo;
	bool MNames;

	ControlerDLL();
	~ControlerDLL();

	void initDLLS();

	void Vopros(const char*);
	void Vopros(const char*,V_Otvet&,V_pXdll*,string name="");

	void setCout(bool on,int n=-1);
	string getExample(string name="");

	string getOtvets();
};


//	header.h	:-|