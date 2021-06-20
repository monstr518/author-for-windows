//	header.cpp
#include "stdafx.h"
#include "header.h"



Xdll::Xdll(const char*namedll){
	way=namedll;
	dll=LoadLibrary(namedll);
	SendString=(void (*)(const char*))GetProcAddress(dll,"SendString");
	RecvString=(const char* (*)())GetProcAddress(dll,"RecvString");
	Questions=Paralele=0;
	Recurse=1;
}


Xdll::~Xdll(){
	FreeLibrary(dll);
}


void Xdll::Send(const char*str){
	(*SendString)(str);
}



string Xdll::Recv(){
	string r;
	r=(*RecvString)();
	return r;
}


bool Xdll::isCorectLoadLibrary(){
	return SendString && RecvString;
}


//---------------------------------------------------------------------------------------
char*ControlerDLL::names[]={
	//?: get code y=f(x) for table: head(x,y){x:1,2;y:5,10;}
	"modul1\\Debug\\modul1.dll",
	
	//?: get code(n) for signal(A,B) {A:1,2;B:0,5,10;} pset{0,4}
	"modul2\\Debug\\modul2.dll",

	//?: get code(x) for row {0,2,4,6} first(0)
	"modul3\\Debug\\modul3.dll",

	//?: get code(n) for signal(A,B) {A:1,3;B:0,2,4;}
	"modul4\\Debug\\modul4.dll",

	//?: get code y=f(x) for table:head(x,y){x:2,3,4;y:4,9,16;}
	"modul5\\Debug\\modul5.dll",

	//?: what result for formul(x*x+x-2) from {x:2, 3, 4;} 
	"formuls\\Debug\\formuls.dll",

	"simplezakonf\\Debug\\simplezakonf.dll",

	"generalizedf\\Debug\\generalizedf.dll",

	"interval\\Debug\\interval.dll",

	NULL
};



ControlerDLL::ControlerDLL(){Gotovo=0;MNames=0;}


ControlerDLL::~ControlerDLL(){
	for(int i=0;i<dlls.size();++i)delete dlls[i];
}



void ControlerDLL::initDLLS(){
	LimitHistory=50;
	for(int i=0;names[i];++i){
		string full=way+"\\SYS\\DLLS\\what\\"+names[i];
		Xdll*D=new(Xdll)(full.c_str());
		if(!D->isCorectLoadLibrary()){
			delete D;
			continue;
			}
		dlls.push_back(D);
		D->Send("name?");
		D->name=D->Recv();
		D->Send("type?");
		string s=D->Recv();
		if(s==".")continue;
		transform(s.begin(), s.end(),s.begin(), tolower );
		V_S Words;
		int p,u=0;
		do{
			p=s.find_first_of("|",u);
			if(p<0){Words.push_back(s.substr(u));break;}
			Words.push_back(s.substr(u,p));
			u=p+1;
			}while(1);
		for(p=0;p<Words.size();++p){
			//cout<<Words[p].c_str()<<endl;
			s=Words[p];
			if(s=="noquestions")D->Questions=0;
			if(s=="questions")D->Questions=1;
			if(s=="noparalele")D->Paralele=0;
			if(s=="paralele")D->Paralele=1;
			if(s=="norecurse")D->Recurse=0;
			if(s=="recurse")D->Recurse=1;
			}
		}
}



void ControlerDLL::Vopros(const char*vopros){
	otvets.clear();
	history.clear();
	Vopros(vopros,otvets,&history);
	nmax=iterator=0;
	for(int i=0;i<otvets.size();++i)nmax+=otvets[i].hto.size();
	Gotovo=1;
	/*
	for(int i=0;i<otvets.size();++i){
		cout<<"-----------------------------------------------------------\n";
		for(int j=0;j<otvets[i].hto.size();++j){
			for(int p=0,nn=0;p<otvets[i].hto[j].length();++p)
				if(otvets[i].hto[j][p]=='\n')++nn;
			cout<<otvets[i].kto->name.c_str()<<": ";
			if(nn)cout<<endl;
			cout<<otvets[i].hto[j].c_str()<<endl;
			}
		cout<<endl;
		}
	*/
}


