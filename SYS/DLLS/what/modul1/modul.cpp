// modul1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Problem.h"




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
	return 1;
}





string Problem::genf1(double a,double b){
	string R;
	if(a){
		if(a!=1){
			R=SCANER::toString(a);
			R+="*";
			}
		R+=nameX;
		if(b>0)R+="+";
		}
	if(b)R+=SCANER::toString(b);
	if(!a && !b)R="0";
	if(isCode){
		if(isY)R=nameY+"="+R;
		}else R=nameF+"("+nameX+"){return "+R+";}";
	return R;
}





string Problem::genf2(double a,double b){
	string R;
	if(a){
		if(a!=1){
			R=SCANER::toString(a);
			R+="*";
			}
		R+=nameX+"*"+nameX;
		if(b>0)R+="+";
		}
	if(b)R+=SCANER::toString(b);
	if(isCode){
		if(isY)R=nameY+"="+R;
		}else R=nameF+"("+nameX+"){return "+R+";}";
	return R;
}




string Problem::run(){
	//a*x+b==y
	double RR=0;
	for(int a=0;a<10000;++a){
		double R=0,odno;
		bool ok=1,first=1;
		for(int i=0;i<X.size();++i){
			double yy=a*X[i];
			double rr=Y[i]-yy;
			R+=rr*rr;
			if(first){
				first=0;
				odno=rr;
				continue;
				}
			if(odno!=rr)ok=0;
			}
		if(ok){
			return genf1(a,odno);
			}
		if(RR==0)RR=R;
		if(RR<R)break;
		}
	//a*x*x+b
	RR=0;
	for(a=0;a<10000;++a){
		double R=0,odno;
		bool ok=1,first=1;
		for(int i=0;i<X.size();++i){
			double yy=a*X[i]*X[i];
			double rr=Y[i]-yy;
			R+=rr*rr;
			if(first){
				first=0;
				odno=rr;
				continue;
				}
			if(odno!=rr)ok=0;
			}
		if(ok){
			return genf2(a,odno);
			}
		if(RR==0)RR=R;
		if(RR<R)break;
		}
	return ".";
}




