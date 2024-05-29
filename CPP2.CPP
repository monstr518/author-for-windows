//	CPP2.cpp

#include "main.h"


//--------------------------------------------------------------------------------------------------
void CallFunc::forchar(CVARIANT*thisVal,string&name,CVARIANT*&V,V_CVARIANT&VCV){
	char thisChar = thisVal->DATA.bVal;
	string skobki = "([{}])";
	bool isSkobki = (skobki.find(thisChar)!=-1);
	if(name=="isSkobki"){
		V=new(CVARIANT);
		V->avtoSet("bool");
		V->DATA.boolVal = isSkobki;
		return;
		}
	if(!isSkobki)isSkobki = (thisChar=='<' || thisChar=='>');
	string Open = "([{<";
	bool isOpen = (Open.find(thisChar)!=-1);
	if(name=="isOpen"){
		V=new(CVARIANT);
		if(!isSkobki)return;
		V->avtoSet("bool");
		V->DATA.boolVal = isOpen;
		return;
		}
	if(name=="isClose"){
		V=new(CVARIANT);
		if(!isSkobki)return;
		V->avtoSet("bool");
		V->DATA.boolVal = !isOpen;
		return;
		}
}





//--------------------------------------------------------------------------------------------------
void CallFunc::forinterval(CVARIANT*thisVal,string&name,CVARIANT*&V,V_CVARIANT&VCV){
	if(name=="getZ" || name=="getN"){
		bool b=(name=="getN");
		V=new(CVARIANT);
		V->avtoSet("vector");
		CInterval*X=thisVal->DATA.intervalVal;
		int i=X->A;
		if(i<X->A)++i;
		if(i==X->A)if(!X->a)++i;
		for(;i<=X->B;++i){
			if(i==X->B)if(!X->b)break;
			if(b)if(i<0)continue;
			CVARIANT *CV = new CVARIANT();
			CV->avtoSet("int");
			CV->DATA.intVal=i;
			V->DATA.vectorVal->push_back(CV);
			}
		}
	if(name=="contains" || name=="belongs" || name=="in")if(VCV.size()>0){
		CInterval*X=thisVal->DATA.intervalVal;
		VCV[0].TransformType("double");
		double N = *VCV[0].DATA.dblVal;
		bool ok=(X->A<N && X->B>N);
		if(X->A==N && X->a)ok=1;
		if(X->B==N && X->b)ok=1;
		V = new(CVARIANT);
		V->avtoSet("bool");
		V->DATA.boolVal=ok;
		}
	if(name=="A" || name=="B"){
		CInterval*X=thisVal->DATA.intervalVal;
		double n = (name=="A")?X->A:X->B;
		V = new(CVARIANT);
		V->avtoSet("double");
		*V->DATA.dblVal = n;
		}
	if(name=="a" || name=="b" || name=="limA" || name=="limB"){
		CInterval*X=thisVal->DATA.intervalVal;
		V=new(CVARIANT);
		V->avtoSet("bool");
		V->DATA.boolVal=(((name=="a" || name=="limA")?X->a:X->b)!=0);
		}
	if(name=="setA" || name=="setB")if(VCV.size()>0){
		bool is8 = VCV[0].isType("void");
		VCV[0].TransformType("double");
		CInterval*X=thisVal->DATA.intervalVal;
		double*n=(name=="setA")?&X->A:&X->B;
		*n = *VCV[0].DATA.dblVal;
		if(is8){
			char*x=(name=="setA")?&X->a:&X->b;
			*x|=2;
			}
		}
	if(name=="setLimA" || name=="setLimB")if(VCV.size()>0){
		VCV[0].TransformType("bool");
		CInterval*X=thisVal->DATA.intervalVal;
		char*x=(name=="setLimA")?&X->a:&X->b;
		*x|=(VCV[0].DATA.boolVal?1:0);
		}
	if(name=="U" || name=="notU")if(VCV.size()>0){
		if(!VCV[0].isType("interval"))return;
		CInterval*X=thisVal->DATA.intervalVal,*B;
		CInterval Y;
		Y=*X;
		B=VCV[0].DATA.intervalVal;
		bool ok;
		if(name=="U")ok=(Y|=*B);else ok=(Y&=*B);
		V=new(CVARIANT);
		if(!ok){
			V->avtoSet("bool");
			V->DATA.boolVal=0;
			return;
			}
		V->avtoSet("interval");
		*V->DATA.intervalVal=Y;
		}
}












void CallFunc::forpointer(I*Pset,CVARIANT*thisVal,string&name,CVARIANT*&V,V_CVARIANT&VCV,MAIN*M){
	bool ok = (name=="isNULL");
	if(name=="isset" || ok){
		bool isset = 0;
		string s=*thisVal->DATA.ps;
		if(!s.find("*")){
			s=s.substr(1);
			V=M->getUnLink2(Pset,s);
			isset = (V!=NULL);
			}
		V = new(CVARIANT);
		V->avtoSet("bool");
		if(ok)isset = !isset;
		V->DATA.boolVal = isset;
		return;
		}
	ok = (name=="isDynamic");
	if(name=="isStatic" || ok){
		string s=*thisVal->DATA.ps;
		bool isDynamic = !s.find("*$:");
		V = new(CVARIANT);
		V->avtoSet("bool");
		if(!ok)isDynamic = !isDynamic;
		V->DATA.boolVal = isDynamic;
		return;
		}
}




V_pCVARIANT::iterator find_pointer(V_pCVARIANT::iterator A,const V_pCVARIANT::iterator B,const CVARIANT&V){
	while(A!=B){
		if(**A==V)return A;
		++A;
		}
	return A;
}



