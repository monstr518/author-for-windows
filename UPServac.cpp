//	UPServac.cpp

#include <winsock2.h>
#include "main.h"


int UPServac::User::sizeofbuffer = 1024;

//--------------------------------------------------------------------------------------------------
double UPServac::gettimeofday(){
	struct timeval tv;
	FILETIME ft;
	unsigned __int64 tmpres = 0;
	static int tzflag;
	GetSystemTimeAsFileTime(&ft);
	tmpres |= ft.dwHighDateTime;
	tmpres <<= 32;
	tmpres |= ft.dwLowDateTime;
	/*converting file time to unix epoch*/
	//DELTA_EPOCH_IN_MICROSECS;
	tmpres -= 11644473600000000Ui64;
	tmpres /= 10; /*convert into microseconds*/
	tv.tv_sec = (long)(tmpres / 1000000UL);
	tv.tv_usec = (long)(tmpres % 1000000UL);
	int tt=time(NULL);
	return (double)tt+tv.tv_usec/(1000000.0);
}





//--------------------------------------------------------------------------------------------------
UPServac::PairWait::PairWait(SOCKET ClientConn){
	this->ClientConn = ClientConn;
	this->timeEnd = UPServac::gettimeofday();
}

UPServac::PairWait::~PairWait(){
	closeMe();
}


UPServac::PairWait::closeMe(){
	int result = closesocket(ClientConn);
    if (result == SOCKET_ERROR) {
        int error_code = WSAGetLastError();
        printf("(!) Problem SOCKET_ERROR is: %d\n", error_code);
	    }
}



//--------------------------------------------------------------------------------------------------
UPServac::UPServac(int port){
	Port = port;
	InitializeCriticalSection(&CriticalSection);
	isDocladCompleate = 0;
	isON = 1;
}


UPServac::~UPServac(){
	closesocket(ServSock);
	WaitForSingleObject(HListen, INFINITE);
	CloseHandle(HListen);
	//TerminateProcess(HListen,0);
	EnterCriticalSection( &CriticalSection );
	int i, size = AllUsers.size();
	for(i=0;i<size;++i){
		User*U = AllUsers[i];
		if(U){
			PairWait* PW = new PairWait(U->ClientConn);
			AllWaits.push_back(PW);
			delete U;
			}
		}
	LeaveCriticalSection( &CriticalSection );
	WSACleanup();
	DeleteCriticalSection( &CriticalSection );
}



UPServac::User::User(SOCKET S, CRITICAL_SECTION*pcs){
	ClientConn = S;
	isFull = 0;
	isNeedExit = 0;
	buffer = new char[sizeofbuffer];
	pCriticalSection = pcs;
	timeEnd = gettimeofday();
}



UPServac::User::~User(){
	isNeedExit = 1;
	//GenerateConsoleCtrlEvent(CTRL_BREAK_EVENT, (HANDLE)HRecv);
	//TerminateProcess(HRecv,0);
	LeaveCriticalSection( pCriticalSection );
	WaitForSingleObject(HRecv, INFINITE);
	EnterCriticalSection( pCriticalSection );
	CloseHandle(HRecv);
	delete[] buffer;
	buffer = NULL;
}



void UPServac::User::RunRecv(){
	HRecv = CreateThread(NULL, 0, RecvStartFunction, this, 0, NULL);
}




