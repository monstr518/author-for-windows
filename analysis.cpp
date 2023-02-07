// analysis.cpp
#include "main.h"

//--------------------------------------------------------------------------------------------------
CVARIANT Algorithm::Analysis(){
	CVARIANT R;
	R.avtoSet("map");
	return R;
}


CVARIANT Base::Analysis(){
	CVARIANT R,V;
	R.avtoSet("map");
	V.avtoSet("int");
	V.DATA.intVal=type==4?5:0;
	(*R.DATA.mapVal)["vid"]=V;
	V.avtoSet("string");
	if(!type){
		*V.DATA.ps=text;
		(*R.DATA.mapVal)["name"]=V;
		return R;
		}
	if(type!=4){
		//CVARIANT*X;
		int ifree=0;
		//ZapuskTree(NULL,X,ifree);
		//(*R.DATA.mapVal)[S]=*X;
		//delete X;
		return R;
		}
	*V.DATA.ps=text;
	(*R.DATA.mapVal)["value"]=V;
	return R;
}



CVARIANT Prefix::Analysis(){
	CVARIANT R,V;
	R.avtoSet("map");
	V.avtoSet("int");
	V.DATA.intVal=1;
	(*R.DATA.mapVal)["vid"]=V;
	(*R.DATA.mapVal)["A"]=X->Analysis();
	V.avtoSet("string");
	*V.DATA.ps=m[n];
	(*R.DATA.mapVal)["operator"]=V;
	*V.DATA.ps="prefix";
	(*R.DATA.mapVal)["is"]=V;
	return R;
}



CVARIANT Base2::Analysis(){
	CVARIANT R,V;
	R.avtoSet("map");
	V.avtoSet("int");
	V.DATA.intVal=2;
	(*R.DATA.mapVal)["vid"]=V;
	(*R.DATA.mapVal)["A"]=A->Analysis();
	(*R.DATA.mapVal)["B"]=B->Analysis();
	V.avtoSet("string");
	*V.DATA.ps=m[n];
	(*R.DATA.mapVal)["operator"]=V;
	return R;
}



CVARIANT Base3::Analysis(){
	CVARIANT R,V;
	R.avtoSet("map");
	V.avtoSet("int");
	V.DATA.intVal=3;
	(*R.DATA.mapVal)["vid"]=V;
	(*R.DATA.mapVal)["A"]=A->Analysis();
	(*R.DATA.mapVal)["B"]=B->Analysis();
	(*R.DATA.mapVal)["V"]=this->V->Analysis();
	V.avtoSet("string");
	*V.DATA.ps=m[n];
	(*R.DATA.mapVal)["operator"]=V;
	return R;
}



CVARIANT Sufix::Analysis(){
	CVARIANT R,V;
	R.avtoSet("map");
	V.avtoSet("int");
	V.DATA.intVal=1;
	(*R.DATA.mapVal)["vid"]=V;
	(*R.DATA.mapVal)["A"]=X->Analysis();
	V.avtoSet("string");
	*V.DATA.ps=m[n];
	(*R.DATA.mapVal)["operator"]=V;
	*V.DATA.ps="sufix";
	(*R.DATA.mapVal)["is"]=V;
	return R;
}



CVARIANT CallFunc::Analysis(){
	CVARIANT R,V;
	R.avtoSet("map");
	V.avtoSet("string");
	*V.DATA.ps="function";
	(*R.DATA.mapVal)["vid"]=V;
	Base*B=dynamic_cast<Base*>(X);
	if(B)*V.DATA.ps=B->text;
	Base2*B2=dynamic_cast<Base2*>(X);
	if(B2){
		string s;
		B=dynamic_cast<Base*>(B2->A);
		if(B)s=B->text;
		s+="::";
		B=dynamic_cast<Base*>(B2->B);
		if(B)s+=B->text;
		*V.DATA.ps=s;
		}
	(*R.DATA.mapVal)["name"]=V;
	V.avtoSet("vector");
	int i;
	for(i=0;i<params.size();++i){
		CVARIANT*X = new CVARIANT();
		*X = params[i]->Analysis();
		V.DATA.vectorVal->push_back(X);
		}
	(*R.DATA.mapVal)["args"]=V;
	return R;
}



CVARIANT AccesMasiv::Analysis(){
	CVARIANT R,V;
	R.avtoSet("map");
	V.avtoSet("string");
	*V.DATA.ps="[]";
	(*R.DATA.mapVal)["vid"]=V;
	V=X->Analysis();
	(*R.DATA.mapVal)["A"]=V;
	V=P->Analysis();
	(*R.DATA.mapVal)["arg"]=V;
	return R;
}