void CallFunc::forvector(CVARIANT*thisVal,string&name,CVARIANT*&V,V_CVARIANT&VCV){
	if(name=="in")if(VCV.size()>0){
		V_pCVARIANT*S=thisVal->DATA.vectorVal;
		bool b=find_pointer(S->begin(),S->end(),VCV[0])!=S->end();
		V=new(CVARIANT);
		V->avtoSet("bool");
		V->DATA.boolVal=b;
		return;
		}
	if(name=="find" || name=="findOf" || name=="indexOf")if(VCV.size()>0){
		int i,j;
		for(j=-1,i=0;i<thisVal->DATA.vectorVal->size();++i)
			if(*(*thisVal->DATA.vectorVal)[i]==VCV[0]){j=i;break;}
		V=new(CVARIANT);
		V->avtoSet("int");
		V->DATA.intVal=j;
		return;
		}
	if(name=="size" || name=="length"){
		V=new(CVARIANT);
		V->avtoSet("int");
		V->DATA.intVal=thisVal->DATA.vectorVal->size();
		return;
		}
	if(name=="empty"){
		V=new(CVARIANT);
		V->avtoSet("bool");
		V->DATA.boolVal=thisVal->DATA.vectorVal->empty();
		return;
		}
	if(name=="export"){
		string e="{";
		int i;
		for(i=0;i<thisVal->DATA.vectorVal->size();++i){
			string s;
			CVARIANT*X=(*thisVal->DATA.vectorVal)[i];
			bool strok=1;
			if(X->isType("vector")){
				strok=0;
				forvector(X,name,V,VCV);
				if(V){
					s=*V->DATA.ps;
					delete V;
					V=NULL;
					}
				}
			if(X->isType("set")){
				strok=0;
				forset(X,name,V,VCV);
				if(V){
					s=*V->DATA.ps;
					delete V;
					V=NULL;
					}
				}
			if(X->isType("map")){
				strok=0;
				formap(X,name,V,VCV);
				if(V){
					s=*V->DATA.ps;
					delete V;
					V=NULL;
					}
				}
			if(X->isType("char")){
				strok=0;
				s="'#'";
				s[1]=X->DATA.bVal;
				}
			if(X->isType("string")){
				strok=0;
				s=SCANER::writeString(*X->DATA.ps);
				}
			if(strok)X->toString(s);
			if(i)e+=",";
			e+=s;
			}
		e+="}";
		V=new(CVARIANT);
		V->avtoSet("string");
		*V->DATA.ps=e;
		return;
		}
	if(name=="explode" || name=="implode"){
		string s="",p="";
		if(VCV.size()>0){
			VCV[0].TransformType("string");
			p=*VCV[0].DATA.ps;
			}
		int i;
		for(i=0;i<thisVal->DATA.vectorVal->size();++i){
			CVARIANT H=*(*thisVal->DATA.vectorVal)[i];
			if(i)s+=p;
			H.TransformType("string");
			s+=*H.DATA.ps;
			}
		V=new(CVARIANT);
		V->avtoSet("string");
		*V->DATA.ps=s;
		return;
		}
	if(name=="push_back" || name=="push")if(VCV.size()>0){
		CVARIANT*W = new CVARIANT();
		*W = VCV[0];
		thisVal->DATA.vectorVal->push_back(W);
		}
	if(name=="push_begin")if(VCV.size()>0){
		CVARIANT*W = new CVARIANT();
		*W = VCV[0];
		thisVal->DATA.vectorVal->insert(thisVal->DATA.vectorVal->begin(),W);
		}
	if(name=="pop_back" || name=="pop")if(thisVal->DATA.vectorVal->size()){
		if(thisVal->DATA.vectorVal->empty())return;
		V=*thisVal->DATA.vectorVal->rbegin();
		thisVal->DATA.vectorVal->pop_back();
		return;
		}
	if(name=="pop_begin")if(thisVal->DATA.vectorVal->size()){
		if(thisVal->DATA.vectorVal->empty())return;
		V=*thisVal->DATA.vectorVal->begin();
		thisVal->DATA.vectorVal->erase(thisVal->DATA.vectorVal->begin());
		return;
		}
	if(name=="erase" || name=="eraseItem")if(VCV.size()>0){
		VCV[0].TransformType("int");
		//V=thisVal->copy();
		int n = VCV[0].DATA.intVal;
		if(n>=0 && n<thisVal->DATA.vectorVal->size())
			thisVal->DATA.vectorVal->erase(thisVal->DATA.vectorVal->begin()+n);
		return;
		}
	if(name=="eraseItems")if(VCV.size()>0){
		V=new(CVARIANT);
		V->avtoSet("bool");
		V->DATA.boolVal = 0;
		if(VCV[0].isType("set") || VCV[0].isType("vector")){
			VCV[0].TransformType("set");
			VCV[0].TransformType("vector");
			V_I S;
			V_pCVARIANT::iterator it = VCV[0].DATA.vectorVal->begin();
			for(;it!=VCV[0].DATA.vectorVal->end();++it)
				if((*it)->isType("int"))S.push_back((*it)->DATA.intVal);
			sort(S.begin(),S.end());
			V_I::reverse_iterator jt = S.rbegin();
			for(;jt!=S.rend();++jt)if(*jt>=0 && *jt<thisVal->DATA.vectorVal->size()){
				delete *(thisVal->DATA.vectorVal->begin()+*jt);
				thisVal->DATA.vectorVal->erase(thisVal->DATA.vectorVal->begin()+*jt);
				V->DATA.boolVal=1;
				}
			return;
			}
		VCV[0].TransformType("int");
		int a, b, i;
		a = VCV[0].DATA.intVal;
		b = -1;
		if(VCV.size()>1){
			VCV[1].TransformType("int");
			b = VCV[1].DATA.intVal;
			}
		if(b<0 || b>thisVal->DATA.vectorVal->size())b = thisVal->DATA.vectorVal->size();
		if(a<0 || a>=b)return;
		i = a;
		for(;i<b;++i)delete (*thisVal->DATA.vectorVal)[i];
		thisVal->DATA.vectorVal->erase(
				thisVal->DATA.vectorVal->begin()+a,
				thisVal->DATA.vectorVal->begin()+b
				);
		V->DATA.boolVal = 1;
		return;
		}
	if(name=="mid")if(VCV.size()>0){
		VCV[0].TransformType("int");
		int a,b;
		a = VCV[0].DATA.intVal;
		b = -1;
		if(VCV.size()>1){
			VCV[1].TransformType("int");
			b = VCV[1].DATA.intVal;
			}
		if(b<0 || b>thisVal->DATA.vectorVal->size())b = thisVal->DATA.vectorVal->size();
		if(a<0)a = 0;
		V = new(CVARIANT);
		V->avtoSet("vector");
		V_pCVARIANT::iterator it;
		for(;a<b;++a){
			it = thisVal->DATA.vectorVal->begin() + a;
			V->DATA.vectorVal->push_back(new CVARIANT(**it));
			}
		return;
		}
	if(name=="insert")if(VCV.size()>1){
		VCV[0].TransformType("int");
		int a=VCV[0].DATA.intVal;
		if(VCV.size()==3){
			// S.insert(S.begin()+3,2,12);
			VCV[1].TransformType("int");
			int n=VCV[1].DATA.intVal;
			CVARIANT*W = new CVARIANT();
			*W = VCV[2];
			thisVal->DATA.vectorVal->insert(thisVal->DATA.vectorVal->begin()+a,n,W);
			return;
			}
		if(VCV[1].isType("vector")){
			// S.insert(S.begin(),R.begin(),R.end());
			thisVal->DATA.vectorVal->insert(
					thisVal->DATA.vectorVal->begin()+a,
					VCV[1].DATA.vectorVal->begin(),
					VCV[1].DATA.vectorVal->end()
					);
			return;
			}
		// S.insert(S.begin()+1,11);
		CVARIANT*W = new CVARIANT();
		*W = VCV[1];
		thisVal->DATA.vectorVal->insert(thisVal->DATA.vectorVal->begin()+a,W);
		return;
		}
	if(name=="sort"){
		V_pCVARIANT*S = thisVal->DATA.vectorVal;
		V_pCVARIANT R;
		int i,t;
		while(S->size()){
			for(i=1,t=0;i<S->size();++i)if(*(*S)[i]<*(*S)[t])t=i;
			R.push_back((*S)[t]);
			S->erase(S->begin()+t);
			}
		R.swap(*S);
		return;
		}
	if(name=="unsort" || name=="asort"){
		V_pCVARIANT*S=thisVal->DATA.vectorVal;
		V_pCVARIANT R;
		V_I Q;
		int i,t;
		for(i=0;i<S->size();++i)Q.push_back(i);
		for(i=0;i<S->size();++i){
			t=::rand()%Q.size();
			R.push_back((*S)[Q[t]]);
			Q.erase(Q.begin()+t);
			}
		R.swap(*S);
		return;
		}
	if(name=="reverse" || name=="revers"){
		V_pCVARIANT*S=thisVal->DATA.vectorVal;
		V_pCVARIANT R;
		int i;
		while(i=S->size()){
			R.push_back((*S)[--i]);
			S->erase(S->begin()+i);
			}
		R.swap(*S);
		return;
		}
	if(name=="end" || name=="getEnd"){
		int n=thisVal->DATA.vectorVal->size();
		if(!n)return;
		V=new(CVARIANT);
		--n;
		*V=**(thisVal->DATA.vectorVal->begin()+n);
		return;
		}
	if(name=="begin" || name=="getBegin" || name=="home"){
		int n=thisVal->DATA.vectorVal->size();
		if(!n)return;
		V=new(CVARIANT);
		*V=**thisVal->DATA.vectorVal->begin();
		return;
		}
	if(name=="sum" || name=="p" || name=="P"){
		double x=(name=="sum"?0:1);
		CVARIANT A;
		V_pCVARIANT::iterator it=thisVal->DATA.vectorVal->begin();
		for(;it!=thisVal->DATA.vectorVal->end();++it){
			A=**it;
			A.TransformType("double");
			if(name=="sum")x += *A.DATA.dblVal; else x *= *A.DATA.dblVal;
			}
		V=new(CVARIANT);
		int n=x;
		if(x==(double)n){
			V->avtoSet("int");
			V->DATA.intVal=n;
			return;
			}
		V->avtoSet("double");
		*V->DATA.dblVal = x;
		return;
		}
	if(name=="newName"){
		int n=0,type=0;
		if(VCV.size()>0){
			VCV[0].TransformType("int");
			n=VCV[0].DATA.intVal;
			}
		if(VCV.size()>1){
			VCV[1].TransformType("int");
			type=VCV[1].DATA.intVal;
			}
		if(!type)type=1;
		if(n<1)n=1;
		string s;
		int nalfa=('z'-'a'+1),ndigit=('9'-'0'+1);
		int size=0;
		if(type&1)size+=nalfa;
		if(type&2)size+=nalfa;
		if(type&4)size+=ndigit;
		int i=0,x,stop=10;
		do{
			s="";
			for(i=0;i<n;++i){
				x=size;
				if(!i && (type&4) && (type&3))x-=ndigit;
				x=::rand()%x;
				char c='.';
				if(type&1){
					if(x<nalfa){
						c='a'+x;
						x=-1;
						}else x-=nalfa;
					}
				if(x>=0 && (type&2)){
					if(x<nalfa){
						c='A'+x;
						x=-1;
						}else x-=nalfa;
					}
				if(x>=0 && (type&4))c='0'+x;
				s+=c;
				}
			int size = thisVal->DATA.vectorVal->size();
			for(i=0;i<size;++i){
				CVARIANT*X=(*thisVal->DATA.vectorVal)[i];
				if(!X->isType("string"))continue;
				if(*X->DATA.ps==s){
					i=-1;
					break;
					}
				}
			--stop;
			if(stop<=0){stop=10;++n;}
			}while(i<0);
		V=new(CVARIANT);
		V->avtoSet("string");
		*V->DATA.ps=s;
		return;
		}
	if(name=="RandomIndex"){
		V_pCVARIANT* Row = thisVal->DATA.vectorVal;
		int sum,i,size = Row->size();
		sum = i = 0;
		for(;i<size;++i){
			CVARIANT*it = (*Row)[i];
			if(it->isType("int"))sum += it->DATA.intVal;
			}
		int pos,t;
		pos = -1;
		if(sum>0){
			t = ::rand()%sum;
			sum = i = 0;
			for(;i<size;++i){
				CVARIANT*it = (*Row)[i];
				if(it->isType("int")){
					sum += it->DATA.intVal;
					if(sum>t){
						pos = i;
						break;
						}
					}
				}
			}
		V=new(CVARIANT);
		V->avtoSet("int");
		V->DATA.intVal = pos;
		return;
		}
	if(name=="U")if(VCV.size()>0){
		VCV[0].TransformType("vector");
		V_pCVARIANT*S=VCV[0].DATA.vectorVal;
		V_pCVARIANT::iterator it=S->begin();
		for(;it!=S->end();++it)thisVal->DATA.vectorVal->push_back(new CVARIANT(**it));
		return;
		}
	if(name=="notU")if(VCV.size()>0){
		VCV[0].TransformType("vector");
		V_pCVARIANT*S=VCV[0].DATA.vectorVal,W;
		int i,j;
		for(i=0;i<thisVal->DATA.vectorVal->size();++i){
			bool ok=0;
			for(j=0;j<S->size();++j)if(*(*thisVal->DATA.vectorVal)[i]==*(*S)[j]){ok=1;break;}
			if(ok){
				W.push_back(new CVARIANT(*(*S)[j]));
				S->erase(S->begin()+j);
				}
			}
		thisVal->avtoSet("vector");
		thisVal->DATA.vectorVal->swap(W);
		return;
		}
	if(name=="findAll")if(VCV.size()>0){
		V=new(CVARIANT);
		V->avtoSet("vector");
		int i,n;
		n = thisVal->DATA.vectorVal->size();
		for(i=0;i<n;++i)
			if(*(*thisVal->DATA.vectorVal)[i]==VCV[0]){
				CVARIANT *X = new CVARIANT();
				X->avtoSet("int");
				X->DATA.intVal=i;
				V->DATA.vectorVal->push_back(X);
				}
		return;
		}
	if(name=="join"){
		string Result, separator;
		if(VCV.size()>0){
			VCV[0].TransformType("string");
			separator = *VCV[0].DATA.ps;
			}
		int i,n;
		n = thisVal->DATA.vectorVal->size();
		CVARIANT S;
		for(i=0;i<n;++i){
			CVARIANT *P = (*thisVal->DATA.vectorVal)[i];
			if(!P->isType("string")){
				S = *P;
				S.TransformType("string");
				P = &S;
				}
			if(i)Result += separator;
			if(P->isType("string"))Result += *P->DATA.ps;
			}
		V=new(CVARIANT);
		V->avtoSet("string");
		*V->DATA.ps = Result;
		return;
		}
	int si=0;
	if(name=="max" || name=="getMax")si=1;
	if(name=="min" || name=="getMin")si=2;
	if(si){
		bool ok;
		CVARIANT *X,*M=NULL;
		int i;
		for(i=0;i<thisVal->DATA.vectorVal->size();++i){
			X=(*thisVal->DATA.vectorVal)[i];
			if(!M){
				M=X;
				continue;
				}
			ok=(si==1)?(*M<*X):(*M>*X);
			if(ok)M=X;
			}
		if(!M)return;
		V=new(CVARIANT);
		*V=*M;
		return;
		}
	si=0;
	if(name=="OR" || name=="or")si=1;
	if(name=="AND" || name=="and")si=2;
	if(si){
		int size=thisVal->DATA.vectorVal->size();
		V=new(CVARIANT);
		V->avtoSet("bool");
		if(!size){
			V->DATA.boolVal=(si==1);
			return;
			}
		bool R=(si==2);
		CVARIANT *X;
		int i;
		bool stop = 0;
		for(i=0;i<size;++i){
			X=(*thisVal->DATA.vectorVal)[i];
			if(X->isType("void"))continue;
			bool needConvert = !X->isType("bool");
			if(needConvert){
				X = X->copy();
				X->TransformType("bool");
				}
			if(si==1){
				if(X->DATA.boolVal){R=1;stop=1;}
				}else{
				if(!X->DATA.boolVal){R=0;stop=1;}
				}
			if(needConvert)delete X;
			if(stop)break;
			}
		V->DATA.boolVal=R;
		return;
		}
	if(name=="isEquale" || name=="isequale"){
		bool isEquale=1;
		int size=thisVal->DATA.vectorVal->size();
		int i;
		for(i=1;i<size;++i){
			CVARIANT&A=*(*thisVal->DATA.vectorVal)[0];
			CVARIANT&B=*(*thisVal->DATA.vectorVal)[i];
			if(A==B)continue;
			isEquale=0;
			break;
			}
		V=new(CVARIANT);
		V->avtoSet("bool");
		V->DATA.boolVal=isEquale;
		return;
		}
}




void CallFunc::forset(CVARIANT*thisVal,string&name,CVARIANT*&V,V_CVARIANT&VCV){
	if(name=="erase")if(VCV.size()>0){
		S_CVARIANT::iterator it=find(thisVal->DATA.setVal->begin(),thisVal->DATA.setVal->end(),VCV[0]);
		if(it!=thisVal->DATA.setVal->end())thisVal->DATA.setVal->erase(it);
		}
	if(name=="in")if(VCV.size()>0){
		bool b=thisVal->DATA.setVal->find(VCV[0])!=thisVal->DATA.setVal->end();
		V=new(CVARIANT);
		V->avtoSet("bool");
		V->DATA.boolVal=b;
		return;
		}
	if(name=="insert")if(VCV.size()>0){
		bool b=thisVal->DATA.setVal->insert(VCV[0]).second;
		V=new(CVARIANT);
		V->avtoSet("bool");
		V->DATA.boolVal=b;
		return;
		}
	if(name=="size"){
		int n=thisVal->DATA.setVal->size();
		V=new(CVARIANT);
		V->avtoSet("int");
		V->DATA.intVal=n;
		return;
		}
	if(name=="empty"){
		bool b=thisVal->DATA.setVal->empty();
		V=new(CVARIANT);
		V->avtoSet("bool");
		V->DATA.boolVal=b;
		return;
		}
	if(name=="implode")if(VCV.size()>=1){
		VCV[0].TransformType("string");
		V=new(CVARIANT);
		V->avtoSet("string");
		S_CVARIANT*S=thisVal->DATA.setVal;
		S_CVARIANT::iterator it=S->begin();
		CVARIANT T;
		int i;
		for(i=0;it!=S->end();++i){
			if(i)*V->DATA.ps+=*VCV[0].DATA.ps;
			T=*it;
			T.TransformType("string");
			*V->DATA.ps+=*T.DATA.ps;
			}
		return;
		}
	if(name=="U")if(VCV.size()>0){
		VCV[0].TransformType("set");
		S_CVARIANT*S=VCV[0].DATA.setVal;
		S_CVARIANT::iterator it=S->begin();
		for(;it!=S->end();++it)thisVal->DATA.setVal->insert(*it);
		return;
		}
	if(name=="notU")if(VCV.size()>0){
		VCV[0].TransformType("set");
		S_CVARIANT*S=VCV[0].DATA.setVal,W;
		S_CVARIANT::iterator it=thisVal->DATA.setVal->begin();
		for(;it!=thisVal->DATA.setVal->end();++it)if(!S->insert(*it).second)W.insert(*it);
		thisVal->DATA.setVal->swap(W);
		return;
		}
	if(name=="export"){
		string e="(set){";
		S_CVARIANT::iterator it=thisVal->DATA.setVal->begin();
		int i;
		for(i=0;it!=thisVal->DATA.setVal->end();++it,++i){
			string s;
			CVARIANT*X=&*it;
			bool strok=1;
			if(X->isType("set")){
				strok=0;
				forset(X,name,V,VCV);
				if(V){
					s=*V->DATA.ps;
					delete V;
					V=NULL;
					}
				}
			if(X->isType("vector")){
				strok=0;
				forvector(X,name,V,VCV);
				if(V){
					s=*V->DATA.ps;
					delete V;
					V=NULL;
					}
				}
			if(X->isType("map")){
				strok=0;
				formap(X,name,V,VCV);
				if(V){
					s=*V->DATA.ps;
					delete V;
					V=NULL;
					}
				}
			if(X->isType("char")){
				strok=0;
				s="'#'";
				s[1]=X->DATA.bVal;
				}
			if(X->isType("string")){
				strok=0;
				s=SCANER::writeString(*X->DATA.ps);
				}
			if(strok)X->toString(s);
			if(i)e+=",";
			e+=s;
			}
		e+="}";
		V=new(CVARIANT);
		V->avtoSet("string");
		*V->DATA.ps=e;
		}
}




