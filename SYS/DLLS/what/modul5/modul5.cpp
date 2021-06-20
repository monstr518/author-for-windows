// modul5.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "modul5.h"
#include "module.h"

int cok;
ProblemA*P;
string buf;


BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	cok=0;
	P=NULL;
    return TRUE;
}




MODUL5_API void SendString(const char*str){
	string sstr=str;
	if(sstr=="name?"){buf="module5";return;}
	if(sstr=="type?"){buf="questions|noparalele";return;}
	if(sstr=="set cout on"){cok=1;buf="!";return;}
	if(sstr=="set cout off"){cok=0;buf="!";return;}
	if(sstr=="example?"){buf=
		"?: get code y=f(x) for table:head(x,y){x:2,3,4;y:4,10,18;}\n"
		"?: get code y=f(x) for table:head(x,y){y:2,3,4;x:4,6,8;}\n"
		"?: get code y=f(x) for table:head(x,y){x:0,1,2,3,4,5,6,7;y:0,1,4,3,16,5,36,7;}\n"
		"?: get code y=f(x) for table:head(x,y){x:0,1,2,3,4,5,6,7;y:0,4,4,6,16,8,36,10;}\n"
		"?: get code y=f(x) for table:head(x,y){x:0,1,2,3,4,5,6,7;y:1,0,9,4,3,9,7,6;}\n"
		"?: get code y=f(x) for table:head(x,y){x:0,1,2,3,4;y:5,8,5,8,5;}\n"
		"?: get code y=f(x) for table:head(x,y){x:0,3,5,6,7,9;y:8,8,6,8,1,8;}\n"
		"?: get code y=f(x) for table:head(x,y){x:0,1,2,3,4,5,6,7;y:1,0,8,4,3,5,7,6;}\n";
		return;
		}

	if(!P){
		P=new(ProblemA);
		if(P->ParserProblem(str)){
			P->run();
			}else{
			buf=":";
			delete P;
			P=NULL;
			}
		return;
		}
	P->Set(str);
	P->run();
}



MODUL5_API const char* RecvString(){
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
