//body.cpp
#include "stdafx.h"
#include "simplezakonf.h"

void SCANER::noProbel(char*&s){
	while(*s=='\r' || *s=='\n' || *s==' ' || *s=='	')++s;
}


int SCANER::scanSlovo(char*slovo,char*&s){
	int i=strncmp(slovo,s,strlen(slovo));
	if(i)return 0;
	s+=strlen(slovo);
	return 1;
}


string SCANER::getName(char*&s){
	string name;
	if(!isalpha(*s))return "";
	do{
		name+=*s;
		++s;
		}while(isalpha(*s) || isdigit(*s));
	return name;
}



double SCANER::getDigit(char*&s){
	float n;
	sscanf(s,"%f",&n);
	if(*s=='-')++s;
	while(isdigit(*s))++s;
	if(*s=='.'){
		++s;
		while(isdigit(*s))++s;
		}
	return n;
}


/*
char* SCANER::toString(double n){
	static char s[10];
	if(n==(int)n)sprintf(s,"%d",(int)n);else
	sprintf(s,"%f",(float)n);
	return s;
}
*/

//--------------------------------------------------------------------------------------------
//x:1,2;
string Problem::getLine(char*&s,V_D&V){
	string name;
	SCANER::noProbel(s);
	name=SCANER::getName(s);
	SCANER::noProbel(s);
	if(*s!=':')return "";
	do{
		++s;
		SCANER::noProbel(s);
		double n=SCANER::getDigit(s);
		V.push_back(n);
		SCANER::noProbel(s);
		}while(*s==',');
	if(*s!=';')return "";
	++s;
	return name;
}



//?: get code y=f(x) for table: head(x,y){x:1,2;y:5,10;}
int Problem::ParserProblem(const char*ss){
	char*s=const_cast<char*>(ss);
	SCANER::noProbel(s);
	if(!SCANER::scanSlovo("?:",s))return 0;
	SCANER::noProbel(s);
	if(!SCANER::scanSlovo("get",s))return 0;
	SCANER::noProbel(s);
	isCode=1;
	if(!SCANER::scanSlovo("code",s)){
		isCode=0;
		if(!SCANER::scanSlovo("function",s))return 0;
		}
	SCANER::noProbel(s);
	if(!isCode)nameF=SCANER::getName(s); else{
		nameY=SCANER::getName(s);
		SCANER::noProbel(s);
		isY=0;
		if(*s=='='){
			isY=1;
			++s;
			SCANER::noProbel(s);
			if(*s!='f')return 0;
			++s;
			}else{
			if(nameY!="f")return 0;
			nameY="";
			}
		}
	SCANER::noProbel(s);
	if(*s!='(')return 0;
	++s;
	nameX=SCANER::getName(s);
	SCANER::noProbel(s);
	if(*s!=')')return 0;
	++s;
	SCANER::noProbel(s);
	if(!SCANER::scanSlovo("for",s))return 0;
	SCANER::noProbel(s);
	if(!SCANER::scanSlovo("table",s))return 0;
	SCANER::noProbel(s);
	if(*s!=':')return 0;
	++s;
	SCANER::noProbel(s);
	if(!SCANER::scanSlovo("head",s))return 0;
	SCANER::noProbel(s);
	if(*s!='(')return 0;
	++s;
	string nameA,nameB,*nY=NULL;
	nameA=SCANER::getName(s);
	SCANER::noProbel(s);
	if(*s!=',')return 0;
	++s;
	SCANER::noProbel(s);
	nameB=SCANER::getName(s);
	SCANER::noProbel(s);
	if(*s!=')')return 0;
	++s;
	SCANER::noProbel(s);
	if(nameX==nameA)nY=&nameB;
	if(nameX==nameB)nY=&nameA;
	if(!nY)return 0;
	if(isCode){
		if(!nameY.empty())if(nameY!=*nY)return 0;
		nameY=*nY;
		}
	if(*s!='{')return 0;
	++s;
	V_D V1,V2;
	nameA=getLine(s,V1);
	nameB=getLine(s,V2);
	SCANER::noProbel(s);
	if(*s!='}')return 0;
	int normal=2;
	if(nameA==nameX && nameB==nameY)normal=1;
	if(nameA==nameY && nameB==nameX)normal=0;
	if(normal==2)return 0;
	if(normal){X=V1;Y=V2;}else{X=V2;Y=V1;}
	while(X.size()>Y.size())X.pop_back();
	while(X.size()<Y.size())Y.pop_back();
	if(X.empty())return 0;
	n=0;
	Reception=NULL;
	return 1;
}


