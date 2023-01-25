//	RegularExpressions.h

//	Vadym Dovhopolyy
//	(C) OFF	2023


#pragma warning( disable:4786)
#pragma warning( disable:4005)
#pragma warning( default:4355)


#include <vector>
#include <map>
#include <string>
#include <algorithm>


using namespace std;


typedef vector<string>	V_S;

namespace RegularExpressions {

//-------------------------------------------------------------------------------------------------
class RowRE {
public:
	class OneTermRE;
	typedef vector<OneTermRE*> V_pOneTermRE;
	class OneTermRE {
	public:
		bool isString;
		union UnionRE {
			string *ps;
			RowRE  *pr;
		} one;

		OneTermRE(const char*);
		OneTermRE(RowRE*);
		~OneTermRE();
		string toString() const;
		OneTermRE* copy() const;
	};


	V_pOneTermRE Row;

	~RowRE();

	static RowRE* parse(const char*);
	static RowRE* parseRow(const char*&);
	static OneTermRE* getTEXT(const char*,const char*);
	string toString() const;
	RowRE* copy() const;
};





//-------------------------------------------------------------------------------------------------
class BaseFinder;
class SimvolFinder;
class PovtorFinder;
class FindersMahine;
class CTask;



class PointerRE {
public:
	char type;
	union UPointerRE {
		BaseFinder  *BF; // 1
		SimvolFinder*SF; // 2
		PovtorFinder*PF; // 3
		} one;

	PointerRE();

	void clear();
	string toString() const;
};



class BaseFinder {
public:
	int type;
	// 0 - (#Comment)
	string text; // 1
	typedef vector<PointerRE> V_PRE;
	V_PRE List; // 2-Row ((AA)(BB))	3-OR (A|B|C)
	// 4	OR (|)

	// 9	?:
	// 10	?=
	// 11	?!
	// 12	?<=
	// 13	?<!
	// 14	?>
	// 15	?()A|B

	// 20	-ismx

	// 30 - is link of condition in (1) \1

	// 50	^
	// 51	$
	// 52	\b
	// 53	\B

	// 100	($name1:())
	// 101	{name1}

	// 110	{:String}
	// 111	{:Number}
	// 112	{:Name}

	~BaseFinder();

	static BaseFinder* ConvertRowRE(const RowRE*);
	string toString() const;

	static string parseName(const char*&);

	void RAnalize(FindersMahine*) const;
	char isValidRecurse(FindersMahine*,V_S&) const; // 0-DANGER	1-OK	2-IGNORE

	bool Scaner(FindersMahine*,CTask*) const;
};





// [^:.0-5]
class SimvolFinder {
	static char newStatus(char,bool isUP);
public:
	bool isInvert;
	string nabor; // abc
	string OT,DO; // 0-9 a-z
	char S,D,W; // 1-ON(S) 2-OFF(s) 0-IGNORE
	// S [^ \f\n\r\t\v]
	// D [^0-9]
	// W [^A-Za-z0-9_]

	// . [\S\s]
	bool isPsetON;

	SimvolFinder();
	SimvolFinder(const char);

	static char SpecialeSimvolParse(const char*&);
	static char StandartSimvolParse(const char*&);

	static SimvolFinder* parse(const char*&);
	static string toEcraner(const char);
	static bool isProbelSimvol(const char);
	string toString() const;

	bool Scaner(FindersMahine*,CTask*) const;
};





class PovtorFinder {
public:
	bool isLazy; // ?
	bool isRevnive; // +
	int A,B;
	PointerRE PRE;

	PovtorFinder();
	~PovtorFinder();

	static PovtorFinder* parse(const char*&);
	static int parseInt(const char*&);

	string toString() const;
	bool isValid() const;

	void RAnalize(FindersMahine*) const;
	char isValidRecurse(FindersMahine*,V_S&) const; // 0-DANGER	1-OK	2-IGNORE

	bool Scaner(FindersMahine*,CTask*) const;
};





//-------------------------------------------------------------------------------------------------
typedef map<string,const BaseFinder*>	M_SBF;
typedef vector<CTask*>	V_pCT;


class CTask {
public:
	const char* s;
	const char* end;
	CTask**SubTasks;
	int iterator,size;
	char c;
	V_pCT PovtorsList;
	string text;
	char type;
	// 0-ignore
	// 1-char
	// 2-{} ? + *
	// 3-string
	// 4-List or one in List
	// 5-Result value: (toString()!="")
	// 6-step via all construction & ignore
	// 7-Condition & SubTasks[1]
	// 8-set up ISMX
	// 9-Sub Expression

	char ISMX; // 0000XMSI
	CTask*FirstUPRow; // for access List[(int)text] \1 (1)

	~CTask();

	string toString() const;
	RowRE* toExport() const;
};




class FindersMahine {
	const char* Expression;
public:
	string ErrorMessage;

	BaseFinder*BF;

	M_SBF TableSubFinders;
	V_S	  CallNames;

	const char* Data;

	FindersMahine(const char*);
	~FindersMahine();

	bool Build();
	RowRE* RunParser(const char*);

	string toString() const;
	static string ExamplesExpression(string);
};


//-------------------------------------------------------------------------------------------------
}



//	RegularExpressions.h	:-|