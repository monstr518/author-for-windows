//	CVARIANT.cpp
#include "main.h"

//--------------------------------------------------------------------------------------------------
CInterval::CInterval(){A=B=a=b=0;}
CInterval::CInterval(int aa,double AA,double BB,int bb){A=AA;B=BB;a=aa;b=bb;}

CInterval::CInterval(const CInterval&t){*this=t;}


CInterval& CInterval::operator = (const CInterval&t){
	A=t.A;
	B=t.B;
	a=t.a;
	b=t.b;
	return *this;
}


bool CInterval::operator == (const CInterval&t) const{
	return A==t.A && B==t.B && a==t.a && b==t.b;
}


bool CInterval::operator >  (const CInterval&t) const {return B>t.A;}
bool CInterval::operator <  (const CInterval&t) const {return B<t.A;}


CInterval::operator string(){
	control();
	string s;
	bool is8;
	s+=(a==1)?"[":"(";
	int t=A;
	is8 = ((a&2)!=0);
	if(is8)s+="#"; else
	if(A==(double)t)s+=SCANER::toString(t);else s+=SCANER::toString(A);
	s+=";";
	t=B;
	is8 = ((b&2)!=0);
	if(is8)s+="#"; else
	if(B==(double)t)s+=SCANER::toString(t);else s+=SCANER::toString(B);
	s+=(b==1)?"]":")";
	return s;
}


void CInterval::toString(string&s){s=(string)*this;}

void CInterval::control(){
	if((a&2) || (b&2))return;
	if(A<=B)return;
	double D=A;
	A=B;
	B=D;
	bool d = (a!=0);
	a=b;
	b=d;
}


bool CInterval::operator &=  (const CInterval&t){
	bool ok=0;
	if(t.B<A || B<t.A)ok=1;
	if(t.B==A && !t.b && !a)ok=1;
	if(t.A==B && !t.a && !b)ok=1;
	if(ok){
		A=B=0;
		a=b=0;
		return 0;
		}
	if(t.B==A && (t.b||a)){
		B=A;
		a=b=1;
		return 1;
		}
	if(B==t.A && (b||t.a)){
		A=B;
		a=b=1;
		return 1;
		}
	ok=0;
	if(t.A>=A){
		A=t.A;
		a=t.a;
		ok=1;
		}
	if(t.B<=B){
		B=t.B;
		b=t.b;
		ok=1;
		}
	if(A==t.A)a=a&&t.a;
	if(B==t.B)b=b&&t.b;
	return ok;
}


bool CInterval::operator |=  (const CInterval&t){
	if(t.B<A || B<t.A)return 0;
	if(t.B==A && !t.b && !a)return 0;
	if(t.A==B && !t.a && !b)return 0;
	if(t.B==A && (t.b||a)){
		A=t.A;
		a=t.a;
		return 1;
		}
	if(B==t.A && (b||t.a)){
		B=t.B;
		b=t.b;
		return 1;
		}
	bool ok=0;
	if(t.A<=A && t.B>A){
		A=t.A;
		a=t.a;
		ok=1;
		}
	if(t.B>=A && t.A>A){
		B=t.B;
		b=t.b;
		ok=1;
		}
	if(A==t.A)a=a||t.a;
	if(B==t.B)b=b||t.b;
	if(t.A<A){
		A=t.A;
		a=t.a;
		ok=1;
		}
	if(t.B>B){
		B=t.B;
		b=t.b;
		ok=1;
		}
	return ok;
}



//--------------------------------------------------------------------------------------------------
CFunction::CFunction(Function*f){F=f;isOriginal=0;}

CFunction::~CFunction(){
	if(!isOriginal)if(F)delete F;
}

CFunction::CFunction(const CFunction&t){
	F=NULL;
	isOriginal=0;
	*this=t;
}

CFunction& CFunction::operator = (const CFunction&t){
	if(!isOriginal)if(F)delete F;
	F=t.F;
	isOriginal=t.isOriginal;
	if(!isOriginal)F=F->copy();
	return *this;
}


bool CFunction::operator == (const CFunction&t) const{
	if(F==t.F)return 1;
	//if(isOriginal!=t.isOriginal)return 0;
	return *F==*t.F;
}


bool CFunction::operator > (const CFunction&t) const{
	if(isOriginal!=t.isOriginal)return isOriginal>t.isOriginal;
	t.F->controlSxema();
	F->controlSxema();
	int a=F->sxema->table.size();
	int b=t.F->sxema->table.size();
	if(a!=b)return a>b;
	return *F>*t.F;
}


