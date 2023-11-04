// Digit.cpp

#include "main.h"


Digits::Digits(){znak=1;mlimit=10;}

Digits::Digits(const Digits&X){*this=X;}

Digits& Digits:: operator = (const Digits&X){
	znak = X.znak;
	mantisa = X.mantisa;
	exponenta = X.exponenta;
	mlimit = X.mlimit;
	return *this;
}


void Digits::clear(){
	znak = 1;
	exponenta.clear();
	mantisa.clear();
}



void Digits::zdvigUP1E(char c){
	if(exponenta.empty()){
		if(!c)return;
		exponenta.push_back(c);
		return;
		}
	char a,b;
	int i;
	for(i=0;i<exponenta.size();++i){
		char*x=&exponenta[i];
		a=*x/10;
		b=*x%10;
		*x=b*10+c;
		c=a;
		}
	if(c)exponenta.push_back(c);
}



void Digits::set(int n){
	clear();
	if(n<0){
		znak=0;
		n=-n;
		}
	char s[12];
	sprintf(s,"%d",n);
	int i;
	for(i=0;s[i];++i)zdvigUP1E(s[i]-'0');
}



void Digits::set(double n){
	int t=n;
	set(t);
	n-=t;
	if(!n)return;
	if(n<0)n=-n;
	bool r=1;
	do{
		n*=10;
		t=n;
		n-=t;
		t=t%10;
		if(mantisa.empty())mantisa.push_back(t*10);else{
			char*h=&*mantisa.rbegin();
			if(r)*h+=t;else mantisa.push_back(t*10);
			r=!r;
			}
		}while(n);
}




void Digits::set(const char*n){
	clear();
	while(*n==' ')++n;
	if(*n=='-'){
		znak = 0;
		++n;
		}
	while(*n && *n=='0')++n;
	const char *End = n;
	while(*End && *End!='.')++End;
	int p, size = End - n;
	p = size%2;
	{
		int sizeE = p + size/2;
		V_C E(sizeE,0);
		exponenta.swap(E);
	}
	const char *Pos = End;
	--Pos;
	char a, b, x;
	p = 0;
	while(n<=Pos){
		b = *Pos - '0';
		a = 0;
		--Pos;
		if(n<=Pos){
			a = *Pos - '0';
			a *= 10;
			--Pos;
			}
		a += b;
		exponenta[p] = a;
		++p;
		}
	n = End;
	if(!*n)return;
	if(*n=='.'){
		++n;
		bool r=1;
		while(*n){
			x=*n-'0';
			if(x>9)break;
			if(mantisa.empty())mantisa.push_back(x*10);else{
				char*h=&*mantisa.rbegin();
				if(r)*h+=x;else mantisa.push_back(x*10);
				r=!r;
				}
			++n;
			}
		antiZero(mantisa);
		if(!*n)return;
		}
	if(!(*n=='e' || *n=='E'))return;
	++n;
	bool minus = 0;
	if(*n=='-')minus=1;
	if(*n=='-' || *n=='+')++n;
	int u = 0;
	sscanf(n,"%d",&u);
	if(minus)*this>>u; else *this<<u;
	antiZero(mantisa);
}





void Digits::antiZero(V_C&row){
	while(row.size()){
		char h = *row.rbegin();
		if(h)break;
		row.pop_back();
		}
}





string Digits::toString(){
	// znak: 0- 1+
	antiZero(exponenta);
	antiZero(mantisa);
	int size, sizeE, sizeM, pos;
	int sizeER, sizeMR, i;
	sizeER = sizeE = exponenta.size();
	sizeMR = sizeM = mantisa.size();
	bool isEmptyFirstE, isEmptyLastM; // 01.10
	if(sizeE){
		char c = exponenta[sizeE - 1];
		sizeE *= 2;
		isEmptyFirstE = (c<10);
		if(isEmptyFirstE)--sizeE;
		}
	if(sizeM){
		char c = mantisa[sizeM - 1];
		sizeM *= 2;
		isEmptyLastM = ((c%10)==0);
		if(isEmptyLastM)--sizeM;
		}
	bool isZerro = !(sizeE || sizeM);
	bool isNeedMinus = (!znak && !isZerro);
	bool isNeedPset = (sizeM>0);
	bool isNeedZerro = (isNeedPset && (sizeE==0) || isZerro);
	size = sizeE + sizeM + 1;
	if(isNeedMinus)++size;
	if(isNeedZerro)++size;
	if(isNeedPset)++size;
	char *text = new char[size];
	//ZeroMemory(text,size);
	text[size-1] = 0;
	pos = 0;
	if(isNeedMinus)text[pos++] = '-';
	if(isNeedZerro)text[pos++] = '0';
	i = sizeER - 1;
	bool ok = !isEmptyFirstE;
	char a, b, c;
	for(;i>=0;--i){
		c = exponenta[i];
		a = (c/10);
		b = (c%10);
		if(ok)text[pos++] = ('0' + a);
		ok = true;
		text[pos++] = ('0' + b);
		}
	if(isNeedPset)text[pos++] = '.';
	i = 0;
	ok = true;
	while(i<sizeMR){
		c = mantisa[i];
		a = (c/10);
		b = (c%10);
		++i;
		text[pos++] = ('0' + a);
		if(i==sizeMR)if(isEmptyLastM)ok = false;
		if(ok)text[pos++] = ('0' + b);
		}
	string s(text);
	delete[] text;
	return s;
}





