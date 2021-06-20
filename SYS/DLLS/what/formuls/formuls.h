//	formuls.h
#define MODUL6_API __declspec(dllexport)


extern "C" {
	MODUL6_API void SendString(const char*);
	MODUL6_API const char* RecvString();
}



#pragma warning( disable:4786)
#pragma warning( disable:4005)


#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <set>
#include <iostream>
#include <math.h>
#include <algorithm>


using namespace std;

typedef map<string,double>	M_SD;
typedef vector<int>		 V_I;
typedef vector<double>	 V_D;
typedef vector<V_D>		 V2_D;
typedef vector<string>	 V_S;
typedef set<string>		 S_S;
typedef map<int,int>	 M_II;
typedef map<string,V_D>	 M_SDD;


class formuls{
	static int posin(char*,char);
public:
	virtual ~formuls();
	virtual string toString()=0;
	static formuls*getFormul(const char*&);
	virtual bool isCorect();
	virtual double compute(M_SD*table=NULL)=0;
	virtual void ReplaceValue(M_SD&table)=0;
	virtual bool operator == (formuls&)=0;
	bool operator != (formuls&);
	virtual formuls* copy()=0;
	virtual bool calculated()=0;
	virtual int estimate()=0;
	virtual void getVariables(S_S&)=0;
	virtual bool isUnknown(S_S&)=0;
};


typedef vector<formuls*> V_f;
typedef vector<formuls**> V_ppf;


//-----------------------------------------------------------------------------------------------
class value : public formuls{
public:
	bool	isValue;
	double	x;
	string	name;
	string toString();
	double compute(M_SD*table=NULL);
	void ReplaceValue(M_SD&table);
	bool operator == (formuls&);
	formuls* copy();
	bool calculated();
	int estimate();
	void getVariables(S_S&);
	bool isUnknown(S_S&);
};


//-----------------------------------------------------------------------------------------------
class row : public formuls{
public:
	int  level;//0+- 1*/
	V_f	sub;
	V_I inverse;
	~row();
	string toString();
	bool isCorect();
	double compute(M_SD*table=NULL);
	void ReplaceValue(M_SD&table);
	bool operator == (formuls&);
	formuls* copy();
	bool calculated();
	void ReversZnak();
	int estimate();
	void getVariables(S_S&);
	bool isUnknown(S_S&);
};


//-----------------------------------------------------------------------------------------------
class Pow : public formuls{
public:
	formuls *A,*B;//pow(A,B)
	~Pow();
	string toString();
	bool isCorect();
	double compute(M_SD*table=NULL);
	void ReplaceValue(M_SD&table);
	bool operator == (formuls&);
	formuls* copy();
	bool calculated();
	int estimate();
	void getVariables(S_S&);
	bool isUnknown(S_S&);
};


//-----------------------------------------------------------------------------------------------
class SCANER{
public:
	static void noProbel(const char*&);
	static bool getDigit(const char*&,double&);
	static string getName(const char*&);
	static int scanSlovo(const char*,const char*&);
	static string getLine(const char*&,V_D&);
};


//-----------------------------------------------------------------------------------------------
class Optimizator{
	bool noPow;
	void clearFormuls();
public:
	V_f Formuls;
	Optimizator(formuls*);
	~Optimizator();

	void insertF(formuls*);
	formuls* run();
	static int f0(formuls*&);
	void f1(formuls*);
	void f2(formuls*);
	void f3(formuls*);
	void f4(formuls*);
	void f5(formuls*);
	void f6(formuls*);
	void f7(formuls*);
	void f8(formuls*);
	void f9(formuls*);
	void f10(formuls*);
	void f11(formuls*);
	void f12(formuls*);
	void f13(formuls*);
	void f14(formuls*);
	void f15(formuls*);
	void f16(formuls*);
	void f17(formuls*);
	
	formuls* runone(int);
	formuls* Open2();
};



//-----------------------------------------------------------------------------------------------
class Problem{
public:
	virtual ~Problem();
	virtual bool load(char*)=0;
	virtual string run()=0;
	static Problem*getProblem(char*);
};


//-----------------------------------------------------------------------------------------------
class Compare :public Problem{
public:
	formuls*A,*B;
	string znak;

	Compare();
	~Compare();

	static bool Ravno(formuls*,formuls*);

	bool load(char*);
	string run();

};


//-----------------------------------------------------------------------------------------------
class Result :public Problem{
public:
	formuls*f;
	V2_D	v2d;
	V_S		names;

	Result();
	~Result();

	bool load(char*);
	string run();

};


//-----------------------------------------------------------------------------------------------
class Optimize :public Problem{
public:
	formuls*f;
	M_SD	table;

	Optimize();
	~Optimize();

	bool load(char*);
	string run();

};


//-----------------------------------------------------------------------------------------------
class Equality :public Problem{
	static void setRow0(formuls*&);
	void restructXC(formuls*,row*,row*,bool);
	void ControlKvadrat(string&);
	static formuls*getX(formuls*,formuls*,formuls*,bool p=0);
	void analizeK(int,row*,row*);
public:
	formuls*A,*B;
	S_S		unknown;

	Equality();
	~Equality();

	bool load(char*);
	string run();
	
};




//-----------------------------------------------------------------------------------------------
class Generalized :public Problem{
public:
	V_f S;
	M_SDD*msd;

	Generalized();
	~Generalized();

	bool load(char*);
	string run();
	int NinRow(int&);
	static string newName(S_S&);
	formuls* Comparison(S_S&);
	bool complement(formuls*,formuls*);

};



//-----------------------------------------------------------------------------------------------
class VarNames :public Problem{
public:
	V_f S;

	VarNames();
	~VarNames();

	bool load(char*);
	string run();

};



//-----------------------------------------------------------------------------------------------
class Systems :public Problem{
	int u,Exit;
	int add(formuls*,formuls*,int);
	void push(formuls*);
public:
	V_f A,B,Find;
	V_I C;

	Systems();
	~Systems();

	bool load(char*);
	string run();

};


//	formuls.h	:-|