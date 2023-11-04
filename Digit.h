//	Digit.h

#ifdef DEF_PCONS_CLASS
	class Digits;
#endif



#ifdef DEF_PCONS_TYPES
	typedef vector<char>	V_C;
#endif




#ifdef DEF_PCONS_CONTENT
//--------------------------------------------------------------------------------------------------

class Digits{
	bool znak; // 1+ 0-
	void clear();

	void zdvigUP1E(char c=0);
	char zdvigUP1M();
	void zdvigUP2E(char c=0);
	char zdvigUP2M();

	char zdvigDOWN1E();
	void zdvigDOWN1M(char c=0);
	char zdvigDOWN2E();
	void zdvigDOWN2M(char c=0);

	void plus1();
	void minus1();
	void not();
	

	static void antiZero(V_C&);

	char compareABS(const Digits&) const;
	void umnogenie(const char);
public:
	static char*cpi;
	static char*ce;

	V_C exponenta, mantisa;
	int mlimit;

	Digits();
	Digits(const Digits&);
	Digits& operator = (const Digits&);

	void invertZnak();

	void set(int);
	void set(double);
	void set(const char*);

	void zdvigUP1();
	void zdvigUP2();

	void zdvigDOWN1();
	void zdvigDOWN2();

	void operator << (int);
	void operator >> (int);

	Digits& operator ++ ();
	Digits& operator -- ();

	operator string ();
	operator int ();
	operator double ();

	bool operator == (const Digits&);
	bool operator > (const Digits&);
	bool operator < (const Digits&);

	Digits& operator += (const Digits&);
	Digits& operator -= (const Digits&);

	Digits& operator *= (const Digits&);
	Digits& operator /= (const Digits&);
	Digits& operator %= (const Digits&);

	void Pow(Digits&);

	bool isZero() const;

	string toString();
	char compare(const Digits&);

	void suma(const Digits&);
	void minus(const Digits&);

	void away(int);
	void round(int);

	int getSizeOf() const;

	string toNumberSystem(char) const;
	void ImportNumberSystem(string&,char);

};


#endif



//	Digit.h	:-|