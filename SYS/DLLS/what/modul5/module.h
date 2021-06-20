// module.h
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <iostream>


#pragma warning( disable:4786)
#pragma warning( disable:4005)


using namespace std;

class ProblemA;
class Answer;

typedef vector<double>	V_D;
typedef vector<V_D>		VV_D;
typedef vector<string>	V_S;
typedef vector<Answer*> V_A;
typedef map<int,int>	M_II;
typedef map<int,string>	M_IS;
//typedef vector<int> V_I;


class Answer{
public:
	int n,t;
	V_S S;
	Answer(int x=0);
	virtual void Set(const char*);
	string Next();
};


class M1:public Answer{
public:
	string formula;
	M1(string);
};


class NaborF:public Answer{
public:
	int k;
	M_IS mis;
	string votRowF(ProblemA*);
	string assembleOtvet(ProblemA*);
};



class ProblemA{
	int n;
	void ClearQ();
	Answer*getQ();
	void sort(V_D&X,V_D&Y);
	bool controlInput();
public:
	V_D X,Y;
	bool isCode,isY;
	string nameX,nameY,nameF,buf;
	//for [%t]:
	int K,nMaxX;
	M_II mii;

	Answer Results;
	V_A Q;//Склад ответов
	Answer*Reception;//Прием

	~ProblemA();

	string getLine(char*&,V_D&);
	int ParserProblem(const char*);
	void Set(const char*);
	void run();
	void f0(V_D&X,V_D&Y);
	void f1(const char*);
	void f2(Answer*);
	void f3(Answer*);
	bool controlRow(char*,V_D&);
	bool isDescretX();
	void f5();
	void getRow(NaborF*,V_D&,V_D&);
};


//typedef vector<ProblemA*>	V_PA;

//--------------------------------------------------------------------------------------------
class SCANER{
public:
	static void noProbel(char*&);
	static int scanSlovo(char*,char*&);
	static string getName(char*&);
	static double getDigit(char*&);
	//static char* toString(double);
};


// module.h :-|