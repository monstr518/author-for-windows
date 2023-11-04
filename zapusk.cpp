// zapusk.cpp
#include "main.h"



//--------------------------------------------------------------------------------------------------
TRAVERS::TRAVERS(){}

TRAVERS::TRAVERS(CVARIANT*t,int i){
	X=t;
	free=i;
}



//--------------------------------------------------------------------------------------------------
FRAGMENT::FRAGMENT(FRAGMENT*P){Prev=P;}

FRAGMENT::~FRAGMENT(){
	int i;
	for(i=0;i<Next.size();++i)if(Next[i])delete Next[i];
}


string FRAGMENT::toString(){
	string s;
	int i;
	for(i=0;i<Memorys.size();++i){
		string a;
		Memorys[i]->toString(a);
		s+=a;
		}
	return s;
}


CVARIANT* FRAGMENT::getDynamicValue(int n){
	if(Dynamic.find(n)!=Dynamic.end())return Dynamic[n];
	if(!Prev)return NULL;
	CVARIANT*R=Prev->getDynamicValue(n);
	if(!R)return NULL;
	Prev->Dynamic.erase(n);
	int i;
	for(i=0;i<Prev->Next.size();++i)
		Prev->Next[i]->Dynamic[n]=i?R->copy():R;
	return Dynamic[n];
}


//--------------------------------------------------------------------------------------------------
subI::subI(Function*f){
	ActivF=f;
	pos=NULL;
	isFreeFunction=0;
}


subI::subI(const subI&t){
	M_AlgoT::const_iterator jt=t.Bloki.begin();
	for(;jt!=t.Bloki.end();++jt){
		TRAVERS TR=jt->second;
		if(TR.free==1)TR.X=TR.X->copy();
			else if(TR.X->isType("program")){
				/*
				M_AA::const_iterator it=t.problemEval.begin();
				for(;it!=t.problemEval.end();++it){
					Algorithm*A=it->first;
					if(A==TR.X->DATA.programVal){
						TR.X=TR.X->copy();
						//problemEval[it->first]=TR.X->DATA.programVal;
						break;
						}
					}
				*/
				TR.free=1;
				TR.X=TR.X->copy();
				}
		Bloki[jt->first]=TR;
		}
	problemEval=t.problemEval;
	/*
	M_AA::const_iterator it=t.problemEval.begin();
	for(;it!=t.problemEval.end();++it){
		Algorithm*A=it->second;
		A=A->copy();
		A->initUP(NULL);
		problemEval[it->first]=A;
		}
	*/
	ActivF=t.ActivF;
	pos=t.pos;
	Return=t.Return;
	isFreeFunction=t.isFreeFunction;
	tableLinks=t.tableLinks;
}



//--------------------------------------------------------------------------------------------------
Laver::Laver(int n){
	N=n;
	SuperN=-1;
	R=0;
	rop=0;
	ExtraExit=0;
	needNameFunction=0;
	BufferSend=NULL;
	stanBuffer=0;
	nLaverWait=-2;
}


Laver::~Laver(){
	V_II::iterator it=IS.begin();
	for(;it!=IS.end();++it)if(*it)delete *it;
	if(BufferSend)delete BufferSend;
}


string Laver::printVariants(FRAGMENT*F,V_pFRAGMENT*VF){
	if(!VF){
		VF=new(V_pFRAGMENT);
		string s=printVariants(F,VF);
		delete VF;
		return s;
		}
	string s;
	VF->push_back(F);
	int i;
	for(i=0;i<F->Next.size();++i){
		s+=printVariants(F->Next[i],VF);
		}
	if(F->Next.empty()){
		for(i=0;i<VF->size();++i){
			int n=(*VF)[i]->Memorys.size();
			s+=SCANER::toString(i);
			if(!n)s+="-";
			while(n>0){
				s+=(n>1?":":".");
				n-=2;
				}
			}
		s+="\n";
		}
	VF->pop_back();
	return s;
}


string Laver::printVariants(){
	return printVariants(&Head);
}


//--------------------------------------------------------------------------------------------------
I::I(int n){
	Laver=n;
	stop=0;
	off=0;
	numberDefine=0;
}



