// head.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "header.h"


ControlerDLL CDLLS;

string buf;

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}



//---------------------------------------------------------------------------------------
EXPORT void SendString(char*s){
	string str=s;
	if(str=="HELP" || str.empty()){
		buf="FUNCTIONS:WAY,setcout,Example,ModuleNames,Rejoin,?:";
		CDLLS.Gotovo=0;
		return;
		}
	if(str.substr(0,5)=="HELP:"){
		str=str.substr(5);
		if(str=="WAY")buf="WAY - Путь к папке с самим языком author.exe";
		if(str=="setcout")buf="setcout(on/off) - Включает/выключает режим отовражения подпроцесов в модулях. Пример setcout(on,3) - включить все cout в модуле номер 3.";
		if(str=="Example")buf="Example - Вернёт примеры вопросов решаемые модулями dll библиотек.";
		if(str=="ModuleNames")buf="ModuleNames - Режым возврата имён модулей.";
		if(str=="Rejoin")buf="Rejoin - Режым возврата ответов.";
		if(str=="?:")buf="?: - Тело вопроса.";
		CDLLS.Gotovo=0;
		return;
		}
	if(str.substr(0,4)=="WAY:"){
		CDLLS.way=str.substr(4);
		CDLLS.initDLLS();
		return;
		}
	//setcout(on,12)
	if(str.substr(0,7)=="setcout"){
		str=str.substr(8);
		bool on=0;
		if(str.substr(0,2)=="on"){on=1;str=str.substr(2);}else
			if(str.substr(0,3)=="off"){on=0;str=str.substr(3);}
		const char* ss=str.c_str();
		if(*ss==')'){CDLLS.setCout(on);return;}
		if(*ss!=','){cout<<"error syntax."<<endl;return;}
		++ss;
		int n=0;
		sscanf(ss,"%d",&n);
		CDLLS.setCout(on,n);
		return;
		}
	if(str=="Example" || str=="EXAMPLE" || str=="example"){
		buf=CDLLS.getExample();
		CDLLS.Gotovo=0;
		return;
		}
	if(str=="ModuleNames"){
		CDLLS.MNames=1;
		CDLLS.iterator=0;
		return;
		}
	if(str=="Rejoin"){
		CDLLS.MNames=0;
		CDLLS.iterator=0;
		return;
		}
	if(str.substr(0,2)=="?:"){
		CDLLS.Vopros(s);
		char u[10];
		sprintf(u,"%d",CDLLS.nmax);
		buf=u;
		return;
		}
	buf="";
}




EXPORT char* RecvString(){
	if(CDLLS.Gotovo==2){
		if(CDLLS.nmax>CDLLS.iterator)buf=CDLLS.getOtvets();else CDLLS.Gotovo=0;
		}
	if(CDLLS.Gotovo==1)CDLLS.Gotovo=2;
	return const_cast<char*>(buf.c_str());
}
