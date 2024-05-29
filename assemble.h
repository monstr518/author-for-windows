// assemble.h

#ifdef DEF_PCONS_CLASS
	class Assemble;
	class Algorithm;
#endif



#ifdef DEF_PCONS_TYPES
	typedef vector<int>			V_I;
	typedef vector<double>		V_D;
	typedef vector<Algorithm*>	V_AL;
	typedef vector<string>		V_S;
	typedef list<Algorithm*>	L_AL;
	typedef map<string,Algorithm*> M_SA;
	typedef map<Algorithm*,L_AL::iterator> M_Ait;
#endif



#ifdef DEF_PCONS_CONTENT
//--------------------------------------------------------------------------------------------------
class oneterm{
public:
	static char*m[];
	char*c;
	int nfunc;
	Algorithm*A;
	string term;

	oneterm();
	~oneterm();
	oneterm(const oneterm&);
	oneterm& operator = (const oneterm&);

	static oneterm getTerm(char*&,Assemble*);
	static char* getSTerm(char*&s);
};


typedef list<oneterm> L_OT;
//--------------------------------------------------------------------------------------------------
class Type{
public:
	string name;
	int n; // ****
	bool isAmpersent,isConst;

	Type();

	bool operator == (const Type&) const;
	bool operator != (const Type&) const;
	bool operator > (const Type&) const;
	bool operator < (const Type&) const;

	static Type* getType(L_OT::iterator&);
	string toString();
	bool isSET();
	CVARIANT Analysis();
	void synthesis(M_SV*);
};

typedef vector<Type> V_TIP;


class Algorithm{
public:
	Algorithm*up;

	virtual ~Algorithm();

	virtual string toString()=0;
	virtual Algorithm* copy()=0;
	virtual void initUP(Algorithm*);
	virtual int ZapuskTree(I*,MAIN*,CVARIANT*&);
	virtual CVARIANT Analysis();
	virtual Algorithm*findLabel(int);
	//virtual CVARIANT* getMapOperator(MAIN*);
	virtual string getFormul(string);
	virtual string getReverseFormul(string);
	TRAVERS* Border(I*,MAIN*);
	bool isProsto();
	bool isIF();

	virtual bool operator == (const Algorithm&) const;
	virtual bool operator > (const Algorithm&) const;
	virtual bool operator < (const Algorithm&) const;
	virtual int getSub(V_I*,int,Algorithm**&);
	virtual string getAdress();
	virtual int Power(int n) const;
	virtual int ReplaceName(const string*,const string*);
	virtual void getNames(V_S*,int)=0;
	int PowerW() const;
};



class BaseFunction :public Algorithm{
public:
	bool argumentsON;
	V_S argumentsNames;
	Algorithm*Body;

	BaseFunction();
	~BaseFunction();

	string toString();
	Algorithm* copy();
	void getNames(V_S*,int);
	void initUP(Algorithm*);

	int ZapuskTree(I*,MAIN*,CVARIANT*&);

	bool operator == (const Algorithm&) const;
	bool operator > (const Algorithm&) const;
	bool operator < (const Algorithm&) const;

	int getSub(V_I*,int,Algorithm**&);
	int Power(int n) const;
};



class SubBase{
public:
	//virtual ~SubBase();

	//virtual SubBase** findRfor(int);
	virtual int getPriorited();
	virtual Algorithm** getR()=0;
	bool needForScobki(Algorithm*);
};


class Base :public Algorithm{
public:
	string text;
	int type;//0-var 1-string 2-char 3-digit 5-Algorithm

	string toString();
	Algorithm* copy();
	int ZapuskTree(I*,MAIN*,CVARIANT*&);
	static int findVar(I*,MAIN*,CVARIANT*&,string,int&pos,bool create=0);
	CVARIANT Analysis();
	string getFormul(string);
	string getReverseFormul(string);

	bool operator == (const Algorithm&) const;
	virtual bool operator > (const Algorithm&) const;
	virtual bool operator < (const Algorithm&) const;

	int getSub(V_I*,int,Algorithm**&);
	string getAdress();
	int Power(int n) const;
	virtual int ReplaceName(const string*,const string*);
	virtual void getNames(V_S*,int);
};


