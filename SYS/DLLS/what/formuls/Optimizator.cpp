// Optimizator.cpp
#include "stdafx.h"
#include "formuls.h"

//-----------------------------------------------------------------------------------------------
Optimizator::Optimizator(formuls*f){
	insertF(f);
	noPow=0;
}


Optimizator::~Optimizator(){
	clearFormuls();
}


void Optimizator::clearFormuls(){
	int i;
	for(i=0;i<Formuls.size();++i)delete Formuls[i];
	Formuls.clear();
}


void Optimizator::insertF(formuls*f){
	int n=Formuls.size();
	int i;
	for(i=0;i<n;++i)
		if(*Formuls[i]==*f){
			delete f;
			return;
			}
	Formuls.push_back(f);
	//cout<<f->toString().c_str()<<"	:"<<f->estimate()<<endl;
}


formuls* Optimizator::run(){
	typedef void (Optimizator::*funct)(formuls*);
	funct mf[]={
		Optimizator::f1,
		Optimizator::f2,
		Optimizator::f3,
		Optimizator::f4,
		Optimizator::f5,
		Optimizator::f6,
		Optimizator::f7,
		Optimizator::f8,
		Optimizator::f9,
		Optimizator::f10,
		Optimizator::f11,
		Optimizator::f12,
		Optimizator::f13,
		Optimizator::f14,
		Optimizator::f15,
		Optimizator::f16,
		Optimizator::f17,
		NULL
		};
	formuls*f;
	int e,t,n,tn,count=400;
	tn=Formuls[0]->estimate();
	do{
		e=tn;
		n=0;
		while(n<Formuls.size() && n<count){
			formuls*f=Formuls[n];
			int i;
			for(i=0;mf[i];++i){
				//cout<<f->toString().c_str()<<":\n";
				(this->*mf[i])(f);
				//cout<<"---------\n";
				}
			++n;
			}
		int i;
		for(i=0;i<Formuls.size();++i){
			t=Formuls[i]->estimate();
			if(!i){
				n=i;
				tn=t;
				continue;
				}
			if(tn>t){
				tn=t;
				n=i;
				}
			}
		//if(!e)e=tn+1;
		f=Formuls[n];
		t=Formuls.size();
		Formuls.erase(Formuls.begin()+n);
		clearFormuls();
		Formuls.push_back(f);
		}while(t>=count && e>tn);
	//cout<<endl;
	//cout<<f->toString().c_str()<<endl;
	return f;
}



formuls* Optimizator::runone(int n){
	typedef void (Optimizator::*funct)(formuls*);
	funct mf[]={
		Optimizator::f1,
		Optimizator::f2,
		Optimizator::f3,
		Optimizator::f4,
		Optimizator::f5,
		Optimizator::f6,
		Optimizator::f7,
		Optimizator::f8,
		Optimizator::f9,
		Optimizator::f10,
		Optimizator::f11,
		Optimizator::f12,
		Optimizator::f13,
		Optimizator::f14,
		Optimizator::f15,
		Optimizator::f16,
		Optimizator::f17,
		NULL
		};
	int i=0;
	formuls*f;
	while(i<Formuls.size()){
		f=Formuls[i];
		(this->*mf[n])(f);
		++i;
		}
	i=Formuls.size()-1;
	f=Formuls[i];
	Formuls.erase(Formuls.begin()+i);
	return f;
}



//раскрыть все скобки
//раскрыть все квадраты pow(f(x),2) => f(x)*f(x)
formuls* Optimizator::Open2(){
	typedef void (Optimizator::*funct)(formuls*);
	funct mf[]={
		Optimizator::f1,
		Optimizator::f2,
		Optimizator::f3,
		Optimizator::f5,
		Optimizator::f6,
		Optimizator::f10,
		Optimizator::f11,
		Optimizator::f12,
		Optimizator::f15,
		Optimizator::f16,
		NULL
		};
	noPow=1;
	formuls*f;
	int count=500,n,i,c;
	for(c=0;c<2;++c){
		n=0;
		while(n<Formuls.size() && n<count){
			formuls*f=Formuls[n];
			for(i=0;mf[i];++i){
				//cout<<f->toString().c_str()<<":\n";
				(this->*mf[i])(f);
				//cout<<"---------\n";
				}
			++n;
			}
		//cout<<"Formuls.size()=="<<Formuls.size()<<endl;
		i=Formuls.size()-1;
		f=Formuls[i];
		Formuls.pop_back();
		clearFormuls();
		Formuls.push_back(f);
		}
	Formuls.clear();
	return f;
}