CVARIANT Cast::Analysis(){
	CVARIANT R,V;
	R.avtoSet("map");
	V.avtoSet("string");
	*V.DATA.ps="cast";
	(*R.DATA.mapVal)["vid"]=V;
	V=X->Analysis();
	(*R.DATA.mapVal)["A"]=V;
	V=cast.Analysis();
	(*R.DATA.mapVal)["cast"]=V;
	return R;
}



CVARIANT Type::Analysis(){
	CVARIANT R,V;
	R.avtoSet("map");
	V.avtoSet("string");
	*V.DATA.ps=name;
	(*R.DATA.mapVal)["name"]=V;
	V.avtoSet("int");
	V.DATA.intVal=n;
	(*R.DATA.mapVal)["n"]=V;
	return R;
}



CVARIANT AlgoSet::Analysis(){
	CVARIANT R,V;
	R.avtoSet("map");
	V.avtoSet("string");
	*V.DATA.ps="{}";
	(*R.DATA.mapVal)["vid"]=V;
	V.avtoSet("vector");
	int i;
	for(i=0;i<nabor.size();++i){
		CVARIANT*X = new CVARIANT();
		*X = nabor[i]->Analysis();
		V.DATA.vectorVal->push_back(X);
		}
	(*R.DATA.mapVal)["args"]=V;
	return R;
}



CVARIANT AComposition::Analysis(){
	CVARIANT R,V;
	R.avtoSet("map");
	V.avtoSet("string");
	*V.DATA.ps="table";
	(*R.DATA.mapVal)["vid"]=V;
	V.avtoSet("map");
	M_SA::iterator it=table.begin();
	for(;it!=table.end();++it){
		(*V.DATA.mapVal)[it->first]=it->second->Analysis();
		}
	(*R.DATA.mapVal)["args"]=V;
	return R;
}


CVARIANT Label::Analysis(){
	CVARIANT R,V;
	V.avtoSet("string");
	*V.DATA.ps="label";
	(*R.DATA.mapVal)["vid"]=V;
	V.avtoSet("int");
	V.DATA.intVal=id;
	(*R.DATA.mapVal)["id"]=V;
	return R;
}



CVARIANT Sequence::Analysis(){
	CVARIANT R,V;
	R.avtoSet("map");
	V.avtoSet("string");
	*V.DATA.ps="sequence";
	(*R.DATA.mapVal)["vid"]=V;
	return R;
}



CVARIANT CreateVar::Analysis(){
	CVARIANT R,V,X;
	R.avtoSet("map");
	V.avtoSet("string");
	*V.DATA.ps="var";
	(*R.DATA.mapVal)["vid"]=V;
	*V.DATA.ps=name;
	(*R.DATA.mapVal)["name"]=V;
	(*R.DATA.mapVal)["type"]=tip.Analysis();
	if(Init){
		(*R.DATA.mapVal)["init"]=Init->Analysis();
		}
	if(isMassiv){
		V.avtoSet("vector");
		int i;
		for(i=0;i<params.size();++i){
			if(params[i])X=params[i]->Analysis();else{
				X.avtoSet("int");
				X.DATA.intVal=0;
				}
			CVARIANT*XX = new CVARIANT();
			*XX = X;
			V.DATA.vectorVal->push_back(XX);
			}
		(*R.DATA.mapVal)["size"]=V;
		}
	return R;
}




CVARIANT SpecSumbol::Analysis(){
	CVARIANT R,V;
	R.avtoSet("map");
	V.avtoSet("string");
	*V.DATA.ps=m[n];
	(*R.DATA.mapVal)["vid"]=V;
	return R;
}



CVARIANT SpecSumbol2::Analysis(){
	CVARIANT R,V;
	R.avtoSet("map");
	V.avtoSet("string");
	*V.DATA.ps=m[n];
	(*R.DATA.mapVal)["vid"]=V;
	(*R.DATA.mapVal)["A"]=X->Analysis();
	return R;
}



CVARIANT IF::Analysis(){
	CVARIANT R,V;
	R.avtoSet("map");
	V.avtoSet("string");
	*V.DATA.ps="if";
	(*R.DATA.mapVal)["vid"]=V;
	(*R.DATA.mapVal)["A"]=X->Analysis();
	return R;
}



