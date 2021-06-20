// GENERALIZEDF.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "GENERALIZEDF.h"


string buf;
int cok;
Problem*P;

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	cok=0;
	P=NULL;
    return TRUE;
}



 
GENERALIZEDF_API void SendString(const char*s){
	string str=s;
	if(str=="name?"){buf="generalizef";return;}
	if(str=="type?"){buf="questions|noparalele";return;}
	if(str=="set cout on"){cok=1;buf="!";return;}
	if(str=="set cout off"){cok=0;buf="!";return;}
	if(str=="example?"){buf=
		"?: what generalized formula for (-0.5*i+6.5, (15-i)/2) values{t:7,8;}\n";
		return;
		}
	if(!P){
		P=new(Problem);
		if(P->ParserProblem(s)){
			P->run();
			}else{
			buf=":";
			delete P;
			P=NULL;
			}
		return;
		}
	P->Set(s);
	P->run();
}



GENERALIZEDF_API const char* RecvString(){
	if(P){
		if(P->buf.size())return P->buf.c_str();
		if(P->Results.S.size()){
			if(P->Results.n){
				buf=P->Results.Next();
				if(P->Results.S.empty()){
					delete P;
					P=NULL;
					}
				}else{
				P->Results.n=P->Results.S.size();
				char s[10];
				sprintf(s,"%d",P->Results.n);
				buf="{}:";
				buf+=s;
				if(P->Results.n==1)return RecvString();
				}
			}
		}
	return buf.c_str();
}

