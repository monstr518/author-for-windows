//	scaner.cpp
#include "main.h"



//--------------------------------------------------------------------------------------------------
void SCANER::noProbel(const char*&s){
	while(*s==' ' || *s=='	' || *s=='\n' || *s=='\r')++s;
}


char*SCANER::toString(int i){
	static char s[10];
	sprintf(s,"%d",i);
	return s;
}


char*SCANER::toString(double i){
	static char s[20];
	sprintf(s,"%f",i);
	return s;
}



string SCANER::writeChar(char c){
	string s=" ";
	s[0]=c;
	if(c=='\\')s="\\\\";
	if(c=='\n')s="\\n";
	s="'"+s+"'";
	return s;
}


string SCANER::writeString(const string&s){
	string R="\"";
	char c;
	int n=s.size(),i=0;
	for(;i<n;++i){
		c=s[i];
		if(c==10){
			R+="\\n";
			continue;
			}
		if(c=='\r'){
			R+="\\r";
			continue;
			}
		if(c=='\\' || c=='\"')R+='\\';
		R+=c;
		}
	R+="\"";
	return R;
}



string SCANER::convertString(string&s){
	string R;
	char c;
	int i=1,n=s.size()-1;
	for(;i<n;++i){
		c=s[i];
		if(c=='\\'){
			c=s[++i];
			if(c=='n')c='\n';
			if(c=='r')c='\r';
			}
		R+=c;
		}
	return R;
}



string SCANER::convertChar(string&s){
	if(s=="'\\n'")return "\n";
	if(s=="'\\\\'")return "\\";
	return s.substr(1,1);
}


void SCANER::randomize(){
	long t;
	time(&t);
	srand(t);
}



string SCANER::replaceAll(string s,string a,string na){
	int i=0;
	string r;
	do{
		i=s.find(a);
		if(i<0)break;
		r+=s.substr(0,i);
		r+=na;
		i+=a.length();
		s=s.substr(i);
		}while(1);
	r+=s;
	return r;
}


//--------------------------------------------------------------------------------------------------
bool SCANER::isDigit(char c,int sus,int*t){
	int n=-1;
	if(isdigit(c))n=c-'0';
	if(c>='A'&&c<='Z')n=c-'A'+10;
	if(c>='a'&&c<='z')n=c-'a'+10;
	if(t)*t=n;
	return (n>-1 && n<sus);
}


void SCANER::scanDigit(const char*&p,string&s,int sus){
	while(SCANER::isDigit(*p,sus))s+=*(p++);
}


unsigned long double SCANER::getNumber(string&s,int sus){
	unsigned long double R=0;
	int t;
	int i;
	for(i=0;i<s.length();i++){
		R*=sus;
		isDigit(s[i],sus,&t);
		if(t>=sus)return R;
		R+=t;
		}
	return R;
}





long double SCANER::f_digit(const char*s){
	noProbel(s);
	long double A=0,B=0;
	int ST=0,i=0;
	if(*s=='0' && *(s+1)!='.'){
		//06456435
		//0xFFee
		//0b10010010100101
		++s;
		int sus=8;
		if(*s=='X'||*s=='x'){++s;sus=16;}
		if(*s=='B'||*s=='b'){++s;sus=2;}
		if(*s=='T'||*s=='t'){++s;sus=3;}
		string ss;
		scanDigit(s,ss,sus);
		A=getNumber(ss,sus);
		}else{
		//76548
		//12143e+321
		//154.31232143e-321
		//10312.321323265
		string ss,d,po;
		scanDigit(s,ss,10);
		if(*s=='.'){
			++s;
			scanDigit(s,d,10);
			}
		if(*s=='e'||*s=='E'){
			++s;
			int z=1;
			if(*s=='+')++s;else
			if(*s=='-'){++s;z=0;}
			scanDigit(s,po,10);
			ST=getNumber(po,10);
			if(z==0)ST=-ST;
			}
		A=getNumber(ss,10);
		B=getNumber(d,10);
		for(i=0;i<d.size();++i)B/=10;
		}
	long double R=A+B;
	if(ST>0) for(i=0;i<ST;++i)R*=10;
		else for(i=0;i>ST;--i)R/=(double)10;
	return R;
}



