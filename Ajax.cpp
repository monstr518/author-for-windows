#include <winsock2.h>
//#include <ws2tcpip.h>
//#include <stdio.h>

#include <string>

#include <windows.h>
#include <wininet.h>

//#include <iostream.h>

#pragma comment(lib, "ws2_32.lib")
//#pragma comment(lib, "wininet.lib")

using namespace std;


/*
// Need: wininet.lib
bool ServerISON(string&Host_name){
	//MessageBox(NULL, "Сервер работает.", "Информация", MB_OK);
	HINTERNET hInternet = InternetOpen("HTTP Request", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (hInternet == NULL) {
        //MessageBox(NULL, "Ошибка при инициализации WinINet.", "Ошибка", MB_OK | MB_ICONERROR);
        return 0;
		}
	bool Result = 0;
	//"http://example.com"
    HINTERNET hConnect = InternetOpenUrl(hInternet, Host_name.c_str(), NULL, 0, INTERNET_FLAG_RELOAD, 0);
    if (hConnect == NULL) {
		MessageBox(NULL, "Сервер недоступен. 1", "Информация", MB_OK | MB_ICONWARNING);
		Result = 1;
		} else {
		DWORD statusCode = 0;
		DWORD statusSize = sizeof(statusCode);
		HttpQueryInfo(hConnect, HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER, &statusCode, &statusSize, NULL);
		if (statusCode == 200) {
			MessageBox(NULL, "Сервер работает. 2", "Информация", MB_OK);
			Result = 1;
			} else {
			MessageBox(NULL, "Сервер недоступен. 3", "Информация", MB_OK | MB_ICONWARNING);
			Result = 0;
			}
		// Закрытие соединения
		InternetCloseHandle(hConnect);
		}
    // Закрытие WinINet
    InternetCloseHandle(hInternet);
    return Result;
}
*/



bool Ajax(string Host_name, int port, string method, string heads, string body, string& Response){
/*
	if(!ServerISON(Host_name)){
		Response = "Server not found!";
		return 0;
		}
*/


    //-----------------------------------------
    // Declare and initialize variables
    WSADATA wsaData;
    int iResult;

    DWORD dwError;

    struct hostent *remoteHost;
    const char *host_name;
	char R[256];

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if(iResult != 0) {
        sprintf(R,"WSAStartup failed: %d\n", iResult);
		Response = R;
        return false;
	    }

    host_name = Host_name.c_str();
	int i = 0;
	bool isIP = true;
	for(;host_name[i];++i){
		char c = host_name[i];
		bool isValid = (isdigit(c) || c=='.');
		if(!isValid)isIP = false;
		}

	if(!isIP){
		//gethostname(R, 255);
		remoteHost = gethostbyname(host_name); // host_name R
		if(remoteHost == NULL) {
			dwError = WSAGetLastError();
			if (dwError != 0) {
				if (dwError == WSAHOST_NOT_FOUND) {
					Response = "Host not found\n";
					return false;
					} else if (dwError == WSANO_DATA) {
					Response = "No data record found\n";
					return false;
					} else {
					sprintf(R,"Function failed with error: %ld\n", dwError);
					Response = R;
					return false;
					}
				}
			}
		/*
		struct in_addr **addr_list;
		addr_list = (struct in_addr **)remoteHost->h_addr_list;
		cout << "IP addresses: "  << endl;
		for(i = 0; addr_list[i] != NULL; i++) {
			cout << inet_ntoa(*addr_list[i]) << endl;
			}
		*/
		}

	int sockfd;
    struct sockaddr_in server;
 
    //Create socket
    sockfd = socket(AF_INET , SOCK_STREAM , IPPROTO_TCP); // IPv4, STREAM NOT DDGRAM, TCP // 0 IPPROTO_TCP
 
    if(sockfd < 0){
		Response = "Could not create socket\n";
		return false;
		}

	ZeroMemory((void*)&server, sizeof(server));
	//server.sin_zero
    server.sin_family = AF_INET;    //IPv4
	server.sin_port = htons(port); // Host-to-network-short 16 bit. convert port to Network Byte Order (Big-Endian )
	if(isIP){
		server.sin_addr.s_addr = inet_addr(host_name); // "192.168.1.3"
		}else{
		server.sin_addr.s_addr = *((unsigned long *)remoteHost->h_addr);
		}


/*
	u_long nonBlockingMode = 1;
    if(ioctlsocket(sockfd, FIONBIO, &nonBlockingMode) != 0) {
		Response = "ioctlsocket?";
        //std::cerr << "Ошибка при установке неблокирующего режима: " << WSAGetLastError() << std::endl;
        closesocket(sockfd);
        WSACleanup();
        return false;
		}
*/


    //Connect to remote server
	int connectResult = -1;
	connectResult = connect(sockfd, (struct sockaddr *)&server, sizeof(server));
    if(connectResult < 0){
		Response = "connect failed. Error";
		return false;
		}






    string request; // Send this message to server

    /*
    GET /ip HTTP/1.1\r\n
    HOST: httpbin.org\r\n
    \r\n
	XN--80AKHBYKNJ4F
    */

	if(heads.size()){
		char c = heads[heads.size()-1];
		if(c!='\n')heads += "\r\n";
		}

	// " / HTTP/1.1\r\nHOST: www." + 
	// " / HTTP/1.1\r\nHOST: " + 
    request = method + 
		" /ip HTTP/1.1\r\nHOST: " + 
		Host_name + 
		"\r\n" + 
		heads + 
		"\r\n" + 
		body;
 
    //printf("Send data with TCP:\n%s\n",request);
	//Send some data with TCP
    if(send(sockfd, request.c_str(), request.length(), 0) < 0){
		closesocket(sockfd);
		Response = "Send failed\n";
		return false;
		}
    //printf("Request sended\n");

	int size = 1024;
    char *server_reply; //buffer for server reply
	server_reply = new char[size];

	string Server_reply;
	bool ok = true;
    while(ok){
		Sleep(1);
		//int t;
		//for(t=0;t<1000;++t)
		//memset(server_reply, 0, size);
		i = recv(sockfd, server_reply, size-1, 0);
		if(!i)break;
		if(i<0){
			int E = WSAGetLastError();
			printf("recv failed: %d\n", E);
			if(E==10054)printf("WSAECONNRESET\n");
			if(E==10035){
				printf("WSAEWOULDBLOCK\n");
				//Sleep(1000);
				continue;
				}
			printf("size: %d\n", Server_reply.size());
			break;
			}
		server_reply[i] = 0;
		Server_reply += server_reply;
		//printf(":%s", server_reply);
		//printf(":%d", i);
		//printf(" ");
		ok = (i==size-1);
		}
    delete[] server_reply;
    closesocket(sockfd);
	//WSACleanup();

	if(i<0){
		//cout<<Server_reply.c_str()<<endl;
		Response = "recv failed !!!";
		return false;
		}

	Response = Server_reply;
	return true;
}














