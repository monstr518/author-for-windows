// problem.cpp
#include "stdafx.h"
#include "formuls.h"

Problem::~Problem(){}

Problem* Problem::getProblem(char*s){
	Systems*S=new(Systems);
	if(S->load(s))return S;
	delete S;
	VarNames*V=new(VarNames);
	if(V->load(s))return V;
	delete V;
	Result*R=new(Result);
	if(R->load(s))return R;
	delete R;
	Compare*C=new(Compare);
	if(C->load(s))return C;
	delete C;
	Optimize*O=new(Optimize);
	if(O->load(s))return O;
	delete O;
	Equality*E=new(Equality);
	if(E->load(s))return O;
	delete E;
	Generalized*G=new(Generalized);
	if(G->load(s))return G;
	delete G;
	return NULL;
}


//-----------------------------------------------------------------------------------------------
VarNames::VarNames(){}

VarNames::~VarNames(){
	for(int i=0;i<S.size();++i)delete S[i];
}


//?: what var names formul (a+b=c=t)
bool VarNames::load(char*s){
	int n=0;
	SCANER::noProbel(s);
	n+=SCANER::scanSlovo("?:",s);
	SCANER::noProbel(s);
	n+=SCANER::scanSlovo("what",s);
	SCANER::noProbel(s);
	n+=SCANER::scanSlovo("var",s);
	SCANER::noProbel(s);
	n+=SCANER::scanSlovo("names",s);
	SCANER::noProbel(s);
	n+=SCANER::scanSlovo("formul",s);
	SCANER::noProbel(s);
	if(n<5)return 0;
	if(*s!='(')return 0;
	do{
		++s;
		SCANER::noProbel(s);
		formuls*f=NULL;
		f=formuls::getFormul(s);
		if(!f)return 0;
		S.push_back(f);
		SCANER::noProbel(s);
		}while(*s=='=');
	if(*s!=')')return 0;
	return 1;
}


string VarNames::run(){
	string R;
	S_S List;
	for(int i=0;i<S.size();++i)S[i]->getVariables(List);
	S_S::iterator it=List.begin();
	for(int first=0;it!=List.end();++it){
		if(first)R+=",";
		first=1;
		R+=*it;
		}
	return R;
}


//-----------------------------------------------------------------------------------------------
Result::Result(){f=NULL;}

Result::~Result(){
	if(f)delete f;
}


//?: what result for formula(x+1) from {x:1,2,4;}
bool Result::load(char*s){
	int n=0;
	SCANER::noProbel(s);
	n+=SCANER::scanSlovo("?:",s);
	SCANER::noProbel(s);
	n+=SCANER::scanSlovo("what",s);
	SCANER::noProbel(s);
	n+=SCANER::scanSlovo("result",s);
	SCANER::noProbel(s);
	n+=SCANER::scanSlovo("for",s);
	SCANER::noProbel(s);
	n+=SCANER::scanSlovo("formula",s);
	SCANER::noProbel(s);
	if(n<5)return 0;
	if(*s!='(')return 0;
	++s;
	SCANER::noProbel(s);
	f=formuls::getFormul(s);
	SCANER::noProbel(s);
	if(*s!=')')return 0;
	++s;
	if(!f)return 0;
	SCANER::noProbel(s);
	n=SCANER::scanSlovo("from",s);
	n+=SCANER::scanSlovo("for",s);
	if(n<1)return 0;
	SCANER::noProbel(s);
	if(*s!='{')return 0;
	++s;
	int m=0;
	do{
		SCANER::noProbel(s);
		V_D vd;
		string name=SCANER::getLine(s,vd);
		if(name.empty())return 0;
		SCANER::noProbel(s);
		names.push_back(name);
		v2d.push_back(vd);
		if(vd.size()>m)m=vd.size();
		}while(*s!='}');
	++s;
	for(int i=0;i<v2d.size();++i){
		n=v2d[i].size();
		for(;n<m;++n)v2d[i].push_back(0);
		}
	return m?1:0;
}



string Result::run(){
	S_S fnames;
	f->getVariables(fnames);
	S_S::iterator it=fnames.begin();
	for(;it!=fnames.end();++it)
		if(find(names.begin(),names.end(),*it)==names.end())return ".";
	stringstream str;
	str<<"{";
	int m=v2d[0].size();
	for(int i=0;i<m;++i){
		M_SD table;
		for(int j=0;j<v2d.size();++j)table[names[j]]=v2d[j][i];
		double x=f->compute(&table);
		if(i)str<<", ";
		str<<x;
		}
	str<<"}";
	return str.str();
}





//-----------------------------------------------------------------------------------------------
string result;

void fmain(char*w){
	/*
	char*w=
		//"?: what result for formula(2*x+10-a) from {x:1,2,4;a:8,10,20;}";
		//"?: compare (x+n==x+2-(2-n))";
		//"?: optimize (u*(a-x-x)) from {b=1;}";
		//"?: get equality for ((x+12)*(x-3)=0) unknown {x}";
		//"?: get equality for (12/n=12*7/(18*7)) unknown {n}";
		"?: get common formula from {2*(a+x),2*(a+n*x)}";//2*(a+x),a+n*x
		*/
	Problem*P=Problem::getProblem(w);
	if(P)result=P->run(); else result=":";
	//cout<<result.c_str()<<endl;
	if(P)delete P;
}




// problem.cpp	:-|