class Base2 :public SubBase,public Algorithm{
	bool controlSwapConstruction(I*,MAIN*,CVARIANT*,int);
public:
	static char*m[];
	Algorithm*A,*B;
	int n;//operator

	Base2();
	virtual ~Base2();

	static bool isBase2(string&,int&);
	int getPriorited();
	Algorithm** getR();
	bool isR();

	virtual string toString();
	virtual Algorithm* copy();
	virtual void initUP(Algorithm*);
	virtual int ZapuskTree(I*,MAIN*,CVARIANT*&);
	virtual CVARIANT Analysis();
	virtual string getFormul(string);
	virtual string getReverseFormul(string);
	virtual int ReplaceName(const string*,const string*);

	int Pirsa(string str,CVARIANT*&V,MAIN*M,CVARIANT*XA,int xa,Base*);
	virtual bool operator == (const Algorithm&) const;
	virtual bool operator > (const Algorithm&) const;
	virtual bool operator < (const Algorithm&) const;

	int getSub(V_I*,int,Algorithm**&);
	string getAdress();
	int Power(int n) const;
	virtual void getNames(V_S*,int);
};


class Base3 :public Base2{
public:
	Algorithm*V;//B-else A-condition

	Base3();
	~Base3();

	string toString();
	Algorithm* copy();
	void initUP(Algorithm*);
	int ZapuskTree(I*,MAIN*,CVARIANT*&);
	CVARIANT Analysis();
	string getFormul(string);
	string getReverseFormul(string);
	bool operator == (const Algorithm&) const;
	bool operator > (const Algorithm&) const;
	bool operator < (const Algorithm&) const;

	int getSub(V_I*,int,Algorithm**&);
	int Power(int n) const;
	virtual int ReplaceName(const string*,const string*);
	virtual void getNames(V_S*,int);
};


class Prefix :public SubBase,public Algorithm{
public:
	static char*m[];
	int n;//m[n]==0 - Algorithm
	Algorithm*X;

	Prefix();
	~Prefix();

	static bool isPrefix(string,int&);
	int getPriorited();
	Algorithm** getR();

	string toString();
	Algorithm* copy();
	void initUP(Algorithm*);
	int ZapuskTree(I*,MAIN*,CVARIANT*&);
	CVARIANT Analysis();
	string getFormul(string);
	string getReverseFormul(string);

	virtual bool operator == (const Algorithm&) const;
	virtual bool operator > (const Algorithm&) const;
	virtual bool operator < (const Algorithm&) const;
	virtual int getSub(V_I*,int,Algorithm**&);
	string getAdress();
	virtual int Power(int n) const;
	virtual int ReplaceName(const string*,const string*);
	virtual void getNames(V_S*,int);
};


class Sufix :public Algorithm{
public:
	static char*m[];
	int n;
	Algorithm*X;

	Sufix();
	virtual ~Sufix();

	static bool isSufix(string&,int&);

	virtual string toString();
	virtual Algorithm* copy();
	virtual void initUP(Algorithm*);
	virtual int ZapuskTree(I*,MAIN*,CVARIANT*&);
	CVARIANT Analysis();
	string getFormul(string);
	string getReverseFormul(string);

	virtual bool operator == (const Algorithm&) const;
	virtual bool operator > (const Algorithm&) const;
	virtual bool operator < (const Algorithm&) const;

	virtual int getSub(V_I*,int,Algorithm**&);
	virtual int Power(int n) const;
	virtual int ReplaceName(const string*,const string*);
	virtual void getNames(V_S*,int);
};


