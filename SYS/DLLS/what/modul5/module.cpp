// module.cpp

#include "stdafx.h"
#include "module.h"


void SCANER::noProbel(char*&s){
	while(*s=='\r' || *s=='\n' || *s==' ' || *s=='	')++s;
}


int SCANER::scanSlovo(char*slovo,char*&s){
	int i=strncmp(slovo,s,strlen(slovo));
	if(i)return 0;
	s+=strlen(slovo);
	return 1;
}


string SCANER::getName(char*&s){
	string name;
	if(!isalpha(*s))return "";
	do{
		name+=*s;
		++s;
		}while(isalpha(*s) || isdigit(*s));
	return name;
}



double SCANER::getDigit(char*&s){
	float n;
	sscanf(s,"%f",&n);
	if(*s=='-')++s;
	while(isdigit(*s))++s;
	if(*s=='.'){
		++s;
		while(isdigit(*s))++s;
		}
	return n;
}


/*
char* SCANER::toString(double n){
	static char s[10];
	if(n==(int)n)sprintf(s,"%d",(int)n);else
	sprintf(s,"%f",(float)n);
	return s;
}
*/




//--------------------------------------------------------------------------------------------
ProblemA::~ProblemA(){ClearQ();}

//x:1,2;
string ProblemA::getLine(char*&s,V_D&V){
	string name;
	SCANER::noProbel(s);
	name=SCANER::getName(s);
	SCANER::noProbel(s);
	if(*s!=':')return "";
	do{
		++s;
		SCANER::noProbel(s);
		double n=SCANER::getDigit(s);
		V.push_back(n);
		SCANER::noProbel(s);
		}while(*s==',');
	if(*s!=';')return "";
	++s;
	return name;
}



//?: get code y=f(x) for table:head(x,y){x:2,3,4;y:4,9,16;}
int ProblemA::ParserProblem(const char*ss){
	char*s=const_cast<char*>(ss);
	SCANER::noProbel(s);
	if(!SCANER::scanSlovo("?:",s))return 0;
	SCANER::noProbel(s);
	if(!SCANER::scanSlovo("get",s))return 0;
	SCANER::noProbel(s);
	isCode=1;
	if(!SCANER::scanSlovo("code",s)){
		isCode=0;
		if(!SCANER::scanSlovo("function",s))return 0;
		}
	SCANER::noProbel(s);
	if(!isCode)nameF=SCANER::getName(s); else{
		nameY=SCANER::getName(s);
		SCANER::noProbel(s);
		isY=0;
		if(*s=='='){
			isY=1;
			++s;
			SCANER::noProbel(s);
			if(*s!='f')return 0;
			++s;
			}else{
			if(nameY!="f")return 0;
			nameY="";
			}
		}
	SCANER::noProbel(s);
	if(*s!='(')return 0;
	++s;
	nameX=SCANER::getName(s);
	SCANER::noProbel(s);
	if(*s!=')')return 0;
	++s;
	SCANER::noProbel(s);
	if(!SCANER::scanSlovo("for",s))return 0;
	SCANER::noProbel(s);
	if(!SCANER::scanSlovo("table",s))return 0;
	SCANER::noProbel(s);
	if(*s!=':')return 0;
	++s;
	SCANER::noProbel(s);
	if(!SCANER::scanSlovo("head",s))return 0;
	SCANER::noProbel(s);
	if(*s!='(')return 0;
	++s;
	string nameA,nameB,*nY=NULL;
	nameA=SCANER::getName(s);
	SCANER::noProbel(s);
	if(*s!=',')return 0;
	++s;
	SCANER::noProbel(s);
	nameB=SCANER::getName(s);
	SCANER::noProbel(s);
	if(*s!=')')return 0;
	++s;
	SCANER::noProbel(s);
	if(nameX==nameA)nY=&nameB;
	if(nameX==nameB)nY=&nameA;
	if(!nY)return 0;
	if(isCode){
		if(!nameY.empty())if(nameY!=*nY)return 0;
		nameY=*nY;
		}
	if(*s!='{')return 0;
	++s;
	V_D V1,V2;
	nameA=getLine(s,V1);
	nameB=getLine(s,V2);
	SCANER::noProbel(s);
	if(*s!='}')return 0;
	int normal=2;
	if(nameA==nameX && nameB==nameY)normal=1;
	if(nameA==nameY && nameB==nameX)normal=0;
	if(normal==2)return 0;
	if(normal){X=V1;Y=V2;}else{X=V2;Y=V1;}
	while(X.size()>Y.size())X.pop_back();
	while(X.size()<Y.size())Y.pop_back();
	if(X.empty())return 0;
	n=0;
	Reception=NULL;
	return 1;
}