//--------------------------------------------------------------------------------------------
Problem::~Problem(){ClearQ();}


bool Problem::controlInput(){
	int i=0,j;
	for(;i<X.size();++i){
		j=i+1;
		while(j<X.size()){
			if(X[i]==X[j]){
				if(Y[i]!=Y[j])return 0;
				X.erase(X.begin()+j);
				Y.erase(Y.begin()+j);
				continue;
				}
			++j;
			}
		}
	return 1;
}



bool Problem::controlRow(char*s,V_D&Y){
	SCANER::noProbel(s);
	if(*s!='{')return 0;
	++s;
	int i=0,ok;
	do{
		ok=0;
		SCANER::noProbel(s);
		double t=SCANER::getDigit(s);
		SCANER::noProbel(s);
		if(t!=Y[i++])return 0;
		if(*s==','){++s;ok=1;}
		}while(ok);
	if(*s!='}')return 0;
	return 1;
}



void Problem::Set(const char*str){
	if(!Reception)return;
	Reception->Set(str);
	if(!Reception->n)Reception=NULL;
}


Answer* Problem::getQ(){
	Answer*A=NULL;
	for(int i=0;i<Q.size();++i)if(Q[i]->S.size()){
		A=Q[i];
		break;
		}
	if(!A)ClearQ();
	buf="";
	return A;
}


void Problem::ClearQ(){
	for(int i=0;i<Q.size();++i)if(Q[i])delete Q[i];
	Q.clear();
}

//--------------------------------------------------------------------------------------------
Answer::Answer(int x){t=x;n=0;}

void Answer::Set(const char*s){
	if(s[0]=='.')return;
	string sstr=s;
	if(!n){
		if(sstr.substr(0,3)=="{}:"){
			char*ss=const_cast<char*>(s)+3;
			n=SCANER::getDigit(ss);
			return;
			}
		S.push_back(sstr);
		return;
		}
	--n;
	S.push_back(sstr);
}


string Answer::Next(){
	string s=*S.rbegin();
	S.pop_back();
	return s;
}


//--------------------------------------------------------------------------------------------
M1::M1(string s):Answer(2){formula=s;}


//--------------------------------------------------------------------------------------------
void Problem::run(){
	if(!n){
		if(!controlInput()){
			n=4;
			run();
			return;
			}
		ostringstream vopros;
		vopros<<"?first module: get code "<<nameX<<"=f("<<nameY<<") for table:head("
			<<nameX<<","<<nameY<<"){";
		vopros<<nameY<<":";
		for(int i=0;i<Y.size();++i){
			if(i)vopros<<",";
			vopros<<Y[i];
			}
		vopros<<";"<<nameX<<":";
		for(i=0;i<X.size();++i){
			if(i)vopros<<",";
			vopros<<X[i];
			}
		vopros<<";}";
		buf=vopros.str();
		Reception=new(Answer);
		Q.push_back(Reception);
		n=1;
		return;
		}
	if(n==1){
		Answer*A=getQ();
		if(A){
			ostringstream vopros;
			vopros<<"?: get equality for ("<<A->S[0]<<") unknown {"<<nameY<<"}";
			A->S.clear();
			buf=vopros.str();
			Reception=A;
			n=2;
			return;
			}
		n=3;
		}
	if(n==2){
		//cout<<"ALL OK! NO FORMULS!"<<endl;
		Answer*A=getQ();
		if(A){
			if(!A->t){
				string s=A->S[0];
				A->S.clear();
				int p=s.find("{");
				if(p<0){
					Results.S.push_back(s);
					return;
					}
				A->t=1;
				string g;
				for(int i=++p,d=0;i<s.length();++i){
					if(s[i]=='(')++d;
					if(s[i]==')')--d;
					if(s[i]==',' && !d){
						A->S.push_back(g);
						g="";
						continue;
						}
					g+=s[i];
					}
				run();
				return;
				}
			if(A->t==1){
				string s=A->Next();
				ostringstream vopros;
				vopros<<"?: what result for formula("<<s<<") from {"<<nameX<<":";
				for(int i=0;i<X.size();++i){
					if(i)vopros<<",";
					vopros<<X[i];
					}
				vopros<<";}";
				buf=vopros.str();
				Reception=new M1(s);
				Q.push_back(Reception);
				return;
				}
			if(A->t==2){
				string s=A->S[0];
				A->S.clear();
				if(!controlRow(const_cast<char*>(s.c_str()),Y)){
					run();
					return;
					}
				M1*m=dynamic_cast<M1*>(A);
				s=nameY+"="+m->formula;
				Results.S.push_back(s);
				return;
				}
			}
		n=3;
		}
	if(n==3)if(X.size()>1){
		zakon2();
		if(Results.S.size())return;
		zakon3();
		if(Results.S.size())return;
		}
	Results.S.push_back(".");
}