CVARIANT WHILE::Analysis(){
	CVARIANT R,V;
	R.avtoSet("map");
	V.avtoSet("string");
	*V.DATA.ps=(n==1?"while":"do");
	(*R.DATA.mapVal)["vid"]=V;
	(*R.DATA.mapVal)["A"]=X->Analysis();
	return R;
}



CVARIANT FOR::Analysis(){
	CVARIANT R,V;
	R.avtoSet("map");
	V.avtoSet("string");
	*V.DATA.ps="for";
	(*R.DATA.mapVal)["vid"]=V;
	(*R.DATA.mapVal)["A"]=X->Analysis();
	return R;
}



//--------------------------------------------------------------------------------------------------
int Algorithm::getSub(V_I*,int,Algorithm**&){return 0;}

int BaseFunction::getSub(V_I*maps,int n,Algorithm**&W){
	int r=1;
	if(!maps)return r;
	if(maps->size()<=n)return r;
	int t=(*maps)[n];
	++n;
	if(t>=r)return r;
	W=&Body;
	r=(*W)->getSub(maps,n,W);
	return r;
}


int Base::getSub(V_I*maps,int n,Algorithm**&A){return 0;}

int Base2::getSub(V_I*maps,int n,Algorithm**&W){
	int r=2;
	if(!maps)return r;
	if(maps->size()<=n)return r;
	int t=(*maps)[n];
	++n;
	if(t>=r)return r;
	if(t==0)W=&A;
	if(t==1)W=&B;
	r=(*W)->getSub(maps,n,W);
	return r;
}


int Base3::getSub(V_I*maps,int n,Algorithm**&W){
	int r=3;
	if(!maps)return r;
	if(maps->size()<=n)return r;
	int t=(*maps)[n];
	++n;
	if(t>=r)return r;
	if(t==0)W=&A;
	if(t==1)W=&V;
	if(t==2)W=&B;
	r=(*W)->getSub(maps,n,W);
	return r;
}


int Prefix::getSub(V_I*maps,int n,Algorithm**&W){
	int r=1;
	if(!maps)return r;
	if(maps->size()<=n)return r;
	int t=(*maps)[n];
	++n;
	if(t>=r)return r;
	if(t==0)W=&X;
	r=(*W)->getSub(maps,n,W);
	return r;
}


int Sufix::getSub(V_I*maps,int n,Algorithm**&W){
	int r=1;
	if(!maps)return r;
	if(maps->size()<=n)return r;
	int t=(*maps)[n];
	++n;
	if(t>=r)return r;
	if(t==0)W=&X;
	r=(*W)->getSub(maps,n,W);
	return r;
}


int CallFunc::getSub(V_I*maps,int n,Algorithm**&W){
	int r=params.size()+1;
	if(!maps)return r;
	if(maps->size()<=n)return r;
	int t=(*maps)[n];
	++n;
	if(t>=r)return r;
	if(t==0)W=&X;else W=&params[t-1];
	r=(*W)->getSub(maps,n,W);
	return r;
}


int AccesMasiv::getSub(V_I*maps,int n,Algorithm**&W){
	int r=2;
	if(!maps)return r;
	if(maps->size()<=n)return r;
	int t=(*maps)[n];
	++n;
	if(t>=r)return r;
	if(t==0)W=&X;
	if(t==1)W=&P;
	r=(*W)->getSub(maps,n,W);
	return r;
}


int Cast::getSub(V_I*maps,int n,Algorithm**&W){
	int r=1;
	if(!maps)return r;
	if(maps->size()<=n)return r;
	int t=(*maps)[n];
	++n;
	if(t>=r)return r;
	if(t==0)W=&X;
	r=(*W)->getSub(maps,n,W);
	return r;
}


int Interval::getSub(V_I*maps,int n,Algorithm**&W){
	int r=2;
	if(!maps)return r;
	if(maps->size()<=n)return r;
	int t=(*maps)[n];
	++n;
	if(t>=r)return r;
	if(t==0)W=&A;
	if(t==1)W=&B;
	r=(*W)->getSub(maps,n,W);
	return r;
}


int AlgoSet::getSub(V_I*maps,int n,Algorithm**&W){
	int r=nabor.size();
	if(!maps)return r;
	if(maps->size()<=n)return r;
	int t=(*maps)[n];
	++n;
	if(t>=r)return r;
	W=&nabor[t];
	r=(*W)->getSub(maps,n,W);
	return r;
}


