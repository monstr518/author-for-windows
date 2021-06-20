// program.cpp

#include "stdafx.h"
#include "interval.h"

extern int cok;
extern string buf;
//--------------------------------------------------------------------------------------------------
base::~base(){}

void base::NotOK(base*&t){t=t->notControl(0);}
int base::Vsos(){return 0;}
string base::toBasisString(){return "";}


void base::problem(const char*c){
	//?: optimize conditions
	//?: whot basis conditions
	SCANER::noProbel(c);
	if(*c!='?')return;
	++c;
	SCANER::noProbel(c);
	if(*c!=':')return;
	++c;
	SCANER::noProbel(c);
	int t=0;
	if(SCANER::scanSlovo("optimize",c))t=1;else{
		t+=SCANER::scanSlovo("whot",c);
		SCANER::noProbel(c);
		t+=SCANER::scanSlovo("basis",c);
		if(t<2)return;
		}
	SCANER::noProbel(c);
	if(!SCANER::scanSlovo("conditions",c))return;
	SCANER::noProbel(c);
	if(*c==':')++c;
	SCANER::noProbel(c);
	base*B=base::read(c);
	if(!B)return;

	//cout<<B->toString().c_str()<<endl;
	base::NotOK(B);
	B->Vsos();
	B->optimize();
	if(t==1)buf=B->toString();
	if(t==2)buf=B->toBasisString();
}



base* base::read(string s){
	const char*c=s.c_str();
	return read(c);
}


base* base::read(const char*&c){
	SCANER::noProbel(c);
	if(*c!='(')return NULL;
	++c;
	SCANER::noProbel(c);
	base*Root=NULL,*t;
	base**A=&Root;
	base**A2=A;
	nabor*B=NULL;
	int r=1;// 1-prefix  2-operator
	while(*c){
		if(r==1){
			if(*c=='!'){
				++c;
				SCANER::noProbel(c);
				not*N=new not(NULL);
				*A=N;
				A=&N->X;
				}
			if(*c=='(')t=base::read(c);else{
				t=NULL;
				if(*c=='R'||*c=='1'){
					++c;
					t=new condition(0,1,1,1);
					}else if(*c=='0'){
					++c;
					t=new condition(0,0,0,0);
					}
				if(!t)t=condition::load(c);
				if(!t)break;
				SCANER::noProbel(c);
				}
			*A=t;
			if(t)r=2;
			if(!*c)break;
			if(*c==')')break;
			}
		if(r==2){
			SCANER::noProbel(c);
			int f=10;
			if(SCANER::scanSlovo("||",c))f=1;else
				if(SCANER::scanSlovo("&&",c))f=0;
			if(f>1){
				delete Root;
				Root=NULL;
				break;
				}
			r=1;
			SCANER::noProbel(c);
			nabor*N=dynamic_cast<nabor*>(Root);
			if(!N){
				N=new nabor(f);
				N->sub.push_back(Root);
				N->sub.push_back(NULL);
				A2=A=&N->sub[1];
				Root=N;
				continue;
				}
			if(N->U==f){
				if(f==1)B=NULL;
				N->sub.push_back(NULL);
				A2=A=&N->sub[N->sub.size()-1];
				continue;
				}
			if(N->U==1){
				if(!B){
					B=new nabor(f);
					B->sub.push_back(*A2);
					*A2=B;
					B->sub.push_back(NULL);
					A2=A=&B->sub[1];
					}else{
					B->sub.push_back(NULL);
					A2=A=&B->sub[B->sub.size()-1];
					}
				continue;
				}
			B=N;
			N=new nabor(1);
			N->sub.push_back(B);
			N->sub.push_back(NULL);
			A2=A=&N->sub[1];
			Root=N;
			B=NULL;
			}
		}
	if(*c!=')'){
		if(Root)delete Root;
		return NULL;
		}
	++c;
	SCANER::noProbel(c);
	return Root;
}


void base::optimize(){}


//--------------------------------------------------------------------------------------------------
not::not(base*A){X=A;}
not::~not(){if(X)delete X;}

