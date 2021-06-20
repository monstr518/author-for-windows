// UGraf.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "asemble.h"
#include "head.h"



BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}

//--------------------------------------------------------------------------------------------------
MenagerUG Director;

MenagerUG::MenagerUG(){Nfree=1;}

MenagerUG::~MenagerUG(){
	M_IUG::iterator it=ALLUG.begin();
	for(;it!=ALLUG.end();++it)delete it->second;
}

void MenagerUG::clear(){
	M_IUG::iterator it=ALLUG.begin();
	for(;it!=ALLUG.end();++it)delete it->second;
	ALLUG.clear();
	Nfree=1;
}


void MenagerUG::NewUG(){
	UGraf*UG=new(UGraf);
	ALLUG[Nfree]=UG;
	pos=Nfree;
	++Nfree;
}


void MenagerUG::getPos(){
	buf=SCANER::toString(pos);
}


//NET(1,-,slovo 1,>,2)
//NET(1,-,"mf;l\"vfl;d",>,2)
void MenagerUG::NET(string str){
	str=str.substr(4);
	int a=0,b=0,i;
	bool x,y;
	sscanf(str.c_str(),"%d",&a);
	i=str.find(",");
	if(i<0)return;
	str=str.substr(i+1);
	x=(UGraf::getVector(str[0])==1);
	i=str.find(",");
	if(i<0)return;
	str=str.substr(i+1);
	string m;
	if(str[0]!=','){
		if(str[0]=='"'){
			const char*c=str.c_str();
			m=SCANER::getString(c);
			str=++c;
			}else{
			i=str.find(",");
			if(i<0)return;
			m=str.substr(0,i);
			str=str.substr(i+1);
			}
		}else str=str.substr(1);
	//cout<<str.c_str()<<endl;//
	y=(UGraf::getVector(str[0])==1);
	i=str.find(",");
	if(i<0)return;
	str=str.substr(i+1);
	sscanf(str.c_str(),"%d",&b);
	NetLine*net=new NetLine(a,x,m.c_str(),y,b);
	//cout<<net->toString().c_str()<<endl;
	UGraf*UG=ALLUG[pos];
	if(UG)UG->add(net);else delete net;
}


//MARKER(1,-,"",-,"")
void MenagerUG::MARKER(string str){
	str=str.substr(7);
	int a=0,i;
	bool x,y;
	sscanf(str.c_str(),"%d",&a);
	i=str.find(",");
	if(i<0)return;
	str=str.substr(i+1);
	x=(UGraf::getVector(str[0])==1);
	i=str.find(",");
	if(i<0)return;
	str=str.substr(i+1);
	string m,mm;
	if(str[0]!=','){
		if(str[0]=='"'){
			const char*c=str.c_str();
			m=SCANER::getString(c);
			str=++c;
			}else{
			i=str.find(",");
			if(i<0)return;
			m=str.substr(0,i);
			str=str.substr(i+1);
			}
		}else str=str.substr(1);
	y=(UGraf::getVector(str[0])==1);
	i=str.find(",");
	if(i<0)return;
	str=str.substr(i+1);
	if(str[0]!=')'){
		if(str[0]=='"'){
			const char*c=str.c_str();
			mm=SCANER::getString(c);
			str=++c;
			}else{
			i=str.find(")");
			if(i<0)return;
			mm=str.substr(0,i);
			str=str.substr(i+1);
			}
		}
	//cout<<str.c_str()<<endl;//
	MarkerLine*mnet=new MarkerLine(a,x,m.c_str(),y,mm.c_str());
	//cout<<mnet->toString().c_str()<<endl;
	UGraf*UG=ALLUG[pos];
	if(UG)UG->add(mnet);else delete mnet;
}


void MenagerUG::getNewUnitNumber(){
	UGraf*UG=ALLUG[pos];
	buf="";
	if(!UG)return;
	buf=SCANER::toString(UG->nfree++);
}