void CallFunc::formap(CVARIANT*thisVal,string&name,CVARIANT*&V,V_CVARIANT&VCV){
	if(name=="getKeys"){
		V=new(CVARIANT);
		V->avtoSet("vector");
		M_SV::iterator it=thisVal->DATA.mapVal->begin();
		for(;it!=thisVal->DATA.mapVal->end();++it){
			CVARIANT *S = new CVARIANT();
			S->avtoSet("string");
			*S->DATA.ps=it->first;
			V->DATA.vectorVal->push_back(S);
			}
		}
	if(name=="erase" || name=="delete")if(VCV.size()>0){
		VCV[0].TransformType("string");
		thisVal->DATA.mapVal->erase(*VCV[0].DATA.ps);
		}
	if(name=="empty"){
		V=new(CVARIANT);
		V->avtoSet("bool");
		V->DATA.boolVal=thisVal->DATA.mapVal->empty();
		}
	if(name=="isset" || name=="find")if(VCV.size()>0){
		VCV[0].TransformType("string");
		bool b=thisVal->DATA.mapVal->find(*VCV[0].DATA.ps)!=thisVal->DATA.mapVal->end();
		V=new(CVARIANT);
		V->avtoSet("bool");
		V->DATA.boolVal=b;
		}
	if(name=="export"){
		string e="{";
		M_SV::iterator it=thisVal->DATA.mapVal->begin();
		int i;
		for(i=0;it!=thisVal->DATA.mapVal->end();++i,++it){
			string s;
			CVARIANT*X=&it->second;
			bool strok=1;
			if(X->isType("set")){
				strok=0;
				forset(X,name,V,VCV);
				if(V){
					s=*V->DATA.ps;
					delete V;
					V=NULL;
					}
				}
			if(X->isType("vector")){
				strok=0;
				forvector(X,name,V,VCV);
				if(V){
					s=*V->DATA.ps;
					delete V;
					V=NULL;
					}
				}
			if(X->isType("map")){
				strok=0;
				formap(X,name,V,VCV);
				if(V){
					s=*V->DATA.ps;
					delete V;
					V=NULL;
					}
				}
			if(X->isType("char")){
				strok=0;
				s="'#'";
				s[1]=X->DATA.bVal;
				}
			if(X->isType("string")){
				strok=0;
				s=SCANER::writeString(*X->DATA.ps);
				}
			if(strok)X->toString(s);
			if(i)e+=",";
			e+=SCANER::writeString(it->first)+":"+s;
			}
		e+="}";
		if(e=="{}")e="(map){}";
		V=new(CVARIANT);
		V->avtoSet("string");
		*V->DATA.ps=e;
		return;
		}
}




