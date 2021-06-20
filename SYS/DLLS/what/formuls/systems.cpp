//	systems.cpp

#include "stdafx.h"
#include "formuls.h"

extern int cok;

Systems::Systems(){}

Systems::~Systems(){
	for(int i=0;i<A.size();++i)delete A[i];
	for(i=0;i<B.size();++i)delete B[i];
	for(i=0;i<Find.size();++i)delete Find[i];
}


//?: system equations {2*x+5*y=15;3*x+8*y=-1;x=?;y=?}
bool Systems::load(char*s){
	int n=0;
	SCANER::noProbel(s);
	n+=SCANER::scanSlovo("?:",s);
	SCANER::noProbel(s);
	n+=SCANER::scanSlovo("system",s);
	SCANER::noProbel(s);
	n+=SCANER::scanSlovo("equations",s);
	SCANER::noProbel(s);
	if(n<3)return 0;
	if(*s!='{')return 0;
	++s;
	SCANER::noProbel(s);
	int a,b;
	u=0;
	Exit=0;
	do{
		a=b=0;
		if(*s=='?'){a=1;++s;}
		formuls*f=NULL,*f2=NULL;
		if(!a){
			f=formuls::getFormul(s);
			if(!f)return 0;
			}
		SCANER::noProbel(s);
		if(*s!='=')return 0;
		++s;
		SCANER::noProbel(s);
		if(*s=='?'){b=1;++s;}
		if(!b){
			f2=formuls::getFormul(s);
			if(!f2)return 0;
			}
		SCANER::noProbel(s);
		while(*s==';'){
			++s;
			SCANER::noProbel(s);
			}
		if(!*s)return 0;
		if(!a&&!b){
			A.push_back(f);
			B.push_back(f2);
			C.push_back(++u);
			}
		if(a && !b)push(f2);
		if(!a && b)push(f);
		}while(*s!='}');
	return 1;
}



void Systems::push(formuls*f){
	int ok=1;
	for(int i=0;i<Find.size();++i)if(*Find[i]==*f){ok=0;break;}
	if(ok)Find.push_back(f);else delete f;
}



string Systems::run(){
	V_I Result,FOK;
	FOK.reserve(Find.size());
	for(int t=0;t<Find.size();++t)FOK[t]=0;
	for(t=0;t<A.size();++t)Result.push_back(-1);
	for(int R,i=0;i<A.size();++i){
		if(i>1000)return ".";
		if(cok)cout<<A[i]->toString().c_str()<<"="<<B[i]->toString().c_str()<<endl;
		S_S Names;
		A[i]->getVariables(Names);
		B[i]->getVariables(Names);
		S_S::iterator it=Names.begin();
		string var;
		value*v=dynamic_cast<value*>(A[i]);
		if(v)if(!v->isValue)var=v->name;
		for(;it!=Names.end();++it){
			if(var==*it)continue;
			Equality E;
			E.A=A[i]->copy();
			E.B=B[i]->copy();
			E.unknown.insert(*it);
			string str=E.run();
			const char*s=str.c_str();
			//cout<<s<<endl;
			int x=0;
			formuls*f1=formuls::getFormul(s),*f2,*ff1;
			SCANER::noProbel(s);
			++s;//'='
			SCANER::noProbel(s);
			if(*s=='{'){
				++s;
				SCANER::noProbel(s);
				x=1;
				ff1=f1->copy();
				}
			f2=formuls::getFormul(s);
			R=add(f1,f2,C[i]);
			if(R>-2){
				if(R>=0)FOK[R]=1;
				Result.push_back(R);
				}
			SCANER::noProbel(s);
			if(x){
				++s;//','
				SCANER::noProbel(s);
				f2=formuls::getFormul(s);
				R=add(ff1,f2,C[i]);
				if(R>-2){
					if(R>=0)FOK[R]=1;
					Result.push_back(R);
					}
				}
			}
		for(int j=0;j<i;++j)if(C[i]!=C[j])if(*A[i]==*A[j]){
			R=add(B[i]->copy(),B[j]->copy(),++u);
			if(R>-2)Result.push_back(R);
			}
		if(Exit)break;
		for(j=t=0;t<Find.size();++t)if(FOK[t])++j;
		if(j==Find.size())break;
		}
	for(i=t=0;t<Find.size();++t)if(FOK[t])++i;
	string otvet;
	if(i<Find.size()){
		for(t=0;t<Find.size();++t){
			for(i=0;i<A.size();++i)if(*A[i]==*Find[t])break;
			if(i<A.size()){
				if(!otvet.empty())otvet+=";";
				otvet+=Find[t]->toString()+"="+B[i]->toString();
				}else break;
			}
		if(t>=Find.size())return otvet;
		Exit=1;
		}
	if(Exit)return "nopset";
	for(t=0;t<Find.size();++t){
		if(t)otvet+=";";
		string str;
		int n=0;
		for(i=0;i<Result.size();++i)if(Result[i]==t){
			if(n)str+=",";
			str+=B[i]->toString();
			++n;
			}
		if(n>1)str="{"+str+"}";
		otvet+=Find[t]->toString();
		if(n>0)otvet+="="+str;else{
			for(i=0;i<A.size();++i)if(*A[i]==*Find[t])break;
			if(i<A.size())otvet+="="+B[i]->toString();
			}
		}
	return otvet;
}



int Systems::add(formuls*a,formuls*b,int c){
	int ok=1;
	for(int i=0;i<A.size();++i)
		if(*A[i]==*a && *B[i]==*b){
			ok=0;
			break;
			}
	if(a->calculated() && b->calculated()){
		ok=0;
		double x=a->compute()-b->compute();
		if(x<0)x=-x;
		if(x>1e-2)Exit=1;
		}
	if(ok){
		A.push_back(a);
		B.push_back(b);
		C.push_back(c);
		int R=-1;
		for(int t=0;t<Find.size();++t)if(*Find[t]==*a)if(b->calculated()){
			R=t;
			break;
			}
		return R;
		}
	delete a;
	delete b;
	return -2;
}



//	systems.cpp	:-|