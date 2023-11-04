//	UPServac.h

#pragma comment(lib, "Ws2_32.lib")
//add WSOCK32.LIB in Progect\Setings...\Link   // Not need.





#ifdef DEF_PCONS_INCLUDE
#endif


#ifdef DEF_PCONS_CLASS
	class UPServac;
#endif




#ifdef DEF_PCONS_TYPES
#endif




#ifdef DEF_PCONS_CONTENT
//--------------------------------------------------------------------------------------------------

class UPServac {
public:

	class User {
		static int sizeofbuffer;
	public:
		SOCKET ClientConn;
		char* buffer;
		string Buffer;
		bool isFull;
		bool isNeedExit;
		double timeEnd;
		HANDLE HRecv;
		CRITICAL_SECTION* pCriticalSection;

		User(SOCKET,CRITICAL_SECTION*);
		~User();

		void RunRecv();
		static DWORD WINAPI RecvStartFunction(void*);
		void Recv();
		void Send(const char*);
	};

	typedef vector<User*> V_pUser;


	class PairWait {
	public:
		SOCKET ClientConn;
		double timeEnd;

		PairWait(SOCKET ClientConn);
		~PairWait();

		closeMe();

	};

	typedef vector<PairWait*> V_pPairWait;



	SOCKET ServSock;
	int Port;
	string Doclad;
	bool isDocladCompleate;
	bool isON;

	V_pUser AllUsers;
	V_pPairWait AllWaits;
	CRITICAL_SECTION CriticalSection;
	HANDLE HListen;
	User* DutyUser;

	UPServac(int port);
	~UPServac();

	void RunServer();
	static DWORD WINAPI StartServer(void*);
	void Listen();
	string getDoclad();

	User* FindActualUser();
	void WaitUser();
	void KillDutyUser();
	void SendAndClose(const char*);
	string GetBuffer();
	int getCountWaitUsers();
	void ControlCloseWaits();

	static double gettimeofday();
};


//--------------------------------------------------------------------------------------------------
#endif



//	UPServac.h	:-|