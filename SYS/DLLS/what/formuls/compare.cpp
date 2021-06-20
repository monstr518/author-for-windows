// compare.cpp
#include "stdafx.h"
#include "formuls.h"

int cok=0;

Compare::Compare(){A=B=NULL;}

Compare::~Compare(){
	if(A)delete A;
	if(B)delete B;
}


//-----------------------------------------------------------------------------------------------
bool Compare::Ravno(formuls*A,formuls*B){
	if(*A==*B)return 1;
	Optimizator opA(A->copy()),opB(B->copy());
	A=opA.run();
	B=opB.run();
	if(*A==*B)return 1;
	for(int i=0;i<opA.Formuls.size();++i)
		for(int j=0;j<opB.Formuls.size();++j)
			if(*opA.Formuls[i]==*opB.Formuls[j])return 1;
	return 0;
}


//-----------------------------------------------------------------------------------------------
//?: compare (x+n==x+2-(-n-2))
bool Compare::load(char*s){
	int n;
	SCANER::noProbel(s);
	n=SCANER::scanSlovo("?:",s);
	SCANER::noProbel(s);
	n+=SCANER::scanSlovo("compare",s);
	if(n<2)return 0;
	SCANER::noProbel(s);
	if(*s!='(')return 0;
	++s;
	SCANER::noProbel(s);
	A=formuls::getFormul(s);
	SCANER::noProbel(s);
	char*m[]={"==","!=","<",">","<>",">=","<=",NULL};
	for(int i=0;m[i];++i){
		n=SCANER::scanSlovo(m[i],s);
		if(n)break;
		}
	if(!m[i])return 0;
	if(i==4)i=1;
	znak=m[i];
	SCANER::noProbel(s);
	B=formuls::getFormul(s);
	SCANER::noProbel(s);
	if(*s!=')')return 0;
	++s;
	if(!A || !B)return 0;
	return 1;
}


string Compare::run(){
	bool not=(znak=="!=");
	if(not || znak=="=="){
		bool ok=Compare::Ravno(A,B);
		if(not)ok=!ok;
		return ok?"yes":"no";
		}
	return ".";
}





//-----------------------------------------------------------------------------------------------
Optimize::Optimize(){f=NULL;}

Optimize::~Optimize(){
	if(f)delete f;
}


//?: optimize (pow(a,1+1)) from {a=5;}
bool Optimize::load(char*s){
	int n;
	SCANER::noProbel(s);
	n=SCANER::scanSlovo("?:",s);
	SCANER::noProbel(s);
	n+=SCANER::scanSlovo("optimize",s);
	if(n<2)return 0;
	SCANER::noProbel(s);
	if(*s!='(')return 0;
	++s;
	SCANER::noProbel(s);
	f=formuls::getFormul(s);
	SCANER::noProbel(s);
	if(*s!=')')return 0;
	++s;
	SCANER::noProbel(s);
	n=SCANER::scanSlovo("from",s);
	n+=SCANER::scanSlovo("for",s);
	if(n){
		SCANER::noProbel(s);
		if(*s!='{')return 0;
		++s;
		do{
			SCANER::noProbel(s);
			string name=SCANER::getName(s);
			SCANER::noProbel(s);
			++s;
			double x;
			if(!SCANER::getDigit(s,x))return 0;
			SCANER::noProbel(s);
			++s;
			table[name]=x;
			}while(*s!='}');
		++s;
		}
	return 1;
}


string Optimize::run(){
	if(table.size())f->ReplaceValue(table);
	Optimizator op(f->copy());
	formuls*f2=op.run();
	return f2->toString();
}


//-----------------------------------------------------------------------------------------------
Equality::Equality(){A=B=NULL;}

Equality::~Equality(){
	if(A)delete A;
	if(B)delete B;
}