void CallFunc::forfunction(I*Pset,CVARIANT*thisVal,string&name,CVARIANT*&V,V_CVARIANT&VCV,MAIN*M){
	if(name=="exportText"){
		V=new(CVARIANT);
		V->avtoSet("string");
		Function*F=thisVal->DATA.functionVal->F;
		if(!F)return;
		string s;
		if(!F->Body && F->sxema){
			Sxema*Sxem=F->sxema->copy();
			F->Body=Sxem->Coder();
			delete Sxem;
			}
		s=F->toString();
		*V->DATA.ps=s;
		return;
		}
	if(name=="exportFunction"){
		V=new(CVARIANT);
		V->avtoSet("string");
		Function*F=thisVal->DATA.functionVal->F;
		if(!F)return;
		string s;
		if(!F->Body && F->sxema){
			Sxema*Sxem=F->sxema->copy();
			F->Body=Sxem->Coder();
			delete Sxem;
			}
		s = F->getHead(0);
		int pos = s.find("(");
		s = s.substr(pos);
		s += "=>";
		if(F->Body)s += F->Body->toString();
		*V->DATA.ps=s;
		return;
		}
	if(name=="toString"){
		V=new(CVARIANT);
		V->avtoSet("string");
		*V->DATA.ps=thisVal->DATA.functionVal->toString();
		return;
		}
	if(name=="export" || name=="PROGRAM"){
		Function*F=thisVal->DATA.functionVal->F;
		F->controlSxema();
		Sxema*S=F->sxema->copy();
		Algorithm*Code=S->Coder();
		delete S;
		V=new(CVARIANT);
		if(Code){
			V->avtoSet("program");
			if(V->DATA.programVal)delete V->DATA.programVal;
			V->DATA.programVal=Code;
			return;
			}
		V->avtoSet("string");
		*V->DATA.ps="is not normal sxem";
		return;
		}
	if(name=="Root"){
		Function*F=thisVal->DATA.functionVal->F;
		F->controlSxema();
		F->sxema->controlTable();
		V=new(CVARIANT);
		V->avtoSet("int");
		V->DATA.intVal=F->sxema->HeadEnd->accessNumber;
		return;
		}
	if(name=="Next" || name=="Down" || name=="Up")if(VCV.size()>0){
		VCV[0].TransformType("int");
		int pos=VCV[0].DATA.intVal;
		int result=2;
		if(VCV.size()>1){
			VCV[1].TransformType("bool");
			result=VCV[1].DATA.boolVal;
			}
		Function*F=thisVal->DATA.functionVal->F;
		F->controlSxema();
		F->sxema->controlTable();
		Bloc*X=F->sxema->table[pos];
		if(!X)return;
		V_I S;
		V_B*Y=(name=="Up"?&X->up:&X->down);
		int i;
		if(result<2 && Y->size()==2){
			S.push_back((*Y)[1-result]->accessNumber);
			}else for(i=0;i<Y->size();++i)S.push_back((*Y)[i]->accessNumber);
		V=new(CVARIANT);
		if(S.size()==1){
			V->avtoSet("int");
			V->DATA.intVal=S[0];
			return;
			}
		V->avtoSet("vector");
		for(i=0;i<S.size();++i){
			CVARIANT *N = new CVARIANT();
			N->avtoSet("int");
			N->DATA.intVal=S[i];
			V->DATA.vectorVal->push_back(N);
			}
		return;
		}
	if(name=="getComand" || name=="getCommand")if(VCV.size()>0){
		VCV[0].TransformType("int");
		int pos=VCV[0].DATA.intVal;
		Function*F=thisVal->DATA.functionVal->F;
		F->controlSxema();
		F->sxema->controlTable();
		Bloc*X=F->sxema->table[pos];
		if(!X)return;
		V=new(CVARIANT);
		if(X->A){
			V->avtoSet("program");
			Algorithm*Code=X->A->copy();
			Code->initUP(NULL);
			V->DATA.programVal=Code;
			}else{
			V->avtoSet("int");
			V->DATA.intVal=0;
			}
		return;
		}
	if(name=="setComand" || name=="setCommand" || name=="replaceCommand" || name=="setIF" || name=="setCucle")
	if(VCV.size()>1){
		VCV[0].TransformType("int");
		int pos=VCV[0].DATA.intVal;
		Function*F=thisVal->DATA.functionVal->F;
		F->controlSxema();
		F->sxema->controlTable();
		if(F->sxema->table.find(pos)==F->sxema->table.end())return;
		Bloc*X=F->sxema->table[pos];
		if(!X)return;
		if(VCV[1].isType("program")){
			Algorithm*A=VCV[1].DATA.programVal;
			if(!A->isProsto()){
				if(!A->isIF())return;
				}
			A=A->copy();
			A->initUP(NULL);
			if(X->A)delete X->A;
			X->A=A;
			if(F->file)F->file->NeedSave=1;
			if(F->Body){delete F->Body;F->Body=NULL;}
			return;
			}
		if(!VCV[1].isType("string"))return;
		string error,s=*VCV[1].DATA.ps;
		s="{"+s+";}";
		int er=0;
		Algorithm*A;
		Sequence*S=NULL;
			{
			Assemble A(error,M,"");
			char*c=const_cast<char*>(s.c_str());
			S=A.getNabor(c,er);
			}
		if(er){
			if(S)delete S;
			return;
			}
		if(S->nabor.size()==1){
			A=(*S->nabor.begin())->copy();
			delete S;
			}else A=S;
		if(!A->isProsto()){
			delete A;
			return;
			}
		if(name=="setIF" || name=="setCucle"){
			IF*iif=new(IF);
			iif->X=A;
			A=iif;
			if(name=="setIF"){
				if(X->down.size()==1){
					X->down.push_back(X->down[0]);
					X->down[0]->up.push_back(X);
					}
				}else{
				X->down.push_back(X);
				X->up.push_back(X);
				}
			}
		A->initUP(NULL);
		if(X->A)delete X->A;
		X->A=A;
		if(F->file)F->file->NeedSave=1;
		if(F->Body){delete F->Body;F->Body=NULL;}
		return;
		}
	if(name=="insertDown" || name=="insertUp" || name=="insertUpRang")if(VCV.size()>0){
		VCV[0].TransformType("int");
		int pos=VCV[0].DATA.intVal;
		Function*F=thisVal->DATA.functionVal->F;
		F->controlSxema();
		F->sxema->controlTable();
		if(F->sxema->table.find(pos)==F->sxema->table.end())return;
		Bloc*X=F->sxema->table[pos];
		if(!X)return;
		int poz=-1;
		if(VCV.size()>1){
			if(VCV[1].isType("bool")){poz=!VCV[1].DATA.boolVal;}else{
				VCV[1].TransformType("int");
				poz=VCV[1].DATA.intVal;
				}
			}
		Bloc*BB=new(Bloc),*B2;
		pos=F->sxema->freeN++;
		BB->accessNumber=pos;
		bool ok=0;
		if(name=="insertDown"){
			if(poz<0){
				V_B next;
				next=X->down;
				X->disconnectDown();
				int i;
				for(i=0;i<next.size();++i)next[i]->up.push_back(BB);
				BB->down=next;
				X->down.push_back(BB);
				BB->up.push_back(X);
				}else if(poz<X->down.size()){
				BB->down.push_back(B2=X->down[poz]);
				BB->up.push_back(X);
				X->down[poz]=BB;
				Bloc**pB2=find(B2->up.begin(),B2->up.end(),X);
				*pB2=BB;
				}
			ok=1;
			}
		if(name=="insertUp"){
			if(poz<0){
				int i;
				for(i=0;i<X->up.size();++i){
					B2=X->up[i];
					Bloc**pB2=find(B2->down.begin(),B2->down.end(),X);
					*pB2=BB;
					BB->up.push_back(B2);
					}
				X->up.clear();
				X->up.push_back(BB);
				BB->down.push_back(X);
				}else if(poz<X->up.size()){
				BB->up.push_back(B2=X->up[poz]);
				BB->down.push_back(X);
				X->up[poz]=BB;
				Bloc**pB2=find(B2->down.begin(),B2->down.end(),X);
				*pB2=BB;
				}
			ok=1;
			}
		if(name=="insertUpRang"){
			F->sxema->controlRang();
			V_B ListUP;
			ListUP.push_back(BB);
			V_B::iterator it=X->up.begin();
			for(;it!=X->up.end();++it)if((*it)->rang<X->rang){
				Bloc**pB2=find((*it)->down.begin(),(*it)->down.end(),X);
				*pB2=BB;
				BB->up.push_back(*it);
				}else ListUP.push_back(*it);
			BB->down.push_back(X);
			X->up=ListUP;
			ok=1;
			}
		if(ok){
			if(F->file)F->file->NeedSave=1;
			F->sxema->table[pos]=BB;
			F->sxema->rangOK=0;
			if(F->Body){delete F->Body;F->Body=NULL;}
			}else{
			delete BB;
			pos=-1;
			}
		V=new(CVARIANT);
		V->avtoSet("int");
		V->DATA.intVal=pos;
		return;
		}
	if(name=="getNamesStart"){
		V=new(CVARIANT);
		V->avtoSet("vector");
		Function*F=thisVal->DATA.functionVal->F;
		int i;
		for(i=0;i<F->names.size();++i){
			CVARIANT *S = new CVARIANT();
			S->avtoSet("string");
			*S->DATA.ps=F->names[i];
			V->DATA.vectorVal->push_back(S);
			}
		return;
		}
	if(name=="getName"){
		Function*F=thisVal->DATA.functionVal->F;
		V=new(CVARIANT);
		V->avtoSet("string");
		*V->DATA.ps=F->name;
		return;
		}
	if(name=="setName")if(VCV.size()>0){
		Function*F=thisVal->DATA.functionVal->F;
		VCV[0].TransformType("string");
		F->name=*VCV[0].DATA.ps;
		return;
		}
	if(name=="integrate")if(VCV.size()>0){
		Function*F=thisVal->DATA.functionVal->F;
		if(F->file)return;
		VCV[0].TransformType("string");
		File*Fi=M->findModule(*VCV[0].DATA.ps);
		if(!Fi){
			string s=*VCV[0].DATA.ps;
			int u=s.find_last_of("\\");
			if(u<0)return;
			Fi=new File(M);
			Fi->way=s.substr(0,u);
			Fi->name=s.substr(u+1);
			M->Files[Fi->id]=Fi;
			}
		Function*xf=Fi->FindFunction(F->name);
		if(xf){
			Fi->functions.erase(find(Fi->functions.begin(),Fi->functions.end(),xf));
			delete xf;
			}
		Fi->functions.push_back(F);
		F->file=Fi;
		Fi->NeedSave=1;
		if(F->Body){delete F->Body;F->Body=NULL;}
		return;
		}
	if(name=="delete")if(VCV.size()>0){
		Function*F=thisVal->DATA.functionVal->F;
		F->controlSxema();
		VCV[0].TransformType("int");
		int pos=VCV[0].DATA.intVal;
		F->sxema->controlTable();
		Bloc*X=F->sxema->table[pos];
		if(!X)return;
		if(X->down.size()==2)if(X->down[0]==X->down[1]){
			X->down.pop_back();
			Bloc*E=X->down[0];
			E->up.erase(find(E->up.begin(),E->up.end(),X));
			}
		X->deleted();
		F->sxema->table.erase(X->accessNumber);
		F->sxema->rangOK=0;
		if(F->file)F->file->NeedSave=1;
		if(F->Body){delete F->Body;F->Body=NULL;}
		delete X;
		return;
		}
	if(name=="isProsto" || name=="isIF")if(VCV.size()>0){
		VCV[0].TransformType("int");
		int pos=VCV[0].DATA.intVal;
		Function*F=thisVal->DATA.functionVal->F;
		F->controlSxema();
		F->sxema->controlTable();
		Bloc*X=F->sxema->table[pos];
		if(!X)return;
		V=new(CVARIANT);
		V->avtoSet("bool");
		V->DATA.boolVal=(name=="isIF"?X->A->isIF():X->A->isProsto());
		return;
		}
	if(name=="Rang")if(VCV.size()>0){
		VCV[0].TransformType("int");
		int pos=VCV[0].DATA.intVal;
		Function*F=thisVal->DATA.functionVal->F;
		F->controlSxema();
		F->sxema->controlTable();
		Bloc*X=F->sxema->table[pos];
		if(!X)return;
		F->sxema->controlRang();
		V=new(CVARIANT);
		V->avtoSet("int");
		V->DATA.intVal=X->rang;
		return;
		}
	if(name=="isEmpty")if(VCV.size()>0){
		VCV[0].TransformType("int");
		int pos=VCV[0].DATA.intVal;
		Function*F=thisVal->DATA.functionVal->F;
		F->controlSxema();
		F->sxema->controlTable();
		Bloc*X=F->sxema->table[pos];
		if(!X)return;
		V=new(CVARIANT);
		V->avtoSet("int");
		V->DATA.boolVal=!(X->A);
		return;
		}
	if(name=="insertSub")if(VCV.size()>1){
		VCV[0].TransformType("int");
		if(!VCV[1].isType("function"))return;
		int pos=VCV[0].DATA.intVal;
		Function*F=thisVal->DATA.functionVal->F;
		F->controlSxema();
		F->sxema->controlTable();
		Bloc*X=F->sxema->table[pos];
		if(!X)return;
		if(F->sxema->HeadEnd==X)return;
		if(X->A)delete X->A;
		X->A=NULL;
		Function*FSUB=VCV[1].DATA.functionVal->F;
		FSUB->controlSxema();
		Sxema*Sub=FSUB->sxema->copy();
		Bloc*HeadEnd=Sub->HeadEnd;
		Sub->HeadEnd=NULL;
		delete Sub;
		Bloc*BB=new(Bloc),*B2;
		int i;
		for(i=0;i<X->up.size();++i){
			B2=X->up[i];
			Bloc**pB2=find(B2->down.begin(),B2->down.end(),X);
			*pB2=BB;
			BB->up.push_back(B2);
			}
		X->up.clear();
		for(i=0;i<HeadEnd->down.size();++i){
			B2=HeadEnd->down[i];
			*find(B2->up.begin(),B2->up.end(),HeadEnd)=BB;
			BB->down.push_back(B2);
			}
		HeadEnd->down.clear();
		HeadEnd->down.push_back(X);
		X->up.push_back(HeadEnd);
		F->sxema->rangOK=0;
		F->sxema->table.clear();
		if(F->file)F->file->NeedSave=1;
		if(F->Body){delete F->Body;F->Body=NULL;}
		return;
		}
	if(name=="insert")if(VCV.size()>1){ // {a,b}->new->c
		if(VCV[0].isType("vector") && VCV[1].isType("vector"))return;
		int posA=-1;
		V_pCVARIANT*S=NULL;
		if(VCV[0].isType("vector")){
			VCV[1].TransformType("int");
			posA=VCV[1].DATA.intVal;
			S=VCV[0].DATA.vectorVal;
			}
		if(VCV[1].isType("vector")){
			VCV[0].TransformType("int");
			posA=VCV[0].DATA.intVal;
			S=VCV[1].DATA.vectorVal;
			}
		int posB=-1;
		V_I POS;
		if(!S){
			VCV[0].TransformType("int");
			VCV[1].TransformType("int");
			posA=VCV[0].DATA.intVal;
			posB=VCV[1].DATA.intVal;
			POS.push_back(posB);
			}else{
			int i;
			for(i=0;i<S->size();++i){
				(*S)[i]->TransformType("int");
				POS.push_back((*S)[i]->DATA.intVal);
				}
			}
		if(POS.empty())return;
		Function*F=thisVal->DATA.functionVal->F;
		F->controlSxema();
		F->sxema->controlTable();
		Bloc*A=F->sxema->table[posA],*X;
		if(!A)return;
		V_B B;
		int i;
		for(i=0;i<POS.size();++i){
			X=F->sxema->table[POS[i]];
			if(X)B.push_back(X);
			}
		if(B.empty())return;
		bool ok=1,good=0;
		for(i=0;i<B.size();++i)
			if(find(A->down.begin(),A->down.end(),B[i])==A->down.end())ok=0;
		Bloc*C=NULL;
		if(ok){
			C=new(Bloc);
			for(i=0;i<B.size();++i){
				Bloc**p=find(A->down.begin(),A->down.end(),B[i]);
				if(!i)*p=C;else A->down.erase(p);
				p=find(B[i]->up.begin(),B[i]->up.end(),A);
				*p=C;
				C->down.push_back(B[i]);
				}
			C->up.push_back(A);
			good=1;
			}else{
			ok=1;
			for(i=0;i<B.size();++i)
				if(find(A->up.begin(),A->up.end(),B[i])==A->up.end())ok=0;
			if(ok){
				C=new(Bloc);
				for(i=0;i<B.size();++i){
					Bloc**p=find(A->up.begin(),A->up.end(),B[i]);
					if(!i)*p=C;else A->up.erase(p);
					p=find(B[i]->down.begin(),B[i]->down.end(),A);
					*p=C;
					C->up.push_back(B[i]);
					}
				C->down.push_back(A);
				good=1;
				}
			}
		if(!good)return;
		F->sxema->rangOK=0;
		if(F->file)F->file->NeedSave=1;
		if(F->Body){delete F->Body;F->Body=NULL;}
		if(!C)return;
		C->accessNumber=F->sxema->freeN++;
		F->sxema->table[C->accessNumber]=C;
		V=new(CVARIANT);
		V->avtoSet("int");
		V->DATA.intVal=C->accessNumber;
		return;
		}
	if(name=="copy"){
		Function*F=thisVal->DATA.functionVal->F;
		if(!F)return;
		V=new(CVARIANT);
		V->avtoSet("function");
		V->DATA.functionVal->F=F->copy();
		return;
		}
	if(name=="run" || name=="run1"){
		Function*f=thisVal->DATA.functionVal->F;
		if(!f)return;
		V_CVARIANT*Args=&VCV;
		V_pCVARIANT*PArgs=NULL;
		bool isRun1 = 0;
		if(name=="run1")if(VCV.size()>0)isRun1 = VCV[0].isType("vector");
		if(isRun1)PArgs=VCV[0].DATA.vectorVal;
		CVARIANT NF,*VO;
		NF.avtoSet("string");
		Laver*L=M->tableLavers[Pset->Laver];
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
		*NF.DATA.ps="f:"+f->getPsetWay()+f->name;
		(*CEL->DATA.mapVal)["this-f"]=NF;
		int i,size;
		size = (isRun1?PArgs->size():Args->size());
		for(i=0;i<f->names.size() && i<size;++i){
			CVARIANT *A = (isRun1?(*PArgs)[i]:&(*Args)[i]);
			(*CEL->DATA.mapVal)[f->names[i]]=*A;
			}
		if(f->argumentsON){
			CVARIANT ARGS;
			ARGS.avtoSet("vector");
			for(;i<size;++i){
				CVARIANT *A = (isRun1?(*PArgs)[i]:&(*Args)[i]);
				A = A->copy();
				ARGS.DATA.vectorVal->push_back(A);
				}
			(*CEL->DATA.mapVal)["arguments"]=ARGS;
			}
		Pset->Fundament->Memorys.push_back(CEL);
		return;
		}
	if(name=="getIDs"){
		Function*f=thisVal->DATA.functionVal->F;
		V=new(CVARIANT);
		V->avtoSet("vector");
		f->controlSxema();
		f->sxema->controlTable();
		M_IB::iterator it=f->sxema->table.begin();
		for(;it!=f->sxema->table.end();++it)if(it->first!=f->sxema->HeadEnd->accessNumber){
			CVARIANT *Q = new CVARIANT();
			Q->avtoSet("int");
			Q->DATA.intVal=it->first;
			V->DATA.vectorVal->push_back(Q);
			}
		return;
		}
	if(name=="import")if(VCV.size()>0){
		if(!VCV[0].isType("program"))return;
		Algorithm*X=VCV[0].DATA.programVal;
		Function*f=thisVal->DATA.functionVal->F;
		if(f->Body)delete f->Body;
		if(f->sxema){
			f->sxema=NULL;
			delete f->sxema;
			}
		f->Body=X->copy();
		f->Body->initUP(NULL);
		f->controlSxema();
		if(f->file)f->file->NeedSave=1;
		return;
		}
	if(name=="isBodyIF")if(VCV.size()>0){
		VCV[0].TransformType("int");
		int pos=VCV[0].DATA.intVal;
		Function*F=thisVal->DATA.functionVal->F;
		F->controlSxema();
		F->sxema->controlTable();
		Bloc*X=F->sxema->table[pos];
		if(!X)return;
		bool b=(X->A!=NULL);
		if(b)b=X->A->isIF();
		if(b)b=(X->down.size()==2);
		if(b){
			F->sxema->controlRang();
			V_B S1,S2,S3;
			F->sxema->findEndIF(X,&S1,&S2,&S3);
			b=(S3.size()==1);
			}
		V=new(CVARIANT);
		V->avtoSet("bool");
		V->DATA.boolVal=b;
		return;
		}
	if(name=="terminateIF")if(VCV.size()>0){
		VCV[0].TransformType("int");
		int vetka=2;
		if(VCV.size()>1){
			if(VCV[1].isType("bool"))vetka=VCV[1].DATA.boolVal?0:1;
			if(VCV[1].isType("int"))vetka=VCV[1].DATA.intVal;
			if(vetka>2 || vetka<0)vetka=2;
			}
		int pos=VCV[0].DATA.intVal;
		Function*F=thisVal->DATA.functionVal->F;
		F->controlSxema();
		F->sxema->controlTable();
		Bloc*X=F->sxema->table[pos],*Y;
		if(!X)return;
		bool b=(X->A!=NULL);
		if(b)b=X->A->isIF();
		if(b)b=(X->down.size()==2);
		if(b){
			F->sxema->controlRang();
			V_B S1,S2,S3;
			F->sxema->findEndIF(X,&S1,&S2,&S3);
			b=(S3.size()==1);
			if(b){
				delete X->A;
				X->A=NULL;
				if(vetka==1 || vetka==2){
					Y=X->down[1];
					X->down.pop_back();
					Y->up.erase(find(Y->up.begin(),Y->up.end(),X));
					Y->terminateValey(S3[0]);
					}
				if(vetka==0 || vetka==2){
					Y=X->down[0];
					X->down.erase(X->down.begin());
					Y->up.erase(find(Y->up.begin(),Y->up.end(),X));
					Y->terminateValey(S3[0]);
					if(vetka==2){
						Y=S3[0];
						X->down.push_back(Y);
						Y->up.push_back(X);
						}
					}
				F->sxema->table.clear();
				F->sxema->rangOK=0;
				if(F->file)F->file->NeedSave=1;
				if(F->Body){delete F->Body;F->Body=NULL;}
				}
			}
		return;
		}
	if(name=="getLabel")if(VCV.size()>0){
		VCV[0].TransformType("int");
		int id=VCV[0].DATA.intVal,pos=0;
		Function*f=thisVal->DATA.functionVal->F;
		if(!f)return;
		f->controlSxema();
		f->sxema->controlTable();
		M_IB::iterator it=f->sxema->table.begin();
		for(;it!=f->sxema->table.end();++it){
			Algorithm*A=it->second->A;
			Label*LX=dynamic_cast<Label*>(A);
			if(LX)if(LX->id==id){
				pos=it->first;
				break;
				}
			}
		V=new(CVARIANT);
		V->avtoSet("int");
		V->DATA.intVal=pos;
		return;
		}
	if(name=="setNamesStart")if(VCV.size()>0){
		Function*f=thisVal->DATA.functionVal->F;
		if(!f)return;
		f->names.clear();
		f->tips.clear();
		VCV[0].TransformType("vector");
		V_pCVARIANT::iterator it=VCV[0].DATA.vectorVal->begin();
		int i;
		for(i=0;it!=VCV[0].DATA.vectorVal->end();++it,++i){
			(*it)->TransformType("string");
			f->names.push_back(*(*it)->DATA.ps);
			f->tips.push_back(Type());
			}
		if(f->file)f->file->NeedSave=1;
		return;
		}
	if(name=="setReturnType")if(VCV.size()>0){
		Function*f=thisVal->DATA.functionVal->F;
		if(!f)return;
		VCV[0].TransformType("string");
		CVARIANT X;
		X.avtoSet(*VCV[0].DATA.ps);
		string namet=CVARIANT::name[X.Ntype];
		if(f->ret.name==namet)return;
		f->ret.name=namet;
		if(f->file)f->file->NeedSave=1;
		return;
		}
	if(name=="getReturnType"){
		Function*f=thisVal->DATA.functionVal->F;
		if(!f)return;
		V=new(CVARIANT);
		V->avtoSet("string");
		*V->DATA.ps=f->ret.name;
		return;
		}
	if(name=="getGraph" || name=="toGraph"){
		Function*f=thisVal->DATA.functionVal->F;
		if(!f)return;
		f->controlSxema();
		UGraf*UG = new UGraf();
		f->sxema->getGraph(UG);
		V=new(CVARIANT);
		V->avtoSet("graph");
		if(V->DATA.grafVal)delete V->DATA.grafVal;
		V->DATA.grafVal = UG;
		return;
		}
	if(name=="isAccesibleSub" || name=="getSubF" || name=="deleteSubTree")if(VCV.size()>=2){
		Function*f=thisVal->DATA.functionVal->F;
		if(!f)return;
		VCV[0].TransformType("int");
		VCV[1].TransformType("int");
		int pos1,pos2;
		pos1 = VCV[0].DATA.intVal;
		pos2 = VCV[1].DATA.intVal;
		S_I SI;
		bool ok = f->isAccesibleSub(pos1,pos2,SI);
		if(name=="isAccesibleSub"){
			V=new(CVARIANT);
			V->avtoSet("bool");
			V->DATA.boolVal = ok;
			return;
			}
		if(name=="getSubF"){
			if(!ok)return;
			V=new(CVARIANT);
			V->avtoSet("function");
			if(V->DATA.functionVal->F)delete V->DATA.functionVal->F;
			Function*f2=new Function();
			V->DATA.functionVal->F=f2;
			f2->name = "subF";
			f2->sxema = f->getSubSxema(SI);
			f2->sxema->f = f2;
			return;
			}
		if(ok)f->deleteSubTree(pos1,pos2,SI);
		V=new(CVARIANT);
		V->avtoSet("bool");
		V->DATA.boolVal = ok;
		return;
		}
	if(name=="insertInUnit")if(VCV.size()>=2){
		int pos=2;
		if(VCV[0].isType("function"))pos=0;
		if(VCV[1].isType("function"))pos=1;
		if(pos==2)return;
		Function*F=thisVal->DATA.functionVal->F;
		Function*FIZ=VCV[pos].DATA.functionVal->F;
		int i=1-pos;
		VCV[i].TransformType("int");
		pos=VCV[i].DATA.intVal;
		F->insertInUnit(pos,FIZ);
		return;
		}
	bool isGet=0;
	if(name=="setDataTable" || (isGet=(name=="getDataTable"))){
		Function*F=thisVal->DATA.functionVal->F;
		if(isGet){
			if(F->DataTable){
				V=F->DataTable->copy();
				return;
				}
			V=new CVARIANT;
			return;
			}
		if(VCV.size()>=1){
			VCV[0].TransformType("map");
			if(F->DataTable)delete F->DataTable;
			F->DataTable=VCV[0].copy();
			}
		return;
		}
	if(name=="toTerm"){
		Function*F=thisVal->DATA.functionVal->F;
		Algorithm*X=NULL;
		if(!F->Body){
			if(F->sxema){
				Sxema*Sxem=F->sxema->copy();
				X=Sxem->Coder();
				delete Sxem;
				}
			}else X=F->Body->copy();
		if(!X)return;
		BaseFunction*BF=new BaseFunction;
		X->initUP(NULL);
		BF->Body=X;
		BF->argumentsON=F->argumentsON;
		BF->argumentsNames=F->names;
		V=new CVARIANT;
		V->avtoSet("program");
		if(V->DATA.programVal)delete V->DATA.programVal;
		V->DATA.programVal=BF;
		return;
		}
}




