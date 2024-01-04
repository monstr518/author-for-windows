// assemble2.cpp

#include "main.h"
//--------------------------------------------------------------------------------------------------

void Algorithm::initUP(Algorithm*P){up=P;}
//--------------------------------------------------------------------------------------------------
void BaseFunction::initUP(Algorithm*P){
	up=P;
	if(Body)Body->initUP(this);
}
//--------------------------------------------------------------------------------------------------
void Base2::initUP(Algorithm*P){
	up=P;
	A->initUP(this);
	B->initUP(this);
}
//--------------------------------------------------------------------------------------------------
void Base3::initUP(Algorithm*P){
	up=P;
	A->initUP(this);
	B->initUP(this);
	V->initUP(this);
}
//--------------------------------------------------------------------------------------------------
void Prefix::initUP(Algorithm*P){
	up=P;
	X->initUP(this);
}
//--------------------------------------------------------------------------------------------------
void Sufix::initUP(Algorithm*P){
	up=P;
	X->initUP(this);
}
//--------------------------------------------------------------------------------------------------
void AccesMasiv::initUP(Algorithm*a){
	up=a;
	X->initUP(this);
	P->initUP(this);
}
//--------------------------------------------------------------------------------------------------
void Interval::initUP(Algorithm*P){
	up=P;
	A->initUP(this);
	B->initUP(this);
}
//--------------------------------------------------------------------------------------------------
void AlgoSet::initUP(Algorithm*P){
	up=P;
	int i;
	for(i=0;i<nabor.size();++i)
		nabor[i]->initUP(this);
}
//--------------------------------------------------------------------------------------------------
void AComposition::initUP(Algorithm*P){
	up=P;
	M_SA::iterator it=table.begin();
	for(;it!=table.end();++it)
		it->second->initUP(this);
}
//--------------------------------------------------------------------------------------------------
void Sequence::initUP(Algorithm*P){
	up=P;
	L_AL::iterator it=nabor.begin();
	for(;it!=nabor.end();++it)if(*it)(*it)->initUP(this);
}
//--------------------------------------------------------------------------------------------------
void CreateVar::initUP(Algorithm*P){
	up=P;
	if(Init)Init->initUP(this);
	int i;
	for(i=0;i<params.size();++i)
		if(params[i])params[i]->initUP(this);
}
//--------------------------------------------------------------------------------------------------
void SpecSumbol2::initUP(Algorithm*P){
	up=P;
	X->initUP(this);
}
//--------------------------------------------------------------------------------------------------
void IF::initUP(Algorithm*P){
	up=P;
	X->initUP(this);
	if(A)A->initUP(this);
	if(E)E->initUP(this);
}
//--------------------------------------------------------------------------------------------------
void WHILE::initUP(Algorithm*P){
	up=P;
	if(X)X->initUP(this);
	if(A)A->initUP(this);
}
//--------------------------------------------------------------------------------------------------
void FOR::initUP(Algorithm*a){
	up=a;
	if(X)X->initUP(this);
	if(B)B->initUP(this);
	if(P)P->initUP(this);
}
//--------------------------------------------------------------------------------------------------
void CallFunc::initUP(Algorithm*P){
	up=P;
	X->initUP(this);
	int i;
	for(i=0;i<params.size();++i)
		params[i]->initUP(this);
}



//--------------------------------------------------------------------------------------------------
Function::Function(){
	argumentsON=0;
	isOperator=isStatic=isVirtual=0;
	Body=NULL;
	file=NULL;
	Space=NULL;
	sxema=NULL;
	DataTable=NULL;
}


Function::~Function(){
	if(Body)delete Body;
	if(sxema)delete sxema;
	if(DataTable)delete DataTable;
	int i;
	for(i=0;i<defaults.size();++i)
		if(defaults[i])delete defaults[i];
}


string Function::getPsetWay(){
	string way;
	CLASS*X=Space;
	while(X){
		if(!X->name.empty())way=X->name+"::"+way;
		X=X->Space;
		}
	return way;
}


string Function::getHead(bool full){
	string s=ret.toString(),tmp;
	if(!s.empty())s+=" ";
	if(full){
		s+=getPsetWay();
		}else{
		string p;
		if(isVirtual)p="virtual ";
		if(isStatic)p+="static ";
		s=p+s;
		}
	if(isOperator)s+=" operator ";
	//if(isDestructor)s+="~";
	s+=name;
	if(isOperator)s+=" ";
	s+="(";
	int i;
	for(i=0;i<tips.size();++i){
		if(i)s+=",";
		/*
		tmp=tips[i].toString();
		s+=tmp;
		if(!tmp.empty())s+=" ";
		*/
		if(i>=names.size())continue;
		s+=names[i];
		if(!full)
		if(defaults.size()>i)
		if(defaults[i])s+="="+defaults[i]->toString();
		}
	if(argumentsON){
		if(i)s+=",";
		s+="...";
		}
	s+=")";
	return s;
}