Digits:: operator string (){return toString();}

Digits:: operator int (){
	int r = 0;
	int i;
	i = exponenta.size()-1;
	for(;i>=0;--i){ // if(exponenta.size()>i)
		r *= 100;
		r += exponenta[i];
		}
	if(!znak)r = -r;
	return r;
}


Digits:: operator double (){
	double r=0,p=1;
	int i;
	for(i=exponenta.size()-1;i>=0;--i){ // if(exponenta.size()>i)
		r*=100;
		r+=exponenta[i];
		}
	for(i=0;i<mantisa.size();++i){ // if(mantisa.size()>i)
		p/=100;
		r+=mantisa[i]*p;
		}
	if(!znak)r=-r;
	return r;
}

void Digits::invertZnak(){
	znak=!znak;
}


//--------------------------------------------------------------------------------------------------
void Digits::zdvigUP1(){zdvigUP1E(zdvigUP1M());}


char Digits::zdvigUP1M(){
	char a,b,c=0;
	int i;
	for(i=mantisa.size()-1;i>=0;--i){
		char*x=&mantisa[i];
		a=*x/10;
		b=*x%10;
		*x=b*10+c;
		c=a;
		}
	if(mantisa.size())if(!*mantisa.rbegin())mantisa.pop_back();
	return c;
}


void Digits::zdvigUP2(){zdvigUP2E(zdvigUP2M());}

void Digits::zdvigUP2E(char c){exponenta.insert(exponenta.begin(),c);}

char Digits::zdvigUP2M(){
	char c=0;
	if(mantisa.size()){
		c=*mantisa.begin();
		mantisa.erase(mantisa.begin());
		}
	return c;
}


void Digits:: operator << (int a){
	while(a>1){
		zdvigUP2();
		a-=2;
		}
	if(a>0)zdvigUP1();
	antiZero(exponenta);
	antiZero(mantisa);
}


//--------------------------------------------------------------------------------------------------
char Digits::zdvigDOWN1E(){
	char a,b,c=0;
	int i;
	for(i=exponenta.size()-1;i>=0;--i){
		char*x=&exponenta[i];
		a=*x/10;
		b=*x%10;
		*x=c*10+a;
		c=b;
		}
	if(exponenta.size())if(!*exponenta.rbegin())exponenta.pop_back();
	return c;
}


void Digits::zdvigDOWN1M(char c){
	if(mantisa.empty()){
		if(c)mantisa.push_back(c*10);
		return;
		}
	char a,b;
	int i;
	for(i=0;i<mantisa.size();++i){
		char*x=&mantisa[i];
		a=*x/10;
		b=*x%10;
		*x=c*10+a;
		c=b;
		}
	if(c)mantisa.push_back(c*10);
}



char Digits::zdvigDOWN2E(){
	char c = 0;
	if(exponenta.size()){
		c = *exponenta.begin();
		exponenta.erase(exponenta.begin());
		}
	return c;
}


void Digits::zdvigDOWN2M(char c){
	mantisa.insert(mantisa.begin(),c);
	antiZero(mantisa);
}


void Digits::zdvigDOWN1(){zdvigDOWN1M(zdvigDOWN1E());}

void Digits::zdvigDOWN2(){zdvigDOWN2M(zdvigDOWN2E());}


void Digits:: operator >> (int a){
	while(a>1){
		zdvigDOWN2();
		a -= 2;
		}
	if(a>0)zdvigDOWN1();
	antiZero(exponenta);
	antiZero(mantisa);
}