//-----------------------------------------------------------------------------------------------
// *x+a==x+a
// x*+a==x*a
// (x+a)+y==x+a+y
// (x*a)/y==x*a/y
int  Optimizator::f0(formuls*&f){
	formuls**t=&f;
	int one,one2=0;
	V_ppf S;
	S.push_back(t);
	while(!S.empty()){
		t=*S.begin();
		S.erase(S.begin());
		do{
			one=0;
			row*A=dynamic_cast<row*>(*t);
			if(A){
				if(A->sub.size()==1){
					if(!A->level)if(A->inverse[0]){// -5=>[-5]
						value*v=dynamic_cast<value*>(A->sub[0]);
						if(v)if(v->isValue){
							v->x=-v->x;
							A->inverse[0]=0;
							}
						}
					if(!A->inverse[0]){
						*t=A->sub[0];
						A->sub[0]=NULL;
						delete A;
						++one;
						++one2;
						continue;
						}
					}
				int i;
				for(i=0;i<A->sub.size();++i){
					row*B=dynamic_cast<row*>(A->sub[i]);
					if(!B)continue;
					if(A->level!=B->level)continue;
					for(int j=0;j<B->sub.size();++j){
						A->sub.push_back(B->sub[j]);
						B->sub[j]=NULL;
						int b=B->inverse[j];
						if(A->inverse[i])b=1-b;
						A->inverse.push_back(b);
						}
					delete B;
					A->sub.erase(A->sub.begin()+i);
					A->inverse.erase(A->inverse.begin()+i);
					++one;
					++one2;
					break;
					}
				if(!one)for(int i=0;i<A->sub.size();++i)S.push_back(&A->sub[i]);
				}
			Pow*P=dynamic_cast<Pow*>(*t);
			if(P){
				S.push_back(&P->A);
				S.push_back(&P->B);
				}
			}while(one);
		}
	return one2;
}



void Optimizator::f1(formuls*f){
	f=f->copy();
	if(!f0(f)){
		delete f;
		return;
		}
	insertF(f);
}



// x+0==x
// x*1==x
void Optimizator::f2(formuls*f){
	f=f->copy();
	formuls*t=f;
	int fok,one=0;
	V_f S;
	S.push_back(f);
	while(!S.empty()){
		f=*S.begin();
		S.erase(S.begin());
		do{
			fok=0;
			row*A=dynamic_cast<row*>(f);
			if(A)if(A->sub.size()>1){
				for(int i=0;i<A->sub.size();++i){
					value*v=dynamic_cast<value*>(A->sub[i]);
					if(!v)continue;
					if(!v->isValue)continue;
					if(A->level){
						if(v->x==1)fok=1;
						}else{
						if(v->x==0)fok=1;
						}
					if(fok){
						delete v;
						A->sub.erase(A->sub.begin()+i);
						A->inverse.erase(A->inverse.begin()+i);
						++one;
						break;
						}
					}
				}
			if(A)if(!fok)for(int i=0;i<A->sub.size();++i)S.push_back(A->sub[i]);
			}while(fok);
		Pow*P=dynamic_cast<Pow*>(f);
		if(P){
			S.push_back(P->A);
			S.push_back(P->B);
			}
		}
	if(!one){
		delete t;
		return;
		}
	f0(t);
	insertF(t);
}



// x-x==>0
// x+x==>x*2
// x/x==>1
// x*x==>pow(x,2)
void Optimizator::f3(formuls*f){
	f=f->copy();
	formuls*t=f;
	int one=0;
	V_f S;
	S.push_back(f);
	while(!S.empty()){
		f=*S.begin();
		S.erase(S.begin());
		row*A=dynamic_cast<row*>(f);
		if(A){
			int n=A->sub.size(),k;
			for(int i=0;i<n;++i){
				formuls*x=A->sub[i];
				V_I m(n);// метки для удаления
				m[i]=1;
				k=1;
				for(int j=i+1;j<n;++j)
					if(*x==*A->sub[j]){
						m[j]=1;
						++k;
						}
				if(k<=1)continue;
				row B;
				k=0;
				j=-1;
				for(i=0;i<n;++i)if(m[i]){
					if(j<0)j=i;
					if(A->inverse[i])--k; else ++k;
					}
				if(A->level)if(k){
					value*v=dynamic_cast<value*>(x);
					if(v)if(v->isValue)continue;
					if(noPow)continue;
					}
				for(i=0;i<n;++i)if(!m[i]){
					B.sub.push_back(A->sub[i]);
					A->sub[i]=NULL;
					B.inverse.push_back(A->inverse[i]);
					}
				A->sub.swap(B.sub);
				A->inverse.swap(B.inverse);
				formuls*z;
				if(A->level){
					if(k){
						value*v=new(value);
						v->isValue=1;
						v->x=k;
						Pow*p=new(Pow);
						p->A=B.sub[j];
						B.sub[j]=NULL;
						p->B=v;
						z=p;
						}else{
						value*v=new(value);
						v->isValue=1;
						v->x=1;
						z=v;
						}
					}else{
					if(k){
						value*v=new(value);
						v->isValue=1;
						v->x=k;
						row*h=new(row);
						h->level=1;
						h->sub.push_back(v);
						h->inverse.push_back(0);
						h->sub.push_back(B.sub[j]);
						B.sub[j]=NULL;
						h->inverse.push_back(0);
						z=h;
						}else{
						value*v=new(value);
						v->isValue=1;
						v->x=0;
						z=v;
						}
					}
				A->sub.push_back(z);
				A->inverse.push_back(0);
				++one;
				break;
				}
			for(i=0;i<A->sub.size();++i)S.push_back(A->sub[i]);
			}
		Pow*P=dynamic_cast<Pow*>(f);
		if(P){
			S.push_back(P->A);
			S.push_back(P->B);
			}
		}
	if(!one){
		delete t;
		return;
		}
	f0(f);
	insertF(t);
}



