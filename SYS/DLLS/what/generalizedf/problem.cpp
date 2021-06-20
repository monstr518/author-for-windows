// problem.cpp
#include "stdafx.h"
#include "GENERALIZEDF.h"

extern int cok;

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
//M1::M1(string s):Answer(2){formula=s;}

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


//?: what generalized formula for (-0.5*i+6.5, (15-i)/2) values{t:7,8;}
int Problem::ParserProblem(const char*ss){
	char*s=const_cast<char*>(ss);
	SCANER::noProbel(s);
	if(!SCANER::scanSlovo("?:",s))return 0;
	SCANER::noProbel(s);
	n=SCANER::scanSlovo("what",s);
	SCANER::noProbel(s);
	n+=SCANER::scanSlovo("generalized",s);
	SCANER::noProbel(s);
	n+=SCANER::scanSlovo("formula",s);
	SCANER::noProbel(s);
	n+=SCANER::scanSlovo("for",s);
	if(n<4)return 0;
	SCANER::noProbel(s);
	if(*s!='(')return 0;
	++s;
	SCANER::noProbel(s);
	n=0;
	do{
		if(*s=='(')++n;
		if(*s==')')--n;
		if(n<0)break;
		GFormula+=*s;
		++s;
		}while(*s);
	SCANER::noProbel(s);
	if(*s!=')')return 0;
	++s;
	SCANER::noProbel(s);
	if(!SCANER::scanSlovo("values",s))return 0;
	SCANER::noProbel(s);
	if(*s!='{')return 0;
	++s;
	string name;
	n=0;
	do{
		V_D V;
		name=getLine(s,V);
		if(name.size()){
			if(!n)n=V.size();else if(n!=V.size())return 0;
			table[name]=V;
			}
		SCANER::noProbel(s);
		}while(name.size());
	if(*s!='}')return 0;
	n=0;
	Reception=NULL;
	return 1;
}


//--------------------------------------------------------------------------------------------
Problem::~Problem(){ClearQ();}


//заменить имя в формуле
void Problem::replaceNames(string stra,string str){
	int i=0;
	while(i>=0){
		i=GFormula.find(stra,i);
		if(i>=0){
			if(i-1>=0){
				char c=GFormula[i-1];
				if(!(c=='+'||c=='*'||c=='-'||c=='/'||c==','||c=='(')){
					i+=stra.length();
					continue;
					}
				}
			if(i+stra.length()<GFormula.length()){
				char c=GFormula[i+stra.length()];
				if(!(c=='+'||c=='*'||c=='-'||c=='/'||c==','||c==')')){
					i+=stra.length();
					continue;
					}
				}
			GFormula.replace(i,stra.length(),str);
			i+=str.length();
			}
		}
}



void Problem::run(){
	if(Reception)return;
	if(!n){
		buf="?: get generalized tree formula for ("+GFormula+")";
		//cout<<"OK! "<<GFormula.c_str()<<endl;
		Reception=new(Answer);
		Q.push_back(Reception);
		n=1;
		return;
		}
	if(n==1){
		Answer*A=getQ();
		if(!A){n=5;run();return;}
		//cout<<A->S[0].c_str()<<endl;
		//((a-i)/b) for table {a:13,15;b:2,2;}
		string S=A->S[0];
		ClearQ();
		int i=S.find_last_of(')');
		GFormula=S.substr(1,i-1);
		//cout<<GFormula.c_str()<<endl;
		i=S.find('{');
		char*s=const_cast<char*>(S.c_str())+i;
		++s;
		string name;
		do{
			V_D V;
			name=getLine(s,V);
			if(name.size())tableEX[name]=V;
			SCANER::noProbel(s);
			}while(name.size());
		//if(*s!='}')return 0;
		M_SDD NEW;
		M_SDD::iterator it=tableEX.begin();
		for(;it!=tableEX.end();++it){
			string str=it->first;
			do{
				if(table.find(str)==table.end()){
					NEW[str]=it->second;
					break;
					}
				str+=('0'+rand()%10);
				}while(1);
			if(str==it->first)continue;
			//заменить имя в формуле
			replaceNames(it->first,str);
			}
		NEW.swap(tableEX);
		//cout<<GFormula<<endl;
		n=2;
		}
	if(n==2){
		if(tableEX.empty()){Results.S.push_back(GFormula);return;}
		//сформулируй вопрос какая закономерность tableEX[i] от table
		M_SDD::iterator it=tableEX.begin();
		string NameY=it->first;
		ostringstream vopros;
		vopros<<"?: get code "<<NameY<<"=f(";
		M_SDD::iterator jt=table.begin();
		string nams;
		for(bool fi=1;jt!=table.end();++jt){
			if(!fi)nams+=",";
			fi=0;
			nams+=jt->first;
			}
		vopros<<nams<<") for table: head("<<nams<<","<<NameY<<"){";
		//"x:1,2;t:5,10;"
		jt=table.begin();
		for(;jt!=table.end();++jt){
			vopros<<jt->first<<":";
			V_D&VD=jt->second;
			for(int i=0;i<VD.size();++i){
				if(i)vopros<<",";
				vopros<<VD[i];
				}
			vopros<<";";
			}
		vopros<<NameY<<":";
		V_D&VD=it->second;
		for(int i=0;i<VD.size();++i){
			if(i)vopros<<",";
			vopros<<VD[i];
			}
		vopros<<";}";
		buf=vopros.str();
		//cout<<buf<<endl;//
		Reception=new Answer(3);
		Q.push_back(Reception);
		n=3;
		tableEX.erase(NameY);
		return;
		}
	if(n==3){
		Answer*A=getQ();
		if(!A){n=2;run();return;}
		if(A->t==3){
			string ff=A->Next();
			//cout<<ff<<endl;//a=2*t-1
			int j=ff.find("=");
			string NameY=ff.substr(0,j);
			ff=ff.substr(j+1);
			//cout<<NameY<<endl;
			//cout<<ff<<endl;
			//нужно заменить имя на выражение в GFormula
			replaceNames(NameY,ff);
			//cout<<GFormula<<endl;
			n=2;
			run();
			return;
			}
		}
	Results.S.push_back(".");
}
//?: what generalized formula for (-0.5*i+6.5, (15-i)/2) values{t:7,8;}



// problem.cpp	:-|