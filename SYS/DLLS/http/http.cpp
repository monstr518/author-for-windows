// http.cpp : Defines the entry point for the DLL application.
//
//#define CURL_STATICLIB

#include "stdafx.h"

#pragma comment(lib,"lib\\libcurl.lib")
#include "head.h"
#include "string"
#include "curl/curl.h"

using namespace std;

string result,url,data;
bool ok=1;

static size_t writer(char *ptr, size_t size, size_t nmemb, string* data){
  if (data) 
   {
     data->append(ptr, size*nmemb);
     return size*nmemb;
   }
  else return 0;
}



string http(const char*url, const char*text){
  CURL *curl;
  CURLcode res;
  string content;
  struct curl_slist *headers=NULL; 
  
  //curl_slist_append(headers, "Accept: application/json");  

  /* In windows, this will init the winsock stuff */
  curl_global_init(CURL_GLOBAL_WIN32);
  /* get a curl handle */
  curl = curl_easy_init();
  if(curl) {
    /* First set the URL that is about to receive our POST. This URL can
       just as well be a https:// URL if that is what should receive the
       data. */
    curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    /* Now specify the POST data */
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, text);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &content);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);

    /* Perform the request, res will get the return code */
    res = curl_easy_perform(curl);
    /* Check for errors */
    if(res != CURLE_OK)content=curl_easy_strerror(res);
    /* always cleanup */
    curl_easy_cleanup(curl);
  }
  curl_global_cleanup();
  return content;
}




BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}



MODUL_API void SendString(char*input){
	string str=input;
	if(str.substr(0,4)=="url:"){
		url=str.substr(4);
		return;
		}
	if(str.substr(0,5)=="data:"){
		data=str.substr(5);
		return;
		}
	if(str.substr(0,4)=="send"){
		result=http(url.c_str(),data.c_str());
		ok=1;
		return;
		}
}



MODUL_API char* RecvString(){
	if(ok){
		ok=0;
		static char*count="1";
		return count;
		}
	return const_cast<char*>(result.c_str());
}