// a*x+x*b=>(a+b)*x
// a*x+x=>(a+1)*x
void Optimizator::f4(formuls*f){
	f=f->copy();
	formuls*t=f;
	V_f S;
	S.push_back(f);
	while(!S.empty()){
		f=*S.begin();
		S.erase(S.begin());
		row*A=dynamic_cast<row*>(f);
		if(A){
			if(!A->level){
				int n=A->sub.size();
				for(int i=0;i<n;++i){
					formuls*x=A->sub[i];
					for(int j=0;j<n;++j)if(i!=j){
						row*B=dynamic_cast<row*>(A->sub[j]);
						if(!B)continue;
						if(B->level!=1)continue;
						for(int y=0;y<B->sub.size();++y){
							if(B->inverse[y])continue;
							if(*B->sub[y]!=*x)continue;
							//1+2-a*b/n*x+3+x
							int inversex=A->inverse[i];
							delete x;
							//1+2-a*b/n*x+3
							x=B->sub[y];
							//formuls*AA=A->sub[j];
							B->sub.erase(B->sub.begin()+y);
							B->inverse.erase(B->inverse.begin()+y);
							//1+2-a*b/n+3
							value*v=new(value);
							v->isValue=1;
							v->x=1;
							row*G=new(row);
							G->level=0;
							G->sub.push_back(B);
							G->inverse.push_back(0);
							G->sub.push_back(v);
							if(A->inverse[j])inversex=1-inversex;
							G->inverse.push_back(inversex);
							row*U=new(row);
							U->level=1;
							U->sub.push_back(G);
							U->inverse.push_back(0);
							U->sub.push_back(x);
							U->inverse.push_back(0);
							A->sub[j]=U;
							//1+2-(a*b/n+1)*x+3
							A->sub.erase(A->sub.begin()+i);
							A->inverse.erase(A->inverse.begin()+i);
							f0(t);
							insertF(t);
							return;
							}
						}
					}
				for(i=0;i<n;++i){
					row*AA=dynamic_cast<row*>(A->sub[i]);
					if(!AA)continue;
					if(!AA->level)continue;
					for(int j=0;j<n;++j)if(i!=j){
						row*BB=dynamic_cast<row*>(A->sub[j]);
						if(!BB)continue;
						if(!BB->level)continue;
						int ok=0;
						for(int bb,aa=0;aa<AA->sub.size();++aa){
							for(bb=0;bb<BB->sub.size();++bb)//if(aa!=bb)
								if(AA->inverse[aa]==BB->inverse[bb])
								if(*AA->sub[aa]==*BB->sub[bb]){
									ok=1;
									break;
									}
							if(ok)break;
							}
						if(!ok)continue;
						// 1+c*a*x+x*b/n
						int znakX=AA->inverse[aa];
						formuls*x=AA->sub[aa];
						AA->sub.erase(AA->sub.begin()+aa);
						AA->inverse.erase(AA->inverse.begin()+aa);
						delete BB->sub[bb];
						BB->sub.erase(BB->sub.begin()+bb);
						BB->inverse.erase(BB->inverse.begin()+bb);
						// 1+c*a+b/n
						int znakA,znakB;
						znakA=A->inverse[i];
						znakB=A->inverse[j];
						if(i>j){
							A->sub.erase(A->sub.begin()+i);
							A->sub.erase(A->sub.begin()+j);
							A->inverse.erase(A->inverse.begin()+i);
							A->inverse.erase(A->inverse.begin()+j);
							}else{
							A->sub.erase(A->sub.begin()+j);
							A->sub.erase(A->sub.begin()+i);
							A->inverse.erase(A->inverse.begin()+j);
							A->inverse.erase(A->inverse.begin()+i);
							}
						// 1+
						row*G=new(row);
						G->level=0;
						G->sub.push_back(AA);
						G->inverse.push_back(znakA);
						G->sub.push_back(BB);
						G->inverse.push_back(znakB);
						//1+  (c*a+b/n)
						row*U=new(row);
						U->level=1;
						U->sub.push_back(G);
						U->inverse.push_back(0);
						U->sub.push_back(x);
						U->inverse.push_back(znakX);
						//1+  (c*a+b/n)*x
						A->sub.push_back(U);
						A->inverse.push_back(0);
						f0(t);
						insertF(t);
						return;
						}
					}
				}
			for(int i=0;i<A->sub.size();++i)S.push_back(A->sub[i]);
			}
		Pow*P=dynamic_cast<Pow*>(f);
		if(P){
			S.push_back(P->A);
			S.push_back(P->B);
			}
		}
	delete f;
}



// (1+2)*x==3*x
// pow(3,2)+x==9+x
void Optimizator::f5(formuls*f){
	f=f->copy();
	formuls**t=&f;
	V_ppf S;
	S.push_back(t);
	while(!S.empty()){
		t=*S.begin();
		S.erase(S.begin());
		value*U=dynamic_cast<value*>(*t);
		if(U)continue;
		if((*t)->calculated()){
			value*v=new(value);
			v->isValue=1;
			v->x=(*t)->compute();
			delete *t;
			*t=v;
			insertF(f);
			return;
			}
		Pow*P=dynamic_cast<Pow*>(*t);
		if(P){
			S.push_back(&P->A);
			S.push_back(&P->B);
			}
		row*R=dynamic_cast<row*>(*t);
		if(R){
			for(int i=0;i<R->sub.size();++i)S.push_back(&R->sub[i]);
			}
		}
	delete f;
}




