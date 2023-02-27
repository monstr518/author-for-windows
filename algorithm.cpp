// algorithm.cpp

#include "main.h"

//--------------------------------------------------------------------------------------------------
oneterm::oneterm(){A=NULL;}

oneterm::~oneterm(){
	if(A)delete A;
}

oneterm::oneterm(const oneterm&t){
	*this=t;
}


oneterm& oneterm:: operator = (const oneterm&t){
	c=t.c;
	nfunc=t.nfunc;
	term=t.term;
	if(t.A)A=t.A->copy();else A=NULL;
	return *this;
}


char* oneterm::m[]={
	">>=",
	"<<=",
	">>",
	"<<",
	"::",
	"||",
	"&&",
	"->",
	"==",
	"!=",
	"-=",
	"+=",
	"/=",
	"*=",
	"|=",
	"&=",
	"^=",
	//"#=",
	"++",
	"--",
	"<=",
	">=",
	"%=",
	"^^",
	//"!!",
	"??",
	"=",
	"&",
	"|",
	"^",
	"*",
	"/",
	"-",
	"+",
	"%",
	".",
	":",
	",",
	";",
	"(",
	"[",
	"{",
	"}",
	"]",
	")",
	"<",
	">",
	"?",
	"!",
	"#",
	NULL
};



oneterm oneterm::getTerm(char*&s,Assemble*AS){
	string t;
	char*s2=s;
	int nfunc=0;
	Algorithm*A=NULL;
	int isFunction = SCANER::scanSlovo("function",s);
	if(isFunction || *s=='('){
		SCANER::noProbel(s);
		int er=0,x;
		string name;
		BaseFunction*BF=new BaseFunction;
		if(*s=='(')++s; else er=1;
		if(!er){
			SCANER::noProbel(s);
			while(*s!=')' && *s){
				x=0;
				name=SCANER::readName(s);
				if(name.empty()){
					x=SCANER::scanSlovo("...",s);
					if(x)BF->argumentsON=1;
					} else BF->argumentsNames.push_back(name);
				if(name.empty() && !x)break;
				SCANER::noProbel(s);
				if(*s==')')break;
				if(*s!=',')break;
				++s;
				SCANER::noProbel(s);
				}
			if(*s!=')')er=1;
			++s;
			}
		if(!er){
			SCANER::noProbel(s);
			if(!isFunction){
				if(!SCANER::scanSlovo("=>",s))er=1;
				SCANER::noProbel(s);
				}
			}
		if(!er){
			A=AS->getNabor(s,er);
			if(A){
				Sequence*S=dynamic_cast<Sequence*>(A);
				if(!S){
					S=new Sequence;
					S->nabor.push_back(A);
					A=S;
					}
				BF->Body=A;
				}
			}
		if(er){
			s=s2;
			delete BF;
			} else {
			nfunc=6;
			t="#";
			A=BF;
			}
		}
	if(!nfunc)t=SCANER::readName(s);
	if(t.empty()){
		s=s2;
		nfunc=1;
		t=SCANER::readString(s);
		}
	if(t.empty()){
		s=s2;
		nfunc=2;
		t=SCANER::readChar(s);
		}
	if(t.empty()){
		s=s2;
		nfunc=3;
		t=SCANER::readDigit(s);
		}
	if(t.empty()){
		s=s2;
		nfunc=4;
		char*r=getSTerm(s);
		if(r)t=r;
		}
	oneterm r;
	r.c=s2;
	r.term=t;
	r.nfunc=nfunc;
	r.A=A;
	return r;
}


char* oneterm::getSTerm(char*&s){
	int i;
	for(i=0;m[i];++i)
		if(SCANER::scanSlovo(m[i],s))break;
	return m[i];
}


//--------------------------------------------------------------------------------------------------
Type::Type(){n=0;isAmpersent=isConst=0;}

Type* Type::getType(L_OT::iterator&pos){
	string name;
	int t=1;
	while(!pos->nfunc || pos->nfunc==4){ // unsigned long double
		if(pos->nfunc==4){
			if(pos->term!="::")break;
			t=1;
			}else if(!t)name+=" ";else t=0;
		name+=pos->term;
		++pos;
		}
	if(name.empty())return NULL;
	Type*x=new(Type);
	while(pos->term=="*"){
		++x->n;
		++pos;
		}
	if(pos->term=="&"){
		x->isAmpersent=1;
		++pos;
		}
	x->name=name;
	return x;
}


string Type::toString(){
	string s;
	if(isConst)s="const ";
	s+=name;
	int i;
	for(i=0;i<n;++i)s+="*";
	if(isAmpersent)s+="&";
	return s;
}


bool Type:: operator == (const Type&t) const{
	if(t.name!=name)return 0;
	if(t.isAmpersent!=isAmpersent)return 0;
	if(t.isConst!=isConst)return 0;
	return t.n==n;
}


bool Type:: operator != (const Type&t) const{
	return !(*this==t);
}


bool Type::isSET(){
	if(name=="set")if(!isAmpersent)if(!n)return 1;
	return 0;
}


bool Type:: operator > (const Type&t) const{
	if(t.name!=name)return name>t.name;
	if(t.isAmpersent!=isAmpersent)return isAmpersent>t.isAmpersent;
	if(t.isConst!=isConst)return isConst>t.isConst;
	return n>t.n;
}

bool Type:: operator < (const Type&t) const{
	if(t.name!=name)return name<t.name;
	if(t.isAmpersent!=isAmpersent)return isAmpersent<t.isAmpersent;
	if(t.isConst!=isConst)return isConst<t.isConst;
	return n>t.n;
}

//--------------------------------------------------------------------------------------------------
Algorithm::~Algorithm(){}

bool Algorithm:: operator == (const Algorithm&t) const{return 1;}

bool Algorithm::isProsto(){
	//Label*U1=dynamic_cast<Label*>(this);
	Sequence*U2=dynamic_cast<Sequence*>(this);
	//CreateVar*U3=dynamic_cast<CreateVar*>(this);
	//SpecSumbol*U4=dynamic_cast<SpecSumbol*>(this);
	IF*U5=dynamic_cast<IF*>(this);
	WHILE*U6=dynamic_cast<WHILE*>(this);
	return (U2||U5||U6)?0:1;

}

bool Algorithm::isIF(){
	return dynamic_cast<IF*>(this) != NULL;
}

string Algorithm::getAdress(){return "";}


//--------------------------------------------------------------------------------------------------
bool SubBase::needForScobki(Algorithm*t){
	bool nado=1;
	SubBase*x=dynamic_cast<SubBase*>(t->up);
	if(x){
		int p,u;
		p=x->getPriorited();
		u=getPriorited();
		if(u<p)nado=0;
		if(u==p){
			Base2*b2=dynamic_cast<Base2*>(t->up);
			if(b2){
				bool z=(*x->getR()!=t);
				if(b2->isR())z=!z;
				if(z)nado=0;
				}
			if(dynamic_cast<Prefix*>(t))nado=0;
			}
		}else nado=0;
	/*
	Sequence*ss=dynamic_cast<Sequence*>(t->up);
	if(ss)nado=0;
	*/
	return nado;
}


//--------------------------------------------------------------------------------------------------
string Base::toString(){
	if(type==2)return SCANER::writeChar(text[0]);
	if(type==1)return SCANER::writeString(text);
	if(text=="#")return "#";
	return text;
}


Algorithm* Base::copy(){
	Base*x=new(Base);
	x->text=text;
	x->type=type;
	return x;
}


bool Base:: operator == (const Algorithm&t) const{
	const Base*X=dynamic_cast<const Base*>(&t);
	if(!X)return 0;
	if(type!=X->type)return 0;
	return text==X->text;
}

//--------------------------------------------------------------------------------------------------
char* Prefix::m[]={
	"new",
	"!!",
	"::",
	"++",
	"--",
	"+",
	"-",
	"!",
	"*",
	"&",
	"~",
	NULL
};



Prefix::Prefix(){X=NULL;n=0;}

Prefix::~Prefix(){
	if(X)delete X;
}



bool Prefix::isPrefix(string s,int&i){
	for(i=0;m[i];++i)if(s==m[i])return 1;
	return 0;
}


string Prefix::toString(){
	string s;
	if(m[n])s=m[n]; else s="Algorithm";
	if(!n)s+=" ";
	//if(n>1)if(dynamic_cast<SubBase*>(up))s=" "+s;
	s+=X->toString();
	if(needForScobki(this)||n==8)s="("+s+")";
	return s;
}


Algorithm* Prefix::copy(){
	Prefix*x=new(Prefix);
	x->n=n;
	x->X=X->copy();
	return x;
}


bool Prefix:: operator == (const Algorithm&t) const{
	const Prefix*Y=dynamic_cast<const Prefix*>(&t);
	if(!Y)return 0;
	if(n!=Y->n)return 0;
	return *X==*Y->X;
}


//--------------------------------------------------------------------------------------------------
char* Sufix::m[]={
	"++",
	"--",
	"(",
	"[",
	"{",
	NULL
};


Sufix::Sufix(){X=NULL;n=0;}

Sufix::~Sufix(){
	if(X)delete X;
}


bool Sufix::isSufix(string&s,int&i){
	for(i=0;m[i];++i)if(s==m[i])return 1;
	return 0;
}


string Sufix::toString(){
	string s=X->toString();
	s+=m[n];
	if(!dynamic_cast<FOR*>(up))s="("+s+")";
	return s;
}