string Function::toString(){
	if(sxema&&!Body){
		string s="SXEMA: ";
		s+=getHead(1);
		s+="{\n";
		s+=sxema->toString();
		s+="}";
		return s;
		}
	string s=getHead(1);
	if(!Body){
		s+=";";
		return s;
		}
	Sequence*SBody=dynamic_cast<Sequence*>(Body);
	L_AL::iterator it=SBody->nabor.begin();
	for(bool first=1;it!=SBody->nabor.end();++it){
		CreateVar*cv=dynamic_cast<CreateVar*>(*it);
		if(!cv)continue;
		if(!cv->tip.name.empty())continue;
		if(first)s+=":"; else s+=",";
		first=0;
		s+=cv->toString2();
		}
	s+=Body->toString();
	if(s[s.size()-2]=='	')s.erase(s.size()-2,1);
	return s;
}



Algorithm* Function::findLabelSUB(Algorithm*A,int id){
	Label*L=dynamic_cast<Label*>(A);
	if(L)if(L->id==id)return A;
	Algorithm*F;
	IF*iif=dynamic_cast<IF*>(A);
	if(iif){
		if(iif->A){
			F=findLabelSUB(iif->A,id);
			if(F)return F;
			}
		if(iif->E){
			F=findLabelSUB(iif->E,id);
			if(F)return F;
			}
		}
	WHILE*W=dynamic_cast<WHILE*>(A);
	if(W)if(W->A){
		F=findLabelSUB(W->A,id);
		if(F)return F;
		}
	Sequence*S=dynamic_cast<Sequence*>(A);
	if(S){
		L_AL::iterator it=S->nabor.begin();
		for(;it!=S->nabor.end();++it){
			F=findLabelSUB(*it,id);
			if(F)return F;
			}
		}
	return NULL;
}



void Function::controlSxema(){
	if(sxema)return;
	sxema=new(Sxema);
	sxema->ToSxema(Body);
	delete Body;
	Body=NULL;
	sxema->f=this;
}


Function* Function::copy() const{
	Function*R=new Function;
	R->isOperator=isOperator;
	R->isStatic=isStatic;
	R->isVirtual=isVirtual;
	R->ret=ret;
	R->argumentsON=argumentsON;
	//R->file=file;
	R->tips=tips;
	R->name=name;
	R->names=names;
	R->Space=Space;
	if(Body){
		R->Body=Body->copy();
		R->Body->initUP(NULL);
		}
	if(sxema)R->sxema=sxema->copy();
	//if(DataTable)R->DataTable=DataTable->copy();
	return R;
}


bool Function::operator == (Function&t){
	if(t.isOperator!=isOperator)return 0;
	if(t.isStatic!=isStatic)return 0;
	if(t.isVirtual!=isVirtual)return 0;
	if(t.argumentsON!=argumentsON)return 0;
	t.controlSxema();
	t.sxema->controlTable();
	controlSxema();
	sxema->controlTable();
	if(t.sxema->table.size()!=sxema->table.size())return 0;
	return *t.sxema==*sxema;
}


bool Function:: operator > (Function&t){
	sxema->controlRang();
	t.sxema->controlRang();
	return sxema->HeadEnd->Less(t.sxema->HeadEnd);
}


bool Function:: operator < (Function&t){
	sxema->controlRang();
	t.sxema->controlRang();
	return t.sxema->HeadEnd->Less(sxema->HeadEnd);
}