//--------------------------------------------------------------------------------------------------
void Digits::plus1(){
	bool ok = 1;
	int i = 0;
	for(;i<exponenta.size() && ok;++i){
		ok = 0;
		char *h = &exponenta[i];
		++*h;
		if(*h>=100){
			*h = 0;
			ok = 1;
			}
		}
	if(ok)exponenta.push_back(1);
}


void Digits::minus1(){
	bool ok=1;
	int i;
	for(i=0;i<exponenta.size() && ok;++i){
		ok=0;
		char*h=&exponenta[i];
		if(*h)--*h;else{
			*h=99;
			ok=1;
			}
		}
	if(ok){
		exponenta.clear();
		znak=!znak;
		not();
		}
}


void Digits::not(){
	char*x=NULL;
	int i;
	for(i=0;i<mantisa.size();++i){
		x=&mantisa[i];
		*x=100-*x-1;
		}
	if(x)++*x;
}


Digits& Digits:: operator ++ (){
	if(znak)plus1();else minus1();
	return *this;
}


Digits& Digits:: operator -- (){
	if(!znak)plus1();else minus1();
	return *this;
}


//--------------------------------------------------------------------------------------------------
bool Digits::isZero() const {
	return exponenta.empty() && mantisa.empty();
}


char Digits::compare(const Digits&A){
	if(isZero() && A.isZero())return '=';
	if(znak!=A.znak)return znak?'>':'<';
	if(exponenta.size()!=A.exponenta.size()){
		bool r=(exponenta.size()>A.exponenta.size());
		if(!znak)r=!r;
		return r?'>':'<';
		}
	bool r;
	int i;
	for(i=exponenta.size()-1;i>=0;--i)if(exponenta[i]!=A.exponenta[i]){
		r=(exponenta[i]>A.exponenta[i]);
		if(!znak)r=!r;
		return r?'>':'<';
		}
	for(i=0;i<mantisa.size() && i<A.mantisa.size();++i)if(mantisa[i]!=A.mantisa[i]){
		r=(mantisa[i]>A.mantisa[i]);
		if(!znak)r=!r;
		return r?'>':'<';
		}
	if(mantisa.size()==A.mantisa.size())return '=';
	r=(mantisa.size()>A.mantisa.size());
	if(!znak)r=!r;
	return r?'>':'<';
}


char Digits::compareABS(const Digits&A) const {
	if(isZero() && A.isZero())return '=';
	if(exponenta.size()!=A.exponenta.size()){
		bool r=(exponenta.size()>A.exponenta.size());
		return r?'>':'<';
		}
	bool r;
	int i;
	for(i=exponenta.size()-1;i>=0;--i)if(exponenta[i]!=A.exponenta[i]){
		r=(exponenta[i]>A.exponenta[i]);
		return r?'>':'<';
		}
	for(i=0;i<mantisa.size() && i<A.mantisa.size();++i)if(mantisa[i]!=A.mantisa[i]){
		r=(mantisa[i]>A.mantisa[i]);
		return r?'>':'<';
		}
	if(mantisa.size()==A.mantisa.size())return '=';
	r=(mantisa.size()>A.mantisa.size());
	return r?'>':'<';
}



bool Digits:: operator == (const Digits&X){return compare(X)=='=';}

bool Digits:: operator > (const Digits&X){return compare(X)=='>';}

bool Digits:: operator < (const Digits&X){return compare(X)=='<';}


//--------------------------------------------------------------------------------------------------
void Digits::suma(const Digits&A){
	while(mantisa.size()<A.mantisa.size())mantisa.push_back(0);
	unsigned char p=0,x;
	int i;
	for(i=A.mantisa.size()-1;i>=0;--i){
		x=A.mantisa[i]+mantisa[i]+p;
		p=x/100;
		mantisa[i]=x%100;
		}
	for(i=0;i<A.exponenta.size() || i<exponenta.size() || p;++i){
		x=p;
		if(i<A.exponenta.size())x+=A.exponenta[i];
		if(i<exponenta.size())x+=exponenta[i];
		p=x/100;
		x=x%100;
		if(i<exponenta.size())exponenta[i]=x;else exponenta.push_back(x);
		}
	antiZero(mantisa);
}


// this mast be >= A
void Digits::minus(const Digits&A){
	while(mantisa.size()<A.mantisa.size())mantisa.push_back(0);
	unsigned char p=0;
	int x;
	int i;
	for(i=A.mantisa.size()-1;i>=0;--i){
		x=mantisa[i]-A.mantisa[i]-p;
		if(x<0){p=1;x+=100;}else p=0;
		mantisa[i]=x;
		}
	for(i=0;i<exponenta.size();++i){
		x=exponenta[i]-p;
		if(i<A.exponenta.size())x-=A.exponenta[i];else 
			if(!p)break;
		if(x<0){p=1;x+=100;}else p=0;
		exponenta[i]=x;
		}
	antiZero(mantisa);
	antiZero(exponenta);
}