Algorithm* Sufix::copy(){
	Sufix*x=new(Sufix);
	x->n=n;
	x->X=X->copy();
	return x;
}


bool Sufix:: operator == (const Algorithm&t) const{
	const Sufix*Y=dynamic_cast<const Sufix*>(&t);
	if(!Y)return 0;
	if(n!=Y->n)return 0;
	return *X==*Y->X;
}


//--------------------------------------------------------------------------------------------------
CallFunc::CallFunc():Sufix(){}

CallFunc::~CallFunc(){
	int i;
	for(i=0;i<params.size();++i)delete params[i];
}


string CallFunc::toString(){
	string s=X->toString();
	s+="(";
	int i;
	for(i=0;i<params.size();++i){
		if(i)s+=",";
		s+=params[i]->toString();
		}
	s+=")";
	return s;
}


Algorithm* CallFunc::copy(){
	CallFunc*x=new(CallFunc);
	x->n=n;
	int i;
	for(i=0;i<params.size();++i)
		x->params.push_back(params[i]->copy());
	x->X=X->copy();
	return x;
}


bool CallFunc:: operator == (const Algorithm&t) const{
	const CallFunc*Y=dynamic_cast<const CallFunc*>(&t);
	if(!Y)return 0;
	if(params.size()!=Y->params.size())return 0;
	if(!(*X==*Y->X))return 0;
	int i;
	for(i=0;i<params.size();++i)if(!(*params[i]==*Y->params[i]))return 0;
	return 1;
}


//--------------------------------------------------------------------------------------------------
AccesMasiv::AccesMasiv():Sufix(){P=NULL;}

AccesMasiv::~AccesMasiv(){
	if(P)delete P;
}

string AccesMasiv::toString(){
	string s=P->toString(),t;
	if(n==3)s="["+s+"]"; else s="{"+s+"}";
	t=X->toString();
	if(dynamic_cast<Base2*>(X))t="("+t+")";
	s=t+s;
	return s;
}


Algorithm* AccesMasiv::copy(){
	AccesMasiv*x=new(AccesMasiv);
	x->n=n;
	x->P=P->copy();
	x->X=X->copy();
	return x;
}


bool AccesMasiv:: operator == (const Algorithm&t) const{
	const AccesMasiv*Y=dynamic_cast<const AccesMasiv*>(&t);
	if(!Y)return 0;
	if(!(*X==*Y->X))return 0;
	return *P==*Y->P;
}


//--------------------------------------------------------------------------------------------------
Cast::Cast():Prefix(){}

string Cast::toString(){
	string s;
	if(!n){
		s=cast.toString();
		s="("+s+")";
		string g=X->toString();
		//if(dynamic_cast<CallFunc*>(X) || dynamic_cast<AccesMasiv*>(X))g="("+g+")";
		if(dynamic_cast<AlgoSet*>(X))g="("+g+")";
		s+=g;
		s="("+s+")";
		return s;
		}
	s="dynamic_cast<";
	s+=cast.toString();
	s+=">(";
	s+=X->toString();
	s+=")";
	return s;
}


Algorithm* Cast::copy(){
	Cast*x=new(Cast);
	x->cast=cast;
	x->n=n;
	x->X=X->copy();
	return x;
}


bool Cast:: operator == (const Algorithm&t) const{
	const Cast*Y=dynamic_cast<const Cast*>(&t);
	if(!Y)return 0;
	if(!(*X==*Y->X))return 0;
	return cast==Y->cast;
}




//--------------------------------------------------------------------------------------------------
BaseFunction::BaseFunction(){
	argumentsON=0;
	Body=NULL;
}

BaseFunction::~BaseFunction(){
	if(Body)delete Body;
}



string BaseFunction::toString(){
	string s = "function";
	string t;
	int i;
	for(i=0;i<argumentsNames.size();++i){
		if(i)t+=",";
		t+=argumentsNames[i];
		}
	if(argumentsON){
		if(i)t+=",";
		t+="...";
		}
	s+="("+t+")";
	if(!Body)return s+"{}";
	s+=Body->toString();
	return s;
}


Algorithm* BaseFunction::copy(){
	BaseFunction*R=new BaseFunction;
	R->argumentsNames=argumentsNames;
	R->argumentsON=argumentsON;
	if(Body)R->Body=Body->copy();
	return R;
}


bool BaseFunction:: operator == (const Algorithm&t) const {
	const BaseFunction*BF=dynamic_cast<const BaseFunction*>(&t);
	if(!BF)return 0;
	if(BF->argumentsON!=argumentsON)return 0;
	if(BF->argumentsNames!=argumentsNames)return 0;
	return BF->Body == Body;
}




//--------------------------------------------------------------------------------------------------
char* Base2::m[]={
	"+=", "-=",	"*=", "/=", "%=", "<<=", ">>=", "&=", "|=", "^=", "=", //14 R
	"?", //13 R
	"||", // 12 R
	"&&",
	"|",
	"^",
	"&", //8 R
	"==", "!=", //7 R
	"<=", ">=", "<", ">", //6 R
	">>", "<<", //5 L
	"+", "-", //4 L
	"*", "/", "%", //3 L
	"->", ".", "::", //1 L
	"??",
	NULL
};


bool Base2::isR(){
	return (n<=23);
}


Base2::Base2(){A=B=NULL;}

Base2::~Base2(){
	if(A)delete A;
	if(B)delete B;
}


bool Base2::isBase2(string&s,int&i){
	for(i=0;m[i];++i)if(s==m[i])return 1;
	return 0;
}


int Base2::getPriorited(){
	if(n<=10)return 14;
	if(n<=16)return 24-n;
	if(n<19)return 7;
	if(n<23)return 6;
	if(n<25)return 5;
	if(n<27)return 4;
	if(n<30)return 3;
	if(n==33)return 2;
	return 1;
}


string Base2::toString(){
	string s=A->toString();
	string op=m[n];
	string R=B->toString();
	if((op=="-" && R[0]=='-') || (op=="+" && R[0]=='+'))s+=op+" "+R; else s+=op+R;
	if(needForScobki(this))s="("+s+")";
	return s;
}


Algorithm* Base2::copy(){
	Base2*x=new(Base2);
	x->n=n;
	x->A=A->copy();
	x->B=B->copy();
	return x;
}


bool Base2:: operator == (const Algorithm&t) const{
	const Base2*X=dynamic_cast<const Base2*>(&t);
	if(!X)return 0;
	if(n!=X->n)return 0;
	return *A==*X->A && *B==*X->B;
}


//--------------------------------------------------------------------------------------------------
Base3::Base3():Base2(){V=NULL;}

Base3::~Base3(){
	if(V)delete V;
}


string Base3::toString(){
	string s=A->toString();
	s+="?";
	s+=V->toString();
	s+=":";
	s+=B->toString();
	if(needForScobki(this))s="("+s+")";
	return s;
}


Algorithm* Base3::copy(){
	Base3*x=new(Base3);
	x->n=n;
	x->A=A->copy();
	x->B=B->copy();
	x->V=V->copy();
	return x;
}

bool Base3:: operator == (const Algorithm&t) const{
	const Base3*X=dynamic_cast<const Base3*>(&t);
	if(!X)return 0;
	if(n!=X->n)return 0;
	return *A==*X->A && *B==*X->B && *V==*X->V;
}

//--------------------------------------------------------------------------------------------------
Interval::Interval(){A=B=NULL;}

Interval::~Interval(){
	if(A)delete A;
	if(B)delete B;
}


string Interval::toString(){
	string s=a?"[":"(";
	s+=A->toString();
	s+=";";
	s+=B->toString();
	s+=b?"]":")";
	if(dynamic_cast<Cast*>(up))s = "("+s+")";
	return s;
}


Algorithm* Interval::copy(){
	Interval*x=new(Interval);
	x->a=a;
	x->b=b;
	x->A=A->copy();
	x->B=B->copy();
	return x;
}


bool Interval:: operator == (const Algorithm&t) const{
	const Interval*Y=dynamic_cast<const Interval*>(&t);
	if(!Y)return 0;
	if(a!=Y->a || b!=Y->b)return 0;
	if(!(*A==*Y->A))return 0;
	return *B==*Y->B;
}


//--------------------------------------------------------------------------------------------------
AlgoSet::~AlgoSet(){
	int i;
	for(i=0;i<nabor.size();++i)delete nabor[i];
}


string AlgoSet::toString(){
	string s,sub;
	int i;
	for(i=0;i<nabor.size();++i){
		if(i)sub+=",";
		if(sub.size()>=50){
			s+=sub+"\n";
			sub="";
			}
		sub+=nabor[i]->toString();
		}
	if(s.empty())s="{"+sub+"}"; else {
		s+=sub+"\n";
		s="{\n"+s+"}";
		SCANER::poTabu(s);
		}
	return s;
}


Algorithm* AlgoSet::copy(){
	AlgoSet*x=new(AlgoSet);
	int i;
	for(i=0;i<nabor.size();++i)
		x->nabor.push_back(nabor[i]->copy());
	return x;
}


bool AlgoSet:: operator == (const Algorithm&t) const{
	const AlgoSet*Y=dynamic_cast<const AlgoSet*>(&t);
	if(!Y)return 0;
	if(nabor.size()!=Y->nabor.size())return 0;
	int i;
	for(i=0;i<nabor.size();++i)if(!(*nabor[i]==*Y->nabor[i]))return 0;
	return 1;
}


//--------------------------------------------------------------------------------------------------
AComposition::~AComposition(){
	M_SA::iterator it=table.begin();
	for(;it!=table.end();++it)
		if(it->second)delete it->second;
}


