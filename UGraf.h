//	UGraf.h

#ifdef DEF_PCONS_CLASS
	class BasisLine;
	class NetLine;
	class MarkerLine;
	class UGraf;
#endif



#ifdef DEF_PCONS_TYPES
	typedef vector<BasisLine*>	V_BL;
	typedef vector<NetLine*>	V_NL;
	typedef vector<MarkerLine*>	V_ML;
	typedef map<int,int>		M_II;
	typedef deque<int>			DE_I;
#endif






#ifdef DEF_PCONS_CONTENT
//--------------------------------------------------------------------------------------------------
class BasisLine{
protected:
	string getStr() const;
	string getStr2() const;
public:
	char c;//0< 1>
	int  a;
	string name;
	BasisLine(const int,const char);
	BasisLine(int,char,const char*);
	virtual ~BasisLine();
	BasisLine(const BasisLine&);
	virtual string toString() const = 0;
	virtual string toString2() const = 0;
};



class NetLine:public BasisLine{
public:
	int b;
	NetLine(int,char,const char*,char,int);
	~NetLine();
	NetLine(const NetLine&);
	string toString() const;
	string toString2() const;
};



class MarkerLine:public BasisLine{
public:
	string marker;
	MarkerLine(int,char,const char*,char,const char*);
	~MarkerLine();
	MarkerLine(const MarkerLine&);
	string toString() const;
	string toString2() const;
};



//------------------------------------------------------------------------------------------------
class UGraf{
	class I2{
	public:
		int		i;//pos in program
		int		fn;//find in this
		M_II	mii;//const n - n
		V_NL	vnlok;
		I2();
	};
	typedef deque<I2*>	DE_I2;
	void getNumberLinksTable(V_I&vso,M_II&,const V_NL&);//sub ==
public:
	int nfree;
	V_NL lincs;
	V_ML markers;

	UGraf();
	virtual ~UGraf();
	
	UGraf(const UGraf&);
	int getUnits(S_I&) const;

	void add(NetLine*);
	void add(MarkerLine*);

	string toString() const;
	string toString(string) const;

	void findMarker(V_BL&vbl,int,char,string*,char,string*) const;
	void findNet(V_BL&,int,char,string*,char,int) const;
	void findNetBlizko(V_NL&,int);
	void deleteMarker(int,char,string*,char,string*);
	void deleteNet(int,char,string*,char,int);

	//static void outMAP(M_II*);
	UGraf&  operator = (const UGraf&);
	int getnew();
	static char getVector(const char);
	bool operator == (const UGraf&);
	static bool equale(UGraf*UGA,UGraf*UGB,int a,int b);
	bool operator < (const UGraf&);
	bool operator > (const UGraf&);
	operator string () const;
	void import(const char*);
	int getSizeOf() const;
	CVARIANT* ExportData() const;
	bool ImportData(CVARIANT*);

};


#endif



//	UGraf.h	:-|