// pow(x,n)*x=>pow(x,n+1)
// x*0=>0
// pow(x,0)=>1
// pow(x,1)=>x
void Optimizator::f6(formuls*f){
	f=f->copy();
	formuls**t=&f;
	V_ppf S;
	S.push_back(t);
	while(!S.empty()){
		t=*S.begin();
		S.erase(S.begin());
		Pow*P=dynamic_cast<Pow*>(*t);
		if(P){
			value*v=dynamic_cast<value*>(P->B);
			if(v)if(v->isValue){
				if(!v->x){
					v=new(value);
					v->isValue=1;
					v->x=1;
					delete *t;
					*t=v;
					insertF(f);
					return;
					}
				if(v->x==1){
					formuls*ff=P->A;
					P->A=NULL;
					delete P;
					*t=ff;
					insertF(f);
					return;
					}
				}
			S.push_back(&P->A);
			S.push_back(&P->B);
			}
		row*R=dynamic_cast<row*>(*t);
		if(R){
			if(R->level==1){
				for(int i=0;i<R->sub.size();++i){
					value*v=dynamic_cast<value*>(R->sub[i]);
					if(v)if(v->isValue)if(!v->x){
						R->sub[i]=NULL;
						delete *t;
						*t=v;
						insertF(f);
						return;
						}
					P=dynamic_cast<Pow*>(R->sub[i]);
					if(!P)continue;
					if(R->inverse[i])continue;
					for(int j=0;j<R->sub.size();++j)if(i!=j){
						if(*P->A!=*R->sub[j])continue;
						delete R->sub[j];
						int znak=R->inverse[j];
						R->sub.erase(R->sub.begin()+j);
						R->inverse.erase(R->inverse.begin()+j);
						v=new(value);
						v->isValue=1;
						v->x=1;
						row*A=new(row);
						A->level=0;
						A->sub.push_back(P->B);
						A->inverse.push_back(0);
						A->sub.push_back(v);
						A->inverse.push_back(znak);
						P->B=A;
						insertF(f);
						return;
						}
					}
				}
			for(int i=0;i<R->sub.size();++i)S.push_back(&R->sub[i]);
			}
		}
	delete f;
}



// pow(pow(x,a),y)=>pow(x,a*y)
// pow(x,n)*pow(x,m)=>pow(x,n+m)
void Optimizator::f7(formuls*f){
	f=f->copy();
	formuls*t=f;
	V_f S;
	S.push_back(t);
	while(!S.empty()){
		t=*S.begin();
		S.erase(S.begin());
		row*R=dynamic_cast<row*>(t);
		if(R){
			if(R->level==1){
				for(int j,i=0;i<R->sub.size();++i){
					Pow*A,*B;
					A=dynamic_cast<Pow*>(R->sub[i]);
					if(!A)continue;
					for(j=i+1;j<R->sub.size();++j){
						B=dynamic_cast<Pow*>(R->sub[j]);
						if(!B)continue;
						if(*A->A!=*B->A)continue;
						formuls*m=B->B;
						B->B=NULL;
						int znak=R->inverse[j];
						delete R->sub[j];
						R->sub.erase(R->sub.begin()+j);
						R->inverse.erase(R->inverse.begin()+j);
						row*Q=new(row);
						Q->level=0;
						Q->sub.push_back(A->B);
						Q->inverse.push_back(0);
						Q->sub.push_back(m);
						if(R->inverse[i])znak=1-znak;
						Q->inverse.push_back(znak);
						A->B=Q;
						insertF(f);
						return;
						}
					}
				}
			for(int i=0;i<R->sub.size();++i)S.push_back(R->sub[i]);
			}
		Pow*P=dynamic_cast<Pow*>(t);
		if(P){
			Pow*P2=dynamic_cast<Pow*>(P->A);
			if(P2){
				formuls*x=P2->A,*a=P2->B;
				P2->A=P2->B=NULL;
				delete P2;
				P->A=x;
				row*Q=new(row);
				Q->level=1;
				Q->sub.push_back(a);
				Q->inverse.push_back(0);
				Q->sub.push_back(P->B);
				Q->inverse.push_back(0);
				P->B=Q;
				insertF(f);
				return;
				}
			S.push_back(P->A);
			S.push_back(P->B);
			}
		}
	delete f;
}