bool Function::BuildMe(const UGraf*UG,MAIN*M){
	if(sxema)delete sxema;
	if(Body)delete Body;
	Body = NULL;
	sxema = new Sxema();
	int i,a,b,Vector,BeginNumber,EndNumber;

	V_BL vbl;
	string key = "Begin";
	bool ok=0;
	UG->findMarker(vbl,-1,2,NULL,2,&key);
	for(i=0;i<vbl.size();++i){
		BasisLine*X=vbl[i];
		V_BL vbl2;
		UG->findNet(vbl2,X->a,1,NULL,2,-1);
		if(vbl2.empty()){
			ok=1;
			BeginNumber=X->a;
			break;
			}
		}
	if(!ok)return 0;

	vbl.clear();
	key = "End";
	ok=0;
	UG->findMarker(vbl,-1,2,NULL,2,&key);
	for(i=0;i<vbl.size();++i){
		BasisLine*X=vbl[i];
		V_BL vbl2;
		UG->findNet(vbl2,X->a,2,NULL,1,-1);
		if(vbl2.empty()){
			ok=1;
			EndNumber=X->a;
			break;
			}
		}
	if(!ok)return 0;

	M_IB table;
	S_I SI;
	UG->getUnits(SI);
	S_I::iterator it = SI.begin();
	for(;it!=SI.end();++it)table[*it] = new Bloc();
	for(Vector=0;Vector<=1;++Vector)
	for(i=0;i<UG->lincs.size();++i){
		NetLine*NL=UG->lincs[i];
		bool ok = (NL->name!="-");
		if(Vector)ok=!ok;
		if(!ok)continue;
		if(!(NL->c==1 || NL->c==2))continue;
		a=NL->a;
		b=NL->b;
		if(NL->c==1){
			b=NL->a;
			a=NL->b;
			}
		if(b==EndNumber)b=BeginNumber;
		Bloc*A = table[a],*B = table[b];
		A->down.push_back(B);
		B->up.push_back(A);
		}
	for(i=0;i<UG->markers.size();++i){
		MarkerLine*ML = UG->markers[i];
		if(ML->name!="" || ML->c!=0)continue;
		string command = ML->marker,error;
		command = "{" + command +";}";
		int er=0;
		Algorithm*A=NULL;
		Sequence*S=NULL;
			{
			Assemble A(error,M,"");
			char*c=const_cast<char*>(command.c_str());
			S=A.getNabor(c,er);
			}
		if(er){
			if(S)delete S;
			continue;
			}
		if(S->nabor.size()==1){
			A=(*S->nabor.begin())->copy();
			A->initUP(NULL);
			}
		delete S;
		Bloc*X = table[ML->a];
		X->A = A;
		}

	sxema->HeadEnd = table[BeginNumber];
	if(sxema->HeadEnd->A)delete sxema->HeadEnd->A;
	sxema->HeadEnd->A = NULL;
	if(!sxema->controlCorrectSxema()){
		sxema->HeadEnd = NULL;
		M_IB::iterator it = table.begin();
		for(;it!=table.end();++it)delete it->second;
		return 0;
		}
	S_B S;
	sxema->HeadEnd->getAll(S);
	M_IB::iterator it2 = table.begin();
	for(;it2!=table.end();++it2){
		ok = find(S.begin(),S.end(),it2->second)==S.end();
		if(ok)delete it2->second;
		}
	return 1;
}




//--------------------------------------------------------------------------------------------------
bool Function::isAccesibleSub(int pos1,int pos2,S_I&SI){
	controlSxema();
	sxema->controlTable();
	sxema->controlRang();
	Bloc*A,*B;
	A=sxema->table[pos1];
	B=sxema->table[pos2];
	if(!A || !B)return 0;
	if(A==sxema->HeadEnd || B==sxema->HeadEnd)return 0;
	if(A->rang>B->rang){
		B=sxema->table[pos1];
		A=sxema->table[pos2];
		}
	if(B->down.size()!=1)return 0;
	Bloc*P,*X;
	V_B VB;
	VB.push_back(A);
	bool firstUnit = 1;
	while(!VB.empty()){
		P=*(VB.end()-1);
		VB.pop_back();
		if(!SI.insert(P->accessNumber).second)continue;
		if(P==sxema->HeadEnd)return 0;
		int i,si;
		if(!firstUnit){
			si=P->up.size();
			for(i=0;i<si;++i){
				X=P->up[i];
				if(X->rang<A->rang)return 0;
				VB.push_back(X);
				}
			}
		firstUnit = 0;
		if(P==B)continue;
		si=P->down.size();
		for(i=0;i<si;++i){
			X=P->down[i];
			if(X->rang<=A->rang)return 0;
			if(X->rang>B->rang)return 0;
			VB.push_back(X);
			}
		}
	return 1;
}



