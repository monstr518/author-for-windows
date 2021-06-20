#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <windows.h>
#include <string>

#pragma comment(lib, "ws2_32.lib")
using namespace std;



string Ajax(string Host_name,int port,string method,string heads,string body){
    //-----------------------------------------
    // Declare and initialize variables
    WSADATA wsaData;
    int iResult;

    DWORD dwError;
    int i = 0;

    struct hostent *remoteHost;
    const char *host_name;
    struct in_addr addr;

    char **pAlias;
	char R[100];
    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        sprintf(R,"WSAStartup failed: %d\n", iResult);
        return R;
	    }

    host_name = Host_name.c_str();

    sprintf(R,"Calling gethostbyname with %s\n", host_name);
    remoteHost = gethostbyname(host_name);
    
    if (remoteHost == NULL) {
        dwError = WSAGetLastError();
        if (dwError != 0) {
            if (dwError == WSAHOST_NOT_FOUND) {
                return "Host not found\n";
				} else if (dwError == WSANO_DATA) {
                return "No data record found\n";
				} else {
                sprintf(R,"Function failed with error: %ld\n", dwError);
                return R;
				}
			}
		}
	/*	else {
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
        if (remoteHost->h_addrtype == AF_INET){
            while (remoteHost->h_addr_list[i] != 0) {
                addr.s_addr = *(u_long *) remoteHost->h_addr_list[i++];
                printf("\tIP Address #%d: %s\n", i, inet_ntoa(addr));
				}
			}
        else if (remoteHost->h_addrtype == AF_NETBIOS){   
            printf("NETBIOS address was returned\n");
			}
		}
	*/

	int sockfd;
 
    char *enteredString;//enter in console
    char *hostname=remoteHost->h_name;
    char path[1000];
 
    struct hostent *he;
    int p1, p2;
 
    struct sockaddr_in server;
 
    //Create socket
    sockfd = socket(AF_INET , SOCK_STREAM , 0);//IPv4, STREAM NOT DDGRAM, TCP
 
    if (sockfd < 0)return "Could not create socket\n";
     
    server.sin_family = AF_INET;    //IPv4
    /* copy the network address to sockaddr_in structure */
    //memcpy(&server.sin_addr, remoteHost->h_addr_list[0], strlen(remoteHost->h_addr_list[0]));//where, what, len
	server.sin_addr.s_addr=*(u_long *) remoteHost->h_addr_list[0];
    server.sin_port = htons(port);//Host-to-network-short 16 bit. convert port to Network Byte Order (Big-Endian )
 
	//addr.s_addr = *(u_long *) remoteHost->h_addr_list[0];
    //sprintf(path,"/%s", inet_ntoa(addr));

    //Connect to remote server
    if (connect(sockfd , (struct sockaddr *)&server , sizeof(server)) < 0)return "connect failed. Error";
 
    string request;//Send this message to server
    char server_reply[100];//buffer for server reply
	

    /*
    GET /ip HTTP/1.1\r\n
    HOST: httpbin.org\r\n
    \r\n
	XN--80AKHBYKNJ4F
    */
    request = method + " / HTTP/1.1\r\nHOST: www." + Host_name + "\r\n" + heads + "\r\n\r\n" + body;
 
    //printf("Send data with TCP:\n%s\n",request);

	//Send some data with TCP
    if(send(sockfd, request.c_str(), request.length(), 0) < 0)return "Send failed\n";

    //printf("Request sended\n");
	string Server_reply;
    do {
		Sleep(1);
		memset(server_reply,0,sizeof(server_reply));
		i=recv(sockfd, server_reply , sizeof(server_reply)-1, 0);
		if(i<0)break;
		Server_reply+=server_reply;
		}while(i==sizeof(server_reply)-1);
	if(i<0)return "recv failed\n";
 
    //printf("Server reply: \n\n%s\n", server_reply);//print the answer
     
    closesocket(sockfd);

	return Server_reply;
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