string not::toString(){
	return string("!")+X->toString();
}


base* not::notControl(bool not){
	base*Y=X->notControl(!not);
	X=NULL;
	delete this;
	return Y;
}


int not::Vsos(){return X->Vsos();}

void not::optimize(){X->optimize();}

base* not::copy(){return new not(X->copy());}
//--------------------------------------------------------------------------------------------------
condition::condition(){}
// n, <, =, >
condition::condition(double x,int a,int b,int cc){
	n=x;
	c=cc|(b<<1)|(a<<2);
}


condition* condition::U(condition*X){
	if(c==0)return X; // 0+A==A
	if(c==7)return this; // R+A==R
	if(X->c==0)return this; // 0+A==A
	if(X->c==7)return X; // R+A==R
	if(X->n==n){
		if(c==X->c)return this; // A=B
		if((c|X->c)==7)return new condition(0,1,1,1);
		if((c|X->c)==5)return NULL; // R not n, x<>n
		if((c|X->c)==3){
			if(c==3)return this;
			if(X->c==3)return X;
			return new condition(n,0,1,1);
			}
		//c|X->c==6
		if(c==6)return this;
		if(X->c==6)return X;
		return new condition(n,1,1,0);
		}
	condition*A=this;
	if(n>X->n){
		A=X;
		X=this;
		}
	if(A->c&1 && X->c&4)return new condition(0,1,1,1); // R
	if(A->c&1)return A;
	if(X->c&4)return X;
	return NULL; // OK
}


condition* condition::notU(condition*X){
	if(c==0)return this; // 0*A==0
	if(c==7)return X; // R*A==A
	if(X->c==0)return X; // 0*A==0
	if(X->c==7)return this; // R*A==A
	if(X->n==n){
		if(c==X->c)return this; // A=B
		if((c&X->c)==0)return new condition(0,0,0,0);
		if((c&X->c)==2)return new condition(n,0,1,0);
		if((c&X->c)==1){
			if(c==1)return this;
			if(X->c==1)return X;
			}
		//(c&X->c==4)
		if(c==4)return this;
		if(X->c==4)return X;
		}
	condition*A=this;
	if(n>X->n){
		A=X;
		X=this;
		}
	if(!(A->c&1) && !(X->c&4))return new condition(0,0,0,0);
	if(A->c==5 && X->c==5)return NULL; // OK
	if((A->c&1) && (X->c&1) && !(X->c&4))return X;
	if((X->c&4) && (A->c&4) && !(A->c&1))return A;
	return NULL; // OK
}


string condition::getNumber(){
	char str[100];
	sprintf(str,"%0.20lf",n);
	char*f=NULL;
	bool ok=0;
	for(char*c=str;*c;++c){
		if(*c=='.')ok=1;
		if(*c=='0'&&!f)f=c;
		if(*c!='0')f=NULL;
		}
	if(ok){
		if(f)*f=0;
		if(*--f=='.')*f=0;
		}
	return str;
}


string condition::toString(){
	if(c==7)return "R";
	if(c==0)return "0";
	string s;
	s="x";
	if(c==5)s+="!=";else{
		if(c&4)s+="<";
		if(c&1)s+=">";
		if(c&2)s+="=";
		}
	s+=getNumber();
	return s;
}


base* condition::notControl(bool not){
	if(!not)return this;
	int a,b,cc;
	a=!(c&4);
	b=!(c&2);
	cc=!(c&1);
	c=cc|(b<<1)|(a<<2);
	return this;
}



base* condition::load(const char*&s){
	SCANER::noProbel(s);
	if(!SCANER::scanSlovo("x",s))return NULL;
	SCANER::noProbel(s);
	int a,b,c;
	a=b=c=0;
	if(SCANER::scanSlovo("!=",s))a=c=1;else{
		bool ok=1;
		--s;
		while(ok){
			++s;
			ok=0;
			if(*s=='>'){c=1;ok=1;}
			if(*s=='='){b=1;ok=1;}
			if(*s=='<'){a=1;ok=1;}
			}
		}
	SCANER::noProbel(s);
	double n=0;
	if(!SCANER::getDigit(s,n))return NULL;
	SCANER::noProbel(s);
	return new condition(n,a,b,c);
}