I::I(const I&t){
	Laver=t.Laver;
	stop=t.stop;
	off=t.off;
	numberDefine=t.numberDefine;

	FRAGMENT*F=t.Fundament;
	Fundament=new(FRAGMENT);
	Fundament->Prev=F;
	F->Next.push_back(Fundament);

	D_subI::const_iterator it2,it=t.posicse.begin();
	int first;
	for(first=1;it!=t.posicse.end();++it){
		posicse.push_back(new subI(**it));
		if(!first){
			M_AA::const_iterator iit=(*it2)->problemEval.begin();
			for(;iit!=(*it2)->problemEval.end();++iit){
				if(iit->second==(*it)->Return){
					subI*SI=*posicse.rbegin();
					SI->Return=iit->first;
					}
				}
			}
		it2=it;
		first=0;
		}
	sub=*posicse.rbegin();
	stop=t.stop;
	off=t.off;
	numberDefine=t.numberDefine;
}



I::~I(){
	D_subI::iterator it=posicse.begin();
	for(;it!=posicse.end();++it)delete *it;
	FRAGMENT*F=Fundament,*F2;
	while(F->Next.empty()){
		F2=F->Prev;
		if(!F2)break;
		F2->Next.erase(find(F2->Next.begin(),F2->Next.end(),F));
		delete F;
		F=F2;
		}
	posicse.clear();
	sub=NULL;
}


I* I::OFFtoStop(MAIN*M){
	FRAGMENT*&F=Fundament,*F2;
	string S,E;
	S="this-f";
	E="stopoff*";
	while(F->Next.empty()){
		while(F->Memorys.size()){
			CVARIANT*X=*F->Memorys.rbegin();
			M_SV*map=X->DATA.mapVal;
			M_SV::iterator it=map->begin();
			for(;it!=map->end();++it){
				if(it->first==S){
					delete sub;
					posicse.pop_back();
					if(posicse.empty())sub=NULL;else sub=*posicse.rbegin();
					}
				if(it->first==E){
					CVARIANT*Y=&it->second;
					Function*f=sub->ActivF;
					f->sxema->controlTable();
					sub->pos=f->sxema->table[Y->DATA.intVal];
					return this;
					}
				}
			F->Memorys.pop_back();
			}
		F2=F->Prev;
		if(!F2)break;
		F2->Next.erase(find(F2->Next.begin(),F2->Next.end(),F));
		delete F;
		F=F2;
		}
	return NULL;
}


//--------------------------------------------------------------------------------------------------
unsigned long __stdcall MAIN::fork(void*X){
	interfaceFORK*Y=(interfaceFORK*)X;
	MAIN*M=Y->M;
	if(Y->VCV.empty()){
		cout<<"Error fork. Whot name function?"<<endl;
		}else M->RUN(&Y->VCV);
	delete Y;
	return 0;
}



void MAIN::RUN(V_CVARIANT*VCV){
	string name="Main";
	File*file=NULL;
	Function*f=NULL;
	if(!VCV){
		file=findModule("SYS\\main");
		if(!file){
			cout<<"SYS\\main noy found!!!"<<endl;
			Sleep(2000);
			return;
			}
		f=file->FindFunction(name);
		if(f)if(!f->Body && !f->sxema)f=NULL;
		if(!f){
			cout<<"Undefine start function: "<<name.c_str()<<endl;
			Sleep(2000);
			return;
			}
		}else{
		if((*VCV)[0].isType("function"))f=(*VCV)[0].DATA.functionVal->F;
		if(!f){
			(*VCV)[0].TransformType("string");
			name=*(*VCV)[0].DATA.ps;
			f=getFunction(NULL,name);
			}
		if(!f){
			cout<<"Error fork. Function "<<name.c_str()<<" no found"<<endl;
			return;
			}
		}
	Laver*L=tableLavers[FreeLaver]=new Laver(FreeLaver);
	I*me=new I(FreeLaver);
	CVARIANT *A,NF;
	A=new(CVARIANT);
	A->avtoSet("map");
	string S;
	NF.avtoSet("string");
	S="this-f";
	*NF.DATA.ps="f:"+name;
	(*A->DATA.mapVal)[S]=NF;
	int i;
	if(VCV)for(i=1;i<VCV->size()&&i-1<f->names.size();++i){
		S=f->names[i-1];
		(*A->DATA.mapVal)[S]=(*VCV)[i];
		}
	L->Head.Memorys.push_back(A);
	L->IS.push_back(me);
	me->Fundament=&L->Head;
	me->sub=new subI(f);
	me->posicse.push_back(me->sub);

	StartLaver(FreeLaver++);
	Mahine(L->activLaver);
}



