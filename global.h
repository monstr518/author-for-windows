// global.h

#ifdef DEF_PCONS_CLASS
	class File;
	class Function;
	class CLASS;
	class MAIN;
	class I;
	class CLASTER;
	class TRAVERS;
	class subI;
	class FRAGMENT;
	class Laver;
	class Xdll;
#endif




#ifdef DEF_PCONS_TYPES
	typedef map<string,CLASS*>	M_SC;
	typedef vector<Function*>	V_Func;
	typedef map<int,File*>		M_iFile;
	typedef vector<CLASS*>		V_CLASS;
	typedef map<int,Bloc*>		M_IBloc;
	typedef vector<I*>			V_II;
	typedef deque<subI*>		D_subI;
	typedef map<Algorithm*,TRAVERS>	M_AlgoT;
	typedef map<Algorithm*,int>	M_AI;
	typedef vector<FRAGMENT*>	V_pFRAGMENT;
	typedef map<int,Laver*>		M_ILAVER;
	typedef map<string,Xdll*>	M_SXDLL;
	typedef map<int,CVARIANT*>	M_ICV;
	typedef map<Algorithm*,Algorithm*>	M_AA;
	typedef vector<CVARIANT**> V_ppCVARIANT;
#endif




#ifdef DEF_PCONS_CONTENT
//--------------------------------------------------------------------------------------------------
struct interfaceFORK{MAIN*M;V_CVARIANT VCV;};
//--------------------------------------------------------------------------------------------------
class Function{
public:
	bool argumentsON;
	bool isVirtual,isStatic,isOperator;
	Type ret;
	string name;
	V_TIP	tips;
	V_S		names;
	V_AL	defaults;//умолчания

	CVARIANT*DataTable;
	Algorithm*Body;
	Sxema*sxema;

	File*file;//файл которому пренадлежит код функции
	CLASS*Space;//класс которому пренадлежит функция

	Function();
	~Function();

	string getHead(bool full=0);
	string toString();
	static Algorithm* findLabelSUB(Algorithm*,int);
	void controlSxema();
	string getPsetWay();

	bool operator == (Function&);
	bool operator >  (Function&);
	bool operator <  (Function&);
	Function*copy() const;
	bool BuildMe(const UGraf*,MAIN*);
	bool isAccesibleSub(int pos1,int pos2,S_I&SI);
	Sxema*getSubSxema(const S_I&SI);
	void deleteSubTree(int pos1,int pos2,const S_I&SI);
	void insertInUnit(int,Function*);
};



//--------------------------------------------------------------------------------------------------
class CLASS{
public:
	string name;
	M_SC SubSpace;
	V_Func functions;
	File*file;//файл в котором описан класс
	CLASS*Space;//класс которому пренадлежит етот класс
	CLASS*Parent;//класс предок
	Sequence init;//исполнить перед конструктором обекта
	CVARIANT Map;//static values

	CLASS();
	~CLASS();

	CLASS* FindSpace(string,bool create=0);
	Function* FindFunction(string fullname,bool create=0);
	//Function* FindFunction(string,V_TIP*,bool Creat=0);
	//Function* FindFunction2(string);

	string toString();
};



//--------------------------------------------------------------------------------------------------
class File{
public:
	int id;
	bool NeedSave;//нужно сохранить код
	//int LoadCompleate;//1-загружен 0-в процессе загрузки
	int Nusers;//количество пользователей даного файл/модуля
	string name,way;
	V_Func functions;
	V_CLASS Classes;
	V_S modules;
	MAIN*Main;

	File(MAIN*);
	~File();

	string toString();
	string toPsets();
	void controlSave();

	Function* FindFunction(string,bool create=0);
};



//--------------------------------------------------------------------------------------------------
class FRAGMENT{
public:
	FRAGMENT*Prev;
	V_pFRAGMENT Next;
	V_pCVARIANT Memorys;
	M_ICV Dynamic;

	FRAGMENT(FRAGMENT*P=NULL);
	virtual ~FRAGMENT();

	string toString();
	CVARIANT* getDynamicValue(int n);
};


//--------------------------------------------------------------------------------------------------
class TRAVERS{
public:
	int free;
	CVARIANT*X;
	//int tableProtect;//покрыт строкой таблици I::table
	TRAVERS();
	TRAVERS(CVARIANT*,int);
};



