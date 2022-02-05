// sxema.h

#ifdef DEF_PCONS_CLASS
	class Bloc;
	class Sxema;
#endif



#ifdef DEF_PCONS_TYPES
	typedef vector<Bloc*>	V_B;
	typedef set<Bloc*>		S_B;
	typedef set<int>		S_I;
	typedef map<int,Bloc*>	M_IB;
	typedef map<Bloc*,int>	M_BI;
	typedef map<Bloc*,Bloc*> M_BB;
#endif



#ifdef DEF_PCONS_CONTENT
//--------------------------------------------------------------------------------------------------

#if defined(_MSC_VER) || defined(_MSC_EXTENSIONS)
#define DELTA_EPOCH_IN_MICROSECS 11644473600000000Ui64
#else
#define DELTA_EPOCH_IN_MICROSECS 11644473600000000ULL
#endif

//--------------------------------------------------------------------------------------------------

class Bloc{
public:
	int accessNumber;//уникальный номер блока в схеме
	int rang;
	V_B up,down;
	Algorithm*A;

	Bloc();
	~Bloc();

	bool operator == (const Bloc&);
	bool compareTree(const Bloc*,const Bloc*HE=NULL) const;
	bool Less(const Bloc*,const Bloc*HE=NULL) const;

	void getAll(S_B&);
	void disconnectDown();
	void disconnectUp();
	void deleted();
	void terminateValey(const Bloc*);

	string toString();
};



class Sxema{
	class UnitCucle;
	typedef vector<UnitCucle*> V_pUC;
	class UnitCucle{
	public:
		UnitCucle();
		~UnitCucle();
		bool operator == (const UnitCucle&) const;
		int t;//0-start 1-while 2-do 3-if
		bool isFor,isWhile;
		V_pUC next;
		Bloc*Begin,*End,*Exit;
		int Scaner(Sxema*,V_pUC&,int&vxodov);//0-error 1-ok
		Sequence* Build();
		int ControlBreak(Sxema*S,V_pUC&WAY,int n,Bloc*P);
	};

	void clearRang();
	void setRang(int rang=1,Bloc*X=NULL,S_B*S=NULL);
	void ClearSxema();
	void OneToSxema(V_I&,V_B&Goto,M_IB&mib,Bloc*,Algorithm*,V_B*Cont,V_B*Break);
	static void AddUp(Bloc*P,Bloc*N);//P->N
	static void AddDown(Bloc*P,Bloc*N);//P->N

	void diveLineEnd(Bloc*,Bloc*);
	Bloc* findPenultimate(Bloc*,Bloc*,V_B*U=NULL);

	bool findWayOpenPages(Bloc*P,Bloc*E,V_B&VB);
	int findOpenPagesTo(Bloc*A);
	int getCountPagesOpen(Bloc*,Bloc*);
	void insertPagesForGoto(Bloc*,int);
	int NLabel;
	//найдет нейбольшую метку и вернет новый, уникальный номер метки
	int getNextNLabel();
public:
	bool rangOK;
	Bloc*HeadEnd;
	Function*f;
	M_IB table;
	int freeN;//in table[freeN]

	Sxema();
	~Sxema();

	Sxema* copy();

	void controlRang();
	void ToSxema(Algorithm*);
	string toString(bool rangs=0);
	void OptimizeFree();
	void controlTable();

	bool controlCorrectSxema();

	Algorithm* Coder();//destroy sxema
	//разделит условные блоки с более двух переходов, на блоки по два перехода
	void ower_ifs();
	void DEFINE_ONE();
	void Antitupik();
	void findEndIF(Bloc*,V_B*S1,V_B*S2,V_B*S3);
	//void DEVIDE(Bloc*A,Bloc*B);

	bool operator == (Sxema&);

	void getGraph(UGraf*);
};


#endif


// sxema.h	:-|