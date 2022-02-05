// sxema.cpp
#include "main.h"


Bloc::Bloc(){
	A=NULL;
	rang=0;
}


Bloc::~Bloc(){if(A)delete A;}


void Bloc::getAll(S_B&S){
	if(this==NULL)return;
	if(!S.insert(this).second)return;
	Bloc*X=this;
	while(X->down.size()==1){
		if(X->down[0]->up.size()==1){
			X=X->down[0];
			if(!S.insert(X).second)return;
			}else break;
		}
	int i;
	for(i=0;i<X->down.size();++i)X->down[i]->getAll(S);
	for(i=0;i<X->up.size();++i)X->up[i]->getAll(S);
}


void Bloc::disconnectUp(){
	int i;
	for(i=0;i<up.size();++i){
		Bloc*D=up[i];
		D->down.erase(find(D->down.begin(),D->down.end(),this));
		}
	up.clear();
}



void Bloc::disconnectDown(){
	int i;
	for(i=0;i<down.size();++i){
		Bloc*D=down[i];
		D->up.erase(find(D->up.begin(),D->up.end(),this));
		}
	down.clear();
}



bool Bloc:: operator == (const Bloc&t){
	//if(this!=&t)return 0;
	if(!(*A==*t.A))return 0;
	return 1;
}


void Bloc::deleted(){ // delete one Bloc
	V_B xup=up,xdown=down;
	//disconnectUp();
	disconnectDown();
	int i,j;
	for(i=0;i<xup.size();++i)if(xup[i]!=this)
	for(j=0;j<xdown.size();++j)if(xdown[j]!=this){
		V_B::iterator it=find(xup[i]->down.begin(),xup[i]->down.end(),this);
		if(it!=xup[i]->down.end())*it=xdown[j];	else 
			xup[i]->down.push_back(xdown[j]);
		xdown[j]->up.push_back(xup[i]);
		}
	up.clear();
}



bool Bloc::compareTree(const Bloc*t,const Bloc*HE) const {
	if(((bool)A)!=((bool)t->A))return 0;
	if(A)if(!(*A==*t->A))return 0;
	if(t->down.size()!=down.size())return 0;
	if(HE==this)return 1;
	if(!HE)HE=this;
	bool ok,ok2;
	int i;
	for(i=0;i<down.size();++i){
		ok=down[i]->rang>rang;
		ok2=t->down[i]->rang>t->rang;
		if(ok!=ok2)return 0;
		ok=down[i]->up.size();
		ok2=t->down[i]->up.size();
		if(!(ok&&ok2))return 0;
		ok=(down[i]->up[0]==this);
		ok2=(t->down[i]->up[0]==t);
		if(ok!=ok2)return 0;
		if(ok)if(!down[i]->compareTree(t->down[i],HE))return 0;
		}
	return 1;
}



bool Bloc::Less(const Bloc*t,const Bloc*HE) const {
	if(((bool)A)!=((bool)t->A))return A > t->A;
	if(A)if(!(*A==*t->A))return *A > *t->A;
	if(t->down.size()!=down.size())return down.size() > t->down.size();
	if(!HE)HE=this;
	bool ok,ok2;
	int a,b;
	int i;
	for(i=0;i<down.size();++i){
		ok=down[i]->rang>rang;
		ok2=t->down[i]->rang>t->rang;
		if(ok!=ok2)return ok > ok2;
		a=down[i]->up.size();
		b=t->down[i]->up.size();
		if(a!=b)return a > b;
		if(!a)continue;
		ok=(down[i]->up[0]==this);
		ok2=(t->down[i]->up[0]==t);
		if(ok!=ok2)return ok > ok2;
		if(!ok)continue;
		if(!down[i]->compareTree(t->down[i],HE))return down[i]->Less(t->down[i],HE);
		}
	return 0;
}


string Bloc::toString(){
	string s;
	if(A)s=A->toString();
	return s;
}


void Bloc::terminateValey(const Bloc*Stop){
	if(this==Stop)return;
	bool upcount=0;
	int i;
	for(i=0;i<up.size();++i)
		if(rang>up[i]->rang)++upcount;
	if(upcount)return;
	disconnectUp();
	V_B D;
	D.swap(down);
	for(i=0;i<D.size();++i)D[i]->up.erase(find(D[i]->up.begin(),D[i]->up.end(),this));
	for(i=0;i<D.size();++i)D[i]->terminateValey(Stop);
	delete this;
}


//--------------------------------------------------------------------------------------------------
Sxema::Sxema(){
	rangOK=0;
	HeadEnd=NULL;
	f=NULL;
	NLabel=0;
}

Sxema::~Sxema(){ClearSxema();}

void Sxema::ClearSxema(){
	S_B S;
	HeadEnd->getAll(S);
	S_B::iterator it=S.begin();
	for(;it!=S.end();++it)delete *it;
	HeadEnd=NULL;
}


int Sxema::getNextNLabel(){
	if(NLabel)return ++NLabel;
	S_B S;
	HeadEnd->getAll(S);
	S_B::iterator it=S.begin();
	for(;it!=S.end();++it){
		Label*L=dynamic_cast<Label*>((*it)->A);
		if(L)if(L->id>NLabel)NLabel=L->id;
		}
	return ++NLabel;
}


void Sxema::controlRang(){
	if(rangOK)return;
	setRang();
	rangOK=1;
}


void Sxema::clearRang(){
	S_B S;
	HeadEnd->getAll(S);
	S_B::iterator it=S.begin();
	for(;it!=S.end();++it)(*it)->rang=0;
}


void Sxema::setRang(int rang,Bloc*X,S_B*S){
	if(!S){
		if(!HeadEnd)return;
		clearRang();
		S_B SS;
		setRang(1,HeadEnd,&SS);
		return;
		}
	if(!S->insert(X).second)return;
	V_B VB;
	VB.push_back(X);
	if(X->rang<rang){
		X->rang=rang;
		++rang;
		bool ok=1;
		while(X->down.size()==1){
			X=X->down[0];
			if(X->rang>=rang){ok=0;break;}
			if(!S->insert(X).second){ok=0;break;}
			X->rang=rang;
			++rang;
			VB.push_back(X);
			}
		if(ok){
			V_B::iterator it=X->down.begin();
			for(;it!=X->down.end();++it)setRang(rang,*it,S);
			}
		}
	V_B::iterator it=VB.begin();
	for(;it!=VB.end();++it)S->erase(*it);
}