//GETNET(,,,,)
void MenagerUG::GETNET(string str){
	str=str.substr(7);
	int a=-1,b=-1,i;
	int x=2,y=2,z=0;
	i=str.find(",");
	if(i<0)return;
	if(i>0)sscanf(str.c_str(),"%d",&a);
	str=str.substr(i+1);
	i=str.find(",");
	if(i<0)return;
	if(i>0)x=(UGraf::getVector(str[0])==1);
	str=str.substr(i+1);
	string m;
	if(str[0]!=','){
		if(str[0]=='"'){
			const char*c=str.c_str();
			m=SCANER::getString(c);
			str=++c;
			z=1;
			}
		}else str=str.substr(1);
	i=str.find(",");
	if(i<0)return;
	if(i>0)y=(UGraf::getVector(str[0])==1);
	str=str.substr(i+1);
	i=str.find(")");
	if(i<0)return;
	if(i>0)sscanf(str.c_str(),"%d",&b);
	UGraf*UG=ALLUG[pos];
	if(!UG){
		buf="0";
		return;
		}
	Result.clear();
	V_BL vbl;
	UG->findNet(vbl,a,x,z?&m:NULL,y,b);
	for(i=0;i<vbl.size();++i){
		NetLine*net=(NetLine*)vbl[i];
		str="";
		bool revers=0,xx,yy;
		xx=net->c&1;
		yy=(net->c>>1)&1;
		if(a>=0 && b>=0){
			revers=(net->a==b);
			}else if(a>=0 || b>=0){
			if(a>=0)revers=(net->b==a);
			if(b>=0)revers=(net->a==b);
			}
		if(a<0)str+=SCANER::toString(revers?net->b:net->a);
		if(x==2){
			if(str.size())str+=",";
			str+=(revers?yy:xx)?"<":"-";
			}
		if(!z){
			if(str.size())str+=",";
			str+=SCANER::toSleh(net->name);
			}
		if(y==2){
			if(str.size())str+=",";
			str+=(revers?xx:yy)?">":"-";
			}
		if(b<0){
			if(str.size())str+=",";
			str+=SCANER::toString(revers?net->a:net->b);
			}
		Result.push_back(str);
		}
	Result.push_back(SCANER::toString(Result.size()));
}



//GETMARKER(,,,,)
void MenagerUG::GETMARKER(string str){
	str=str.substr(10);
	int a=-1,i;
	int x=2,y=2,z=0,zz=0;
	i=str.find(",");
	if(i<0)return;
	if(i>0)sscanf(str.c_str(),"%d",&a);
	str=str.substr(i+1);
	i=str.find(",");
	if(i<0)return;
	if(i>0)x=(UGraf::getVector(str[0])==1);
	str=str.substr(i+1);
	string m,mm;
	if(str[0]!=','){
		if(str[0]=='"'){
			const char*c=str.c_str();
			m=SCANER::getString(c);
			str=++c;
			z=1;
			}
		}else str=str.substr(1);
	i=str.find(",");
	if(i<0)return;
	if(i>0)y=(UGraf::getVector(str[0])==1);
	str=str.substr(i+1);
	if(str[0]!=')'){
		if(str[0]=='"'){
			const char*c=str.c_str();
			mm=SCANER::getString(c);
			str=++c;
			zz=1;
			}
		}
	UGraf*UG=ALLUG[pos];
	if(!UG){
		buf="0";
		return;
		}
	Result.clear();
	V_BL vbl;
	UG->findMarker(vbl,a,x,z?&m:NULL,y,zz?&mm:NULL);
	for(i=0;i<vbl.size();++i){
		MarkerLine*net=(MarkerLine*)vbl[i];
		str="";
		bool xx,yy;
		xx=net->c&1;
		yy=(net->c>>1)&1;
		if(a<0)str+=SCANER::toString(net->a);
		if(x==2){
			if(str.size())str+=",";
			str+=xx?"<":"-";
			}
		if(!z){
			if(str.size())str+=",";
			str+=SCANER::toSleh(net->name);
			}
		if(y==2){
			if(str.size())str+=",";
			str+=yy?">":"-";
			}
		if(!zz){
			if(str.size())str+=",";
			str+=SCANER::toSleh(net->marker);
			}
		Result.push_back(str);
		}
	Result.push_back(SCANER::toString(Result.size()));
}