void CallFunc::forprogram(CVARIANT*thisVal,string&name,CVARIANT*&V,V_CVARIANT&VCV){
	if(name=="getRowSize"){
		V_I maps;
		if(VCV.size()>0){
			if(!VCV[0].isType("vector"))return;
			V_pCVARIANT*VC=VCV[0].DATA.vectorVal;
			int i;
			for(i=0;i<VC->size();++i){
				(*VC)[i]->TransformType("int");
				maps.push_back((*VC)[i]->DATA.intVal);
				}
			}
		Algorithm*A=thisVal->DATA.programVal;
		Algorithm**X;
		int n=A->getSub(&maps,0,X);
		V=new(CVARIANT);
		V->avtoSet("int");
		V->DATA.intVal=n;
		return;
		}
	if(name=="getSub"){
		V_I maps;
		if(VCV.size()>0){
			if(!VCV[0].isType("vector"))return;
			V_pCVARIANT*VC=VCV[0].DATA.vectorVal;
			int i;
			for(i=0;i<VC->size();++i){
				(*VC)[i]->TransformType("int");
				maps.push_back((*VC)[i]->DATA.intVal);
				}
			}
		Algorithm*A=thisVal->DATA.programVal;
		Algorithm**X=NULL;
		A->getSub(&maps,0,X);
		if(!X)X=&A;
		V=new(CVARIANT);
		if(!*X){
			V->avtoSet("void");
			return;
			}
		Algorithm*XX=(*X)->copy();
		XX->initUP(NULL);
		V=new(CVARIANT);
		V->avtoSet("program");
		if(V->DATA.programVal)delete V->DATA.programVal;
		V->DATA.programVal=XX;
		return;
		}
	if(name=="setSub")if(VCV.size()>1){
		if(!VCV[0].isType("vector"))return;
		if(!VCV[1].isType("program"))return;
		V_I maps;
		V_pCVARIANT*VC=VCV[0].DATA.vectorVal;
		int i;
		for(i=0;i<VC->size();++i){
			(*VC)[i]->TransformType("int");
			maps.push_back((*VC)[i]->DATA.intVal);
			}
		Algorithm**A=&thisVal->DATA.programVal;
		Algorithm**X=NULL;
		(*A)->getSub(&maps,0,X);
		if(!X)X=A;
		if(!*X)return;
		Algorithm*P=(*X)->up;
		delete *X;
		*X=VCV[1].DATA.programVal->copy();
		(*X)->initUP(P);
		return;
		}
	if(name=="typeof"){
		int regim=0;
		V_I maps;
		if(VCV.size()>0){
			int na=-1,nb=-1;
			if(VCV[0].isType("vector"))na=0;else{na=1;nb=0;}
			if(VCV.size()>1){
				if(!VCV[1].isType("vector")){na=0;nb=1;}
				}else if(na==1)na=-1;
			if(nb>=0){
				VCV[nb].TransformType("string");
				if(*VCV[nb].DATA.ps=="type")regim=1;
				if(*VCV[nb].DATA.ps=="name")regim=2;
				if(*VCV[nb].DATA.ps=="value")regim=3;
				}
			if(na>=0){
				V_pCVARIANT*VC=VCV[na].DATA.vectorVal;
				int i;
				for(i=0;i<VC->size();++i){
					(*VC)[i]->TransformType("int");
					maps.push_back((*VC)[i]->DATA.intVal);
					}
				}
			}
		Algorithm*A=thisVal->DATA.programVal;
		Algorithm**X=NULL;
		A->getSub(&maps,0,X);
		if(!X)X=&A;
		if(!*X){
			V=new(CVARIANT);
			V->avtoSet("string");
			return;
			}
		string s;
		if(VCV.size()>2){
			VCV[2].TransformType("string");
			s=*VCV[2].DATA.ps;
			CreateVar*Q13=dynamic_cast<CreateVar*>(*X);
			if(Q13){if(regim==2)Q13->name=s;}
			return;
			}
		Base*Q1=dynamic_cast<Base*>(*X);
		if(Q1){
			const char*m[]={"var","string","char","digit","#"};
			s=m[Q1->type];
			if(regim==1)s="Base";
			if(regim==3)s=Q1->text;
			}
		Base2*Q2=dynamic_cast<Base2*>(*X);
		if(Q2){
			s=Base2::m[Q2->n];
			if(regim==1)s="Base2";
			}
		Cast*Q7=dynamic_cast<Cast*>(*X);
		if(Q7){
			s="Cast";
			if(regim==1)s=Q7->cast.toString();
			}else{
			Prefix*Q3=dynamic_cast<Prefix*>(*X);
			if(Q3){
				s=(string)Prefix::m[Q3->n]+"#";
				if(regim==1)s="Prefix";
				}
			}
		CallFunc*Q5=dynamic_cast<CallFunc*>(*X);
		if(Q5)s="function";else{
			AccesMasiv*Q6=dynamic_cast<AccesMasiv*>(*X);
			if(Q6)s="AccessArray";else{
				Sufix*Q4=dynamic_cast<Sufix*>(*X);
				if(Q4){
					s=string("#")+Sufix::m[Q4->n];
					if(regim==1)s="Sufix";
					}
				}
			}
		Interval*Q8=dynamic_cast<Interval*>(*X);
		if(Q8)s="Interval";
		AlgoSet*Q9=dynamic_cast<AlgoSet*>(*X);
		if(Q9)s="AlgoSet";
		AComposition*Q10=dynamic_cast<AComposition*>(*X);
		if(Q10){
			s="Composition";
			if(regim){
				V=new(CVARIANT);
				V->avtoSet("vector");
				M_SA::iterator it = Q10->table.begin();
				for(;it!=Q10->table.end();++it){
					CVARIANT *X = new CVARIANT();
					X->avtoSet("string");
					*X->DATA.ps=it->first;
					V->DATA.vectorVal->push_back(X);
					}
				return;
				}
			}
		Label*Q11=dynamic_cast<Label*>(*X);
		if(Q11){
			s="Label";
			if(regim>1){
				V=new(CVARIANT);
				V->avtoSet("int");
				V->DATA.intVal=Q11->id;
				return;
				}
			}
		Sequence*Q12=dynamic_cast<Sequence*>(*X);
		if(Q12)s="Sequence";
		CreateVar*Q13=dynamic_cast<CreateVar*>(*X);
		if(Q13){
			s="CreateVar";
			if(regim==1)s=Q13->tip.toString();
			if(regim==2)s=Q13->name;
			}
		SpecSumbol*Q14=dynamic_cast<SpecSumbol*>(*X);
		if(Q14)s=SpecSumbol::m[Q14->n];
		IF*Q15=dynamic_cast<IF*>(*X);
		if(Q15)s="if";
		WHILE*Q16=dynamic_cast<WHILE*>(*X);
		if(Q16)s=(Q16->n==1?"while":"do");
		FOR*Q17=dynamic_cast<FOR*>(*X);
		if(Q17)s="for";
		V=new(CVARIANT);
		V->avtoSet("string");
		*V->DATA.ps=s;
		return;
		}
	if(name=="getPower"){
		int n=0;
		if(VCV.size()>0){
			VCV[0].TransformType("int");
			n=VCV[0].DATA.intVal;
			}
		Algorithm*A=thisVal->DATA.programVal;
		V=new(CVARIANT);
		V->avtoSet("int");
		V->DATA.intVal = A->Power(n);
		return;
		}
	if(name=="isProsto"){
		Algorithm*A=thisVal->DATA.programVal;
		V=new(CVARIANT);
		V->avtoSet("bool");
		V->DATA.boolVal=A->isProsto();
		return;
		}
	if(name=="isIF"){
		Algorithm*A=thisVal->DATA.programVal;
		V=new(CVARIANT);
		V->avtoSet("bool");
		V->DATA.boolVal=A->isIF();
		return;
		}
	if(name=="replace")if(VCV.size()>=2){
		VCV[0].TransformType("string");
		VCV[1].TransformType("string");
		Algorithm*A=thisVal->DATA.programVal;
		int n=A->ReplaceName(VCV[0].DATA.ps,VCV[1].DATA.ps);
		V=new(CVARIANT);
		V->avtoSet("int");
		V->DATA.intVal=n;
		return;
		}
	if(name=="toFunction"){
		Algorithm*A=thisVal->DATA.programVal;
		Function*F=new(Function);
		F->Body=A->copy();
		F->Body->initUP(NULL);
		F->controlSxema();
		F->name="noname";
		F->ret.name="var";
		V=new(CVARIANT);
		V->avtoSet("function");
		if(V->DATA.functionVal->F)delete V->DATA.functionVal->F;
		V->DATA.functionVal->F=F;
		return;
		}
	if(name=="getNames"){
		Algorithm*A=thisVal->DATA.programVal;
		int mask=8|1;
		if(VCV.size()){
			if(VCV[0].isType("int")){
				int t=VCV[0].DATA.intVal;
				mask=0;
				if(t&4)mask|=8;
				if(t&1 && t&2)mask|=1;
				if(t&2)mask|=4;
				}
			if(VCV[0].isType("string")){
				string t=*VCV[0].DATA.ps;
				if(t=="var" || t=="vars")mask=0;
				if(t=="function" || t=="functions")mask=4;
				if(t=="key" || t=="keys")mask=8;
				}
			}
		V_S S;
		A->getNames(&S,mask);
		V=new(CVARIANT);
		V->avtoSet("vector");
		int i;
		for(i=0;i<S.size();++i){
			CVARIANT *X = new CVARIANT();
			X->avtoSet("string");
			*X->DATA.ps=S[i];
			V->DATA.vectorVal->push_back(X);
			}
		return;
		}
}