string AComposition::toString(){
	string s;
	M_SA::iterator it=table.begin();
	for(bool first=0;it!=table.end();++it){
		if(first)s+=",";
		first=1;
		s+="\""+it->first+"\"";
		s+=":";
		s+=it->second->toString();
		}
	s="{"+s+"}";
	return s;
}


Algorithm* AComposition::copy(){
	AComposition*x=new(AComposition);
	M_SA::iterator it=table.begin();
	for(;it!=table.end();++it)
		x->table[it->first]=it->second->copy();
	return x;
}


bool AComposition:: operator == (const Algorithm&t) const{
	const AComposition*Y=dynamic_cast<const AComposition*>(&t);
	if(!Y)return 0;
	if(table.size()!=Y->table.size())return 0;
	M_SA::const_iterator it=table.begin();
	M_SA::const_iterator jt=Y->table.begin();
	for(;it!=table.end();++it,++jt){
		if(it->first!=jt->first)return 0;
		if(!(*it->second==*jt->second))return 0;
		}
	return 1;
}


//--------------------------------------------------------------------------------------------------
string Label::toString(){
	string s="<label:";
	s+=SCANER::toString(id);
	s+=">";
	return s;
}


Algorithm* Label::copy(){
	Label*x=new(Label);
	x->id=id;
	return x;
}


bool Label:: operator == (const Algorithm&t) const{
	const Label*Y=dynamic_cast<const Label*>(&t);
	if(!Y)return 0;
	return id==Y->id;
}


//--------------------------------------------------------------------------------------------------
Sequence::~Sequence(){
	L_AL::iterator it=nabor.begin();
	for(;it!=nabor.end();++it)if(*it)delete *it;
}


string Sequence::toString(){
	if(nabor.empty())return "{}";
	string s="{\n";
	L_AL::iterator it=nabor.begin();
	for(;it!=nabor.end();++it){
		string u=(*it)->toString();
		if(u.empty())continue;
		s+="	";
		SCANER::poTabu(u);
		s+=u;
		if(dynamic_cast<Sequence*>(*it)){s+="\n";continue;}
		if(dynamic_cast<IF*>(*it)){s+="\n";continue;}
		if(dynamic_cast<WHILE*>(*it) ||	dynamic_cast<FOR*>(*it)){s+="\n";continue;}
		if(dynamic_cast<Label*>(*it)){s+="\n";continue;}
		s+=";\n";
		}
	s+="	}";
	return s;
}


Algorithm* Sequence::copy(){
	Sequence*x=new(Sequence);
	L_AL::iterator it=nabor.begin();
	for(;it!=nabor.end();++it)
		x->nabor.push_back((*it)->copy());
	return x;
}


bool Sequence:: operator == (const Algorithm&t) const{
	const Sequence*Y=dynamic_cast<const Sequence*>(&t);
	if(!Y)return 0;
	if(nabor.size()!=Y->nabor.size())return 0;
	L_AL::const_iterator it=nabor.begin();
	L_AL::const_iterator jt=Y->nabor.begin();
	for(;it!=nabor.end();++it,++jt)if(!(**it==**jt))return 0;
	return 1;
}


//--------------------------------------------------------------------------------------------------
CreateVar::CreateVar(){Init=NULL;isMassiv=isInit=0;}

CreateVar::~CreateVar(){
	if(Init)delete Init;
	int i;
	for(i=0;i<params.size();++i)
		if(params[i])delete params[i];
}


string CreateVar::toString(){
	string s=tip.toString();
	if(s.empty())return s;//зарезервировано для инициализации свойств обекта
	s+=" ";
	s+=toString2();
	return s;
}



string CreateVar::toString2(){
	string s=name;
	if(isMassiv){
		int i;
		for(i=0;i<params.size();++i){
			s+="[";
			if(params[i])s+=params[i]->toString();
			s+="]";
			}
		if(Init){
			s+="=";
			s+=Init->toString();
			}
		return s;
		}
	if(isInit)
	if(Init){
		s+="=";
		s+=Init->toString();
		}else{
		s+="(";
		int i;
		for(i=0;i<params.size();++i){
			if(i)s+=",";
			s+=params[i]->toString();
			}
		s+=")";
		}
	return s;
}



Algorithm* CreateVar::copy(){
	CreateVar*x=new(CreateVar);
	x->isInit=isInit;
	x->isMassiv=isMassiv;
	x->name=name;
	if(Init)x->Init=Init->copy();
	x->tip=tip;
	int i;
	for(i=0;i<params.size();++i)
		x->params.push_back(params[i]?params[i]->copy():NULL);
	return x;
}



bool CreateVar:: operator == (const Algorithm&t) const{
	const CreateVar*Y=dynamic_cast<const CreateVar*>(&t);
	if(!Y)return 0;
	if(isInit!=Y->isInit || isMassiv!=Y->isMassiv)return 0;
	if(!(tip==Y->tip))return 0;
	if(name!=Y->name)return 0;
	if(Init&&Y->Init)if(!(*Init==*Y->Init))return 0;
	if(!Init&&Y->Init || Init&&!Y->Init)return 0;
	if(params.size()!=Y->params.size())return 0;
	int i;
	for(i=0;i<params.size();++i)if(!(*params[i]==*Y->params[i]))return 0;
	return 1;
}



//--------------------------------------------------------------------------------------------------
char* SpecSumbol::m[]={
	"return",
	"delete",
	"goto",
	"continue",
	"break",
	"PAGECLOSE",
	"PAGEOPEN",
	"OFF",
	NULL
};


SpecSumbol::~SpecSumbol(){}


string SpecSumbol::toString(){
	return m[n];
}


Algorithm* SpecSumbol::copy(){
	SpecSumbol*x=new(SpecSumbol);
	x->n=n;
	return x;
}


bool SpecSumbol:: operator == (const Algorithm&t) const{
	const SpecSumbol*Y=dynamic_cast<const SpecSumbol*>(&t);
	if(!Y)return 0;
	return n==Y->n;
}


//--------------------------------------------------------------------------------------------------
SpecSumbol2::SpecSumbol2(){X=NULL;}

SpecSumbol2::~SpecSumbol2(){
	if(X)delete X;
}


string SpecSumbol2::toString(){
	string s=m[n];
	s+=" ";
	s+=X->toString();
	return s;
}


Algorithm* SpecSumbol2::copy(){
	SpecSumbol2*x=new(SpecSumbol2);
	x->n=n;
	x->X=X->copy();
	return x;
}


bool SpecSumbol2:: operator == (const Algorithm&t) const{
	const SpecSumbol2*Y=dynamic_cast<const SpecSumbol2*>(&t);
	if(!Y)return 0;
	if(!(*X==*Y->X))return 0;
	return n==Y->n;
}


//--------------------------------------------------------------------------------------------------
IF::IF(){X=A=E=NULL;}

IF::~IF(){
	if(X)delete X;
	if(A)delete A;
	if(E)delete E;
}


string IF::toString(){
	string s="if(";
	s+=X->toString();
	s+=")";
	if(A){
		string u;
		u=A->toString();
		IF* ii=dynamic_cast<IF*>(A);
		WHILE*W=dynamic_cast<WHILE*>(A);
		if(!dynamic_cast<Sequence*>(A) && !ii && !W)if(u[u.size()-1]!=';')u+=";";
		if(!ii){
			Algorithm*H=NULL;
			WHILE*W=dynamic_cast<WHILE*>(A);
			while(W)W=dynamic_cast<WHILE*>(H=W->A);
			ii=dynamic_cast<IF*>(H);
			}
		if(ii)u="{"+u+"}";
		s+=u;
		}else s+=";";
	if(E){
		s+="else ";
		string u;
		u=E->toString();
		IF* ii=dynamic_cast<IF*>(E);
		if(!dynamic_cast<Sequence*>(E) && !ii)if(u[u.size()-1]!=';')u+=";";
		if(ii)u="{"+u+"}";
		s+=u;
		}
	return s;
}


Algorithm* IF::copy(){
	IF*x=new(IF);
	x->X=X->copy();
	x->A=x->E=NULL;
	if(A)x->A=A->copy();
	if(E)x->E=E->copy();
	return x;
}


bool IF:: operator == (const Algorithm&t) const{
	const IF*Y=dynamic_cast<const IF*>(&t);
	if(!Y)return 0;
	if(A && Y->A)if(!(*A==*Y->A))return 0;
	if(!A && Y->A || A && !Y->A)return 0;
	if(E && Y->E)if(!(*E==*Y->E))return 0;
	if(!E && Y->E || E && !Y->E)return 0;
	return *X==*Y->X;
}


//--------------------------------------------------------------------------------------------------
WHILE::WHILE(){X=A=NULL;n=0;}

WHILE::~WHILE(){
	if(X)delete X;
	if(A)delete A;
}


string WHILE::toString(){
	string s;
	if(!n){ // repeat
		s="repeat";
		if(X)s+="("+X->toString()+")";
		if(A)s+=A->toString();
		if(!dynamic_cast<Sequence*>(A))s+=";";
		return s;
		}
	if(n==1){ // while
		s="while(";
		s+=X->toString();
		s+=")";
		if(A)s+=A->toString();
		if(!dynamic_cast<Sequence*>(A))if(s[s.size()-1]!=';')s+=";";
		return s;
		}
	// do
	s="do";
	if(A){
		Sequence*seq=dynamic_cast<Sequence*>(A);
		if(!seq)s+=" ";
		string g=A->toString();
		if(!seq)if(g[g.size()-1]!=';')g+=";";
		if(!seq)g="{"+g+"}";
		s+=g;
		}else s+=";";
	s+="while(";
	s+=X->toString();
	s+=");";
	return s;
}


