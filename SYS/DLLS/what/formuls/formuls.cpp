// formuls.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "formuls.h"

extern string result;
extern int cok;

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	//cok=0;
    return TRUE;
}



void fmain(char*);

MODUL6_API void SendString(const char*w){
	string str=w;
	if(str=="name?"){result="formuls";return;}
	if(str=="type?"){result="noquestions|noparalele";return;}
	if(str=="set cout on"){cok=1;result="!";return;}
	if(str=="set cout off"){cok=0;result="!";return;}
	if(str=="example?"){result=
		"?: what result for formula(x*x+x-2) from {x:2,3,4;}\n"
		"?: compare (x+n==x+2-(2-n))\n"
		"?: optimize (u*(a-x)+u*(a-x)) from {x=1;}\n"
		"?: optimize (-0.5*i+6.5)\n"
		"?: get equality for ((x+12)*(x-3)=0) unknown {x}\n"
		"?: get equality for (-1.5*y*y+17.5*y-45=x) unknown {y}\n"
		"?: get generalized tree formula for (-0.5*i+6.5, (15-i)/2)\n"
		"?: what var names formul (a+b=c=t)\n"
		"?: system equations {2*x+5*y=15;3*x+8*y=-1;x=?;y=?}\n";
		return;
		}
	fmain(const_cast<char*>(w));
}



MODUL6_API const char* RecvString(){
	return result.c_str();
}