//--------------------------------------------------------------------------------------------------
void CallFunc::forgraf(CVARIANT*thisVal,string&name,CVARIANT*&V,V_CVARIANT&VCV,MAIN*M){
	if(name=="export" || name=="toString"){
		UGraf*UG=thisVal->DATA.grafVal;
		V=new(CVARIANT);
		V->avtoSet("string");
		if(UG){
			if(VCV.size()>0){
				VCV[0].TransformType("string");
				string s=*VCV[0].DATA.ps;
				*V->DATA.ps=UG->toString(s);
				return;
				}
			*V->DATA.ps=UG->toString();
			}
		return;
		}
	if(name=="NET" || name=="setNet")if(VCV.size()>=5){
		int a,b,va,vb;
		string str;
		VCV[0].TransformType("int");
		VCV[1].TransformType("int");
		VCV[2].TransformType("string");
		VCV[3].TransformType("int");
		VCV[4].TransformType("int");
		a=VCV[0].DATA.intVal;
		va=VCV[1].DATA.intVal;
		str=*VCV[2].DATA.ps;
		vb=VCV[3].DATA.intVal;
		b=VCV[4].DATA.intVal;
		NetLine*X=new NetLine(a,va,str.c_str(),vb,b);
		UGraf*UG=thisVal->DATA.grafVal;
		UG->add(X);
		return;
		}
	if(name=="MARKER" || name=="setMarker")if(VCV.size()>=5){
		int a,vb,va;
		string str,m;
		VCV[0].TransformType("int");
		VCV[1].TransformType("int");
		VCV[2].TransformType("string");
		VCV[3].TransformType("int");
		VCV[4].TransformType("string");
		a=VCV[0].DATA.intVal;
		va=VCV[1].DATA.intVal;
		str=*VCV[2].DATA.ps;
		vb=VCV[3].DATA.intVal;
		m=*VCV[4].DATA.ps;
		MarkerLine*X=new MarkerLine(a,va,str.c_str(),vb,m.c_str());
		UGraf*UG=thisVal->DATA.grafVal;
		UG->add(X);
		return;
		}
	if(name=="FREE" || name=="getFree"){
		UGraf*UG=thisVal->DATA.grafVal;
		V=new(CVARIANT);
		V->avtoSet("int");
		V->DATA.intVal=UG->getnew();
		return;
		}
	if(name=="getNET"){
		V_CVARIANT D=VCV;
		CVARIANT XX,*Y;
		XX.avtoSet("void");
		while(D.size()<5)D.push_back(XX);
		int a,b,va,vb;
		string*str=NULL;
		a=b=-1;
		va=vb=2;
		if(!D[0].isType("void")){D[0].TransformType("int");a=D[0].DATA.intVal;}
		if(!D[1].isType("void")){D[1].TransformType("int");va=D[1].DATA.intVal;}
		if(!D[2].isType("void")){D[2].TransformType("string");str=D[2].DATA.ps;}
		if(!D[3].isType("void")){D[3].TransformType("int");vb=D[3].DATA.intVal;}
		if(!D[4].isType("void")){D[4].TransformType("int");b=D[4].DATA.intVal;}
		if(va<0)va=2;
		if(vb<0)vb=2;
		UGraf*UG=thisVal->DATA.grafVal;
		V_BL vbl;
		UG->findNet(vbl,a,va,str,vb,b);
		V=new(CVARIANT);
		V->avtoSet("vector");
		int i;
		for(i=0;i<vbl.size();++i){
			NetLine*NL=dynamic_cast<NetLine*>(vbl[i]);
			bool revers=0;
			if(a>=0 && b>=0)revers=(NL->a!=a); else{
				if(a>=0)revers=(NL->a!=a);
				if(b>=0)revers=(NL->b!=b);
				}
			CVARIANT *X = new CVARIANT();
			X->avtoSet("vector");
			char xx,yy;
			xx=NL->c&1;
			yy=(NL->c>>1)&1;
			if(revers){
				if(a<0){
					Y = new CVARIANT();
					Y->avtoSet("int");
					Y->DATA.intVal=NL->b;
					X->DATA.vectorVal->push_back(Y);
					}
				if(va>=2){
					Y = new CVARIANT();
					Y->avtoSet("int");
					Y->DATA.intVal=yy;
					X->DATA.vectorVal->push_back(Y);
					}
				if(!str){
					Y = new CVARIANT();
					Y->avtoSet("string");
					*Y->DATA.ps=NL->name;
					X->DATA.vectorVal->push_back(Y);
					}
				if(vb>=2){
					Y = new CVARIANT();
					Y->avtoSet("int");
					Y->DATA.intVal=xx;
					X->DATA.vectorVal->push_back(Y);
					}
				if(b<0){
					Y = new CVARIANT();
					Y->avtoSet("int");
					Y->DATA.intVal=NL->a;
					X->DATA.vectorVal->push_back(Y);
					}
				}else{
				if(a<0){
					Y = new CVARIANT();
					Y->avtoSet("int");
					Y->DATA.intVal=NL->a;
					X->DATA.vectorVal->push_back(Y);
					}
				if(va>=2){
					Y = new CVARIANT();
					Y->avtoSet("int");
					Y->DATA.intVal=xx;
					X->DATA.vectorVal->push_back(Y);
					}
				if(!str){
					Y = new CVARIANT();
					Y->avtoSet("string");
					*Y->DATA.ps=NL->name;
					X->DATA.vectorVal->push_back(Y);
					}
				if(vb>=2){
					Y = new CVARIANT();
					Y->avtoSet("int");
					Y->DATA.intVal=yy;
					X->DATA.vectorVal->push_back(Y);
					}
				if(b<0){
					Y = new CVARIANT();
					Y->avtoSet("int");
					Y->DATA.intVal=NL->b;
					X->DATA.vectorVal->push_back(Y);
					}
				}
			V->DATA.vectorVal->push_back(X);
			}
		return;
		}
	if(name=="getMARKER"){
		V_CVARIANT D=VCV;
		CVARIANT XX,*Y;
		XX.avtoSet("void");
		while(D.size()<5)D.push_back(XX);
		int a,va,vb;
		string*str=NULL,*m=NULL;
		a=-1;
		va=vb=2;
		if(!D[0].isType("void")){D[0].TransformType("int");a=D[0].DATA.intVal;}
		if(!D[1].isType("void")){D[1].TransformType("int");va=D[1].DATA.intVal;}
		if(!D[2].isType("void")){D[2].TransformType("string");str=D[2].DATA.ps;}
		if(!D[3].isType("void")){D[3].TransformType("int");vb=D[3].DATA.intVal;}
		if(!D[4].isType("void")){D[4].TransformType("string");m=D[4].DATA.ps;}
		if(va<0)va=2;
		if(vb<0)vb=2;
		UGraf*UG=thisVal->DATA.grafVal;
		V_BL vbl;
		UG->findMarker(vbl,a,va,str,vb,m);
		V=new(CVARIANT);
		V->avtoSet("vector");
		int i;
		for(i=0;i<vbl.size();++i){
			MarkerLine*ML=dynamic_cast<MarkerLine*>(vbl[i]);
			CVARIANT *X = new CVARIANT();
			X->avtoSet("vector");
			char xx,yy;
			xx=ML->c&1;
			yy=(ML->c>>1)&1;
			if(a<0){
				Y = new CVARIANT();
				Y->avtoSet("int");
				Y->DATA.intVal=ML->a;
				X->DATA.vectorVal->push_back(Y);
				}
			if(va>=2){
				Y = new CVARIANT();
				Y->avtoSet("int");
				Y->DATA.intVal=xx;
				X->DATA.vectorVal->push_back(Y);
				}
			if(!str){
				Y = new CVARIANT();
				Y->avtoSet("string");
				*Y->DATA.ps=ML->name;
				X->DATA.vectorVal->push_back(Y);
				}
			if(vb>=2){
				Y = new CVARIANT();
				Y->avtoSet("int");
				Y->DATA.intVal=yy;
				X->DATA.vectorVal->push_back(Y);
				}
			if(!m){
				Y = new CVARIANT();
				Y->avtoSet("string");
				*Y->DATA.ps=ML->marker;
				X->DATA.vectorVal->push_back(Y);
				}
			V->DATA.vectorVal->push_back(X);
			}
		return;
		}
	if(name=="replaceNET")if(VCV.size()>=2){
		VCV[0].TransformType("vector");
		VCV[1].TransformType("vector");
		V_pCVARIANT&D=*VCV[0].DATA.vectorVal;
		while(D.size()<5){
			CVARIANT *X = new CVARIANT();
			X->avtoSet("void");
			D.push_back(X);
			}
		int a,b,va,vb;
		string*str=NULL;
		a=b=-1;
		va=vb=2;
		if(!D[0]->isType("void")){D[0]->TransformType("int");a=D[0]->DATA.intVal;}
		if(!D[1]->isType("void")){D[1]->TransformType("int");va=D[1]->DATA.intVal;}
		if(!D[2]->isType("void")){D[2]->TransformType("string");str=D[2]->DATA.ps;}
		if(!D[3]->isType("void")){D[3]->TransformType("int");vb=D[3]->DATA.intVal;}
		if(!D[4]->isType("void")){D[4]->TransformType("int");b=D[4]->DATA.intVal;}
		if(va<0)va=2;
		if(vb<0)vb=2;
		V_pCVARIANT&D2=*VCV[1].DATA.vectorVal;
		while(D2.size()<5){
			CVARIANT *X = new CVARIANT();
			X->avtoSet("void");
			D2.push_back(X);
			}
		int a2,b2,va2,vb2;
		string*str2=NULL;
		a2=b2=-1;
		va2=vb2=2;
		if(!D2[0]->isType("void")){D2[0]->TransformType("int");a2=D2[0]->DATA.intVal;}
		if(!D2[1]->isType("void")){D2[1]->TransformType("int");va2=D2[1]->DATA.intVal;}
		if(!D2[2]->isType("void")){D2[2]->TransformType("string");str2=D2[2]->DATA.ps;}
		if(!D2[3]->isType("void")){D2[3]->TransformType("int");vb2=D2[3]->DATA.intVal;}
		if(!D2[4]->isType("void")){D2[4]->TransformType("int");b2=D2[4]->DATA.intVal;}
		if(va2<0)va2=2;
		if(vb2<0)vb2=2;
		UGraf*UG=thisVal->DATA.grafVal;
		V_BL vbl;
		UG->findNet(vbl,a,va,str,vb,b);
		int i;
		for(i=0;i<vbl.size();++i){
			NetLine*NL=dynamic_cast<NetLine*>(vbl[i]);
			bool revers=0;
			if(a>=0 && b>=0)revers=(NL->a!=a); else{
				if(a>=0)revers=(NL->a!=a);
				if(b>=0)revers=(NL->b!=b);
				}
			char xx,yy;
			xx=NL->c&1;
			yy=(NL->c>>1)&1;
			if(revers){
				if(a2>=0)NL->b=a2;
				if(va2<2)yy=va2;
				if(str2)NL->name=*str2;
				if(vb2<2)xx=vb2;
				if(b2>=0)NL->a=b2;
				}else{
				if(a2>=0)NL->a=a2;
				if(va2<2)xx=va2;
				if(str2)NL->name=*str2;
				if(vb2<2)yy=vb2;
				if(b2>=0)NL->b=b2;
				}
			NL->c=xx|(yy<<1);
			}
		return;
		}
	if(name=="replaceMARKER")if(VCV.size()>=2){
		VCV[0].TransformType("vector");
		VCV[1].TransformType("vector");
		V_pCVARIANT&D=*VCV[0].DATA.vectorVal;
		while(D.size()<5){
			CVARIANT *X = new CVARIANT();
			X->avtoSet("void");
			D.push_back(X);
			}
		int a,va,vb;
		string*str=NULL,*m=NULL;
		a=-1;
		va=vb=2;
		if(!D[0]->isType("void")){D[0]->TransformType("int");a=D[0]->DATA.intVal;}
		if(!D[1]->isType("void")){D[1]->TransformType("int");va=D[1]->DATA.intVal;}
		if(!D[2]->isType("void")){D[2]->TransformType("string");str=D[2]->DATA.ps;}
		if(!D[3]->isType("void")){D[3]->TransformType("int");vb=D[3]->DATA.intVal;}
		if(!D[4]->isType("void")){D[4]->TransformType("string");m=D[4]->DATA.ps;}
		if(va<0)va=2;
		if(vb<0)vb=2;
		V_pCVARIANT&D2=*VCV[1].DATA.vectorVal;
		while(D2.size()<5){
			CVARIANT *X = new CVARIANT();
			X->avtoSet("void");
			D2.push_back(X);
			}
		int a2,va2,vb2;
		string*str2=NULL,*m2=NULL;
		a2=-1;
		va2=vb2=2;
		if(!D2[0]->isType("void")){D2[0]->TransformType("int");a2=D2[0]->DATA.intVal;}
		if(!D2[1]->isType("void")){D2[1]->TransformType("int");va2=D2[1]->DATA.intVal;}
		if(!D2[2]->isType("void")){D2[2]->TransformType("string");str2=D2[2]->DATA.ps;}
		if(!D2[3]->isType("void")){D2[3]->TransformType("int");vb2=D2[3]->DATA.intVal;}
		if(!D2[4]->isType("void")){D2[4]->TransformType("string");m2=D2[4]->DATA.ps;}
		if(va2<0)va2=2;
		if(vb2<0)vb2=2;
		UGraf*UG=thisVal->DATA.grafVal;
		V_BL vbl;
		UG->findMarker(vbl,a,va,str,vb,m);
		int i;
		for(i=0;i<vbl.size();++i){
			MarkerLine*ML=dynamic_cast<MarkerLine*>(vbl[i]);
			char xx,yy;
			xx=ML->c&1;
			yy=(ML->c>>1)&1;
			if(a2>=0)ML->a=a2;
			if(va2<2)xx=va2;
			if(str2)ML->name=*str2;
			if(vb2<2)yy=vb2;
			if(m2)ML->marker=*m2;
			ML->c=xx|(yy<<1);
			}
		return;
		}
	if(name=="deleteNET"){
		V_CVARIANT D=VCV;
		CVARIANT X;
		X.avtoSet("void");
		while(D.size()<5)D.push_back(X);
		int a,b,va,vb;
		string*str=NULL;
		a=b=-1;
		va=vb=2;
		if(!D[0].isType("void")){D[0].TransformType("int");a=D[0].DATA.intVal;}
		if(!D[1].isType("void")){D[1].TransformType("int");va=D[1].DATA.intVal;}
		if(!D[2].isType("void")){D[2].TransformType("string");str=D[2].DATA.ps;}
		if(!D[3].isType("void")){D[3].TransformType("int");vb=D[3].DATA.intVal;}
		if(!D[4].isType("void")){D[4].TransformType("int");b=D[4].DATA.intVal;}
		if(va<0)va=2;
		if(vb<0)vb=2;
		UGraf*UG=thisVal->DATA.grafVal;
		UG->deleteNet(a,va,str,vb,b);
		return;
		}
	if(name=="deleteMARKER"){
		V_CVARIANT D=VCV;
		CVARIANT X,Y;
		X.avtoSet("void");
		while(D.size()<5)D.push_back(X);
		int a,va,vb;
		string*str=NULL,*m=NULL;
		a=-1;
		va=vb=2;
		if(!D[0].isType("void")){D[0].TransformType("int");a=D[0].DATA.intVal;}
		if(!D[1].isType("void")){D[1].TransformType("int");va=D[1].DATA.intVal;}
		if(!D[2].isType("void")){D[2].TransformType("string");str=D[2].DATA.ps;}
		if(!D[3].isType("void")){D[3].TransformType("int");vb=D[3].DATA.intVal;}
		if(!D[4].isType("void")){D[4].TransformType("string");m=D[4].DATA.ps;}
		if(va<0)va=2;
		if(vb<0)vb=2;
		UGraf*UG=thisVal->DATA.grafVal;
		UG->deleteMarker(a,va,str,vb,m);
		return;
		}
	if(name=="import")if(VCV.size()>=1){
		VCV[0].TransformType("string");
		UGraf*UG=thisVal->DATA.grafVal;
		UG->import(VCV[0].DATA.ps->c_str());
		return;
		}
	if(name=="getUnits"){
		UGraf*UG=thisVal->DATA.grafVal;
		S_I SI;
		UG->getUnits(SI);
		V=new(CVARIANT);
		V->avtoSet("vector");
		S_I::iterator it=SI.begin();
		for(;it!=SI.end();++it){
			CVARIANT *I = new CVARIANT();
			I->avtoSet("int");
			I->DATA.intVal=*it;
			V->DATA.vectorVal->push_back(I);
			}
		return;
		}
	if(name=="getFunction" || name=="toFunction"){
		Function*f = new Function();
		f->name = "noname";
		bool ok = f->BuildMe(thisVal->DATA.grafVal,M);
		V = new CVARIANT();
		if(ok){
			V->avtoSet("function");
			V->DATA.functionVal->F = f;
			}else delete f;
		return;
		}
	if(name=="addGraph")if(VCV.size()>=3){
		if(!VCV[0].isType("graph"))return;
		VCV[1].TransformType("vector");
		VCV[2].TransformType("vector");
		UGraf*UG=thisVal->DATA.grafVal;
		const UGraf*UGX=VCV[0].DATA.grafVal;
		int size = VCV[1].DATA.vectorVal->size();
		int n = VCV[2].DATA.vectorVal->size(),i;
		if(n<size)size=n;
		V_I A,B,C;
		CVARIANT*Q;
		for(n=0;n<size;++n){
			Q=(*VCV[1].DATA.vectorVal)[n];
			Q->TransformType("int");
			A.push_back(Q->DATA.intVal);
			Q=(*VCV[2].DATA.vectorVal)[n];
			Q->TransformType("int");
			B.push_back(Q->DATA.intVal);
			}
		S_I S;
		UGX->getUnits(S);
		S_I::iterator it = S.begin();
		for(;it!=S.end();++it){
			int n2=*it,nme;
			V_I::iterator t = find(B.begin(),B.end(),n2);
			if(t==B.end())nme=UG->getnew(); else nme=A[t-B.begin()];
			C.push_back(nme);
			}
		size=UGX->lincs.size();
		for(n=0;n<size;++n){
			NetLine*NL=UGX->lincs[n];
			NL = new NetLine(*NL);
			it = S.begin();
			for(i=0;it!=S.end();++it,++i)if(*it==NL->a)break;
			NL->a = C[i];
			it = S.begin();
			for(i=0;it!=S.end();++it,++i)if(*it==NL->b)break;
			NL->b = C[i];
			UG->add(NL);
			}
		size=UGX->markers.size();
		for(n=0;n<size;++n){
			MarkerLine*ML=UGX->markers[n];
			ML = new MarkerLine(*ML);
			it = S.begin();
			for(i=0;it!=S.end();++it,++i)if(*it==ML->a)break;
			ML->a = C[i];
			UG->add(ML);
			}
		return;
		}
	if(name=="ExportData"){
		UGraf*UG = thisVal->DATA.grafVal;
		V = UG->ExportData();
		return;
		}
	if(name=="ImportData")if(VCV.size()>=1){
		UGraf*UG = thisVal->DATA.grafVal;
		bool isValid = UG->ImportData(&VCV[0]);
		V = new CVARIANT();
		V->avtoSet("bool");
		V->DATA.boolVal = isValid;
		return;
		}
}