Algorithm* WHILE::copy(){
	WHILE*x=new(WHILE);
	x->n=n;
	x->A=A->copy();
	x->X=X->copy();
	return x;
}


bool WHILE:: operator == (const Algorithm&t) const{
	const WHILE*Y=dynamic_cast<const WHILE*>(&t);
	if(!Y)return 0;
	if(A && Y->A)if(!(*A==*Y->A))return 0;
	if(!A && Y->A || A && !Y->A)return 0;
	return *X==*Y->X;
}



//--------------------------------------------------------------------------------------------------
FOR::FOR(){X=B=NULL;P=NULL;}

FOR::~FOR(){
	if(P)delete P;
	if(X)delete X;
	if(B)delete B;
}


string FOR::toString(){
	string s="for(;";
	if(X)s+=X->toString();
	s+=";";
	if(P){
		Sequence*S=dynamic_cast<Sequence*>(P);
		if(S){
			L_AL::iterator it=S->nabor.begin();
			for(bool nfi=0;it!=S->nabor.end();++it){
				if(nfi)s+=",";
				nfi=1;
				s+=(*it)->toString();
				}
			}else s+=P->toString();
		}
	s+=")";
	if(B)s+=B->toString();
	if(!dynamic_cast<Sequence*>(B))if(s[s.size()-1]!=';')s+=";";
	return s;
}


Algorithm* FOR::copy(){
	FOR*x=new(FOR);
	if(X)x->X=X->copy();
	if(B)x->B=B->copy();
	if(P)x->P=P->copy();
	return x;
}


bool FOR:: operator == (const Algorithm&t) const{
	const FOR*Y=dynamic_cast<const FOR*>(&t);
	if(!Y)return 0;
	if(B && Y->B)if(!(*B==*Y->B))return 0;
	if(!B && Y->B || B && !Y->B)return 0;
	if(P && Y->P)if(!(*P==*Y->P))return 0;
	if(!P && Y->P || P && !Y->P)return 0;
	return *X==*Y->X;
}





//--------------------------------------------------------------------------------------------------
Assemble::Assemble(string&P,MAIN*M,const char*f):PHTML(P),F(f){
	Main=M;
	GlobalSpace=&M->GlobalSpace;
	//cout<<F.text<<"."<<endl;
}

Assemble::~Assemble(){}


Algorithm* Assemble::getComand(MAIN*M,string s){
	Algorithm*A=NULL;
	string error;
	int er=0;
	s="{"+s+";}";
	Sequence*S=NULL;
		{
		Assemble A(error,M,"");
		char*c=const_cast<char*>(s.c_str());
		S=A.getNabor(c,er);
		}
	if(er){
		if(S)delete S;
		}else{
		if(S->nabor.size()==1)A=(*S->nabor.begin())->copy();
		delete S;
		}
	return A;
}


void Assemble::Readterms(char*&s,L_OT&lot){
	V_I vskobkax;
	int pos=-1;
	string lifo;
	oneterm t;
	bool ok;
	do{
		SCANER::noProbel(s);
		t=oneterm::getTerm(s,this);
		ok=!t.term.empty();
		if(ok){
			if(t.term=="(" || t.term=="[" || t.term=="{"){
				lifo+=t.term;
				vskobkax.push_back(0);
				++pos;
				}
			if(t.term==")" || t.term=="]" || t.term=="}"){
				if(pos<0){
					//if(!lot.empty())lot.erase(lot.rend());
					--s;
					return;
					}
				lifo=lifo.substr(0,pos);
				--pos;
				vskobkax.pop_back();
				}
			lot.push_back(t);
			if(t.term==";"){
				if(pos<0)ok=0; else	if(++vskobkax[pos]>1){
					if(lifo[pos]!='{')ok=0;
					}
				}
			if(t.term==",")if(pos<0)ok=0;
			}
		}while(ok);
	if(t.term.empty()){
		bool isOK = Main->GoErrorMessage(
			PHTML,
			"UnknownOperator",
			"",
			SCANER::toString(SCANER::findNumberStringLine(F.text,s))
			);
		if(!isOK){
			PHTML+="<font class='red'>Неизвестный оператор или неожиданный конец файла в строке: ";
			PHTML+=SCANER::toString(SCANER::findNumberStringLine(F.text,s));
			PHTML+="</font><br/>\n";
			}
		lot.clear();
		}
}


void Assemble::outLOT(L_OT&lot){
	L_OT::iterator it=lot.begin();
	for(;it!=lot.end();++it)
		cout<<it->term.c_str()<<" ";
	cout<<endl;
}


//проверить сочитание скобок
bool Assemble::isCorectSkobki(L_OT&lot){
	string lifo;
	int pos=0,er=0;
	char *final;
	L_OT::iterator it=lot.begin();
	for(;it!=lot.end();++it)if(it->nfunc==4){
		char c=it->term[0];
		final=it->c;
		if(c=='{' || c=='(' || c=='['){
			lifo+=c;
			++pos;
			}
		if(c=='}'){
			--pos;
			if(pos<0){
				er=1;
				break;
				}
			c=lifo[pos];
			if(c!='{'){
				er=2;
				break;
				}
			lifo=lifo.substr(0,pos);
			continue;
			}
		if(c==')' || c==']'){
			--pos;
			if(pos<0){
				er=1;
				break;
				}
			c=lifo[pos];
			if(c!='(' && c!='['){
				er=2;
				break;
				}
			lifo=lifo.substr(0,pos);
			}
		}
	if(er==1){
		bool isOK = Main->GoErrorMessage(
			PHTML,
			"Bracket",
			it->term.c_str(),
			SCANER::toString(SCANER::findNumberStringLine(F.text,it->c))
			);
		if(!isOK){
			PHTML+="<font class='red'>Лишняя скобка '";
			PHTML+=it->term+"' в строке: ";
			PHTML+=SCANER::toString(SCANER::findNumberStringLine(F.text,it->c));
			PHTML+="</font><br/>\n";
			}
		}
	if(er==2){
		bool isOK = Main->GoErrorMessage(
			PHTML,
			"CloseBracket",
			it->term.c_str(),
			SCANER::toString(SCANER::findNumberStringLine(F.text,it->c))
			);
		if(!isOK){
			PHTML+="<font class='red'>Закрытая скобка '";
			PHTML+=it->term.c_str();
			PHTML+="' не соответствует открытой в строке: ";
			PHTML+=SCANER::toString(SCANER::findNumberStringLine(F.text,it->c));
			PHTML+="</font><br/>\n";
			}
		}
	if(lifo.size()){
		bool isOK = Main->GoErrorMessage(
			PHTML,
			"NeedCloseBracket",
			lifo.c_str(),
			SCANER::toString(SCANER::findNumberStringLine(F.text,final))
			);
		if(!isOK){
			PHTML+="<font class='red'>Нехватает закрытых скобок к открытым \"";
			PHTML+=lifo+"\" в строке: "+
				SCANER::toString(SCANER::findNumberStringLine(F.text,final))+"</font><br/>\n";
			}
		er=3;
		}
	return !er;
}




void Assemble::adaptationString(L_OT&lot){
	L_OT::iterator it=lot.begin(),jt;
	for(;it!=lot.end();++it){
		if(it->nfunc==2)it->term=SCANER::convertChar(it->term);
		if(it->nfunc==1)it->term=SCANER::convertString(it->term);
		}
	int blin=0;
	it=lot.begin();
	for(;it!=lot.end();++it)if(it->nfunc==1){
		if(blin){blin=0;--it;}
		jt=it;
		++jt;
		if(jt==lot.end())continue;
		if(jt->nfunc==1){
			it->term+=jt->term;
			lot.erase(jt);
			blin=1;
			}
		}
}



Algorithm* Assemble::Razbor(char*&s,int n){//1-; 2-,; 0-(_,) 3-as 4-[]
	L_OT lot;
	char*s2=s;
	Readterms(s,lot);
	if(lot.empty()){
		s=s2;
		return NULL;
		}
	//outLOT(lot);
	adaptationString(lot);
	if(!isCorectSkobki(lot))return NULL;
	L_OT::iterator pos=lot.begin();
	Algorithm*A=gather(lot,pos);
	bool ok=0;
	if(n){
		if(n==4)ok=1;
		if(n==3)if(pos->term=="as"){
			ok=1;
			s=pos->c;
			pos=lot.end();
			}
		if(n==2){
			if(pos->term==","){++pos;ok=1;}
				else if(pos->term==";"){++pos;ok=1;}
			}
		if(n==1)if(pos!=lot.end())if(pos->term==";"){++pos;ok=1;}
		}else{
		if(pos!=lot.end())if(pos->term==","){++pos;n=5;}
		ok=1;
		}
	if(pos!=lot.end() || !ok){
		delete A;
		A=NULL;
		/*
		PHTML+="<font class='red'>";
		PHTML+="Ошибка в выражении в строке: ";
		PHTML+=SCANER::toString(SCANER::findNumberStringLine(F.text,ok?pos->c:s2));
		PHTML+="</font><br/>\n";
		*/
		}
	if(A){
		//cout<<A->toString().c_str()<<endl;
		if(n && n<3){
			s=lot.rbegin()->c;
			++s;
			}
		}
	return A;
}