//sorted X
void ProblemA::sort(V_D&X,V_D&Y){
	for(int i=0;i<X.size();++i){
		for(int j=i+1,n=i;j<X.size();++j)
			if(X[j]<X[n])n=j;
		double w=X[i];
		X[i]=X[n];
		X[n]=w;
		w=Y[i];
		Y[i]=Y[n];
		Y[n]=w;
		}
}


void ProblemA::f0(V_D&X,V_D&Y){
	sort(X,Y);
	//разбить на промижутки в которых Х возростает на единицу
	VV_D promY;
	V_D  promX;
	double a,aa;
	int first=1;
	V_D yy;
	for(int i=0;i<X.size();++i){
		if(first){
			first=0;
			aa=a=X[i];
			yy.push_back(Y[i]);
			continue;
			}
		++a;
		if(a==X[i]){
			yy.push_back(Y[i]);
			continue;
			}
		promY.push_back(yy);
		promX.push_back(aa);
		yy.clear();
		first=1;
		--i;
		}
	promY.push_back(yy);
	promX.push_back(aa);
	//выбрать максимально длинный промежуток
	int max=0;
	for(i=1;i<promY.size();++i)
		if(promY[max].size()<promY[i].size())max=i;
	//построить вопрос
	string nameV=&X==&this->X?nameX:nameY;
	ostringstream vopros;
	vopros<<"?: get code("<<nameV<<") for row {";
	for(i=0;i<promY[max].size();++i){
		if(i)vopros<<", ";
		vopros<<promY[max][i];
		}
	vopros<<"} first(";
	vopros<<promX[max];
	vopros<<")";
	buf=vopros.str();
	//Q->Send(Q->S,vopros.str());
	//cout<<"=="<<vopros.str();
	Reception=new Answer(1);
	Q.push_back(Reception);
}




void ProblemA::f1(const char*otvet){
	//построить вопрос: вычислить по формуле множество вариантов
	//?: get row for formul(x*2+1) from {x:2,3,4;}
	ostringstream vopros;
	vopros<<"?: what result for formula(";
	vopros<<otvet;
	vopros<<") from {"<<nameX<<":";
	for(int i=0;i<X.size();++i){
		if(i)vopros<<", ";
		vopros<<X[i];
		}
	vopros<<";}";
	buf=vopros.str();
	Reception=new M1(otvet);
	Q.push_back(Reception);
}



void ProblemA::f2(Answer*A){
	string s=A->Next();
	buf="";
	if(!controlRow(const_cast<char*>(s.c_str()),Y))return;
	M1*m=dynamic_cast<M1*>(A);
	s=m->formula;
	if(isY)s=nameY+"="+s;
	if(!isCode){
		s="function "+nameF+"{return "+s+";}";
		}
	Results.S.push_back(s);
}




void ProblemA::f3(Answer*A){
	string s=A->Next();
	ostringstream vopros;
	vopros<<"?: get equality for ("<<s<<"="<<nameX<<") unknown {"<<nameY<<"}";
	buf=vopros.str();
	Reception=new Answer(4);
	Q.push_back(Reception);
}



//--------------------------------------------------------------------------------------------
bool ProblemA::isDescretX(){
	nMaxX=0;
	for(int i=0;i<X.size();++i){
		if(X[i]>int(X[i]))return 0;
		if(X[i]>X[nMaxX])nMaxX=i;
		}
	return 1;
}

