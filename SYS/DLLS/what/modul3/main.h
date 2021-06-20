//	main.h

#include <string>
#include <sstream>
#include <vector>
#include <iostream>


using namespace std;


typedef vector<double> V_D;
typedef vector<int> V_I;


class ProblemA{
public:
	V_D M;
	V_I N,P;
	V_D A;
	double firstX;
	int er;
	string name;

	ProblemA();
	int ParserProblem(const char*);
	//void initTest();
	int oneF(V_D&W,V_I&PP,double&a);
	void run();
	string getResult();

};


class SCANER{
public:
	static void noProbel(char*&);
	static int scanSlovo(char*,char*&);
	static string getName(char*&);
	static double getDigit(char*&);
	//static char* toString(double);
};



//	main.h	:-|