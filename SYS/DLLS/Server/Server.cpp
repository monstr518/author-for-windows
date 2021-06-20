// Server.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "string"
#include "vector"
#include "stdio.h"
#include "iostream.h"
#pragma comment (lib,"WSOCK32.LIB")//add WSOCK32.LIB in Progect\Setings...\Link
//#include "windows.h"
#include "winsock.h"

using namespace std;

#include "head.h"

Serv Server;

BOOL APIENTRY DllMain( HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved){
    return TRUE;
}

string buf;

OneLink::OneLink(){thread=NULL;port=0;}

DWORD WINAPI OneLink::Poisc_client(void*param){
	WSAData WSData;
	WSAStartup(0x101,&WSData);
	SOCKET S = socket(AF_INET,SOCK_STREAM,0);
	sockaddr_in SA;
	SA.sin_family=AF_INET;
	SA.sin_port=htons(port);
	SA.sin_addr.S_un.S_addr=INADDR_ANY;
	bind(S,(sockaddr*)&SA,sizeof(SA));

	listen(S,SOMAXCONN);

	SOCKET NS;
	
	while((NS=accept(S,0,0))!=INVALID_SOCKET){
		cout<<"Access is possible!"<<endl;
		SO=NS;
		break;
		closesocket(NS);
		cout<<"Disconnect!\n"<<endl;
		}
	
	closesocket(S);
	return 0;
}



DWORD WINAPI OneLink::run_exe(void*param){
	OneLink*This=(OneLink*)param;
	system(This->wayexe.c_str());
	return 0;
}


//--------------------------------------------------------------------------------------------------
void Serv::Send(const char*s){send(List[active]->SO,s,strlen(s)+1,0);}

void Serv::Recv(){
	OneLink*OL=List[active];
	string&S=OL->S;
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
		recv(OL->SO,byf,n-1,0);
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



MODUL_API void SendString(char*s){
	string str=s;
	if(str=="" || str=="HELP")buf="FUNCTIONS:newserver,setactive,setport,setexe,connect,slux,send,close";
	if(str.substr(0,5)=="HELP:"){
		str=str.substr(5);
		if(str=="setexe")buf="setexe - Установка пути к exe файлу. (Пример setexe:\"d:\\author\\author.exe\" help.txt)";
		if(str=="connect")buf="connect - Запускает ехе и устанавливает с ним сокет связь.";
		if(str=="slux" || str=="recv")buf="slux,recv - Приём слова от интерфейса.";
		if(str=="send")buf="send - Отправит данные в интерфейс ролик. Пример: send:{comand} send:quit";
		if(str=="newserver")buf="newserver - Подготовит новое соединение.";
		if(str=="setactive")buf="setactive - Установит активное соединение для диалога. setactive(0)";
		if(str=="setport")buf="setport - Задать порт. setport(8958)";
		if(str=="close")buf="close - Закроет активное соединение.";
		return;
		}
	if(str.substr(0,9)=="setactive"){
		str=str.substr(10);
		sscanf(str.c_str(),"%d",&Server.active);
		return;
		}
	if(str=="newserver"){
		OneLink*OL=new(OneLink);
		Server.List.push_back(OL);
		Server.active=Server.List.size()-1;
		}
	if(str.substr(0,7)=="setport"){
		str=str.substr(8);
		int p=0;
		sscanf(str.c_str(),"%d",&p);
		Server.List[Server.active]->port=p;
		return;
		}
	if(str.substr(0,7)=="setexe:"){
		Server.List[Server.active]->wayexe=str.substr(7);
		}
	if(str=="close"){//CloseHandle
		CloseHandle(Server.List[Server.active]->thread);
		}
	if(str=="connect"){
		//CreateThread(NULL, 0, Serv::Poisc_client, NULL, 0, NULL);
		if(!Server.List[Server.active]->wayexe.empty())
			Server.List[Server.active]->thread=CreateThread(NULL, 0, OneLink::run_exe, (void*) Server.List[Server.active] , 0, NULL);
		Server.List[Server.active]->Poisc_client(NULL);
		}
	if(str=="slux"||str=="recv")Server.Recv();
	if(str.substr(0,5)=="send:")Server.Send(str.substr(5).c_str());
}


MODUL_API char* RecvString(){
	return const_cast<char*>(buf.c_str());
}