string ControlerDLL::getOtvets(){
	int p=0;
	for(int i=0;i<otvets.size();++i)
	for(int j=0;j<otvets[i].hto.size();++j,++p)if(p==iterator){
		++iterator;
		if(MNames)return otvets[i].kto->name;
			else return otvets[i].hto[j];
		}
	return "out of range otvetov.";
}



void ControlerDLL::Vopros(const char*vopros,V_Otvet&otvets,V_pXdll*history,string name){
	if(history->size()>LimitHistory)return;
	for(int i=0;i<dlls.size();++i){
		if(name.length())if(name!=dlls[i]->name)continue;
		if(dlls[i]->Paralele){
			if(!dlls[i]->Recurse)
			if(history->size())
			if(*history->rbegin()==dlls[i])continue;
			}else{
			if(find(history->begin(),history->end(),dlls[i])!=history->end())continue;
			}
		//if(i==5)i=i;
		dlls[i]->Send(vopros);
		history->push_back(dlls[i]);
		string s,idvopros,dllname;
		int ER=0;
		do{
			s=dlls[i]->Recv();
			int t,p,ii=s.find('?');
			if(ii<0)break;
			for(p=0;p<ii;++p)if(!(isdigit(s[p])||isalpha(s[p])))ii=-1;
			if(ii<0)break;
			//cout<<s.c_str()<<endl;
			idvopros=s.substr(0,ii);
			t=s.find_first_of(':',ii);
			dllname=s.substr(ii+1,t-ii-1);
			ii=1;
			for(p=0;p<dllname.length();++p)if(dllname[p]!=' ')ii=0;
			if(ii)dllname="";
			s="?"+s.substr(t);
			//cout<<"vopros: "<<s.c_str()<<endl;
			if(!dlls[i]->Questions){ER=1;break;}
			V_S AllOtvets;
			V_Otvet otvets2;
			Vopros(s.c_str(),otvets2,history,dllname);
			for(p=0;p<otvets2.size();++p)
			for(t=0;t<otvets2[p].hto.size();++t)AllOtvets.push_back(otvets2[p].hto[t]);

			if(!AllOtvets.size())dlls[i]->Send(".");
			for(ii=p=0;p<AllOtvets.size();++p){
				for(t=0;t<p;++t)if(AllOtvets[p]==AllOtvets[t])AllOtvets[p]="";
				if(AllOtvets[p].length())++ii;
				}
			if(ii>1){
				char str[10]="{}:";
				sprintf(str+3,"%d",ii);
				dlls[i]->Send(str);
				}
			for(p=0;p<AllOtvets.size();++p)if(AllOtvets[p].length()){
				string z=AllOtvets[p];
				if(idvopros.length())z=idvopros+":"+z;
				//cout<<"otvet: "<<z.c_str()<<endl;
				dlls[i]->Send(z.c_str());
				}
			}while(1);
		history->pop_back();
		if(ER)cout<<"Error type dll module: "<<dlls[i]->way<<endl;
		if(ER==1){
			cout<<"No right on a question: "<<s.c_str()<<endl;
			s=".";
			}
		if(history->empty())dlls[i]->isUnderstend=(s==".");
		if(s=="." || s==":")continue;
		Otvet o;
		o.kto=dlls[i];
		if(s.substr(0,3)=="{}:"){
			int n=0;
			sscanf(s.c_str()+3,"%d",&n);
			for(int j=0;j<n;++j){
				s=dlls[i]->Recv();
				o.hto.push_back(s);
				}
			}else o.hto.push_back(s);
		if(o.hto.empty())continue;
		otvets.push_back(o);
		}
}



void ControlerDLL::setCout(bool on,int n){
	const char*s=on?"set cout on":"set cout off";
	if(n>=0 && n<dlls.size()){dlls[n]->Send(s);return;}
	for(int i=0;i<dlls.size();++i)dlls[i]->Send(s);
}



string ControlerDLL::getExample(string name){
	string R;
	for(int i=0;i<dlls.size();++i){
		if(!name.empty())if(name!=dlls[i]->name)continue;
		dlls[i]->Send("example?");
		R+=dlls[i]->Recv();
		}
	return R;
}


//	header.cpp	:-|