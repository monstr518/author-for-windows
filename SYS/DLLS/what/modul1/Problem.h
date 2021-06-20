//	Problem.h

//#include <iostream>
#include <string>
#include <vector>

using namespace std;

typedef vector<double> V_D;

class Problem{
public:
	

	bool isCode,isY;
	string nameF,nameY,nameX;
	V_D X,Y;
	int ParserProblem(const char*);
	static string getLine(char*&,V_D&);
	string run();
	string genf1(double a,double b);
	string genf2(double a,double b);
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