base* condition::copy(){
	condition*t=new condition();
	t->c=c;
	t->n=n;
	return t;
}

//--------------------------------------------------------------------------------------------------
nabor::nabor(int u){U=u;}
nabor::~nabor(){
	for(int i=0;i<sub.size();++i)if(sub[i])delete sub[i];
}


string nabor::toString(){
	string s;
	for(int i=0;i<sub.size();++i){
		if(i)s+=(U?" || ":" && ");
		s+=sub[i]->toString();
		}
	if(i>1)s="("+s+")";
	return s;
}


base* nabor::notControl(bool not){
	if(not)U=!U;
	for(int i=0;i<sub.size();++i)sub[i]=sub[i]->notControl(not);
	return this;
}


int nabor::Vsos(){
	int r=0;
	for(int i=0;i<sub.size();++i)r+=sub[i]->Vsos();
	for(i=0;i<sub.size();++i){
		nabor*X=dynamic_cast<nabor*>(sub[i]);
		if(!X)continue;
		if(U==X->U){
			sub[i]=X->sub[0];
			for(int j=1;j<X->sub.size();++j)sub.push_back(X->sub[j]);
			X->sub.clear();
			delete X;
			r=1;
			}else if(X->sub.size()==1){
			sub[i]=X->sub[0];
			X->sub.clear();
			delete X;
			r=1;
			}
		}
	return r;
}


void nabor::optimize(){
	for(int i=0;i<sub.size();++i){
		sub[i]->optimize();
		nabor*X=dynamic_cast<nabor*>(sub[i]);
		if(!X)continue;
		if(X->sub.size()==1){
			sub[i]=X->sub[0];
			X->sub.clear();
			delete X;
			}
		}
	//cout<<toString().c_str()<<endl;
	V_C X;
	base*A;
	condition*B,*C,*D;
	while(!sub.empty()){
		A=*sub.rbegin();
		sub.pop_back();
		B=dynamic_cast<condition*>(A);
		if(!B){
			X.push_back(A);
			continue;
			}
		int okn=0,ok=1;
		for(i=0;i<X.size();++i){
			C=dynamic_cast<condition*>(X[i]);
			if(!C){
				++okn;
				continue;
				}
			D=U?C->U(B):C->notU(B);
			if(!D){
				++okn;
				continue;
				}
			if(D==C)continue;
			if(D==B){
				delete C;
				X[i]=D;
				ok=0;
				continue;
				}
			delete C;
			X[i]=D;
			ok=0;
			}
		if(okn==X.size())X.push_back(B);else if(ok==1)delete B;
		}
	sub.swap(X);
	if(U==1){
		int r=strax();
		r+=Vsos();
		if(r)optimize();
		return;
		}
	strax2();
}



