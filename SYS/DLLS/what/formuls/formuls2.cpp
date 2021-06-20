// formuls.cpp
#include "stdafx.h"
#include "formuls.h"



void SCANER::noProbel(const char*&s){
	while(*s==' ' || *s=='	' || *s=='\n' || *s=='\r')++s;
}


bool SCANER::getDigit(const char*&s,double&x){
	const char*s2=s;
	while(isdigit(*s))++s;
	if(*s=='.'){
		++s;
		while(isdigit(*s))++s;
		}
	sscanf(s2,"%lf",&x);
	return s2<s;
}


string SCANER::getName(const char*&s){
	if(!isalpha(*s))return "";
	string t;
	while(isalpha(*s) || isdigit(*s)){
		t+=*s;
		++s;
		}
	return t;
}


int SCANER::scanSlovo(const char*slovo,const char*&s){
	int i=strncmp(slovo,s,strlen(slovo));
	if(i)return 0;
	s+=strlen(slovo);
	return 1;
}


//x:1,2;
string SCANER::getLine(const char*&s,V_D&V){
	string name;
	SCANER::noProbel(s);
	name=SCANER::getName(s);
	SCANER::noProbel(s);
	if(*s!=':')return "";
	do{
		++s;
		SCANER::noProbel(s);
		double n=0;
		SCANER::getDigit(s,n);
		V.push_back(n);
		SCANER::noProbel(s);
		}while(*s==',');
	if(*s!=';')return "";
	++s;
	return name;
}


//-----------------------------------------------------------------------------------------------
bool formuls::isCorect(){return 1;}

formuls::~formuls(){}

//-----------------------------------------------------------------------------------------------
int formuls::posin(char*m,char c){
	for(int i=0;m[i];++i)if(m[i]==c)return i;
	return -1;
}



formuls* formuls::getFormul(const char*&s){
	char*prefix="-+/*";
	int plevel[]={0,0,1,1};
	int pinverse[]={1,0,1,0};
	int ro=0;//0-prefix (-A), 1-post operator (A+_)
	int t;
	formuls*R=NULL;
	formuls**A=&R,**B=NULL,**C=NULL; // C+ B*
	do{
		SCANER::noProbel(s);
		t=posin(prefix,*s);
		if(!ro){
			if(t>=0){
				++s;
				if(!*A){
					row*X=new(row);
					X->level=plevel[t];
					X->inverse.push_back(pinverse[t]);
					X->sub.push_back(NULL);
					*A=X;
					if(!B && X->level)B=A;
					if(!C && !X->level)C=A;
					A=&X->sub[X->sub.size()-1];
					continue;
					}

				break;
				}
			double x;
			if(SCANER::getDigit(s,x)){
				value*X=new(value);
				X->isValue=1;
				X->x=x;
				*A=X;
				ro=1;
				continue;
				}
			string name=SCANER::getName(s);
			if(name=="pow"){
				SCANER::noProbel(s);
				if(*s!='(')break;
				++s;
				Pow*P=new(Pow);
				P->A=getFormul(s);
				SCANER::noProbel(s);
				if(*s!=',')break;
				++s;
				P->B=getFormul(s);
				SCANER::noProbel(s);
				if(*s!=')')break;
				++s;
				*A=P;
				ro=1;
				continue;
				}
			if(!name.empty()){
				value*X=new(value);
				X->isValue=0;
				X->name=name;
				*A=X;
				ro=1;
				continue;
				}
			if(*s=='('){
				++s;
				*A=getFormul(s);
				SCANER::noProbel(s);
				if(*s!=')')break;
				++s;
				ro=1;
				continue;
				}
			break;
			}
		if(t>=0){
			++s;
			if(plevel[t]){
				if(B){
					row*X=dynamic_cast<row*>(*B);
					X->inverse.push_back(pinverse[t]);
					X->sub.push_back(NULL);
					A=&X->sub[X->sub.size()-1];
					ro=0;
					continue;
					}
				if(C){
					row*X=dynamic_cast<row*>(*C);
					A=&X->sub[X->sub.size()-1];
					row*Y=new(row);
					Y->level=1;
					Y->sub.push_back(*A);
					Y->inverse.push_back(0);
					Y->inverse.push_back(pinverse[t]);
					Y->sub.push_back(NULL);
					*A=Y;
					B=A;
					A=&Y->sub[Y->sub.size()-1];
					ro=0;
					continue;
					}
				}else{
				if(C){
					row*X=dynamic_cast<row*>(*C);
					X->inverse.push_back(pinverse[t]);
					X->sub.push_back(NULL);
					A=&X->sub[X->sub.size()-1];
					B=NULL;
					ro=0;
					continue;
					}
				if(B){
					row*Y=new(row);
					Y->level=0;
					Y->sub.push_back(*B);
					Y->inverse.push_back(0);
					Y->inverse.push_back(pinverse[t]);
					Y->sub.push_back(NULL);
					A=&Y->sub[Y->sub.size()-1];
					*B=Y;
					C=B;
					B=NULL;
					ro=0;
					continue;
					}
				}
			row*Y=new(row);
			Y->level=plevel[t];
			Y->sub.push_back(*A);
			Y->inverse.push_back(0);
			Y->inverse.push_back(pinverse[t]);
			Y->sub.push_back(NULL);
			*A=Y;
			if(plevel[t])B=A; else C=A;
			A=&Y->sub[Y->sub.size()-1];
			ro=0;
			continue;
			}
		break;
		}while(1);
	if(R)if(!R->isCorect()){
		delete R;
		R=NULL;
		}
	return R;
}



