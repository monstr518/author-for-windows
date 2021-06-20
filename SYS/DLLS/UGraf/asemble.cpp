//	asemble.cpp

#include "stdafx.h"
#include "asemble.h"


void SCANER::noProbel(const char*&s){
	while(*s==' ' || *s=='	' || *s=='\n')++s;
}

char*SCANER::toString(int i){
	static char s[10];
	sprintf(s,"%d",i);
	return s;
}


//	text"2	=>	text\"2
string SCANER::toSleh(string&s){
	string r;
	int i;
	for(i=0;i<s.size();++i){
		if(s[i]=='\\' || s[i]=='"')r+='\\';
		r+=s[i];
		}
	string t="\"";
	t+=r;
	t+="\"";
	return t;
}


bool SCANER::read(char*&s,char*t){
	char*ss=s;
	for(;*t;++t,++ss)
		if(*ss!=*t)return 0;
	s=ss;
	return 1;
}


int SCANER::getInt(char*&s){
	int r=0;
	while(isdigit(*s)){r=r*10+(*s-'0');++s;}
	return r;
}

//	text\"ttt	=>	text"ttt
string SCANER::getString(const char*&s){
	if(*s!='"')return "";
	const char*ss=s;
	++ss;
	string r;
	while(*ss!='"'){
		if(*ss=='\\')++ss;
		r+=*ss;
		++ss;
		}
	++ss;
	s=ss;
	return r;
}



string SCANER::getName(char*&s){
	if(!isalpha(*s))return "";
	string t;
	while(isalpha(*s) || isdigit(*s))t+=*(s++);
	return t;
}


//-----------------------------------------------------------------------------------------
BasisLine::BasisLine(const int t,const char i){a=t;c=i;}
BasisLine::BasisLine(int t,char i,const char*s){
	a=t;
	c=i;
	name=s;
}

BasisLine::~BasisLine(){}

BasisLine::BasisLine(const BasisLine&t){
	a=t.a;
	c=t.c;
	name=t.name;
}


string BasisLine::getStr(){
	string S;
	S+=SCANER::toString(a);
	S+=",";
	char aa=c&1,bb=c&2;
	aa=(aa?'<':'-');
	bb=(bb?'>':'-');
	S+=aa;
	S+=",";
	S+=SCANER::toSleh(name);
	S+=",";
	S+=bb;
	S+=",";
	return S;
}



/*
//-----------------------------------------------------------------------------------------
BasisComand::~BasisComand(){}
bool BasisComand::getParameter(string name,BasisLine*h,int*i,string**ss,bool r){
	bool ok=0;
	if(!h)return ok;
	if(i){
		int ia,ib=0,*pp=NULL;
		*i=0;
		ia=h->a;
		if(name=="a" || name=="A"){pp=&ia;ok=1;}
		char xx,yy;
		h->CtoAB(h->c,xx,yy);
		if(name=="x" || name=="X"){*i=(r?yy:xx);ok=1;}
		if(name=="y" || name=="Y"){*i=(r?xx:yy);ok=1;}
		NetLine*nl=dynamic_cast<NetLine*>(h);
		if(nl)ib=nl->b;
		if(name=="b" || name=="B"){
			pp=&ib;
			ok=1;
			}
		if(ok && pp){
			*i=*pp;
			if(r)*i=(pp==&ia)?ib:ia;
			}
		}
	if(ss){
		*ss=NULL;
		if(name=="name" || name=="NAME"){*ss=h->name;ok=1;}
		if(name=="marker" || name=="MARKER"){
			MarkerLine*ml=dynamic_cast<MarkerLine*>(h);
			*ss=ml->marker;
			ok=1;
			}
		}
	return ok;
}



bool BasisComand::setParameter(string name,BasisLine*h,int i,bool r){
	bool ok=0;
	if(!h)return ok;
	char xx,yy;
	h->CtoAB(h->c,xx,yy);
	if(name=="x" || name=="X"){(r?yy:xx)=i;ok=1;}
	if(name=="y" || name=="Y"){(r?xx:yy)=i;ok=1;}
	if(ok)h->c=h->ABtoC(xx,yy);
	int k=0;
	if(name=="a" || name=="A")k=1;
	if(name=="b" || name=="B")k=2;
	if(r)k=3-k;
	if(k==1){h->a=i;ok=1;}
	if(k==2){
		NetLine*nl=dynamic_cast<NetLine*>(h);
		if(!nl)return 0;
		nl->b=i;
		ok=1;
		}
	return ok;
}



bool BasisComand::setParameter(string name,BasisLine*h,string*ss){
	bool ok=0;
	if(!h)return ok;
	if(name=="name" || name=="NAME"){
		if(h->name && ss)*h->name=*ss;
		if(h->name && !ss){delete h->name;h->name=ss;}
		if(!h->name && ss)h->name=new(string)(*ss);
		ok=1;
		}
	if(name=="marker" || name=="MARKER"){
		MarkerLine*ml=dynamic_cast<MarkerLine*>(h);
		if(!ml)return 0;
		if(ml->marker && ss)*ml->marker=*ss;
		if(ml->marker && !ss){delete ml->marker;ml->marker=ss;}
		if(!ml->marker && ss)ml->marker=new(string)(*ss);
		ok=1;
		}
	return ok;
}

*/