//GETWAY(start,end)
void MenagerUG::GETWAY(string str){
	str=str.substr(7);
	int i;
	string m1,m2;
	if(str[0]=='"'){
		const char*c=str.c_str();
		m1=SCANER::getString(c);
		str=++c;
		}else{
		i=str.find(",");
		if(i<0)return;
		m1=str.substr(0,i);
		str=str.substr(i+1);
		}
	if(str[0]=='"'){
		const char*c=str.c_str();
		m2=SCANER::getString(c);
		str=++c;
		}else{
		i=str.find(")");
		if(i<0)return;
		m2=str.substr(0,i);
		str=str.substr(i+1);
		}
	UGraf*UG=ALLUG[pos];
	if(!UG){
		buf="0";
		return;
		}
	//cout<<m1.c_str()<<endl;cout<<m2.c_str()<<endl;
	V_BL vbl;
	UG->findMarker(vbl,-1,2,NULL,2,&m1);
	if(vbl.size()<1)return;
	M_II mii;
	for(i=0;i<vbl.size();++i)mii[vbl[i]->a]=1;
	vbl.clear();
	UG->findMarker(vbl,-1,2,NULL,2,&m2);
	if(vbl.size()<1)return;
	V_I Fine;
	for(i=0;i<vbl.size();++i)Fine.push_back(vbl[i]->a);
	vbl.clear();

	int n=1,v=-1,revers,to,ok=1;
	while(ok){
		ok=0;
		M_II::iterator it=mii.begin();
		for(;it!=mii.end();++it)if(it->second==n){
			for(i=0;i<Fine.size();++i)if(it->first==Fine[i]){
				v=it->first;
				break;
				}
			if(v>=0)break;
			UG->findNet(vbl,it->first,0,NULL,1,-1);
			for(i=0;i<vbl.size();++i){
				NetLine*net=(NetLine*)vbl[i];
				revers=(net->b==it->first);
				to=revers?net->a:net->b;
				if(mii.find(to)==mii.end()){
					mii[to]=n+1;
					ok=1;
					}
				}
			vbl.clear();
			}
		++n;
		}
	if(v<0){
		buf="NOWAY";
		return;
		}
	//buf="WAY OK";
	V_I WAY;
	int k=mii[v];
	do{
		WAY.push_back(v);
		UG->findNet(vbl,-1,0,NULL,1,v);
		for(i=0;i<vbl.size();++i){
			NetLine*net=(NetLine*)vbl[i];
			revers=(net->a==v);
			to=revers?net->b:net->a;
			if(mii[to]==k-1)v=to;
			}
		vbl.clear();
		--k;
		}while(k>0);
	buf="";
	for(i=WAY.size()-1;i>=0;--i){
		if(buf.size())buf+=",";
		buf+=SCANER::toString(WAY[i]);
		}
}


//--------------------------------------------------------------------------------------------------
MODUL_API void SendString(char*s){
	string str=s;
	if(str=="" || str=="HELP")Director.buf="FUNCTIONS:new";
	if(str.substr(0,5)=="HELP:"){
		str=str.substr(5);
		if(str=="new")Director.buf="new - Создать новый универсальный граф.";

		return;
		}
	if(str=="new")Director.NewUG();
	if(str=="getActivNumber")Director.getPos();
	if(str.substr(0,3)=="NET")Director.NET(str);
	if(str.substr(0,6)=="MARKER")Director.MARKER(str);
	if(str=="getNewUnitNumber")Director.getNewUnitNumber();
	if(str.substr(0,6)=="GETNET")Director.GETNET(str);
	if(str.substr(0,9)=="GETMARKER")Director.GETMARKER(str);
	if(str.substr(0,6)=="GETWAY")Director.GETWAY(str);

}



MODUL_API char* RecvString(){
	if(Director.Result.size()){
		Director.buf=*Director.Result.rbegin();
		Director.Result.pop_back();
		}
	return const_cast<char*>(Director.buf.c_str());
}
