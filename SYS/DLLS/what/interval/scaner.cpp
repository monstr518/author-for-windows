// scaner.cpp
#include "stdafx.h"
#include "interval.h"


void SCANER::noProbel(const char*&s){
	while(*s==' ' || *s=='	' || *s=='\n' || *s=='\r')++s;
}


bool SCANER::getDigit(const char*&s,double&x){
	const char*s2=s;
	if(*s=='-')++s;
	while(isdigit(*s))++s;
	if(*s=='.'){
		++s;
		while(isdigit(*s))++s;
		}
	sscanf(s2,"%lf",&x);
	return s2<s;
}


string SCANER::getName(const char*&s){
	if(!isalpha(*s))return "";
	string t;
	while(isalpha(*s) || isdigit(*s)){
		t+=*s;
		++s;
		}
	return t;
}


int SCANER::scanSlovo(const char*slovo,const char*&s){
	int i=strncmp(slovo,s,strlen(slovo));
	if(i)return 0;
	s+=strlen(slovo);
	return 1;
}



// scaner.cpp :-|