class CallFunc :public Sufix{
	void Help(V_CVARIANT&VCV,CVARIANT*&V);
	void trace(MAIN*M,V_CVARIANT&VCV,char sposob);
	void Sleeep(V_CVARIANT&VCV,bool isSleep);
	void implode(V_CVARIANT&VCV,CVARIANT*&V);
	void split(V_CVARIANT&VCV,CVARIANT*&V);
	void size(V_CVARIANT&VCV,CVARIANT*&V);
	void outAllMemory(I*,V_CVARIANT&VCV,CVARIANT*&V,MAIN*M);
	void rozpad(I*,V_CVARIANT&VCV,CVARIANT*&V,MAIN*M);
	void outAllMTree(I*,CVARIANT*&V,MAIN*M);
	void include(V_CVARIANT&VCV,CVARIANT*&V,MAIN*M);
	void uninclude(V_CVARIANT&VCV,MAIN*M);
	void isset(V_CVARIANT&VCV,CVARIANT*&V);
	void eval(I*,V_CVARIANT&VCV,CVARIANT*&V,MAIN*M);
	void getFunction(I*,V_CVARIANT&VCV,CVARIANT*&V,MAIN*M);
	void getstring(CVARIANT*&V);
	void getPosLaverProcess(V_CVARIANT&VCV,CVARIANT*&V,MAIN*M);
	void reserve(V_CVARIANT&VCV,CVARIANT*&V);
	void getThisFunctionName(I*,CVARIANT*&V,MAIN*M);

	void define(I*,V_CVARIANT&VCV,MAIN*M,CVARIANT*&V);

	void LoadDLL(V_CVARIANT&VCV,MAIN*M);
	void unLoadDLL(V_CVARIANT&VCV,MAIN*M);
	void RunDLL(V_CVARIANT&VCV, CVARIANT*&V, MAIN*M);
	void getNamesAllDll(CVARIANT*&V,MAIN*M);

	void echo(V_CVARIANT&VCV,MAIN*M);
	void rand(V_CVARIANT&,CVARIANT*&V);
	void typeof(V_CVARIANT&VCV,CVARIANT*&V);
	void getValueInLaver(I*,V_CVARIANT&VCV,CVARIANT*&V,MAIN*M);
	void setValueInLaver(I*,V_CVARIANT&VCV,MAIN*M);
	void getThis(I*,CVARIANT*&V,MAIN*M,int);
	void amputace(V_CVARIANT&VCV,MAIN*M);
	void getVariantov(V_CVARIANT&VCV,CVARIANT*&V,MAIN*M);
	void getValuesInLaver(I*,V_CVARIANT&VCV,CVARIANT*&V,MAIN*M);
	void LaverOFF(V_CVARIANT&VCV,MAIN*M);
	void getListLaver(V_CVARIANT&VCV,CVARIANT*&V,MAIN*M);
	void setPrioritetProcess(I*,V_CVARIANT&VCV,MAIN*M);
	void getTime(CVARIANT*&V);
	void timeFormat(V_CVARIANT&VCV,CVARIANT*&V);
	void PROGRAM(V_CVARIANT&VCV,CVARIANT*&V,MAIN*M);
	void ControlOFF(I*,MAIN*M,CVARIANT*&V);
	void ClearPsetOFF(I*,MAIN*M);
	void getAllModules(MAIN*M,CVARIANT*&V);
	void getAllModuleFunctions(V_CVARIANT&VCV,MAIN*M,CVARIANT*&V);
	void runFunction(I*,V_CVARIANT&VCV,MAIN*M,CVARIANT*&V);
	void replaceSumbolHTML(V_CVARIANT&VCV,CVARIANT*&V);
	void issystemf(V_CVARIANT&VCV,CVARIANT*&V);
	void EIInclude(V_CVARIANT&VCV,MAIN*M,bool);
	void Spirit(I*,V_CVARIANT&VCV,MAIN*M);
	void Match(V_CVARIANT&VCV,MAIN*M,CVARIANT*&V);
	void getThisModule(MAIN*M,CVARIANT*&V);
	void FileAccess(string&,V_CVARIANT&VCV,CVARIANT*&V);
	void AJAX(V_CVARIANT&VCV,CVARIANT*&V);
	void JSON(V_CVARIANT&VCV,CVARIANT*&V);
	void Server(MAIN*M,V_CVARIANT&VCV,CVARIANT*&V);
	