void MAIN::StartLaver(int n){
	Laver*L=tableLavers[n];
	L->activLaver=n;
	I*Pset=NULL;
	int R=0; // 0-first, 1-random
	n=L->IS.size();
	if(R)n=rand()%n;else n=0;
	Pset=L->IS[n];
	L->activI=n;
	Pset->sub->ActivF->controlSxema();
	Pset->sub->ActivF->sxema->controlTable();
	Pset->sub->pos=Pset->sub->ActivF->sxema->HeadEnd->down[0];
	Pset->sub->Return=NULL;
}



void MAIN::BlokiClear(I*Pset){
	if(Pset->sub->Bloki.empty())return;
	M_AlgoT::iterator it=Pset->sub->Bloki.begin();
	for(;it!=Pset->sub->Bloki.end();++it)if(it->second.free)if(it->second.X)delete it->second.X;
	Pset->sub->Bloki.clear();
}


//главна€ точка обработки программы
void MAIN::Mahine(int activLaver){
	Laver*LL=tableLavers[activLaver];
	I*Pset=LL->IS[LL->activI];
	Bloc*B=NULL;
	int b,isEndf=0;
	LL->condition=0;
	LL->off=LL->ExtraExit=0;
	while(Pset){
		while(Pset->stop!=1 && !LL->off){
			if(!Pset->sub->ActivF){LL->off=Pset->off=1;break;}
			B = Pset->sub->pos;
			if(B==Pset->sub->ActivF->sxema->HeadEnd){isEndf=1;break;}
			if(!Pset->stop && B->A){
				CVARIANT*V=NULL;
				b=B->A->ZapuskTree(Pset,this,V);
				if(b){
					if(V)delete V;
					Pset->sub->Bloki.erase(B->A);
					}
				if(!Pset)break;
				if(LL->activI<0)break;
				if(LL->off||Pset->stop==1)break;
				if(LL->ExtraExit){
					LL->ExtraExit=0;
					continue;
					}
				BlokiClear(Pset);
				Pset->sub->tableLinks.clear();
				}
			if(LL->off||Pset->stop==1)break;
			if(Pset->stop)Pset->stop=0;
			int si=B->down.size();
			if(LL->condition){
				if(LL->condition==2){//true
					if(si)Pset->sub->pos=B->down[0];
					}else{
					if(si>1)Pset->sub->pos=B->down[1];else 
						if(si)Pset->sub->pos=B->down[0];
					}
				LL->condition=0;
				continue;
				}
			if(!si){
				LL->off=Pset->off=1;
				break;
				}
			if(si==1)Pset->sub->pos=B->down[0];else{
				bool isNeedWarningIF = true;
				if(Data){
					JSON::ONE *JV = Data->one->getValue("isNeedWarningIF#");
					if(JV)isNeedWarningIF = (bool)JV->intVal;
					}
				if(isNeedWarningIF)cout<<B->toString().c_str()<<endl;
				Pset->sub->pos=B->down[0];
				int t;
				for(t=1;t<si;++t){
					I*me=new I(*Pset);
					me->sub->pos=B->down[t];
					LL->IS.push_back(me);
					}
				FRAGMENT*F=Pset->Fundament;
				Pset->Fundament=new(FRAGMENT);
				F->Next.push_back(Pset->Fundament);
				Pset->Fundament->Prev=F;
				}
			}
		if(LL->activI<0){Pset=NULL;LL->off=0;}
		if(LL->off){
			LL->off=0;
			LL->ExtraExit=0;
			if(LL->IS.size()>1){
				LL->IS.erase(LL->IS.begin()+LL->activI);
				delete Pset;
				Pset=NULL;
				}else{
				Pset=Pset->OFFtoStop(this);
				if(!Pset)LL->IS.clear();
				}
			isEndf=0;
			}
		if(isEndf)if(Pset){
			isEndf=0;
			bool noClose=0;
			//LL=tableLavers[activLaver];
			int isFreeFunctionPrev=Pset->sub->isFreeFunction;
			if(Pset->sub->isFreeFunction){
				int i,j;
				for(j=0,i=0;i<LL->IS.size();++i)
					if(LL->IS[i]->sub->ActivF==Pset->sub->ActivF)++j;
				if(j<=1)delete Pset->sub->ActivF;
				noClose=1;
				if(isFreeFunctionPrev==1)Pset->stop=2;
				}
			delete Pset->sub;
			Pset->sub=NULL;
			Pset->posicse.pop_back();
			if(Pset->posicse.size())Pset->sub=*Pset->posicse.rbegin();
			if(!noClose)
			if(Pset->sub){
				while(!PAGECLOSE(Pset,0));
				}else{
				LL->IS.erase(LL->IS.begin()+LL->activI);
				delete Pset;
				Pset=NULL;
				}
			if(noClose)if(isFreeFunctionPrev==2){
				if(LL->SuperN<0)break;
				activLaver=LL->SuperN;
				LL=tableLavers[LL->SuperN];
				Pset=NULL;
				}
			}
		if(Pset)if(Pset->stop==1)Pset=NULL;
		if(!Pset){
			LL->ExtraExit=0;
			//LL=tableLavers[activLaver];
			if(LL->IS.empty()){
				if(LL->SuperN<0)break;
				activLaver=LL->SuperN;
				LL=tableLavers[LL->SuperN];
				}
			int j,i,p,n=0,R=LL->R; // 0-first, 1-random, 2-end
			j=-1;
			for(i=0;i<LL->IS.size();++i)if(LL->IS[i]->off){j=i;break;}
			if(j>=0){
				LL->activI=j;
				Pset=LL->IS[j];
				LL->off=Pset->off;
				//Pset->stop;
				continue;
				}
			for(i=0;i<LL->IS.size();++i)if(LL->IS[i]->stop!=1)++n;
			if(!n){// all stop
				int maxGlubina=0,iGlubina,maxN,iok=1;
				for(i=0;i<LL->IS.size();++i){
					I*me=LL->IS[i];
					iGlubina=me->posicse.size();
					if(maxGlubina<iGlubina)maxGlubina=iGlubina;
					}
				for(i=0;i<LL->IS.size();++i){
					I*me=LL->IS[i];
					iGlubina=me->posicse.size();
					if(maxGlubina==iGlubina){
						if(iok)maxN=me->numberDefine;
						iok=0;
						if(maxN<me->numberDefine)maxN=me->numberDefine;
						}
					}
				V_I mapProcess;
				for(i=0;i<LL->IS.size();++i){
					I*me=LL->IS[i];
					iGlubina=me->posicse.size();
					if(iGlubina!=maxGlubina)continue;
					if(me->numberDefine!=maxN)continue;
					mapProcess.push_back(i);
					}
				i=rand()%mapProcess.size();
				p=mapProcess[i];
				//¬ключаю группу процессов (p)
				Pset=LL->IS[p];
				LL->adaptationMap.clear();
				for(i=0;i<LL->IS.size();++i)
					if(Pset->sub->pos==LL->IS[i]->sub->pos)
					if(Pset->DefineNames==LL->IS[i]->DefineNames){
						LL->IS[i]->stop=0;
						LL->adaptationMap.push_back(i);
						}
				if(Pset->DefineNames.size()){
					BlokiClear(Pset);
					int pos,t,k;
					CVARIANT E,RONE;
					E.avtoSet("void");
					LL->ResultDEF.avtoSet("vector");
					LL->ResultDEF.DATA.vectorVal->clear();
					RONE.avtoSet("vector");
					for(i=0;i<Pset->DefineNames.size();++i){
						for(t=0;t<LL->adaptationMap.size();++t){
							k=LL->adaptationMap[t];
							Pset=LL->IS[k];
							CVARIANT*X=NULL;
							Base::findVar(Pset,this,X,Pset->DefineNames[i],pos);
							RONE.DATA.vectorVal->push_back((X?*X:E).copy());
							}
						LL->ResultDEF.DATA.vectorVal->push_back(RONE.copy());
						RONE.DATA.vectorVal->clear();
						}
					Pset->DefineNames.clear();
					}
				if(LL->SuperN>=0){
					activLaver=LL->SuperN;
					LL=tableLavers[activLaver];
					Pset=LL->IS[LL->activI=0];
					continue;
					}
				Pset=LL->IS[p];
				Pset->stop=0;
				j=p;
				for(i=0;i<LL->IS.size();++i)if(i!=p)if(Pset->sub->pos==LL->IS[i]->sub->pos){
					LL->IS[i]->off=1;
					LL->IS[i]->stop=0;
					j=i;
					}
				LL->activI=j;
				Pset=LL->IS[j];
				Pset->stop=2;
				LL->off=Pset->off;
				continue;
				}
			if(!R)p=0;
			if(R==1)p=rand()%n;
			if(R>1)p=n-1;
			n=-1;
			for(i=0;i<LL->IS.size();++i){
				if(LL->IS[i]->stop!=1)++n;
				if(n==p)break;
				}
			Pset=LL->IS[i];
			if(Pset->stop==1)Pset->stop=2;
			LL->off=Pset->off;
			LL->activI=i;
			}
		}
}