//?: get code y=f(x) for table:head(x,y){y:2,5,-1;x:5,26,2;}
void Problem::zakon2(){
	double x1,x2,y1,y2,k,b;
	x1=X[0];
	x2=X[1];
	y1=Y[0];
	y2=Y[1];
	k=(y1-y2)/(x1-x2);
	b=y1-k*x1;
	for(int i=0;i<X.size();++i)if(X[i]*k+b!=Y[i])return;
	ostringstream F;
	F<<nameY<<"=";
	if(x1-x2==3 || x2-x1==3){
		if(x2-x1==3 && (k==1 || k==-1))k=-k;
		double a=y1-y2,t;
		if(!a)F<<y1;else
		if(k==1){
			F<<nameX;
			t=y1-x1;
			if(t>0)F<<"+"<<t;
			if(t<0)F<<t;
			}else
		if(k==-1){
			t=y1+x1;
			if(t)F<<t;
			F<<"-"<<nameX;
			}else{
			//(x*a+y1*3-a*x1)/3
			if(x2-x1==3 && k>0)a=-a;
			t=y1*3-a*x1;
			if(a>0){
				F<<"(";
				if(a!=1)F<<a<<"*";
				F<<nameX;
				if(t){if(t>0)F<<"+"<<t;else F<<t;}
				F<<")/3";
				}else{
				F<<"(";
				if(t)F<<t;
				F<<"-"<<nameX;
				if(a!=-1)F<<"*"<<-a;
				F<<")/3";
				}
			}
		b=k=0;
		}
	if(k){
		if(k==1)F<<nameX;else
		if(k==-1)F<<"-"<<nameX;else
		F<<k<<"*"<<nameX;
		}
	if(b){
		if(b<0){F<<"-"<<-b;}else{
			if(k)F<<"+";
			F<<b;
			}
		}
	Results.S.push_back(F.str());
}



void Problem::zakon3(){
	if(X.size()<3)return;
	double x1,x2,x3,y1,y2,y3,a,b,c;
	x1=X[0];
	x2=X[1];
	x3=X[2];
	y1=Y[0];
	y2=Y[1];
	y3=Y[2];
	a=(y3-(x3*(y2-y1)+x2*y1-x1*y2)/(x2-x1)) / (x3*(x3-x1-x2)+x1*x2);
	b=(y2-y1)/(x2-x1)-a*(x1+x2);
	c=(x2*y1-x1*y2)/(x2-x1)+a*x1*x2;
	for(int i=0;i<X.size();++i)if(X[i]*X[i]*a+b*X[i]+c!=Y[i])return;
	ostringstream F;
	F<<nameY<<"=";
	if(a==-1)F<<"-";else
	if(a!=1)F<<a<<"*";
	F<<nameX<<"*"<<nameX;
	if(b){
		if(b!=-1 && b!=1){
			if(b<0)F<<"-"<<-b;else F<<"+"<<b;
			F<<"*";
			}
		if(b==-1)F<<"-";
		F<<nameX;
		}
	if(c){
		if(c<0)F<<"-"<<-c;else F<<"+"<<c;
		}
	Results.S.push_back(F.str());
}



//body.cpp	:-|