	void forchar(CVARIANT*thisVal,string&name,CVARIANT*&V,V_CVARIANT&VCV);
	void forinterval(CVARIANT*thisVal,string&name,CVARIANT*&V,V_CVARIANT&VCV);
	void forstring(CVARIANT*thisVal,string&name,CVARIANT*&V,V_CVARIANT&VCV);
	void forpointer(I*Pset,CVARIANT*thisVal,string&name,CVARIANT*&V,V_CVARIANT&VCV,MAIN*M);
	void forvector(CVARIANT*thisVal,string&name,CVARIANT*&V,V_CVARIANT&VCV);
	void forset(CVARIANT*thisVal,string&name,CVARIANT*&V,V_CVARIANT&VCV);
	void formap(CVARIANT*thisVal,string&name,CVARIANT*&V,V_CVARIANT&VCV);
	void forfunction(I*,CVARIANT*thisVal,string&name,CVARIANT*&V,V_CVARIANT&VCV,MAIN*);
	void forprogram(CVARIANT*thisVal,string&name,CVARIANT*&V,V_CVARIANT&VCV);
	void forgraf(CVARIANT*thisVal,string&name,CVARIANT*&V,V_CVARIANT&VCV,MAIN*);
	void fordigit(CVARIANT*thisVal,string&name,CVARIANT*&V,V_CVARIANT&VCV);
	void formodule(CVARIANT*thisVal,string&name,CVARIANT*&V,V_CVARIANT&VCV,MAIN*);
public:
	static const char* m[];
	V_AL params;

	CallFunc();
	~CallFunc();

	string toString();
	Algorithm* copy();
	void initUP(Algorithm*);
	int ZapuskTree(I*,MAIN*,CVARIANT*&);
	void fc(I*,string&,V_CVARIANT&,CVARIANT*&,MAIN*);
	void mc(I*,string&,string&name,V_CVARIANT&VCV,CVARIANT*&V,MAIN*M);
	CVARIANT Analysis();
	static bool isCFunction(string);
	string getFormul(string);
	string getReverseFormul(string);

	bool operator == (const Algorithm&) const;
	bool operator > (const Algorithm&) const;
	bool operator < (const Algorithm&) const;

	int getSub(V_I*,int,Algorithm**&);
	int Power(int n) const;
	virtual int ReplaceName(const string*,const string*);
	virtual void getNames(V_S*,int);
};


class AccesMasiv :public Sufix{
	bool subAccessOne(MAIN*M,Laver*L,I*Pset,CVARIANT*&CV,CVARIANT*&S,CVARIANT*&V,int&v,int a);
public:
	Algorithm*P;//parametr
	//n: 3-[] 4-{}

	AccesMasiv();
	~AccesMasiv();

	string toString();
	Algorithm* copy();
	void initUP(Algorithm*);
	int ZapuskTree(I*,MAIN*,CVARIANT*&);
	CVARIANT Analysis();
	string getFormul(string);
	string getReverseFormul(string);

	bool operator == (const Algorithm&) const;
	bool operator > (const Algorithm&) const;
	bool operator < (const Algorithm&) const;

	int getSub(V_I*,int,Algorithm**&);
	int Power(int n) const;
	virtual int ReplaceName(const string*,const string*);
	virtual void getNames(V_S*,int);
};


class Cast :public Prefix{
public:
	Type cast;

	Cast();

	string toString();
	Algorithm* copy();
	int PodgotovkaRozpada(MAIN*);
	int ZapuskTree(I*,MAIN*,CVARIANT*&);
	CVARIANT Analysis();
	string getFormul(string);
	string getReverseFormul(string);

	bool operator == (const Algorithm&) const;
	bool operator > (const Algorithm&) const;
	bool operator < (const Algorithm&) const;

	int getSub(V_I*,int,Algorithm**&);
	virtual int ReplaceName(const string*,const string*);
	virtual void getNames(V_S*,int);
};


class Interval :public Algorithm{
public:
	bool a,b;//1-[] 0-()
	Algorithm*A,*B;

	Interval();
	~Interval();

	string toString();
	Algorithm* copy();
	void initUP(Algorithm*);
	int ZapuskTree(I*,MAIN*,CVARIANT*&);

	bool operator == (const Algorithm&) const;
	bool operator > (const Algorithm&) const;
	bool operator < (const Algorithm&) const;

	int getSub(V_I*,int,Algorithm**&);
	int Power(int n) const;
	virtual int ReplaceName(const string*,const string*);
	virtual void getNames(V_S*,int);
};


class AlgoSet :public Algorithm{
public:
	V_AL nabor;

	~AlgoSet();

	string toString();
	Algorithm* copy();
	void initUP(Algorithm*);
	int ZapuskTree(I*,MAIN*,CVARIANT*&);
	CVARIANT Analysis();
	string getFormul(string);
	string getReverseFormul(string);