int nabor::strax(){
	V_C X,Y;
	base*A;
	condition*B,*C;
	nabor*N,*N2;
	int ok=0,i,j;
	while(!sub.empty()){
		A=*sub.rbegin();
		sub.pop_back();
		B=dynamic_cast<condition*>(A);
		if(B){
			if(B->c==7)ok=1;
			if(B->c){
				Y.push_back(A);
				continue;
				}
			delete B;
			}else X.push_back(A);
		}
	if(ok){
		for(i=0;i<Y.size();++i)delete Y[i];
		for(i=0;i<X.size();++i)delete X[i];
		sub.push_back(new condition(0,1,1,1));
		return 1;
		}
	if(Y.empty()){
		sub.swap(X);
		return 0;
		}
	if(X.empty()){
		sub.swap(Y);
		return 0;
		}
	condition*AA,*BB;
	int a,b,r=0;
	for(i=0;i<Y.size();++i){
		B=dynamic_cast<condition*>(Y[i]);
		for(j=0;j<X.size();++j){ // multi nabors
			N=dynamic_cast<nabor*>(X[j]);
			if(!N)continue;
			if(N->U)continue; // N->U==0  notU
			AA=BB=NULL;
			a=b=-1;
			for(int k=0;k<N->sub.size();++k){
				C=dynamic_cast<condition*>(N->sub[k]);
				if(!C)continue;
				if(C->c&1){AA=C;a=k;}
				if(C->c&4){BB=C;b=k;}
				}
			if(B->c==2){
				ok=0;
				if(AA && BB)if(B->n>AA->n && B->n<BB->n)ok=1;
				if(AA){
					if(B->n==AA->n){
						AA->c= AA->c | 2;
						ok=1;
						}
					if(!BB)if(B->n>AA->n)ok=1;
					}
				if(BB){
					if(B->n==BB->n){
						BB->c= BB->c | 2;
						ok=1;
						}
					if(!AA)if(B->n<BB->n)ok=1;
					}
				if(ok){
					delete B;
					Y[i]=B=NULL;
					r=1;
					break;
					}
				continue;
				}
			if(!(B->c&4)){
				if(AA){
					if(B->n<AA->n){
						delete AA;
						AA=B;
						//N->sub[a]=B;
						N->sub.erase(N->sub.begin()+a);
						Y[i]=B=NULL;
						r=1;
						}else if(B->n==AA->n){
						AA->c=AA->c | (B->c&2);
						delete B;
						Y[i]=B=NULL;
						r=1;
						}else{ // B->n > AA->n
						if(BB){
							if(B->n>BB->n)continue;
							ok=0;
							if(B->n==BB->n)if((B->c|BB->c)==7)ok=1;
							if(B->n<BB->n)ok=1;
							if(ok){
								delete BB;
								//N->sub[b]=NULL;
								N->sub.erase(N->sub.begin()+b);
								delete B;
								Y[i]=B=NULL;
								r=1;
								}
							}else{
							delete B;
							Y[i]=B=NULL;
							r=1;
							}
						}
					}else{
					delete B;
					Y[i]=B=NULL;
					r=1;
					}
				if(!B)break;
				}
			if(!(B->c&1)){
				if(BB){
					if(B->n>BB->n){
						delete BB;
						BB=B;
						//N->sub[b]=B;
						N->sub.erase(N->sub.begin()+b);
						Y[i]=B=NULL;
						r=1;
						}else if(B->n==BB->n){
						BB->c= BB->c | (B->c&2);
						delete B;
						Y[i]=B=NULL;
						r=1;
						}else{ // B->n < BB->n
						if(AA){
							if(B->n<AA->n)continue;
							ok=0;
							if(B->n==AA->n)if((B->c|AA->c)==7)ok=1;
							if(B->n>AA->n)ok=1;
							if(ok){
								delete AA;
								//N->sub[a]=NULL;
								N->sub.erase(N->sub.begin()+a);
								delete B;
								Y[i]=B=NULL;
								r=1;
								}
							}else{
							delete B;
							Y[i]=B=NULL;
							r=1;
							}
						}
					}else{
					delete B;
					Y[i]=B=NULL;
					r=1;
					}
				if(!B)break;
				}
			}
		}
	//sub.swap(X);
	condition*AA2,*BB2;
	int a2,b2,e,e2,k;
	for(i=0;i<X.size();++i){
		N=dynamic_cast<nabor*>(X[i]);
		if(!N)continue;
		if(N->U)continue; // N->U==0  notU
		AA=BB=NULL;
		a=b=-1;
		e=1;
		for(k=0;k<N->sub.size();++k){
			C=dynamic_cast<condition*>(N->sub[k]);
			if(!C){e=0;continue;}
			if(C->c&1){AA=C;a=k;}
			if(C->c&4){BB=C;b=k;}
			}
		for(j=i+1;j<X.size();++j){
			N2=dynamic_cast<nabor*>(X[j]);
			if(!N2)continue;
			if(N2->U)continue; // N2->U==0  notU
			AA2=BB2=NULL;
			a2=b2=-1;
			e2=1;
			for(k=0;k<N2->sub.size();++k){
				C=dynamic_cast<condition*>(N2->sub[k]);
				if(!C){e2=0;continue;}
				if(C->c&1){AA2=C;a2=k;}
				if(C->c&4){BB2=C;b2=k;}
				}
			if(e+e2==0)continue;
			// control interval in interval [()]
			k=0;
			if(AA && AA2){
				if(AA->n<AA2->n)++k;
				if(AA->n==AA2->n)if(AA->c==AA2->c || (AA->c&2)&&!(AA2->c&2))++k;
				}
			if(!AA)++k;
			if(BB && BB2){
				if(BB->n>BB2->n)++k;
				if(BB->n==BB2->n)if(BB->c==BB2->c || (BB->c&2)&&!(BB2->c&2))++k;
				}
			if(!BB)++k;
			if(k==2){ // OK [()]
				if(AA2){
					delete AA2;
					N2->sub[a2]=NULL;
					}
				if(BB2){
					delete BB2;
					N2->sub[b2]=NULL;
					}
				for(k=0;k<N2->sub.size();++k)if(N2->sub[k]){
					N->sub.push_back(N2->sub[k]);
					N2->sub[k]=NULL;
					}
				e=(e&e2);
				delete N2;
				X[j]=NULL;
				r=1;
				continue;
				}
			// control interval in interval ([])
			k=0;
			if(AA && AA2){
				if(AA->n>AA2->n)++k;
				if(AA->n==AA2->n)if(AA->c==AA2->c || !(AA->c&2)&&(AA2->c&2))++k;
				}
			if(!AA2)++k;
			if(BB && BB2){
				if(BB->n<BB2->n)++k;
				if(BB->n==BB2->n)if(BB->c==BB2->c || !(BB->c&2)&&(BB2->c&2))++k;
				}
			if(!BB2)++k;
			if(k==2){ // OK ([])
				if(AA){
					delete AA;
					N->sub[a]=NULL;
					}
				if(BB){
					delete BB;
					N->sub[b]=NULL;
					}
				for(k=0;k<N->sub.size();++k)if(!N->sub[k]){
					N->sub.erase(N->sub.begin()+k);
					--k;
					}
				for(k=0;k<N2->sub.size();++k){
					N->sub.push_back(N2->sub[k]);
					N2->sub[k]=NULL;
					}
				e=(e&e2);
				delete N2;
				X[j]=NULL;
				r=1;
				continue;
				}
			if(AA->n<BB2->n){
				// control vpritul []()
				if(BB && AA2){
					ok=0;
					if(BB->n==AA2->n)if((BB->c|AA2->c)==7)ok=1;
					if(BB->n>AA2->n)ok=1;
					if(ok){
						delete BB;
						N->sub.erase(N->sub.begin()+b);
						delete AA2;
						N2->sub.erase(N2->sub.begin()+a2);
						for(k=0;k<N2->sub.size();++k){
							N->sub.push_back(N2->sub[k]);
							N2->sub[k]=NULL;
							}
						e=(e&e2);
						delete N2;
						X[j]=NULL;
						r=1;
						continue;
						}
					}
				}
			if(AA2->n<BB->n){
				// control vpritul ()[]
				if(BB2 && AA){
					ok=0;
					if(BB2->n==AA->n)if((BB2->c|AA->c)==7)ok=1;
					if(BB2->n>AA->n)ok=1;
					if(ok){
						delete BB2;
						N2->sub.erase(N2->sub.begin()+b2);
						delete AA;
						N->sub.erase(N->sub.begin()+a);
						for(k=0;k<N2->sub.size();++k){
							N->sub.push_back(N2->sub[k]);
							N2->sub[k]=NULL;
							}
						e=(e&e2);
						delete N2;
						X[j]=NULL;
						r=1;
						continue;
						}
					}
				}
			}
		}
	for(i=0;i<X.size();++i)if(X[i])sub.push_back(X[i]);
	for(i=0;i<Y.size();++i)if(Y[i])sub.push_back(Y[i]);
	return r;
}