void Sxema::insertPagesForGoto(Bloc*X,int raznica){
	X->A=NULL;
	SpecSumbol*ss;
	SpecSumbol2*S2;
	Bloc*N;
	if(raznica>0){
		if(raznica>1){
			S2=new(SpecSumbol2);
			Base*B=new(Base);
			B->type=3;//digit
			B->text=SCANER::toString(raznica);
			S2->X=B;
			ss=S2;
			}else ss=new(SpecSumbol);
		ss->n=5;//PAGECLOSE
		X->A=ss;
		}
	while(raznica<0){
		N=new(Bloc);
		ss=new(SpecSumbol);
		ss->n=6;//PAGEOPEN
		N->A=ss;
		AddDown(X,N);
		++raznica;
		}
}


void Sxema::ToSxema(Algorithm*Q){
	ClearSxema();
	HeadEnd=new(Bloc);
	Bloc*N=new(Bloc);
	N->up.push_back(HeadEnd);
	HeadEnd->down.push_back(N);
	N->down.push_back(HeadEnd);
	HeadEnd->up.push_back(N);
	V_B Goto;
	M_IB mib;
	V_B Cont,Break;
	V_I CycleAndOpenPage;
	OneToSxema(CycleAndOpenPage,Goto,mib,N,Q,&Cont,&Break);
	int i;
	for(i=0;i<Goto.size();++i){//Big problems
		SpecSumbol2*S2=dynamic_cast<SpecSumbol2*>(Goto[i]->A);
		Base*B=dynamic_cast<Base*>(S2->X);
		if(B)if(B->type==3){//is digit
			int n;
			sscanf(B->text.c_str(),"%d",&n);
			if(mib.find(n)==mib.end())continue;
			int raznica=getCountPagesOpen(Goto[i],mib[n]);
			Goto[i]->disconnectDown();
			Goto[i]->down.push_back(mib[n]);
			mib[n]->up.push_back(Goto[i]);
			delete Goto[i]->A;
			insertPagesForGoto(Goto[i],raznica);
			continue;
			}
		AlgoSet*as=dynamic_cast<AlgoSet*>(S2->X);
		if(as){
			V_I Adr,Ra;
			int n;
			int j;
			for(j=0;j<as->nabor.size();++j){
				B=dynamic_cast<Base*>(as->nabor[j]);
				if(!B)continue;
				if(B->type!=3)continue;
				sscanf(B->text.c_str(),"%d",&n);
				if(mib.find(n)==mib.end())continue;
				Adr.push_back(n);
				}
			if(Adr.empty())continue;
			for(j=0;j<Adr.size();++j){
				n=Adr[j];
				Ra.push_back(getCountPagesOpen(Goto[i],mib[n]));
				}
			Goto[i]->disconnectDown();
			for(j=0;j<Adr.size();++j){
				n=Adr[j];
				N=new(Bloc);
				N->down.push_back(mib[n]);
				N->up.push_back(Goto[i]);
				Goto[i]->down.push_back(N);
				mib[n]->up.push_back(N);
				insertPagesForGoto(N,Ra[j]);
				}
			}
		delete Goto[i]->A;
		Goto[i]->A=NULL;
		}
}



void Sxema::getGraph(UGraf*UG){
	controlTable();
	NetLine*net;
	MarkerLine*marker;
	marker = new MarkerLine(freeN,0,"",0,"End");
	UG->add(marker);
	int size,n;
	M_IB::iterator it = table.begin();
	for(;it!=table.end();++it){
		Bloc*B=it->second,*B2;
		size = B->down.size();
		if(size==1){
			B2=B->down[0];
			n=B2->accessNumber;
			if(n==HeadEnd->accessNumber)n=freeN;
			net = new NetLine(B->accessNumber,0,"",1,n);
			UG->add(net);
			}
		if(size==2){
			B2=B->down[0];
			n=B2->accessNumber;
			if(n==HeadEnd->accessNumber)n=freeN;
			net = new NetLine(B->accessNumber,0,"+",1,n);
			UG->add(net);
			B2=B->down[1];
			n=B2->accessNumber;
			if(n==HeadEnd->accessNumber)n=freeN;
			net = new NetLine(B->accessNumber,0,"-",1,n);
			UG->add(net);
			}
		if(!B->A)continue;
		marker = new MarkerLine(B->accessNumber,0,"",0,B->A->toString().c_str());
		UG->add(marker);
		}
	marker = new MarkerLine(HeadEnd->accessNumber,0,"",0,"Begin");
	UG->add(marker);
}



void Sxema::AddUp(Bloc*P,Bloc*N){
	int i;
	for(i=0;i<P->up.size();++i){
		Bloc*T=P->up[i];
		*find(T->down.begin(),T->down.end(),P)=N;
		N->up.push_back(T);
		}
	P->up.clear();
	P->up.push_back(N);
	N->down.push_back(P);
}



void Sxema::AddDown(Bloc*P,Bloc*N){
	int i;
	for(i=0;i<P->down.size();++i){
		Bloc*T=P->down[i];
		T->up.erase(find(T->up.begin(),T->up.end(),P));
		T->up.push_back(N);
		N->down.push_back(T);
		}
	P->down.clear();
	P->down.push_back(N);
	N->up.push_back(P);
}



