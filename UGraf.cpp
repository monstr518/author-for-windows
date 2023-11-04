//	UGraf.cpp

#include "main.h"

//--------------------------------------------------------------------------------------------------
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


string BasisLine::getStr() const{
	string S;
	S+=SCANER::toString(a);
	S+=",";
	char aa=c&1,bb=c&2;
	aa=(aa?'<':'-');
	bb=(bb?'>':'-');
	S+=aa;
	S+=",";
	S+=SCANER::writeString(name);
	S+=",";
	S+=bb;
	S+=",";
	return S;
}


string BasisLine::getStr2() const{
	string S;
	S+=SCANER::toString(a);
	S+=",";
	char aa=c&1,bb=c&2;
	aa=(aa?'1':'0');
	bb=(bb?'1':'0');
	S+=aa;
	S+=",";
	S+=SCANER::writeString(name);
	S+=",";
	S+=bb;
	S+=",";
	return S;
}



//--------------------------------------------------------------------------------------------------
NetLine::NetLine(int a,char x,const char*s,char y,int bb):BasisLine(a,x|(y<<1),s){b=bb;}
NetLine::~NetLine(){}

string NetLine::toString() const{
	string S("NET(");
	S+=getStr();
	S+=SCANER::toString(b);
	S+=")";
	return S;
}

string NetLine::toString2() const{
	string S("NET(");
	S+=getStr2();
	S+=SCANER::toString(b);
	S+=");";
	return S;
}


NetLine::NetLine(const NetLine&t):BasisLine(t){b=t.b;}

//--------------------------------------------------------------------------------------------------
MarkerLine::MarkerLine(int a,char x,const char*s,char y,const char*m):BasisLine(a,x|(y<<1),s){
	marker=m;
}

MarkerLine::~MarkerLine(){}

string MarkerLine::toString() const{
	string S("MARKER(");
	S+=getStr();
	S+=SCANER::writeString(marker);
	S+=")";
	return S;
}

string MarkerLine::toString2() const{
	string S("MARKER(");
	S+=getStr2();
	S+=SCANER::writeString(marker);
	S+=");";
	return S;
}

MarkerLine::MarkerLine(const MarkerLine&t):BasisLine(t){
	marker=t.marker;
}




//--------------------------------------------------------------------------------------------------
UGraf::UGraf(){nfree=0;}

