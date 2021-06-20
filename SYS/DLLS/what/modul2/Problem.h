//	Problem.h

//#include <iostream>
#include <string>
#include <vector>

using namespace std;

typedef vector<double> V_D;
typedef vector<string> V_S;

class Row{
public:
	string nameA;
	string Result;
	V_D R;
	double A,B;
	bool getLine(char*&s);
	void controlAB();
};

typedef vector<Row> V_Row;
typedef vector<Row*> V_pRow;


class Problem{
public:
	string	nameValue;
	V_S		Ret;
	V_Row	vrow;
	V_D		pset;

	int ParserProblem(const char*);
	static string getLine(char*&,V_D&);
	string run();
	void controlAB();
	double findX(bool&,bool,double,double,bool,bool freeA=0,bool freeB=0);
};



class SCANER{
public:
	static void noProbel(char*&);
	static int scanSlovo(char*,char*&);
	static string getName(char*&);
	static double getDigit(char*&);
	static char* toString(double);
};

//	Problem.h	:-|