bool CFunction::operator < (const CFunction&t) const{
	if(isOriginal!=t.isOriginal)return isOriginal<t.isOriginal;
	t.F->controlSxema();
	F->controlSxema();
	int a=F->sxema->table.size();
	int b=t.F->sxema->table.size();
	if(a!=b)return a<b;
	return *F<*t.F;
}


string CFunction::toString(){
	if(F)return F->toString();
	return "Error";
}


//--------------------------------------------------------------------------------------------------
CModule::CModule(int i){id=i;}
CModule::CModule(const CModule&t){id=t.id;}
CModule& CModule:: operator = (const CModule&t){id=t.id;return *this;}

bool CModule:: operator == (const CModule&t) const {return id==t.id;}
bool CModule:: operator >  (const CModule&t) const {return id > t.id;}
bool CModule:: operator <  (const CModule&t) const {return id < t.id;}


//--------------------------------------------------------------------------------------------------
CVARIANT::CVARIANT(){Ntype = 0;}
CVARIANT::~CVARIANT(){clear();}
void CVARIANT::clear(){
	if(isType("string"))if(DATA.ps)delete DATA.ps;
	if(isType("pointer"))if(DATA.ps)delete DATA.ps;
	#define defdel(ss,uu)	if(isType(ss))if(DATA.uu){delete DATA.uu;Ntype = 0;return;}
	defdel("double",dblVal);
	defdel("deque",dequeVal);
	defdel("map",mapVal);
	if(isType("vector")){
		if(DATA.vectorVal){
			V_pCVARIANT::iterator it = DATA.vectorVal->begin();
			for(;it!=DATA.vectorVal->end();++it)
				if(*it)delete *it;
			delete DATA.vectorVal;
			}
		Ntype = 0;
		return;
		}
	defdel("set",setVal);
	defdel("interval",intervalVal);
	defdel("function",functionVal);
	defdel("program",programVal);
	defdel("graph",grafVal);
	defdel("digit",digitVal);
	defdel("module",moduleVal);
	Ntype = 0;
}


int CVARIANT::getN(const char*s){
	int i;
	for(i=0;name[i];++i)
		if(!strcmp(name[i],s))return i;
	return 0;
}



bool CVARIANT::isType(const char*s) const {
	return !strcmp(name[Ntype],s);
}



const char* CVARIANT::name[]={
	"void",
	"char",
	"short",
	"unsigned short",
	"int",
	"long",
	"unsigned long",
	"float",
	"double",
	"bool",
	"string",
	"vector",
	"set",
	"deque",
	"map",
	"interval",
	"function",
	"quantum",
	"program",
	"graph",
	"digit",
	"module",
	"pointer",
	NULL
};



//--------------------------------------------------------------------------------------------------
CVARIANT::CVARIANT(const CVARIANT&t){
	Ntype = 0;
	*this = t;
}



CVARIANT& CVARIANT::operator = (const CVARIANT&t){
	if(this==&t)return *this;
	clear();
	Ntype=t.Ntype;
	DATA=t.DATA;
	if(t.isType("double")){
		DATA.dblVal = new double(*t.DATA.dblVal);
		return *this;
		}
	#define def_copy(ss,tclass,uu)	if(t.isType(ss)) \
			{DATA.uu=new (tclass)(*t.DATA.uu);return *this;}
	def_copy("string",string,ps);
	def_copy("pointer",string,ps);
	if(t.isType("vector")){
		DATA.vectorVal = new V_pCVARIANT(*t.DATA.vectorVal);
		int i,size = DATA.vectorVal->size();
		for(i=0;i<size;++i){
			(*DATA.vectorVal)[i] = (*t.DATA.vectorVal)[i]->copy();
			}
		return *this;
		}
	def_copy("deque",deque_CVARIANT,dequeVal);
	def_copy("set",S_CVARIANT,setVal);
	def_copy("map",M_SV,mapVal);
	def_copy("interval",CInterval,intervalVal);
	def_copy("function",CFunction,functionVal);
	if(t.isType("program")){
		DATA.programVal=t.DATA.programVal->copy();
		DATA.programVal->initUP(NULL);
		}
	def_copy("graph",UGraf,grafVal);
	def_copy("digit",Digits,digitVal);
	def_copy("module",CModule,moduleVal);
	return *this;
}