// /pow(a,n)==*pow(a,-n)
void Optimizator::f8(formuls*f){
	f=f->copy();
	formuls*t=f;
	V_f S;
	S.push_back(t);
	while(!S.empty()){
		t=*S.begin();
		S.erase(S.begin());
		row*R=dynamic_cast<row*>(t);
		if(R){
			if(R->level==1){
				for(int i=0;i<R->sub.size();++i){
					Pow*P=dynamic_cast<Pow*>(R->sub[i]);
					if(!P)continue;
					int ok=R->inverse[i];
					row*W=dynamic_cast<row*>(P->B);
					if(W)if(!W->level)ok=1;
					if(!ok)continue;
					R->inverse[i]=1-R->inverse[i];
					if(W)if(!W->level){ok=0;W->ReversZnak();}
					if(ok){
						W=new(row);
						W->level=0;
						W->sub.push_back(P->B);
						W->inverse.push_back(1);
						P->B=W;
						}
					insertF(f);
					return;
					}
				}
			for(int i=0;i<R->sub.size();++i)S.push_back(R->sub[i]);
			}
		Pow*P=dynamic_cast<Pow*>(t);
		if(P){
			S.push_back(P->A);
			S.push_back(P->B);
			}
		}
	delete f;
}



// x-a-y=>x-(a+y)
// x/a/y=>x/(a*y)
void Optimizator::f9(formuls*f){
	f=f->copy();
	formuls*t=f;
	V_f S;
	S.push_back(t);
	while(!S.empty()){
		t=*S.begin();
		S.erase(S.begin());
		row*R=dynamic_cast<row*>(t);
		if(R){
			int n=0;
			for(int i=0;i<R->inverse.size();++i)
				if(R->inverse[i])++n;
			if(n>1 && n<R->inverse.size()){
				row*U=new(row);
				U->level=R->level;
				for(i=0;i<R->sub.size();++i)if(R->inverse[i]){
					U->sub.push_back(R->sub[i]);
					U->inverse.push_back(0);
					}
				int ok;
				do{
					ok=0;
					for(i=0;i<R->sub.size();++i)if(R->inverse[i]){
						R->sub.erase(R->sub.begin()+i);
						R->inverse.erase(R->inverse.begin()+i);
						ok=1;
						break;
						}
					}while(ok);
				R->sub.push_back(U);
				R->inverse.push_back(1);
				insertF(f);
				return;
				}
			for(i=0;i<R->sub.size();++i)S.push_back(R->sub[i]);
			}
		Pow*P=dynamic_cast<Pow*>(t);
		if(P){
			S.push_back(P->A);
			S.push_back(P->B);
			}
		}
	delete f;
}




// pow(a/b,n)=>pow(a,n)/pow(b,n)
// (a+b)*x=>a*x+b*x
void Optimizator::f10(formuls*f){
	f=f->copy();
	formuls**t=&f;
	V_ppf S;
	S.push_back(t);
	while(!S.empty()){
		t=*S.begin();
		S.erase(S.begin());
		row*R=dynamic_cast<row*>(*t);
		if(R){
			if(R->level==1)if(R->sub.size()>1){
				for(int i=0;i<R->sub.size();++i){
					if(R->inverse[i])continue;
					row*R2=dynamic_cast<row*>(R->sub[i]);
					if(!R2)continue;
					if(R2->level)continue;
					R->sub.erase(R->sub.begin()+i);
					R->inverse.erase(R->inverse.begin()+i);
					row*Q=new(row);
					*t=Q;
					Q->level=0;
					for(int j=0;j<R2->sub.size();++j){
						row*U=new(row);
						U->level=1;
						U->sub.push_back(R2->sub[j]);
						U->inverse.push_back(0);
						U->sub.push_back(R->copy());
						U->inverse.push_back(0);
						Q->sub.push_back(U);
						Q->inverse.push_back(R2->inverse[j]);
						R2->sub[j]=NULL;
						}
					delete R2;
					delete R;
					f0(f);
					insertF(f);
					return;
					}
				}
			for(int i=0;i<R->sub.size();++i)S.push_back(&R->sub[i]);
			}
		Pow*P=dynamic_cast<Pow*>(*t);
		if(P){
			R=dynamic_cast<row*>(P->A);
			if(R)if(R->level){
				formuls*n=P->B;
				P->B=P->A=NULL;
				delete P;
				row*Q=new(row);
				*t=Q;
				Q->level=1;
				for(int i=0;i<R->sub.size();++i){
					P=new(Pow);
					P->A=R->sub[i];
					P->B=i?n->copy():n;
					Q->sub.push_back(P);
					Q->inverse.push_back(R->inverse[i]);
					}
				insertF(f);
				return;
				}
			S.push_back(&P->A);
			S.push_back(&P->B);
			}
		}
	delete f;
}



// 1+2+x=>3+x
// 2*5*x=>10*x
// -[-2]=>+2
void Optimizator::f11(formuls*f){
	f=f->copy();
	formuls*t=f;
	V_f S;
	S.push_back(t);
	while(!S.empty()){
		t=*S.begin();
		S.erase(S.begin());
		row*R=dynamic_cast<row*>(t);
		if(R){
			if(!R->level){
				bool ok=0;
				for(int i=0;i<R->sub.size();++i){
					value*v=dynamic_cast<value*>(R->sub[i]);
					if(!v)continue;
					if(!v->isValue)continue;
					if(!R->inverse[i])continue;
					if(v->x>0)continue;
					R->inverse[i]=0;
					v->x=-v->x;
					ok=1;
					}
				if(ok){
					insertF(f);
					return;
					}
				}
			//if(!R->calculated()){
				double s=0,p=1;
				int k=0;
				for(int i=0;i<R->sub.size();++i){
					if(!R->sub[i]->calculated())continue;
					double t=R->sub[i]->compute();
					if(R->level){
						if(R->inverse[i])p/=t; else p*=t;
						}else{
						if(R->inverse[i])s-=t; else s+=t;
						}
					++k;
					}
				if(k>1){
					for(int i=R->sub.size()-1;i>=0;--i){
						if(!R->sub[i]->calculated())continue;
						R->sub.erase(R->sub.begin()+i);
						R->inverse.erase(R->inverse.begin()+i);
						}
					value*v=new(value);
					v->isValue=1;
					v->x=R->level?p:s;
					R->sub.push_back(v);
					R->inverse.push_back(0);
					insertF(f);
					return;
					}
			//	}
			for(i=0;i<R->sub.size();++i)S.push_back(R->sub[i]);
			}
		Pow*P=dynamic_cast<Pow*>(t);
		if(P){
			S.push_back(P->A);
			S.push_back(P->B);
			}
		}
	delete f;
}



