// modul2.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "modul2.h"
#include "Problem.h"


int cok;


BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	cok=0;
    return TRUE;
}



string run(string str){
	Problem P;
	if(str=="name?")return "module2";
	if(str=="type?")return "noquestions|noparalele";
	if(str=="set cout on"){cok=1;return "!";}
	if(str=="set cout off"){cok=0;return "!";}
	if(str=="example?")return "?: get code(n) for signal(A,B) {A:1,2;B:0,5,7;} pset{0,4}\n";
	string r;
	if(!P.ParserProblem(str.c_str()))r=":"; else r=P.run();
	return r;
}



string buf;

MODUL2_API void SendString(const char*str){
	//cout<<str<<endl;
	string s=run(str);
	buf=s;
	//cout<<s<<endl;
}


MODUL2_API const char* RecvString(){
	return buf.c_str();
}