//<label:12>
Label* Assemble::getLabel(char*&s){
	SCANER::noProbel(s);
	if(*s!='<')return NULL;
	++s;
	SCANER::noProbel(s);
	int n=SCANER::scanSlovo("label",s);
	SCANER::noProbel(s);
	if(*s==':'){++s;++n;}
	SCANER::noProbel(s);
	string t=SCANER::readDigit(s);
	SCANER::noProbel(s);
	if(*s=='>'){++s;++n;}
	if(n<3){
		bool isOK = Main->GoErrorMessage(
			PHTML,
			"LabelError",
			"",
			SCANER::toString(SCANER::findNumberStringLine(F.text,s))
			);
		if(!isOK){
			PHTML+="<font class='red'>";
			PHTML+="Нарушен синтаксис метки в строке: ";
			PHTML+=SCANER::toString(SCANER::findNumberStringLine(F.text,s));
			PHTML+=SCANER::replaceSpecSumbolHTML(". Пример: <label:10>");
			PHTML+="</font><br/>\n";
			}
		return NULL;
		}
	Label*X=new(Label);
	sscanf(t.c_str(),"%d",&X->id);
	return X;
}




void Assemble::probaVarReadSub(char*&s,Type&tip,string&str){
	while(*s=='*'){
		++tip.n;
		++s;
		SCANER::noProbel(s);
		}
	if(*s=='&'){
		tip.isAmpersent=1;
		++s;
		SCANER::noProbel(s);
		}
	str=SCANER::readName(s);
	SCANER::noProbel(s);
}



//const nfls k(1,1),*t;
//int m[][15]={{1},{0}};
int Assemble::probaVarRead(char*&s,L_AL&nabor){
	char*s2=s;
	string str;
	V_S S;
	Type tip;
	do{
		SCANER::noProbel(s);
		if(SCANER::scanSlovo("::",s)){
			str="::";
			S.push_back(str);
			continue;
			}
		str=SCANER::readName(s);
		if(str=="new" || str=="return" || str=="delete"){
			s=s2;
			return 0;
			}
		if(str=="const"){
			tip.isConst=1;
			continue;
			}
		if(!str.empty())S.push_back(str);
		}while(!str.empty());
	SCANER::noProbel(s);
	int t=0,n=S.size();
	if(*s=='*' || *s=='&')if(n)t=1;
	if(*s=='=' || *s=='[' || *s=='(' || *s==';'|| *s==',')if(n>1)t=2;
	if(!t){
		s=s2;
		return 0;
		}
	if(t==2)--n;
	if(S[n-1]=="::"){
		s=s2;
		return 0;
		}
	int i;
	for(i=0;i<n;++i){
		if(i)tip.name+=" ";
		tip.name+=S[i];
		}
	if(!CVARIANT::getN(tip.name.c_str()))
		if(tip.name!="var"){s=s2;return 0;}
	if(t==1)probaVarReadSub(s,tip,str);
	if(t==2)str=S[n];
	char c=0;
	CreateVar*cvar;
	int er=0,narg;
	if(str.empty()){
		//if(*s=='=' || *s=='(' || *s=='&'){
			s=s2;
			return 0;
		//	}
		//er=1;
		}
	//if(!er)
	while(1){
		cvar=new(CreateVar);
		cvar->name=str;
		cvar->tip=tip;
		while(*s=='['){
			cvar->isMassiv=1;
			++s;
			Algorithm*X=Razbor(s,4);
			SCANER::noProbel(s);
			if(*s==']')++s;
			cvar->params.push_back(X);
			SCANER::noProbel(s);
			}
		if(*s=='='){
			char*ss=s;
			++s;
			cvar->isInit=1;
			cvar->Init=Razbor(s,2);
			if(!cvar->Init){
				s=ss;
				er=2;
				break;
				}
			c=*(s-1);
			if(c==';')break;
			}
		if(*s=='('){
			++s;
			cvar->isInit=1;
			Algorithm*X;
			SCANER::noProbel(s);
			narg=0;
			while(*s!=')'){
				++narg;
				X=Razbor(s,0);
				if(!X){
					er=3;
					break;
					}
				cvar->params.push_back(X);
				SCANER::noProbel(s);
				}
			if(er)break;
			++s;
			SCANER::noProbel(s);
			}
		if(*s==',' || c==','){
			if(*s==',')++s;
			c=0;
			nabor.push_back(cvar);
			SCANER::noProbel(s);
			tip.n=0;
			tip.isAmpersent=0;
			probaVarReadSub(s,tip,str);
			if(str.empty()){
				er=1;
				break;
				}
			continue;
			}
		if(*s==';'){
			++s;
			break;
			}
		er=4;
		break;
		}
	if(er){
		const char* EE[] = {"E1","E2","E3","E4"};
		bool isOK = Main->GoErrorMessage(
			PHTML,
			EE[er-1],
			SCANER::toString(narg),
			SCANER::toString(SCANER::findNumberStringLine(F.text,s))
			);
		if(!isOK){
			//if(er==4){s=s2;return 0;}
			PHTML+="<font class='red'>";
			if(er==1)PHTML+="Нет имени переменной после описания типа ее в строке: ";
			if(er==2)PHTML+="Нет инициализации переменной после '=' в строке: ";
			if(er==3){PHTML+="Нет аргумента ";PHTML+=SCANER::toString(narg);PHTML+=" в конструкторе, в строке: ";}
			if(er==4)PHTML+="Нужен разделитель ',' в описании переменных в строке: ";
			PHTML+=SCANER::toString(SCANER::findNumberStringLine(F.text,s));
			PHTML+="</font><br/>\n";
			}
		return 2;
		}
	nabor.push_back(cvar);
	return 1;
}




SpecSumbol* Assemble::getSpecSumbol(char*&s,int&er){
	char*s2=s;
	string slovo=SCANER::readName(s);
	int i;
	for(i=0;SpecSumbol::m[i];++i){
		//s=s2;if(SCANER::scanSlovo(SpecSumbol::m[i],s))break;
		if(slovo==SpecSumbol::m[i])break;
		}
	if(!SpecSumbol::m[i]){s=s2;return NULL;}
	SCANER::noProbel(s);
	if(*s==';'){
		if(i==1){
			bool isOK = Main->GoErrorMessage(
				PHTML,
				"NeedArgDel",
				"",
				SCANER::toString(SCANER::findNumberStringLine(F.text,s))
				);
			if(!isOK){
				PHTML+="<font class='red'>";
				PHTML+="Нужен аргумент после 'delete' в строке: ";
				PHTML+=SCANER::toString(SCANER::findNumberStringLine(F.text,s));
				PHTML+="</font><br/>\n";
				}
			s=s2;
			er=10;
			return NULL;
			}
		++s;
		SpecSumbol*S=new(SpecSumbol);
		S->n=i;
		return S;
		}
	if(i>5){
		bool isOK = Main->GoErrorMessage(
			PHTML,
			"NeedPsetko",
			SpecSumbol::m[i],
			SCANER::toString(SCANER::findNumberStringLine(F.text,s))
			);
		if(!isOK){
			PHTML+="<font class='red'>";
			PHTML+="Отсутствует ';' после '";PHTML+=SpecSumbol::m[i];PHTML+="' в строке: ";
			PHTML+=SCANER::toString(SCANER::findNumberStringLine(F.text,s));
			PHTML+="</font><br/>\n";
			}
		return NULL;
		}
	Algorithm*X=Razbor(s,1);
	if(!X){
		bool isOK = Main->GoErrorMessage(
			PHTML,
			"NeedArg",
			SpecSumbol::m[i],
			SCANER::toString(SCANER::findNumberStringLine(F.text,s))
			);
		if(!isOK){
			PHTML+="<font class='red'>";
			PHTML+="Нужен аргумент после '";PHTML+=SpecSumbol::m[i];PHTML+="' в строке: ";
			PHTML+=SCANER::toString(SCANER::findNumberStringLine(F.text,s));
			PHTML+="</font><br/>\n";
			}
		s=s2;
		er=10;
		return NULL;
		}
	if(i>2){// X mast be number
		Base*B=dynamic_cast<Base*>(X);
		if(B)if(B->type==3){if(!int(SCANER::getNumber(B->text,10)))B=NULL;}else B=NULL;
		if(!B){
			delete X;
			bool isOK = Main->GoErrorMessage(
				PHTML,
				"MastBeNumber",
				SpecSumbol::m[i],
				SCANER::toString(SCANER::findNumberStringLine(F.text,s))
				);
			if(!isOK){
				PHTML+="<font class='red'>Spec sumbil <b>";
				PHTML+=SpecSumbol::m[i];
				PHTML+="</b> mast be const int, no zero, value in string: ";
				PHTML+=SCANER::toString(SCANER::findNumberStringLine(F.text,s));
				PHTML+="</font><br/>\n";
				}
			s=s2;
			er=10;
			return NULL;
			}
		}
	SpecSumbol2*S=new(SpecSumbol2);
	S->n=i;
	S->X=X;
	return S;
}