int AComposition::getSub(V_I*maps,int n,Algorithm**&W){
	int r=table.size();
	if(!maps)return r;
	if(maps->size()<=n)return r;
	int t=(*maps)[n];
	++n;
	if(t>=r)return r;
	M_SA::iterator it=table.begin();
	int i;
	for(i=0;i<t;++it,++i);
	W=&it->second;
	r=(*W)->getSub(maps,n,W);
	return r;
}


int Label::getSub(V_I*maps,int n,Algorithm**&W){return 0;}


int Sequence::getSub(V_I*maps,int n,Algorithm**&W){
	int r=nabor.size();
	if(!maps)return r;
	if(maps->size()<=n)return r;
	int t=(*maps)[n];
	++n;
	if(t>=r)return r;
	L_AL::iterator it=nabor.begin();
	int i;
	for(i=0;i<t;++it)++i;
	W=&*it;
	r=(*W)->getSub(maps,n,W);
	return r;
}


int CreateVar::getSub(V_I*maps,int n,Algorithm**&W){
	int r=Init?1:0;
	if(!maps)return r;
	if(maps->size()<=n)return r;
	int t=(*maps)[n];
	++n;
	if(t>=r)return r;
	if(t==0)W=&Init;
	r=(*W)->getSub(maps,n,W);
	return r;
}


int SpecSumbol::getSub(V_I*maps,int n,Algorithm**&W){return 0;}

int SpecSumbol2::getSub(V_I*maps,int n,Algorithm**&W){
	int r=1;
	if(!maps)return r;
	if(maps->size()<=n)return r;
	int t=(*maps)[n];
	++n;
	if(t>=r)return r;
	if(t==0)W=&X;
	r=(*W)->getSub(maps,n,W);
	return r;
}


int IF::getSub(V_I*maps,int n,Algorithm**&W){
	int r=3;
	if(!maps)return r;
	if(maps->size()<=n)return r;
	int t=(*maps)[n];
	++n;
	if(t>=r)return r;
	if(t==0)W=&X;
	if(t==1)W=&A;
	if(t==2)W=&E;
	if(*W)r=(*W)->getSub(maps,n,W);else r=0;
	return r;
}


int WHILE::getSub(V_I*maps,int n,Algorithm**&W){
	int r=2;
	if(!maps)return r;
	if(maps->size()<=n)return r;
	int t=(*maps)[n];
	++n;
	if(t>=r)return r;
	if(t==0)W=&X;
	if(t==1)W=&A;
	if(*W)r=(*W)->getSub(maps,n,W);else r=0;
	return r;
}


int FOR::getSub(V_I*maps,int n,Algorithm**&W){
	int r=3;
	if(!maps)return r;
	if(maps->size()<=n)return r;
	int t=(*maps)[n];
	++n;
	if(t>=r)return r;
	if(t==0)W=&X;
	if(t==1)W=&P;
	if(t==2)W=&B;
	if(*W)r=(*W)->getSub(maps,n,W);else r=0;
	return r;
}



//--------------------------------------------------------------------------------------------------
int Algorithm::Power(int n) const{return 0;}


int BaseFunction::Power(int n) const {return Body->Power(n)+10;}

int Base::Power(int n) const{
	int nn=1;
	if(!n)if(type==3)if((int)text.find(".")>=0)++nn;
	return nn;
}

int Prefix::Power(int n) const{return X->Power(n)+1;}

int Sufix::Power(int n) const{return X->Power(n)+1;}

int CallFunc::Power(int n) const{
	int nn=X->Power(n)+1;
	int i;
	for(i=0;i<params.size();++i)nn+=params[i]->Power(n);
	return nn;
}

int AccesMasiv::Power(int n) const{return P->Power(n)+1;}

int Base2::Power(int n) const{return A->Power(n)+B->Power(n)+1;}

int Base3::Power(int n) const{return V->Power(n)+A->Power(n)+B->Power(n)+1;}

int Interval::Power(int n) const{return A->Power(n)+B->Power(n)+1;}

int AlgoSet::Power(int n) const{
	int nn=1;
	int i;
	for(i=0;i<nabor.size();++i)nn+=nabor[i]->Power(n);
	return nn;
}

int AComposition::Power(int n) const{
	int nn=1;
	M_SA::const_iterator it=table.begin();
	for(;it!=table.end();++it)nn+=it->second->Power(n);
	return nn;
}

int Label::Power(int n) const{return 1;}

