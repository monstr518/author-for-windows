//	Problem.h

//#include <iostream>
#include <string>
#include <vector>
#include <set>

using namespace std;

typedef vector<double> V_D;
typedef vector<string> V_S;
typedef set<int>	   S_I;


class Row{
public:
	string nameA;
	string Result;
	V_D R;
	int n;
	bool getLine(char*&s);
	void controlN();
};

typedef vector<Row> V_Row;
typedef vector<Row*> V_pRow;


class Problem{
public:
	string	nameValue;
	V_S		Ret;
	V_Row	vrow;

	int ParserProblem(const char*);
	static string getLine(char*&,V_D&);
	string run();
	void controlN();
	void controlUniqe();
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