//?: get equality for (2*y+1=y-10+a) unknown {y,x}
bool Equality::load(char*s){
	int n=0;
	SCANER::noProbel(s);
	n+=SCANER::scanSlovo("?:",s);
	SCANER::noProbel(s);
	n+=SCANER::scanSlovo("get",s);
	SCANER::noProbel(s);
	n+=SCANER::scanSlovo("equality",s);
	SCANER::noProbel(s);
	n+=SCANER::scanSlovo("for",s);
	if(n<4)return 0;
	SCANER::noProbel(s);
	if(*s!='(')return 0;
	++s;
	SCANER::noProbel(s);
	A=formuls::getFormul(s);
	SCANER::noProbel(s);
	if(*s!='=')return 0;
	++s;
	SCANER::noProbel(s);
	B=formuls::getFormul(s);
	if(!A || !B)return 0;
	SCANER::noProbel(s);
	if(*s!=')')return 0;
	++s;
	SCANER::noProbel(s);
	if(SCANER::scanSlovo("unknown",s)){
		SCANER::noProbel(s);
		if(*s!='{')return 0;
		++s;
		do{
			SCANER::noProbel(s);
			string name=SCANER::getName(s);
			if(name.size())unknown.insert(name);
			SCANER::noProbel(s);
			if(*s!='}')++s;
			}while(*s!='}');
		//а есть ли хоть одна переменна€ в неизвестных?
		S_S XS;
		A->getVariables(XS);
		B->getVariables(XS);
		S_S::iterator it=XS.begin();
		for(;it!=XS.end();++it)
			if(find(unknown.begin(),unknown.end(),*it)!=unknown.end())break;
		if(it==XS.end())return 0;
		}else{
		A->getVariables(unknown);
		B->getVariables(unknown);
		}
	return !unknown.empty();
}



void Equality::setRow0(formuls*&A){
	bool nok=1;
	row*X=dynamic_cast<row*>(A);
	if(X)if(!X->level)nok=0;
	if(nok){
		X=new(row);
		X->level=0;
		X->sub.push_back(A);
		X->inverse.push_back(0);
		A=X;
		}
}


void Equality::restructXC(formuls*f,row*X,row*C,bool x){
	row*U=dynamic_cast<row*>(f);
	for(int i=0;i<U->sub.size();++i){
		bool unk=U->sub[i]->isUnknown(unknown);
		bool znak=U->inverse[i];
		if(unk){
			X->sub.push_back(U->sub[i]);
			if(!x)znak=!znak;
			X->inverse.push_back(znak);
			}else{
			C->sub.push_back(U->sub[i]);
			if(x)znak=!znak;
			C->inverse.push_back(znak);
			}
		U->sub[i]=NULL;
		}
	delete f;
}



string Equality::run(){
	{// раскрыть все скобки
	Optimizator::f0(A);
	Optimizator::f0(B);
	Optimizator AO(A),BO(B);
	A=AO.runone(9);
	B=BO.runone(9);
	}
	formuls*Stoper=NULL;
	do{
		//cout<<A->toString().c_str()<<" = "<<B->toString().c_str()<<endl;
		setRow0(A);
		setRow0(B);
		//cout<<A->toString().c_str()<<" = "<<B->toString().c_str()<<endl;
		row*X=new(row),*C=new(row);
		X->level=C->level=0;
		restructXC(A,X,C,1);
		restructXC(B,X,C,0);
		if(C->sub.empty()){
			value*v=new(value);
			v->isValue=1;
			v->x=0;
			C->sub.push_back(v);
			C->inverse.push_back(0);
			}
		A=X;
		B=C;
		//cout<<X->toString().c_str()<<" = "<<C->toString().c_str()<<endl;
		{
		Optimizator AO(A),BO(B);
		A=AO.run()->copy();
		B=BO.run()->copy();
		}
		Pow*P=dynamic_cast<Pow*>(A);
		if(P){// pow(f(x),0.5)=f2()  =>  f(x)=pow(f2(),1/0.5)
			Pow*P2=new(Pow);
			P2->A=B;
			C=new(row);
			C->level=1;
			C->sub.push_back(P->B->copy());
			C->inverse.push_back(1);
			P2->B=C;
			B=P2;
			A=P->A->copy();
			delete P;
			}
		X=dynamic_cast<row*>(A);
		if(X)if(!X->level)if(X->sub.size()==1)if(X->inverse[0]){//-(x*x)=a*b  =>  x*x=-(a*b)
			//X->inverse[0]=0;
			A=X->sub[0];
			X->sub[0]=NULL;
			delete X;
			C=dynamic_cast<row*>(B);
			if(C)if(C->level)C=NULL;
			if(!C){
				C=new(row);
				C->level=0;
				C->sub.push_back(B);
				C->inverse.push_back(0);
				B=C;
				}
			C->ReversZnak();
			}
		//cout<<A->toString().c_str()<<" = "<<B->toString().c_str()<<endl;
		C=dynamic_cast<row*>(B);
		if(C)if(!C->level)C=NULL;
		if(!C){
			C=new(row);
			C->level=1;
			C->sub.push_back(B);
			C->inverse.push_back(0);
			B=C;
			}
		//cout<<A->toString().c_str()<<" = "<<B->toString().c_str()<<endl;
		X=dynamic_cast<row*>(A);
		if(X)if(X->level){
			for(int i=X->sub.size()-1;i>=0;--i){
				formuls*t=X->sub[i];
				if(t->isUnknown(unknown))continue;
				bool znak=X->inverse[i];
				X->sub.erase(X->sub.begin()+i);
				X->inverse.erase(X->inverse.begin()+i);
				C->sub.push_back(t);
				C->inverse.push_back(!znak);
				}
			if(X->sub.size()==1)if(X->inverse[0]){
				X->inverse[0]=0;
				C->ReversZnak();
				}
			}
		//cout<<A->toString().c_str()<<" = "<<B->toString().c_str()<<endl;
		{
		Optimizator AO(A),BO(B);
		A=AO.run()->copy();
		B=BO.run()->copy();
		}
		if(!Stoper)Stoper=A->copy();else{
			if(Compare::Ravno(Stoper,A))break;
			delete Stoper;
			Stoper=A->copy();
			}
		}while(!dynamic_cast<value*>(A));
	if(Stoper)delete Stoper;
	string R=A->toString()+"="+B->toString();
	if(unknown.size()==1)ControlKvadrat(R);
	return R;
}



