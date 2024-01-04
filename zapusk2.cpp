// zapusk2.cpp
#include "main.h"

//--------------------------------------------------------------------------------------------------
Algorithm::ZapuskTree(I*,MAIN *,CVARIANT * &){return 0;}

TRAVERS* Algorithm::Border(I*Pset,MAIN*M){
	if(Pset->sub->Bloki.find(this)!=Pset->sub->Bloki.end())
		return &Pset->sub->Bloki[this];
	return NULL;
}


CallFunc::ZapuskTree(I*Pset,MAIN*M,CVARIANT*&V){
	TRAVERS*T=Border(Pset,M);
	if(T){
		V=T->X;
		return T->free;
		}
	CVARIANT*Name=NULL;
	Laver*L=M->tableLavers[Pset->Laver];
	L->needNameFunction=1;
	int rop=L->rop;
	L->rop=0;
	int bb=X->ZapuskTree(Pset,M,Name);
	L->rop=rop;
	L->needNameFunction=0;
	if(L->ExtraExit)return 0;
	V_I mb;
	V_pCVARIANT PVCV;
	V_CVARIANT VCV;
	V_AL::iterator it=params.begin();
	int k;
	for(k=0;it!=params.end();++it,++k){
		if(!k)L->SpiritA=*it;
		CVARIANT*CV=NULL;
		int b=(*it)->ZapuskTree(Pset,M,CV);
		if(L->ExtraExit)return 0;
		if(!CV){
			CVARIANT W;
			W.avtoSet("void");
			VCV.push_back(W);
			}else VCV.push_back(*CV);
		PVCV.push_back(CV);
		mb.push_back(b);
		}
	for(k=1;k<PVCV.size();++k){
		if(mb[k])if(PVCV[k]){
			delete PVCV[k];
			}
		//if(PVCV[k])
		Pset->sub->Bloki.erase(params[k]);
		}
	if(!bb){
		CVARIANT*CV=new(CVARIANT);
		if(Name)*CV=*Name;
		Name=CV;
		bb=1;
		}
	Function*f=NULL;
	string name;
	if(Name->isType("function")){
		f=Name->DATA.functionVal->F;
		} else {
		Name->TransformType("string");
		name=*Name->DATA.ps;
		if(bb)if(Name){
			Pset->sub->Bloki.erase(X);
			delete Name;
			}
		}
	int p=name.find("fc:");
	if(!p){
		name=name.substr(3,name.length());
		V=NULL;
		if(name=="eval")if(PVCV.size()){
			Algorithm*A=PVCV[0]->DATA.programVal;
			T=A->Border(Pset,M);
			if(T){
				V=T->X;
				Pset->sub->Bloki.erase(A);
				}
			}
		if(!V)fc(Pset,name,VCV,V,M);
		if(name=="eval")if(L->ExtraExit){
			if(PVCV.size()){
				if(Pset){
					if(Pset->sub->Return==VCV[0].DATA.programVal)
						Pset->sub->Return=PVCV[0]->DATA.programVal;
					}
				}
			return 0;
			}
		if(name=="Spirit")return 0;
		if(mb.size()){
			if(mb[0])if(PVCV[0]){
				delete PVCV[0];
				}
			Pset->sub->Bloki.erase(params[0]);
			}
		//if(L->ExtraExit)return 0;
		bb=V?1:0;
		if(Pset)Pset->sub->Bloki[this]=TRAVERS(V,bb);
		return bb;
		}
	if(mb.size()){
		if(mb[0])if(PVCV[0]){
			delete PVCV[0];
			}
		Pset->sub->Bloki.erase(params[0]);
		}
	p=name.find("f:");
	if(!p){
		name=name.substr(2);
		f=M->getFunction(Pset,name);
		}
	if(f){//if(VCV.size()==f->names.size())
		CVARIANT NF,*VO;
		string K;
		NF.avtoSet("string");
		L->ExtraExit=1;
		VO=new(CVARIANT);
		VO->avtoSet("void");
		Pset->sub->Bloki[this]=TRAVERS(VO,1);
		Pset->sub=new subI(f);
		Pset->sub->ActivF=f;
		f->controlSxema();
		Pset->posicse.push_back(Pset->sub);
		Pset->sub->Return=this;
		Pset->sub->pos=f->sxema->HeadEnd->down[0];
		CVARIANT*CEL;
		CEL=new(CVARIANT);
		CEL->avtoSet("map");
		*NF.DATA.ps="f:"+f->getPsetWay()+name;
		(*CEL->DATA.mapVal)["this-f"]=NF;
		int i,namesSize,VCVSize;
		namesSize=f->names.size();
		VCVSize=VCV.size();
		for(i=0;i<namesSize;++i){
			K=f->names[i];
			(*CEL->DATA.mapVal)[K]=(i<VCVSize?VCV[i]:CVARIANT());
			}
		if(f->argumentsON){
			CVARIANT ARGS;
			ARGS.avtoSet("vector");
			for(;i<VCVSize;++i)ARGS.DATA.vectorVal->push_back(VCV[i].copy());
			(*CEL->DATA.mapVal)["arguments"]=ARGS;
			}
		Pset->Fundament->Memorys.push_back(CEL);
		}
	if(!p)return 0;
	p=name.find("m:");
	if(!p){
		name=name.substr(2);
		p=name.find(";");
		if(p>=0){
			string t=name.substr(p+1);
			name=name.substr(0,p);
			V=NULL;
			mc(Pset,t,name,VCV,V,M);
			if(L->ExtraExit)return 0;
			bb=V?1:0;
			Pset->sub->Bloki[this]=TRAVERS(V,bb);
			return bb;
			}
		}
	return 0;
}