	bool operator == (const Algorithm&) const;
	bool operator > (const Algorithm&) const;
	bool operator < (const Algorithm&) const;

	int getSub(V_I*,int,Algorithm**&);
	int Power(int n) const;
	virtual int ReplaceName(const string*,const string*);
	virtual void getNames(V_S*,int);
};



class AComposition :public Algorithm{
public:
	M_SA table;

	~AComposition();

	string toString();
	Algorithm* copy();
	void initUP(Algorithm*);
	int ZapuskTree(I*,MAIN*,CVARIANT*&);
	CVARIANT Analysis();

	bool operator == (const Algorithm&) const;
	bool operator > (const Algorithm&) const;
	bool operator < (const Algorithm&) const;

	int getSub(V_I*,int,Algorithm**&);
	int Power(int n) const;
	virtual int ReplaceName(const string*,const string*);
	virtual void getNames(V_S*,int);
};



//--------------------------------------------------------------------------------------------------
class Label :public Algorithm{
public:
	int id;

	string toString();
	Algorithm* copy();
	CVARIANT Analysis();
	Algorithm*findLabel(int);

	bool operator == (const Algorithm&) const;
	bool operator > (const Algorithm&) const;
	bool operator < (const Algorithm&) const;

	int getSub(V_I*,int,Algorithm**&);
	int Power(int n) const;
	void getNames(V_S*,int);
};



//--------------------------------------------------------------------------------------------------
class Sequence :public Algorithm{
public:
	L_AL nabor;

	~Sequence();

	string toString();
	Algorithm* copy();
	void initUP(Algorithm*);
	CVARIANT Analysis();
	Algorithm*findLabel(int);

	bool operator == (const Algorithm&) const;
	bool operator > (const Algorithm&) const;
	bool operator < (const Algorithm&) const;

	int getSub(V_I*,int,Algorithm**&);
	int Power(int n) const;
	virtual int ReplaceName(const string*,const string*);
	virtual void getNames(V_S*,int);
};



//--------------------------------------------------------------------------------------------------
class CreateVar :public Algorithm{
public:
	bool isInit,isMassiv;
	Type tip;
	string name;
	Algorithm*Init;//x=0;
	V_AL params;//x(0,0);

	CreateVar();
	~CreateVar();

	string toString();
	string toString2();
	Algorithm* copy();
	void initUP(Algorithm*);
	int ZapuskTree(I*,MAIN*,CVARIANT*&);
	CVARIANT* RECmasiv(V_I&,int n=0);
	CVARIANT Analysis();
	string getFormul(string);
	string getReverseFormul(string);

	bool operator == (const Algorithm&) const;
	bool operator > (const Algorithm&) const;
	bool operator < (const Algorithm&) const;

	int getSub(V_I*,int,Algorithm**&);
	int Power(int n) const;
	virtual int ReplaceName(const string*,const string*);
	virtual void getNames(V_S*,int);
};


//--------------------------------------------------------------------------------------------------
class SpecSumbol :public Algorithm{
public:
	static char*m[];
	int n;

	virtual ~SpecSumbol();

	virtual string toString();
	virtual Algorithm* copy();
	static void ExitFunction(MAIN*,CVARIANT*XA=NULL);
	virtual int ZapuskTree(I*,MAIN*,CVARIANT*&);
	virtual CVARIANT Analysis();
	//virtual CVARIANT* getMapOperator(MAIN*);
	virtual string getFormul(string);
	virtual string getReverseFormul(string);

	virtual bool operator == (const Algorithm&) const;
	virtual bool operator > (const Algorithm&) const;
	virtual bool operator < (const Algorithm&) const;

	virtual int getSub(V_I*,int,Algorithm**&);
	virtual int Power(int n) const;
	virtual int ReplaceName(const string*,const string*);
	virtual void getNames(V_S*,int);
};



//--------------------------------------------------------------------------------------------------
class SpecSumbol2 :public SpecSumbol{
public:
	Algorithm*X;

	SpecSumbol2();
	~SpecSumbol2();

	string toString();
	Algorithm* copy();
	void initUP(Algorithm*);
	int ZapuskTree(I*,MAIN*,CVARIANT*&);
	CVARIANT Analysis();
	//CVARIANT* getMapOperator(MAIN*);
	string getFormul(string);
	string getReverseFormul(string);