//—ложить формулу дл€ a b c
void Equality::analizeK(int nn,row*X,row*ff){
	ff->level=0;
	for(int i=0;i<X->sub.size();++i){
		int j,k=0;
		row*Q=dynamic_cast<row*>(X->sub[i]);
		if(!Q){
			value*v=dynamic_cast<value*>(X->sub[i]);
			if(!v)continue;
			if(v->isUnknown(unknown))k=1;
			}else
		for(j=0;j<Q->sub.size();++j){
			if(Q->sub[j]->isUnknown(unknown))++k;
			}
		if(k!=nn)continue;
		row*M=new(row);
		M->level=1;
		if(Q)for(j=0;j<Q->sub.size();++j){
			if(!Q->sub[j]->isUnknown(unknown)){
				M->sub.push_back(Q->sub[j]->copy());
				M->inverse.push_back(Q->inverse[j]);
				}
			}
		if(M->sub.empty()){
			value*v=new(value);
			v->isValue=1;
			v->x=1;
			M->sub.push_back(v);
			M->inverse.push_back(0);
			}
		ff->sub.push_back(M);
		ff->inverse.push_back(X->inverse[i]);
		}
}



//ѕроверить квадрвтное ли оно и решить
void Equality::ControlKvadrat(string&s){
	// (x+2)*x=a
	// x*x=a
	// pow(x+1,2)+m*n*x=a
	// 7*pow(x+1,2)+m*n*x=a
	// ?: get equality for (7*pow(x+1,2)+m*n*x=a) unknown {x}
	// ?: get equality for (x*x+x-(2+n)=0) unknown {x}
	{// раскрыть все скобки и все квадраты pow(f(x),2) => f(x)*f(x)
	Optimizator AO(A);
	A=AO.Open2();
	}
	setRow0(A);
	if(cok)cout<<A->toString().c_str()<<" = "<<B->toString().c_str()<<endl;
	row*X=dynamic_cast<row*>(A);
	if(!X)return;
	int nokvadr=0;
	//ѕроверить квадрвтное ли оно
	for(int i=0;i<X->sub.size();++i){
		Pow*P=dynamic_cast<Pow*>(X->sub[i]);
		if(P){
			if(P->A->isUnknown(unknown))if(P->B->calculated()){
				double n=P->B->compute();
				if(n!=2){
					nokvadr=1;
					break;
					}
				}
			if(P->B->isUnknown(unknown))nokvadr=1;
			}
		row*Q=dynamic_cast<row*>(X->sub[i]);
		if(!Q)continue;
		int k=0;
		for(int j=0;j<Q->sub.size();++j){
			if(Q->sub[j]->isUnknown(unknown)){
				if(Q->inverse[j])--k; else ++k;
				continue;
				}
			}
		if(k==2 && !nokvadr)nokvadr=2;
		if(k>2 || k<0){
			nokvadr=1;
			break;
			}
		}
	if(nokvadr!=2)return;
	//—ложить формулу дл€ а
	row*aa=new(row);
	analizeK(2,X,aa);
	if(cok)cout<<"aa: "<<aa->toString().c_str()<<endl;
	//—ложить формулу дл€ b
	row*bb=new(row);
	analizeK(1,X,bb);
	if(cok)cout<<"bb: "<<bb->toString().c_str()<<endl;
	//—ложить формулу дл€ c
	setRow0(B);
	row*cc=dynamic_cast<row*>(B->copy());
	cc->ReversZnak();
	analizeK(0,X,cc);
	if(cok)cout<<"cc: "<<cc->toString().c_str()<<endl;
	// D=b^2-4*a*c
	row*D=new(row);
	D->level=0;
	value*v=new(value);
	v->isValue=1;
	v->x=2;
	Pow*a2=new(Pow);
	a2->A=bb->copy();
	a2->B=v->copy();
	D->sub.push_back(a2);
	D->inverse.push_back(0);
	row*Q=new(row);
	Q->level=1;
	v->x=4;
	Q->sub.push_back(v);
	Q->inverse.push_back(0);
	Q->sub.push_back(aa->copy());
	Q->inverse.push_back(0);
	Q->sub.push_back(cc->copy());
	Q->inverse.push_back(0);
	D->sub.push_back(Q);
	D->inverse.push_back(1);
	if(cok)cout<<"D: "<<D->toString().c_str()<<endl;
	formuls*DD=D;
	double d=1;
	{
	Optimizator DO(D);
	DD=DO.run()->copy();
	}
	if(DD->calculated())d=DD->compute();
	if(d<0){
		delete aa;
		delete bb;
		delete cc;
		delete DD;
		return;
		}
	if(cok)cout<<"D: "<<DD->toString().c_str()<<endl;
	formuls*X1,*X2;
	X1=getX(DD,aa,bb);
	if(cok)cout<<"X1: "<<X1->toString().c_str()<<endl;
	if(d)X2=getX(DD,aa,bb,1);
	if(cok)if(d)cout<<"X2: "<<X2->toString().c_str()<<endl;
	{
	Optimizator X1O(X1);
	X1=X1O.run()->copy();
	if(d){
		Optimizator X2O(X2);
		X2=X2O.run()->copy();
		}
	}
	if(cok)cout<<"X1: "<<X1->toString().c_str()<<endl;
	if(cok)if(d)cout<<"X2: "<<X2->toString().c_str()<<endl;
	delete aa;
	delete bb;
	delete cc;
	delete DD;
	s=*unknown.begin();
	s+="=";
	if(d==0){
		s+=X1->toString();
		delete X1;
		return;
		}
	s+="{";
	s+=X1->toString();
	s+=", ";
	s+=X2->toString();
	s+="}";
	delete X1;
	delete X2;
}