void nabor::strax2(){
	// (A+B)*C == (A*C)+(B*C)
	// (A+B)*(C+D) == ((A+B)*C)+((A+B)*D)
	int i,j;
	nabor*N,*N2,*N3;
	for(i=0;i<sub.size();++i){
		N=dynamic_cast<nabor*>(sub[i]);
		if(!N)continue;
		N->strax2();
		}
	if(U)return;
	if(sub.size()<2)return;
	Vsos();
	base*A=NULL;
	for(i=0;i<sub.size();++i){
		N=dynamic_cast<nabor*>(sub[i]);
		if(N)if(N->U)break;
		A=sub[j=i];
		}
	if(!A)A=sub[j=1];
	if(N)if(N->U){
		//U=1;
		V_C X;
		X.swap(N->sub);
		//sub.clear();
		if(i>j){
			sub.erase(sub.begin()+i);
			sub.erase(sub.begin()+j);
			}else{
			sub.erase(sub.begin()+j);
			sub.erase(sub.begin()+i);
			}
		delete N;
		N3=new nabor(1);
		for(i=0;i<X.size();++i){
			N2=new nabor(0);
			N2->sub.push_back(X[i]);
			N2->sub.push_back(i?A->copy():A);
			N3->sub.push_back(N2);
			}
		//N3->optimize();
		sub.push_back(N3);
		//cout<<">>"<<toString().c_str()<<endl;
		optimize();
		Vsos();
		return;
		}
}