Digits& Digits:: operator += (const Digits&A){
	if(znak==A.znak){
		suma(A);
		return *this;
		}
	bool b = (*this<A);
	if(znak)b = !b;
	if(b){
		minus(A);
		return *this;
		}
	Digits X = A;
	X.minus(*this);
	*this = X;
	return *this;
}



Digits& Digits:: operator -= (const Digits&A){
	if(znak!=A.znak){
		suma(A);
		return *this;
		}
	char t=compareABS(A);
	if(t=='='){
		clear();
		return *this;
		}
	if(t=='>'){
		minus(A);
		return *this;
		}
	Digits X=A;
	X.minus(*this);
	*this = X;
	znak = !znak;
	return *this;
}



//--------------------------------------------------------------------------------------------------
void Digits::umnogenie(const char n){
	int x;
	char p = 0;
	int i, size;
	for(i = mantisa.size()-1;i>=0;--i){
		x = mantisa[i]*n + p;
		p = x/100;
		mantisa[i] = x%100;
		}
	size = exponenta.size();
	for(i = 0;i<size;++i){
		x = exponenta[i]*n + p;
		p = x/100;
		exponenta[i] = x%100;
		}
	if(p)exponenta.push_back(p);
}





Digits& Digits:: operator *= (const Digits&A){
	bool z = (znak==A.znak);
	Digits X = *this, Y = *this;
	clear();
	int i, size;
	char c;
	size = A.exponenta.size();
	for(i=0;i<size;++i){
		c = A.exponenta[i];
		if(i)X.zdvigUP2();
		if(c>0){
			if(c>1){
				Digits W = X;
				W.umnogenie(c);
				suma(W);
				}else{
				suma(X);
				}
			}
		}
	size = A.mantisa.size();
	for(i=0;i<size;++i){
		Y.zdvigDOWN2();
		c = A.mantisa[i];
		if(c>0){
			if(c>1){
				Digits W = Y;
				W.umnogenie(c);
				suma(W);
				}else{
				suma(Y);
				}
			}
		}
	znak = z;
	antiZero(mantisa);
	return *this;
}





Digits& Digits:: operator %= (const Digits&A){
	Digits N;
	N.exponenta = A.exponenta;
	int zdvig = exponenta.size() - N.exponenta.size();
	if(zdvig<=0){
		if(!zdvig)
		while(compareABS(N)!='<')minus(N);
		return *this;
		}
	N<<(2*zdvig);
	bool ok=1;
	while(ok){
		ok = 0;
		while(compareABS(N)!='<')minus(N);
		if(zdvig){
			--zdvig;
			N.zdvigDOWN2();
			ok = 1;
			}
		}
	return *this;
}




Digits& Digits:: operator /= (const Digits&A){
	if(A.isZero() || isZero())return *this;
	Digits N = A, R;
	R.mlimit = mlimit;
	bool z = (znak==A.znak);
	int zdvig = (exponenta.size()-A.exponenta.size())*2;
	if(exponenta.size() && A.exponenta.size()){
		char a, b, bb;
		a = *exponenta.rbegin();
		b = *A.exponenta.rbegin();
		a -= a%10;
		b -= bb = b%10;
		if(a && !b)if(a>bb*10)++zdvig;
		}
	if(zdvig<0)N>>-(zdvig);else N<<(zdvig);
	int p = mlimit;
	while(zdvig>=-p){
		char x = 0;
		while(compareABS(N)!='<'){
			minus(N);
			++x;
			}
		if(x){
			Digits P;
			P.exponenta.push_back(x);
			//if(zdvig==-p)if(x==9)++P;
			if(zdvig<0)P>>-(zdvig); else P<<(zdvig);
			R += P;
			}
		if(isZero())break;
		--zdvig;
		N.zdvigDOWN1();
		}
	*this = R;
	znak = z;
	antiZero(exponenta);
	antiZero(mantisa);
	return *this;
}





char*Digits::cpi="3.141592653589793238462643383279502884197169399375105820974944592307816406286208998628034825342117067982148086513282306647093844609550582231725359408128";
char*Digits::ce ="2.71828182845904523536028747135266249775724709369995957496696762772407663035354759457138217852516642742746639193200305992181741359662904357290033429526";




