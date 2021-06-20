// interval.h



#define EXPORT __declspec(dllexport)


extern "C" {
	EXPORT void SendString(const char*);
	EXPORT const char* RecvString();
}


#include <string>
#include <vector>
#include <iostream>


using namespace std;

//#pragma warning( disable:4554)


class base;
typedef vector<base*> V_C;
typedef vector<double> V_D;
typedef vector<string> V_S;


//--------------------------------------------------------------------------------------------------
class SCANER{
public:
	static void noProbel(const char*&);
	static bool getDigit(const char*&,double&);
	static string getName(const char*&);
	static int scanSlovo(const char*,const char*&);
};


//--------------------------------------------------------------------------------------------------
class base{
public:
	virtual ~base();

	virtual void optimize();
	virtual string toString()=0;
	virtual string toBasisString();
	virtual base* notControl(bool not)=0;
	static void NotOK(base*&);
	virtual int Vsos();
	virtual base* copy()=0;

	static base* read(string);
	static base* read(const char*&);
	static void problem(const char*);
};




class not : public base {
public:
	base*X;

	not(base*);
	~not();

	string toString();
	base* notControl(bool not);
	int Vsos();
	void optimize();
	base* copy();
};



class condition : public base {
public:
	int c; // 4< 2= 1>
	double n;

	// n, <, =, >
	condition(double,int,int,int);
	condition();

	condition* U(condition*);
	condition* notU(condition*);
	string toString();
	string getNumber();
	base* notControl(bool not);
	static base* load(const char*&);
	base* copy();
};



class nabor : public base {
public:
	int U; // 1-U(+) 0-notU(*)
	V_C sub;

	nabor(int);
	~nabor();

	string toString();
	string toBasisString();
	base* notControl(bool not);
	int Vsos();
	void optimize();
	int strax();
	void strax2();
	base* copy();
};


// interval.h :-|