bool CVARIANT::operator == (const CVARIANT&t) const{
	if(Ntype!=t.Ntype)return 0;
	if(!Ntype)return 1;
	#define def_compare(ss,uu)	if(isType(ss))return (*DATA.uu==*t.DATA.uu);
	def_compare("string",ps);
	def_compare("pointer",ps);
	//def_compare("vector",vectorVal);
	if(isType("vector")){
		int i,size = DATA.vectorVal->size();
		bool isOK = (size == t.DATA.vectorVal->size());
		if(!isOK)return 0;
		i = 0;
		for(;i<size;++i){
			isOK = (*(*DATA.vectorVal)[i] == *(*t.DATA.vectorVal)[i]);
			if(!isOK)break;
			}
		return isOK;
		}
	def_compare("double",dblVal);
	def_compare("deque",dequeVal);
	def_compare("set",setVal);
	def_compare("interval",intervalVal);
	def_compare("map",mapVal);
	def_compare("function",functionVal);
	def_compare("program",programVal);
	def_compare("graph",grafVal);
	def_compare("digit",digitVal);
	def_compare("module",moduleVal);
	string s1,s2;
	toString(s1);
	t.toString(s2);
	return s1==s2;
}


bool CVARIANT::operator != (const CVARIANT&t) const{return !(t==*this);}

bool CVARIANT::operator >  (const CVARIANT&t) const{
	if(t.Ntype<Ntype)return 0;
	if(t.Ntype>Ntype)return 1;
	#define fop(ss,uu)	if(isType(ss))return DATA.uu>t.DATA.uu;
	fop("unsigned long",ulVal);
	fop("long",lVal);
	fop("int",intVal);
	fop("short",iVal);
	fop("unsigned short",uiVal);
	fop("char",bVal);
	fop("float",fltVal);
	//fop("double",dblVal);
	if(isType("double"))return *DATA.dblVal>*t.DATA.dblVal;
	#define def_compare(ss,uu)	if(isType(ss))return (*DATA.uu>*t.DATA.uu);
	def_compare("string",ps);
	def_compare("pointer",ps);
	def_compare("type",ps);
	if(isType("vector")){
		int size,sizet;
		size = DATA.vectorVal->size();
		sizet = t.DATA.vectorVal->size();
		bool isOK = (size==sizet);
		if(!isOK)return (size>sizet);
		int i = 0;
		for(;i<size;++i){
			isOK = (*(*DATA.vectorVal)[i] == *(*t.DATA.vectorVal)[i]);
			if(isOK)continue;
			isOK = (*(*DATA.vectorVal)[i] > *(*t.DATA.vectorVal)[i]);
			return isOK;
			}
		return false;
		}
	def_compare("deque",dequeVal);
	def_compare("set",setVal);
	def_compare("map",mapVal);
	def_compare("interval",intervalVal);
	def_compare("function",functionVal);
	def_compare("program",programVal);
	def_compare("graph",grafVal);
	def_compare("digit",digitVal);
	def_compare("module",moduleVal);
	return 0;
}

bool CVARIANT::operator <  (const CVARIANT&t) const{
	if(t.Ntype<Ntype)return 1;
	if(t.Ntype>Ntype)return 0;
	#define fop(ss,uu)	if(isType(ss))return DATA.uu<t.DATA.uu;
	fop("unsigned long",ulVal);
	fop("long",lVal);
	fop("int",intVal);
	fop("short",iVal);
	fop("unsigned short",uiVal);
	fop("char",bVal);
	fop("float",fltVal);
	fop("bool",boolVal);
	//fop("double",dblVal);
	if(isType("double"))return *DATA.dblVal<*t.DATA.dblVal;
	#define def_compare(ss,uu)	if(isType(ss))return (*DATA.uu<*t.DATA.uu);
	def_compare("string",ps);
	def_compare("pointer",ps);
	def_compare("type",ps);
	//def_compare("vector",vectorVal);
	if(isType("vector")){
		int size,sizet;
		size = DATA.vectorVal->size();
		sizet = t.DATA.vectorVal->size();
		bool isOK = (size==sizet);
		if(!isOK)return (size<sizet);
		int i = 0;
		for(;i<size;++i){
			isOK = (*(*DATA.vectorVal)[i] == *(*t.DATA.vectorVal)[i]);
			if(isOK)continue;
			isOK = (*(*DATA.vectorVal)[i] < *(*t.DATA.vectorVal)[i]);
			return isOK;
			}
		return false;
		}
	def_compare("deque",dequeVal);
	def_compare("set",setVal);
	def_compare("map",mapVal);
	def_compare("interval",intervalVal);
	def_compare("function",functionVal);
	def_compare("program",programVal);
	def_compare("graph",grafVal);
	def_compare("digit",digitVal);
	def_compare("module",moduleVal);
	return 1;
}

