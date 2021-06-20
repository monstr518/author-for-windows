// modul4.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "modul4.h"
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


string buf;


MODUL4_API void SendString(const char*str){
	string sstr=str;
	if(sstr=="name?"){buf="module4";return;}
	if(sstr=="type?"){buf="noquestions|noparalele";return;}
	if(sstr=="set cout on"){cok=1;buf="!";return;}
	if(sstr=="set cout off"){cok=0;buf="!";return;}
	if(sstr=="example?"){buf=
		"?: get code(n) for signal(A,B) {A:1,3;B:0,2,4;}\n"
		"?: get code(n) for signal(A) {A:0,3,6;B:1,2,4,5,7,8;}\n";
		return;
		}
	Problem P;
	if(P.ParserProblem(str)){
		buf=P.run();
		}else buf=":";
}


MODUL4_API const char* RecvString(){
	return buf.c_str();
}