//--------------------------------------------------------------------------------------------------
void Digits::Pow(Digits&P){
	P.mantisa.clear();
	if(P.isZero()){
		mantisa.clear();
		exponenta.clear();
		exponenta.push_back(1);
		return;
		}
	if(!P.znak){
		clear();
		return;
		}
	// 2^((2+1)*2+1)  **(6+1)
	Digits N, NN, Dva;
	Dva.set(2);
	V_C Row;
	Row.reserve(50);
	char c;
	while(1){
		c = P.exponenta[0];
		if(c==1)if(P.exponenta.size()==1)break;
		c = (c & 1); // (c % 2)
		Row.push_back(c);
		if(c)--P;
		P /= Dva;
		}
	N = *this;
	int i = Row.size();
	while(i>0){
		--i;
		c = Row[i];
		NN = N;
		N *= NN;
		if(c)N *= *this;
		}
	*this = N;
}



//--------------------------------------------------------------------------------------------------
//отброс с числа поcле точки (-n).(n)
void Digits::away(int n){
	if(n>0){
		bool t=n&1;
		n=(n+1)/2;
		while(mantisa.size()>n)mantisa.pop_back();
		if(t)if(mantisa.size())*mantisa.rbegin()=(*mantisa.rbegin()-(*mantisa.rbegin()%10));
		antiZero(mantisa);
		}else{
		mantisa.clear();
		if(!n)return;
		n=-n;
		bool t=n&1;
		n=n/2;
		if(n>=exponenta.size()){
			znak=1;
			exponenta.clear();
			return;
			}
		int i;
		for(i=0;i<n;++i)exponenta[i]=0;
		if(t){
			char x=exponenta[n];
			exponenta[n]=(x-x%10);
			antiZero(exponenta);
			}
		}
}



//округлит знак (-n).(n)
void Digits::round(int n){
	if(!n){
		mantisa.clear();
		return;
		}
	bool ok=0;
	int i,u=n;
	if(u<0){
		while(!ok){
			++u;
			if(!u)break;
			i=-u;
			bool t=i&1;
			i=(i-1)/2;
			if(exponenta.size()<=i)break;
			char x=exponenta[i];
			if(t)x/=10;else x%=10;
			if(x>5)ok=1;
			if(x<5)break;
			}

		}
	if(u>=0){
		while(!ok){
			++u;
			i=u;
			bool t=i&1;
			i=(i-1)/2;
			if(mantisa.size()<=i){
				ok=rand()&1;
				break;
				}
			char x=mantisa[i];
			if(t)x/=10;else x%=10;
			if(x>5)ok=1;
			if(x<5)break;
			}
		}
	if(ok){
		Digits P;
		P.set(1);
		if(n>0)P>>n;else P<<(-n-1);
		*this+=P;
		}
	away(n);
}





int Digits::getSizeOf() const {
	int size = (sizeof(znak) + sizeof(mlimit));
	size += exponenta.size() * sizeof(char);
	size += mantisa.size() * sizeof(char);
	return size;
}




//--------------------------------------------------------------------------------------------------
string Digits::toNumberSystem(char NS) const {
	int max = 'Z' - 'A' + 11;
	if(NS<2 || NS>max)return "";
	if(exponenta.empty())return "0";
	string R;
	Digits A = *this;
	A.mantisa.clear();
	Digits B, DNS;
	DNS.set(NS);
	int v;
	char c, cc[2];
	cc[1] = 0;
	while(!A.isZero()){
		B = A;
		B %= DNS;
		v = (int)B;
		if(v<10)c = '0' + v; else {
			if(v<max){
				v -= 10;
				c = 'A' + v;
				}else c = '#';
			}
		cc[1] = c;
		R = c + R;
		A -= B;
		A /= DNS;
		}
	return R;
}




void Digits::ImportNumberSystem(string&Data, char NS){
	clear();
	int max = 'Z' - 'A' + 11;
	if(NS<2 || NS>max){
		set(-2);
		return;
		}
	const char *s = Data.c_str();
	char c;
	int n, i, size;
	bool isDigit, isAlpha;
	Digits N, DNS;
	DNS.set(NS);
	size = Data.size();
	i = 0;
	for(;i<size;++i){
		c = s[i];
		isDigit = (isdigit(c)!=0);
		isAlpha = (c>='A' && c<='Z');
		if(isDigit||isAlpha){
			*this *= DNS;
			n = (isDigit?(c - '0'):(c - 'A' + 10));
			N.set(n);
			*this += N;
			continue;
			}
		set(-1);
		return;
		}
}





// Digit.cpp	:-|