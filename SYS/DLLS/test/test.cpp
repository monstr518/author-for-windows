// test.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "head.h"
#include "iostream.h"
#include "string"

using namespace std;


BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}

string buf;

//--------------------------------------------------------------------------------------------------
void f1(int i){
	i=i*3+2;
	char s[100];
	sprintf(s,"%d",i);
	buf=s;
}

void f2(int i){
	i=(i+1)*i/2;
	char s[100];
	sprintf(s,"%d",i);
	buf=s;
}


//--------------------------------------------------------------------------------------------------
MODUL_API void SendString(char*s){
	string str=s;
	if(str=="" || str=="HELP")buf="FUNCTIONS:author,f1,f2";
	if(str=="f1" || str=="f2"){
		str=str.substr(3);
		int i=0;
		sscanf(str.c_str(),"%d",&i);
		if(str=="f1")f1(i);
		if(str=="f2")f2(i);
		}
	if(str=="author"){
		buf="Язык создал: Вадим Довгополый<br/>";
		buf+="e-mail: monstr518@ukr.net<br/>";
		buf+="тел.: 0965901304<br/>";
		buf+="<a href=\"http://www.monstr.domivo4ka.com\" >http://www.monstr.domivo4ka.com</a>";
		}
	if(str.substr(0,5)=="HELP:"){
		str=str.substr(5);
		if(str=="author")buf="author - Создатель языка.";
		if(str=="f1")buf="f1(int i)==i*3+2";
		if(str=="f2")buf="f2(int i)==(i+1)*i/2";
		}
}		


MODUL_API char* RecvString(){
	return const_cast<char*>(buf.c_str());
}