formuls* Equality::getX(formuls*D,formuls*A,formuls*B,bool p){
	//(-B-pow(D,0.5))/2/A
	row*X=new(row);
	X->level=1;
	Pow*K=new(Pow);
	K->A=D->copy();
	value*v=new(value);
	v->isValue=1;
	v->x=0.5;
	K->B=v->copy();
	row*S=new(row);
	S->level=0;
	S->sub.push_back(B->copy());
	S->inverse.push_back(1);
	S->sub.push_back(K);
	S->inverse.push_back(p);
	X->sub.push_back(S);
	X->inverse.push_back(0);
	v->x=2;
	X->sub.push_back(v);
	X->inverse.push_back(1);
	X->sub.push_back(A->copy());
	X->inverse.push_back(1);
	return X;
}



//-----------------------------------------------------------------------------------------------
Generalized::Generalized(){}

Generalized::~Generalized(){
	for(int i=0;i<S.size();++i)delete S[i];
}


//?: get generalized tree formula for (-0.5*i+6.5, (15-i)/2)
bool Generalized::load(char*s){
	int n=0;
	SCANER::noProbel(s);
	n+=SCANER::scanSlovo("?:",s);
	SCANER::noProbel(s);
	n+=SCANER::scanSlovo("get",s);
	SCANER::noProbel(s);
	n+=SCANER::scanSlovo("generalized",s);
	SCANER::noProbel(s);
	n+=SCANER::scanSlovo("tree",s);
	SCANER::noProbel(s);
	n+=SCANER::scanSlovo("formula",s);
	SCANER::noProbel(s);
	if(n<5)return 0;
	n=SCANER::scanSlovo("for",s);
	SCANER::noProbel(s);
	n+=SCANER::scanSlovo("from",s);
	SCANER::noProbel(s);
	if(n<1)return 0;
	//if(*s!='{')return 0;
	++s;
	do{
		n=0;
		SCANER::noProbel(s);
		formuls*f=formuls::getFormul(s);
		SCANER::noProbel(s);
		if(!f)return 0;
		S.push_back(f);
		if(*s==','){++s;n=1;}
		}while(n);
	return 1;
}