//------------------------------------------------------------------------------------------------
NetLine::NetLine(int a,char x,const char*s,char y,int bb):BasisLine(a,x|(y<<1),s){b=bb;}
NetLine::~NetLine(){}

string NetLine::toString(){
	string S("NET(");
	S+=getStr();
	S+=SCANER::toString(b);
	S+=")";
	return S;
}

NetLine::NetLine(const NetLine&t):BasisLine(t){b=t.b;}

//------------------------------------------------------------------------------------------------
MarkerLine::MarkerLine(int a,char x,const char*s,char y,const char*m):BasisLine(a,x|(y<<1),s){
	marker=m;
}

MarkerLine::~MarkerLine(){}

string MarkerLine::toString(){
	string S("MARKER(");
	S+=getStr();
	S+=SCANER::toSleh(marker);
	S+=")";
	return S;
}

MarkerLine::MarkerLine(const MarkerLine&t):BasisLine(t){
	marker=t.marker;
}




//------------------------------------------------------------------------------------------------
UGraf::UGraf(){nfree=1;}

UGraf::~UGraf(){
	/*
	V_BL::iterator it=nets.begin();
	for(;it!=nets.end();++it)delete *it;
	*/
	int i;
	for(i=0;i<lincs.size();++i)delete lincs[i];
	for(i=0;i<markers.size();++i)delete markers[i];
}


UGraf::UGraf(const UGraf&t){*this=t;}


UGraf& UGraf::operator = (const UGraf&t){
	V_NL::const_iterator it=t.lincs.begin();
	for(;it!=t.lincs.end();++it){
		NetLine*nl=new NetLine(**it);
		lincs.push_back(nl);
		}
	V_ML::const_iterator jt=t.markers.begin();
	for(;jt!=t.markers.end();++jt){
		MarkerLine*ml=new MarkerLine(**jt);
		markers.push_back(ml);
		}
	nfree=t.nfree;
	return *this;
}



int UGraf::getnew(){return nfree++;}


void UGraf::add(NetLine*n){
	lincs.push_back(n);
	if(n->a>=nfree)nfree=n->a+1;
	if(n->b>=nfree)nfree=n->b+1;
}

void UGraf::add(MarkerLine*m){
	markers.push_back(m);
	if(m->a>=nfree)nfree=m->a+1;
}


// 0- 1<
char UGraf::getVector(const char s){
	if(s=='<' || s=='>')return 1;
	if(s=='|' || s=='-')return 0;
	return 2;
}


string UGraf::toString(){
	string s;
	V_NL::iterator it=lincs.begin();
	for(;it!=lincs.end();++it){
		s+=(*it)->toString();
		s+="\n";
		}
	V_ML::iterator jt=markers.begin();
	for(;jt!=markers.end();++jt){
		s+=(*jt)->toString();
		s+="\n";
		}
	return s;
}




int UGraf::getUnits(S_I&si){
	V_NL::iterator it=lincs.begin();
	for(;it!=lincs.end();++it){
		NetLine*nl=*it;
		si.insert(nl->a);
		si.insert(nl->b);
		}
	V_ML::iterator jt=markers.begin();
	for(;jt!=markers.end();++jt)si.insert((*jt)->a);
	return si.size();
}




void UGraf::outMAP(M_II*mii){
	M_II::iterator it=mii->begin();
	for(;it!=mii->end();++it){
		cout<<SCANER::toString(it->first)<<" ]==[ ";
		cout<<SCANER::toString(it->second)<<endl;
		}
	cout<<endl;
}