bool CVARIANT::operator >= (const CVARIANT&t) const{return !(*this<t);}

bool CVARIANT::operator <= (const CVARIANT&t) const{return !(*this>t);}


//--------------------------------------------------------------------------------------------------
//1++ 0--
void CVARIANT::wplus(bool plus){
	#define fop(ss,uu)	if(isType(ss)) {if(plus)++DATA.uu; else --DATA.uu;}
	fop("unsigned long",ulVal);
	fop("long",lVal);
	fop("int",intVal);
	fop("short",iVal);
	fop("unsigned short",uiVal);
	fop("char",bVal);
	fop("float",fltVal);
	fop("bool",dblVal);
	if(isType("digit")){if(plus)++*DATA.digitVal; else --*DATA.digitVal;}
	//fop("double",dblVal);
	if(isType("double")){
		if(plus)++*DATA.dblVal; else --*DATA.dblVal;
		}
}



CVARIANT& CVARIANT::operator -- (){
	wplus(false);
	return *this;
}



CVARIANT& CVARIANT::operator ++ (){
	wplus(true);
	return *this;
}



//--------------------------------------------------------------------------------------------------
void CVARIANT::toString(string&s) const{
	if(isType("void"))s+="#";
	if(isType("string"))s+=SCANER::writeString(*DATA.ps);
	if(isType("pointer"))s+=*DATA.ps;
	char t[200]="";
	if(isType("short"))		sprintf(t,"%d",DATA.iVal);
	if(isType("int"))		sprintf(t,"%d",DATA.intVal);
	if(isType("char"))		sprintf(t,"%c",DATA.bVal);
	if(isType("unsigned short"))sprintf(t,"%d",DATA.uiVal);
	if(isType("long"))		sprintf(t,"%d",DATA.lVal);
	if(isType("unsigned long"))sprintf(t,"%u",DATA.ulVal);
	if(isType("float"))		sprintf(t,"%0.7f",DATA.fltVal);
	if(isType("double"))	sprintf(t,"%0.7f",*DATA.dblVal);
	if(isType("float")||isType("double")){
		int i=0;
		bool ok=0;
		for(;t[i];++i)if(t[i]=='.')ok=1;
		if(ok){
			--i;
			for(;i>0&&t[i]!='.';--i)if(t[i]=='0')t[i]=0;else break;
			if(t[i]=='.')t[i]=0;
			}
		}
	s+=t;
	if(isType("bool"))		s+=DATA.boolVal?"true":"false";
	if(!DATA.ps)return;
	if(isType("map")){
		s+="map";
		M_SV::iterator it=DATA.mapVal->begin();
		for(;it!=DATA.mapVal->end();++it){
			s+="[";
			s+=it->first;
			s+="=";
			it->second.toString(s);
			s+="]";
			}
		return;
		}
	if(isType("set")){
		s+="set[";
		S_CVARIANT::iterator it=DATA.setVal->begin();
		for(bool fi=0;it!=DATA.setVal->end();++it,fi=1){
			if(fi)s+=',';
			it->toString(s);
			}
		s+="]";
		return;
		}
	if(isType("deque")){
		s+="deque[";
		deque_CVARIANT::iterator it=DATA.dequeVal->begin();
		for(bool fi=0;it!=DATA.dequeVal->end();++it,fi=1){
			if(fi)s+=',';
			it->toString(s);
			}
		s+="]";
		return;
		}
	if(isType("vector")){
		s+="vector[";
		V_pCVARIANT::iterator it=DATA.vectorVal->begin();
		for(bool fi=0;it!=DATA.vectorVal->end();++it,fi=1){
			if(fi)s+=',';
			(*it)->toString(s);
			}
		s+="]";
		return;
		}
	if(isType("function")){
		if(DATA.functionVal)s+=DATA.functionVal->toString();
		return;
		}
	if(isType("interval")){
		if(DATA.intervalVal)s+=(string)*DATA.intervalVal;
		return;
		}
	if(isType("program")){
		if(DATA.programVal)s+=DATA.programVal->toString();
		}
	if(isType("graph")){
		if(DATA.grafVal)s+=DATA.grafVal->toString();
		}
	if(isType("digit")){
		if(DATA.digitVal)s+=DATA.digitVal->toString();
		}
	if(isType("module")){
		s+="module";
		}
}