void Sxema::OneToSxema(V_I&CycleAndOpenPage,V_B&Goto,M_IB&mib,Bloc*X,Algorithm*Q,V_B*Cont,V_B*Break){
	Label*L=dynamic_cast<Label*>(Q);
	if(L){
		mib[L->id]=X;
		X->A=L->copy();
		X->A->initUP(NULL);
		return;
		}
	SpecSumbol2*S2=dynamic_cast<SpecSumbol2*>(Q);
	if(S2)if(S2->n==2){//goto
		X->A=S2->copy();
		X->A->initUP(NULL);
		Goto.push_back(X);
		return;
		}
	SpecSumbol*U=dynamic_cast<SpecSumbol*>(Q);
	if(U)if(U->n==3 || U->n==4){
		int level=1;
		if(S2){
			Base*B=dynamic_cast<Base*>(S2->X);
			if(B)level=SCANER::getNumber(B->text,10);
			}
		if(level<1)level=1;
		if(level>Cont->size())level=Cont->size();
		int NneedClose=0,Ncycle=0;
		int i;
		for(i=CycleAndOpenPage.size()-1;i>=0;--i)if(CycleAndOpenPage[i])++NneedClose;else{
			++Ncycle;
			if(Ncycle>=level)break;
			}
		level=Cont->size()-level;
		if(NneedClose){
			SpecSumbol*UU;
			if(NneedClose>1){
				S2=new(SpecSumbol2);
				Base*B=new(Base);
				B->type=3;//digit
				B->text=SCANER::toString(NneedClose);
				S2->X=B;
				S2->X->initUP(NULL);
				UU=S2;
				}else UU=new(SpecSumbol);
			UU->n=5;//PAGECLOSE
			X->A=UU;
			}
		if(U->n==3){//continue
			if(Cont->size()>level){
				X->disconnectDown();
				X->down.push_back((*Cont)[level]);
				(*Cont)[level]->up.push_back(X);
				}
			}
		if(U->n==4){//break
			if(Break->size()>level){
				X->disconnectDown();
				X->down.push_back((*Break)[level]);
				(*Break)[level]->up.push_back(X);
				}
			}
		return;
		}
	FOR*For=dynamic_cast<FOR*>(Q);
	Sequence*S=dynamic_cast<Sequence*>(Q);
	IF*iif=dynamic_cast<IF*>(Q);
	WHILE*WH=dynamic_cast<WHILE*>(Q);
	if(!(S || iif || For || WH)){
		if(Q){
			X->A=Q->copy();
			X->A->initUP(NULL);
			}
		return;
		}
	if(S){
		typedef map<Bloc*,Algorithm*>	M_BA;
		M_BA table;
		Bloc*P=X,*H=X;
		bool NeedPage=0;
		L_AL::iterator it=S->nabor.begin();
		int f;
		for(f=1;it!=S->nabor.end();++it,f=0){
			if(!*it)continue;
			CreateVar*cv=dynamic_cast<CreateVar*>(*it);
			if(cv)NeedPage=1;
			Bloc*N=f?X:new(Bloc);
			table[N]=*it;
			if(f)continue;
			AddDown(P,N);
			P=N;
			}
		//mib[]=;
		if(NeedPage){
			Bloc*N=new(Bloc);
			SpecSumbol*ss=new(SpecSumbol);
			ss->n=6;//PAGEOPEN
			N->A=ss;
			AddUp(H,N);
			H=N;
			N=new(Bloc);
			ss=new(SpecSumbol);
			ss->n=5;//PAGECLOSE
			N->A=ss;
			AddDown(P,N);
			P=N;
			CycleAndOpenPage.push_back(1);
			}
		M_BA::iterator jt=table.begin();
		for(;jt!=table.end();++jt)OneToSxema(CycleAndOpenPage,Goto,mib,jt->first,jt->second,Cont,Break);
		if(NeedPage)CycleAndOpenPage.pop_back();
		return;
		}
	if(iif){
		IF*w=new(IF);
		w->X=iif->X->copy();
		w->X->initUP(NULL);
		X->A=w;
		if(X->down.size()>1){
			Bloc*N=new(Bloc);
			AddDown(X,N);
			}
		Bloc*P=X->down[0];
		if(iif->A){
			Bloc*N=new(Bloc);
			AddDown(X,N);
			OneToSxema(CycleAndOpenPage,Goto,mib,N,iif->A,Cont,Break);
			}
		if(iif->E){
			Bloc*N=new(Bloc);
			X->down.push_back(N);
			N->up.push_back(X);
			N->down.push_back(P);
			P->up.push_back(N);
			OneToSxema(CycleAndOpenPage,Goto,mib,N,iif->E,Cont,Break);
			}else{
			X->down.push_back(P);
			P->up.push_back(X);
			}
		return;
		}
	if(WH){
		Bloc*N;
		if(WH->n==1){// while
			if(X->down.size()>1){
				N=new(Bloc);
				AddDown(X,N);
				}
			IF*w=new(IF);
			w->X=WH->X->copy();
			w->X->initUP(NULL);
			X->A=w;
			X->down.push_back(X->down[0]);
			N=new(Bloc);
			X->down[0]=N;
			N->up.push_back(X);
			N->down.push_back(X);
			X->up.push_back(N);
			Cont->push_back(X);
			Break->push_back(X->down[1]);
			CycleAndOpenPage.push_back(0);
			OneToSxema(CycleAndOpenPage,Goto,mib,N,WH->A,Cont,Break);
			CycleAndOpenPage.pop_back();
			Cont->pop_back();
			Break->pop_back();
			}
		if(WH->n==2){// do
			if(X->down.size()>1){
				N=new(Bloc);
				AddDown(X,N);
				}
			N=new(Bloc);
			AddDown(X,N);
			N->down.push_back(N->down[0]);
			N->down[0]=X;
			X->up.push_back(N);
			IF*w=new(IF);
			w->X=WH->X->copy();
			w->X->initUP(NULL);
			N->A=w;
			Cont->push_back(N);
			Break->push_back(N->down[1]);
			CycleAndOpenPage.push_back(0);
			OneToSxema(CycleAndOpenPage,Goto,mib,X,WH->A,Cont,Break);
			CycleAndOpenPage.pop_back();
			Cont->pop_back();
			Break->pop_back();
			}
		return;
		}
	FOR*F=dynamic_cast<FOR*>(Q);
	if(F){
		Bloc*N,*N2;
		if(X->down.size()>1){
			N=new(Bloc);
			AddDown(X,N);
			}
		IF*w=new(IF);
		w->X=F->X->copy();
		w->X->initUP(NULL);
		X->A=w;
		X->down.push_back(X->down[0]);
		N=new(Bloc);
		X->down[0]=N;
		N->up.push_back(X);
		N->down.push_back(X);
		X->up.push_back(N);
		if(F->B && F->P){
			N2=new(Bloc);
			AddDown(N,N2);
			Cont->push_back(N2);
			Break->push_back(X->down[1]);
			CycleAndOpenPage.push_back(0);
			OneToSxema(CycleAndOpenPage,Goto,mib,N,F->B,Cont,Break);
			CycleAndOpenPage.pop_back();
			Cont->pop_back();
			Break->pop_back();
			OneToSxema(CycleAndOpenPage,Goto,mib,N2,F->P,Cont,Break);
			return;
			}
		if(F->B){
			Cont->push_back(X);
			Break->push_back(X->down[1]);
			CycleAndOpenPage.push_back(0);
			OneToSxema(CycleAndOpenPage,Goto,mib,N,F->B,Cont,Break);
			CycleAndOpenPage.pop_back();
			Cont->pop_back();
			Break->pop_back();
			}
		if(F->P)OneToSxema(CycleAndOpenPage,Goto,mib,N,F->P,Cont,Break);
		}
}


