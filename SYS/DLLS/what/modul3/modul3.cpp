// modul3.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "modul3.h"
#include "main.h"

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
	//cout<<str<<endl;
	if(str=="name?")return "module3";
	if(str=="type?")return "noquestions|noparalele";
	if(str=="set cout on"){cok=1;return "!";}
	if(str=="set cout off"){cok=0;return "!";}
	if(str=="example?")return 
		"?: get code for row {0,2,4,6} first(0)\n"
		"?: get code for row {0,1,4,9} first(5)\n";

	string s;
	ProblemA P;
	if(P.ParserProblem(str.c_str())){
		P.run();
		s=P.getResult();
		if(s.empty())s=".";
		}else s=":";
	//cout<<s<<endl;
	return s;
}



string buf;

MODUL3_API void SendString(const char*str){
	string s=run(str);
	buf=s;
}


MODUL3_API const char* RecvString(){
	return buf.c_str();
}