void MAIN::L_IS(Laver*LL,Function*f){
	int i;
	for(i=0;i<LL->IS.size();++i){
		D_subI::iterator it=LL->IS[i]->posicse.begin();
		for(;it!=LL->IS[i]->posicse.end();++it)
			if((*it)->ActivF==f)(*it)->ActivF=NULL;
		}
}



void MAIN::ProgresMemory(I*Pset,CVARIANT&A,CVARIANT&t){
	Laver*L=tableLavers[Pset->Laver];
	int i;
	for(i=0;i<L->IS.size();++i){
		I*me=L->IS[i];
		int j;
		for(j=0;j<me->posicse.size();++j){
			subI*sub=me->posicse[j];
			M_AlgoT::iterator it=sub->Bloki.begin();
			for(;it!=sub->Bloki.end();++it){
				M_SV::iterator jt=t.DATA.mapVal->begin();
				for(;jt!=t.DATA.mapVal->end();++jt)if(it->second.X==&jt->second){
					CVARIANT*V=&(*A.DATA.mapVal)[jt->first];
					it->second.X=V;
					if(sub==Pset->sub){
						int k;
						for(k=0;k<sub->problemRozpada.size();++k){
							CVARIANT**X=sub->problemRozpada[k];
							if(*X==&jt->second)*X=V;
							}
						}
					}
				}
			}
		}
}