int SpecSumbol::ZapuskTree(I*Pset,MAIN*M,CVARIANT*&V){
	Laver*L=M->tableLavers[Pset->Laver];
	//find const:
	if(n==7){//OFF
		L->off=1;
		return 0;
		}
	int open=0;
	if(n==6)open=1;//"PAGEOPEN"
	if(n==5)open=2;//"PAGECLOSE"
	//open=3-open; // if reverse
	if(open==1){
		//Laver*L=M->tableLavers[M->activLaver];
		CVARIANT*CEL=new(CVARIANT);
		CEL->avtoSet("map");
		//L->Head.Memorys.push_back(CEL);
		Pset->Fundament->Memorys.push_back(CEL);
		}
	if(open==2){
		M->PAGECLOSE(Pset);
		}
	if(n==0){
		delete Pset->sub;
		Pset->sub=NULL;
		Pset->posicse.pop_back();
		if(Pset->posicse.size())Pset->sub=*Pset->posicse.rbegin();
		if(Pset->sub){
			L->ExtraExit=1;
			while(!M->PAGECLOSE(Pset,0));
			}else L->off=Pset->off=1;
		}
	return 0;
}



string Base::getAdress(){return text;}


int Base::ZapuskTree(I*Pset,MAIN*M,CVARIANT*&X){
	//type: 0-var 1-string 2-char 3-digit 4-# 5-Algorithm
	TRAVERS*T=Border(Pset,M);
	if(T){X=T->X;return T->free;}
	if(type){
		//if(type==4)return 0;
		X=new(CVARIANT);
		char*m[]={"string","char","double","void"};
		X->avtoSet(m[type-1]);
		if(type==1)*X->DATA.ps=text;
		if(type==2)X->DATA.bVal=text.empty()?'_':text[0];
		if(type==3){
			double x = SCANER::f_digit(text.c_str());
			*X->DATA.dblVal = x;
			if(x==(int)x){
				X->avtoSet("int");
				X->DATA.intVal=x;
				} else
			if(text.size()>20){
				X->avtoSet("digit");
				X->DATA.digitVal->set(text.c_str());
				}
			}
		Pset->sub->Bloki[this]=TRAVERS(X,1);
		return 1;
		}
	bool isTrue=(text=="true");
	if(isTrue || (text=="false")){
		X=new(CVARIANT);
		X->avtoSet("bool");
		X->DATA.boolVal=isTrue;
		Pset->sub->Bloki[this]=TRAVERS(X,1);
		return 1;
		}
	//find var:
	CVARIANT S;
	S.avtoSet("string");
	//*S.DATA.ps="this-f";
	string S2;
	S2=text;
	Laver*L=M->tableLavers[Pset->Laver];
	if(!L->needNameFunction){
		int pos;
		if(findVar(Pset,M,X,text,pos)){
			string s="I:";
			s+=(string)SCANER::toString(pos)+";"+text;
			Pset->sub->adres=s;
			//if(!L->rop &&)
			Pset->sub->Bloki[this]=TRAVERS(X,0);
			return 0;
			}
		CVARIANT*DataTable=Pset->sub->ActivF->DataTable;
		if(DataTable){
			M_SV*MDT=DataTable->DATA.mapVal;
			if(MDT->find(S2)!=MDT->end()){
				X=&(*MDT)[S2];
				return 0;
				}
			}
		}
	if(!L->rop){
		//find function:
		string fullname=text;
		Function*f=M->getFunction(Pset,fullname);
		if(!f){
			Function*func=Pset->sub->ActivF;
			CLASS*C=func->Space;
			while(C){
				fullname=C->name+"::"+fullname;
				C=C->Space;
				}
			f=M->getFunction(Pset,fullname);
			}
		if(f){
			X=new(CVARIANT);
			X->avtoSet("string");
			string str="f:";
			str+=fullname;
			*X->DATA.ps=str;
			Pset->sub->Bloki[this]=TRAVERS(X,1);
			return 1;
			}
		//find const:
		if(CallFunc::isCFunction(text)){
			X=new(CVARIANT);
			X->avtoSet("string");
			string str="fc:";
			str+=text;
			*X->DATA.ps=str;
			Pset->sub->Bloki[this]=TRAVERS(X,1);
			return 1;
			}
		}
	//find in global space:
	M_SV*GM=M->GlobalSpace.Map.DATA.mapVal;
	if(GM->find(S2)!=GM->end()){
		Pset->sub->adres="::"+text;
		X=&(*GM)[S2];
		Pset->sub->Bloki[this]=TRAVERS(X,0);
		return 0;
		}
	//create new variable:
	FRAGMENT*F=Pset->Fundament;
	while(F){
		if(F->Memorys.size())break;
		F=F->Prev;
		}
	CVARIANT*G=*F->Memorys.rbegin();
	S.avtoSet("void");
	(*G->DATA.mapVal)[S2]=S;
	X=&(*G->DATA.mapVal)[S2];
	Pset->sub->Bloki[this]=TRAVERS(X,0);
	return 0;
}



