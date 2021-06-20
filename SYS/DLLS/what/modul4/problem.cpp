// problem.cpp

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
	double n;
	sscanf(s,"%lf",&n);
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
	sprintf(s,"%lf",n);
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



//?: get code(n) for signal(A,B) {A:1,3;B:0,2,4;}
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
	return Ret.size();
}



void Row::controlN(){
	int first=1;
	n=1;
	for(int i=0;i<R.size();++i)
		for(int j=0;j<R.size();++j)if(R[i]>R[j]){
			int t=R[i]-R[j];
			if(!t)continue;
			if(first){
				first=0;
				n=t;
				continue;
				}
			if(t<n)n=t;
			}
	if(R.size()==1)n=R[0];
}



void Problem::controlN(){
	for(int i=0;i<vrow.size();++i)vrow[i].controlN();
}



void Problem::controlUniqe(){
	int i,j,a,b;
	for(i=0;i<vrow.size();++i)
		for(j=i+1;j<vrow.size();++j){
			int ok=1;
			for(a=0;a<vrow[i].R.size();++a){
				for(b=0;b<vrow[j].R.size();++b)
					if(vrow[i].R[a]==vrow[j].R[b]){
						ok=0;
						break;
						}
				if(!ok)break;
				}
			if(!ok){
				vrow[i].Result=vrow[j].Result=".";
				}
			}
}



string Problem::run(){
	controlN();
	controlUniqe();
	int m=0;
	for(int i=0;i<vrow.size();++i){
		if(vrow[i].n==1)continue;
		if(!m)m=vrow[i].n;
		if(m!=vrow[i].n){
			if(!m%vrow[i].n){
				m=vrow[i].n;
				continue;
				}
			if(!vrow[i].n%m)continue;
			m=0;
			break;
			}
		}
	string str;
	if(!m){
		return ".";
		/*
		if(Ret.size()==1)return ".";
		for(i=0;i<Ret.size();++i)str+=Ret[i]+":.\n";
		return str;
		*/
		}
	for(i=0;i<vrow.size();++i){
		if(vrow[i].Result==".")continue;
		S_I me,notme,*on;
		for(int k=0;k<vrow[i].R.size();++k)me.insert(int(vrow[i].R[k])%m);
		for(int j=0;j<vrow.size();++j)if(j!=i)
			for(k=0;k<vrow[j].R.size();++k)notme.insert(int(vrow[j].R[k])%m);
		int inv=0;
		// все в me не должно быть в notme
		int bed=0;
		S_I::iterator it=me.begin();
		for(;it!=me.end();++it)
			if(find(notme.begin(),notme.end(),*it)!=notme.end()){
				bed=1;
				break;
				}
		if(bed){
			vrow[i].Result=".";
			continue;
			}
		if(me.size()<=notme.size())on=&me;else{
			on=&notme;
			inv=1;
			}
		string t;
		it=on->begin();
		for(int first=1;it!=on->end();++it){
			if(first)first=0;else t+=" && ";
			t+=nameValue+"%"+SCANER::toString(m)+"==";
			t+=SCANER::toString(*it);
			}
		if(inv){
			if(on->size()>1)t="("+t+")";
			t="!"+t;
			}
		vrow[i].Result=t;
		}
	int ok=0;
	for(int u=0;u<Ret.size();++u)
	for(i=0;i<vrow.size();++i)if(Ret[u]==vrow[i].nameA){
		if(Ret.size()==1)return vrow[i].Result;
		str+=Ret[u]+":"+vrow[i].Result+";\n";
		if(vrow[i].Result!=".")ok=1;
		}
	if(!ok)return ".";
	return str;
}



// problem.cpp	:-|