void UPServac::Listen(){
	WSADATA wsData;
	int LastError;
	char sLastError[20];
	int erStat = WSAStartup(MAKEWORD(2,2), &wsData);
	if ( erStat != 0 ) {
		Doclad += "Error WinSock version initializaion #";
		LastError = WSAGetLastError();
		sprintf(sLastError,"%d",LastError);
		Doclad += sLastError;
		isDocladCompleate = 1;
		isON = 0;
		return;
		}else Doclad += "WinSock initialization is OK\n";

	ServSock = socket(AF_INET, SOCK_STREAM, 0); // IPPROTO_TCP

	if (ServSock == INVALID_SOCKET) {
		Doclad += "Error initialization socket #";
		LastError = WSAGetLastError();
		sprintf(sLastError,"%d",LastError);
		Doclad += sLastError;
		closesocket(ServSock);
		WSACleanup();
		isDocladCompleate = 1;
		isON = 0;
		return;
		}else Doclad += "Server socket initialization is OK\n";

	SOCKADDR_IN addr;
	int addrl = sizeof(addr);
	addr.sin_addr.S_un.S_addr = INADDR_ANY;
	addr.sin_port = htons(Port);
	addr.sin_family = AF_INET;

	erStat = bind(ServSock, (struct sockaddr*)&addr, sizeof(addr));

	if ( erStat != 0 ) {
		Doclad += "Error Socket binding to server info. Error #";
		LastError = WSAGetLastError();
		sprintf(sLastError,"%d",LastError);
		Doclad += sLastError;
		closesocket(ServSock);
		WSACleanup();
		isDocladCompleate = 1;
		isON = 0;
		return;
		}else Doclad += "Binding socket to Server info is OK\n";

	erStat = listen(ServSock, SOMAXCONN);

	if ( erStat != 0 ) {
		Doclad += "Can't start to listen to. Error #";
		LastError = WSAGetLastError();
		sprintf(sLastError,"%d",LastError);
		Doclad += sLastError;
		closesocket(ServSock);
		WSACleanup();
		isDocladCompleate = 1;
		isON = 0;
		return;
		}else{
		sprintf(sLastError,"Port: %d\n",Port);
		Doclad += sLastError;
		Doclad += "Listening...\n";
		}

	sockaddr_in clientInfo; 
	ZeroMemory(&clientInfo, sizeof(clientInfo));
	int clientInfo_size = sizeof(clientInfo);
	isDocladCompleate = 1;
	while(1){
		SOCKET ClientConn = accept(ServSock, (sockaddr*)&clientInfo, &clientInfo_size);
		if(ClientConn == INVALID_SOCKET) {
			/*
			EnterCriticalSection( &CriticalSection );
				Doclad += "Client detected, but can't connect to a client. Error #";
				LastError = WSAGetLastError();
				sprintf(sLastError,"%d",LastError);
				Doclad += sLastError;
			LeaveCriticalSection( &CriticalSection );
			*/
			//closesocket(ServSock);
			closesocket(ClientConn);
			WSACleanup();
			isON = 0;
			return;
			}
		//cout << "Connection to a client established successfully" << endl;
		int size;
		EnterCriticalSection( &CriticalSection );
		size = AllUsers.size();
		LeaveCriticalSection( &CriticalSection );
		if(size>50){
			closesocket(ClientConn);
			//cout<<"ClientKilled!"<<endl;
			//Sleep(20);
			continue;
			}
		EnterCriticalSection( &CriticalSection );
		User*U = new User(ClientConn, &CriticalSection);
		U->RunRecv();
		AllUsers.push_back(U);
		LeaveCriticalSection( &CriticalSection );
		}
}




void UPServac::RunServer(){
	HListen = CreateThread(NULL, 0, StartServer, this, 0, NULL);
	while(!isDocladCompleate)Sleep(10);
	EnterCriticalSection( &CriticalSection );
}


DWORD WINAPI UPServac::StartServer(void* Param){
	UPServac *UPS = (UPServac*)Param;
	UPS->Listen();
	return 0;
}



DWORD WINAPI UPServac::User::RecvStartFunction(void* Param){
	((UPServac::User*)Param)->Recv();
	return 0;
}




