// modul2.cpp
//

#include "stdafx.h"
#include "Problem.h"
#include <algorithm>



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



char* SCANER::toString(double n){
	static char s[10];
	if(n==(int)n)sprintf(s,"%d",(int)n);else
	sprintf(s,"%f",(float)n);
	return s;
}



//A:1,2;
bool Row::getLine(char*&s){
	SCANER::noProbel(s);
	nameA=SCANER::getName(s);
	SCANER::noProbel(s);
	if(*s!=':')return 0;
	do{
		++s;
		SCANER::noProbel(s);
		double n=SCANER::getDigit(s);
		R.push_back(n);
		SCANER::noProbel(s);
		}while(*s==',');
	if(*s!=';')return 0;
	++s;
	return 1;
}



//?: get code(n) for signal(A,B) {A:1,2;B:0,5,10;} pset{0,4}
int Problem::ParserProblem(const char*ss){
	char*s=const_cast<char*>(ss);
	SCANER::noProbel(s);
	if(!SCANER::scanSlovo("?:",s))return 0;
	SCANER::noProbel(s);
	if(!SCANER::scanSlovo("get",s))return 0;
	SCANER::noProbel(s);
	if(!SCANER::scanSlovo("code",s))return 0;
	SCANER::noProbel(s);
	if(*s!='(')return 0;
	++s;
	nameValue=SCANER::getName(s);
	SCANER::noProbel(s);
	if(*s!=')')return 0;
	++s;
	SCANER::noProbel(s);
	if(!SCANER::scanSlovo("for",s))return 0;
	SCANER::noProbel(s);
	if(!SCANER::scanSlovo("signal",s))return 0;
	SCANER::noProbel(s);
	if(*s!='(')return 0;
	++s;
	string nameA;
	int nex;
	do{
		SCANER::noProbel(s);
		nameA=SCANER::getName(s);
		Ret.push_back(nameA);
		SCANER::noProbel(s);
		nex=0;
		if(*s==','){++s;nex=1;}
		}while(nex);
	SCANER::noProbel(s);
	if(*s!=')')return 0;
	++s;
	SCANER::noProbel(s);
	if(*s!='{')return 0;
	++s;
	for(;;){
		Row r;
		if(!r.getLine(s))break;
		vrow.push_back(r);
		}
	for(int i=0;i<Ret.size();++i){
		int j,ok=0;
		for(j=0;j<vrow.size();++j)
			if(vrow[j].nameA==Ret[i]){ok=1;break;}
		if(!ok)return 0;
		}
	SCANER::noProbel(s);
	if(*s!='}')return 0;
	++s;
	SCANER::noProbel(s);
	if(SCANER::scanSlovo("pset",s)){
		SCANER::noProbel(s);
		if(*s!='{')return 0;
		do{
			++s;
			SCANER::noProbel(s);
			double n=SCANER::getDigit(s);
			pset.push_back(n);
			SCANER::noProbel(s);
			}while(*s==',');
		if(*s!='}')return 0;
		}
	return 1;
}






void Row::controlAB(){
	for(int i=0;i<R.size();++i){
		if(!i)A=B=R[i];
		if(A>R[i])A=R[i];
		if(B<R[i])B=R[i];
		}
}


void Problem::controlAB(){
	for(int i=0;i<vrow.size();++i)vrow[i].controlAB();
}



double Problem::findX(bool&c,bool a,double A,double B,bool b,bool freeA,bool freeB){
	V_D Q;
	for(int i=0;i<pset.size();++i){
		int okA=0,okB=0;
		if(freeA)okA=1;
		if(freeB)okB=1;
		if(pset[i]<B)okB=1;
		if(b)if(pset[i]==B)okB=1;
		if(pset[i]>A)okA=1;
		if(a)if(pset[i]==A)okA=1;
		if(okA && okB)Q.push_back(pset[i]);
		}
	if(!Q.empty()){
		c=0;
		i=rand()%Q.size();
		return Q[i];
		}
	if(freeA){c=!b;return B;}
	if(freeB){c=!a;return A;}
	c=!a;
	return A;
}



