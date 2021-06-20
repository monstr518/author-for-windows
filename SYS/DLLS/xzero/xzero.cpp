// xzero.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "string"
#include "stdio.h"
#include "iostream.h"
//add WSOCK32.LIB in Progect\Setings...\Link
//#include "windows.h"
#include "winsock.h"

using namespace std;

#include "head.h"


Serv*Ser=NULL;

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}

string buf;

DWORD WINAPI Serv::Poisc_client(void*param){
	WSAData WSData;
	WSAStartup(0x101,&WSData);
	SOCKET S = socket(AF_INET,SOCK_STREAM,0);
	sockaddr_in SA;
	SA.sin_family=AF_INET;
	SA.sin_port=htons(8958);
	SA.sin_addr.S_un.S_addr=INADDR_ANY;
	bind(S,(sockaddr*)&SA,sizeof(SA));

	listen(S,SOMAXCONN);

	SOCKET NS;
	//cout<<"INVALID_SOCKET"<<endl;
	while((NS=accept(S,0,0))!=INVALID_SOCKET){
		cout<<"Access is possible!"<<endl;
		if(Ser)Ser->SO=NS;
		break;
		closesocket(NS);
		cout<<"Disconnect!\n"<<endl;
		}
	
	closesocket(S);
	return 0;
}



DWORD WINAPI Serv::run_flash(void*param){
		string t="\""+Ser->way;
		t+="\\SYS\\SAFlashPlayer.exe\" ";
		t+=Ser->way+"\\code\\example\\xzero\\xzero.swf";
		system(t.c_str());
		return 0;
}


//--------------------------------------------------------------------------------------------------
void Serv::Send(const char*s){send(SO,s,strlen(s)+1,0);}

void Serv::Recv(){
	int k=S.find(".");
	if(k>=0){
		buf=S.substr(0,k);
		S=S.substr(k+1);
		return;
		}
	char byf[1000];
	int n=sizeof(byf);
	while(n){
		memset(byf,0,sizeof(byf));
		recv(SO,byf,n-1,0);
		char *i=strchr(byf,'.');
		if(!i){S+=byf;continue;}
		*i=0;
		S+=byf;
		buf=S;
		n=0;
		S=i+1;
		}
	
/*		
		Send("{comand,message}");
		Send("trace");
		Send("[6,4,65,354]");
		Send("{comand}");
		Send("quit");
*/
}


//--------------------------------------------------------------------------------------------------
MODUL_API void SendString(char*s){
	string str=s;
	if(str=="" || str=="HELP")buf="FUNCTIONS:setWAY,connect,slux,send";
	if(str.substr(0,5)=="HELP:"){
		str=str.substr(5);
		if(str=="setWAY")buf="setWAY - Установка пути к корневой папки языка. (Пример setWAY:d:\author)";
		if(str=="connect")buf="connect - Вызывает флеш ролик - интерфейс игры и устанавливает с ним сокет связь.";
		if(str=="slux" || str=="recv")buf="slux,recv - Приём слова от интерфейса.";
		if(str=="send")buf="send - Отправит данные в интерфейс ролик. Пример: send:{comand} send:quit";
		return;
		}
	if(str.substr(0,7)=="setWAY:")if(!Ser){
		Ser=new Serv;
		Ser->way=str.substr(7);
		}
	if(str=="connect"){
		//CreateThread(NULL, 0, Serv::Poisc_client, NULL, 0, NULL);
		CreateThread(NULL, 0, Serv::run_flash, NULL, 0, NULL);
		Serv::Poisc_client(NULL);
		}
	if(str=="slux"||str=="recv")Ser->Recv();
	if(str.substr(0,5)=="send:")Ser->Send(str.substr(5).c_str());
}




MODUL_API char* RecvString(){
	return const_cast<char*>(buf.c_str());
}