string SCANER::replaceSpecSumbolHTML(const char*s){
	const char*Spec="<>&	\"\n";
	const char*Alter[]={"&#60;","&#62;","&#38;",
		"&#160;&#160;&#160;&#160;","&#34;","<br/>\n"};
	string t;
	int n=strlen(s),i=0,j;
	for(;i<n;++i){
		for(j=0;Spec[j];++j)if(s[i]==Spec[j]){
			t+=Alter[j];
			break;
			}
		if(Spec[j])continue;
		t+=s[i];
		}
	return t;
}



int SCANER::findNumberStringLine(const char*begin,const char*s){
	int n=1;
	for(;begin;++begin){
		if(*begin=='\n')++n;
		if(begin==s)break;
		}
	if(!begin)n=0; //не найдено.
	return n;
}


string SCANER::trim(string s){
	int i;
	for(i=0;i<s.c_str()[i];++i)if(s.c_str()[i]!=' '){s=s.substr(i);break;}
	for(i=s.length()-1;i>=0;--i)if(s.c_str()[i]!=' ')break;
	s=s.substr(0,i+1);
	return s;
}


//--------------------------------------------------------------------------------------------------
string SCANER::readName(const char*&s){
	string str;
	if(!(isalpha(*s) || *s=='_'))return str;
	str=*s;
	++s;
	while(isalpha(*s) || isdigit(*s) || *s=='_'){
		str+=*s;
		++s;
		}
	return str;
}



//прочитать строку как есть.
string SCANER::readString(const char*&s){
	string str;
	if(*s!='"')return str;
	str+=*s;
	++s;
	while(*s && *s!='"'){
		if(*s=='\\'){str+=*s;++s;}
		str+=*s;
		++s;
		}
	if(!*s)return "";
	str+=*s;
	++s;
	return str;
}


string SCANER::readChar(const char*&s){
	string str;
	if(*s!='\'')return str;
	str+=*s;
	++s;
	if(*s=='\\'){str+=*s;++s;}
	str+=*s;
	++s;
	if(*s!='\'')return "";
	str+=*s;
	++s;
	return str;
}


string SCANER::readDigit(const char*&s){
	string str;
	if(!isdigit(*s))return "";
	bool h=0;
	if(*s=='0'){
		str+=*s;
		++s;
		if(*s=='x' || *s=='X'){
			str+=*s;
			++s;
			h=1;
			}else if(*s=='t' || *s=='T'){
			str+=*s;
			++s;
			}
		}
	while(isdigit(*s) || (*s>='a' && *s<='f') || (*s>='A' && *s<='F')){
		str+=*s;
		++s;
		if(!h)if(*s=='e' || *s=='E')break;
		}
	if(*s=='.'){
		str+=*s;
		++s;
		while(isdigit(*s) || (*s>='a' && *s<='f') || (*s>='A' && *s<='F')){
			str+=*s;
			++s;
			if(!h)if(*s=='e' || *s=='E')break;
			}
		}
	if(*s=='e' || *s=='E'){
		str+=*s;
		++s;
		if(*s=='+' || *s=='-'){str+=*s;++s;}
		while(isdigit(*s)){str+=*s;++s;}
		}
	return str;
}



int SCANER::scanSlovo(const char*slovo,const char*&s){
	int i=strncmp(slovo,s,strlen(slovo));
	if(i)return 0;
	s+=strlen(slovo);
	return 1;
}



void SCANER::poTabu(string&u){
	int i;
	for(i=0;i<u.size();++i)
		if(u[i]=='\n'){
			++i;
			u.insert(i,"	");
			}
}




//	scaner.cpp	:-|