int Sequence::Power(int n) const{
	int nn=1;
	L_AL::const_iterator it=nabor.begin();
	for(;it!=nabor.end();++it)nn+=(*it)->Power(n);
	return nn;
}

int CreateVar::Power(int n) const{return Init->Power(n)+1;}

int SpecSumbol::Power(int n) const{return 1;}

int SpecSumbol2::Power(int n) const{return X->Power(n)+1;}

int IF::Power(int n) const{
	int nn=X->Power(n)+1;
	if(A)nn+=A->Power(n);
	if(E)nn+=E->Power(n);
	return nn;
}

int WHILE::Power(int n) const{
	int nn=X->Power(n)+1;
	if(A)nn+=A->Power(n);
	return nn;
}


int FOR::Power(int n) const{
	int nn=X->Power(n)+1;
	if(B)nn+=B->Power(n);
	if(P)nn+=P->Power(n);
	return nn;
}



//--------------------------------------------------------------------------------------------------
int Algorithm::PowerW() const {
	if(dynamic_cast<const Base*>(this))return 1;
	if(dynamic_cast<const Base2*>(this))return 2;
	if(dynamic_cast<const Base3*>(this))return 3;
	if(dynamic_cast<const Prefix*>(this))return 4;
	if(dynamic_cast<const Sufix*>(this))return 5;
	if(dynamic_cast<const CallFunc*>(this))return 6;
	if(dynamic_cast<const AccesMasiv*>(this))return 7;
	if(dynamic_cast<const Cast*>(this))return 8;
	if(dynamic_cast<const Interval*>(this))return 9;
	if(dynamic_cast<const AlgoSet*>(this))return 10;
	if(dynamic_cast<const AComposition*>(this))return 11;
	if(dynamic_cast<const Label*>(this))return 12;
	if(dynamic_cast<const Sequence*>(this))return 13;
	if(dynamic_cast<const CreateVar*>(this))return 14;
	if(dynamic_cast<const SpecSumbol*>(this))return 15;
	if(dynamic_cast<const SpecSumbol2*>(this))return 16;
	if(dynamic_cast<const IF*>(this))return 17;
	if(dynamic_cast<const WHILE*>(this))return 18;
	if(dynamic_cast<const FOR*>(this))return 19;
	if(dynamic_cast<const BaseFunction*>(this))return 20;
	return 100;
}



//--------------------------------------------------------------------------------------------------
bool Algorithm:: operator > (const Algorithm&) const {return 1;}
bool Algorithm:: operator < (const Algorithm&) const {return 0;}


bool BaseFunction:: operator > (const Algorithm&Z) const {
	const BaseFunction*BF=dynamic_cast<const BaseFunction*>(&Z);
	if(argumentsON != BF->argumentsON)return (argumentsON && !BF->argumentsON);
	if(argumentsNames.size() != BF->argumentsNames.size())return argumentsNames.size() > BF->argumentsNames.size();
	return Body > BF->Body;
}

bool BaseFunction:: operator < (const Algorithm&Z) const {
	const BaseFunction*BF=dynamic_cast<const BaseFunction*>(&Z);
	if(argumentsON != BF->argumentsON)return (!argumentsON && BF->argumentsON);
	if(argumentsNames.size() != BF->argumentsNames.size())return argumentsNames.size() < BF->argumentsNames.size();
	return Body < BF->Body;
}


bool Base:: operator > (const Algorithm&Z) const {
	const Base*Q=dynamic_cast<const Base*>(&Z);
	if(!Q)return PowerW()>Z.PowerW();
	if(Q->type!=type)return type>Q->type;
	return text>Q->text;
}


bool Base2:: operator > (const Algorithm&Z) const {
	const Base2*Q=dynamic_cast<const Base2*>(&Z);
	if(!Q)return PowerW()>Z.PowerW();
	if(n!=Q->n)return n>Q->n;
	if(!(*A==*Q->A))return *A>*Q->A;
	return *B>*Q->B;
}


bool Base3:: operator > (const Algorithm&Z) const {
	const Base3*Q=dynamic_cast<const Base3*>(&Z);
	if(!Q)return PowerW()>Z.PowerW();
	if(n!=Q->n)return n>Q->n;
	if(!(*A==*Q->A))return *A>*Q->A;
	if(!(*V==*Q->V))return *V>*Q->V;
	return *B>*Q->B;
}