IF* Assemble::getIF(char*&s,int&er){ // if
	char*s2=s;
	if(!SCANER::scanSlovo("if",s))return NULL;
	SCANER::noProbel(s);
	if(*s!='('){
		s=s2;
		return NULL;
		}
	++s;
	IF*iif=new(IF);
	iif->X=Razbor(s,0);
	if(*s!=')'){
		delete iif;
		bool isOK = Main->GoErrorMessage(
			PHTML,
			"NeedBracketInIF",
			"",
			SCANER::toString(SCANER::findNumberStringLine(F.text,s))
			);
		if(!isOK){
			PHTML+="<font class='red'>";
			PHTML+="Требуетса скобка ')' после условия в блоке if() в строке: ";
			PHTML+=SCANER::toString(SCANER::findNumberStringLine(F.text,s));
			PHTML+="</font><br/>\n";
			}
		er=1;
		return NULL;
		}
	++s;
	if(!iif->X){
		delete iif;
		bool isOK = Main->GoErrorMessage(
			PHTML,
			"NeedCondition",
			"",
			SCANER::toString(SCANER::findNumberStringLine(F.text,s2))
			);
		if(!isOK){
			PHTML+="<font class='red'>";
			PHTML+="Требуетса условие в блоке if() в строке: ";
			PHTML+=SCANER::toString(SCANER::findNumberStringLine(F.text,s2));
			PHTML+="</font><br/>\n";
			}
		er=1;
		return NULL;
		}
	SCANER::noProbel(s);
	if(*s!=';'){
		iif->A=getONE(s,er);
		/*
		if(!iif->A){
			delete iif;
			er=1;
			return NULL;
			}
		*/
		}else ++s;
	SCANER::noProbel(s);
	if(SCANER::scanSlovo("else",s)){
		SCANER::noProbel(s);
		if(*s!=';'){
			iif->E=getONE(s,er);
			/*
			if(!iif->E){
				delete iif;
				er=1;
				return NULL;
				}
			*/
			}else ++s;
		}
	return iif;
}




WHILE* Assemble::getWHILE(char*&s,int&er){
	WHILE*W=NULL;
	if(SCANER::scanSlovo("repeat",s)){
		SCANER::noProbel(s);
		W=new(WHILE);
		if(*s=='('){
			++s;
			W->X=Razbor(s,0);
			SCANER::noProbel(s);
			if(*s!=')'){
				bool isOK = Main->GoErrorMessage(
					PHTML,
					"NeedInRepeat",
					"",
					SCANER::toString(SCANER::findNumberStringLine(F.text,s))
					);
				if(!isOK){
					PHTML+="<font class='red'>";
					PHTML+="Требуетса скобка ')' в блоке 'repeat()', в строке: ";
					PHTML+=SCANER::toString(SCANER::findNumberStringLine(F.text,s));
					PHTML+="</font><br/>\n";
					}
				er=1;
				return W;
				}
			++s;
			SCANER::noProbel(s);
			}
		if(*s==';')return W;
		W->A=getNabor(s,er);
		if(!W->A)if(*s!=';'){
			er=1;
			bool isOK = Main->GoErrorMessage(
				PHTML,
				"NeedPsetkoR",
				"",
				SCANER::toString(SCANER::findNumberStringLine(F.text,s))
				);
			if(!isOK){
				PHTML+="<font class='red'>";
				PHTML+="Требуетса ';' после блока 'repeat', в строке: ";
				PHTML+=SCANER::toString(SCANER::findNumberStringLine(F.text,s));
				PHTML+="</font><br/>\n";
				}
			}
		return W;
		}
	if(SCANER::scanSlovo("while",s)){
		if(*s!=' ' && *s!='	' && *s!='(' && *s!='\n'){s-=5;return W;}
		SCANER::noProbel(s);
		if(*s!='('){
			er=1;
			bool isOK = Main->GoErrorMessage(
				PHTML,
				"NeedBracketW",
				"",
				SCANER::toString(SCANER::findNumberStringLine(F.text,s))
				);
			if(!isOK){
				PHTML+="<font class='red'>";
				PHTML+="Требуетса '(' после блоке 'while', в строке: ";
				PHTML+=SCANER::toString(SCANER::findNumberStringLine(F.text,s));
				PHTML+="</font><br/>\n";
				}
			return NULL;
			}
		++s;
		W=new(WHILE);
		W->n=1;
		W->X=Razbor(s,0);
		SCANER::noProbel(s);
		if(*s!=')')er=1;
		if(!W->X)er=2;
		if(er){
			bool isOK = Main->GoErrorMessage(
				PHTML,
				((er==1)?"NeedCBracketW":"NeedConditionW"),
				"",
				SCANER::toString(SCANER::findNumberStringLine(F.text,s))
				);
			if(!isOK){
				PHTML+="<font class='red'>";
				if(er==1)PHTML+="Требуетса ')' в блоке 'while()', в строке: ";
				if(er==2)PHTML+="Требуетса условие в блоке 'while(?)', в строке: ";
				PHTML+=SCANER::toString(SCANER::findNumberStringLine(F.text,s));
				PHTML+="</font><br/>\n";
				}
			return W;
			}
		++s;
		SCANER::noProbel(s);
		if(*s==';')return W;
		W->A=getONE(s,er);
		return W;
		}
	if(SCANER::scanSlovo("do",s)){
		if(*s!=' ' && *s!='	' && *s!='{' && *s!='\n'){s-=2;return W;}
		char*s2=s;
		SCANER::noProbel(s);
		W=new(WHILE);
		W->n=2;
		if(*s==';')++s; else if(s2<s || *s=='{'){
			W->A=getONE(s,er);
			//if(!W->A)er=1;
			}
		SCANER::noProbel(s);
		if(s2==s)er=1; else
		if(!SCANER::scanSlovo("while",s))er=2;
		SCANER::noProbel(s);
		if(*s!='(')if(!er)er=3;
		if(er){
			const char* mer[] = {"NE01","NW02","NB03"};
			bool isOK = Main->GoErrorMessage(
				PHTML,
				mer[er-1],
				"",
				SCANER::toString(SCANER::findNumberStringLine(F.text,s))
				);
			if(!isOK){
				PHTML+="<font class='red'>";
				if(er==1)PHTML+="Нужен отступ после 'do', в строке: ";
				if(er==2)PHTML+="Нужно слово 'while' в блоке 'do{}while()', в строке: ";
				if(er==3)PHTML+="Требуетса '(' в блоке 'do{}while()', в строке: ";
				PHTML+=SCANER::toString(SCANER::findNumberStringLine(F.text,s));
				PHTML+="</font><br/>\n";
				}
			return W;
			}
		++s;
		W->X=Razbor(s,0);
		SCANER::noProbel(s);
		if(*s!=')')er=1; else
		if(!W->X)er=2;
		++s;
		SCANER::noProbel(s);
		if(*s!=';')if(!er)er=3;
		if(er){
			const char* mer[] = {"NB04","NC05","NP0W"};
			bool isOK = Main->GoErrorMessage(
				PHTML,
				mer[er-1],
				"",
				SCANER::toString(SCANER::findNumberStringLine(F.text,s))
				);
			if(!isOK){
				PHTML+="<font class='red'>";
				if(er==1)PHTML+="Требуетса ')' в блоке 'do{}while()', в строке: ";
				if(er==2)PHTML+="Нужно условие в блоке 'do{}while(?)', в строке: ";
				if(er==3)PHTML+="Требуетса ';' после блока 'do{}while()', в строке: ";
				PHTML+=SCANER::toString(SCANER::findNumberStringLine(F.text,s));
				PHTML+="</font><br/>\n";
				}
			}
		++s;
		}
	return W;
}