/*
int main(int argc, char **argv)
{

    //-----------------------------------------
    // Declare and initialize variables
    WSADATA wsaData;
    int iResult;

    DWORD dwError;
    int i = 0;

    struct hostent *remoteHost;
    char *host_name;
    struct in_addr addr;

    char **pAlias;
    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed: %d\n", iResult);
        return 1;
    }

    host_name = "google.com";

    printf("Calling gethostbyname with %s\n", host_name);
    remoteHost = gethostbyname(host_name);
    
    if (remoteHost == NULL) {
        dwError = WSAGetLastError();
        if (dwError != 0) {
            if (dwError == WSAHOST_NOT_FOUND) {
                printf("Host not found\n");
                return 1;
            } else if (dwError == WSANO_DATA) {
                printf("No data record found\n");
                return 1;
            } else {
                printf("Function failed with error: %ld\n", dwError);
                return 1;
            }
        }
    } else {
        printf("Function returned:\n");
        printf("\tOfficial name: %s\n", remoteHost->h_name);
        for (pAlias = remoteHost->h_aliases; *pAlias != 0; pAlias++) {
            printf("\tAlternate name #%d: %s\n", ++i, *pAlias);
        }
        printf("\tAddress type: ");
        switch (remoteHost->h_addrtype) {
        case AF_INET:
            printf("AF_INET\n");
            break;
        case AF_NETBIOS:
            printf("AF_NETBIOS\n");
            break;
        default:
            printf(" %d\n", remoteHost->h_addrtype);
            break;
        }
        printf("\tAddress length: %d\n", remoteHost->h_length);

        i = 0;
        if (remoteHost->h_addrtype == AF_INET)
        {
            while (remoteHost->h_addr_list[i] != 0) {
                addr.s_addr = *(u_long *) remoteHost->h_addr_list[i++];
                printf("\tIP Address #%d: %s\n", i, inet_ntoa(addr));
            }
        }
        else if (remoteHost->h_addrtype == AF_NETBIOS)
        {   
            printf("NETBIOS address was returned\n");
        }   
    }






	int sockfd;
 
    char *enteredString;//enter in console
    char *hostname=remoteHost->h_name;
    char path[1000];
 
    struct hostent *he;
    int p1, p2;
 
    struct sockaddr_in server;
 



	int port = 80;
 
   
     
    //Create socket
    sockfd = socket(AF_INET , SOCK_STREAM , 0);//IPv4, STREAM NOT DDGRAM, TCP
 
    if (sockfd < 0) {
        printf("Could not create socket\n");
    }
 
    printf("Socket created\n");
 
     
    server.sin_family = AF_INET;    //IPv4

    //memcpy(&server.sin_addr, remoteHost->h_addr_list[0], strlen(remoteHost->h_addr_list[0]));//where, what, len
	server.sin_addr.s_addr=*(u_long *) remoteHost->h_addr_list[0];
    server.sin_port = htons(port);//Host-to-network-short 16 bit. convert port to Network Byte Order (Big-Endian )
 

	addr.s_addr = *(u_long *) remoteHost->h_addr_list[0];
    sprintf(path,"/%s", inet_ntoa(addr));

    //Connect to remote server
    if (connect(sockfd , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }
 
    char request[256];//Send this message to server
    char server_reply[100000];//buffer for server reply
	memset(server_reply,0,100000);
 
    sprintf(request, "%s /%s %s%s www.%s%s\r\n", "GET", "", "HTTP/1.1\r\n", 
                                        "HOST:", hostname, "\r\n"
										);
 
    printf("Send data with TCP:\n%s\n",request);
	//Send some data with TCP
    if(send(sockfd, request, strlen(request), 0) < 0) {
        printf("Send failed\n");
        exit(1);
    }
 
 
    printf("Request sended\n");
 
    if(recv(sockfd, server_reply , sizeof(server_reply), 0) < 0) {//receive answer
        printf("recv failed\n");
    }
 
 
 
    printf("Server reply: \n\n%s\n", server_reply);//print the answer
     
    closesocket(sockfd);





    return 0;
}





/*
int main(int argc, char **argv)
{
	string host_name = "example.com";
	string X = Ajax(host_name,80,"GET");
	printf("%s", X.c_str());
    return 0;
}
*/