bool Prefix:: operator > (const Algorithm&Z) const {
	const Prefix*Q=dynamic_cast<const Prefix*>(&Z);
	if(!Q)return PowerW()>Z.PowerW();
	if(n!=Q->n)return n>Q->n;
	return *X>*Q->X;
}


bool Sufix:: operator > (const Algorithm&Z) const {
	const Sufix*Q=dynamic_cast<const Sufix*>(&Z);
	if(!Q)return PowerW()>Z.PowerW();
	if(n!=Q->n)return n>Q->n;
	return *X>*Q->X;
}


bool CallFunc:: operator > (const Algorithm&Z) const {
	const CallFunc*Q=dynamic_cast<const CallFunc*>(&Z);
	if(!Q)return PowerW()>Z.PowerW();
	if(n!=Q->n)return n>Q->n;
	int t=params.size();
	if(t!=Q->params.size())return t>Q->params.size();
	int i;
	for(i=0;i<t;++i)
		if(!(*params[i]==*Q->params[i]))return *params[i]>*Q->params[i];
	return *X>*Q->X;
}


bool AccesMasiv:: operator > (const Algorithm&Z) const {
	const AccesMasiv*Q=dynamic_cast<const AccesMasiv*>(&Z);
	if(!Q)return PowerW()>Z.PowerW();
	if(n!=Q->n)return n>Q->n;
	if(!(*P==*Q->P))return *P>*Q->P;
	return *X>*Q->X;
}


bool Cast:: operator > (const Algorithm&Z) const {
	const Cast*Q=dynamic_cast<const Cast*>(&Z);
	if(!Q)return PowerW()>Z.PowerW();
	if(n!=Q->n)return n>Q->n;
	if(!(cast==Q->cast))return cast>Q->cast;
	return *X>*Q->X;
}


bool Interval:: operator > (const Algorithm&Z) const {
	const Interval*Q=dynamic_cast<const Interval*>(&Z);
	if(!Q)return PowerW()>Z.PowerW();
	if(a!=Q->a)return a>Q->a;
	if(b!=Q->b)return b>Q->b;
	if(!(*A==*Q->A))return *A>*Q->A;
	return *B>*Q->B;
}


bool AlgoSet:: operator > (const Algorithm&Z) const {
	const AlgoSet*Q=dynamic_cast<const AlgoSet*>(&Z);
	if(!Q)return PowerW()>Z.PowerW();
	int t=nabor.size();
	if(t!=Q->nabor.size())return t>Q->nabor.size();
	int i;
	for(i=0;i<t;++i)
		if(!(*nabor[i]==*Q->nabor[i]))return *nabor[i]>*Q->nabor[i];
	return 0;
}


bool AComposition:: operator > (const Algorithm&Z) const {
	const AComposition*Q=dynamic_cast<const AComposition*>(&Z);
	if(!Q)return PowerW()>Z.PowerW();
	if(table.size()!=Q->table.size())return table.size()>Q->table.size();
	M_SA::const_iterator it=table.begin(),it2=Q->table.begin();
	for(;it!=table.end();++it,++it2){
		if(it->first!=it2->first)return it->first>it2->first;
		if(!(*it->second==*it2->second))return *it->second>*it2->second;
		}
	return 0;
}


bool Label:: operator > (const Algorithm&Z) const {
	const Label*Q=dynamic_cast<const Label*>(&Z);
	if(!Q)return PowerW()>Z.PowerW();
	return id>Q->id;
}


bool Sequence:: operator > (const Algorithm&Z) const {
	const Sequence*Q=dynamic_cast<const Sequence*>(&Z);
	if(!Q)return PowerW()>Z.PowerW();
	int t=nabor.size();
	if(t!=Q->nabor.size())return t>Q->nabor.size();
	L_AL::const_iterator it=nabor.begin(), it2=Q->nabor.begin();
	for(;it!=nabor.end();++it,++it2)
		if(!(*it==*it2))return *it>*it2;
	return 0;
}


bool CreateVar:: operator > (const Algorithm&Z) const {
	const CreateVar*Q=dynamic_cast<const CreateVar*>(&Z);
	if(!Q)return PowerW()>Z.PowerW();
	if(tip!=Q->tip)return tip>Q->tip;
	if(name!=Q->name)return name>Q->name;
	return *Init>*Q->Init;
}


bool SpecSumbol:: operator > (const Algorithm&Z) const {
	const SpecSumbol*Q=dynamic_cast<const SpecSumbol*>(&Z);
	if(!Q)return PowerW()>Z.PowerW();
	return n>Q->n;
}


