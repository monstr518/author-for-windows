//scaner.h


#ifdef DEF_PCONS_CLASS
	class SCANER;
#endif



#ifdef DEF_PCONS_TYPES
#endif



#ifdef DEF_PCONS_CONTENT
//--------------------------------------------------------------------------------------------------
class SCANER{
public:
	static void noProbel(const char*&);
	static char*toString(int);
	static char*toString(double);
	static string writeChar(char);
	static string writeString(const string&);
	static string convertString(string&);
	static string convertChar(string&);

	static void randomize();

	static bool isDigit(char,int sus,int*t=NULL);
	static void scanDigit(const char*&,string&,int sus);
	static unsigned long double getNumber(string&,int sus);
	static long double f_digit(const char*);
	static int scanSlovo(const char*slovo,const char*&);

	static string replaceSpecSumbolHTML(const char*);
	static int findNumberStringLine(const char*begin,const char*);

	//прочитает имя
	static string readName(const char*&s);
	//прочитает строку
	static string readString(const char*&s);
	//прочитает символ
	static string readChar(const char*&s);
	//прочитает число
	static string readDigit(const char*&s);
	
	static string replaceAll(string,string,string);

	static string trim(string);

	static void poTabu(string&);
};


//--------------------------------------------------------------------------------------------------
#endif
//scaner.h	:-|