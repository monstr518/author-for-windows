// author.cpp
#include "main.h"

//--------------------------------------------------------------------------------------------------



void main(int argc, char* argv[]){
	string s;
	{
	TCHAR buffer[MAX_PATH];
	GetModuleFileName(NULL, buffer, MAX_PATH );
	s=buffer;
	}
	int i;
	bool isOtladka=0;
	i=s.find_last_of("\\");
	s=s.substr(0,i);
	i=s.find_last_of("\\");
	if(s.substr(i+1)=="Debug"){s=s.substr(0,i);isOtladka=1;}
	{
	string t=s;
	t+="\\SYS\\*.*";
	WIN32_FIND_DATA pDATA;
	HANDLE hFile = FindFirstFile(t.c_str(),&pDATA);
	if(hFile == INVALID_HANDLE_VALUE){
		printf("Cennot find folder SYS.\n");
		Sleep(1000);
		return;
		}
	FindClose(hFile);
	}
	srand(time(NULL)%172);
	MAIN Main;
	{
	Main.GlobalSpace.Map.avtoSet("map");
	M_SV&GM=*Main.GlobalSpace.Map.DATA.mapVal;
	CVARIANT A,B;
	A.avtoSet("string");
	B.avtoSet("string");
	*A.DATA.ps="WAY";
	*B.DATA.ps=s;
	GM["WAY"]=B;
	B.avtoSet("vector");
	int t;
	for(t=0;t<argc;++t){
		*A.DATA.ps=argv[t];
		B.DATA.vectorVal->push_back(A);
		}
	GM["ARGS"]=B;
	B.avtoSet("bool");
	B.DATA.boolVal=isOtladka;
	GM["isOtladka"]=B;
	B.avtoSet("map");
	GM["GLOBALS"]=B;
	B.avtoSet("vector");
	GM["DLLS"]=B;
	}
	s+="\\SYS\\main.txt";
	i=Main.IncludeFILE(s.c_str());
	if(i){
		Main.outHTML(Main.PHTML,"out.html");
		return;
		}
	Main.RUN();
	Main.outHTML(Main.PHTML,"out.html");
	cout<<"(C) OFF."<<endl;
	Sleep(100);
}


// author.cpp	:-|