Sxema* Function::getSubSxema(const S_I&SI){
	controlSxema();
	sxema->controlTable();
	sxema->controlRang();
	Sxema*sxema2 = new Sxema();
	sxema2->HeadEnd = new Bloc();
	V_B VB;
	Bloc*WA=NULL,*WB=NULL;
	S_I::const_iterator tt,jt,it=SI.begin();
	for(;it!=SI.end();++it){
		Bloc*X=sxema->table[*it];
		if(!WA)WA=WB=X;
		if(WA->rang>X->rang)WA=X;
		if(WB->rang<X->rang)WB=X;
		Bloc*B=new Bloc();
		if(X->A){
			B->A = X->A->copy();
			B->A->initUP(NULL);
			}
		VB.push_back(B);
		}
	int i,si,pos,posi;
	it=SI.begin();
	for(posi=0;it!=SI.end();++it,++posi){
		Bloc*A=VB[posi];
		Bloc*X=sxema->table[*it];
		if(X==WA)WA=A;
		if(X==WB)WB=A;
		si=X->down.size();
		for(i=0;i<si;++i){
			Bloc*B=X->down[i];
			jt=SI.find(B->accessNumber);
			if(jt==SI.end())continue;
			tt=SI.begin();
			for(pos=0;tt!=jt;++tt,++pos);
			B=VB[pos];
			A->down.push_back(B);
			B->up.push_back(A);
			}
		}
	sxema2->HeadEnd->down.push_back(WA);
	WA->up.push_back(sxema2->HeadEnd);
	sxema2->HeadEnd->up.push_back(WB);
	WB->down.push_back(sxema2->HeadEnd);
	return sxema2;
}




void Function::deleteSubTree(int pos1,int pos2,const S_I&SI){
	controlSxema();
	sxema->controlTable();
	sxema->controlRang();
	Bloc*A,*B,*C;
	A=sxema->table[pos1];
	B=sxema->table[pos2];
	if(A->rang>B->rang){
		B=sxema->table[pos1];
		A=sxema->table[pos2];
		}
	C=B->down[0];
	C->up.erase(find(C->up.begin(),C->up.end(),B));
	if(A->A){
		delete A->A;
		A->A=NULL;
		}
	A->down.clear();
	A->down.push_back(C);
	C->up.push_back(A);
	sxema->table.clear();
	if(file)file->NeedSave=1;
	//delete musor:
	S_I::const_iterator it=SI.begin();
	for(;it!=SI.end();++it){
		C=sxema->table[*it];
		if(A!=C)delete C;
		}
}



void Function::insertInUnit(int pos,Function*FIZ){
	controlSxema();
	sxema->controlTable();
	Bloc*X=sxema->table[pos];
	if(!X)return;
	if(X->down.size()!=1)return;
	FIZ->controlSxema();
	Sxema*SX=FIZ->sxema->copy();
	Bloc*B=SX->HeadEnd->down[0];
	if(B==SX->HeadEnd)return;
	if(X->A){
		delete X->A;
		X->A=NULL;
		}
	Bloc*X2=X->down[0];
	X->down.clear();
	X2->up.erase(find(X2->up.begin(),X2->up.end(),X));
	B->up.clear();
	SX->HeadEnd->down.clear();
	X->down.push_back(B);
	B->up.push_back(X);
	SX->HeadEnd->down.push_back(X2);
	X2->up.push_back(SX->HeadEnd);
	SX->HeadEnd=NULL;
	delete SX;
	if(file)file->NeedSave=1;
	sxema->table.clear();
	sxema->rangOK=0;
}



//--------------------------------------------------------------------------------------------------
//::A::B
string Assemble::getRowNAMES(char*&s){
	string R;
	string one;
	int dt=2,v;
	do{
		SCANER::noProbel(s);
		char*s2=s;
		v=0;
		if(SCANER::scanSlovo("::",s))v=1; else{
			int d=0;
			if(*s=='~'){
				d=1;
				++s;
				SCANER::noProbel(s);
				}
			one=SCANER::readName(s);
			if(d)one="~"+one;
			}
		if(dt==v){
			s=s2;
			break;
			}
		dt=v;
		if(v)R+="::"; else {
			if(one.empty())break;
			R+=one;
			}
		}while(1);
	return R;
}



