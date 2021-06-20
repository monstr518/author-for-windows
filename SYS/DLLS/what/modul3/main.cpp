// main.cpp

#include "stdafx.h"
#include "main.h"

extern int cok;

//--------------------------------------------------------------------------------------------
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




//--------------------------------------------------------------------------------------------
ProblemA::ProblemA(){
	firstX=0;
	er=0;
	name="x";
}




//?: get code for row {0,2,4,6} first(0)
int ProblemA::ParserProblem(const char*ss){
	char*s=const_cast<char*>(ss);
	SCANER::noProbel(s);
	if(!SCANER::scanSlovo("?:",s))return 0;
	SCANER::noProbel(s);
	if(!SCANER::scanSlovo("get",s))return 0;
	SCANER::noProbel(s);
	if(!SCANER::scanSlovo("code",s))return 0;
	SCANER::noProbel(s);
	if(*s=='('){
		++s;
		SCANER::noProbel(s);
		name=SCANER::getName(s);
		SCANER::noProbel(s);
		if(*s!=')')return 0;
		++s;
		SCANER::noProbel(s);
		}
	if(!SCANER::scanSlovo("for",s))return 0;
	SCANER::noProbel(s);
	if(!SCANER::scanSlovo("row",s))return 0;
	SCANER::noProbel(s);
	if(*s!='{')return 0;
	do{
		++s;
		SCANER::noProbel(s);
		int p=0;
		double n=0;
		if(*s=='*'){
			++s;
			p=1;
			}else n=SCANER::getDigit(s);
		M.push_back(n);
		P.push_back(p);
		SCANER::noProbel(s);
		}while(*s==',');
	if(*s!='}')return 0;
	++s;
	SCANER::noProbel(s);
	if(SCANER::scanSlovo("first",s)){
		SCANER::noProbel(s);
		if(*s!='(')return 0;
		++s;
		SCANER::noProbel(s);
		firstX=SCANER::getDigit(s);
		SCANER::noProbel(s);
		if(*s!=')')return 0;
		}
	return 1;
}




int ProblemA::oneF(V_D&W,V_I&PP,double&a){
	double n;
	int first=1;
	for(int i=0;i<W.size();++i){
		if(PP[i])continue;
		if(first){
			n=W[i];
			first=0;
			continue;
			}
		if(1e-10<abs(W[i]-n))break;
		}
	if(first)return -1;
	if(i==W.size()){
		a=n;
		return 0;
		}
	V_D U;
	V_I P2;
	int p;
	for(i=1;i<W.size();++i){
		double y=W[i]-W[i-1];
		U.push_back(y);
		p=(PP[i] || PP[i-1]);
		P2.push_back(p);
		if(cok){
			if(i>1)cout<<", ";
			if(p)cout<<"*";else cout<<y;
			}
		}
	if(cok)cout<<endl;
	i=oneF(U,P2,n);
	if(i<0)return i;
	a=n;
	return i+1;
}




void ProblemA::run(){
	double a,sa;
	int n,i,j,k;
	V_D Y;
	do{
	Y=M;
	for(i=0;i<Y.size();++i){
		sa=0;
		//a*x^n/n!
		for(j=0;j<N.size();++j){
			double xn=1,nf=1;
			for(k=1;k<=N[j];++k){
				xn*=(i+firstX);
				nf*=k;
				}
			a=A[j]*xn/nf;
			sa+=a;
			}
		Y[i]-=sa;
		if(cok){
			if(i)cout<<", ";
			if(P[i])cout<<"*";else cout<<Y[i];
			}
		}
	if(cok)cout<<endl;
	n=oneF(Y,P,a);
	if(n<0){er=1;return;}
	if(cok)cout<<endl;
	if(!N.empty())if(N[N.size()-1]<=n){er=1;return;}
	A.push_back(a);
	N.push_back(n);
	}while(n);
}




string ProblemA::getResult(){
	if(er)return ".";
	ostringstream ss;
	int i,k;
	for(i=0;i<N.size();++i){
		if(i)if(A[i]>0)ss<<"+";
		if(!A[i])continue;
		double a,nf=1;
		for(k=1;k<=N[i];++k)nf*=k;
		a=A[i]/nf;
		if(N[i]){
			if(a!=1 && a!=-1){
				ss<<a;
				ss<<"*";
				}
			}else ss<<a;
		if(a==-1)ss<<"-";
		for(k=0;k<N[i];++k){
			if(k)ss<<"*";
			ss<<name;
			}
		}
	return ss.str();
}


// main.cpp	:-|