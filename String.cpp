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
CVARIANT* TrioToVector(const int a, const int b, const int c){
	CVARIANT *X, N;
	N.avtoSet("int");
	X = new CVARIANT();
	X->avtoSet("vector");
	N.DATA.intVal = a; // Begin;
	X->DATA.vectorVal->push_back(N.copy());
	N.DATA.intVal = b; // End
	X->DATA.vectorVal->push_back(N.copy());
	N.DATA.intVal = c; // CodeStatus NeedAnalize
	X->DATA.vectorVal->push_back(N.copy());
	return X;
}




CVARIANT* SeeKomentary(const char *text){
	CVARIANT *Row, N;
	Row = new CVARIANT();
	Row->avtoSet("vector");
	N.avtoSet("int");
	int ww = 1, vv = 1; // ww - "    vv - '
	int Begin = 0, pos = -1;
	int i,j;
	bool isPrevSlesh = 0;
	for(i=0;text[i];++i){
		if(ww&&vv&&text[i]=='/')if(text[i+1]=='/'){
			if(Begin<i)Row->DATA.vectorVal->push_back(TrioToVector(Begin,i,0)); // 0: NeedAnalize
			Begin = i;
			for(j=2;text[i+j]&&text[i+j]!=10;++j);
			i += j;
			if(!text[i])--i;
			++i;
			Row->DATA.vectorVal->push_back(TrioToVector(Begin,i,1)); // 1: //...
			Begin = i;
			--i;
			continue;
			}
		if(ww&&vv&&text[i]=='/')if(text[i+1]=='*'){
			if(Begin<i)Row->DATA.vectorVal->push_back(TrioToVector(Begin,i,0)); // 0: NeedAnalize
			Begin = i;
			i += 2;
			for(;text[i];++i){
				if(text[i]=='*' && text[i+1]=='/')break;
				}
			if(text[i])i += 2;
			Row->DATA.vectorVal->push_back(TrioToVector(Begin,i,2)); // 2: /*...*/
			Begin = i;
			--i;
			continue;
			}
		if(!isPrevSlesh){
			j = 0;
			if(vv&&text[i]=='"'){
				ww = 1-ww;
				j = 2;
				}
			if(ww&&text[i]==39 ){
				vv = 1-vv;
				j = 1;
				}
			if(j){
				bool isOpen = (j==1?vv:ww)==0;
				if(isOpen){
					if(Begin<i)Row->DATA.vectorVal->push_back(TrioToVector(Begin,i,0)); // 0: NeedAnalize
					Begin = i;
					}else{
					Row->DATA.vectorVal->push_back(TrioToVector(Begin,i+1,j+2)); // 3: '#'  4: "###"
					Begin = i+1;
					}
				}
			}
		if(text[i]==10){
			ww = 1;
			vv = 1;
			}
		isPrevSlesh = (!isPrevSlesh && (!ww||!vv) && text[i]=='\\');
		}
	if(Begin<i)Row->DATA.vectorVal->push_back(TrioToVector(Begin,i,0)); // 0: NeedAnalize
	return Row;
}




void SeeNumbers(const char *text, CVARIANT&Row){
	int size, i;
	size = Row.DATA.vectorVal->size();
	i = 0;
	CVARIANT* Trio;
	int a, b, c, p, pos;
	char cc, nPsets;
	bool isN, isInN, isE, isMP, isH;
	bool isNeed, isPevAlpha;
	for(;i<size;++i){
		Trio = (*Row.DATA.vectorVal)[i];
		c = (*Trio->DATA.vectorVal)[2]->DATA.intVal;
		if(c)continue;
		a = (*Trio->DATA.vectorVal)[0]->DATA.intVal;
		b = (*Trio->DATA.vectorVal)[1]->DATA.intVal;
		pos = -1;
		isPevAlpha = isNeed = false;
		nPsets = 0;
		for(p=a;p<b;++p){
			cc = text[p];
			isN = (cc>='0' && cc<='9');
			isInN = (cc=='x' || cc=='X' || cc=='b' || cc=='B');
			isMP = isE && (cc=='-' || cc=='+');
			isE = (cc=='e' || cc=='E');
			isH = ((cc>='a' && cc<='f') || (cc>='A' && cc<='F'));
			if(cc=='.'){
				++nPsets;
				isInN = (nPsets<=1);
				}
			if(isN && pos<0 &&!isPevAlpha)pos = p;
			isInN = (isN || isInN || isMP || isE || isH);
			if(!isInN && pos>=0){
				isNeed = true;
				break;
				}
			isPevAlpha = (isalpha(cc)!=0);
			}
		if(p==b && pos>=0)isNeed = true;
		if(isNeed){
			int nc = 5; // Number code
			if(a==pos && p==b){
				(*Trio->DATA.vectorVal)[2]->DATA.intVal = nc;
				continue;
				}
			if(a==pos){
				(*Trio->DATA.vectorVal)[2]->DATA.intVal = nc;
				(*Trio->DATA.vectorVal)[1]->DATA.intVal = p;
				Trio = TrioToVector(p,b,0);
				Row.DATA.vectorVal->insert(Row.DATA.vectorVal->begin()+i+1,Trio);
				++size;
				continue;
				}
			(*Trio->DATA.vectorVal)[1]->DATA.intVal = pos;
			Trio = TrioToVector(pos,p,nc);
			Row.DATA.vectorVal->insert(Row.DATA.vectorVal->begin()+i+1,Trio);
			++size;
			if(p<b){
				Trio = TrioToVector(p,b,0);
				Row.DATA.vectorVal->insert(Row.DATA.vectorVal->begin()+i+2,Trio);
				++size;
				}
			}
		}
}