int Base::findVar(I*Pset,MAIN*M,CVARIANT*&X,string text,int&pos,bool create){
	FRAGMENT*H=Pset->Fundament;
	string S,S2;
	S="this-f";
	S2=text;
	M_SV::iterator it;
	bool isFind=0,ok=1;
	pos=0;
	while(H){
		int i;
		for(i=H->Memorys.size()-1;i>=0;--i){
			CVARIANT*C=H->Memorys[i];
			M_SV*MC=C->DATA.mapVal;
			it=MC->find(S2);
			if(it!=MC->end()){isFind=1;pos=i;break;}
			if(MC->find(S)!=MC->end()){H=NULL;break;}
			}
		if(isFind){
			H=H->Prev;
			while(H){
				pos+=H->Memorys.size();
				H=H->Prev;
				}
			break;
			}
		if(!H)break;
		H=H->Prev;
		ok=0;
		}
	if(isFind){
		isFind=0;
		FRAGMENT*P;
		Laver*L=M->tableLavers[Pset->Laver];
		if(L->rop&&!ok){
			//L->printVariants();//
			H=Pset->Fundament->Prev;
			while(H){
				while(H->Memorys.size()){
					int i2=H->Memorys.size();
					CVARIANT*tmp=H->Memorys[i2-1];
					int i;
					for(i=0;i<H->Next.size();++i){
						P=H->Next[i];
						P->Memorys.insert(P->Memorys.begin(),tmp->copy());
						M->ProgresMemory(Pset,**P->Memorys.begin(),*tmp);
						//cout<<M->FragmentOneToString(P->Memorys.begin()->DATA.mapVal).c_str()<<endl;//
						}
					//cout<<M->FragmentOneToString(H->Memorys.rbegin()->DATA.mapVal).c_str()<<endl;//
					H->Memorys.pop_back();
					delete tmp;
					M_SV*MC=Pset->Fundament->Memorys[0]->DATA.mapVal;
					it=MC->find(S2);
					if(it!=MC->end()){isFind=1;break;}
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
			//L->printVariants();//
			if(isFind)X=&it->second;
			}else X=&it->second;
		return 1;
		}
	if(create){
		H=Pset->Fundament;
		while(!H->Memorys.size())H=H->Prev;
		CVARIANT*t=*H->Memorys.rbegin();
		CVARIANT E;
		E.avtoSet("int");
		(*t->DATA.mapVal)[S2]=E;
		X=&(*t->DATA.mapVal)[S2];
		pos=0;
		while(H){
			pos+=H->Memorys.size();
			H=H->Prev;
			}
		return 1;
		}
	return 0;
}




bool Base2::controlSwapConstruction(I*Pset,MAIN*M,CVARIANT*XB,int xb){
	// n==10  m[n]="="
	if(n!=10)return 0;
	AlgoSet*AS=dynamic_cast<AlgoSet*>(A);
	if(!AS)return 0;
	if(!XB->isType("vector"))return 1;
	int size = XB->DATA.vectorVal->size();
	int i = AS->nabor.size();
	if(i<size)size=i;
	if(!xb)XB=XB->copy();
	Laver*L=M->tableLavers[Pset->Laver];
	++L->rop;
	for(i=0;i<size;++i){
		Base*X=dynamic_cast<Base*>(AS->nabor[i]);
		if(!X)continue;
		if(X->type)continue;
		CVARIANT*V=NULL;
		int t;
		t=X->ZapuskTree(Pset,M,V);
		if(t){
			Pset->sub->Bloki.erase(X);
			delete V;
			continue;
			}
		*V=*(*XB->DATA.vectorVal)[i];
		}
	--L->rop;
	if(!xb)delete XB;
	return 1;
}



string Base2::getAdress(){
	string s;
	s=A->getAdress();
	s+=m[n];
	s+=B->getAdress();
	return s;
}



int Base2::ZapuskTree(I*Pset,MAIN*M,CVARIANT*&V){
	TRAVERS*T=Border(Pset,M);
	if(T){V=T->X;return T->free;}
	if(n==30 || n==31 || n==32){//  ->  .  ::
		string s=getAdress();
		if((int)s.find(".")<0 && (int)s.find("->")<0){
			V=new(CVARIANT);
			V->avtoSet("string");
			*V->DATA.ps="f:"+s;
			Pset->sub->Bloki[this]=TRAVERS(V,1);
			return 1;
			}
		Laver*L=M->tableLavers[Pset->Laver];
		if(!L->needNameFunction)
		if(s.find(".") && s.find("->") && s.find("*")){
			bool ok=0;
			V=M->getUnLink(Pset,s,ok);
			if(V){
				Pset->sub->adres=s;
				Pset->sub->Bloki[this]=TRAVERS(V,ok);
				return ok;
				}
			s="*";
			}
		Algorithm*F=A;
		V=NULL;
		int x;
		bool needNameFunction=L->needNameFunction;
		L->needNameFunction=0;
		++L->rop;
		x=F->ZapuskTree(Pset,M,V);
		--L->rop;
		L->needNameFunction=needNameFunction;
		if(L->ExtraExit)return 0;
		if(!V)return 0;
		CVARIANT*R=V;
		if(s.size())if(s.substr(0,1)!="." && s.substr(0,2)!="->")s=string(m[n])+B->getAdress();
		string ss=s;
		M->getMapKeys(Pset,V,s,L->rop!=0);
		if(V){
			Pset->sub->adres+=s;
			if(x){
				V=V->copy();
				delete R;
				}
			Pset->sub->Bloki[this]=TRAVERS(V,x);
			return x;
			}
		s=ss;
		int p=s.find_last_of("->");
		if(p>0){
			CVARIANT W=*R;
			W.TransformType("string");
			string tmp=*W.DATA.ps;
			if(tmp.size())if(tmp[0]=='*')tmp=tmp.substr(1);
			V=M->getUnLink2(Pset,tmp);
			if(!V){
				Pset->sub->Bloki[this]=TRAVERS(NULL,0);
				return 0;
				}
			TRAVERS*T=&Pset->sub->Bloki[F];
			T->X=V;
			T->free=0;
			s=s.substr(2);
			}else{
			p=s.find_last_of(".");
			s=s.substr(p+1);
			}
		if(!needNameFunction)return 0;
		int u=Pset->sub->tableLinks.size();
		Pset->sub->tableLinks.push_back(F);
		string str="m:"+s+";";
		str+="&:";
		str+=SCANER::toString(u);
		V=new CVARIANT();
		V->avtoSet("string");
		*V->DATA.ps=str;
		Pset->sub->Bloki[this]=TRAVERS(V,1);
		return 1;
		}
	Laver*L=M->tableLavers[Pset->Laver];
	CVARIANT*XA,*XB;
	XA=XB=NULL;
	int xa=0,xb=0;

		xb=B->ZapuskTree(Pset,M,XB);
		if(L->ExtraExit)return 0;
		if(!XB)return 0;
		if(controlSwapConstruction(Pset,M,XB,xb)){
			Pset->sub->Bloki.erase(B);
			V=XB;
			Pset->sub->Bloki[this]=TRAVERS(XB,xb);
			return xb;
			}
		subI*HE=Pset->sub;
		if(!xb)HE->problemRozpada.push_back(&XB);
		bool k=(n<=10);
		if(k)++L->rop;
		xa=A->ZapuskTree(Pset,M,XA);
		if(k)--L->rop;
		if(!xb)HE->problemRozpada.pop_back();
		if(L->ExtraExit)return 0;
		if(!XA){
			if(xb){
				Pset->sub->Bloki.erase(B);
				delete XB;
				}
			return 0;
			}

	if(XA->isType("string") && n==10){ // n==10  m[n]="="
		CVARIANT*P=XA;
		string s=*XA->DATA.ps;
		if(!s.find("&char:")){
			s=s.substr(6);
			int k=s.find_last_of(";");
			string n;
			if(k>0)n=s.substr(k+1);
			XA=M->getUnLink2(Pset,s);
			if(XA){
				if(xa){
					Pset->sub->Bloki.erase(A);
					delete P;
					}
				CVARIANT U=*XB;
				Pset->sub->Bloki.erase(B);
				U.TransformType("char");
				char c=U.DATA.bVal;
				int i=0;
				sscanf(n.c_str(),"%d",&i);
				if(i>=0 && i<XA->DATA.ps->size())(*XA->DATA.ps)[i]=c;
				V=XB;
				Pset->sub->Bloki[this]=TRAVERS(XB,xb);
				return xb;
				}
			if(xa){
				Pset->sub->Bloki.erase(A);
				delete P;
				}
			Pset->sub->Bloki.erase(B);
			Pset->sub->Bloki[this]=TRAVERS(NULL,0);
			return 0;
			}
		}

	if(n!=10)if(XB){
		if(!xb){
			CVARIANT*CV=new(CVARIANT);
			*CV=*XB;
			XB=CV;
			xb=1;
			}
		const char*ca=NULL,*cb=NULL;
		int vca=0,vcb=0;
		if(string(XA->name[XA->Ntype])=="int"){ca=XA->name[XA->Ntype];vca=1;}
		if(string(XB->name[XB->Ntype])=="int"){cb=XB->name[XB->Ntype];vcb=1;}
		if(string(XA->name[XA->Ntype])=="float"){ca=XA->name[XA->Ntype];vca=2;}
		if(string(XB->name[XB->Ntype])=="float"){cb=XB->name[XB->Ntype];vcb=2;}
		if(string(XA->name[XA->Ntype])=="double"){ca=XA->name[XA->Ntype];vca=3;}
		if(string(XB->name[XB->Ntype])=="double"){cb=XB->name[XB->Ntype];vcb=3;}
		if(string(XA->name[XA->Ntype])=="digit"){ca=XA->name[XA->Ntype];vca=4;}
		if(string(XB->name[XB->Ntype])=="digit"){cb=XB->name[XB->Ntype];vcb=4;}
		if(n>10)if(!xa){
			CVARIANT*CV=new(CVARIANT);
			*CV=*XA;
			XA=CV;
			xa=1;
			}
		if(!(n==17 || n==18)) // ==(17) !=(18)
		if(string(XB->name[XB->Ntype])=="string" && ca)XA->TransformType("string");
		if(!(ca && cb))ca=NULL;
		if(ca){
			if(vca>vcb)XB->TransformType(ca);
			if(vca<vcb)XA->TransformType(cb);
			}else
			if(!(n==17 || n==18)) // ==(17) !=(18)
			{
			ca=XA->name[XA->Ntype];
			XB->TransformType(ca);
			}
		}
	if(n<10){ // += -= ...
		XA->OPETATOR(XA,XB,m[n]);
		V=XA;
		if(xa){
			Pset->sub->Bloki.erase(A);
			}
		if(xb)if(XB){
			Pset->sub->Bloki.erase(B);
			delete XB;
			}
		Pset->sub->Bloki[this]=TRAVERS(V,xa);
		return xa;
		}
	// "||","&&","|","^","&"   ("<",">",) ">>","<<","+","-","*","/","%"
	if(n>=12 && n<=16 || n>=23 && n<=29){
		string op=m[n];
		op+="=";
		if(!xa){
			CVARIANT*CV=new(CVARIANT);
			if(XA)*CV=*XA;
			XA=CV;
			xa=1;
			}
		CVARIANT ProblemDive = *XB;
		bool error=0;
		if(n==29){ // %
			bool isZero;
			if(ProblemDive.isType("digit")){
				isZero = ProblemDive.DATA.digitVal->isZero();
				} else {
				ProblemDive.TransformType("double");
				isZero = (*ProblemDive.DATA.dblVal==0);
				}
			if(isZero){
				error = 1;
				cout<<"% by zero"<<endl;
				}
			if(XA->isType("void")){
				XA->avtoSet("int");
				XA->DATA.intVal = rand();
				}
			}
		if(n==28){// dive bu zero
			bool isZero;
			if(ProblemDive.isType("digit")){
				isZero = ProblemDive.DATA.digitVal->isZero();
				} else {
				ProblemDive.TransformType("double");
				isZero = (*ProblemDive.DATA.dblVal==0);
				}
			if(isZero){
				error = 1;
				cout<<"divide by zero"<<endl;
				}else{
				ProblemDive = *XA;
				ProblemDive.TransformType("double");
				CVARIANT B2;
				B2=*XB;
				B2.TransformType("double");
				CVARIANT::OPETATOR(&ProblemDive,&B2,"/=");
				}
			}
		if(!error)CVARIANT::OPETATOR(XA,XB,op.c_str());
		if(n==28)if(!XA->isType("digit")){
			CVARIANT B1;
			B1 = *XA;
			B1.TransformType("double");
			if(ProblemDive!=B1)*XA = ProblemDive;
			}
		V=XA;
		if(xa)if(XA){
			Pset->sub->Bloki.erase(A);
			}
		if(xb)if(XB){
			Pset->sub->Bloki.erase(B);
			delete XB;
			}
		Pset->sub->Bloki[this]=TRAVERS(V,xa);
		return xa;
		}else{
		if(n==33){
			int t=rand()%2;
			V=t?XA:XB;
			if(xa)if(XA){
				Pset->sub->Bloki.erase(A);
				if(!t)delete XA;
				}
			if(xb)if(XB){
				Pset->sub->Bloki.erase(B);
				if(t)delete XB;
				}
			Pset->sub->Bloki[this]=TRAVERS(XA,xa);
			return t?xa:xb;
			}
		if(n!=10){// == != = < > <= >=
			bool t=CVARIANT::compare(XA,XB,m[n]);
			V=new(CVARIANT);
			
			V->avtoSet("bool");
			V->DATA.boolVal=t;
				
			if(xa)if(XA){
				Pset->sub->Bloki.erase(A);
				delete XA;
				}
			if(xb)if(XB){
				Pset->sub->Bloki.erase(B);
				delete XB;
				}
			Pset->sub->Bloki[this]=TRAVERS(V,1);
			return 1;
			}
		}
	// n==10  m[n]="="
	if(XB && XA)CVARIANT::OPETATOR(XA,XB,m[n]);
	if(xa)if(XA){
		Pset->sub->Bloki.erase(A);
		}
	if(xb)if(XB){
		Pset->sub->Bloki.erase(B);
		delete XB;
		}
	Pset->sub->Bloki[this]=TRAVERS(XA,xa);
	V=XA;
	return xa;
}




int AlgoSet::ZapuskTree(I*Pset,MAIN*M,CVARIANT*&V){
	TRAVERS*T=Border(Pset,M);
	if(T){V=T->X;return T->free;}
	V_pCVARIANT problem;
	V_I BS;
	V=new(CVARIANT);
	V->avtoSet("vector");
	int i;
	for(i=0;i<nabor.size();++i){
		CVARIANT*CV=NULL;
		int b=nabor[i]->ZapuskTree(Pset,M,CV);
		Laver*L=M->tableLavers[Pset->Laver];
		if(L->ExtraExit)return 0;
		problem.push_back(CV);
		BS.push_back(b);
		}
	for(i=0;i<nabor.size();++i){
		CVARIANT *P = problem[i];
		int b = BS[i];
		if(!P){
			P = new CVARIANT();
			P->avtoSet("void");
			b=1;
			}
		if(!b)P = P->copy();
		V->DATA.vectorVal->push_back(P);
		Pset->sub->Bloki.erase(nabor[i]);
		}
	Pset->sub->Bloki[this]=TRAVERS(V,1);
	return 1;
}


int Cast::ZapuskTree(I*Pset,MAIN*M,CVARIANT*&V){
	TRAVERS*T=Border(Pset,M);
	if(T){V=T->X;return T->free;}
	CVARIANT*CV=NULL;
	Laver*L=M->tableLavers[Pset->Laver];
	int b=X->ZapuskTree(Pset,M,CV);
	if(L->ExtraExit)return 0;
	if(!CV)return 0;
	Pset->sub->Bloki.erase(X);
	V=CV;
	if(!b){
		V=new(CVARIANT);
		*V=*CV;
		b=1;
		}
	bool isToFunction = (cast.name=="function");
	if(isToFunction){
		//if(!V->isType("function")):
		bool isNameFunction = V->isType("string");
		bool isNeedNewF = V->isType("void");
		if(isNameFunction){
			string s=*V->DATA.ps;
			if(b)delete V;
			Function*F=NULL;
			F=M->getFunction(Pset,s);
			if(F){
				F->controlSxema();
				V=new(CVARIANT);
				V->avtoSet("function");
				CFunction P(F);
				P.isOriginal=1;
				(*V->DATA.functionVal)=P;
				}else{
				V=new(CVARIANT);
				V->avtoSet("void");
				}
			b=1;
			}
		if(isNeedNewF)V->TransformType("function");
		}else{
		V->TransformType(cast.name.c_str());
		}
	Pset->sub->Bloki[this]=TRAVERS(V,b);
	return b;
}


string Prefix::getAdress(){
	string s=m[n];
	s+=X->getAdress();
	return s;
}


int Prefix::ZapuskTree(I*Pset,MAIN*M,CVARIANT*&V){
	TRAVERS*T=Border(Pset,M);
	if(T){V=T->X;return T->free;}
	V=NULL;
	Laver*L=M->tableLavers[Pset->Laver];
	string s=m[n],linkObj;
	bool k=(s=="--" || s=="++");
	if(k)++L->rop;
	int b=X->ZapuskTree(Pset,M,V);
	if(k)--L->rop;
	if(L->ExtraExit)return 0;
	Pset->sub->Bloki.erase(X);
	if(s=="new"){
		if(!b){
			if(V)V = V->copy(); else{
				V = new(CVARIANT);
				V->avtoSet("void");
				}
			}
		FRAGMENT*F=Pset->Fundament;
		F->Dynamic[M->NfreeOBJ]=V;
		V=new(CVARIANT);
		V->avtoSet("pointer");
		*V->DATA.ps=string("*$:")+SCANER::toString(M->NfreeOBJ)+";";
		++M->NfreeOBJ;
		b = 1;
		}
	if(V){
		bool isInvert = (s=="-" || s=="!" || s=="~");
		if(isInvert || s=="+"){
			if(!b){
				V = V->copy();
				/*
				CVARIANT*CV=new(CVARIANT);
				*CV=*V;
				V=CV;
				*/
				b = 1;
				}
			if(s=="!"){
				if(V->isType("void"))return b;
				V->TransformType("bool");
				}
			if(s=="-" || s=="+"){
				V->TransformType("double");
				double x = *V->DATA.dblVal;
				if(x==(double)(int)x)V->TransformType("int");
				}
			if(isInvert)V->INVERT(m[n]);
			}
		if(s=="--")--*V;
		if(s=="++")++*V;
		}
	if(s=="&"){
		string s="*"+Pset->sub->adres;
		if(b)if(V){
			//Pset->sub->Bloki.erase(X);
			delete V;
			}
		if(V)Pset->sub->Bloki.erase(X);
		V=new(CVARIANT);
		V->avtoSet("pointer");
		*V->DATA.ps=s;
		b=1;
		}
	if(s=="*")if(V->isType("string") || V->isType("pointer")){
		CVARIANT*P=V;
		string s=*V->DATA.ps;
		if(!s.find("*")){
			s=s.substr(1);
			Pset->sub->adres=s;
			V=M->getUnLink2(Pset,s);
			if(V){
				//Pset->sub->Bloki.erase(X);
				if(b){
					V=V->copy();
					delete P;
					}
				}
			}else{
			if(!b)V=new(CVARIANT);
			V->avtoSet("void");
			b=1;
			}
		}
	if(s=="::"){
		string s=getAdress();
		Pset->sub->adres=s;
		bool ok=0;
		V=M->getUnLink(Pset,s,ok);
		}
	Pset->sub->Bloki[this]=TRAVERS(V,b);
	return b;
}



int Sufix::ZapuskTree(I*Pset,MAIN*M,CVARIANT*&V){
	TRAVERS*T=Border(Pset,M);
	if(T){V=T->X;return T->free;}
	Laver*L=M->tableLavers[Pset->Laver];
	CVARIANT*CV=NULL;
	int b=X->ZapuskTree(Pset,M,CV);
	if(L->ExtraExit)return 0;
	if(!CV)return 0;
	string s=m[n];
	V=new(CVARIANT);
	*V=*CV;
	if(s=="--")--*CV;
	if(s=="++")++*CV;
	if(b){
		Pset->sub->Bloki.erase(X);
		delete CV;
		}
	Pset->sub->Bloki[this]=TRAVERS(V,1);
	return 1;
}




bool AccesMasiv::subAccessOne(MAIN*M,Laver*L,I*Pset,CVARIANT*&CV,CVARIANT*&S,CVARIANT*&V,int&v,int a){
	bool isInSet = 0;
	int i;
	if(S->isType("vector")||S->isType("set")){
		if(CV->isType("void")){
			int n=(S->isType("vector"))?S->DATA.vectorVal->size():S->DATA.setVal->size();
			i=(n?rand()%n:0);
			CV->avtoSet("int");
			CV->DATA.intVal=i;
			}
		if(CV->isType("interval")){
			CInterval *CI = CV->DATA.intervalVal;
			if(S->isType("vector")){
				CI->control();
				int t=CI->A;
				if(CI->A==(double)t){
					if(CI->a==0)++t;
					}else ++t;
				if(t<0)t=0;
				if(CI->a&2)t=0;
				int finaln = CI->B;
				if(CI->B==(double)finaln){
					if(CI->b==0)--finaln;
					}
				int maxfinaln = S->DATA.vectorVal->size()-1;
				if(CI->b&2)finaln=maxfinaln;
				if(finaln>maxfinaln)finaln=maxfinaln;
				V=new(CVARIANT);
				V->avtoSet("vector");
				for(i=t;i<=finaln;++i)
					V->DATA.vectorVal->push_back((*S->DATA.vectorVal)[i]->copy());
				v=1;
				}
			} else {
			CV->TransformType("int");
			i=CV->DATA.intVal;
			if(S->isType("vector"))if(i<0){
				i+=S->DATA.vectorVal->size();
				CV->DATA.intVal=i;
				}
			CV->TransformType("string");
			Pset->sub->adres+=(string)"."+*CV->DATA.ps;
			if(S->isType("vector")){
				if(i>=0&&i<S->DATA.vectorVal->size())V=(*S->DATA.vectorVal)[i];
				}else if(i>=0&&i<S->DATA.setVal->size()){
				S_CVARIANT::iterator it=S->DATA.setVal->begin();
				int p;
				for(p=0;it!=S->DATA.setVal->end();++it,++p)if(p==i)break;
				V=&*it;
				isInSet = 1;
				}
			v=0;
			if(!V){//send: out of range
				if(this->up)cout<<"out of range in: "<<this->up->toString().c_str()<<endl;
				Algorithm*X=this->up,*Y=NULL;
				while(X){
					Y=X;
					X=X->up;
					}
				if(this->up!=Y)if(Y)cout<<Y->toString().c_str()<<endl;
				V=new(CVARIANT);
				V->avtoSet("void");
				v=1;
				}
			}
		}
	if(S->isType("string") || S->isType("pointer")){
		bool isLoop=0;
		if(CV->isType("string"))isLoop=(*CV->DATA.ps=="*");
		if(isLoop){
			CVARIANT*P=S;
			string s=*S->DATA.ps;
			if(!s.find("*")){
				s=s.substr(1);
				Pset->sub->adres=s;
				V=M->getUnLink2(Pset,s);
				v=0;
				}else{
				V=new(CVARIANT);
				V->avtoSet("void");
				v=1;
				}
			return isInSet;
			}
		CV->TransformType("int");
		i=CV->DATA.intVal;
		if(i>=0&&i<S->DATA.ps->size()){
			V=new(CVARIANT);
			if(L->rop){
				string str="&char:";
				if(!a){//find in memory
					str+=Pset->sub->adres+";";
					}
				str+=SCANER::toString(i);
				V->avtoSet("string");
				*V->DATA.ps=str;
				}else{
				V->avtoSet("char");
				V->DATA.bVal=(*S->DATA.ps)[i];
				}
			v=1;
			}
		}
	if(S->isType("map")){
		bool u=L->rop>0;
		CV->TransformType("string");
		string adr = *CV->DATA.ps;
		if(S->DATA.mapVal->find(adr)!=S->DATA.mapVal->end())u=1;
		if(u){
			V=&(*S->DATA.mapVal)[adr];
			Pset->sub->adres+=(string)"."+*CV->DATA.ps;
			v=0;
			}else{
			V=new(CVARIANT);
			V->avtoSet("void");
			v=1;
			}
		}
	return isInSet;
}




int AccesMasiv::ZapuskTree(I*Pset,MAIN*M,CVARIANT*&V){
	TRAVERS*T=Border(Pset,M);
	if(T){V=T->X;return T->free;}
	Laver*L=M->tableLavers[Pset->Laver];
	CVARIANT*CV=NULL,*S=NULL;
	int rop=L->rop;
	L->rop=0;
	int b=P->ZapuskTree(Pset,M,CV),a,v; // pos
	L->rop=rop;
	if(L->ExtraExit)return 0;
	if(!CV)b=0;
	if(!b){
		if(CV)CV = CV->copy(); else CV = new(CVARIANT);
		/*
		CVARIANT*CV2=new(CVARIANT);
		if(CV)*CV2=*CV;else CV2->avtoSet("void");
		CV=CV2;
		*/
		b = 1;
		}
	++L->rop;
	a=X->ZapuskTree(Pset,M,S);
	--L->rop;
	if(L->ExtraExit)return 0;
	if(!S){
		V=new(CVARIANT);
		V->avtoSet("void");
		return 1;
		}
	if(S->isType("void")){
		V=new(CVARIANT);
		V->avtoSet("void");
		return 1;
		}

	bool isInSet = 0;
	bool isVectorAccess = CV->isType("vector");
	if(isVectorAccess){
		CVARIANT*Data = S;
		V = Data;
		v = a;
		int aa = a;
		bool first = 1,isInSetX;
		V_pCVARIANT::iterator it = CV->DATA.vectorVal->begin();
		for(;it!=CV->DATA.vectorVal->end();++it){
			CVARIANT*Q = *it;
			isInSetX = subAccessOne(M,L,Pset,Q,Data,V,v,aa);
			if(isInSetX)isInSet = 1;
			if(aa && !first){
				if(Data)delete Data;
				}
			first = 0;
			Data = V;
			aa = v;
			}
		} else isInSet = subAccessOne(M,L,Pset,CV,S,V,v,a);

	if(isInSet && L->rop){
		if(!v || !V)V = new CVARIANT();
		v=1;
		V->avtoSet("void");
		}

	if(b)if(CV){
		Pset->sub->Bloki.erase(P);
		delete CV;
		}
	if(a)if(S){
		if(!v)if(V)V=V->copy();
		v=1;
		Pset->sub->Bloki.erase(X);
		delete S;
		}
	if(V)Pset->sub->Bloki[this]=TRAVERS(V,v);
	return v;
}



int Base3::ZapuskTree(I*Pset,MAIN*M,CVARIANT*&W){
	TRAVERS*T=Border(Pset,M);
	if(T){W=T->X;return T->free;}
	Laver*L=M->tableLavers[Pset->Laver];
	CVARIANT*CV=NULL;
	int b=A->ZapuskTree(Pset,M,CV);
	if(L->ExtraExit)return 0;
	CVARIANT C;
	if(CV)C=*CV;
	if(C.isType("void")){
		C.avtoSet("bool");
		C.DATA.boolVal=(rand()%2)?1:0;
		Pset->sub->Bloki[A]=TRAVERS(new CVARIANT(C),b=1);
		}
	C.TransformType("bool");
	Algorithm*U=C.DATA.boolVal?V:B;
	int b2=U->ZapuskTree(Pset,M,W);
	if(L->ExtraExit)return 0;
	Pset->sub->Bloki[this]=TRAVERS(W,b2);
	Pset->sub->Bloki.erase(U);
	if(b){
		delete Pset->sub->Bloki[A].X;
		Pset->sub->Bloki.erase(A);
		}
	return b2;
}



int AComposition::ZapuskTree(I*Pset,MAIN*M,CVARIANT*&V){
	TRAVERS*T=Border(Pset,M);
	if(T){V=T->X;return T->free;}
	V=new(CVARIANT);
	V->avtoSet("map");
	M_SA::iterator it=table.begin();
	for(;it!=table.end();++it){
		string k=it->first;
		CVARIANT*CV=NULL;
		int b=it->second->ZapuskTree(Pset,M,CV);
		Laver*L=M->tableLavers[Pset->Laver];
		if(L->ExtraExit)return 0;
		if(CV)(*V->DATA.mapVal)[k]=*CV;
		if(b)if(CV){
			Pset->sub->Bloki.erase(it->second);
			delete CV;
			}
		}
	Pset->sub->Bloki[this]=TRAVERS(V,1);
	return 1;
}



int CreateVar::ZapuskTree(I*Pset,MAIN*M,CVARIANT*&V){
	TRAVERS*T=Border(Pset,M);
	if(T){V=T->X;return T->free;}
	FRAGMENT*F=Pset->Fundament;
	while(F->Memorys.empty())F=F->Prev;
	CVARIANT*CV=*F->Memorys.rbegin();
	CVARIANT B,*C=NULL;
	string A=name;
	B.avtoSet("int");
	Laver*L=M->tableLavers[Pset->Laver];
	if(!Init)(*CV->DATA.mapVal)[A]=B;else{
		int b=Init->ZapuskTree(Pset,M,C);
		if(L->ExtraExit&&L->off){
			FRAGMENT*H=Pset->Fundament->Prev,*P;
			int ok=0;
			while(H){
				if(H->Memorys.size()){
					CVARIANT*tmp=H->Memorys[H->Memorys.size()-1];
					int i;
					for(i=0;i<H->Next.size();++i){
						P=H->Next[i];
						P->Memorys.insert(P->Memorys.begin(),tmp->copy());
						M->ProgresMemory(Pset,**P->Memorys.begin(),*tmp);
						}
					delete tmp;
					H->Memorys.pop_back();
					ok=1;
					}
				if(ok)break;
				H=H->Prev;
				}
			CV=*F->Memorys.rbegin();
			}
		if(L->ExtraExit)return 0;
		if(C)(*CV->DATA.mapVal)[A]=*C;
		if(b)if(C){
			Pset->sub->Bloki.erase(Init);
			delete C;
			}
		}
	Pset->sub->Bloki[this]=TRAVERS(V,0);
	return 0;
}



int IF::ZapuskTree(I*Pset,MAIN*M,CVARIANT*&V){
	int b=X->ZapuskTree(Pset,M,V);
	Laver*L=M->tableLavers[Pset->Laver];
	if(L->ExtraExit)return 0;
	L->condition=0;
	if(V)if(!V->isType("void")){
		CVARIANT CV=*V;
		CV.TransformType("bool");
		L->condition=CV.DATA.boolVal?2:1;
		}
	Pset->sub->Bloki.erase(X);
	return b;
}



int SpecSumbol2::ZapuskTree(I*Pset,MAIN*M,CVARIANT*&V){
	TRAVERS*T=Border(Pset,M);
	if(T){
		V=T->X;
		return T->free;
		}
	int b=X->ZapuskTree(Pset,M,V);
	Laver*L=M->tableLavers[Pset->Laver];
	if(L->ExtraExit)return 0;
	if(n==1){//delete
		V->TransformType("string");
		string s=*V->DATA.ps;
		if(!s.find("*$:")){
			s=s.substr(3);
			int n=0;
			sscanf(s.c_str(),"%d",&n);
			FRAGMENT*F=Pset->Fundament;
			CVARIANT*R=F->getDynamicValue(n);
			if(R){
				F->Dynamic.erase(n);
				delete R;
				}
			}
		}
	if(n==0){//return
		Algorithm*A=NULL;
		if(Pset->sub->Return)A=Pset->sub->Return;
		delete Pset->sub;
		Pset->sub=NULL;
		Pset->posicse.pop_back();
		if(Pset->posicse.size())Pset->sub=*Pset->posicse.rbegin();
		if(Pset->sub){
			if(A){
				if(!b)if(V)V=V->copy();
				if(Pset->sub->Bloki[A].free)
				if(Pset->sub->Bloki[A].X)delete Pset->sub->Bloki[A].X;
				b=1;
				Pset->sub->Bloki[A]=TRAVERS(V,b);
				}
			L->ExtraExit=1;
			while(!M->PAGECLOSE(Pset,0));
			}else L->off=Pset->off=1;
		return 0;//no del return value.
		}
	return b;
}




int BaseFunction::ZapuskTree(I*Pset,MAIN*M,CVARIANT*&V){
	TRAVERS*T=Border(Pset,M);
	if(T){V=T->X;return T->free;}
	V=new CVARIANT;
	V->avtoSet("function");
	if(V->DATA.functionVal->F)delete V->DATA.functionVal->F;
	Function*F=new Function;
	F->argumentsON=argumentsON;
	F->Body=Body->copy();
	F->Body->initUP(NULL);
	F->ret.name="var";
	F->names=argumentsNames;
	F->name="subFunction";
	int i;
	for(i=0;i<F->names.size();++i)F->tips.push_back(Type());
	V->DATA.functionVal->F=F;
	Pset->sub->Bloki[this]=TRAVERS(V,1);
	return 1;
}





int Interval::ZapuskTree(I*Pset,MAIN*M,CVARIANT*&V){
	TRAVERS*T=Border(Pset,M);
	if(T){V=T->X;return T->free;}
	CVARIANT*CVA=NULL,*CVB=NULL;
	int ba,bb;
	ba=A->ZapuskTree(Pset,M,CVA);
	Laver*L=M->tableLavers[Pset->Laver];
	if(L->ExtraExit)return 0;
	if(!CVA)return 0;
	bb=B->ZapuskTree(Pset,M,CVB);
	if(L->ExtraExit)return 0;
	if(!CVB)return 0;
	CVARIANT IA=*CVA,IB=*CVB;
	bool isa8,isb8;
	isa8 = IA.isType("void");
	isb8 = IB.isType("void");
	IA.TransformType("double");
	IB.TransformType("double");
	double ia,ib;
	ia = *IA.DATA.dblVal;
	ib = *IB.DATA.dblVal;
	V = new(CVARIANT);
	V->avtoSet("interval");
	(*V->DATA.intervalVal)=CInterval(a+2*isa8,ia,ib,b+2*isb8);
	V->DATA.intervalVal->control();
	if(ba)if(CVA){
		Pset->sub->Bloki.erase(A);
		delete CVA;
		}
	if(bb)if(CVB){
		Pset->sub->Bloki.erase(B);
		delete CVB;
		}
	Pset->sub->Bloki[this]=TRAVERS(V,1);
	return 1;
}


// zapusk2.cpp	:-|