//?: get code y=f(x) for table:head(x,y){x:0,1,2,3,4,5,6,7;y:0,1,4,3,16,5,36,7;}
//?: get code y=f(x) for table:head(x,y){x:0,1,2,3,4,5,6,7;y:5,8,5,8,5,8,5,8;}
//?: get code y=f(x) for table:head(x,y){x:0,1,2,3,4,5,6,7,8,9;y:8,1,6,8,1,6,8,1,6,8;}
void ProblemA::f5(){
	mii.clear();
	++K;
	//cout<<"X[nMaxX]=="<<X[nMaxX]<<endl;
	if(K>=int(X[nMaxX]))return;
	for(int i=0;i<X.size();++i)++mii[int(X[i])%K];
	if(mii.size()<2||mii.size()>=i){
		f5();
		return;
		}
	M_II::iterator it=mii.begin();
	for(;it!=mii.end();++it)it->second=-1;
	//проверить все ряды на равенство одному числу ряда.
	//mii[i]==-2  -не равно в ряде [i].
	//mii[i]>=0   -Y[mii[i]]== всему ряду [i].
	int n,t,perfect=1;
	for(i=0;i<X.size();++i){
		t=int(X[i])%K;
		n=mii[t];
		if(n==-1){mii[t]=i;continue;}
		if(n<0)continue;
		if(Y[n]!=Y[i]){mii[t]=-2;perfect=0;}
		}
	if(perfect){//все ряды равны числу ряда
		//cout<<"perfect:"<<endl;
		ostringstream otvet;
		if(mii.size()==2){
			otvet<<nameY<<"=";
			bool first=1;
			it=mii.begin();
			for(;it!=mii.end();++it){
				if(first)otvet<<nameX<<"%"<<K<<"=="<<it->first<<"?";
				otvet<<Y[it->second];
				if(first){
					first=0;
					otvet<<":";
					}
				}
			Results.S.push_back(otvet.str());
			return;
			}
		bool ok=1;
		it=mii.begin();
		for(;it!=mii.end();++it)if(it->first>=mii.size())ok=0;
		if(ok){
			otvet<<nameY<<"=({";
			bool first=1;
			it=mii.begin();
			for(;it!=mii.end();++it){
				if(first)first=0;else otvet<<",";
				otvet<<Y[it->second];
				}
			otvet<<"})["<<nameX<<"%"<<K<<"]";
			Results.S.push_back(otvet.str());
			return;
			}
		it=mii.begin();
		for(;it!=mii.end();++it){
			otvet<<"if("<<nameX<<"%"<<K<<"=="<<it->first<<")"<<nameY<<"=";
			otvet<<Y[it->second]<<";\n";
			}
		Results.S.push_back(otvet.str());
		return;
		}
	NaborF*NF=new NaborF();
	Reception=NF;
	Q.push_back(Reception);
	buf=NF->votRowF(this);
	Reception->t=6;
}



//--------------------------------------------------------------------------------------------
bool ProblemA::controlInput(){
	int i=0,j;
	for(;i<X.size();++i){
		j=i+1;
		while(j<X.size()){
			if(X[i]==X[j]){
				if(Y[i]!=Y[j])return 0;
				X.erase(X.begin()+j);
				Y.erase(Y.begin()+j);
				continue;
				}
			++j;
			}
		}
	return 1;
}



void ProblemA::run(){
	if(Reception)return;
	if(!n){
		if(!controlInput()){
			n=4;
			run();
			return;
			}
		if(Q.empty()){
			f0(X,Y);
			return;
			}
		Answer*A=getQ();
		if(A){
			if(A->t==1){
				f1(A->Next().c_str());
				return;
				}
			if(A->t==2){
				f2(A);
				run();
				return;
				}
			}
		n=1;
		if(Results.S.size())return;
		}
	if(n==1){
		if(Q.empty()){
			f0(Y,X);
			Reception->t=3;
			return;
			}
		Answer*A=getQ();
		if(A){
			if(A->t==3){
				f3(A);
				Reception->t=4;
				return;
				}
			if(A->t==4){
				string s=A->Next();
				int p=s.find("=");
				s=s.substr(p+1);
				if(s[0]=='{'){
					s=s.substr(1,s.length()-2);
					//cout<<"!!!: "<<s.c_str()<<endl;//
					string g;
					for(int i=0,d=0;i<s.length();++i){
						if(s[i]=='(')++d;
						if(s[i]==')')--d;
						if(s[i]==',' && !d){
							A->S.push_back(g);
							g="";
							continue;
							}
						g+=s[i];
						}
					if(!d)A->S.push_back(g);
					run();
					return;
					}
				f1(s.c_str());
				Reception->t=5;
				return;
				}
			if(A->t==5){
				f2(A);
				run();
				return;
				}
			}
		n=2;
		if(Results.S.size())return;
		}
	if(n==2){
		n=3;
		sort(X,Y);
		if(!isDescretX())n=4;
		K=1;
		}
	if(n==3){
		if(Q.empty()){
			f5();
			if(Results.S.size())return;
			if(Reception)return;
			n=4;
			run();
			return;
			}
		Answer*A=getQ();
		if(A){
			if(A->t==6){
				NaborF*NF=dynamic_cast<NaborF*>(A);
				string s=NF->S[rand()%NF->S.size()];
				int p=s.find("=");
				s=s.substr(p+1);
				NF->mis[NF->k]=s;
				NF->S.clear();
				buf=NF->votRowF(this);
				if(buf.size()){
					Reception=NF;
					return;
					}
				Results.S.push_back(NF->assembleOtvet(this));
				//Обрабатывать остальные Q ненужно. Обект только один.
				}
			return;
			}
		run();
		return;
		}
	Results.S.push_back(".");
}