CVARIANT* CVARIANT::copy(){
	CVARIANT*x=new(CVARIANT);
	*x=*this;
	return x;
}


bool CVARIANT::sBelongs(const char*s,char**m){
	int i;
	for(i=0;m[i];++i)
		if(!strcmp(s,m[i]))return 1;
	return 0;
}


bool CVARIANT::OPETATOR(CVARIANT*A,CVARIANT*B,const char*c){
	#define is(ss) !strcmp((ss),c)
	if(is("=="))return *A==*B;
	if(is("=")){
		CVARIANT X=*B;
		*A=X;
		return 1;
		}
	char*t[]={"<<=", ">>=","%=",NULL};
	if(sBelongs(c,t)){
		int n=0;
		if(B->isType("unsigned long"))n=B->DATA.ulVal;
		if(B->isType("long"))n=B->DATA.lVal;
		if(B->isType("int"))n=B->DATA.intVal;
		if(B->isType("unsigned short"))n=B->DATA.uiVal;
		if(B->isType("short"))n=B->DATA.iVal;
		if(B->isType("char"))n=B->DATA.bVal;
		#define ff(ss,uu)	if(A->isType(ss)){	\
					if(is("%=")) A->DATA.uu%=n;	\
					if(is("<<="))A->DATA.uu<<=n;\
					if(is(">>="))A->DATA.uu>>=n;}
		ff("unsigned long",ulVal);
		ff("long",lVal);
		ff("int",intVal);
		ff("short",iVal);
		ff("unsigned short",uiVal);
		ff("char",bVal);
		if(A->isType("digit")){
			if(B->isType("digit")){
				n=(int)*B->DATA.digitVal;
				if(is("%=")) *A->DATA.digitVal%=*B->DATA.digitVal;
				if(is("<<="))*A->DATA.digitVal<<n;
				if(is(">>="))*A->DATA.digitVal>>n;
				}else{
				if(is("%=")){
					Digits X;
					X.set(n);
					*A->DATA.digitVal%=X;
					}
				if(is("<<="))*A->DATA.digitVal<<n;
				if(is(">>="))*A->DATA.digitVal>>n;
				}
			}
		return 1;
		}
	if(is("||=") || is("&&=")){
		if(A->Ntype)A->TransformType("bool");
		if(B->Ntype)B->TransformType("bool");
		if(is("||=")){
			if(A->Ntype && B->Ntype)A->DATA.boolVal=A->DATA.boolVal||B->DATA.boolVal; else{
				bool ok=0;
				if(A->Ntype)ok=(A->DATA.boolVal);
				if(B->Ntype)ok=(B->DATA.boolVal);
				if(ok){
					A->TransformType("bool");
					A->DATA.boolVal=1;
					}else{
					A->avtoSet("void");
					}
				}
			}else{
			if(A->Ntype && B->Ntype)A->DATA.boolVal=A->DATA.boolVal&&B->DATA.boolVal; else{
				bool ok=1;
				if(A->Ntype)ok=(A->DATA.boolVal);
				if(B->Ntype)ok=(B->DATA.boolVal);
				if(!ok){
					A->TransformType("bool");
					A->DATA.boolVal=0;
					}else{
					A->avtoSet("void");
					}
				}
			}
		return 1;
		}
	if(A->Ntype!=B->Ntype)return 0;
	char*t2[]={"|=", "&=", "^=",NULL};
	if(sBelongs(c,t2)){
		#define fop(t,uu)	if(A->isType(#t)){ \
			t&a=A->DATA.uu,b=B->DATA.uu;\
			if(is("^="))a^=b;\
			if(is("|="))a|=b;\
			if(is("&="))a&=b;\
			return 1; }
		fop(unsigned long,ulVal);
		fop(long,lVal);
		fop(int,intVal);
		fop(short,iVal);
		fop(unsigned short,uiVal);
		fop(char,bVal);
		fop(bool,boolVal);
		if(A->isType("interval")){
			if(is("|="))*A->DATA.intervalVal |= *B->DATA.intervalVal;
			if(is("&="))*A->DATA.intervalVal &= *B->DATA.intervalVal;
			return 1;
			}
		return 0;
		}
	char*tp[]={"+=", "-=", "*=", "/=",NULL};
	if(!sBelongs(c,tp))return 0;
	#define fop(ss,uu)	if(A->isType(ss)){ \
					if(is("+="))A->DATA.uu+=B->DATA.uu;\
					if(is("-="))A->DATA.uu-=B->DATA.uu;\
					if(is("*="))A->DATA.uu*=B->DATA.uu;\
					if(is("/="))A->DATA.uu/=B->DATA.uu;}
	fop("unsigned long",ulVal);
	fop("long",lVal);
	fop("int",intVal);
	fop("short",iVal);
	fop("unsigned short",uiVal);
	fop("char",bVal);
	fop("float",fltVal);
	//fop("double",dblVal);
	if(A->isType("double")){
		if(is("+="))*A->DATA.dblVal += *B->DATA.dblVal;
		if(is("-="))*A->DATA.dblVal -= *B->DATA.dblVal;
		if(is("*="))*A->DATA.dblVal *= *B->DATA.dblVal;
		if(is("/="))*A->DATA.dblVal /= *B->DATA.dblVal;
		}
	if(A->isType("string")){
		if(is("+="))*A->DATA.ps += *B->DATA.ps;
		}
	if(A->isType("set")){
		if(is("+=")){
			S_CVARIANT::iterator it=B->DATA.setVal->begin();
			for(;it!=B->DATA.setVal->end();++it)A->DATA.setVal->insert(*it);
			}
		if(is("-=")){
			S_CVARIANT::iterator it=B->DATA.setVal->begin();
			for(;it!=B->DATA.setVal->end();++it)A->DATA.setVal->erase(*it);
			}
		if(is("*=")){
			CVARIANT Q;
			Q.avtoSet("set");
			S_CVARIANT::iterator jt,it=B->DATA.setVal->begin();
			for(;it!=B->DATA.setVal->end();++it){
				jt=find(A->DATA.setVal->begin(),A->DATA.setVal->end(),*it);
				if(jt==A->DATA.setVal->end())continue;
				Q.DATA.setVal->insert(*jt);
				}
			*A=Q;
			}
		}
	if(A->isType("vector")){
		if(is("+=")){
			V_pCVARIANT::iterator it=B->DATA.vectorVal->begin();
			for(;it!=B->DATA.vectorVal->end();++it){
				A->DATA.vectorVal->push_back((*it)->copy());
				}
			}
		if(is("-=")){
			V_pCVARIANT::iterator jt,it=B->DATA.vectorVal->begin();
			for(;it!=B->DATA.vectorVal->end();++it){
				jt=find_pointer(A->DATA.vectorVal->begin(),A->DATA.vectorVal->end(),**it);
				if(jt==A->DATA.vectorVal->end())continue;
				A->DATA.vectorVal->erase(jt);
				}
			}
		if(is("*=")){
			CVARIANT Q;
			Q.avtoSet("vector");
			V_pCVARIANT::iterator jt,it=B->DATA.vectorVal->begin();
			for(;it!=B->DATA.vectorVal->end();++it){
				jt=find_pointer(A->DATA.vectorVal->begin(),A->DATA.vectorVal->end(),**it);
				if(jt==A->DATA.vectorVal->end())continue;
				Q.DATA.vectorVal->push_back((*jt)->copy());
				}
			*A=Q;
			}
		}
	if(A->isType("digit")){
		if(is("+="))*A->DATA.digitVal+=*B->DATA.digitVal;
		if(is("-="))*A->DATA.digitVal-=*B->DATA.digitVal;
		if(is("*="))*A->DATA.digitVal*=*B->DATA.digitVal;
		if(is("/="))*A->DATA.digitVal/=*B->DATA.digitVal;
		}
	if(A->isType("map")){
		if(is("+=")){
			M_SV::iterator it=B->DATA.mapVal->begin();
			for(;it!=B->DATA.mapVal->end();++it)(*A->DATA.mapVal)[it->first]=it->second;
			}
		if(is("-=")){
			M_SV::iterator it=B->DATA.mapVal->begin(),jt;
			for(;it!=B->DATA.mapVal->end();++it){
				jt=A->DATA.mapVal->find(it->first);
				if(jt!=A->DATA.mapVal->end())A->DATA.mapVal->erase(jt);
				}
			}
		}
	return 1;
}