int Assemble::getSpisokArgs(V_TIP&tips,V_S&names,V_AL&defaults,char*&s){
	bool argumentsON=0;
	SCANER::noProbel(s);
	char*ss=s;
	while(*s!=')'){
		if(SCANER::scanSlovo("...",s)){
			argumentsON=1;
			SCANER::noProbel(s);
			if(*s==',')++s;else if(*s!=')'){s=ss;return 0;}
			SCANER::noProbel(s);
			continue;
			}
		Type t;
		string name;
		Algorithm*D=NULL;
		if(SCANER::scanSlovo("const",s)){
			char*s3=s;
			SCANER::noProbel(s);
			if(s>s3)t.isConst=1; else s=ss;
			}
		bool nfirst=0;
		string one;
		do{
			char*s3=s;
			one=SCANER::readName(s);
			SCANER::noProbel(s);
			if(*s==',' || *s==':' || *s==')' || *s=='='){s=s3;break;}
			if(nfirst)t.name+=" ";
			nfirst=1;
			t.name+=one;
			}while(*s!='*' && *s!='&');
		if(t.name.empty())t.name=getRowNAMES(s);
		do{
			SCANER::noProbel(s);
			if(*s=='*'){++t.n;++s;}else break;
			}while(1);
		SCANER::noProbel(s);
		if(*s=='&'){t.isAmpersent=1;++s;}
		SCANER::noProbel(s);
		name=SCANER::readName(s);
		SCANER::noProbel(s);
		if(*s=='='){
			++s;
			D=Razbor(s,0);
			if(!D)return 2;
			D->initUP(NULL);
			if(*(s-1)==',')--s;
			}
		if(name.empty()){
			name=t.name;
			t.name="var";
			t.n=0;
			t.isAmpersent=0;
			}
		tips.push_back(t);
		names.push_back(name);
		defaults.push_back(D);
		if(*s==',')++s;else if(*s!=')'){s=ss;return 0;}
		SCANER::noProbel(s);
		}
	++s;
	return argumentsON?10:1;// ok
}



Assemble::SX::SX(){B=NULL;}
Assemble::SX::~SX(){if(B)delete B;};


Sxema* Assemble::getSxema(char*&s){
	SCANER::noProbel(s);
	if(*s!='{')return NULL;
	++s;
	typedef map<int,SX> M_ISX;
	M_ISX table;
	int nstart=0,k;
	while(*s!='}'){
		SX sx;
		SCANER::noProbel(s);
		if(SCANER::scanSlovo("start:",s))nstart=0;
		SCANER::noProbel(s);
		string t=SCANER::readDigit(s);
		sscanf(t.c_str(),"%d",&sx.n);
		if(!nstart)nstart=sx.n;
		SCANER::noProbel(s);
		if(*s!=':')return NULL;
		++s;
		SCANER::noProbel(s);
		int er=0;
		sx.B=new(Bloc);
		if(*s==';')++s;else{
			Sequence S;
			sx.B->A=getONE(s,er,&S);
			if(er)return NULL;
			if(!sx.B->A && S.nabor.size())sx.B->A=(*S.nabor.begin())->copy();
			sx.B->A->initUP(NULL);
			}
		SCANER::noProbel(s);
		if(*s==';')++s;
		SCANER::noProbel(s);
		if(*s!='{')return NULL;
		++s;
		SCANER::noProbel(s);
		while(*s!='}'){
			t=SCANER::readDigit(s);
			sscanf(t.c_str(),"%d",&k);
			sx.next.push_back(k);
			SCANER::noProbel(s);
			if(*s==',')++s;
			SCANER::noProbel(s);
			}
		++s;
		SCANER::noProbel(s);
		table[sx.n]=sx;
		sx.B=NULL;
		}
	++s;
	SX*E;
	M_ISX::iterator it=table.begin();
	for(;it!=table.end();++it){
		SX*sx=&it->second;
		int i;
		for(i=0;i<sx->next.size();++i){
			sx->B->down.push_back(table[sx->next[i]].B);
			E=&table[sx->next[i]];
			E->B->up.push_back(sx->B);
			}
		}
	Sxema*SXE=new(Sxema);
	SXE->HeadEnd=table[nstart].B;
	//нужно отключить указатели таблици на блоки схемиы.
	it=table.begin();
	for(;it!=table.end();++it){
		SX*sx=&it->second;
		sx->B=NULL;
		}
	return SXE;
}