string Generalized::run(){
	S_S Names;
	for(int i=0;i<S.size();++i){
		Optimizator op(S[i]);
		S[i]=op.run()->copy();
		S[i]->getVariables(Names);
		if(cok)cout<<S[i]->toString().c_str()<<endl;
		}
	if(i==0)return ".";
	if(i==1)return "("+S[0]->toString()+")";
	msd=new(M_SDD);
	formuls*f=Comparison(Names);
	if(!f){
		delete msd;
		return ".";
		}
	stringstream R;
	R<<"("<<f->toString()<<")";
	if(msd->size())R<<" for table {";
	M_SDD::iterator it=msd->begin();
	for(;it!=msd->end();++it){
		R<<it->first;
		R<<":";
		V_D*vd=&it->second;
		for(i=0;i<vd->size();++i){
			if(i)R<<",";
			R<<(*vd)[i];
			}
		R<<";";
		}
	if(msd->size())R<<"}";
	delete msd;
	return R.str();
}



int Generalized::NinRow(int&tt){
	int m,t,max=0,type=10;//0-+ 1/* 2pow 3value 4name
	string*name;
	for(int i=0;i<S.size();++i){
		value*v=dynamic_cast<value*>(S[i]);
		row*R=dynamic_cast<row*>(S[i]);
		Pow*P=dynamic_cast<Pow*>(S[i]);
		if(v){t=v->isValue?3:4;m=1;}
		if(R){t=R->level;m=R->sub.size();}
		if(P){t=2;m=2;}
		if(type==10){
			type=t;
			max=m;
			if(t==4)name=&v->name;
			continue;
			}
		if(type!=t)return 0;
		if(m>max)max=m;
		if(t==4)if(*name!=v->name)return 0;
		}
	tt=type;
	return max;
}




string Generalized::newName(S_S&Names){
	int i=0,n='z'-'a'+1,j;
	do{
	string name;
	j=i;
	do{
		name+=('a'+j%n);
		j/=n;
		}while(j);
	if(Names.insert(name).second)return name;
	++i;
	}while(i<1000000);
	return "_";
}