void CVARIANT::INVERT(const char*s){
	if(!strcmp(s,"!") && isType("bool")){
		DATA.boolVal=!DATA.boolVal;
		return;
		}
	if(!strcmp(s,"~")){
		#define ff(ss,uu)	if(isType(ss))DATA.uu=~DATA.uu;
		ff("unsigned long",ulVal);
		ff("long",lVal);
		ff("int",intVal);
		ff("short",iVal);
		ff("unsigned short",uiVal);
		ff("char",bVal);
		return;
		}
	if(strcmp(s,"-"))return;
	#define fop(ss,uu)	if(isType(ss))DATA.uu=-DATA.uu;
	fop("unsigned long",ulVal);
	fop("long",lVal);
	fop("int",intVal);
	fop("short",iVal);
	fop("unsigned short",uiVal);
	fop("char",bVal);
	fop("float",fltVal);
	//fop("double",dblVal);
	if(isType("double"))*DATA.dblVal = -*DATA.dblVal;
	if(isType("digit"))DATA.digitVal->invertZnak();
}



bool CVARIANT::compare(CVARIANT*A,CVARIANT*B,const char*s){
	#define is(ss)	!strcmp(s,(ss))
	if(is("=="))return *A==*B;
	if(is("!="))return *A!=*B;
	if(A->Ntype!=B->Ntype)return 0;
	if(A->isType("bool")){
		bool a=A->DATA.boolVal,b=B->DATA.boolVal;
		if(is("&&"))return a && b;
		if(is("||"))return a || b;
		if(is("^")) return a ^  b;
		}
	if(is("<")) return *A <  *B;
	if(is(">")) return *A >  *B;
	if(is("<="))return *A <= *B;
	if(is(">="))return *A >= *B;
	return 0;
}