bool Sxema::findWayOpenPages(Bloc*P,Bloc*E,V_B&VB){
	if(P==HeadEnd)if(!VB.empty())return 0;
	if(P==E)return 1;
	V_B::iterator it=VB.begin();
	for(;it!=VB.end();++it)if(P==*it)return 0;
	VB.push_back(P);
	it=P->down.begin();
	for(;it!=P->down.end();++it)if(findWayOpenPages(*it,E,VB))return 1;
	VB.pop_back();
	return 0;
}


int Sxema::findOpenPagesTo(Bloc*A){
	V_B VB;
	findWayOpenPages(HeadEnd,A,VB);
	int n=0;
	V_B::iterator it=VB.begin();
	for(;it!=VB.end();++it){
		SpecSumbol*ss=dynamic_cast<SpecSumbol*>((*it)->A);
		if(!ss)continue;
		if(ss->n==6){//PAGEOPEN
			++n;
			}
		if(ss->n==5){//PAGECLOSE
			int t=1;
			SpecSumbol2*ss2=dynamic_cast<SpecSumbol2*>(ss);
			if(ss2){
				Base*Z=dynamic_cast<Base*>(ss2->X);
				if(Z)if(Z->type==3)sscanf(Z->text.c_str(),"%d",&t);
				}
			n-=t;
			}
		}
	return n;
}



int Sxema::getCountPagesOpen(Bloc*A,Bloc*B){
	int a,b;
	a=findOpenPagesTo(A);
	b=findOpenPagesTo(B);
	return a-b;
}


//--------------------------------------------------------------------------------------------------
void Sxema::diveLineEnd(Bloc*A,Bloc*B){ // for normalize algorithm
	typedef vector<V_B::iterator> VVB;
	VVB PenultimateIts;
	V_B Penultimates;
	V_B U;
	S_B S;
	bool needOne=0;
	if(A==B){
		Penultimates.push_back(A);
		PenultimateIts.push_back(A->down.begin());
		needOne=1;
		}else U.push_back(A);
	while(!U.empty()){
		A=*U.begin();
		U.erase(U.begin());
		if(!S.insert(A).second)continue;
		V_B::iterator it=A->down.begin();
		for(;it!=A->down.end();++it)if(*it==B){
			PenultimateIts.push_back(it);
			Penultimates.push_back(A);
			}else U.push_back(*it);
		}
	if(Penultimates.size()==1 && !needOne)return;
	Bloc*R=new(Bloc);
	int i;
	for(i=0;i<Penultimates.size();++i){
		B->up.erase(find(B->up.begin(),B->up.end(),Penultimates[i]));
		(*PenultimateIts[i])=R;
		R->up.push_back(Penultimates[i]);
		}
	R->down.push_back(B);
	B->up.push_back(R);
}



Bloc* Sxema::findPenultimate(Bloc*A,Bloc*B,V_B*U){ // for one Penultimate A-->(Return)->B
	if(!U){
		Bloc*R;
		if(A==B){
			R=new(Bloc);
			A->down[1]=R;
			R->up.push_back(A);
			A->up.erase(find(A->up.begin(),A->up.end(),A));
			R->down.push_back(A);
			A->up.push_back(R);
			return R;
			}
		U=new(V_B);
		R=findPenultimate(A,B,U);
		delete U;
		IF*iif=dynamic_cast<IF*>(R->A);
		if(iif){
			A=R;
			R=new(Bloc);
			int i;
			for(i=0;i<A->down.size();++i)if(A->down[i]==B){
				A->down[i]=R;
				R->up.push_back(A);
				B->up.erase(find(B->up.begin(),B->up.end(),A));
				}
			B->up.push_back(R);
			R->down.push_back(B);
			}
		return R;
		}
	int i;
	for(i=0;i<U->size();++i)if((*U)[i]==A)return NULL;
	U->push_back(A);
	for(i=0;i<A->down.size();++i){
		if(A->down[i]==B)return A;
		Bloc*R=findPenultimate(A->down[i],B,U);
		if(R)return R;
		}
	U->pop_back();
	return NULL;
}



string Sxema::toString(bool rangs){
	controlRang();
	S_B S;
	HeadEnd->getAll(S);
	controlTable();
	string R;
	S_B::iterator it=S.begin();
	for(;it!=S.end();++it){
		if(*it==HeadEnd)R+="start:";
		R+="	";
		R+=SCANER::toString((*it)->accessNumber*10);
		R+=":";
		if((*it)->A)R+=(*it)->A->toString();
		if(R[R.size()-1]!=';')R+=";";
		R+="{";
		V_B::iterator jt=(*it)->down.begin();
		int f;
		for(f=0;jt!=(*it)->down.end();++jt,++f){
			if(f)R+=",";
			R+=SCANER::toString((*jt)->accessNumber*10);
			}
		R+="}";
		if(rangs){
			R+=" // ";
			R+=SCANER::toString((*it)->rang);
			}
		R+="\n";
		}
	if(!controlCorrectSxema())cout<<"incorrect sxema!!!";
	return R;
}



void Sxema::OptimizeFree(){
	controlRang();
	bool ok=1;
	while(ok){
		while(ok){
			ok=0;
			S_B S;
			HeadEnd->getAll(S);
			S_B::iterator it=S.begin();
			for(;it!=S.end();++it){
				Bloc*X=*it;
				if(X->A || X->rang<2)continue;
				if(X->down.size()!=1)continue;
				Bloc*D=X->down[0];
				X->A=D->A;
				D->A=NULL;
				V_B::iterator jt;
				int i;
				for(i=0;i<D->down.size();++i){
					Bloc*D2=D->down[i];
					jt=find(D2->up.begin(),D2->up.end(),D);
					*jt=X;
					}
				X->down=D->down;
				D->up.erase(find(D->up.begin(),D->up.end(),X));
				for(i=0;i<D->up.size();++i){
					Bloc*U=D->up[i];
					jt=find(U->down.begin(),U->down.end(),D);
					*jt=X;
					X->up.push_back(U);
					}
				delete D;
				ok=1;
				break;
				}
			}
		S_B S;
		HeadEnd->getAll(S);
		S_B::iterator it=S.begin();
		for(;it!=S.end();++it){
			Bloc*X=*it;
			if(X->down.size()<2)continue;
			if(dynamic_cast<IF*>(X->A))continue;
			int i,j;
			for(i=0;i<X->down.size();++i)
				for(j=i+1;j<X->down.size();++j)if(X->down[i]==X->down[j]){
					Bloc*D=X->down[j];
					D->up.erase(find(D->up.begin(),D->up.end(),X));
					X->down.erase(&X->down[j]);
					--j;
					ok=1;
					}
			}
		}
	rangOK=0;
}