Algorithm* Assemble::getFOR(char*&s,int&er,Sequence*S){
	if(!SCANER::scanSlovo("for",s))return NULL;
	if(*s!=' ' && *s!='	' && *s!='\n' && *s!='('){
		s-=3;
		return NULL;
		}
	SCANER::noProbel(s);
	if(*s!='('){
		er=1;
		bool isOK = Main->GoErrorMessage(
			PHTML,
			"NB06",
			"",
			SCANER::toString(SCANER::findNumberStringLine(F.text,s))
			);
		if(!isOK){
			PHTML+="<font class='red'>";
			PHTML+="Требуетса '(' после блока '";
			PHTML+="for";
			PHTML+="', в строке: ";
			PHTML+=SCANER::toString(SCANER::findNumberStringLine(F.text,s));
			PHTML+="</font><br/>\n";
			}
		return NULL;
		}
	++s;
	SCANER::noProbel(s);
	Sequence*S2=S;
	char*ss=s;
	SCANER::noProbel(s);
	Algorithm*X=NULL;
	if(SCANER::scanSlovo("in",s)){
		SCANER::noProbel(s);
		X=Razbor(s,0);
		}

	s=ss;
	SCANER::noProbel(s);
	if(*s!=';'){
		if(!S2)S2=new(Sequence);
		int t=probaVarRead(s,S2->nabor);
		if(!t){
			Algorithm*U=Razbor(s,2);
			if(U)S2->nabor.push_back(U);else er=1;
			while(*(s-1)==','){
				U=Razbor(s,2);
				if(U)S2->nabor.push_back(U);else er=1;
				}
			if(!er)t=1;
			}
		if(t!=1){
			er=1;
			bool isOK = Main->GoErrorMessage(
				PHTML,
				"EI07",
				"",
				SCANER::toString(SCANER::findNumberStringLine(F.text,s))
				);
			if(!isOK){
				PHTML+="<font class='red'>";
				PHTML+="Нарушен блок инициализации для 'for(<b>?</b>;;);', в строке: ";
				PHTML+=SCANER::toString(SCANER::findNumberStringLine(F.text,s));
				PHTML+="</font><br/>\n";
				}
			return NULL;
			}
		}else ++s;
	SCANER::noProbel(s);
	FOR*FF=new(FOR);
	if(*s!=';'){
		FF->X=Razbor(s,1);
		if(!FF->X){
			er=2;
			bool isOK = Main->GoErrorMessage(
				PHTML,
				"EC08",
				"",
				SCANER::toString(SCANER::findNumberStringLine(F.text,s))
				);
			if(!isOK){
				PHTML+="<font class='red'>";
				PHTML+="Нарушен блок условия для 'for(;<b>?</b>;);', в строке: ";
				PHTML+=SCANER::toString(SCANER::findNumberStringLine(F.text,s));
				PHTML+="</font><br/>\n";
				}
			delete FF;
			return NULL;
			}
		}else ++s;
	SCANER::noProbel(s);
	if(*s!=')'){
		Sequence*S3=new(Sequence);
		Algorithm*U=Razbor(s,0);
		if(!U)er=1;
		S3->nabor.push_back(U);
		while(*(s-1)==','){
			U=Razbor(s,0);
			if(!U)er=1;
			S3->nabor.push_back(U);
			}
		if(er){
			bool isOK = Main->GoErrorMessage(
				PHTML,
				"EL09",
				"",
				SCANER::toString(SCANER::findNumberStringLine(F.text,s))
				);
			if(!isOK){
				PHTML+="<font class='red'>";
				PHTML+="Нарушен список пост операций для 'for(;;<b>?</b>);'. ";
				PHTML+="Список должен разделятса комами или быть пустым. Проблема в строке: ";
				PHTML+=SCANER::toString(SCANER::findNumberStringLine(F.text,s));
				PHTML+="</font><br/>\n";
				}
			delete FF;
			delete S3;
			return NULL;
			}
		if(S3->nabor.size()==1){
			FF->P=*S3->nabor.begin();
			S3->nabor.clear();
			delete S3;
			}else FF->P=S3;
		}

	if(S2!=S)S2->nabor.push_back(FF);
		
	if(*s!=')'){
		er=1;
		bool isOK = Main->GoErrorMessage(
			PHTML,
			"NB10",
			"",
			SCANER::toString(SCANER::findNumberStringLine(F.text,s))
			);
		if(!isOK){
			PHTML+="<font class='red'>";
			PHTML+="Требуетса скобка ')' для 'for(;;<b>)</b>;' в строке: ";
			PHTML+=SCANER::toString(SCANER::findNumberStringLine(F.text,s));
			PHTML+="</font><br/>\n";
			}
		delete FF;
		return NULL;
		}
	++s;
	SCANER::noProbel(s);
	if(*s==';')return S2==S?(Algorithm*)FF:S2;
	FF->B=getONE(s,er);
	return S2==S?(Algorithm*)FF:S2;
}




Algorithm* Assemble::getONE(char*&s,int&er,Sequence*S){
	char*s2=s;
	Algorithm*X;
	Sequence*sub;
	sub=getNabor(s,er);
	if(er && er!=10){
		er=0;
		X=Razbor(s,1);
		if(!X){
			er=1;
			s=s2;
			return NULL;
			}
		return X;
		}
	if(sub){
		if(sub->nabor.empty()){
			delete sub;
			return new AlgoSet();
			}
		if(sub->nabor.size()==1){
			AlgoSet*AS=dynamic_cast<AlgoSet*>(*sub->nabor.begin());
			if(AS){
				AlgoSet*A = new AlgoSet();
				A->nabor.push_back(AS);
				sub->nabor.clear();
				delete sub;
				return A;
				}
			}
		return sub;
		}else s=s2;
	X=getLabel(s);
	if(X)return X; else s=s2;
	if(S){
		int t=probaVarRead(s,S->nabor);
		if(t==1)return NULL;
		if(t==2){er=1;return NULL;}
		}
	X=getSpecSumbol(s,er);
	if(er)return NULL;
	if(X)return X;
	X=getIF(s,er);
	if(er)return NULL;
	if(X)return X;
	X=getWHILE(s,er);
	if(X)return X;
	if(er)return NULL;
	X=getFOR(s,er,S);
	if(X)return X;
	if(er)return NULL;
	X=Razbor(s,1);
	if(!X){er=1;s=s2;}
	return X;
}




Sequence* Assemble::getNabor(char*&s,int&er){ //{}
	char*s2=s;
	SCANER::noProbel(s);
	if(*s!='{')return NULL;
	++s;
	SCANER::noProbel(s);
	Sequence*S=new(Sequence);
	Algorithm*X;
	while(1){
		SCANER::noProbel(s);
		if(!(*s!='}' && *s))break;
		if(*s==','){
			er=1;
			break;
			}
		if(*s==';'){
			++s;
			continue;
			}
		X=getONE(s,er,S);
		if(er)break;
		if(X)S->nabor.push_back(X);
		}
	if(er){
		s=s2;
		delete S;
		return NULL;
		}
	if(*s=='}')++s; else{
		delete S;
		S=NULL;
		bool isOK = Main->GoErrorMessage(
			PHTML,
			"EX11",
			"",
			SCANER::toString(SCANER::findNumberStringLine(F.text,s))
			);
		if(!isOK){
			PHTML+="<font class='red'>";
			PHTML+="Неправильно организованы операторные скобки. Проблема в строке: ";
			PHTML+=SCANER::toString(SCANER::findNumberStringLine(F.text,s));
			PHTML+="</font><br/>\n";
			}
		er=1;
		}
	return S;
}




