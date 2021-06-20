
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the GENERALIZEDF_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// GENERALIZEDF_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef GENERALIZEDF_EXPORTS
#define GENERALIZEDF_API __declspec(dllexport)
#else
#define GENERALIZEDF_API __declspec(dllimport)
#endif

extern "C" {
	GENERALIZEDF_API void SendString(const char*);
	GENERALIZEDF_API const char* RecvString();
}


#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <map>


#pragma warning( disable:4786)
#pragma warning( disable:4005)


using namespace std;


class Answer;


typedef vector<double>	V_D;
typedef map<string,V_D>	M_SDD;
typedef vector<string>	V_S;
typedef vector<Answer*> V_A;
//typedef vector<int> V_I;


//--------------------------------------------------------------------------------------------
class SCANER{
public:
	static void noProbel(char*&);
	static int scanSlovo(char*,char*&);
	static string getName(char*&);
	static double getDigit(char*&);
	//static char* toString(double);
};


//--------------------------------------------------------------------------------------------
class Answer{
public:
	int n,t;
	V_S S;
	Answer(int x=0);
	virtual void Set(const char*);
	string Next();
};

/*
class M1:public Answer{
public:
	string formula;
	M1(string);
};
*/


class Problem{
	void ClearQ();
	Answer*getQ();
	void replaceNames(string,string);
public:
	int n;
	string GFormula,buf;
	M_SDD table,tableEX;

	Answer Results;
	V_A Q;//Склад ответов
	Answer*Reception;//Прием

	~Problem();

	string getLine(char*&,V_D&);
	int ParserProblem(const char*);
	void Set(const char*);
	void run();
};