int MAIN::PAGECLOSE(I*Pset,int stop){
	//Laver*L=tableLavers[activLaver];L->printVariants();//
	FRAGMENT*H=Pset->Fundament,*Bake,*P;
	while(H->Memorys.empty()){
		Bake=H;
		H=H->Prev;
		if(!H)break;
		if(H->Next.size()>1){
			if(H->Memorys.empty()){
				Bake->Prev=H->Prev;
				H->Prev->Next.push_back(Bake);
				H->Next.erase(find(H->Next.begin(),H->Next.end(),Bake));
				H=Bake;
				continue;
				}
			CVARIANT*tmp=H->Memorys[H->Memorys.size()-1];
			int i;
			for(i=0;i<H->Next.size();++i){
				P=H->Next[i];
				P->Memorys.insert(P->Memorys.begin(),tmp->copy());
				ProgresMemory(Pset,**P->Memorys.begin(),*tmp);
				}
			delete tmp;
			H->Memorys.pop_back();
			H=Bake;
			}
		}
	bool b=0;
	if(H){
		CVARIANT*V=*H->Memorys.rbegin();
		b=V->DATA.mapVal->find("this-f")!=V->DATA.mapVal->end();
		if(b&&stop)return 0;
		//cout<<FragmentOneToString(H->Memorys.rbegin()->DATA.mapVal).c_str()<<endl;
		delete *H->Memorys.rbegin();
		H->Memorys.pop_back();
		}else cout<<"\nWarning: PAGECLOSE out of memory!"<<endl;
	//L->printVariants();//
	return b;
}



