// test.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"

#include "head.h"
#include "JSON.h"

//#include "iostream.h"
//#include "string"

using namespace std;

string buf;




BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return true;
}



//--------------------------------------------------------------------------------------------------
MODUL_API const char* RUN(const char*Data){
	JSON json;
	bool isParseOK;

	isParseOK = json.parse(Data);

	JSON::ONE *one;
	JSON::ONE *oneData;
	if(isParseOK){
		oneData = json.one;
		json.one = NULL;
		}

	const char *data = "[]";
	json.parse(data);

	if(isParseOK)json.one->Values.push_back(oneData);

	one = new JSON::ONE();
	one->avtoSet("null");
	json.one->Values.push_back(one);

	one = new JSON::ONE();
	one->avtoSet("bool");
	one->intVal = 1;
	json.one->Values.push_back(one);

	one = new JSON::ONE();
	one->avtoSet("int");
	one->intVal = 101;
	json.one->Values.push_back(one);

	buf = json.toString(0);

	return buf.c_str();
}