//--------------------------------------------------------------------------------------------------
void Sxema::controlTable(){
	if(table.size())return;
	S_B S;
	HeadEnd->getAll(S);
	S_B::iterator it=S.begin();
	int i;
	for(i=1;it!=S.end();++it,++i){
		table[i]=*it;
		(*it)->accessNumber=i;
		}
	freeN=++i;
}


bool Sxema:: operator == (Sxema&t){
	t.controlRang();
	controlRang();
	return HeadEnd->compareTree(t.HeadEnd);
}


//--------------------------------------------------------------------------------------------------
Sxema* Sxema::copy(){
	S_B S;
	HeadEnd->getAll(S);
	M_BB table;
	S_B::iterator it=S.begin();
	for(;it!=S.end();++it){
		Bloc*B=new Bloc();
		B->A=NULL;
		if((*it)->A){
			B->A=(*it)->A->copy();
			B->A->initUP(NULL);
			}
		table[*it]=B;
		}
	Sxema*R=new Sxema();
	it=S.begin();
	for(;it!=S.end();++it){
		Bloc*B=table[*it];
		V_B::iterator jt=(*it)->down.begin();
		for(;jt!=(*it)->down.end();++jt)B->down.push_back(table[*jt]);
		jt=(*it)->up.begin();
		for(;jt!=(*it)->up.end();++jt)B->up.push_back(table[*jt]);
		}
	R->HeadEnd=table[HeadEnd];
	return R;
}



//--------------------------------------------------------------------------------------------------
bool Sxema::controlCorrectSxema(){
	S_B S;
	HeadEnd->getAll(S);
	S_B::iterator it=S.begin();
	for(;it!=S.end();++it){
		Bloc*B=*it;
		if(B->down.empty())return 0;
		int i;
		for(i=0;i<B->down.size();++i){
			Bloc*U=B->down[i];
			if(find(U->up.begin(),U->up.end(),B)==U->up.end())return 0;
			}
		if(B->up.empty())return 0;
		for(i=0;i<B->up.size();++i){
			Bloc*U=B->up[i];
			if(find(U->down.begin(),U->down.end(),B)==U->down.end())return 0;
			}
		}
	return 1;
}


Algorithm* Sxema::Coder(){
	//normalizace
	if(!HeadEnd)return NULL;
	bool paralel=0;
	for(;;){
		V_B::iterator it=find(HeadEnd->down.begin(),HeadEnd->down.end(),HeadEnd);
		if(it==HeadEnd->down.end())break;
		HeadEnd->down.erase(it);
		it=find(HeadEnd->up.begin(),HeadEnd->up.end(),HeadEnd);
		HeadEnd->up.erase(it);
		paralel=1;
		}
	Bloc*N=HeadEnd;
	if(HeadEnd->down.size()+((int)paralel)>1){
		N=new(Bloc);
		AddDown(HeadEnd,N);
		rangOK=0;
		}
	if(paralel){
		N->down.push_back(HeadEnd);
		HeadEnd->up.push_back(N);
		}
	if(HeadEnd->up.size()>1){
		N=new(Bloc);
		AddUp(HeadEnd,N);
		rangOK=0;
		}
	//разделит условные блоки с более двух переходов, на блоки по два перехода
	ower_ifs();
	//заменит неоднозначные (неусловные) переходы на условные #
	DEFINE_ONE();
	// удалит все узлы вне прохода схемы.
	Antitupik();
	//controlRang();
	//FIND_CUCLE();
	UnitCucle UC;
	UC.Begin=*HeadEnd->down.begin();
	UC.Exit=UC.End=HeadEnd;
	UC.t=0;
	V_pUC WAY;
	int vxodov=1;
	if(!UC.Scaner(this,WAY,vxodov))return NULL;
	Sequence*S=UC.Build();
	if(S)S->initUP(NULL);
	/*
	if(HeadEnd->down.size()==1 && HeadEnd->up.size()==1)
	if(HeadEnd->down[0]==HeadEnd && HeadEnd->up[0]==HeadEnd){
		Algorithm*A=HeadEnd->A;
		HeadEnd->A=NULL;
		A->initUP(NULL);
		return A;
		}
	*/
	return S;
}


// find and devide (if) exits>2
// A->if3 => A->{if2,if1}
void Sxema::ower_ifs(){
	int povtor=1;
	while(povtor){
		povtor=0;
		S_B S;
		HeadEnd->getAll(S);
		S_B::iterator it=S.begin();
		for(;it!=S.end();++it){
			Bloc*B=*it;
			IF*iif=dynamic_cast<IF*>(B->A);
			if(!iif)continue;
			int n=B->down.size();
			if(n<=2)continue;
			Bloc*X=new(Bloc);
			X->A=iif->X->copy();
			X->A->initUP(NULL);
			bool isParadox=0;
			for(;n>2;--n){
				Bloc*A=*B->down.rbegin();
				if(A==B)isParadox=1;
				X->down.push_back(A);
				B->down.pop_back();
				A->up.erase(find(A->up.begin(),A->up.end(),B));
				A->up.push_back(X);
				}
			V_B::iterator jt=B->up.begin();
			for(;jt!=B->up.end();++jt){
				Bloc*A=*jt;
				A->down.push_back(X);
				X->up.push_back(A);
				}
			if(isParadox){
				X->down.push_back(X);
				X->up.push_back(X);
				}
			rangOK=0;
			povtor=1;
			}
		}
}