bool SpecSumbol2:: operator > (const Algorithm&Z) const {
	const SpecSumbol2*Q=dynamic_cast<const SpecSumbol2*>(&Z);
	if(!Q)return PowerW()>Z.PowerW();
	if(!(*X==*Q->X))return *X>*Q->X;
	return n>Q->n;
}


bool IF:: operator > (const Algorithm&Z) const {
	const IF*Q=dynamic_cast<const IF*>(&Z);
	if(!Q)return PowerW()>Z.PowerW();
	if(!(A&&Q->A))return 1;
	if(!(*A==*Q->A))return *A>*Q->A;
	if(!(E&&Q->E))return 1;
	if(!(*E==*Q->E))return *E>*Q->E;
	return *X>*Q->X;
}


bool WHILE:: operator > (const Algorithm&Z) const {
	const WHILE*Q=dynamic_cast<const WHILE*>(&Z);
	if(!Q)return PowerW()>Z.PowerW();
	if(!(A&&Q->A))return 1;
	if(!(*A==*Q->A))return *A>*Q->A;
	return *X>*Q->X;
}


bool FOR:: operator > (const Algorithm&Z) const {
	const FOR*Q=dynamic_cast<const FOR*>(&Z);
	if(!Q)return PowerW()>Z.PowerW();
	if(!(B&&Q->B))return 1;
	if(!(*B==*Q->B))return *B>*Q->B;
	if(!(P&&Q->P))return 1;
	if(!(*P==*Q->P))return *P>*Q->P;
	return *X>*Q->X;
}




//--------------------------------------------------------------------------------------------------
bool Base:: operator < (const Algorithm&Z) const {
	const Base*Q=dynamic_cast<const Base*>(&Z);
	if(!Q)return PowerW()<Z.PowerW();
	if(Q->type!=type)return type<Q->type;
	return text<Q->text;
}


bool Base2:: operator < (const Algorithm&Z) const {
	const Base2*Q=dynamic_cast<const Base2*>(&Z);
	if(!Q)return PowerW()<Z.PowerW();
	if(n!=Q->n)return n<Q->n;
	if(!(*A==*Q->A))return *A<*Q->A;
	return *B<*Q->B;
}


bool Base3:: operator < (const Algorithm&Z) const {
	const Base3*Q=dynamic_cast<const Base3*>(&Z);
	if(!Q)return PowerW()<Z.PowerW();
	if(n!=Q->n)return n>Q->n;
	if(!(*A==*Q->A))return *A<*Q->A;
	if(!(*V==*Q->V))return *V<*Q->V;
	return *B<*Q->B;
}


bool Prefix:: operator < (const Algorithm&Z) const {
	const Prefix*Q=dynamic_cast<const Prefix*>(&Z);
	if(!Q)return PowerW()<Z.PowerW();
	if(n!=Q->n)return n<Q->n;
	return *X<*Q->X;
}


bool Sufix:: operator < (const Algorithm&Z) const {
	const Sufix*Q=dynamic_cast<const Sufix*>(&Z);
	if(!Q)return PowerW()<Z.PowerW();
	if(n!=Q->n)return n<Q->n;
	return *X<*Q->X;
}


bool CallFunc:: operator < (const Algorithm&Z) const {
	const CallFunc*Q=dynamic_cast<const CallFunc*>(&Z);
	if(!Q)return PowerW()<Z.PowerW();
	if(n!=Q->n)return n<Q->n;
	int t=params.size();
	if(t!=Q->params.size())return t<Q->params.size();
	int i;
	for(i=0;i<t;++i)
		if(!(*params[i]==*Q->params[i]))return *params[i]<*Q->params[i];
	return *X<*Q->X;
}


bool AccesMasiv:: operator < (const Algorithm&Z) const {
	const AccesMasiv*Q=dynamic_cast<const AccesMasiv*>(&Z);
	if(!Q)return PowerW()<Z.PowerW();
	if(n!=Q->n)return n<Q->n;
	if(!(*P==*Q->P))return *P<*Q->P;
	return *X<*Q->X;
}


bool Cast:: operator < (const Algorithm&Z) const {
	const Cast*Q=dynamic_cast<const Cast*>(&Z);
	if(!Q)return PowerW()<Z.PowerW();
	if(n!=Q->n)return n<Q->n;
	if(!(cast==Q->cast))return cast<Q->cast;
	return *X<*Q->X;
}