void SeeReservedWords(const char *text, CVARIANT&Row){
	const char *table[] = {
		"void", "var",
		"vector", "set", "map",
		"char", "int", "float", "double", "bool", "string", "interval",
		"function", "program", "graph", "digit", "module", "pointer",
		NULL
		};
	int size, i;
	size = Row.DATA.vectorVal->size();
	i = 0;
	CVARIANT* Trio;
	int a, b, c, p, pos;
	char cc, nc;
	bool isPsetOK, isAlpha, isPrevVoid = false;
	int j;
	for(;i<size;++i){
		Trio = (*Row.DATA.vectorVal)[i];
		c = (*Trio->DATA.vectorVal)[2]->DATA.intVal;
		if(c)continue;
		a = (*Trio->DATA.vectorVal)[0]->DATA.intVal;
		b = (*Trio->DATA.vectorVal)[1]->DATA.intVal;
		pos = -1;
		nc = 0;
		isPsetOK = false;
		for(p=a;p<b;++p){
			cc = text[p];
			isAlpha = (isalpha(cc)!=0) || (isdigit(cc)!=0);
			if(isAlpha && pos<0)pos = p;
			if(!isAlpha && pos>=0){
				string word;
				word.reserve(p-pos+1);
				for(j = pos;j<p;++j)word += text[j];
				nc = 8;
				if(isPsetOK)nc = 10;
				if(word=="true" || word=="false")nc = 6;
				if(
					word=="return" || word=="if" || word=="else" ||
					word=="while" || word=="do" || word=="for" ||
					word=="continue" || word=="break" ||
					word=="new" || word=="delete"
					)nc = 9;
				if(isPrevVoid)if(word=="include")nc = 9;
				for(j = 0;table[j];++j)if(word==table[j])break;
				if(table[j]){
					nc = 11;
					if(j<5)nc = 12;
					if(j==1)nc = 13;
					if(!j)nc = 14;
					}
				}
			if(isPsetOK && pos<0)isPsetOK = (cc==' ' || cc=='	' || cc=='\n');
			if(cc=='.')isPsetOK = true;
			if(nc){
				isPrevVoid = false;
				break;
				}
			if(cc=='#'){
				nc = 7;
				pos = p;
				++p;
				isPrevVoid = true;
				break;
				}
			}
		if(nc){
			if(a==pos && p==b){
				(*Trio->DATA.vectorVal)[2]->DATA.intVal = nc;
				continue;
				}
			if(a==pos){
				(*Trio->DATA.vectorVal)[2]->DATA.intVal = nc;
				(*Trio->DATA.vectorVal)[1]->DATA.intVal = p;
				Trio = TrioToVector(p,b,0);
				Row.DATA.vectorVal->insert(Row.DATA.vectorVal->begin()+i+1,Trio);
				++size;
				continue;
				}
			(*Trio->DATA.vectorVal)[1]->DATA.intVal = pos;
			Trio = TrioToVector(pos,p,nc);
			Row.DATA.vectorVal->insert(Row.DATA.vectorVal->begin()+i+1,Trio);
			++size;
			if(p<b){
				Trio = TrioToVector(p,b,0);
				Row.DATA.vectorVal->insert(Row.DATA.vectorVal->begin()+i+2,Trio);
				++size;
				}
			}
		}
}