// find and devide to undefined ifs (noif) exit>1
// заменит неоднозначные (неусловные) переходы на условные #
void Sxema::DEFINE_ONE(){
	S_B S;
	HeadEnd->getAll(S);
	S_B::iterator it=S.begin();
	for(;it!=S.end();++it){
		Bloc*B=*it;
		IF*iif=dynamic_cast<IF*>(B->A);
		if(iif)continue;
		int n=B->down.size();
		if(n<2)continue;
		Bloc*N;
		if(B->A){
			N=new(Bloc);
			AddDown(B,N);
			}else N=B;
		V_B D;
		D.push_back(N);
		while(!D.empty()){
			N=*D.rbegin();
			D.pop_back();
			n=N->down.size();
			{
			S_B SB;
			SB.insert(N->down.begin(),N->down.end());
			if(n>SB.size()){
				int i;
				for(i=0;i<N->down.size();++i){
					Bloc*E=N->down[i];
					E->up.erase(find(E->up.begin(),E->up.end(),N));
					}
				N->down.clear();
				S_B::iterator it=SB.begin();
				for(;it!=SB.end();++it){
					N->down.push_back(*it);
					(*it)->up.push_back(N);
					}
				n=N->down.size();
				}
			}
			if(n>2){
				int t=rand()%(n-1);
				Bloc*AA=NULL,*BB=NULL,*W=NULL;
				if(t)AA=new(Bloc);
				if(t<n-2)BB=new(Bloc);
				int i;
				for(i=0;i<n;++i){
					B=N->down[i];
					bool ok=0;
					if(i<=t){
						if(AA){
							AA->down.push_back(B);
							B->up.push_back(AA);
							ok=1;
							}else W=B;
						}else{
							if(BB){
							BB->down.push_back(B);
							B->up.push_back(BB);
							ok=1;
							}else W=B;
						}
					if(ok)B->up.erase(find(B->up.begin(),B->up.end(),N));
					}
				N->down.clear();
				if(AA){D.push_back(AA);N->down.push_back(AA);}
				if(W)N->down.push_back(W);
				if(BB){D.push_back(BB);N->down.push_back(BB);}
				n=2;
				rangOK=0;
				}
			if(n==2){
				Base*F=new(Base);
				F->type=0;
				F->text="#";
				iif=new(IF);
				iif->X=F;
				N->A=iif;
				N->A->initUP(NULL);
				}
			}
		}
}


// удалит все узлы вне прохода схемы.
void Sxema::Antitupik(){
	S_B S;
	HeadEnd->getAll(S);
	Bloc*B;
	S_B::iterator it=S.begin();
	for(;it!=S.end();++it){
		B=*it;
		if(!B->down.empty() || B==HeadEnd)continue;
		B->down.push_back(HeadEnd);
		HeadEnd->up.push_back(B);
		}
	int i;
	for(i=0;i<2;++i){
		S_B U;
		V_B X;
		X.push_back(HeadEnd);
		while(!X.empty()){
			B=*X.rbegin();
			X.pop_back();
			if(!U.insert(B).second)continue;
			V_B::iterator jt=(i?B->down:B->up).begin(),et=(i?B->down:B->up).end();
			for(;jt!=et;++jt)X.push_back(*jt);
			}
		if(U.size()==S.size())continue;
		it=S.begin();
		for(;it!=S.end();++it){
			B=*it;
			if(find(U.begin(),U.end(),B)!=U.end())continue;
			B->disconnectUp();
			B->disconnectDown();
			delete B;
			}
		}
}


// S1 - false, S2 - true
void Sxema::findEndIF(Bloc*A,V_B*S1,V_B*S2,V_B*S3){
	Bloc*L,*R;
	L=A->down[0];
	R=A->down[1];
	if(L==R){
		S3->push_back(R);
		return;
		}
	V_B S;
	V_I V;
	S.push_back(A);
	V.push_back(8);
	S.push_back(L);
	V.push_back((L->rang<=A->rang)?5:1);
	S.push_back(R);
	V.push_back((R->rang<=A->rang)?6:2);
	bool problema=0;
	Bloc*P;
	int i,j,t,e,v,n,na,nb,nc;
	while(1){
		j=-1;
		na=nb=nc=n=0;
		for(i=0;i<S.size();++i){
			P=S[i];
			//if(!P)continue;
			if(V[i]&16)continue;
			if(V[i]&8)continue;
			if(j<0){j=i;t=(V[i]&4);}
			e=(V[i]&4);
			if(!e){
				if(P->rang<=S[j]->rang)j=i;
				if(t)j=i;
				t=0;
				}
			++n;
			if(e){
				if(P->rang<=A->rang)e=0;
				if(S3->size())if((*S3)[0]->rang<=P->rang)e=0;
				}
			if(!e){
				if((V[i]&3)==1)++na;
				if((V[i]&3)==2)++nb;
				if((V[i]&3)==3)++nc;
				}
			}
		if(j<0)break;
		//if(nc>0)break;
		P=S[j];
		e=V[j];
		v=(V[j]&3);
		t=(V[j]&4);
		if(t || P==HeadEnd){
			V[j]=V[j]|16;
			continue;
			}
		if(v==3)if(!(na||nb))break;
		V[j]=V[j]|16;
		if(v==3)continue;
		for(i=0;i<P->down.size();++i){
			int k=4;
			if(P->down[i]->rang>P->rang)k=0;
			for(e=t=0;t<S.size();++t)if(S[t]==P->down[i]){e=1;break;}
			if(e){
				j=V[t]|v;
				V[t]=(j|k)&15;
				continue;
				}
			S.push_back(P->down[i]);
			V.push_back(v|k);
			}
		}
	for(i=0;i<S.size();++i){
		v=(V[i]&3);
		if(!v)continue;
		(v==1?S2:(v==2?S1:S3))->push_back(S[i]);
		}
}



//--------------------------------------------------------------------------------------------------
Sxema::UnitCucle::UnitCucle(){isFor=isWhile=0;}
Sxema::UnitCucle::~UnitCucle(){
	int i;
	for(i=0;i<next.size();++i)delete next[i];
}