string Problem::run(){
	controlAB();
	for(int i=0;i<vrow.size();++i){
		if(find(Ret.begin(),Ret.end(),vrow[i].nameA)==Ret.end())continue;
		int freeA=1,freeB=1,ok=1;
		double A,B;//(A;B)
		V_pRow U;//исключения из (A;B)
		for(int j=0;j<vrow.size();++j)if(i!=j){
			if(vrow[i].B<vrow[j].A){
				freeB=0;
				B=vrow[j].A;
				continue;
				}
			if(vrow[i].A>vrow[j].B){
				freeA=0;
				A=vrow[j].B;
				continue;
				}
			if(vrow[i].A>vrow[j].A && vrow[i].B<vrow[j].B){
				double A2,B2;
				bool fA2=1,fB2=1;
				for(int k=0;k<vrow[j].R.size();++k){
					double x=vrow[j].R[k];
					if(x>=vrow[i].A && x<=vrow[i].B){ok=0;break;}
					if(x>vrow[i].B){
						if(fB2){fB2=0;B2=x;}
						if(B2>x)B2=x;
						}
					if(x<vrow[i].A){
						if(fA2){fA2=0;A2=x;}
						if(A2<x)A2=x;
						}
					}
				if(!ok)break;
				if(B>B2 || freeB){B=B2;freeB=0;}
				if(A<A2 || freeA){A=A2;freeA=0;}
				continue;
				}
			if(vrow[i].A<vrow[j].A && vrow[i].B>vrow[j].B){
				for(int k=0;k<vrow[i].R.size();++k){
					double x=vrow[i].R[k];
					if(x>=vrow[j].A && x<=vrow[j].B){ok=0;break;}
					}
				U.push_back(&vrow[j]);
				continue;
				}
			ok=0;
			break;
			}
		if(!ok){
			vrow[i].Result=".";
			continue;
			}
		bool c1,c2;
		string str;
		A=findX(c1,1,A,vrow[i].A,0,freeA,0);
		B=findX(c2,0,vrow[i].B,B,1,0,freeB);
		if(freeA && !freeB){
			str+=nameValue;
			str+="<";
			if(c2)str+="=";
			str+=SCANER::toString(B);
			}
		if(freeB && !freeA){
			str+=nameValue;
			str+=">";
			if(c1)str+="=";
			str+=SCANER::toString(A);
			}
		if(!freeA && !freeB){
			str+=nameValue;
			str+=">";
			if(c1)str+="=";
			str+=SCANER::toString(A);
			str+=" && ";
			str+=nameValue;
			str+="<";
			if(c2)str+="=";
			str+=SCANER::toString(B);
			}
		for(j=0;j<U.size();++j){
			int firstA=1,firstB=1;
			for(int k=0;k<vrow[i].R.size();++k){
				double x=vrow[i].R[k];
				if(x<U[j]->A)if(firstA || x>A)A=x;
				if(x>U[j]->B)if(firstB || x<B)B=x;
				}
			A=findX(c1,0,A,U[j]->A,1);
			B=findX(c2,1,U[j]->B,B,0);
			if(!str.empty())str+=" && ";
			str+="(";
			str+=nameValue;
			str+="<";
			if(c1)str+="=";
			str+=SCANER::toString(A);
			str+=" || ";
			str+=nameValue;
			str+=">";
			if(c2)str+="=";
			str+=SCANER::toString(B);
			str+=")";
			}
		vrow[i].Result=str;
		}
	string s;
	int one=Ret.size()==1,ok=0;
	for(i=0;i<Ret.size();++i){
		for(int j=0;j<vrow.size();++j)
		if(vrow[j].nameA==Ret[i]){
			if(!one)s+=Ret[i]+":";
			s+=vrow[j].Result;
			if(vrow[j].Result!=".")ok=1;
			if(!one)s+=";\n";
			}
		}
	return ok?s:".";
}