void CallFunc::fordigit(CVARIANT*thisVal,string&name,CVARIANT*&V,V_CVARIANT&VCV){
	if(name=="round")if(VCV.size()>=1){
		VCV[0].TransformType("int");
		thisVal->DATA.digitVal->round(VCV[0].DATA.intVal);
		return;
		}
	if(name=="away")if(VCV.size()>=1){
		VCV[0].TransformType("int");
		thisVal->DATA.digitVal->away(VCV[0].DATA.intVal);
		return;
		}
	if(name=="setPI"){
		thisVal->DATA.digitVal->set(Digits::cpi);
		return;
		}
	if(name=="setE"){
		thisVal->DATA.digitVal->set(Digits::ce);
		return;
		}
	if(name=="setAccuracy")if(VCV.size()>=1){
		VCV[0].TransformType("int");
		int i = VCV[0].DATA.intVal;
		if(i<0)i=0;
		thisVal->DATA.digitVal->mlimit=i;
		return;
		}
	if(name=="exponenta"){
		V = new(CVARIANT);
		V->avtoSet("digit");
		V->DATA.digitVal->exponenta = thisVal->DATA.digitVal->exponenta;
		return;
		}
	if(name=="mantisa"){
		V = new(CVARIANT);
		V->avtoSet("digit");
		V->DATA.digitVal->mantisa = thisVal->DATA.digitVal->mantisa;
		return;
		}
	if(name=="equale")if(VCV.size()>=1){
		VCV[0].TransformType("digit");
		V = new(CVARIANT);
		V->avtoSet("char");
		V->DATA.bVal = thisVal->DATA.digitVal->compare(*VCV[0].DATA.digitVal);
		return;
		}
	if(name=="toString"){
		V = new(CVARIANT);
		V->avtoSet("string");
		*V->DATA.ps = thisVal->DATA.digitVal->toString();
		return;
		}
	if(name=="toNumberSystem"){
		int n = 16;
		if(VCV.size()>=1){
			VCV[0].TransformType("int");
			n = VCV[0].DATA.intVal;
			}
		V = new(CVARIANT);
		V->avtoSet("string");
		*V->DATA.ps = thisVal->DATA.digitVal->toNumberSystem(n);
		return;
		}
	if(name=="ImportNumberSystem")if(VCV.size()>=1){
		int n = 16;
		Digits *D = thisVal->DATA.digitVal;
		if(VCV.size()>=2){
			VCV[1].TransformType("int");
			n = VCV[1].DATA.intVal;
			}
		VCV[0].TransformType("string");
		string *s = VCV[0].DATA.ps;
		D->ImportNumberSystem(*s, n);
		return;
		}
	if(name=="isInt"){
		Digits *D = thisVal->DATA.digitVal;
		int n = (int)*D;
		Digits P;
		P.set(n);
		bool isInt = (P==*D);
		V = new(CVARIANT);
		V->avtoSet("bool");
		V->DATA.boolVal = isInt;
		return;
		}
	if(name=="Pow")if(VCV.size()>=1){
		Digits *D = thisVal->DATA.digitVal;
		VCV[0].TransformType("digit");
		D->Pow(*VCV[0].DATA.digitVal);
		V = new(CVARIANT);
		V->avtoSet("void");
		return;
		}
}