formuls* Generalized::Comparison(S_S&Names){
	int n,t;
	n=NinRow(t);
	if(!n)return NULL;
	if(t==3){
		value*v=new(value);
		v->isValue=0;
		v->name=newName(Names);
		V_D vd;
		for(int i=0;i<S.size();++i){
			value*v2=dynamic_cast<value*>(S[i]);
			vd.push_back(v2->x);
			}
		(*msd)[v->name]=vd;
		return v;
		}
	if(t==4)return S[0]->copy();
	if(t==2){
		Generalized A,B;
		A.msd=B.msd=msd;
		for(int i=0;i<S.size();++i){
			Pow*P=dynamic_cast<Pow*>(S[i]);
			A.S.push_back(P->A);
			B.S.push_back(P->B);
			}
		formuls*fa,*fb;
		fa=A.Comparison(Names);
		fb=B.Comparison(Names);
		A.S.clear();
		B.S.clear();
		if(fa&&fb){
			Pow*P=new(Pow);
			P->A=fa;
			P->B=fb;
			return P;
			}
		return NULL;
		}
	/*
	//нужно дополнить р€ды до одинакового количества компонентов.
	for(int j,i=0;i<S.size();++i){
		row*R=dynamic_cast<row*>(S[i]);
		for(j=R->sub.size();j<n;++j){
			value*v=new(value);
			v->isValue=1;
			v->x=t;
			R->sub.push_back(v);
			R->inverse.push_back(2);
			}
		}
	*/
	//нужно найти матрицу соответствий, если она вообще существует.
	int**M=new(int*[S.size()-1]);
	for(int i=0;i<S.size()-1;++i)M[i]=new(int[n]);
	value*v=new(value);
	v->isValue=1;
	v->x=t;
	formuls*F=NULL;
	formuls*A=NULL,*B;
	row*R;
	int si,ns,j,*m,q,ii,z,zz;
	bool nok,eok;
	for(i=0;i<n;++i){
		for(ns=0;ns<S.size();++ns){
			ii=i;
			if(ns){
				m=M[ns-1];
				m[i]=0;
				}
			do{
				if(ns){
					//следующий не зан€тый m[i]
					for(j=m[i];j<n;++j){
						nok=0;
						for(q=0;q<i;++q)if(m[q]==j){
							nok=1;
							break;
							}
						if(nok)continue;
						m[i]=j;
						break;
						}
					if(j==n){eok=0;break;}
					ii=m[i];
					}
				R=dynamic_cast<row*>(S[ns]);
				si=R->sub.size();
				B=v;
				z=2;
				if(ii<si){
					B=R->sub[ii];
					z=R->inverse[ii];
					}
				if(!ns){
					A=B;
					zz=z;
					eok=1;
					break;
					}
				eok=0;
				if(zz!=z)if(zz<2&&z<2){++m[i];continue;}
				eok=complement(A,B);
				if(!eok)++m[i];
				}while(!eok);
			if(!eok)break;
			}
		if(!eok)break;
		}
	if(eok){
		//матрицу соответствий определена
		R=new(row);
		R->level=t;
		formuls*ff;
		for(i=0;i<n;++i){
			Generalized G;
			G.msd=msd;
			zz=2;
			for(ns=0;ns<S.size();++ns){
				ii=i;
				if(ns)ii=M[ns-1][i];
				row*R2=dynamic_cast<row*>(S[ns]);
				A=v;
				z=2;
				if(ii<R2->sub.size()){
					A=R2->sub[ii];
					z=R2->inverse[ii];
					}
				if(zz==2)zz=z;
				G.S.push_back(A);
				}
			ff=G.Comparison(Names);
			G.S.clear();
			if(ff){
				R->sub.push_back(ff);
				R->inverse.push_back(zz);
				}
			}
		F=R;
		}
	for(i=0;i<S.size()-1;++i)delete[] M[i];
	delete[] M;
	delete v;
	return F;
}




bool Generalized::complement(formuls*A,formuls*B){
	//cout<<"( "<<A->toString().c_str()<<" ?== "<<B->toString().c_str()<<" )"<<endl;
	value*v=dynamic_cast<value*>(A);
	if(v){
		value*v2=dynamic_cast<value*>(B);
		if(!v2)return 0;
		if(v->isValue!=v2->isValue)return 0;
		if(v->isValue)return 1;
		return v->name==v2->name;
		}
	Pow*P=dynamic_cast<Pow*>(A);
	if(P){
		Pow*P2=dynamic_cast<Pow*>(B);
		if(!P2)return 0;
		if(!complement(P->A,P2->A))return 0;
		return complement(P->B,P2->B);
		}
	row*RA=dynamic_cast<row*>(A);
	row*RB=dynamic_cast<row*>(B);
	if(!(RA&&RB))return 0;
	if(RA->level!=RB->level)return 0;
	int a=RA->sub.size(),b=RB->sub.size();
	int *j,i,n,p;
	n=a>b?a:b;
	v=NULL;
	if(a!=b){
		v=new(value);
		v->isValue=1;
		v->x=RA->level;
		}
	bool R=0,nok;
	int*m=new(int[n]);
	int z1,z2;
	for(i=0;i<n;++i){
		A=v;
		z1=2;
		if(i<a){
			A=RA->sub[i];
			z1=RA->inverse[i];
			}
		j=&m[i];
		for(*j=0;*j<n;++*j){
			nok=0;
			for(p=0;p<i;++p)if(m[p]==*j){
				nok=1;
				break;
				}
			if(nok)continue;
			// *j -указывает на номер незан€того елемента в RB->sub[]
			B=v;
			z2=2;
			if(*j<b){
				B=RB->sub[*j];
				z2=RB->inverse[*j];
				}
			if(z1!=z2)if(z1<2&&z2<2)continue;
			if(complement(A,B))break;
			}
		if(*j==n)break;
		}
	R=i==n;
	if(v)delete v;
	delete[] m;
	return R;
}



// compare.cpp	:-|