Algorithm* Assemble::gather(L_OT&lot,L_OT::iterator&pos){
	//1 префикс 1
	//1 строка  2
	//2 суфикс  2
	//2 оператор 1
	Algorithm*S=NULL;//указатель на скобки
	Algorithm*R=NULL,**X;
	X=&R;
	int n=1,i;
	bool krugli;

	for(;pos!=lot.end();++pos){
		if(n==1){
			if(Prefix::isPrefix(pos->term,i))if(pos->nfunc==4 || (pos->term=="new" && pos->nfunc==0)){
				Prefix*P=new(Prefix);
				P->n=i;
				*X=P;
				X=&P->X;
				continue;
				}
			if(pos->nfunc==6){
				*X=pos->A;
				pos->A=NULL;
				X=NULL;
				n=2;
				continue;
				}
			if(pos->nfunc==0)if(pos->term=="dynamic_cast"){
				++pos;
				int er=0;
				if(pos->term!="<")er=1;
				++pos;
				Type*x=Type::getType(pos);
				if(!x)if(!er)er=5;
				if(pos->term!=">"){if(!er)er=2;}else ++pos;
				if(pos->term!="("){if(!er)er=3;}else ++pos;
				Algorithm*A=NULL;
				if(!er){
					A=Assemble::gather(lot,pos);
					if(!A)er=6;
					}
				if(pos->term!=")")if(!er)er=4;
				if(er){
					const char*m[]={"<",">","(",")"};
					const char*pm = "";
					if(er<=4)pm = m[er-1];
					bool isOK = Main->GoErrorMessage(
						PHTML,
						((er<=4)?"NB12":((er==5)?"ET13":"NA13")),
						pm,
						SCANER::toString(SCANER::findNumberStringLine(F.text,pos->c))
						);
					if(!isOK){
						PHTML+="<font class='red'>";
						if(er<=4){
							PHTML+="Требуетса скобка '";
							PHTML+=SCANER::replaceSpecSumbolHTML(m[er-1]);
							PHTML+=SCANER::replaceSpecSumbolHTML("' в блоке 'dynamic_cast<>()' в строке: ");
							}
						if(er==5){
							PHTML+="Неверно заданый тип в блоке ";
							PHTML+=SCANER::replaceSpecSumbolHTML("'dynamic_cast<?>()' в строке: ");
							}
						if(er==6){
							PHTML+="Требуетса выражение в блоке ";
							PHTML+=SCANER::replaceSpecSumbolHTML("'dynamic_cast<>(?)' в строке: ");
							}
						PHTML+=SCANER::toString(SCANER::findNumberStringLine(F.text,pos->c));
						PHTML+="</font><br/>\n";
						}
					if(x)delete x;
					if(A)delete A;
					delete R;
					R=NULL;
					break;
					}
				Cast*h=new(Cast);
				h->n=1;
				h->cast=*x;
				delete x;
				h->X=A;
				*X=h;
				X=NULL;
				n=2;
				continue;
				}
			if(pos->nfunc<4 || pos->term=="#"){
				Base*B=new(Base);
				B->type=pos->nfunc;
				B->text=pos->term;
				*X=B;
				X=NULL;
				n=2;
				continue;
				}
			if(pos->nfunc==4)if(pos->term=="("){ // преобразование типов
				L_OT::iterator pos2=pos;
				++pos;
				Type*x=Type::getType(pos);
				bool ok=(pos->term==")");
				if(!x)ok=0;
				if(ok){
					L_OT::iterator pos3=pos;
					++pos3;
					ok = (
						pos3->term=="(" || 
						pos3->term=="{" || 
						pos3->nfunc<4 || 
						((pos3->nfunc==4 || pos3->nfunc==6) && pos3->term=="#")
						);
					}
				if(ok){
					Cast*h=new(Cast);
					h->cast=*x;
					delete x;
					*X=h;
					X=&h->X;
					continue;
					}
				pos=pos2;
				if(x)delete x;
				}
			if(pos->nfunc==4)if((krugli=pos->term=="(") || pos->term=="["){
				++pos;
				S=Assemble::gather(lot,pos);
				Interval*I=NULL;
				if(pos->term==";"){
					++pos;
					Algorithm*S2=Assemble::gather(lot,pos);
					I=new(Interval);
					I->a=!krugli;
					I->A=S;
					I->B=S2;
					I->b=pos->term=="]";
					S=I;
					}
				*X=S;
				X=NULL;
				if(!I){
					if(pos->term==","){
						bool isOK = Main->GoErrorMessage(
							PHTML,
							"EK14",
							"",
							SCANER::toString(SCANER::findNumberStringLine(F.text,pos->c))
							);
						if(!isOK){
							PHTML+="<font class='red'>Не должно быть ком в скобках в строке: ";
							PHTML+=SCANER::toString(SCANER::findNumberStringLine(F.text,pos->c));
							PHTML+="</font><br/>\n";
							}
						break;
						}
					bool krugli2=pos->term==")";
					if(!krugli || !krugli2){
						bool isOK = Main->GoErrorMessage(
							PHTML,
							"EB15",
							"",
							SCANER::toString(SCANER::findNumberStringLine(F.text,pos->c))
							);
						if(!isOK){
							PHTML+="<font class='red'>Скобки должны быть круглые в строке: ";
							PHTML+=SCANER::toString(SCANER::findNumberStringLine(F.text,pos->c));
							PHTML+="</font><br/>\n";
							}
						break;
						}
					}
				if(!S){
					delete R;
					R=NULL;
					break;
					}
				n=2;
				continue;
				}
			if(pos->nfunc==4)if(pos->term=="{"){
				AlgoSet*as=new(AlgoSet);
				++pos;
				L_OT::iterator pos2=pos;
				bool nfirst=0;
				int nok=0;
				while(pos->term!="}"){
					if(nfirst){
						if(pos->term==",")++pos; else {nok=1;break;}
						if(pos->term=="}")break;
						}
					nfirst=1;
					Algorithm*A=Assemble::gather(lot,pos);
					if(A)as->nabor.push_back(A); else {nok=2;break;}
					}
				*X=as;
				if(nok){
					delete as;
					pos=pos2;
					nok=0;
					AComposition*ac=new(AComposition);
					*X=ac;
					while(pos->term!="}"){
						if(pos->nfunc>1){nok=3;break;}
						string name=pos->term;
						++pos;
						if(pos->term!=":"){nok=4;break;}
						++pos;
						Algorithm*A=Assemble::gather(lot,pos);
						if(A)ac->table[name]=A; else {nok=2;break;}
						if(pos->term!="}")
						if(pos->term==";" || pos->term==",")++pos; else {nok=1;break;}
						}
					if(nok){
						const char* mer[] = {"NP16","EP17","NN18","NP19"};
						bool isOK = Main->GoErrorMessage(
							PHTML,
							mer[nok-1],
							"",
							SCANER::toString(SCANER::findNumberStringLine(F.text,pos->c))
							);
						if(!isOK){
							PHTML+="<font class='red'>";
							if(nok==1)PHTML+="Отсутствует разделитель ';' или ',' внутри композиции в строке: ";
							if(nok==2)PHTML+="Некорректно задан параметр внутри композиции в строке: ";
							if(nok==3)PHTML+="Требуетса имя поля внутри композиции в строке: ";
							if(nok==4)PHTML+="Пропущен разделитель ':' между именем поля и значением его в строке: ";
							PHTML+=SCANER::toString(SCANER::findNumberStringLine(F.text,pos->c));
							PHTML+="</font><br/>\n";
							}
						break;
						}
					}
				X=NULL;
				n=2;
				continue;
				}
			bool isOK = Main->GoErrorMessage(
				PHTML,
				"NA20",
				"",
				SCANER::toString(SCANER::findNumberStringLine(F.text,pos->c))
				);
			if(!isOK){
				PHTML+="<font class='red'>Отсутствует аргумент в строке: ";
				PHTML+=SCANER::toString(SCANER::findNumberStringLine(F.text,pos->c));
				PHTML+="</font><br/>\n";
				}
			delete R;
			R=NULL;
			break;
			}
		//operator zone:
		if(Sufix::isSufix(pos->term,i)){
			X=&R;
			while(1){
				if(*X==S)break;
				SubBase*Y=dynamic_cast<SubBase*>(*X);
				if(!Y)break;
				if(Y->getPriorited()>1)X=Y->getR(); else break;
				}
			if(i==2){ // '(' for function
				CallFunc*cf=new(CallFunc);
				cf->X=*X;
				*X=cf;
				++pos;
				bool nfirst=0;
				int nok=0;
				while(pos->term!=")"){
					if(nfirst){
						if(pos->term==",")++pos; else {nok=1;break;}
						}
					nfirst=1;
					Algorithm*A=Assemble::gather(lot,pos);
					if(A)cf->params.push_back(A); else {nok=2;break;}
					}
				if(nok==1){
					bool isOK = Main->GoErrorMessage(
						PHTML,
						"EB21",
						"",
						SCANER::toString(SCANER::findNumberStringLine(F.text,pos->c))
						);
					if(!isOK){
						PHTML+="<font class='red'>";
						PHTML+="Параметры функции должны быть в круглых скобках разделены комой в строке: ";
						PHTML+=SCANER::toString(SCANER::findNumberStringLine(F.text,pos->c));
						PHTML+="</font><br/>\n";
						}
					break;
					}
				if(nok==2){
					bool isOK = Main->GoErrorMessage(
						PHTML,
						"NP22",
						"",
						SCANER::toString(SCANER::findNumberStringLine(F.text,pos->c))
						);
					if(!isOK){
						PHTML+="<font class='red'>";
						PHTML+="Параметр некорректный или отсутствует в строке: ";
						PHTML+=SCANER::toString(SCANER::findNumberStringLine(F.text,pos->c));
						PHTML+="</font><br/>\n";
						}
					break;
					}
				continue;
				}
			if(i>2){ // [] {}
				AccesMasiv*am=new(AccesMasiv);
				am->n=i;
				am->X=*X;
				*X=am;
				++pos;
				am->P=Assemble::gather(lot,pos);
				bool ok;
				ok=pos->term==(i==3?"]":"}");
				if(!ok || !am->P){
					bool isOK = Main->GoErrorMessage(
						PHTML,
						((!ok)?"EB23":"EI24"),
						"",
						SCANER::toString(SCANER::findNumberStringLine(F.text,pos->c))
						);
					if(!isOK){
						if(!ok){
							PHTML+="<font class='red'>";
							PHTML+="Несоответствие скобок при доступе к масиву в строке: ";
							PHTML+=SCANER::toString(SCANER::findNumberStringLine(F.text,pos->c));
							PHTML+="</font><br/>\n";
							break;
							}
						if(!am->P){
							PHTML+="<font class='red'>";
							PHTML+="Некорректный индекс масива в строке: ";
							PHTML+=SCANER::toString(SCANER::findNumberStringLine(F.text,pos->c));
							PHTML+="</font><br/>\n";
							break;
							}
						}
					break;
					}
				continue;
				}
			Sufix*suf=new(Sufix);
			suf->n=i;
			suf->X=*X;
			*X=suf;
			continue;
			}
		if(pos->nfunc==4)if(Base2::isBase2(pos->term,i)){
			Base2*B2;
			if(i==11){
				Base3*B3=new(Base3);
				B2=B3;
				++pos;
				B3->V=Assemble::gather(lot,pos);
				if(B3->V)if(pos->term!=":"){
					delete B3->V;
					B3->V=NULL;
					}
				if(!B3->V){
					bool isOK = Main->GoErrorMessage(
						PHTML,
						"ET25",
						"",
						SCANER::toString(SCANER::findNumberStringLine(F.text,pos->c))
						);
					if(!isOK){
						PHTML+="<font class='red'>";
						PHTML+="Некорректо задан тренарный оператор (_?_:_) в строке: ";
						PHTML+=SCANER::toString(SCANER::findNumberStringLine(F.text,pos->c));
						PHTML+="</font><br/>\n";
						}
					delete R;
					R=NULL;
					break;
					}
				}else B2=new(Base2);
			B2->n=i;
			int p=B2->getPriorited(),p2;
			X=&R;
			while(1){
				if(*X==S)break;
				SubBase*Y=dynamic_cast<SubBase*>(*X);
				if(!Y)break;
				p2=Y->getPriorited();
				if(p2>p)X=Y->getR(); else{
					if(p2<p)break; else{ // p2==p
						Base2*BB=dynamic_cast<Base2*>(*X);
						if(BB)if(BB->isR()){
							X=Y->getR();
							continue;
							}
						Prefix*PP=dynamic_cast<Prefix*>(*X);
						if(PP){
							X=Y->getR();
							continue;
							}
						break;
						}
					}
				}
			B2->A=*X;
			*X=B2;
			X=&B2->B;
			n=1;
			continue;
			}
		break;
		}
	if(R)if(n==1)if(X)if(!*X){
		delete R;
		R=NULL;
		--pos;
		bool isOK = Main->GoErrorMessage(
			PHTML,
			"NA26",
			"",
			SCANER::toString(SCANER::findNumberStringLine(F.text,pos->c))
			);
		if(!isOK){
			PHTML+="<font class='red'>Отсутствует ожидаемый аргумент в строке: ";
			PHTML+=SCANER::toString(SCANER::findNumberStringLine(F.text,pos->c));
			PHTML+="</font><br/>\n";
			}
		}
	return R;
}



int SubBase::getPriorited(){return 0;}
int Prefix::getPriorited(){
	if(n==2)return 0;
	if(n==7)return 8;
	return 2;
}
Algorithm** Base2::getR(){return &B;}
Algorithm** Prefix::getR(){return &X;}


// algorithm.cpp	:-|