// x-  0- 1< 2?
void UGraf::findMarker(V_BL&vbl,int a,char x,string*n,char y,string*m){
	char xx,yy;
	V_ML::iterator it=markers.begin();
	for(;it!=markers.end();++it){
		MarkerLine*ml=*it;
		if(a>=0)if(ml->a!=a)continue;
		xx=ml->c&1;
		yy=(ml->c>>1)&1;
		if(x<2)if(xx!=x)continue;
		if(y<2)if(yy!=y)continue;
		if(n)if(ml->name!=*n)continue;
		if(m)if(ml->marker!=*m)continue;
		vbl.push_back(ml);
		}
}



// x-  0- 1< 2?
void UGraf::findNet(V_BL&vbl,int a,char x,string*n,char y,int b){
	bool revers=0;
	char xx,yy;
	V_NL::iterator it=lincs.begin();
	for(;it!=lincs.end();++it){
		NetLine*nl=*it;
		if(a>=0 && b>=0){
			revers=(nl->a==b && nl->b==a);
			if(!((nl->a==a && nl->b==b) || revers))continue;
			}else{
			if(a>=0){
				revers=(nl->b==a);
				if(!((nl->a==a) || revers))continue;
				}
			if(b>=0){
				revers=(nl->a==b);
				if(!((nl->b==b) || revers))continue;
				}
			}
		xx=nl->c&1;
		yy=(nl->c>>1)&1;
		if(x<2)if(revers?(yy!=x):(xx!=x))continue;
		if(y<2)if(revers?(xx!=y):(yy!=y))continue;
		if(n){
			if(nl->name!=*n)continue;
			}
		vbl.push_back(nl);
		}
}



void UGraf::findNetBlizko(V_NL&vnl,int i){
	V_NL::iterator it=lincs.begin();
	for(;it!=lincs.end();++it){
		NetLine*nl=*it;
		if(nl->a==i || nl->b==i)vnl.push_back(nl);
		}
}



//sub ==
void UGraf::getNumberLinksTable(V_I&vso,M_II&io,const V_NL&lincs){
	V_NL::const_iterator it=lincs.begin();
	for(;it!=lincs.end();++it){
		NetLine*nl=*it;
		++io[nl->a];
		++io[nl->b];
		}
	M_II::iterator j=io.begin();
	for(;j!=io.end();++j)vso.push_back(j->second);
	sort(vso.begin(),vso.end());
}



I2::I2(){fn=i=0;}


bool UGraf::operator == (const UGraf&t){
	if(t.lincs.size()!=lincs.size())return 0;
	if(t.markers.size()!=markers.size())return 0;
	if(!(t.lincs.size()+t.markers.size()))return 1;
	V_I  so,tso;
	M_II io,tio;
	getNumberLinksTable(so,io,lincs);
	getNumberLinksTable(tso,tio,t.lincs);
	if(!equal(so.begin(),so.end(),tso.begin()))return 0;
	int p,n,nn;
	n=*so.rbegin();
	M_II::iterator itm=tio.begin();
	for(;itm!=tio.end();++itm)
		if(itm->second==n){nn=n=itm->first;break;}
	bool ret=0;
	V_NL	vnl;
		{
		S_I		complect;
		DE_I	Dest;
		Dest.push_back(n);
		while(!Dest.empty()){
			n=*Dest.begin();
			Dest.pop_front();
			if(!complect.insert(n).second)continue;
			V_NL vnlb;
			const_cast<UGraf&>(t).findNetBlizko(vnlb,n);
			V_NL::iterator it=vnlb.begin();
			for(;it!=vnlb.end();++it){
				if(find(vnl.begin(),vnl.end(),*it)!=vnl.end())continue;
				p=(*it)->a;
				if(p==n)p=(*it)->b;
				Dest.push_back(p);
				vnl.push_back(*it);
				}
			}
		}
	NetLine*nl,*nl2;
	I2*i2=new(I2),*ii2;
	i2->fn=nn;
	DE_I2 dei2;
	dei2.push_back(i2);
	int A,B;
	char x,xx,y,yy;
	bool nok;
	i2=NULL;
	while(!dei2.empty()){
		if(i2)delete i2;
		i2=*dei2.rbegin();
		dei2.pop_back();
		if(i2->fn){
			A=tio[i2->fn];
			itm=io.begin();
			for(;itm!=io.end();++itm){
				if(itm->second!=A)continue;
				if(!UGraf::equale(
					const_cast<UGraf*>(&t),this,i2->fn,itm->first))continue;
				ii2=new I2(*i2);
				ii2->fn=0;
				ii2->mii[i2->fn]=itm->first;
				dei2.push_back(ii2);
				}
			continue;
			}
		if(vnl.size()<=i2->i){ret=1;break;}
		nl=vnl[i2->i];
		A=i2->mii[nl->a];
		B=i2->mii[nl->b];
		if(!A || !B){
			if(A)A=nl->b;else A=nl->a;
			i2->fn=A;
			dei2.push_back(i2);
			i2=NULL;
			continue;
			}
		V_NL::iterator it=lincs.begin();
		for(;it!=lincs.end();++it)
			if(find(i2->vnlok.begin(),i2->vnlok.end(),*it)==i2->vnlok.end()){
				nl2=*it;
				nok=(nl2->a==B && nl2->b==A);
				if(!(nok || (nl2->a==A && nl2->b==B)))continue;
				if(nl2->name!=nl->name)continue;
				x=nl->c&1;
				y=(nl->c>>1)&1;
				xx=nl2->c&1;
				yy=(nl2->c>>1)&1;
				if(!(nok?(y==xx && x==yy):(x==xx && y==yy)))continue;
				ii2=new I2(*i2);
				++ii2->i;
				ii2->vnlok.push_back(nl2);
				dei2.push_back(ii2);
				}
		}
	if(i2)delete i2;
	DE_I2::iterator id=dei2.begin();
	for(;id!=dei2.end();++id)delete *id;
	return ret;
}