	bool operator == (const Algorithm&) const;
	bool operator > (const Algorithm&) const;
	bool operator < (const Algorithm&) const;

	int getSub(V_I*,int,Algorithm**&);
	int Power(int n) const;
	virtual int ReplaceName(const string*,const string*);
	virtual void getNames(V_S*,int);
};



//--------------------------------------------------------------------------------------------------
class IF :public Algorithm{
public:
	Algorithm*X,*A,*E;

	IF();
	~IF();

	string toString();
	Algorithm* copy();
	void initUP(Algorithm*);
	int ZapuskTree(I*,MAIN*,CVARIANT*&);
	CVARIANT Analysis();
	Algorithm*findLabel(int);

	bool operator == (const Algorithm&) const;
	bool operator > (const Algorithm&) const;
	bool operator < (const Algorithm&) const;

	int getSub(V_I*,int,Algorithm**&);
	int Power(int n) const;
	virtual int ReplaceName(const string*,const string*);
	virtual void getNames(V_S*,int);
};



//--------------------------------------------------------------------------------------------------
class WHILE :public Algorithm{
public:
	int n;//0-repeat 1-while 2-do
	Algorithm*X,*A;

	WHILE();
	~WHILE();

	string toString();
	Algorithm* copy();
	void initUP(Algorithm*);
	CVARIANT Analysis();
	Algorithm*findLabel(int);

	bool operator == (const Algorithm&) const;
	bool operator > (const Algorithm&) const;
	bool operator < (const Algorithm&) const;

	int getSub(V_I*,int,Algorithm**&);
	int Power(int n) const;
	virtual int ReplaceName(const string*,const string*);
	virtual void getNames(V_S*,int);
};



//--------------------------------------------------------------------------------------------------
class FOR :public Algorithm{
public:
	//X-условие
	Algorithm*X,*B;
	Algorithm*P;// for(;X;P)B;

	FOR();
	~FOR();

	string toString();
	Algorithm* copy();
	void initUP(Algorithm*);
	CVARIANT Analysis();
	Algorithm*findLabel(int);

	bool operator == (const Algorithm&) const;
	bool operator > (const Algorithm&) const;
	bool operator < (const Algorithm&) const;

	int getSub(V_I*,int,Algorithm**&);
	int Power(int n) const;
	virtual int ReplaceName(const string*,const string*);
	virtual void getNames(V_S*,int);
};









//--------------------------------------------------------------------------------------------------
class Assemble{
	class SX{
	public:
		int n;
		Bloc*B;
		V_I next;
		SX();
		~SX();
	};
	void outLOT(L_OT&);
	bool isCorectSkobki(L_OT&);
	void adaptationString(L_OT&);
public:
	string&PHTML;
	File*lastFile;
	LOADER F;
	CLASS*GlobalSpace;
	MAIN*Main;

	Assemble(string&,MAIN*,const char*);
	~Assemble();

	void Readterms(char*&s,L_OT&lot);
	Algorithm* Razbor(char*&,int); //1-; 2-,; 0-(_,)
	Algorithm* gather(L_OT&,L_OT::iterator&);
	Algorithm* getONE(char*&,int&er,Sequence*S=NULL);
	Sequence* getNabor(char*&,int&er); // {code;}
	Label* getLabel(char*&);
	int probaVarRead(char*&,L_AL&);
	void probaVarReadSub(char*&,Type&,string&);
	SpecSumbol*getSpecSumbol(char*&,int&er);
	IF* getIF(char*&,int&er);
	WHILE* getWHILE(char*&,int&er);
	Algorithm* getFOR(char*&,int&er,Sequence*);

	int getSpisokArgs(V_TIP&,V_S&,V_AL&,char*&);
	string getRowNAMES(char*&);//::A::B
	Function* getFunction(char*&,int&er,File*);
	CLASS* getClass(char*&,int&er,File*ff,CLASS*Space=NULL);

	Sxema* getSxema(char*&);

	int Load(const char*,char*,bool);

	static Algorithm* getComand(MAIN*,string);
};


//--------------------------------------------------------------------------------------------------
#endif
// assemble.h	:-|