//	dlls.cpp
#include "main.h"


//--------------------------------------------------------------------------------------------------
Xdll::Xdll(char*namedll){
	dll=LoadLibrary(namedll);
	SendString=(void (*)(char*))GetProcAddress(dll,"SendString");
	RecvString=(char* (*)())GetProcAddress(dll,"RecvString");
}


Xdll::~Xdll(){
	FreeLibrary(dll);
}


void Xdll::Send(char*str){
	(*SendString)(str);
}



string Xdll::Recv(){
	string r;
	r=(*RecvString)();
	return r;
}


bool Xdll::isCorectLoadLibrary(){
	return SendString && RecvString;
}



//--------------------------------------------------------------------------------------------------
ControlerDLL::ControlerDLL(MAIN*M){Main=M;}


ControlerDLL::~ControlerDLL(){
	M_SXDLL::iterator it=dlls.begin();
	for(;it!=dlls.end();++it)delete it->second;
}



void ControlerDLL::Load(string s){
	M_CVARIANT&GM=*Main->GlobalSpace.Map.DATA.mapVal;
	CVARIANT A;
	A.avtoSet("string");
	*A.DATA.ps="WAY";
	A=GM[A];
	string w=*A.DATA.ps+"\\SYS\\DLLS\\"+s;
	//cout<<w.c_str()<<endl;

	string common;
	common=*A.DATA.ps+"\\SYS\\DLLS\\";
	chdir(common.c_str());

	Xdll*X=new(Xdll)(const_cast<char*>(w.c_str()));
	if(!X->isCorectLoadLibrary()){
		delete X;
		return;
		}
	dlls[s]=X;

	common=*A.DATA.ps;
	chdir(common.c_str());
}


void ControlerDLL::unLoad(string s){
	if(dlls.find(s)==dlls.end())return;
	delete dlls[s];
	dlls.erase(s);
}


void ControlerDLL::Send(string s,string t){
	if(dlls.find(s)==dlls.end())return;
	dlls[s]->Send(const_cast<char*>(t.c_str()));
}


string ControlerDLL::Recv(string s){
	if(dlls.find(s)==dlls.end())return "";
	return dlls[s]->Recv();
}


void ControlerDLL::ReWriteVar(){
	M_CVARIANT&GM=*Main->GlobalSpace.Map.DATA.mapVal;
	CVARIANT S;
	S.avtoSet("string");
	*S.DATA.ps="DLLS";
	CVARIANT*R=&GM[S];
	R->avtoSet("vector");
	M_SXDLL::iterator it=Main->CDLL.dlls.begin();
	for(;it!=Main->CDLL.dlls.end();++it){
		*S.DATA.ps=it->first;
		R->DATA.vectorVal->push_back(S);
		}
}


//	dlls.cpp	:-|