//--------------------------------------------------------------------------------------------------
class subI{
public:
	M_AlgoT Bloki;//ассоциативная отсечка для повторного исполлнения
	V_AL tableLinks;//хранилище адресов временных обектов
	M_AA problemEval;
	string adres;

	int isFreeFunction;
	Function*ActivF;
	Bloc*pos;
	Algorithm*Return;//for return value to Bloki.

	V_ppCVARIANT problemRozpada;

	subI(Function*);
	subI(const subI&);

	void EnterFunction(MAIN*,Algorithm*P);
	void ExitFunc(MAIN*);
};



//--------------------------------------------------------------------------------------------------
class I{
public:
	subI*	sub;//sub==posicse.rbegin().
	D_subI	posicse;//стек точек исполнения.
	FRAGMENT*Fundament;
	int Laver;
	int stop,off;
	int numberDefine;
	V_S DefineNames;

	I(int);
	I(const I&);
	~I();

	I* OFFtoStop(MAIN*);

	string toString();
	bool isRozpad(Algorithm*);
	void Rozpad(CVARIANT*,Algorithm*,MAIN*);

};


//--------------------------------------------------------------------------------------------------
class Laver{
	static string printVariants(FRAGMENT*,V_pFRAGMENT*VF=NULL);// out memory tree
public:
	int N,R;// 0-first, 1-random, 2-end
	int SuperN;
	int activLaver,activI,off;
	int condition;// last if()
	bool ExtraExit;
	int rop;// a=b  a is rop==1   b is rop==0
	bool needNameFunction;//для отличия имени переменной от функции

	V_II IS;
	FRAGMENT Head;
	V_I	adaptationMap;
	CVARIANT ResultDEF;
	Algorithm*SpiritA;

	CVARIANT*BufferSend;
	int stanBuffer,nLaverWait;

	Laver(int);
	~Laver();

	string printVariants();
};


//--------------------------------------------------------------------------------------------------
class Xdll{
	HINSTANCE dll;
public:
	const char* (*RUN)(const char*);

	string name;

	Xdll(const char*);
	~Xdll();

	bool isCorectLoadLibrary();

	const char* Run(const char*);
};



class ControlerDLL{
	MAIN* Main;
public:
	M_SXDLL	dlls;

	ControlerDLL(MAIN*);
	~ControlerDLL();

	void Load(string NameDLL, string path);
	void unLoad(string);

	const char* Run(string, string);
};




//--------------------------------------------------------------------------------------------------
class MAIN{
	bool GetFileTime(const char*fileName,SYSTEMTIME&stLocal);
	int CompareFileTime(const char*fileName,string&strFN);//0-error  1-code  -1-txt
	void BlokiClear(I*);
public:
	int NfreeOBJ;
	int indexFile;
	int FreeLaver;

	string PHTML;
	void outHTML(string&,string);
	CLASS GlobalSpace;
	M_iFile Files;
	M_ILAVER tableLavers;

	ControlerDLL CDLL;
	CRITICAL_SECTION CriticalSection1; // for cout

	JSON* Data;
	UPServac* p_Servac;

	MAIN();
	~MAIN();

	int IncludeFILE(const char*);//0-ok 1-nofile 2-error
	File*findModule(string);
	void eraseModule(int);
	void unloadOverlay(File*);
	void unloadOverlay(const char*);

	//string Memory_toString(I*);
	string Alternativs_toString();

	//void PerebrosVLokal();
	//void run(Sxema*,int);
	void RUN(V_CVARIANT*VCV=NULL);
	void StartLaver(int);
	void Mahine(int);
	int PAGECLOSE(I*,int stop=1);
	void ProgresMemory(I*,CVARIANT&,CVARIANT&);
	string FragmentOneToString(M_SV*);

	//string getLink(CVARIANT*);
	CVARIANT*getUnLink(I*,string&,bool&);
	CVARIANT*getUnLink2(I*,string&);
	bool isset(I*,const string&);
	void getMapKeys(I*,CVARIANT*&,string&,bool create=1);

	void L_IS(Laver*,Function*);
	Function*getFunction(I*,string);
	static unsigned long __stdcall fork(void*);

	bool GoErrorMessage(string&,const char* NameError, const char* text, const char* NumberLine = NULL);
};



#endif


// global.h	:-|