bool formuls:: operator != (formuls&f){return !(*this==f);}

//-----------------------------------------------------------------------------------------------
string value::toString(){
	if(!isValue)return name;
	stringstream s;
	s<<x;
	return s.str();
}


double value::compute(M_SD*table){
	if(isValue)return x;
	if(!table)return 0;
	return (*table)[name];
}


void value::ReplaceValue(M_SD&table){
	if(isValue)return;
	if(table.find(name)==table.end())return;
	isValue=1;
	x=table[name];
}


bool value:: operator == (formuls&f){
	value*v=dynamic_cast<value*>(&f);
	if(!v)return 0;
	if(isValue!=v->isValue)return 0;
	if(isValue)return x==v->x;
	return name==v->name;
}


formuls* value::copy(){
	value*t=new(value)(*this);
	return t;
}


bool value::calculated(){
	return isValue;
}


int value::estimate(){
	int n=3;
	if(isValue){
		n=1;
		double t=x;
		if(x<0){++n;t=-x;}
		if(t>int(t))++n;
		}
	return n;
}


void value::getVariables(S_S&S){
	if(!isValue)S.insert(name);
}


bool value::isUnknown(S_S&names){
	if(isValue)return 0;
	return find(names.begin(),names.end(),name)!=names.end();
}



//-----------------------------------------------------------------------------------------------
row::~row(){
	for(int i=0;i<sub.size();++i)if(sub[i])delete sub[i];
}


bool row::isCorect(){
	for(int i=0;i<sub.size();++i){
		if(!sub[i])return 0;
		if(!sub[i]->isCorect())return 0;
		}
	return 1;
}


string row::toString(){
	string s,t;
	//найти неотрицательний елемент
	int n=inverse.size(),i=-1,j=-1;
	for(int ii=0;ii<n;++ii){
		if(inverse[ii])continue;
		if(i<0)i=ii;
		if(j>=0)continue;
		value*v=dynamic_cast<value*>(sub[ii]);
		if(v)if(v->isValue)if(v->x>=0)j=ii;
		}
	if(j>=0)i=j;
	if(i<0){
		if(level)s="1";
		}else{
		t=sub[i]->toString();
		row*R=dynamic_cast<row*>(sub[i]);
		if(R){
			if(level>R->level || inverse[i])t="("+t+")";
			}
		s+=t;
		}
	char* m[]={"+-","*/"};
	for(j=0;j<n;++j)if(j!=i){
		s+=m[level][inverse[j]];
		t=sub[j]->toString();
		row*R=dynamic_cast<row*>(sub[j]);
		if(R){
			if(level>R->level || inverse[j])t="("+t+")";
			}
		s+=t;
		}
	return s;
}