int Sxema::UnitCucle::Scaner(Sxema*S,V_pUC&WAY,int&vxodov){
	int i;
	for(i=0;i<WAY.size();++i)if(*WAY[i]==*this)return 0;
	WAY.push_back(this);
	Bloc*P=Begin,*B;
	int x,numberUP,zanano;
	while(P!=End){
		S->table.clear();//
		if(!S->controlCorrectSxema())return 10;
		//cout<<S->toString(1).c_str()<<endl;//
		S->controlRang();
		if(t)if(P==S->HeadEnd){
			WAY.pop_back();
			return 0;
			}
		numberUP=-1;
		zanano=0;
		int countUp=0;
		int i;
		for(i=0;i<P->up.size();++i){
			if(P->up[i]->rang>P->rang){
				bool ok=1;
				int j;
				for(j=0;j<WAY.size();++j)if(WAY[j]->End==P->up[i]){
					ok=0;
					++zanano;
					}
				if(ok){
					if(numberUP<0)numberUP=i;
					if(P->up[i]->rang>P->up[numberUP]->rang)numberUP=i;
					}
				}
			if(P->up[i]->rang<P->rang)++countUp;
			}
		if(vxodov<countUp){
			WAY.pop_back();
			return 0;
			}
		if(P->up.size()<=zanano+vxodov)
		if(P->down.size()>1){
			if(P->down.size()>2){
				WAY.pop_back();
				return 0;
				}
			V_B S1,S2,S3;
			S->findEndIF(P,&S1,&S2,&S3);
			if(S3.size()==1){
				B=S3[0];
				bool ok=1;
				if(t==2 || t==3)ok=(B->rang<=End->rang);
				if(t==3)if(Begin->rang>End->rang)ok=1;
				//if(B->rang<P->rang)ok=0;
				if(ok)if(P!=B){
					UnitCucle*UC=new UnitCucle();
					UC->End=UC->Exit=B;
					UC->t=3;//if
					UC->Begin=P->down[0];
					int vxodov1=1;
					x=UC->Scaner(S,WAY,vxodov1);
					if(x==10){
						delete UC;
						continue;
						}
					if(x){
						UC->Begin=P->down[1];
						int vxodov2=1;
						x=UC->Scaner(S,WAY,vxodov2);
						if(x==10){
							delete UC;
							continue;
							}
						if(x){
							S1.clear();
							S2.clear();
							S3.clear();
							S->findEndIF(P,&S1,&S2,&S3);
							if(S3.size()==1){
								next.push_back(UC);
								UC->Begin=P;
								P=B;
								vxodov=vxodov1+vxodov2;
								continue;
								}
							delete UC;
							continue;
							}else{
							delete UC;
							}
						}else{
						delete UC;
						}
					}
				}
			}
		if(numberUP>=0)if(P->up[numberUP]->down.size()>1)
		if(P==P->up[numberUP]->down[0]){
			UnitCucle*UC=new UnitCucle();
			UC->t=2;//do
			UC->End=P->up[numberUP];
			UC->Exit=P->up[numberUP]->down[1];
			UC->Begin=P;
			x=UC->Scaner(S,WAY,vxodov);
			if(x!=1){
				delete UC;
				}else{
				next.push_back(UC);
				P=UC->Exit;
				vxodov=1;
				continue;
				}
			}
		if(numberUP<0)if(P->down.size()>1)if(P->down[0]==P)numberUP=0;
		if(P->down.size()>1 && numberUP>=0){
			UnitCucle*UC=new UnitCucle();
			next.push_back(UC);
			UC->t=1;//while
			UC->End=P;
			UC->Exit=P->down[1];
			UC->Begin=P->down[0];
			x=UC->Scaner(S,WAY,vxodov);
			if(x!=1){
				delete UC;
				next.pop_back();
				WAY.pop_back();
				return x;
				}
			UC->Begin=P;
			P=UC->Exit;
			vxodov=1;
			continue;
			}
		if(P->down.size()>1){
			i=ControlBreak(S,WAY,0,P);
			if(!i)i=ControlBreak(S,WAY,1,P);
			if(i){
				if(t==3){
					WAY.pop_back();
					return 10;
					}
				continue;
				}
			WAY.pop_back();
			return 0;
			}
		P=P->down[0];
		vxodov=1;
		}
	WAY.pop_back();
	S->controlRang();
	return 1;
}



int Sxema::UnitCucle::ControlBreak(Sxema*S,V_pUC&WAY,int n,Bloc*P){
	Bloc*B,**PP=&P->down[n],*Prev=P;
	Bloc*Stoper=NULL;
	UnitCucle*toFor=NULL;
	while(1){
		//analize break & continue:
		int ok=0;//break
		int ok2=0;//continue
		int i;
		for(i=WAY.size()-1;i>=0;--i){
			int t=WAY[i]->t;
			if(t==1 || t==2){
				if(WAY[i]->End==*PP){ok2=t;break;}
				if(WAY[i]->Exit==*PP){ok=1;break;}
				}
			if(t==1){
				t=((*PP)->down.size()==1);
				if(t)t=((*PP)->down[0]==WAY[i]->End);
				if(t)t=((*PP)->up.size()>1);
				if(t){
					ok2=1;
					toFor=WAY[i];
					if(!toFor->isWhile)toFor->isFor=1;
					break;
					}
				}
			}
		if(ok || ok2){
			if(ok2){
				int i;
				for(i=1;i<WAY.size();++i){
					int t=WAY[i]->t;
					if(t==1)if(WAY[i]->End==*PP){
						toFor=WAY[i];
						break;
						}
					}
				}
			if(Stoper){
				Prev=Stoper;
				PP=&Stoper->down[0];
				if(toFor)if(!toFor->isWhile)
				if((*PP)->down.size()==1)
				if((*PP)->down[0]==toFor->End)toFor->isFor=1;
				}

			if(ok2==1){
				S->controlRang();
				int nn=0;
				int a;
				for(a=0;a<(*PP)->up.size();++a){
					if((*PP)->up[a]->rang > (*PP)->rang)++nn;
					}
				if(toFor)if(toFor->isFor)nn=0;
				if(nn==1)return 0;
				}
			//if(*PP==S->HeadEnd)return 0;
			if(ok2==2)if(P->down[1-n]->rang >= WAY[i]->End->rang)return 0;
			SpecSumbol*SS=dynamic_cast<SpecSumbol*>(Prev->A);
			if(SS)if(SS->n==3 || SS->n==4)return 0;//Противо зацикалка
			ok=0;
			for(;i<WAY.size();++i){
				int t=WAY[i]->t;
				if(t==1 || t==2)++ok;
				}
			int pset=ok2?3:4; // 3-continue 4-break
			if(ok2)if(toFor)if(!toFor->isFor)toFor->isWhile=1;
			if((*PP)->up.size()==1)return 0;
			if(ok>1){
				SpecSumbol2*SS2=new SpecSumbol2();
				SS2->n=pset;
				Base*X=new Base();
				X->type=3;//digit
				X->text=SCANER::toString(ok);
				SS2->X=X;
				SS=SS2;
				}else{
				SS=new SpecSumbol();
				SS->n=pset;
				}
			SS->initUP(NULL);

			B=*PP;
			B->up.erase(find(B->up.begin(),B->up.end(),Prev));
			B=new Bloc();
			B->A=SS;
			B->up.push_back(Prev);
			*PP=B;
			B->down.push_back(P->down[1-n]);
			P->down[1-n]->up.push_back(B);
			S->rangOK=0;
			break;
			}
		if(*PP!=S->HeadEnd)
		if(Prev->rang>=(*PP)->rang)return 0;
		B=*PP;
		Prev=B;
		if(B==End)return 0;
		if(B==S->HeadEnd)return 0;
		if(B->down.size()==1){
			PP=&B->down[0];
			if(!Stoper)if((*PP)->up.size()>1)Stoper=B;
			continue;
			}
		ok=0;
		while(1){
			V_B S1,S2,S3;
			S->findEndIF(Prev,&S1,&S2,&S3);
			if(S3.size()==1){
				B=S3[0];
				if(Prev!=B){
					if(Prev->rang>=B->rang)break;//return 0;
					Prev=B;
					if(Prev->down.size()==1){
						ok=1;
						PP=&Prev->down[0];
						break;
						}
					continue;
					}
				}
			break;
			}
		if(ok)continue;
		i=ControlBreak(S,WAY,0,Prev);
		if(!i)i=ControlBreak(S,WAY,1,Prev);
		return i;
		}
	return 1;
}



