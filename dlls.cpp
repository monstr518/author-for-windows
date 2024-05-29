//	dlls.cpp
#include "main.h"


//--------------------------------------------------------------------------------------------------
Xdll::Xdll(const char*namedll){
	dll = LoadLibrary(namedll);
	RUN = (const char* (*)(const char*))GetProcAddress(dll,"RUN");
}


Xdll::~Xdll(){
	FreeLibrary(dll);
}


const char* Xdll::Run(const char* Message){
	return (*RUN)(Message);
}


bool Xdll::isCorectLoadLibrary(){
	return (RUN!=NULL);
}



//--------------------------------------------------------------------------------------------------
ControlerDLL::ControlerDLL(MAIN*M){Main=M;}


ControlerDLL::~ControlerDLL(){
	M_SXDLL::iterator it=dlls.begin();
	for(;it!=dlls.end();++it)delete it->second;
}



void ControlerDLL::Load(string NameDLL, string path){
	M_SV&GM = *Main->GlobalSpace.Map.DATA.mapVal;
	CVARIANT A;
	A = GM["WAY"];
	path = *A.DATA.ps + "\\SYS\\DLLS\\" + path;
	/*
	string common;
	common=*A.DATA.ps+"\\SYS\\DLLS\\";
	chdir(common.c_str());
	*/
	Xdll*X = new Xdll(path.c_str());
	if(!X->isCorectLoadLibrary()){
		cout<<"Error LoadDLL: "<<path.c_str()<<" not found!"<<endl;
		delete X;
		return;
		}
	dlls[NameDLL]=X;
	/*
	common=*A.DATA.ps;
	chdir(common.c_str());
	*/
}


void ControlerDLL::unLoad(string NameDLL){
	if(dlls.find(NameDLL)==dlls.end())return;
	delete dlls[NameDLL];
	dlls.erase(NameDLL);
}


const char* ControlerDLL::Run(string NameDLL, string Data){
	if(dlls.find(NameDLL)==dlls.end())return "Need Load DLL!";
	return dlls[NameDLL]->Run(Data.c_str());
}


/*
void ControlerDLL::ReWriteVar(){
	M_SV&GM=*Main->GlobalSpace.Map.DATA.mapVal;
		CVARIANT*R=&GM["DLLS"];
	R->avtoSet("vector");
	M_SXDLL::iterator it=Main->CDLL.dlls.begin();
	for(;it!=Main->CDLL.dlls.end();++it){
		CVARIANT *S = new CVARIANT();
		S->avtoSet("string");
		*S->DATA.ps=it->first;
		R->DATA.vectorVal->push_back(S);
		}
}
*/



//	dlls.cpp	:-|