bool Interval:: operator < (const Algorithm&Z) const {
	const Interval*Q=dynamic_cast<const Interval*>(&Z);
	if(!Q)return PowerW()<Z.PowerW();
	if(a!=Q->a)return a<Q->a;
	if(b!=Q->b)return b<Q->b;
	if(!(*A==*Q->A))return *A<*Q->A;
	return *B<*Q->B;
}


bool AlgoSet:: operator < (const Algorithm&Z) const {
	const AlgoSet*Q=dynamic_cast<const AlgoSet*>(&Z);
	if(!Q)return PowerW()<Z.PowerW();
	int t=nabor.size();
	if(t!=Q->nabor.size())return t<Q->nabor.size();
	int i;
	for(i=0;i<t;++i)
		if(!(*nabor[i]==*Q->nabor[i]))return *nabor[i]<*Q->nabor[i];
	return 0;
}


bool AComposition:: operator < (const Algorithm&Z) const {
	const AComposition*Q=dynamic_cast<const AComposition*>(&Z);
	if(!Q)return PowerW()<Z.PowerW();
	if(table.size()!=Q->table.size())return table.size()<Q->table.size();
	M_SA::const_iterator it=table.begin(),it2=Q->table.begin();
	for(;it!=table.end();++it,++it2){
		if(it->first!=it2->first)return it->first<it2->first;
		if(!(*it->second==*it2->second))return *it->second<*it2->second;
		}
	return 0;
}


bool Label:: operator < (const Algorithm&Z) const {
	const Label*Q=dynamic_cast<const Label*>(&Z);
	if(!Q)return PowerW()<Z.PowerW();
	return id<Q->id;
}


bool Sequence:: operator < (const Algorithm&Z) const {
	const Sequence*Q=dynamic_cast<const Sequence*>(&Z);
	if(!Q)return PowerW()<Z.PowerW();
	int t=nabor.size();
	if(t!=Q->nabor.size())return t<Q->nabor.size();
	L_AL::const_iterator it=nabor.begin(), it2=Q->nabor.begin();
	for(;it!=nabor.end();++it,++it2)
		if(!(*it==*it2))return *it<*it2;
	return 0;
}


bool CreateVar:: operator < (const Algorithm&Z) const {
	const CreateVar*Q=dynamic_cast<const CreateVar*>(&Z);
	if(!Q)return PowerW()<Z.PowerW();
	if(tip!=Q->tip)return tip<Q->tip;
	if(name!=Q->name)return name<Q->name;
	return *Init<*Q->Init;
}


bool SpecSumbol:: operator < (const Algorithm&Z) const {
	const SpecSumbol*Q=dynamic_cast<const SpecSumbol*>(&Z);
	if(!Q)return PowerW()<Z.PowerW();
	return n<Q->n;
}


bool SpecSumbol2:: operator < (const Algorithm&Z) const {
	const SpecSumbol2*Q=dynamic_cast<const SpecSumbol2*>(&Z);
	if(!Q)return PowerW()<Z.PowerW();
	if(!(*X==*Q->X))return *X<*Q->X;
	return n<Q->n;
}


bool IF:: operator < (const Algorithm&Z) const {
	const IF*Q=dynamic_cast<const IF*>(&Z);
	if(!Q)return PowerW()<Z.PowerW();
	if(!(A&&Q->A))return 0;
	if(!(*A==*Q->A))return *A<*Q->A;
	if(!(E&&Q->E))return 0;
	if(!(*E==*Q->E))return *E<*Q->E;
	return *X<*Q->X;
}


bool WHILE:: operator < (const Algorithm&Z) const {
	const WHILE*Q=dynamic_cast<const WHILE*>(&Z);
	if(!Q)return PowerW()<Z.PowerW();
	if(!(A&&Q->A))return 0;
	if(!(*A==*Q->A))return *A<*Q->A;
	return *X<*Q->X;
}


bool FOR:: operator < (const Algorithm&Z) const {
	const FOR*Q=dynamic_cast<const FOR*>(&Z);
	if(!Q)return PowerW()<Z.PowerW();
	if(!(B&&Q->B))return 0;
	if(!(*B==*Q->B))return *B<*Q->B;
	if(!(P&&Q->P))return 0;
	if(!(*P==*Q->P))return *P<*Q->P;
	return *X<*Q->X;
}





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
Label
Sequence
CreateVar
SpecSumbol
SpecSumbol2
IF
WHILE
FOR
*/



// analysis.cpp	:-|