// (a+b)*(p+t)==a*(p+t)+b*(p+t)
void Optimizator::f12(formuls*f){
	f=f->copy();
	formuls*t=f;
	V_f S;
	S.push_back(t);
	while(!S.empty()){
		t=*S.begin();
		S.erase(S.begin());
		row*R=dynamic_cast<row*>(t);
		if(R){
			if(R->level){
				for(int a=0;a<R->sub.size();++a){
					if(R->inverse[a])continue;
					row*A=dynamic_cast<row*>(R->sub[a]);
					if(!A)continue;
					if(A->level)continue;
					for(int b=0;b<R->sub.size();++b)if(a!=b){
						row*B=dynamic_cast<row*>(R->sub[b]);
						if(!B)continue;
						if(B->level)continue;
						int znak=R->inverse[b];
						row*G=new(row);
						G->level=0;
						for(int i=0;i<A->sub.size();++i){
							row*U=new(row);
							U->level=1;
							U->sub.push_back(A->sub[i]);
							U->inverse.push_back(0);
							A->sub[i]=NULL;
							U->sub.push_back(i?B->copy():B);
							U->inverse.push_back(znak);
							G->sub.push_back(U);
							G->inverse.push_back(A->inverse[i]);
							}
						delete A;
						R->sub[a]=G;
						R->sub.erase(R->sub.begin()+b);
						R->inverse.erase(R->inverse.begin()+b);
						f0(f);
						insertF(f);
						return;
						}
					}
				}
			for(int i=0;i<R->sub.size();++i)S.push_back(R->sub[i]);
			}
		Pow*P=dynamic_cast<Pow*>(t);
		if(P){
			S.push_back(P->A);
			S.push_back(P->B);
			}
		}
	delete f;
}