Sequence* Sxema::UnitCucle::Build(){
	Algorithm**A;
	Sequence*S=new Sequence();
	Bloc*P=Begin,*B;
	while(P!=End){
		int i,u;
		for(i=0,u=0;i<next.size();++i)if(next[i]->Begin==P && next[i]->t==2){u=1;break;}
		if(u){//do
			Sequence*SA;
			UnitCucle*UC=next[i];
			SA=UC->Build();
			IF*iif=dynamic_cast<IF*>(UC->End->A);
			Algorithm*X=iif->X;
			iif->X=NULL;
			delete iif;
			WHILE*W=new WHILE();
			W->n=2;
			W->X=X;
			if(SA){
				if(SA->nabor.size()>1)W->A=SA;else{
					if(SA->nabor.size()==1)
						W->A=(*SA->nabor.begin())->copy();
					delete SA;
					}
				}
			UC->End->A=W;
			S->nabor.push_back(UC->End->A);
			UC->End->A=NULL;
			P=UC->End;
			P->up.erase(find(P->up.begin(),P->up.end(),P));
			P->down.erase(find(P->down.begin(),P->down.end(),P));
			P->deleted();
			delete P;
			P=UC->Exit;
			continue;
			}
		bool ok=1;
		SpecSumbol*ss=dynamic_cast<SpecSumbol*>(P->A);
		if(ss)if(ss->n==5 || ss->n==6)ok=0; // PAGECLOSE & PAGEOPEN
		if(!P->A)ok=0;
		if(ok){
			S->nabor.push_back(P->A);
			A=&*S->nabor.rbegin();
			P->A=NULL;
			}
		if(P->down.size()>1){
			if(P->down.size()>2)return NULL;
			for(i=0,u=0;i<next.size();++i)if(next[i]->Begin==P && next[i]->t==3){u=1;break;}
			if(u){//if
				if(!(*A)->isIF())return NULL;
				Sequence*SA,*SB;
				UnitCucle*UC=next[i];
				UC->Begin=P->down[0];
				SA=UC->Build();
				UC->Begin=P->down[1];
				SB=UC->Build();
				UC->Begin=P;
				IF*iif=dynamic_cast<IF*>(*A);
				if(SA){
					if(SA->nabor.size()>1)iif->A=SA;else{
						if(SA->nabor.size()==1)iif->A=(*SA->nabor.begin())->copy();
						delete SA;
						}
					}
				if(SB){
					if(SB->nabor.size()>1)iif->E=SB;else{
						if(SB->nabor.size()==1)iif->E=(*SB->nabor.begin())->copy();
						delete SB;
						}
					}
				B=P;
				P->down.pop_back();
				P=P->down[0];
				P->up.erase(find(P->up.begin(),P->up.end(),B));
				B->deleted();
				delete B;
				continue;
				}
			for(i=0,u=0;i<next.size();++i)if(next[i]->Begin==P && next[i]->t==1){u=1;break;}
			if(u){//while
				Sequence*SA;
				UnitCucle*UC=next[i];
				UC->Begin=P->down[0];
				SA=UC->Build();
				UC->Begin=P;
				IF*iif=dynamic_cast<IF*>(*A);
				Algorithm*X=iif->X;
				iif->X=NULL;
				delete iif;
				if(!UC->isWhile && !UC->isFor)if(SA->nabor.size()){
					Prefix*P=dynamic_cast<Prefix*>(*SA->nabor.rbegin());
					if(P)if(P->n==3 || P->n==4)UC->isFor=1; // ++. --.
					Sufix*SU=dynamic_cast<Sufix*>(*SA->nabor.rbegin());
					if(SU)if(!dynamic_cast<CallFunc*>(SU))if(SU->n<=1)UC->isFor=1; // .++ .--
					}
				if(UC->isFor){
					FOR*For=new FOR();
					For->X=X;
					For->B=SA;
					For->P=*SA->nabor.rbegin();
					For->P->initUP(NULL);
					SA->nabor.pop_back();
					if(SA->nabor.size()==1){
						For->B=*SA->nabor.begin();
						SA->nabor.clear();
						delete SA;
						}
					For->B->initUP(NULL);
					*A=For;
					}else{
					WHILE*W=new WHILE();
					W->n=1;
					W->X=X;
					if(SA){
						if(SA->nabor.size()>1)W->A=SA;else{
							if(SA->nabor.size()==1){
								W->A=*SA->nabor.begin();
								SA->nabor.clear();
								}
							delete SA;
							}
						}
					*A=W;
					}
				B=P;
				V_B::iterator krax1=find(P->up.begin(),P->up.end(),P);
				V_B::iterator krax2=find(P->down.begin(),P->down.end(),P);
				if(krax1==P->up.end() || krax2==P->down.end()){
					delete S;
					return NULL;
					}
				P->up.erase(krax1);
				P->down.erase(krax2);
				P=UC->Exit;
				B->deleted();
				delete B;
				continue;
				}
			delete S;
			return NULL;
			}
		B=P;
		P=P->down[0];
		B->deleted();
		delete B;
		}
	return S;
}




bool Sxema::UnitCucle:: operator == (const UnitCucle&X) const {
	if(X.t!=t)return 0;
	if(X.isFor!=isFor)return 0;
	if(X.isWhile!=isWhile)return 0;
	return (X.Begin==Begin && X.End==End && X.Exit==Exit);
}


// sxema.cpp	:-|