Function* Assemble::getFunction(char*&s,int&er,File*file){
	SCANER::noProbel(s);
	char*s2=s,*ss=s,*s3;
	while(*s2&&*s2!='(')++s2;
	if(*s2!='(')return NULL;
	bool virt=0,stat=0,isop=0,isSxema=0;
	if(SCANER::scanSlovo("SXEMA",s)){
		SCANER::noProbel(s);
		if(*s==':'){
			++s;
			isSxema=1;
			}
		}
	if(SCANER::scanSlovo("virtual",s)){
		s3=s;
		SCANER::noProbel(s);
		if(s>s3)virt=1; else s=s3;
		}
	if(SCANER::scanSlovo("static",s)){
		s3=s;
		SCANER::noProbel(s);
		if(s>s3)stat=1; else s=s3;
		}
	Type t;
	if(SCANER::scanSlovo("const",s)){
		s3=s;
		SCANER::noProbel(s);
		if(s>s3)t.isConst=1; else s=ss;
		}
	// unsigned long int
	string one;
	bool nfirst=0;
	bool argumentsON=0;
	do{
		char*s3=s;
		one=SCANER::readName(s);
		if(one.empty())
			if(t.name.empty())break; else {s=ss;return NULL;}
		SCANER::noProbel(s);
		if(*s=='(' || *s==':'){s=s3;break;}
		if(nfirst)t.name+=" ";
		nfirst=1;
		t.name+=one;
		}while(*s!='*' && *s!='&' && *s!='~');
	string row;
	if(t.name.empty()){
		// ::dsg::tre::x
		one=getRowNAMES(s);
		SCANER::noProbel(s);
		if(*s=='(')row=one; else t.name=one;
		}
	do{
		SCANER::noProbel(s);
		if(*s=='*'){++t.n;++s;}else break;
		}while(1);
	SCANER::noProbel(s);
	if(*s=='&'){t.isAmpersent=1;++s;}
	SCANER::noProbel(s);
	if(t.name.empty())t.name="int";
	if(row.empty())row=getRowNAMES(s);
	int i=row.find_last_of(':');
	one=row.substr(i+1);
	//if(i>0)row=row.substr(0,i-1);else row="";
	if(one.empty()){
		s=ss;
		return NULL;
		}
	SCANER::noProbel(s);
	//if(isDes)one.erase(0,1);
	if(one=="operator"){
		isop=1;
		char*r=oneterm::getSTerm(s);
		if(r)one=r;
		SCANER::noProbel(s);
		}
	if(*s!='('){
		if(isop){
			er=1;
			bool isOK = Main->GoErrorMessage(
				PHTML,
				"EZ001",
				"",
				SCANER::toString(SCANER::findNumberStringLine(this->F.text,s))
				);
			if(!isOK){
				PHTML+="<font class='red'>";
				PHTML+="Обявлен недопустимый оператор в строке: ";
				PHTML+=SCANER::toString(SCANER::findNumberStringLine(this->F.text,s));
				PHTML+="</font><br/>\n";
				}
			}
		s=ss;
		return NULL;
		}
	++s;
	V_TIP	tips;
	V_S		names;
	V_AL	defaults;
	i=getSpisokArgs(tips,names,defaults,s);
	if(i==10)argumentsON=1;
	if(i==2){
		er=1;
		for(i=0;i<defaults.size();++i)
			if(defaults[i])delete defaults[i];
		i=0;
		}
	if(!i){
		s=ss;
		return NULL;
		}
	SCANER::noProbel(s);
	SCANER::scanSlovo("const",s);
	SCANER::noProbel(s);
	i=0;
	if(*s=='='){
		++s;
		SCANER::noProbel(s);
		if(*s!='0')i=1;
		++s;
		SCANER::noProbel(s);
		if(*s!=';')i=1;
		++s;
		if(!i)i=3;
		}else if(*s==';'){i=3;++s;}
	if(i==1){
		er=1;
		bool isOK = Main->GoErrorMessage(
			PHTML,
			"EF002",
			"",
			SCANER::toString(SCANER::findNumberStringLine(F.text,ss))
			);
		if(!isOK){
			PHTML+="<font class='red'>";
			PHTML+="Объявление функции должно заканчиваться символом ';' в строке: ";
			PHTML+=SCANER::toString(SCANER::findNumberStringLine(F.text,ss));
			PHTML+="</font><br/>\n";
			}
		return NULL;
		}
	Function*F=file->FindFunction(row,1);
		//Space->FindFunction(one,&tips,1);
	if(virt)F->isVirtual=1;
	if(stat)F->isStatic=1;
	F->isOperator=isop;
	F->ret=t;
	F->tips=tips;
	if(F->names.empty())F->names=names;else{
		int j;
		for(j=0;j<names.size();++j)
			if(!names[j].empty())F->names[j]=names[j];
		}
	if(F->defaults.empty())F->defaults=defaults;else{
		int j;
		for(j=0;j<defaults.size();++j){
			if(!F->defaults[j] && defaults[j]){
				F->defaults[j]=defaults[j];
				continue;
				}
			if(F->defaults[j] && defaults[j]){
				delete F->defaults[j];
				F->defaults[j]=defaults[j];
				}
			}
		}
	if(i)return F;
	if(F->Body){
		er=1;
		bool isOK = Main->GoErrorMessage(
			PHTML,
			"ER003",
			F->getHead().c_str(),
			SCANER::toString(SCANER::findNumberStringLine(this->F.text,ss))
			);
		if(!isOK){
			PHTML+="<font class='red'>";
			PHTML+="Функция \"";PHTML+=F->getHead();
			PHTML+="\" уже задана. Не должно быть повторного обявления заданой функции в строке: ";
			PHTML+=SCANER::toString(SCANER::findNumberStringLine(this->F.text,ss));
			PHTML+="</font><br/>\n";
			}
		return NULL;
		}
	SCANER::noProbel(s);
	Sequence SE;
	if(*s==':'){
		do{
			++s;
			SCANER::noProbel(s);
			CreateVar cv;
			cv.isInit=1;
			cv.name=SCANER::readName(s);
			if(cv.name.empty())er=1;
			SCANER::noProbel(s);
			if(*s!='(')er=1;
			++s;
			if(!er)while(*s!=')'){
				SCANER::noProbel(s);
				Algorithm*A=Razbor(s,0);
				if(A)cv.params.push_back(A);else er=2;
				SCANER::noProbel(s);
				if(*s==',')++s;
				}
			SE.nabor.push_back(cv.copy());
			if(er){
				bool isOK = Main->GoErrorMessage(
					PHTML,
					"ES004",
					"",
					SCANER::toString(SCANER::findNumberStringLine(this->F.text,ss))
					);
				if(!isOK){
					PHTML+="<font class='red'>";
					PHTML+="Синтаксическая ошибка в заголовке функции в строке: ";
					PHTML+=SCANER::toString(SCANER::findNumberStringLine(this->F.text,ss));
					PHTML+=". Пример: f(int i):a(i),p(i,i){}</font><br/>\n";
					}
				return NULL;
				}
			++s;
			SCANER::noProbel(s);
			}while(*s==',');
		}
	SCANER::noProbel(s);
	if(*s!='{')er=1;
	if(isSxema){
		F->sxema=getSxema(s);
		if(F->sxema)F->sxema->f=F;
		if(!F->sxema){
			bool isOK = Main->GoErrorMessage(
				PHTML,
				"EF005",
				"",
				SCANER::toString(SCANER::findNumberStringLine(this->F.text,ss))
				);
			if(!isOK){
				PHTML+="<font class='red'>";
				PHTML+="Ошибка в теле функции-схемы в строке: ";
				PHTML+=SCANER::toString(SCANER::findNumberStringLine(this->F.text,ss));
				PHTML+="</font><br/>\n";
				}
			}
		return F;
		}
	F->Body=getNabor(s,er);
	if(F->Body){
		Sequence*SBody=dynamic_cast<Sequence*>(F->Body);
		L_AL::reverse_iterator it=SE.nabor.rbegin();
		for(;it!=SE.nabor.rend();++it){
			SBody->nabor.insert(SBody->nabor.begin(),*it);
			*it=NULL;
			}
		}else F->Body=SE.copy();
	if(F->Body)F->Body->initUP(NULL);
	if(er){
		bool isOK = Main->GoErrorMessage(
			PHTML,
			"EF006",
			"",
			SCANER::toString(SCANER::findNumberStringLine(this->F.text,ss))
			);
		if(!isOK){
			PHTML+="<font class='red'>";
			PHTML+="Ошибка в теле функции в строке: ";
			PHTML+=SCANER::toString(SCANER::findNumberStringLine(this->F.text,ss));
			PHTML+="</font><br/>\n";
			}
		}
	F->argumentsON=argumentsON;
	return F;
}