base* nabor::copy(){
	nabor*N=new nabor(U);
	for(int i=0;i<sub.size();++i)N->sub.push_back(sub[i]->copy());
	return N;
}


string nabor::toBasisString(){
	string s;
	condition*C,*AA,*BB;
	AA=BB=NULL;
	for(int i=0;i<sub.size();++i){
		C=dynamic_cast<condition*>(sub[i]);
		if(!C)continue;
		if(C->c&4)AA=C;
		if(C->c&1)BB=C;
		}
	if(!U){
		if(BB && AA)if(sub.size()==2){
			s+=(BB->c&2?"[":"(");
			s+=BB->getNumber();
			s+=";";
			s+=AA->getNumber();
			s+=(AA->c&2?"]":")");
			}

		return s;
		}
	if(AA){
		s+="(-;";
		s+=AA->getNumber();
		s+=(AA->c&2?"]":")");
		}
	V_S m;
	V_D xs;
	for(i=0;i<sub.size();++i){
		C=dynamic_cast<condition*>(sub[i]);
		if(!C){
			nabor*N=dynamic_cast<nabor*>(sub[i]);
			if(!N)continue;
			double x=0;
			if(N->sub.size()>0){
				C=dynamic_cast<condition*>(N->sub[0]);
				if(C)x=C->n;
				}
			if(N->sub.size()>1){
				C=dynamic_cast<condition*>(N->sub[1]);
				if(C)x=(x+C->n)/2;
				}
			xs.push_back(x);
			m.push_back(N->toBasisString());
			continue;
			}
		if(C==AA)continue;
		if(C==BB)continue;
		xs.push_back(C->n);
		m.push_back(C->getNumber());
		}
	for(i=0;i<xs.size();++i){
		int k=i;
		for(int j=i+1;j<xs.size();++j)if(xs[k]>xs[j])k=j;
		double x=xs[k];
		xs[k]=xs[i];
		xs[i]=x;
		string t=m[k];
		m[k]=m[i];
		m[i]=t;
		}
	for(i=0;i<xs.size();++i){
		if(!s.empty())s+="+";
		s+=m[i];
		}
	if(BB){
		if(!s.empty())s+="+";
		s+=(BB->c&2?"[":"(");
		s+=BB->getNumber();
		s+=";+)";
		}
	return s;
}



// program.cpp :-|