void CallFunc::formodule(CVARIANT*thisVal,string&name,CVARIANT*&V,V_CVARIANT&VCV,MAIN*M){
	File*file = NULL;
	if(M->Files.find(thisVal->DATA.moduleVal->id)!=M->Files.end())
		file = M->Files[thisVal->DATA.moduleVal->id];
	if(file){
		if(name=="getName"){
			V = new(CVARIANT);
			V->avtoSet("string");
			*V->DATA.ps = file->name;
			return;
			}
		if(name=="getWay"){
			V = new(CVARIANT);
			V->avtoSet("string");
			*V->DATA.ps = file->way;
			return;
			}
		if(name=="NeedSave"){
			if(VCV.size()>0){
				VCV[0].TransformType("bool");
				file->NeedSave=VCV[0].DATA.boolVal;
				}
			V = new(CVARIANT);
			V->avtoSet("bool");
			V->DATA.boolVal = file->NeedSave;
			return;
			}
		if(name=="controlSave"){
			file->controlSave();
			return;
			}
		if(name=="Save"){
			file->NeedSave = 1;
			file->controlSave();
			return;
			}
		if(name=="FindFunction" || name=="getFunction")if(VCV.size()>0){
			VCV[0].TransformType("string");
			Function*f = file->FindFunction(*VCV[0].DATA.ps);
			V = new(CVARIANT);
			if(!f){
				V->avtoSet("int");
				V->DATA.intVal=0;
				return;
				}
			V->avtoSet("function");
			V->DATA.functionVal->F=f;
			V->DATA.functionVal->isOriginal = 1;
			return;
			}
		if(name=="getAllFunctions"){
			V = new(CVARIANT);
			V->avtoSet("vector");
			V_Func::iterator it=file->functions.begin();
			for(;it!=file->functions.end();++it){
				CVARIANT *X = new CVARIANT();
				X->avtoSet("function");
				X->DATA.functionVal->F=*it;
				X->DATA.functionVal->isOriginal = 1;
				V->DATA.vectorVal->push_back(X);
				}
			return;
			}
		if(name=="getAllFunctionNames"){
			V = new(CVARIANT);
			V->avtoSet("vector");
			V_Func::iterator it=file->functions.begin();
			for(;it!=file->functions.end();++it){
				CVARIANT *X = new CVARIANT();
				X->avtoSet("string");
				*X->DATA.ps = (*it)->name;
				V->DATA.vectorVal->push_back(X);
				}
			return;
			}
		if(name=="NUsers"){
			V=new(CVARIANT);
			V->avtoSet("int");
			V->DATA.intVal = file->Nusers;
			return;
			}
		if(name=="getAllInclude" || name=="getALLInclude"){
			V = new(CVARIANT);
			V->avtoSet("vector");
			CVARIANT K;
			K=(*M->GlobalSpace.Map.DATA.mapVal)["WAY"];
			K.TransformType("string");
			int n = K.DATA.ps->size();
			V_S::iterator it = file->modules.begin();
			for(;it!=file->modules.end();++it){
				string str=*it;
				str=str.substr(n+1);
				int p=str.find_last_of(".");
				if(p>=0)str=str.substr(0,p);
				File*xf=M->findModule(str);
				if(!xf)continue;
				CVARIANT *K = new CVARIANT();
				K->avtoSet("module");
				K->DATA.moduleVal->id = xf->id;
				V->DATA.vectorVal->push_back(K);
				}
			return;
			}
		if(name=="getUsers"){
			V=new(CVARIANT);
			V->avtoSet("vector");
			CVARIANT K;
			K=(*M->GlobalSpace.Map.DATA.mapVal)["WAY"];
			K.TransformType("string");
			string g=*K.DATA.ps+"\\"+file->way+"\\"+file->name;
			M_iFile::iterator it=M->Files.begin();
			for(;it!=M->Files.end();++it){
				File*xf=it->second;
				V_S::iterator u=xf->modules.begin();
				for(;u!=xf->modules.end();++u){
					string str=*u;
					int p=str.find_last_of(".");
					if(p>=0)str=str.substr(0,p);
					if(str==g){
						CVARIANT *K = new CVARIANT();
						K->avtoSet("module");
						K->DATA.moduleVal->id=xf->id;
						V->DATA.vectorVal->push_back(K);
						}
					}
				}
			return;
			}
		if(name=="add")if(VCV.size()>0){
			if(VCV[0].isType("module")){
				File*fme=M->Files[VCV[0].DATA.moduleVal->id];
				++fme->Nusers;
				CVARIANT K;
				K=(*M->GlobalSpace.Map.DATA.mapVal)["WAY"];
				K.TransformType("string");
				string WAY=*K.DATA.ps;
				WAY=WAY+"\\"+fme->way+"\\"+fme->name;
				if(find(file->modules.begin(),file->modules.end(),WAY)==file->modules.end()){
					file->modules.push_back(WAY);
					file->NeedSave=1;
					}
				}
			if(VCV[0].isType("function")){
				Function*func=VCV[0].DATA.functionVal->F;
				func=func->copy();
				file->functions.push_back(func);
				file->NeedSave=1;
				func->file=file;
				V=new(CVARIANT);
				V->avtoSet("function");
				V->DATA.functionVal->F=func;
				V->DATA.functionVal->isOriginal=1;
				}
			return;
			}
		if(name=="replaceFunction")if(VCV.size()>0){
			if(!VCV[0].isType("function"))return;
			Function*func=VCV[0].DATA.functionVal->F;
			Function*f=file->FindFunction(func->name);
			if(f){
				if(f==func)return;
				file->functions.erase(
					find(file->functions.begin(),file->functions.end(),f)
					);
				delete f;
				}
			func=func->copy();
			file->functions.push_back(func);
			file->NeedSave=1;
			func->file=file;
			V=new(CVARIANT);
			V->avtoSet("function");
			V->DATA.functionVal->F=func;
			V->DATA.functionVal->isOriginal = 1;
			return;
			}
		if(name=="eraseFunction")if(VCV.size()>0){
			VCV[0].TransformType("string");
			Function*f=file->FindFunction(*VCV[0].DATA.ps);
			if(!f)return;
			file->functions.erase(
				find(file->functions.begin(),file->functions.end(),f)
				);
			delete f;
			file->NeedSave = 1;
			return;
			}
		if(name=="erase")if(VCV.size()>0){
			if(!VCV[0].isType("module"))return;
			File*fme=M->Files[VCV[0].DATA.moduleVal->id];
			--fme->Nusers;
			CVARIANT K;
			K=(*M->GlobalSpace.Map.DATA.mapVal)["WAY"];
			K.TransformType("string");
			string WAY=*K.DATA.ps;
			WAY=WAY+"\\"+fme->way+"\\"+fme->name;
			V_S::iterator it=find(file->modules.begin(),file->modules.end(),WAY);
			if(it!=file->modules.end()){
				file->modules.erase(it);
				file->NeedSave = 1;
				}
			return;
			}
		if(name=="toString"){
			V = new(CVARIANT);
			V->avtoSet("string");
			*V->DATA.ps = file->toString();
			return;
			}
		}
	// file==# 
	if(name=="Actuale" || name=="actuale"){
		V = new(CVARIANT);
		V->avtoSet("bool");
		V->DATA.boolVal = (file!=NULL);
		return;
		}
	if(name=="create")if(VCV.size()>0){
		VCV[0].TransformType("string");
		string str=*VCV[0].DATA.ps;
		int p=str.find_last_of(".");
		if(p>=0)str=str.substr(0,p);
		p=str.find_last_of("\\");
		File*ff = new File(M);
		M->Files[ff->id] = ff;
		ff->name=(p<0?str:str.substr(p+1));
		ff->way=(p<0?"":str.substr(0,p));
		V = new(CVARIANT);
		V->avtoSet("module");
		V->DATA.moduleVal->id = ff->id;
		return;
		}
	if(name=="find")if(VCV.size()>0){
		VCV[0].TransformType("string");
		string str=*VCV[0].DATA.ps;
		int p=str.find_last_of(".");
		if(p>=0)str=str.substr(0,p);
		File*xf=M->findModule(str);
		V = new(CVARIANT);
		if(!xf){
			V->avtoSet("int");
			V->DATA.intVal=0;
			return;
			}
		V->avtoSet("module");
		V->DATA.moduleVal->id = xf->id;
		return;
		}
	if(name=="setCode" || name=="setProgram")if(VCV.size()>=1){
		if(file)return;
		VCV[0].TransformType("string");
		string*textProgram = VCV[0].DATA.ps;
		LOADER X;
		X.text = LOADER::copy(*textProgram);
		X.delete_komentary();
		string PHTML;
			{
			Assemble A(PHTML,M,NULL);
			A.Load("",X.text,false);
			file = A.lastFile;
			}
		if(file){
			V = new(CVARIANT);
			V->avtoSet("vector");
			int i, size;
			size = file->modules.size();
			for(i=0;i<size;++i){
				CVARIANT*S = new (CVARIANT);
				S->avtoSet("string");
				*S->DATA.ps = file->modules[i];
				V->DATA.vectorVal->push_back(S);
				}
			file->modules.clear();
			M->Files[file->id] = file;
			thisVal->DATA.moduleVal->id = file->id;
			return;
			}
		V = new(CVARIANT);
		V->avtoSet("string");
		*V->DATA.ps = PHTML;
		return;
		}
}




//	CPP2.cpp	:-|