CLASS* Assemble::getClass(char*&s,int&er,File*ff,CLASS*Space){
	if(!Space)Space=GlobalSpace;
	SCANER::noProbel(s);
	if(!SCANER::scanSlovo("class",s))return NULL;
	SCANER::noProbel(s);
	string str=SCANER::readName(s);
	Space->file=ff;
	CLASS*S=Space->FindSpace(str,1);
	SCANER::noProbel(s);
	if(*s==':'){
		++s;
		SCANER::noProbel(s);
		SCANER::scanSlovo("public",s);
		SCANER::noProbel(s);
		str=SCANER::readName(s);
		SCANER::noProbel(s);
		CLASS*P=Space->FindSpace(str,0);
		if(!P)P=GlobalSpace->FindSpace(str,1);
		S->Parent=P;
		}
	if(*s==';'){++s;return S;}
	if(*s!='{'){
		er=1;
		PHTML+="<font class='red'>";
		PHTML+="В описании класса нужна скобка '{' в строке: ";
		PHTML+=SCANER::toString(SCANER::findNumberStringLine(F.text,s));
		PHTML+="</font><br/>\n";
		return S;
		}
	++s;
	do{
		SCANER::noProbel(s);
		CLASS*Sub=getClass(s,er,ff,S);
		if(er)break;
		if(Sub)continue;
		Function*F=getFunction(s,er,ff);
		if(F){
			F->Space=S;
			/*
			if(F->Body)if(!F->file){
				F->file=ff;
				ff->functions.push_back(F);
				}
			*/
			continue;
			}
		if(er)break;
		if(probaVarRead(s,S->init.nabor)==2)er=1;
		if(er)break;
		SCANER::noProbel(s);
		SCANER::scanSlovo("public",s);
		SCANER::noProbel(s);
		SCANER::scanSlovo(":",s);
		SCANER::noProbel(s);
		}while(*s!='}');
	if(er){
		PHTML+="<font class='red'>";
		PHTML+="Синтаксическая ошибка в описании класса в строке: ";
		PHTML+=SCANER::toString(SCANER::findNumberStringLine(F.text,s));
		PHTML+="</font><br/>\n";
		return S;
		}
	++s;
	SCANER::noProbel(s);
	if(*s!=';'){
		er=2;
		PHTML+="<font class='red'>";
		PHTML+="Требуетса символ ';' после описания класса в строке: ";
		PHTML+=SCANER::toString(SCANER::findNumberStringLine(F.text,s));
		PHTML+="</font><br/>\n";
		}
	++s;
	S->init.initUP(NULL);
	return S;
}



