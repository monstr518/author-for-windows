// service.cpp
#include "main.h"



//--------------------------------------------------------------------------------------------------

int Algorithm::ReplaceName(const string*A,const string*B){return 0;}

int Base::ReplaceName(const string*A,const string*B){
	if(!type)if(*A==text){
		text=*B;
		return 1;
		}
	return 0;
}


int Prefix::ReplaceName(const string*A,const string*B){return X->ReplaceName(A,B);}

int Sufix::ReplaceName(const string*A,const string*B){return X->ReplaceName(A,B);}

int CallFunc::ReplaceName(const string*A,const string*B){
	int n=X->ReplaceName(A,B);
	V_AL::iterator it=params.begin();
	for(;it!=params.end();++it)n+=(*it)->ReplaceName(A,B);
	return n;
}

int AccesMasiv::ReplaceName(const string*A,const string*B){
	return P->ReplaceName(A,B)+X->ReplaceName(A,B);
}

int Cast::ReplaceName(const string*A,const string*B){return X->ReplaceName(A,B);}

int Base2::ReplaceName(const string*A,const string*B){
	return this->A->ReplaceName(A,B)+this->B->ReplaceName(A,B);
}

int Base3::ReplaceName(const string*A,const string*B){
	return this->A->ReplaceName(A,B)+this->B->ReplaceName(A,B)+V->ReplaceName(A,B);
}

int Interval::ReplaceName(const string*A,const string*B){
	return this->A->ReplaceName(A,B)+this->B->ReplaceName(A,B);
}

int AlgoSet::ReplaceName(const string*A,const string*B){
	int n=0;
	V_AL::iterator it=nabor.begin();
	for(;it!=nabor.end();++it)n+=(*it)->ReplaceName(A,B);
	return n;
}

int AComposition::ReplaceName(const string*A,const string*B){
	int n=0;
	M_SA::iterator it=table.begin();
	for(;it!=table.end();++it){
		//if(it->first==*A){it->first=*B;++n;}
		n+=it->second->ReplaceName(A,B);
		}
	return n;
}

int Sequence::ReplaceName(const string*A,const string*B){
	int n=0;
	L_AL::iterator it=nabor.begin();
	for(;it!=nabor.end();++it)n+=(*it)->ReplaceName(A,B);
	return n;
}

int CreateVar::ReplaceName(const string*A,const string*B){return Init->ReplaceName(A,B);}

int SpecSumbol::ReplaceName(const string*A,const string*B){return 0;}

int SpecSumbol2::ReplaceName(const string*A,const string*B){return X->ReplaceName(A,B);}

int IF::ReplaceName(const string*A,const string*B){
	int n=X->ReplaceName(A,B);
	if(this->A)n+=this->A->ReplaceName(A,B);
	if(E)n+=E->ReplaceName(A,B);
	return n;
}

int WHILE::ReplaceName(const string*A,const string*B){
	int n=X->ReplaceName(A,B);
	if(this->A)n+=this->A->ReplaceName(A,B);
	return n;
}

int FOR::ReplaceName(const string*A,const string*B){
	int n=0;
	if(X)n+=X->ReplaceName(A,B);
	if(this->B)n+=this->B->ReplaceName(A,B);
	if(P)n+=P->ReplaceName(A,B);
	return n;
}




//--------------------------------------------------------------------------------------------------
void BaseFunction::getNames(V_S*,int){}

void Base::getNames(V_S*S,int r){
	if(!type)
	if((r&1)||((bool)(r&2)==(bool)(r&4)))
	if(text!="true"&&text!="false")S->push_back(text);
}

void Base2::getNames(V_S*S,int r){
	B->getNames(S,r);
	if(string(".")==m[n] || string("->")==m[n])r=r&(8|4|1);
	A->getNames(S,r);
}

void Base3::getNames(V_S*S,int r){
	A->getNames(S,r);
	B->getNames(S,r);
	V->getNames(S,r);
}

void Prefix::getNames(V_S*S,int r){X->getNames(S,r);}

void Sufix::getNames(V_S*S,int r){X->getNames(S,r);}

void CallFunc::getNames(V_S*S,int r){
	X->getNames(S,r|2);
	V_AL::iterator it=params.begin();
	for(;it!=params.end();++it)(*it)->getNames(S,r);
}

void AccesMasiv::getNames(V_S*S,int r){
	P->getNames(S,r);
	X->getNames(S,r);
}

void Cast::getNames(V_S*S,int r){X->getNames(S,r);}

void Interval::getNames(V_S*S,int r){
	A->getNames(S,r);
	B->getNames(S,r);
}

void AlgoSet::getNames(V_S*S,int r){
	V_AL::iterator it=nabor.begin();
	for(;it!=nabor.end();++it)(*it)->getNames(S,r);
}

void AComposition::getNames(V_S*S,int r){
	M_SA::iterator it=table.begin();
	for(;it!=table.end();++it){
		if(r&8)S->push_back(it->first);
		it->second->getNames(S,r);
		}
}

void Sequence::getNames(V_S*S,int r){
	L_AL::iterator it=nabor.begin();
	for(;it!=nabor.end();++it)(*it)->getNames(S,r);
}

void CreateVar::getNames(V_S*S,int r){Init->getNames(S,r);}

void SpecSumbol::getNames(V_S*S,int r){}

void SpecSumbol2::getNames(V_S*S,int r){X->getNames(S,r);}

void Label::getNames(V_S*,int){}

void IF::getNames(V_S*S,int r){
	X->getNames(S,r);
	if(A)A->getNames(S,r);
	if(E)E->getNames(S,r);
}

void WHILE::getNames(V_S*S,int r){
	X->getNames(S,r);
	if(A)A->getNames(S,r);
}

void FOR::getNames(V_S*S,int r){
	X->getNames(S,r);
	if(B)B->getNames(S,r);
	if(P)P->getNames(S,r);
}



//--------------------------------------------------------------------------------------------------


/*
Base
Base2
Base3
Prefix
Sufix
CallFunc
AccesMasiv
Cast
Interval
AlgoSet
AComposition
Sequence
CreateVar
SpecSumbol
SpecSumbol2
IF
WHILE
FOR
*/



// service.cpp	:-|