double row::compute(M_SD*table){
	double R=level,x;
	for(int i=0;i<sub.size();++i){
		x=sub[i]->compute(table);
		if(level){
			if(inverse[i])R/=x; else R*=x;
			}else{
			if(inverse[i])R-=x; else R+=x;
			}
		}
	return R;
}


void row::ReplaceValue(M_SD&table){
	for(int i=0;i<sub.size();++i)sub[i]->ReplaceValue(table);
}


bool row:: operator == (formuls&f){
	row*t=dynamic_cast<row*>(&f);
	if(!t)return 0;
	/*
	if(t->sub.size()==1)if(!t->inverse[0])
		if(*this==*t->sub[0])return 1;
	if(sub.size()==1)if(!inverse[0])
		if(*sub[0]==*t)return 1;
	*/
	if(level!=t->level)return 0;
	int n=sub.size();
	if(n!=t->sub.size())return 0;
	V_I by(sub.size());
	for(int i=0;i<n;++i){
		for(int j=0;j<n;++j)if(!by[j])
			if(inverse[i]==t->inverse[j])if(*sub[i]==*t->sub[j]){
				by[j]=1;
				break;
				}
		if(j==n)return 0;
		}
	return 1;
}


formuls* row::copy(){
	row*t=new(row)(*this);
	for(int i=0;i<sub.size();++i)t->sub[i]=sub[i]->copy();
	return t;
}


bool row::calculated(){
	for(int i=0;i<sub.size();++i)if(!sub[i]->calculated())return 0;
	return 1;
}


void row::ReversZnak(){
	for(int i=0;i<inverse.size();++i)inverse[i]=1-inverse[i];
}


int row::estimate(){
	if(!level){ // +-
		for(int i=0;i<sub.size();++i){
			value*v=dynamic_cast<value*>(sub[i]);
			if(v)if(v->isValue)if(v->x<0){
				v->x=-v->x;
				inverse[i]=1-inverse[i];
				}
			}
		}
	int n=1;
	for(int i=0;i<sub.size();++i)n+=sub[i]->estimate();
	return n;
}


void row::getVariables(S_S&S){
	for(int i=0;i<sub.size();++i)sub[i]->getVariables(S);
}


bool row::isUnknown(S_S&names){
	for(int i=0;i<sub.size();++i)if(sub[i]->isUnknown(names))return 1;
	return 0;
}



//-----------------------------------------------------------------------------------------------
Pow::~Pow(){
	if(A)delete A;
	if(B)delete B;
}


bool Pow::isCorect(){
	if(!(A && B))return 0;
	if(!A->isCorect())return 0;
	if(!B->isCorect())return 0;
	return 1;
}


string Pow::toString(){
	return "pow("+A->toString()+","+B->toString()+")";
}


double Pow::compute(M_SD*table){
	double a,b;
	a=A->compute(table);
	b=B->compute(table);
	return pow(a,b);
}


void Pow::ReplaceValue(M_SD&table){
	A->ReplaceValue(table);
	B->ReplaceValue(table);
}


bool Pow:: operator == (formuls&f){
	Pow*p=dynamic_cast<Pow*>(&f);
	if(!p)return 0;
	return *A==*p->A && *B==*p->B;
}


formuls* Pow::copy(){
	Pow*t=new(Pow);
	t->A=A->copy();
	t->B=B->copy();
	return t;
}


bool Pow::calculated(){
	return A->calculated() && B->calculated();
}


int Pow::estimate(){
	return 2+A->estimate()+B->estimate();
}


void Pow::getVariables(S_S&S){
	A->getVariables(S);
	B->getVariables(S);
}


bool Pow::isUnknown(S_S&names){
	return A->isUnknown(names) || B->isUnknown(names);
}


// formuls.cpp	:-|