// s - text program
int Assemble::Load(const char*way,char*s,bool nedsav){
	File*ff = lastFile = new File(Main);
	ff->Nusers = 1;
	ff->NeedSave = nedsav;
	char*ss; // *s=F.text,
	int er = 0;
	string sway=way;
	int a,b;
	a=sway.find_last_of('\\');
	ff->way=sway.substr(0,a);
	++a;
	b=sway.find_last_of('.');
	ff->name=sway.substr(a,b-a);
	SCANER::noProbel(s);
	CVARIANT K;
	K=(*GlobalSpace->Map.DATA.mapVal)["WAY"];
	K.TransformType("string");
	string WAY=*K.DATA.ps;
	sway=ff->way.substr(0,WAY.size());
	if(WAY==sway){
		if(ff->way.size()==WAY.size())ff->way="";else
			ff->way=ff->way.substr(WAY.size()+1);
		}
	while(*s){
		ss=s;
		if(SCANER::scanSlovo("#include",s)){
			SCANER::noProbel(s);
			bool nolocal=(*s=='<');
			++s;
			string str;
			while(*s && *s!='\n' && *s!='>' && *s!='"'){
				str+=*s;
				++s;
				}
			if(*s=='>' || *s=='"')++s;
			str = WAY+"\\"+str;
			ff->modules.push_back(str);
			}
		CLASS*S=getClass(s,er,ff);
		if(S){
			if(!S->file){
				S->file=ff;
				ff->Classes.push_back(S);
				}
			continue;
			}
		if(er)break;
		Function*f = getFunction(s,er,ff);
		if(er)break;
		if(ss==s){
			er=1;
			break;
			}
		SCANER::noProbel(s);
		}
	//cout<<ff->toString().c_str()<<endl;//
	if(er)ff->NeedSave = 0;
	return er;
}



//--------------------------------------------------------------------------------------------------
Algorithm* Algorithm::findLabel(int n){return NULL;}

Algorithm* Label::findLabel(int n){
	if(id==n)return this;
	return NULL;
}


Algorithm* Sequence::findLabel(int n){
	L_AL::iterator it=nabor.begin();
	for(;it!=nabor.end();++it){
		Algorithm*X=(*it)->findLabel(n);
		if(X)return X;
		}
	return NULL;
}


Algorithm* IF::findLabel(int n){
	Algorithm*X=NULL;
	if(A)A->findLabel(n);
	if(X)return X;
	if(E)X=E->findLabel(n);
	return X;
}


Algorithm* WHILE::findLabel(int n){
	return A?A->findLabel(n):NULL;
}


Algorithm* FOR::findLabel(int n){
	return B?B->findLabel(n):NULL;
}



// assemble2.cpp	:-|