string MAIN::FragmentOneToString(M_SV*MC){
	string str;
	M_SV::iterator it=MC->begin();
	for(;it!=MC->end();++it){
		str+=it->first;
		str+="	: ";
		string s;
		it->second.toString(s);
		str+=s+"\n";
		}
	return str;
}



//().asfd.asf.fdas->x->y.t
void MAIN::getMapKeys(I*Pset,CVARIANT*&R,string&ss,bool create){
	string s=ss;
	ss="";
	int a,b;
	while(s.size()){
		int a2=s.find(".");
		int b2=s.find("->");
		if(a2>0 && b2>0)break;
		if(!a2)s=s.substr(1);
		if(!b2)s=s.substr(2);
		a=s.find(".");
		b=s.find("->");
		int i=0;//b
		if(a>0 && b>0){
			if(b>a)i=1;
			}else{
			if(b<0)i=1;
			}
		if(!i)a=b;
		string t;
		if(a>=0){
			t=s.substr(0,a);
			s=s.substr(a);
			}else{
			t=s;
			s="";
			}
		if(!b2){
			if(!(R->isType("string") || R->isType("pointer"))){
				R=NULL;
				break;
				}
			string str=*R->DATA.ps;
			if(str.find("*"))break;
			ss=str=str.substr(1);
			R=getUnLink2(Pset,str);
			if(!R)break;
			}
		if(R->isType("map")){
			M_SV&GM=*R->DATA.mapVal;
			CVARIANT A;
			A.avtoSet("string");
			*A.DATA.ps=t;
			ss+="."+t;
			
			if(!create)if(GM.find(t)==GM.end()){
				CVARIANT P=A;
				A.TransformType("int");
				A.TransformType("string");
				bool ok=1;
				if(A==P)if(GM.find(t)!=GM.end())ok=0;
				if(ok){
					R=NULL;
					break;
					}
				}
			
			R=&GM[t];
			continue;
			}
		if(R->isType("vector")){
			int n=0;
			sscanf(t.c_str(),"%d",&n);
			if(!n)if(t!="0")n=-1;
			if(n>=0 && n<R->DATA.vectorVal->size()){
				R=(*R->DATA.vectorVal)[n];
				continue;
				}
			}
		R=NULL;
		break;
		}
}



CVARIANT* MAIN::getUnLink(I*Pset,string&s,bool&needKill){
	string prevs=s;
	CVARIANT*R=NULL;
	needKill=0;
	bool isGlobal=0;
	if(!s.find("::")){
		s=s.substr(2);
		int i=s.find(".");
		string t;
		if(i>=0){
			t=s.substr(0,i);
			s=s.substr(i);
			}else{
			t=s;
			s="";
			}
		M_SV&GM=*GlobalSpace.Map.DATA.mapVal;
		R=&GM[t];
		isGlobal=1;
		}
	int p=s.find(".");
	int pp=s.find("->");
	int pos;
	if(p>0 || pp>0){
		if(p<0)p=pp;
		if(pp<p && pp>0)p=pp;
		string t=s.substr(0,p);
		s=s.substr(p);
		Base::findVar(Pset,this,R,t,pos);
		}
	if(!R){
		s=prevs;
		return R;
		}
	getMapKeys(Pset,R,s);
	if((int)prevs.find("->")<0){
		if(!isGlobal){
			s=string("I:")+SCANER::toString(pos)+";";
			s+=prevs;
			}else s=prevs;
		}
	return R;
}