void UPServac::User::Recv(){
	int k = 0;
	bool isFirst = 1;
	while(k>=0){
		if(!buffer || isNeedExit)break;
		EnterCriticalSection( pCriticalSection ); //
		ZeroMemory(buffer, sizeofbuffer);
		timeEnd = gettimeofday();
		if(!isFirst)isFull = 1;
		LeaveCriticalSection( pCriticalSection ); //
		isFirst = 0;
		k = recv(ClientConn, buffer, sizeofbuffer-1, 0);
		if(!buffer || isNeedExit)break;
		isFull = 0;
        if(k>0){
			EnterCriticalSection( pCriticalSection );
			Buffer += buffer;
			LeaveCriticalSection( pCriticalSection );
			}
		//Sleep(10);
		}
	isNeedExit = 1;
}



void UPServac::User::Send(const char*msg){
/*
    int timeout = 15000;
    setsockopt(ClientConn, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));
*/
	int dataLength = strlen(msg);
	int result = send(ClientConn, msg, dataLength, 0);
	//Sleep(1); // 100
	if(result==dataLength)return;
	if(result<0){
		int E = WSAGetLastError();
		cout<<"WSAGetLastError(): "<<E<<endl;
		return;
		}
	cout<<"UPServac::User::Send: "<<result<<" from: "<<dataLength<<endl;
}




int UPServac::getCountWaitUsers(){
	int size = AllUsers.size();
	return size;
}



UPServac::User* UPServac::FindActualUser(){
	User* ActualUser = NULL;
	EnterCriticalSection( &CriticalSection );
	int i, size = AllUsers.size();
	//if(size)cout<<size<<endl; //
	for(i=0;i<size;++i){
		User*U = AllUsers[i];
		if(!U)continue;
		if(U->isNeedExit){
			AllUsers.erase(AllUsers.begin()+i);
			delete U;
			break;
			}
		if(U->isFull){
			double timeVal = gettimeofday();
			bool isEnd = ((timeVal - U->timeEnd) > 0.1);
			if(isEnd){
				ActualUser = U;
				break;
				}
			}else{
			double timeVal = gettimeofday();
			bool isEnd = ((timeVal - U->timeEnd) > 10);
			if(isEnd)U->isNeedExit = 1;
			}
		}
	LeaveCriticalSection( &CriticalSection );
	return ActualUser;
}



void UPServac::WaitUser(){
	LeaveCriticalSection( &CriticalSection );
	while(isON){
		Sleep(40);
		DutyUser = FindActualUser();
		if(DutyUser)break;
		}
	EnterCriticalSection( &CriticalSection );
}



string UPServac::GetBuffer(){
	if(!DutyUser)return "";
	string R;
	//EnterCriticalSection( &CriticalSection );
	R = DutyUser->Buffer;
	//LeaveCriticalSection( &CriticalSection );
	return R;
}




void UPServac::KillDutyUser(){
	if(!DutyUser)return;
	EnterCriticalSection( &CriticalSection );
	V_pUser::iterator it = find(AllUsers.begin(), AllUsers.end(), DutyUser);
	//*it = NULL;
	AllUsers.erase(it);
	delete DutyUser;
	DutyUser = NULL;
	LeaveCriticalSection( &CriticalSection );
}



string UPServac::getDoclad(){
	string s;
	EnterCriticalSection( &CriticalSection );
	s = Doclad;
	LeaveCriticalSection( &CriticalSection );
	return s;
}





//--------------------------------------------------------------------------------------------------
void UPServac::ControlCloseWaits(){
	while(!AllWaits.empty()){
		double realTime = gettimeofday();
		PairWait* PW = AllWaits[0];
		bool isNeedClosed = (realTime - PW->timeEnd) > 60; // seconds
		//isNeedClosed = 1;
		if(isNeedClosed){
			delete PW;
			AllWaits.erase(AllWaits.begin());
			continue;
			}
		WSACleanup();
		return;
		}
}



//--------------------------------------------------------------------------------------------------
void UPServac::SendAndClose(const char*Response){
	if(!DutyUser)return;
	LeaveCriticalSection( &CriticalSection );
	DutyUser->Send(Response);
	Sleep(20);
	KillDutyUser();
	ControlCloseWaits();
	EnterCriticalSection( &CriticalSection );
}






//	UPServac.cpp	:-|