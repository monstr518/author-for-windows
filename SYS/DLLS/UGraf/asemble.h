//	asemble.h

#pragma warning( disable:4786)
#pragma warning( disable:4005)
#pragma warning( default:4355)

#include "string"
#include "deque"
#include "set"
#include "vector"
#include "map"
#include <algorithm>
#include <iostream>

using namespace std;

class BasisLine;
class NetLine;
class MarkerLine;
class UGraf;
class InterfaceGraf;

typedef deque<int>			DE_I;

typedef set<BasisLine*>		S_BL;
typedef vector<BasisLine*>	V_BL;
typedef vector<NetLine*>	V_NL;
typedef vector<MarkerLine*>	V_ML;
typedef vector<int>			V_I;
typedef map<string,BasisLine*>	M_SB;
typedef map<int,int>	M_II;
typedef set<int>		S_I;

//------------------------------------------------------------------------------------------------
class SCANER{
public:
	static void noProbel(const char*&);
	static char*toString(int);
	static string toSleh(string&);
	static bool read(char*&,char*);
	static int  getInt(char*&);
	static string getString(const char*&);
	static string getName(char*&);
};

//------------------------------------------------------------------------------------------------
class BasisLine{
protected:
	string getStr();
public:
	char c;//0< 1>
	int  a;
	string name;
	BasisLine(const int,const char);
	BasisLine(int,char,const char*);
	virtual ~BasisLine();
	BasisLine(const BasisLine&);
	virtual string toString()=0;
};


class NetLine:public BasisLine{
public:
	int b;
	NetLine(int,char,const char*,char,int);
	~NetLine();
	NetLine(const NetLine&);
	string toString();
};


class MarkerLine:public BasisLine{
public:
	string marker;
	MarkerLine(int,char,const char*,char,const char*);
	~MarkerLine();
	MarkerLine(const MarkerLine&);
	string toString();
};


//-----------------------------------------------------------------------------------------
class I2{
public:
	int		i;//pos in program
	int		fn;//find in this
	M_II	mii;//const n - n
	V_NL	vnlok;
	I2();
};


typedef deque<I2*>	DE_I2;

//------------------------------------------------------------------------------------------------
class UGraf{
	void getNumberLinksTable(V_I&vso,M_II&,const V_NL&);//sub ==
public:
	int nfree;
	//S_I meunits;
	//V_BL nets;
	V_NL lincs;
	V_ML markers;
	//InterfaceGraf*IG;

	UGraf();
	virtual ~UGraf();
	
	UGraf(const UGraf&);
	int getUnits(S_I&);

	void add(NetLine*);
	void add(MarkerLine*);

	string toString();

	void findMarker(V_BL&vbl,int,char,string*,char,string*);
	void findNet(V_BL&,int,char,string*,char,int);
	void findNetBlizko(V_NL&,int);

	static void outMAP(M_II*);
	UGraf&  operator = (const UGraf&);
	int getnew();
	static char getVector(const char);
	bool operator == (const UGraf&);
	static bool equale(UGraf*UGA,UGraf*UGB,int a,int b);
};


//--------------------------------------------------------------------------------------------------


//	asemble.h	:-|