void getCodeVisualColors(CVARIANT*&V, const char *text, bool isToString){
	int a, b, c;
	CVARIANT Row, N, *X;
	Row.avtoSet("vector");
	N.avtoSet("int");
	c = strlen(text); // End
	Row.DATA.vectorVal->push_back(TrioToVector(0,c,0));
	CVARIANT *NRow = SeeKomentary(text);
	if(NRow->DATA.vectorVal->size())Row.DATA.vectorVal->swap(*NRow->DATA.vectorVal);
	delete NRow;
	SeeNumbers(text,Row);
	SeeReservedWords(text,Row);
	V = new CVARIANT();
	if(isToString)V->avtoSet("string"); else {
		V->avtoSet("vector");
		V->DATA.vectorVal->reserve(c);
		}
	int size, i;
	size = Row.DATA.vectorVal->size();
	i = 0;
	char cc;
	string s;
	//const char table[] = {15,1,2,3,4,5,6,7,8,9,10,11,12,13,14};
	for(;i<size;++i){
		X = *(Row.DATA.vectorVal->begin() + i);
		a = (*X->DATA.vectorVal)[0]->DATA.intVal;
		b = (*X->DATA.vectorVal)[1]->DATA.intVal;
		c = (*X->DATA.vectorVal)[2]->DATA.intVal;
		//c = table[c];
		c = (c?c:15);
		N.DATA.intVal = c;
		cc = (c>9?'A'+c-10:'0'+c);
		for(;a<b;++a){
			if(isToString)s += cc; else V->DATA.vectorVal->push_back(N.copy());
			}
		}
	if(isToString)*V->DATA.ps = s;
}






//--------------------------------------------------------------------------------------------------
void CallFunc::forstring(CVARIANT*thisVal,string&name,CVARIANT*&V,V_CVARIANT&VCV){
	if(name=="length"||name=="size"){
		V=new(CVARIANT);
		V->avtoSet("int");
		V->DATA.intVal = thisVal->DATA.ps->size();
		return;
		}
	if(name=="empty"){
		V=new(CVARIANT);
		V->avtoSet("bool");
		V->DATA.boolVal = thisVal->DATA.ps->empty();
		return;
		}
	if(name=="substr")if(VCV.size()>0){
		string s = *thisVal->DATA.ps;
		VCV[0].TransformType("int");
		if(VCV[0].DATA.intVal<0)return;
		if(VCV.size()>1){
			VCV[1].TransformType("int");
			s = s.substr(VCV[0].DATA.intVal,VCV[1].DATA.intVal);
			}else{
			int t = VCV[0].DATA.intVal;
			if(t<s.size())s = s.substr(t); else s = "";
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
		string *a, b;
		a = VCV[0].DATA.ps;
		b = *thisVal->DATA.ps;
		int u;
		CVARIANT *S;
		V = new(CVARIANT);
		V->avtoSet("vector");
		while(1){
			u = b.find(a->c_str());
			if(u<0)break;
			S = new CVARIANT();
			S->avtoSet("string");
			*S->DATA.ps = b.substr(0,u);
			V->DATA.vectorVal->push_back(S);
			b = b.substr(u + a->length());
			}
		S = new CVARIANT();
		S->avtoSet("string");
		*S->DATA.ps = b;
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
		V = new(CVARIANT);
		V->avtoSet("string");
		*V->DATA.ps = "First arg mast be: Example, isValid, Details, ErrorMessage or Parse.";
		return;
		}
	if(name=="Reserve"){
		int nArgs = VCV.size();
		if(!nArgs)return;
		int size;
		VCV[0].TransformType("int");
		size = VCV[0].DATA.intVal;
		if(size<0)return;
		++size;
		char c = '0';
		if(nArgs>=2){
			VCV[1].TransformType("char");
			c = VCV[1].DATA.bVal;
			}
		char *text = new char[size];
		//ZeroMemory(text,size);
		memset(text,c,--size);
		text[size] = 0;
		V = new(CVARIANT);
		V->avtoSet("string");
		*V->DATA.ps = text;
		delete[] text;
		return;
		}
	if(name=="isValide")if(VCV.size()>=1){
		VCV[0].TransformType("string");
		const char *S = thisVal->DATA.ps->c_str();
		const char *VS = VCV[0].DATA.ps->c_str();
		int sizeS, sizeVS;
		sizeS = strlen(S);
		sizeVS = strlen(VS);
		char c;
		int is, ivs;
		is = 0;
		for(;is<sizeS;++is){
			c = S[is];
			ivs = 0;
			for(;ivs<sizeVS;++ivs)if(c==VS[ivs])break;
			if(ivs==sizeVS)break;
			}
		bool isValid = !(is<sizeS);
		V = new(CVARIANT);
		V->avtoSet("bool");
		V->DATA.boolVal = isValid;
		return;
		}
	if(name=="getCodeVisualColors"){
		const char *text = thisVal->DATA.ps->c_str();
		bool isToString = false;
		if(VCV.size()>=1){
			VCV[0].TransformType("bool");
			isToString = VCV[0].DATA.boolVal;
			}
		getCodeVisualColors(V, text, isToString);
		return;
		}
	
}





//	String.cpp	:-|