// сравнит маркеры узлов 
bool UGraf::equale(UGraf*UGA,UGraf*UGB,int a,int b){
	char x,y,xx,yy;
	V_BL vbl_a,vbl_b;
	UGA->findMarker(vbl_a,a,2,NULL,2,NULL);
	UGB->findMarker(vbl_b,b,2,NULL,2,NULL);
	if(vbl_a.size()!=vbl_b.size())return 0;
	V_BL::iterator at=vbl_a.begin(),bt;
	for(;at!=vbl_a.end();++at){
		x=(*at)->c&1;
		y=((*at)->c>>1)&1;
		bt=vbl_b.begin();
		for(;bt!=vbl_b.end();++bt)if(*bt){
			xx=(*bt)->c&1;
			yy=((*bt)->c>>1)&1;
			if(x!=xx || y!=yy)continue;
			if((*bt)->name!=(*at)->name)continue;
			string	ssa=((MarkerLine*)(*at))->marker,
					ssb=((MarkerLine*)(*bt))->marker;
			if(ssa!=ssb)continue;
			*bt=NULL;
			break;
			}
		if(bt==vbl_b.end())return 0;
		}
	return 1;
}


/*
//--------------------------------------------------------------------------------------------------
string Param::toString(){
	string s=name;
	if(!p.empty()){s+=".";s+=p;}
	return s;
}

//--------------------------------------------------------------------------------------------------
//BasisDetale::BasisDetale(){name=NULL;a=0;}
BasisDetale::BasisDetale(int i,Param&pa,Param&pb,Param&v1,Param&v2,Param&n,string*s){
	a=i;
	PA=pa;
	PB=pb;
	V1=v1;
	V2=v2;
	Name=n;
	name=(s?new(string)(*s):NULL);
}


BasisDetale::~BasisDetale(){
	if(name)delete name;
}


string BasisDetale::getString(){
	string s="(";
	if(a)s+=SCANER::toString(a);else s+=PA.toString();
	s+=",";
	if(x&8)s+=V1.toString();else{
		if(x&2)s+="<";
		if(x&1)s+="-";
		if(x&4)s+="?";
		}
	s+=",";
	if(name)s+='"'+SCANER::toSleh(*name)+'"';else if(!Name.name.empty())s+=Name.toString();
	s+=",";
	if(y&8)s+=V2.toString();else{
		if(y&4)s+="?";
		if(y&1)s+="-";
		if(y&2)s+=">";
		}
	s+=",";
	return s;
}


//--------------------------------------------------------------------------------------------------
NetComand::NetComand(int i,int j,Param&pa,Param&pb,Param&v1,Param&v2,Param&n,string*s)
:BasisDetale(i,pa,pb,v1,v2,n,s){
	b=j;
	//virtuale=0;
}

NetComand::~NetComand(){}

string NetComand::toString(){
	string s;
	if(no)s="NO";
	s+="NET";
	s+=getString();
	if(b){
		if(!PB.p.empty())s+="0";
		s+=SCANER::toString(b);
		}else s+=PB.toString();
	s+=")";
	return s;
}

//--------------------------------------------------------------------------------------------------
MarkerComand::MarkerComand(int i,Param&pa,Param&pb,Param&v1,Param&v2,Param&n,string*sn,string*sm)
:BasisDetale(i,pa,pb,v1,v2,n,sn){
	marker=(sm?new(string)(*sm):NULL);
}

MarkerComand::~MarkerComand(){
	if(marker)delete marker;
}

string MarkerComand::toString(){
	string s;
	if(no)s="NO";
	s+="MARKER";
	s+=getString();
	if(marker)s+='"'+SCANER::toSleh(*marker)+'"';else 
		if(!PB.name.empty())s+=PB.toString();
	s+=")";
	return s;
}


//--------------------------------------------------------------------------------------------------
RavnoComand::RavnoComand(Param&a,Param&b){
	PA=a;
	PB=b;
	logic=1;
	n=0;
}


RavnoComand::~RavnoComand(){}

string RavnoComand::toString(){
	string s=PA.toString();
	if(!logic)s+="=";else
	if(logic==1)s+="==";else s+="!=";
	if(!PB.name.empty())s+=PB.toString(); else
		if(n)s+=SCANER::toString(n); else {
			s+='"';
			s+=str+'"';
			}
	return s;
}


//--------------------------------------------------------------------------------------------------
FindComand::FindComand(int i,bool b){n=i;virtuale=b;}

string FindComand::toString(){
	return (string)"FIND("+(virtuale?"0":"")+SCANER::toString(n)+")";
}


//--------------------------------------------------------------------------------------------------
EqComand::EqComand(bool b){Equale=b;}

string EqComand::toString(){
	string s;
	s=Equale?"EQUALE":"UNIQUE";
	s+="(";
	V_I::iterator it=I.begin();
	for(bool f=0;it!=I.end();++it,f=1){
		if(f)s+=",";
		s+=SCANER::toString(*it);
		}
	s+=")";
	return s;
}

//--------------------------------------------------------------------------------------------------
DelNetComand::DelNetComand(string&n){name=n;}
string DelNetComand::toString(){
	return (string)"DEL("+name+")";
}

//--------------------------------------------------------------------------------------------------
DelComand::DelComand(int j){i=j;}
string DelComand::toString(){
	return (string)"DEL("+SCANER::toString(i)+")";
}

//--------------------------------------------------------------------------------------------------
GlueComand::GlueComand(){a=b=0;}
string GlueComand::toString(){
	string s;
	s="GLUE(";
	if(a)s+=SCANER::toString(a);else s+=PA.toString();
	s+=",";
	if(b)s+=SCANER::toString(b);else s+=PB.toString();
	s+=")";
	return s;
}


//--------------------------------------------------------------------------------------------------
FindNetComand::FindNetComand(string&s){name=s;bd=NULL;}
FindNetComand::~FindNetComand(){if(bd)delete bd;}

string FindNetComand::toString(){
	string s;
	if(!name.empty())s+=name+"=";
	if(!bd)return s+"*";
	s+="F";
	s+=bd->toString();
	return s;
}


//--------------------------------------------------------------------------------------------------
NewComand::NewComand(int i){n=i;}
string NewComand::toString(){
	string s;
	s="NEW(";
	s+=SCANER::toString(n);
	s+=")";
	return s;
}



//--------------------------------------------------------------------------------------------------
CopytreeComand::CopytreeComand(int i,int j){a=i;b=j;}
string CopytreeComand::toString(){
	string s("COPYTREE");
	s+="(";
	s+=SCANER::toString(a);
	s+=":=";
	s+=SCANER::toString(b);
	s+=")";
	return s;
}


//--------------------------------------------------------------------------------------------------
EqualetreeComand::EqualetreeComand(int i,int j){a=i;b=j;}
string EqualetreeComand::toString(){
	string s("EQUALETREE");
	s+="(";
	s+=SCANER::toString(a);
	s+=",";
	s+=SCANER::toString(b);
	s+=")";
	return s;
}


//--------------------------------------------------------------------------------------------------
DelltreeComand::DelltreeComand(int i){n=i;}
string DelltreeComand::toString(){
	string s("DELTREE");
	s+="(";
	s+=SCANER::toString(n);
	s+=")";
	return s;
}






//--------------------------------------------------------------------------------------------------
Specificator::~Specificator(){clear();}

void Specificator::clear(){
	V_BC::iterator it=vbc_one.begin();
	for(;it!=vbc_one.end();++it)delete *it;
	vbc_one.clear();
}


//--------------------------------------------------------------------------------------------------

*/

//	asemble.cpp	:-|