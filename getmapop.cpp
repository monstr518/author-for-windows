// getmapop.cpp
#include "main.h"

//--------------------------------------------------------------------------------------------------
/*
CVARIANT* Algorithm::getMapOperator(MAIN*M){return NULL;}

CVARIANT* SpecSumbol::getMapOperator(MAIN*){
	CVARIANT*V=new(CVARIANT);
	V->avtoSet("map");
	CVARIANT S,U;
	S.avtoSet("string");
	U.avtoSet("string");
	*S.DATA.ps="is";
	*U.DATA.ps=m[n];
	(*V->DATA.mapVal)[S]=U;
	return V;
}

CVARIANT* SpecSumbol2::getMapOperator(MAIN*){return NULL;}

CVARIANT* Base::getMapOperator(MAIN*){
	CVARIANT*V=new(CVARIANT);
	V->avtoSet("map");
	CVARIANT S,U;
	S.avtoSet("string");
	U.avtoSet("string");
	*S.DATA.ps="is";
	*U.DATA.ps="Base";
	(*V->DATA.mapVal)[S]=U;
	*S.DATA.ps="text";
	*U.DATA.ps=text;
	(*V->DATA.mapVal)[S]=U;
	U.avtoSet("int");
	*S.DATA.ps="type";
	U.DATA.intVal=type;
	(*V->DATA.mapVal)[S]=U;
	return V;

}
*/

string Algorithm::getFormul(string){return "";}
string SpecSumbol::getFormul(string){return m[n];}
string SpecSumbol2::getFormul(string){return toString();}

string Base::getFormul(string){return toString();}

string CreateVar::getFormul(string s){
	if(s.empty())return toString();
	return Init->getFormul(s.substr(1));
}


string Base2::getFormul(string s){
	if(s.empty())return toString();
	if(s[0]=='0')return A->getFormul(s.substr(1));
	return B->getFormul(s.substr(1));
}



string Prefix::getFormul(string s){
	if(s.empty())return toString();
	return X->getFormul(s.substr(1));
}


string Sufix::getFormul(string s){
	if(s.empty())return toString();
	return X->getFormul(s.substr(1));
}


string CallFunc::getFormul(string s){
	if(s.empty())return toString();
	int i=s[0]-'0';
	if(!i)X->getFormul(s.substr(1));
	--i;
	if(i<0 || i>params.size())return "";
	return params[i]->getFormul(s.substr(1));
}


string AccesMasiv::getFormul(string s){
	if(s.empty())return toString();
	if(s[0]=='0')return X->getFormul(s.substr(1));
	return P->getFormul(s.substr(1));
}


string Cast::getFormul(string s){
	if(s.empty())return toString();
	return X->getFormul(s.substr(1));
}


string Base3::getFormul(string s){
	if(s.empty())return toString();
	if(s[0]=='0')return A->getFormul(s.substr(1));
	if(s[0]=='1')return B->getFormul(s.substr(1));
	return V->getFormul(s.substr(1));
}


string AlgoSet::getFormul(string s){
	if(s.empty())return toString();
	int i=s[0]-'0';
	if(i<0 || i>nabor.size())return "";
	return nabor[i]->getFormul(s.substr(1));
}


//--------------------------------------------------------------------------------------------------
string Algorithm::getReverseFormul(string s){return "";}

string Base::getReverseFormul(string s){return toString();}

string Prefix::getReverseFormul(string s){
	if(s.empty())return (string)m[n]+"#";
	return X->getReverseFormul(s.substr(1));
}


string Sufix::getReverseFormul(string s){
	if(s.empty())return (string)"#"+m[n];
	return X->getReverseFormul(s.substr(1));
}


string CallFunc::getReverseFormul(string s){
	if(s.empty()){
		string r="#(";
		int j;
		for(j=0;j<params.size();++j){
			if(j)r+=",";
			r+="#";
			}
		r+=")";
		return r;
		}
	int i=s[0]-'0';
	if(!i)return X->getReverseFormul(s.substr(1));
	--i;
	if(i<0 || i>params.size())return "";
	return params[i]->getReverseFormul(s.substr(1));
}


string AccesMasiv::getReverseFormul(string s){
	if(s.empty())return "#[#]";
	if(s[0]=='0')return X->getReverseFormul(s.substr(1));
	return P->getReverseFormul(s.substr(1));
}


string Cast::getReverseFormul(string s){
	if(s.empty())return "("+cast.toString()+")#";
	return X->getReverseFormul(s.substr(1));
}


string Base2::getReverseFormul(string s){
	if(s.empty())return (string)"#"+m[n]+"#";
	if(s[0]=='0')return A->getReverseFormul(s.substr(1));
	return B->getReverseFormul(s.substr(1));
}


string Base3::getReverseFormul(string s){
	if(s.empty())return "#?#:#";
	if(s[0]=='0')return A->getReverseFormul(s.substr(1));
	if(s[0]=='1')return B->getReverseFormul(s.substr(1));
	return V->getReverseFormul(s.substr(1));
}


string AlgoSet::getReverseFormul(string s){
	if(s.empty()){
		string r="{";
		int j;
		for(j=0;j<nabor.size();++j){
			if(j)r+=",";
			r+="#";
			}
		r+="}";
		return r;
		}
	int i=s[0]-'0';
	if(i<0 || i>nabor.size())return "";
	return nabor[i]->getReverseFormul(s.substr(1));
}


string CreateVar::getReverseFormul(string s){
	if(s.empty()){
		string r=tip.toString();
		r+=" ";
		r+=name;
		if(Init)r+="=#";
		return r;
		}
	if(!Init)return "";
	return Init->getReverseFormul(s.substr(1));
}


string SpecSumbol::getReverseFormul(string s){return m[n];}


string SpecSumbol2::getReverseFormul(string s){
	if(s.empty())return (string)m[n]+"#";
	return X->getReverseFormul(s.substr(1));
}





// getmapop.cpp	:-|