// a/b+x/y+t=>(a*y+x*b)/(b*y)+t
// a/b-x/y=>(a*y-x*b)/(b*y)
void Optimizator::f13(formuls*f){
	f=f->copy();
	f0(f);
	formuls*t=f;
	V_f S;
	S.push_back(t);
	while(!S.empty()){
		t=*S.begin();
		S.erase(S.begin());
		row*R=dynamic_cast<row*>(t);
		if(R){
			if(!R->level)if(R->sub.size()>1){
				int ai,bi,c;
				row*A=NULL;
				for(int a=0;a<R->sub.size();++a){
					A=dynamic_cast<row*>(R->sub[a]);
					if(!A)continue;
					if(A->level){
						for(int n=0,j=0;j<A->sub.size();++j)
							if(A->inverse[j]){
								ai=j;
								++n;
								break;
								}
						if(n)break;
						}
					A=NULL;
					}
				if(!A)continue;
				row*B=NULL;
				formuls*C=NULL;
				for(int b=0;b<R->sub.size();++b)if(a!=b){
					//if(!C)C=R->sub[c=b];
					B=dynamic_cast<row*>(R->sub[b]);
					if(!B){
						if(!C)C=R->sub[c=b];
						continue;
						}
					if(B->level){
						for(int n=0,j=0;j<B->sub.size();++j)
							if(B->inverse[j]){
								if(*B->sub[j]!=*A->sub[ai]){
									bi=j;
									++n;
									break;
									}
								}
						if(n)break;
						}
					B=NULL;
					//if(!C)C=R->sub[c=b];
					}
				if(!B)if(C){
					// a/b+t+k==(a+t*b)/b+k
					int znak=R->inverse[c];
					formuls*Z=A->sub[ai];
					A->sub.erase(A->sub.begin()+ai);
					A->inverse.erase(A->inverse.begin()+ai);
					// w*a*s/m+k  b  t
					row*S=new(row);
					S->level=0;
					S->sub.push_back(A);
					S->inverse.push_back(R->inverse[a]);
					row*U=new(row);
					U->level=1;
					U->sub.push_back(C);
					U->inverse.push_back(0);
					U->sub.push_back(Z->copy());
					U->inverse.push_back(0);
					S->sub.push_back(U);
					S->inverse.push_back(znak);
					R->inverse[a]=0;
					// (w*a*s/m+t*b)  ()+k  b  t
					row*G=new(row);
					G->level=1;
					G->sub.push_back(S);
					G->inverse.push_back(0);
					G->sub.push_back(Z);
					G->inverse.push_back(1);
					R->sub[a]=G;
					// w*a/b*s/m+k  t
					R->sub.erase(R->sub.begin()+c);
					R->inverse.erase(R->inverse.begin()+c);
					f0(f);
					insertF(f);
					return;
					}
				// в A i B недолжно быть 1
				for(int ja=0;ja<A->sub.size();++ja){
					value*v=dynamic_cast<value*>(A->sub[ja]);
					if(!v)continue;
					if(!v->isValue)continue;
					if(v->x==1){
						B=NULL;
						break;
						}
					}
				if(B){
					for(ja=0;ja<B->sub.size();++ja){
						value*v=dynamic_cast<value*>(B->sub[ja]);
						if(!v)continue;
						if(!v->isValue)continue;
						if(v->x==1){
							B=NULL;
							break;
							}
						}
					}
				// недолжно быть обшего /n в A i B :  A(x/n)+B(y/n)
				if(B){
					for(int ja=0;ja<A->sub.size();++ja)if(A->inverse[ja]){
						formuls*AA=A->sub[ja];
						for(int jb=0;jb<B->sub.size();++jb)if(B->inverse[jb])
							if(*AA==*B->sub[jb]){
								B=NULL;
								break;
								}
							if(!B)break;
							}
					}
				if(B){
					// a/b+x/y+k==(a*y+x*b)/(b*y)+k
					R->sub.erase(R->sub.begin()+b);
					int znak=R->inverse[b];
					R->inverse.erase(R->inverse.begin()+b);
					// a/b+k   +x/y
					row*Z=new(row);
					Z->level=1;
					Z->sub.push_back(A->sub[ai]->copy());
					Z->inverse.push_back(0);
					Z->sub.push_back(B->sub[bi]->copy());
					Z->inverse.push_back(0);
					// a/b+k   +x/y   b*y
					formuls*T=A->sub[ai];
					A->sub[ai]=B->sub[bi];
					B->sub[bi]=T;
					A->inverse[ai]=0;
					B->inverse[bi]=0;
					// a*y+k   +x*b   b*y
					row*S=new(row);
					S->level=0;
					S->sub.push_back(A);
					S->inverse.push_back(R->inverse[a]);
					S->sub.push_back(B);
					S->inverse.push_back(znak);
					// (a*y+x*b)   ()+k   b*y
					row*G=new(row);
					G->level=1;
					G->sub.push_back(S);
					G->inverse.push_back(0);
					G->sub.push_back(Z);
					G->inverse.push_back(1);
					R->sub[a]=G;
					R->inverse[a]=0;
					f0(f);
					insertF(f);
					return;
					}
				}
			//for(int i=0;i<R->sub.size();++i)S.push_back(R->sub[i]);
			}
		Pow*P=dynamic_cast<Pow*>(t);
		if(P){
			S.push_back(P->A);
			S.push_back(P->B);
			}
		}
	delete f;
}



// pow(a+b,2)==pow(a,2)+2*a*b+pow(b,2)
// pow(a-b,2)==pow(a,2)-2*a*b+pow(b,2)
void Optimizator::f14(formuls*f){
	f=f->copy();
	formuls**t=&f;
	V_ppf S;
	S.push_back(t);
	while(!S.empty()){
		t=*S.begin();
		S.erase(S.begin());
		row*R=dynamic_cast<row*>(*t);
		if(R){
			for(int i=0;i<R->sub.size();++i)S.push_back(&R->sub[i]);
			}
		Pow*P=dynamic_cast<Pow*>(*t);
		if(P){
			if(P->B->calculated()){
				double x=P->B->compute();
				int n=x;
				if(x==n && n>0 && n<4){
					R=new(row);
					R->level=1;
					for(int i=0;i<n;++i){
						R->sub.push_back(i?P->A->copy():P->A);
						R->inverse.push_back(0);
						}
					P->A=NULL;
					delete P;
					*t=R;
					f0(f);
					insertF(f);
					return;
					}
				}
			S.push_back(&P->A);
			S.push_back(&P->B);
			}
		}
	delete f;
}



// pow(a,2)-pow(b,2)=>(a-b)*(a+b)
void Optimizator::f15(formuls*f){
	f=f->copy();
	formuls*t=f;
	V_f S;
	S.push_back(t);
	while(!S.empty()){
		t=*S.begin();
		S.erase(S.begin());
		row*R=dynamic_cast<row*>(t);
		if(R){
			if(!R->level){
				int a,b;
				Pow*A=NULL,*B=NULL,*T;
				for(int i=0;i<R->sub.size();++i){
					T=dynamic_cast<Pow*>(R->sub[i]);
					if(!T)continue;
					if(!T->B->calculated())continue;
					if(T->B->compute()!=2)continue;
					if(R->inverse[i]){
						if(!B){
							B=T;
							a=i;
							}
						}else{
						if(!A){
							A=T;
							b=i;
							}
						}
					}
				if(A && B){
					if(a>b){
						i=a;
						a=b;
						b=i;
						}
					R->sub.erase(R->sub.begin()+b);
					R->inverse.erase(R->inverse.begin()+b);
					R->sub.erase(R->sub.begin()+a);
					R->inverse.erase(R->inverse.begin()+a);
					row*U=new(row);
					U->level=0;
					U->sub.push_back(A->A->copy());
					U->inverse.push_back(0);
					U->sub.push_back(B->A->copy());
					U->inverse.push_back(1);
					row*G=new(row);
					G->level=1;
					G->sub.push_back(U);
					G->inverse.push_back(0);
					U=new(row);
					U->level=0;
					U->sub.push_back(A->A);
					U->inverse.push_back(0);
					U->sub.push_back(B->A);
					U->inverse.push_back(0);
					A->A=B->A=NULL;
					delete A;
					delete B;
					G->sub.push_back(U);
					G->inverse.push_back(0);
					R->sub.push_back(G);
					R->inverse.push_back(0);
					f0(f);
					insertF(f);
					return;
					}
				}
			for(int i=0;i<R->sub.size();++i)S.push_back(R->sub[i]);
			}
		Pow*P=dynamic_cast<Pow*>(t);
		if(P){
			S.push_back(P->A);
			S.push_back(P->B);
			}
		}
	delete f;
}