void CVARIANT::TransformType(const char*nameType){
	if(isType(nameType))return;
	if(!strcmp(nameType,"void"))return;
	double n=0;
	string str;
	bool b=0;
	V_CVARIANT List;
	#define fop(ss,uu)	if(isType(ss))b=n=DATA.uu;
	fop("unsigned long",ulVal);
	fop("long",lVal);
	fop("int",intVal);
	fop("quantum",intVal);
	fop("short",iVal);
	fop("unsigned short",uiVal);
	fop("float",fltVal);
	fop("bool",boolVal);
	fop("char",bVal);
	//if(isType("char"))b=n=DATA.bVal;
	//fop("double",dblVal);
	if(isType("double"))b = n = *DATA.dblVal;
	if(isType("string")){
		str=*DATA.ps;
		sscanf(DATA.ps->c_str(),"%lf",&n);
		b=str.size();
		}else{
		toString(str);
		if(isType("program"))str=SCANER::trim(str);
		}
	if(isType("vector")){
		V_pCVARIANT::iterator it=DATA.vectorVal->begin();
		for(;it!=DATA.vectorVal->end();++it)List.push_back(**it);
		b=n=DATA.vectorVal->size();
		}
	if(isType("set")){
		S_CVARIANT::iterator it=DATA.setVal->begin();
		for(;it!=DATA.setVal->end();++it)List.push_back(*it);
		b=n=DATA.setVal->size();
		}
	if(isType("digit")){
		str=DATA.digitVal->toString();
		b=n=*DATA.digitVal;
		}
	if(isType("module")){
		str="module";
		n=DATA.moduleVal->id;
		}
	clear();
	//int nn=Ntype;
	Ntype=getN(nameType);
	#define fop(ss,uu)	if(isType(ss)){DATA.uu=n;return;}
	fop("unsigned long",ulVal);
	fop("long",lVal);
	fop("int",intVal);
	fop("short",iVal);
	fop("unsigned short",uiVal);
	fop("char",bVal);
	fop("float",fltVal);
	//fop("double",dblVal);
	if(isType("double")){
		Ntype = 0;
		avtoSet(nameType);
		*DATA.dblVal = n;
		return;
		}
	if(isType("bool")){
		DATA.boolVal=b;
		return;
		}
	if(isType("string")){DATA.ps=new(string)(str);return;}
	if(isType("vector")){
		Ntype = 0;
		avtoSet(nameType);
		DATA.vectorVal = new (V_pCVARIANT);
		V_CVARIANT::iterator it = List.begin();
		for(;it!=List.end();++it)
			DATA.vectorVal->push_back(it->copy());
		return;
		}
	if(isType("set")){
		Ntype = 0;
		avtoSet(nameType);
		DATA.setVal->insert(List.begin(),List.end());
		return;
		}
	if(isType("interval")){
		Ntype = 0;
		avtoSet(nameType);
		(*DATA.intervalVal)=CInterval(1,n,n,1);
		return;
		}
	if(isType("pos")){
		Ntype = 0;
		avtoSet(nameType);
		return;
		}
	if(isType("map")){
		Ntype = 0;
		avtoSet(nameType);
		V_CVARIANT::iterator it=List.begin();
		for(;it!=List.end();++it){
			string str;
			it->toString(str);
			(*DATA.mapVal)[str]=CVARIANT();
			}
		return;
		}
	if(isType("function")){
		Ntype = 0;
		avtoSet(nameType);
		DATA.functionVal->F=new Function();
		DATA.functionVal->F->name="noname";
		return;
		}
	if(isType("program")){
		Ntype = 0;
		avtoSet(nameType);
		return;
		}
	if(isType("graph")){
		Ntype = 0;
		avtoSet(nameType);
		return;
		}
	if(isType("digit")){
		Ntype = 0;
		avtoSet(nameType);
		DATA.digitVal->set(str.c_str());
		return;
		}
	if(isType("module")){
		Ntype = 0;
		avtoSet(nameType);
		DATA.moduleVal->id=n;
		return;
		}
	Ntype = 0;//cennot transform
	avtoSet("string");
	*DATA.ps=string("cennot transform type to ")+nameType;
}