Answer* ProblemA::getQ(){
	Answer*A=NULL;
	for(int i=0;i<Q.size();++i)if(Q[i]->S.size()){
		A=Q[i];
		break;
		}
	if(!A)ClearQ();
	buf="";
	return A;
}


void ProblemA::ClearQ(){
	for(int i=0;i<Q.size();++i)if(Q[i])delete Q[i];
	Q.clear();
}




bool ProblemA::controlRow(char*s,V_D&Y){
	SCANER::noProbel(s);
	if(*s!='{')return 0;
	++s;
	int i=0,ok;
	do{
		ok=0;
		SCANER::noProbel(s);
		double t=SCANER::getDigit(s);
		SCANER::noProbel(s);
		if(t!=Y[i++])return 0;
		if(*s==','){++s;ok=1;}
		}while(ok);
	if(*s!='}')return 0;
	return 1;
}



void ProblemA::Set(const char*str){
	if(!Reception)return;
	Reception->Set(str);
	if(!Reception->n)Reception=NULL;
}


void ProblemA::getRow(NaborF*NF,V_D&VX,V_D&VY){
	M_II::iterator it=mii.begin();
	for(;it!=mii.end();++it)if(it->second==-2)if(NF->mis[it->first].empty())break;
	if(it==mii.end())return;
	int n=it->first;
	for(int i=0;i<X.size();++i)if(int(X[i])%K==n){
		VX.push_back(X[i]);
		VY.push_back(Y[i]);
		}
	NF->k=n;
}



//--------------------------------------------------------------------------------------------
Answer::Answer(int x){t=x;n=0;}

void Answer::Set(const char*s){
	if(s[0]=='.')return;
	string sstr=s;
	if(!n){
		if(sstr.substr(0,3)=="{}:"){
			char*ss=const_cast<char*>(s)+3;
			n=SCANER::getDigit(ss);
			return;
			}
		S.push_back(sstr);
		return;
		}
	--n;
	S.push_back(sstr);
}


string Answer::Next(){
	string s=*S.rbegin();
	S.pop_back();
	return s;
}



//--------------------------------------------------------------------------------------------
M1::M1(string s):Answer(2){formula=s;}

//--------------------------------------------------------------------------------------------
string NaborF::votRowF(ProblemA*P){
	V_D VX,VY;
	P->getRow(this,VX,VY);
	if(VX.empty())return "";
	//?: get code y=f(x) for table: head(x,y){x:1,2;y:5,10;}
	ostringstream vopros;
	vopros<<"?: get code "<<P->nameY<<"=f("<<P->nameX<<")";
	vopros<<" for table: head("<<P->nameX<<","<<P->nameY<<"){";
	vopros<<P->nameX<<":";
	for(int i=0;i<VX.size();++i){
		if(i)vopros<<",";
		vopros<<VX[i];
		}
	vopros<<"; "<<P->nameY<<":";
	for(i=0;i<VY.size();++i){
		if(i)vopros<<",";
		vopros<<VY[i];
		}
	vopros<<";}";
	return vopros.str();
}



string NaborF::assembleOtvet(ProblemA*P){
	ostringstream otvet;
	M_II::iterator it;
	if(P->mii.size()==2){
		otvet<<P->nameY<<"=";
		bool first=1;
		it=P->mii.begin();
		for(;it!=P->mii.end();++it){
			if(first)otvet<<P->nameX<<"%"<<P->K<<"=="<<it->first<<"?";
			if(it->second<0)otvet<<mis[it->first];else otvet<<P->Y[it->second];
			if(first){
				first=0;
				otvet<<":";
				}
			}
		return otvet.str();
		}
	bool ok=1;
	it=P->mii.begin();
	for(;it!=P->mii.end();++it)if(it->first>=P->mii.size())ok=0;
	if(ok){
		otvet<<P->nameY<<"=({";
		bool first=1;
		it=P->mii.begin();
		for(;it!=P->mii.end();++it){
			if(first)first=0;else otvet<<",";
			if(it->second<0)otvet<<mis[it->first];else otvet<<P->Y[it->second];
			}
		otvet<<"})["<<P->nameX<<"%"<<P->K<<"]";
		return otvet.str();
		}
	it=P->mii.begin();
	for(;it!=P->mii.end();++it){
		otvet<<"if("<<P->nameX<<"%"<<P->K<<"=="<<it->first<<")"<<P->nameY<<"=";
		if(it->second<0)otvet<<mis[it->first];else otvet<<P->Y[it->second];
		otvet<<";\n";
		}
	return otvet.str();
}


// module.cpp	:-|