// pow(f,2)==>f*f
void Optimizator::f16(formuls*f){
	f=f->copy();
	formuls**t=&f;
	int one=0;
	V_ppf S;
	S.push_back(t);
	while(!S.empty()){
		t=*S.begin();
		S.erase(S.begin());
		row*A=dynamic_cast<row*>(*t);
		if(A){
			int n=A->sub.size();
			for(int i=0;i<n;++i)S.push_back(&A->sub[i]);
			}
		Pow*P=dynamic_cast<Pow*>(*t);
		if(P){
			double n=0;
			if(P->B->calculated())n=P->B->compute();
			if(n==2){
				formuls*ff=P->A;
				P->A=NULL;
				delete P;
				P=NULL;
				row*w=new(row);
				w->level=1;
				w->sub.push_back(ff);
				w->inverse.push_back(0);
				w->sub.push_back(ff->copy());
				w->inverse.push_back(0);
				*t=w;
				one=1;
				S.push_back(&w->sub[0]);
				S.push_back(&w->sub[1]);
				}
			}
		if(P){
			S.push_back(&P->A);
			S.push_back(&P->B);
			}
		}
	if(!one){
		delete f;
		return;
		}
	f0(f);
	insertF(f);
}


//-0.5*i+6.5 ==> 0.5*(-1*i+13)
//0.5*n ==> /2*n
void Optimizator::f17(formuls*f){
	f=f->copy();
	formuls**t=&f;
	int one=0,j,i,n;
	V_ppf S;
	S.push_back(t);
	while(!S.empty()){
		t=*S.begin();
		S.erase(S.begin());
		row*A=dynamic_cast<row*>(*t);
		if(A){
			value*v;
			n=A->sub.size();
			if(A->level){//0.5*n ==> /2*n
				for(i=0;i<n;++i){
					v=dynamic_cast<value*>(A->sub[i]);
					if(!v)continue;
					if(!v->isValue)continue;
					if(v->x!=int(v->x))if(1/v->x==int(1/v->x))break;
					}
				if(i<n){
					v->x=1/v->x;
					A->inverse[i]=1-A->inverse[i];
					one=1;
					}
				}
			if(!A->level){
				j=-1;
				for(i=0;i<n;++i){
					v=dynamic_cast<value*>(A->sub[i]);
					if(!v){if(j==-1)j=i;else j=-2;}else
						if(!v->isValue)j=-2;
					}
				if(j>=0){
					row*B=dynamic_cast<row*>(A->sub[j]);
					if(B)if(B->level){
						int n2=B->sub.size();
						for(i=0;i<n2;++i){
							v=dynamic_cast<value*>(B->sub[i]);
							if(!v)continue;
							if(v->isValue)break;
							}
						if(i<n2)if(v->x!=1){
							double w=v->x;
							int z=B->inverse[i];
							v->x=1;
							for(i=0;i<n;++i){
								v=dynamic_cast<value*>(A->sub[i]);
								if(!v)continue;
								v->x=z?v->x*w:v->x/w;
								if(v->x<0){
									v->x=-v->x;
									A->inverse[i]=1-A->inverse[i];
									}
								}
							row*C=new(row);
							C->level=1;
							C->sub.push_back(A);
							C->inverse.push_back(0);
							v=new(value);
							v->isValue=1;
							v->x=w;
							C->sub.push_back(v);
							C->inverse.push_back(z);
							*t=C;
							insertF(f);
							return;
							}
						}
					}
				}
			for(i=0;i<n;++i)S.push_back(&A->sub[i]);
			}
		Pow*P=dynamic_cast<Pow*>(*t);
		if(P){
			S.push_back(&P->A);
			S.push_back(&P->B);
			}
		}
	if(!one){
		delete f;
		return;
		}
	insertF(f);
}



// // pow(a+b,3)==pow(a,3)+3*pow(a,2)*b+3*b*pow(b,2)-pow(b,3)
// // pow(a-b,3)==pow(a,3)-3*pow(a,2)*b+3*b*pow(b,2)-pow(b,3)
// // pow(a,3)+pow(a,3)==...
// // a*(x-x1)*(x-x2)==...



// Optimizator.cpp	:-|