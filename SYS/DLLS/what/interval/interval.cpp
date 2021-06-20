// interval.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "interval.h"



BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}


int cok=0;
string buf;


string f1(const char*str){
	string g="name?";
	if(g==str)return "interval module";
	g="project?";
	if(g==str)return "interval";
	g="type?";
	if(g==str)return "noquestions|noparalele";
	g="set cout on";
	if(g==str){cok=1;return "!";}
	g="set cout off";
	if(g==str){cok=0;return "!";}
	g="example?";
	if(g==str)return 
		"?: optimize conditions (R && x<8 || !(x<0 || !x>=10) &&  x=10 || ((x>100|| x<0)))\n"
		"?: optimize conditions ((x>0 && x<10)||(x>10 && x<=20)||x=10)\n"
		"?: optimize conditions ((x<0 || x>10) && (x<20 || x>30) && (x<40 || x>50) || x=5 || x=40)\n"
		"?: whot basis conditions ((x<0 || x>10) && x<20)\n"
		"?: whot basis conditions ((x<0 && x<20) || (x>10 && x<20))\n";
	return ".";
}



EXPORT void SendString(const char*str){
	buf=f1(str);
	if(buf==".")base::problem(str);
}



EXPORT const char* RecvString(){
	return buf.c_str();
}