CVARIANT* MAIN::getUnLink2(I*Pset,string&s){
	if(!s.find("::")){
		bool ok;
		return getUnLink(Pset,s,ok);
		}
	Laver*L=tableLavers[Pset->Laver];
	if(!s.find("$:")){
		string ss=s;
		s=s.substr(2);
		int n=0;
		sscanf(s.c_str(),"%d",&n);
		CVARIANT*R=NULL;
		FRAGMENT*F=Pset->Fundament;
		if(L->rop)R=F->getDynamicValue(n); else do{
			if(F->Dynamic.find(n)!=F->Dynamic.end()){
				R=F->Dynamic[n];
				break;
				}
			F=F->Prev;
			}while(F);
		int i=s.find(";");
		if(i<0)return NULL;
		s=s.substr(i+1);
		MAIN::getMapKeys(Pset,R,s,0);
		if((int)s.find("->")>=0)s=ss;
		return R;
		}
	if(s.find("I:"))return NULL;
	s=s.substr(2);
	int n=0,i,n2;
	sscanf(s.c_str(),"%d",&n);
	n2=n;
	i=s.find(";");
	s=s.substr(i+1);
	string name;
	i=s.find(".");
	if(i<0){
		name=s;
		s="";
		int k=name.find(";");
		if(k>0)name=name.substr(0,k);
		}else{
		name=s.substr(0,i);
		s=s.substr(i);
		}
	FRAGMENT*F=Pset->Fundament;
	V_pFRAGMENT VF;
	while(F){
		VF.push_back(F);
		F=F->Prev;
		}
	CVARIANT*CV=NULL;
	int j;
	for(j=VF.size()-1;j>=0;--j){
		if(VF[j]->Memorys.size()>n)CV=VF[j]->Memorys[n];
		n-=VF[j]->Memorys.size();
		if(n<0)break;
		}
	if(!CV)return NULL;
	CVARIANT*R;
	if(!L->rop)R=&(*CV->DATA.mapVal)[name];else{
		int nvf=0;
		for(j=0;j<VF.size();++j)nvf+=VF[j]->Memorys.size();
		bool isFind=0;
		FRAGMENT*H=Pset->Fundament,*P;
		n2=nvf-n2-1;
		n=n2;
		n2-=H->Memorys.size();
		H=H->Prev;
		if(n2<0){
			CV=Pset->Fundament->Memorys[-n2-1];
			H=NULL;
			}
		while(H){
			while(H->Memorys.size()){
				int i2=H->Memorys.size();
				CVARIANT*tmp=H->Memorys[i2-1];
				int i;
				for(i=0;i<H->Next.size();++i){
					P=H->Next[i];
					P->Memorys.insert(P->Memorys.begin(),tmp->copy());
					ProgresMemory(Pset,**P->Memorys.begin(),*tmp);
					}
				H->Memorys.pop_back();
				delete tmp;
				if(!n2){isFind=1;break;}
				--n2;
				}
			if(isFind){H=NULL;break;}
			if(H->Memorys.empty()){
				H->Next.erase(find(H->Next.begin(),H->Next.end(),Pset->Fundament));
				P=H;
				H=H->Prev;
				if(P->Next.empty()){
					H->Next.erase(find(H->Next.begin(),H->Next.end(),P));
					delete P;
					}
				H->Next.push_back(Pset->Fundament);
				Pset->Fundament->Prev=H;
				}
			}
		if(isFind)CV=Pset->Fundament->Memorys[0];
		R=&(*CV->DATA.mapVal)[name];
		}
	string ss=s;
	MAIN::getMapKeys(Pset,R,ss,0);
	if((int)s.find("->")<0)s=name+s;else s=ss;
	return R;
}



bool MAIN::isset(I*Pset,const string&V){
	FRAGMENT*H=Pset->Fundament;
	while(H){
		int i;
		for(i=H->Memorys.size()-1;i>=0;--i){
			CVARIANT*C=H->Memorys[i];
			M_SV*MC=C->DATA.mapVal;
			if(MC->find(V)!=MC->end())return 1;
			if(MC->find("this-f")!=MC->end()){H=NULL;break;}
			}
		if(!H)break;
		H=H->Prev;
		}
	return 0;
}




// zapusk.cpp	:-|