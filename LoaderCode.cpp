//LoaderCode.cpp
#define DEF_PCONS_INCLUDE
#define DEF_PCONS_CONTENT
#include <string>
#include "LoaderCode.h"



LOADER::LOADER(){text=NULL;}
LOADER::~LOADER(){if(text)delete[] text;}


char*LOADER::copy(std::string&s){
	char*c=new(char[s.size()+1]);
	int i;
	for(i=0;i<s.size();++i)c[i]=s[i];
	c[i]=0;
	return c;
}


void LOADER::load(const char*name){
	ifstream f(name, ios::in | ios::nocreate);
	if(!f){
		cout<<"File "<<name<<" not found."<<endl;
		return;
		}
	int length;
	f.seekg(0, ios::end);    // go to the end
	length = f.tellg();           // report location (this is the length)
	f.seekg(0, ios::beg);    // go back to the beginning
	text = new char[length+1];    // allocate memory for a buffer of appropriate dimension
	memset(text,0,length+1);
	f.read(text, length);       // read the whole file into the buffer
	f.close();                    // close file handle
}




void LOADER::delete_komentary(){
	if(!text)return;
	std::string ee;
	ee.reserve(sizeof(text));
	int pp=0;// количество переносов строки (\) (нужно для вставки потом \n)
	int ww=1,vv=1; // ww - "    vv - '
	int i,j;
	bool isPrevSlesh = 0;
	for(i=0;text[i];++i){
		j=i;
		while(text[j]=='\\'&&text[j+1]==10){j+=2;i=j;++pp;}
		/*
		if(!ww)if(text[i]=='\\'){
			ee+=text[i++];
			ee+=text[i];
			continue;
			}
		*/
		if(ww&&vv&&text[i]=='/')if(text[i+1]=='/'){
			int j;
			for(j=2;text[i+j]&&text[i+j]!=10;++j);
			i+=j;
			if(!text[i])--i;
			ee+='\n';
			continue;
			}
		if(ww&&text[i]=='/')if(text[i+1]=='*'){
			i+=2;
			for(;text[i];++i){
				if(text[i]=='*' && text[i+1]=='/')break;
				if(text[i]=='\n')ee+='\n';
				}
			if(text[i])++i;
			continue;
			}
		if(!isPrevSlesh){
			if(vv&&text[i]=='"') ww=1-ww;
			if(ww&&text[i]==39 ) vv=1-vv;
			}
		if(text[i]==10){
			ww=1;
			vv=1;
			while(pp){ee+="\n";--pp;}
			}
		ee+=text[i];
		isPrevSlesh = (!isPrevSlesh && (!ww||!vv) && text[i]=='\\');
		}
	delete[] text;
	text=copy(ee);
}




LOADER::LOADER(const char*name,bool on_komentaru){
	text=NULL;
	if(name)if(*name)load(name);
	if(on_komentaru)delete_komentary();
}



void LOADER::out(char*name){
	ofstream f(name);
	if(!f)cout<<"File "<<name<<" cen'not writed.\n";
	f.write(text,strlen(text));
}


//LoaderCode.h