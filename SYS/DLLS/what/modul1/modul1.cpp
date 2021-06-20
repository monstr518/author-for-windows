// modul1.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "modul1.h"
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




string f1(const char* str){
	Problem P;
	string g="name?";
	if(g==str)return "first module";
	g="project?";
	if(g==str)return "module1";
	g="type?";
	if(g==str)return "noquestions|noparalele";
	g="set cout on";
	if(g==str){cok=1;return "!";}
	g="set cout off";
	if(g==str){cok=0;return "!";}
	g="example?";
	if(g==str)return "?: get code y=f(x) for table: head(x,y){x:1,2;y:5,10;}\n";
	string r;
	if(!P.ParserProblem(str))r=":"; else r=P.run();
	return r;
}





string buf;

MODUL1_API void SendString(const char*str){
	//cout<<str<<endl;
	string s=f1(str);
	buf=s;
	//cout<<s<<endl;
}


MODUL1_API const char* RecvString(){
	return buf.c_str();
}