//выделение памяти для непремитивных типов.
void CVARIANT::avtoSet(string&type){
	clear();
	DATA.ps = NULL;
	Ntype = getN(type.c_str());
	if(isType("double"))DATA.dblVal = new(double);
	#define def_co(ss,tt,uu)	\
		if(isType(ss)) DATA.uu=new(tt);
	def_co("string",string,ps);
	def_co("pointer",string,ps);
	//def_co("type",string,ps);
	def_co("vector",V_pCVARIANT,vectorVal);
	def_co("deque",deque_CVARIANT,dequeVal);
	def_co("set",S_CVARIANT,setVal);
	def_co("map",M_SV,mapVal);
	def_co("interval",CInterval,intervalVal);
	def_co("function",CFunction,functionVal);
	if(isType("program")){
		Base*A=new Base();
		A->text="0";
		A->type=3;
		A->up=NULL;
		DATA.programVal=A;
		}
	def_co("graph",UGraf,grafVal);
	def_co("digit",Digits,digitVal);
	def_co("module",CModule,moduleVal);
}


//выделение памяти для непремитивных типов.
void CVARIANT::avtoSet(const char*type){
	string t(type);
	avtoSet(t);
}



int CVARIANT::getSizeType(int i){return getSizeType(name[i]);}

int CVARIANT::getSizeType(const char*s){
	#define fop(uu)	if(!strcmp(s,#uu))return sizeof(uu);
	fop(unsigned long);
	fop(long);
	fop(int);
	fop(short);
	fop(unsigned short);
	fop(char);
	fop(float);
	fop(bool);
	//fop(double);
	if(!strcmp(s,"double"))return sizeof(double);
	return sizeof(void*);
}




//преобразует любой тип в string
string CVARIANT::getString(){
	if(isType("string"))return *DATA.ps;
	if(isType("pointer"))return *DATA.ps;
	CVARIANT X(*this);
	X.TransformType("string");
	return *X.DATA.ps;
}



//преобразует любой тип в double
double CVARIANT::getDouble(){
	if(isType("double"))return *DATA.dblVal;
	CVARIANT X(*this);
	X.TransformType("double");
	return *X.DATA.dblVal;
}





int CVARIANT::getSizeOf() const {
	int size = sizeof(CVARIANT::DATA);
	if(isType("string") || isType("pointer"))size += DATA.ps->size();
	if(isType("set")){
		S_CVARIANT::iterator it = DATA.setVal->begin();
		for(;it!=DATA.setVal->end();++it)size += it->getSizeOf();
		}
	// deque_CVARIANT*	dequeVal; - Never used
	if(isType("vector")){
		int i,vs = DATA.vectorVal->size();
		size += vs;
		for(i=0;i<vs;++i)size += (*DATA.vectorVal)[i]->getSizeOf();
		}
	if(isType("map")){
		M_SV::iterator it = DATA.mapVal->begin();
		for(;it!=DATA.mapVal->end();++it){
			size += it->first.size();
			size += it->second.getSizeOf();;
			}
		}
	if(isType("interval")){
		size += sizeof(CInterval);
		}
	if(isType("function")){
		size += 1000;
		}
	if(isType("program")){
		size += DATA.programVal->Power(0)*10;
		}
	if(isType("graph")){
		size += DATA.grafVal->getSizeOf();
		}
	if(isType("digit")){
		size += DATA.digitVal->getSizeOf();
		}
	if(isType("module")){
		size += sizeof(CModule);
		}
	return size;
}





//	CVARIANT.cpp	:-|