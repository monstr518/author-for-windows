//	String.cpp

#include "main.h"

#include "RegularExpressions.h"

using namespace RegularExpressions;



//--------------------------------------------------------------------------------------------------
CVARIANT* AdaptRowREtoCVARIANT(RowRE*RRE){
	CVARIANT* Result = new CVARIANT();
	Result->avtoSet("vector");
	int i,size = RRE->Row.size();
	for(i=0;i<size;++i){
		RowRE::OneTermRE*X = RRE->Row[i];
		if(X->isString){
			CVARIANT*A = new CVARIANT();
			A->avtoSet("string");
			string*p = A->DATA.ps;
			A->DATA.ps = X->one.ps;
			X->one.ps = p;
			Result->DATA.vectorVal->push_back(A);
			}else{
			CVARIANT*A = AdaptRowREtoCVARIANT(X->one.pr);
			Result->DATA.vectorVal->push_back(A);
			}
		}
	return Result;
}




//--------------------------------------------------------------------------------------------------
void CallFunc::forstring(CVARIANT*thisVal,string&name,CVARIANT*&V,V_CVARIANT&VCV){
	if(name=="length"||name=="size"){
		V=new(CVARIANT);
		V->avtoSet("int");
		V->DATA.intVal=thisVal->DATA.ps->size();
		return;
		}
	if(name=="substr")if(VCV.size()>0){
		string s=*thisVal->DATA.ps;
		VCV[0].TransformType("int");
		if(VCV[0].DATA.intVal<0)return;
		if(VCV.size()>1){
			VCV[1].TransformType("int");
			s=s.substr(VCV[0].DATA.intVal,VCV[1].DATA.intVal);
			}else{
			int t=VCV[0].DATA.intVal;
			if(t<s.size())s=s.substr(t);else s="";
			}
		V=new(CVARIANT);
		V->avtoSet("string");
		*V->DATA.ps=s;
		return;
		}
	if(name=="find" ||name=="findOf" || name=="indexOf")if(VCV.size()>0){
		string s=*thisVal->DATA.ps;
		VCV[0].TransformType("string");
		int i=s.find(VCV[0].DATA.ps->c_str());
		V=new(CVARIANT);
		V->avtoSet("int");
		V->DATA.intVal=i;
		return;
		}
	if(name=="export"){
		string s=*thisVal->DATA.ps;
		V=new(CVARIANT);
		V->avtoSet("string");
		*V->DATA.ps=SCANER::writeString(s);
		return;
		}
	if(name=="trim"){
		string s=SCANER::trim(*thisVal->DATA.ps);
		V=new(CVARIANT);
		V->avtoSet("string");
		*V->DATA.ps=s;
		return;
		}
	if(name=="replaceAll")if(VCV.size()>1){
		VCV[0].TransformType("string");
		VCV[1].TransformType("string");
		V=new(CVARIANT);
		V->avtoSet("string");
		*V->DATA.ps=SCANER::replaceAll(*thisVal->DATA.ps,*VCV[0].DATA.ps,*VCV[1].DATA.ps);
		return;
		}
	if(name=="rovno")if(VCV.size()>=1){
		VCV[0].TransformType("int");
		V=new(CVARIANT);
		*V=*thisVal;
		int n=VCV[0].DATA.intVal-V->DATA.ps->size();
		int i;
		for(i=0;i<n;++i)*V->DATA.ps+=" ";
		return;
		}
	if(name=="split")if(VCV.size()>0){// "1|2".split("|") => {"1","2"}
		VCV[0].TransformType("string");
		string*a=VCV[0].DATA.ps,*b=thisVal->DATA.ps;
		int u;
		CVARIANT *S;
		V=new(CVARIANT);
		V->avtoSet("vector");
		while(1){
			u=b->find(a->c_str());
			if(u<0)break;
			S = new CVARIANT();
			S->avtoSet("string");
			*S->DATA.ps=b->substr(0,u);
			V->DATA.vectorVal->push_back(S);
			*b=b->substr(u+a->length());
			}
		S = new CVARIANT();
		S->avtoSet("string");
		*S->DATA.ps=*b;
		V->DATA.vectorVal->push_back(S);
		return;
		}
	if(name=="noComments" || name=="deleteComments"){
		string*str=thisVal->DATA.ps;
		LOADER X;
		X.text = LOADER::copy(*str);
		X.delete_komentary();
		*thisVal->DATA.ps = X.text;
		return;
		}
	if(name=="RegularExpression"){
		string* Expression = thisVal->DATA.ps;
		int nArgs = VCV.size();
		string* Command = NULL;
		if(nArgs>0){
			CVARIANT* A = &VCV[0];
			A->TransformType("string");
			Command = A->DATA.ps;
			if(*Command=="Example")nArgs = 0;
			}
		if(!nArgs){
			V=new(CVARIANT);
			V->avtoSet("string");
			*V->DATA.ps = FindersMahine::ExamplesExpression(*Expression);
			return;
			}
		if(*Command=="isValid"){
			FindersMahine FM(Expression->c_str());
			bool ok = FM.Build();
			V=new(CVARIANT);
			V->avtoSet("bool");
			V->DATA.boolVal = ok;
			return;
			}
		if(*Command=="Details"){
			FindersMahine FM(Expression->c_str());
			FM.Build();
			V=new(CVARIANT);
			V->avtoSet("string");
			*V->DATA.ps = FM.toString();
			return;
			}
		if(*Command=="ErrorMessage"){
			FindersMahine FM(Expression->c_str());
			FM.Build();
			V=new(CVARIANT);
			V->avtoSet("string");
			*V->DATA.ps = FM.ErrorMessage;
			return;
			}
		if(*Command=="Parse"){
			if(nArgs<2){
				V=new(CVARIANT);
				V->avtoSet("string");
				*V->DATA.ps = "Need text in last arg: .RegularExpression(\"Parse\", \"TEXT\")";
				return;
				}
			CVARIANT* B = &VCV[1];
			B->TransformType("string");
			string* Data = B->DATA.ps;
			FindersMahine FM(Expression->c_str());
			FM.Build();
			RowRE* RRE = FM.RunParser(Data->c_str());
			if(!RRE){
				V=new(CVARIANT);
				V->avtoSet("void");
				return;
				}
			V = AdaptRowREtoCVARIANT(RRE);
			delete RRE;
			return;
			}
		V=new(CVARIANT);
		V->avtoSet("string");
		*V->DATA.ps = "First arg mast be: Example, isValid, Details, ErrorMessage or Parse.";
		return;
		}
}





//	String.cpp	:-|