UGraf::~UGraf(){
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


string UGraf::toString() const{
	string s;
	V_NL::const_iterator it=lincs.begin();
	for(;it!=lincs.end();++it){
		if(s.size())s+="\n";
		s+=(*it)->toString();
		}
	V_ML::const_iterator jt=markers.begin();
	for(;jt!=markers.end();++jt){
		if(s.size())s+="\n";
		s+=(*jt)->toString();
		}
	return s;
}


string UGraf::toString(string name) const{
	string s;
	V_NL::const_iterator it=lincs.begin();
	for(;it!=lincs.end();++it){
		if(s.size())s+="\n";
		s+=name+"."+(*it)->toString2();
		}
	V_ML::const_iterator jt=markers.begin();
	for(;jt!=markers.end();++jt){
		if(s.size())s+="\n";
		s+=name+"."+(*jt)->toString2();
		}
	return s;
}



int UGraf::getUnits(S_I&si) const{
	V_NL::const_iterator it=lincs.begin();
	for(;it!=lincs.end();++it){
		NetLine*nl=*it;
		si.insert(nl->a);
		si.insert(nl->b);
		}
	V_ML::const_iterator jt=markers.begin();
	for(;jt!=markers.end();++jt)si.insert((*jt)->a);
	return si.size();
}




// x:-  0- 1< 2?
void UGraf::findMarker(V_BL&vbl,int a,char x,string*n,char y,string*m) const {
	char xx,yy;
	V_ML::const_iterator it=markers.begin();
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



// x:-  0- 1< 2?
void UGraf::findNet(V_BL&vbl,int a,char x,string*n,char y,int b) const {
	bool revers=0;
	char xx,yy;
	V_NL::const_iterator it=lincs.begin();
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



// x:-  0- 1< 2?
void UGraf::deleteMarker(int a,char x,string*n,char y,string*m){
	char xx,yy;
	int i;
	for(i=0;i<markers.size();++i){
		MarkerLine*ml=markers[i];
		if(a>=0)if(ml->a!=a)continue;
		xx=ml->c&1;
		yy=(ml->c>>1)&1;
		if(x<2)if(xx!=x)continue;
		if(y<2)if(yy!=y)continue;
		if(n)if(ml->name!=*n)continue;
		if(m)if(ml->marker!=*m)continue;
		markers.erase(markers.begin()+i);
		--i;
		}
}


// x:-  0- 1< 2?
void UGraf::deleteNet(int a,char x,string*n,char y,int b){
	bool revers=0;
	char xx,yy;
	int i;
	for(i=0;i<lincs.size();++i){
		NetLine*nl=lincs[i];
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
		lincs.erase(lincs.begin()+i);
		--i;
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



UGraf::I2::I2(){fn=-1;i=0;}


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
		if(i2->fn>=0){
			A=tio[i2->fn];
			itm=io.begin();
			for(;itm!=io.end();++itm){
				if(itm->second!=A)continue;
				if(!UGraf::equale(
					const_cast<UGraf*>(&t),this,i2->fn,itm->first))continue;
				ii2=new I2(*i2);
				ii2->fn=-1;
				ii2->mii[i2->fn]=itm->first;
				dei2.push_back(ii2);
				}
			continue;
			}
		if(vnl.size()<=i2->i){ret=1;break;}
		nl=vnl[i2->i];
		bool EA=i2->mii.find(nl->a)==i2->mii.end();
		bool EB=i2->mii.find(nl->b)==i2->mii.end();
		if(EA || EB){
			if(!EA)A=nl->b;else A=nl->a;
			i2->fn=A;
			dei2.push_back(i2);
			i2=NULL;
			continue;
			}
		A=i2->mii[nl->a];
		B=i2->mii[nl->b];
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
	DE_I2::iterator id = dei2.begin();
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



bool UGraf:: operator < (const UGraf&UG){
	return *this > UG;
}


bool UGraf:: operator > (const UGraf&t){
	if(lincs.size()>t.lincs.size())return 1;
	if(lincs.size()<t.lincs.size())return 0;
	if(markers.size()>t.markers.size())return 1;
	if(markers.size()<t.markers.size())return 0;
	if(*this==t)return 0;
	int i;
	for(i=0;i<lincs.size();++i){
		NetLine*A=lincs[i],*B=t.lincs[i];
		if(A->name.size()>B->name.size())return 1;
		if(A->name.size()<B->name.size())return 0;
		}
	for(i=0;i<markers.size();++i){
		MarkerLine*A=markers[i],*B=t.markers[i];
		if(A->name.size()>B->name.size())return 1;
		if(A->name.size()<B->name.size())return 0;
		if(A->marker.size()>B->marker.size())return 1;
		if(A->marker.size()<B->marker.size())return 0;
		}
	return 0;//redko
}



UGraf:: operator  string () const {
	return toString();
}



void UGraf::import(const char*s){
	int t=1;
	char a,b;
	int A,B;
	string m,mm,str;
	while(t){
		SCANER::noProbel(s);
		if(SCANER::scanSlovo("NET",s))t=2;else t=SCANER::scanSlovo("MARKER",s);
		if(!t)return;
		SCANER::noProbel(s);
		if(!SCANER::scanSlovo("(",s))return;
		SCANER::noProbel(s);
		str="";
		SCANER::scanDigit(s,str,10);
		sscanf(str.c_str(),"%d",&A);
		SCANER::noProbel(s);
		if(*s!=',')return;
		++s;
		SCANER::noProbel(s);
		a=0;
		if(*s=='1' || *s=='<')a=1;
		++s;
		SCANER::noProbel(s);
		if(*s!=',')return;
		++s;
		SCANER::noProbel(s);
		m=SCANER::readString(s);
		m=SCANER::convertString(m);
		SCANER::noProbel(s);
		if(*s!=',')return;
		++s;
		SCANER::noProbel(s);
		b=0;
		if(*s=='1' || *s=='>')b=1;
		++s;
		SCANER::noProbel(s);
		if(*s!=',')return;
		++s;
		SCANER::noProbel(s);
		if(t==2){
			str="";
			SCANER::scanDigit(s,str,10);
			sscanf(str.c_str(),"%d",&B);
			}
		if(t==1){
			mm=SCANER::readString(s);
			mm=SCANER::convertString(mm);
			}
		SCANER::noProbel(s);
		if(*s!=')')return;
		++s;
		if(t==2)add(new NetLine(A,a,m.c_str(),b,B));
		if(t==1)add(new MarkerLine(A,a,m.c_str(),b,mm.c_str()));
		}
}




int UGraf::getSizeOf() const {
	int size = sizeof(nfree);
	size += lincs.size() * (sizeof(NetLine) + sizeof(NetLine*));
	size += markers.size() * (sizeof(MarkerLine) + sizeof(MarkerLine*));
	return size;
}




CVARIANT* UGraf::ExportData() const {
	CVARIANT* R = new CVARIANT();
	R->avtoSet("vector");
	CVARIANT*A,*B;
	int i,size;
	size = lincs.size();
	for(i=0;i<size;++i){
		const NetLine*NL = lincs[i];
		A = new CVARIANT();
		A->avtoSet("vector");
		B = new CVARIANT();
		B->avtoSet("int");
		B->DATA.intVal = NL->a;
		A->DATA.vectorVal->push_back(B);
		B = new CVARIANT();
		B->avtoSet("string");
		*B->DATA.ps = NL->name;
		A->DATA.vectorVal->push_back(B);
		B = new CVARIANT();
		B->avtoSet("int");
		B->DATA.intVal = NL->b;
		A->DATA.vectorVal->push_back(B);
		B = new CVARIANT();
		B->avtoSet("int");
		B->DATA.intVal = NL->c;
		A->DATA.vectorVal->push_back(B);
		R->DATA.vectorVal->push_back(A);
		}
	size = markers.size();
	for(i=0;i<size;++i){
		const MarkerLine*ML = markers[i];
		A = new CVARIANT();
		A->avtoSet("vector");
		B = new CVARIANT();
		B->avtoSet("int");
		B->DATA.intVal = ML->a;
		A->DATA.vectorVal->push_back(B);
		B = new CVARIANT();
		B->avtoSet("string");
		*B->DATA.ps = ML->name;
		A->DATA.vectorVal->push_back(B);
		B = new CVARIANT();
		B->avtoSet("string");
		*B->DATA.ps = ML->marker;
		A->DATA.vectorVal->push_back(B);
		B = new CVARIANT();
		B->avtoSet("int");
		B->DATA.intVal = ML->c;
		A->DATA.vectorVal->push_back(B);
		R->DATA.vectorVal->push_back(A);
		}
	return R;
}





bool UGraf::ImportData(CVARIANT*Data){
	bool isValid;
	if(!Data)return 0;
	isValid = Data->isType("vector");
	if(!isValid)return 0;
	int i,size;
	size = Data->DATA.vectorVal->size();
	// validation Data:
	for(i=0;i<size;++i){
		CVARIANT*A = (*Data->DATA.vectorVal)[i];
		isValid = A->isType("vector");
		if(!isValid)return 0;
		isValid = (A->DATA.vectorVal->size()==4);
		if(!isValid)return 0;
		}
	for(i=0;i<size;++i){
		CVARIANT*A = (*Data->DATA.vectorVal)[i];
		CVARIANT*B;
		B = (*A->DATA.vectorVal)[2]; // number or string
		bool isString,isInt;
		isString = B->isType("string"); // for marker
		isInt = B->isType("int"); // for net
		isValid = (isString || isInt);
		if(!isValid)break;
		int a,b,c;
		string*sMarker,*sName;
		if(isString)sMarker = B->DATA.ps;
		if(isInt)b = B->DATA.intVal;
		B = (*A->DATA.vectorVal)[0];
		isValid = B->isType("int");
		if(!isValid)break;
		a = B->DATA.intVal;
		B = (*A->DATA.vectorVal)[1];
		isValid = B->isType("string");
		if(!isValid)break;
		sName = B->DATA.ps;
		B = (*A->DATA.vectorVal)[3];
		isValid = B->isType("int");
		if(!isValid)break;
		c = B->DATA.intVal;
		if(isString){
			MarkerLine*ML = new MarkerLine(a,0,sName->c_str(),0,sMarker->c_str());
			ML->c = c;
			markers.push_back(ML);
			if(nfree<=a)nfree = ++a;
			}
		if(isInt){
			NetLine*NL = new NetLine(a,0,sName->c_str(),0,b);
			NL->c = c;
			lincs.push_back(NL);
			if(nfree<=a)nfree = ++a;
			if(nfree<=b)nfree = ++b;
			}
		}
	return isValid;
}





//	UGraf.cpp	:-|