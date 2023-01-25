//	RegularExpressions.cpp

#include "RegularExpressions.h"

using namespace RegularExpressions;
//-------------------------------------------------------------------------------------------------
// "[]\\/^$.|?*+(){}"
// \Q+-*/\E



/*
\d 	[0-9]
\D 	[^0-9]
\s 	[ \f\n\r\t\v]
\S 	[^ \f\n\r\t\v]
\w 	[A-Za-z0-9_]
\W 	[^A-Za-z0-9_]
*/



RowRE::OneTermRE::OneTermRE(const char*s){
	isString = 1;
	one.ps = new string(s);
}



RowRE::OneTermRE::OneTermRE(RowRE*R){
	isString = 0;
	one.pr = R;
}


RowRE::OneTermRE::~OneTermRE(){
	if(isString){
		if(one.ps)delete one.ps;
		}else{
		if(one.pr)delete one.pr;
		}
}



RowRE::~RowRE(){
	int i,size = Row.size();
	for(i=0;i<size;++i)if(Row[i])delete Row[i];
}



string RowRE::OneTermRE::toString() const {
	if(isString)return *one.ps;
	return "(" + one.pr->toString() + ")";
}



RowRE::OneTermRE* RowRE::OneTermRE::copy() const {
	if(isString){
		return new OneTermRE(one.ps->c_str());
		}
	return new OneTermRE(one.pr->copy());
}



// a(?:bc|b|x)cc
// "a(?:bc|(?:b|ok)|x)cc\\Q+-()*/\\E"

//-------------------------------------------------------------------------------------------------
RowRE* RowRE::parse(const char*s){
	RowRE*R;
	R = parseRow(s);
	if(*s)if(R){
		delete R;
		R = NULL;
		}
	return R;
}




// a-begin s-end.
RowRE::OneTermRE* RowRE::getTEXT(const char*a,const char*s){
	int size = s-a+1;
	char*text = new (char[size]);
	const char*i = a;
	int n = 0;
	for(;i<s;++i,++n)*(text+n)=*i;
	*(text+n)=0;
	OneTermRE *ORE = new OneTermRE(text);
	delete text;
	return ORE;
}




RowRE* RowRE::parseRow(const char*&s){
	RowRE*R = new RowRE();
	const char*a;
	bool OK = 1;
	// 0 \Q \E ( )
	while(*s){
		a = s;
		bool isQ = 0,isE = 0;
		while(*s){
			if(*s=='\\'){
				++s;
				isQ = (*s=='Q');
				isE = (*s=='E');
				if(isQ || isE){
					--s;
					break;
					}
				if(*s=='\\' || (*s=='(' || *s==')'))++s;
				continue;
				}
			if(*s=='(' || *s==')')break;
			++s;
			}
		if(a<s)R->Row.push_back(getTEXT(a,s));
		if(isQ){
			bool isFineOK = 0;
			const char *begin = s;
			while(*s){
				if(*s=='\\'){
					++s;
					isFineOK = (*s=='E');
					if(isFineOK){
						++s;
						break;
						}
					}
				++s;
				}
			if(isFineOK){
				R->Row.push_back(getTEXT(begin,s));
				continue;
				}
			// Error
			OK = 0;
			break;
			}
		if(*s=='('){
			++s;
			RowRE*RRE = parseRow(s);
			if(*s!=')'){
				// Error
				if(RRE)delete RRE;
				RRE = NULL;
				}
			if(!RRE){
				OK = 0;
				break;
				}
			R->Row.push_back(new OneTermRE(RRE));
			++s;
			continue;
			}
		if(*s==')')break;
		if(isE){
			OK = 0;
			break;
			}
		}
	if(R->Row.empty())OK = 0;
	if(!OK){
		delete R;
		R = NULL;
		}
	return R;
}



string RowRE::toString() const {
	string s;
	int i,size = Row.size();
	for(i=0;i<size;++i)s+=Row[i]->toString();
	return s;
}


RowRE* RowRE::copy() const {
	RowRE* R = new RowRE();
	int i,size = Row.size();
	for(i=0;i<size;++i)R->Row.push_back(Row[i]->copy());
	return R;
}





//-------------------------------------------------------------------------------------------------
SimvolFinder::SimvolFinder(){
	isInvert = 0;
	isPsetON = 0;
	S = D = W = 0;
}



SimvolFinder::SimvolFinder(const char c){
	isInvert = 0;
	isPsetON = (c=='.');
	S = D = W = 0;
	if(c=='S' || c=='s')S = (c=='S'?1:2);
	if(c=='D' || c=='d')D = (c=='D'?1:2);
	if(c=='W' || c=='w')W = (c=='W'?1:2);
}



char SimvolFinder::SpecialeSimvolParse(const char*&s){
	if(*s!='\\')return 0;
	char c = *(s+1);
	bool ok = (
		(c=='S' || c=='D' || c=='W') ||
		(c=='s' || c=='d' || c=='w')
		);
	if(!ok)return 0;
	s+=2;
	return c;
}



bool SimvolFinder::isProbelSimvol(const char c){
	if(!c)return 1;
	const char* chars = " \f\n\r\t\v";
	int i;
	for(i=0;chars[i];++i)if(c==chars[i])return 1;
	return 0;
}



char SimvolFinder::StandartSimvolParse(const char*&s){
	if(*s!='\\')return 0;
	char c = *(s+1);
	if(!c)return 0;
	const char* standart = "fnrtv";
	const char* chars = "\f\n\r\t\v";
	int i;
	for(i=0;standart[i];++i)if(c==standart[i]){
		c = chars[i];
		s+=2;
		return c;
		}
	const char*p = s+2;
	if(c=='x'){
		bool isH;
		bool isD;
		bool isUp;
		bool isDown;
		char a = *p;
		if(!a)return 0;
		char A,B;
		isD = (a>='0' && a<='9');
		isUp = (a>='A' && a<='F');
		isDown = (a>='a' && a<='f');
		isH = (isD || isUp || isDown);
		if(!isH)return 0;
		if(isDown)A = (a-'a')+10;
		if(isUp)A = (a-'A')+10;
		if(isD)A = (a-'0');
		++p;
		a = *p;
		if(!a)return 0;
		isD = (a>='0' && a<='9');
		isUp = (a>='A' && a<='F');
		isDown = (a>='a' && a<='f');
		isH = (isD || isUp || isDown);
		if(!isH)return 0;
		if(isDown)B = (a-'a')+10;
		if(isUp)B = (a-'A')+10;
		if(isD)B = (a-'0');
		++p;
		s = p;
		return A*16 + B;
		}
	if(c=='O'){
		bool isD;
		char a = *p;
		if(!a)return 0;
		char A,B,C;
		isD = (a>='0' && a<='7');
		if(!isD)return 0;
		A = (a-'0');
		++p;
		a = *p;
		if(!a)return 0;
		isD = (a>='0' && a<='7');
		if(!isD)return 0;
		B = (a-'0');
		++p;
		a = *p;
		if(!a)return 0;
		isD = (a>='0' && a<='7');
		if(!isD)return 0;
		C = (a-'0');
		++p;
		s = p;
		return A*8*8 + B*8 + C;
		}
	s+=2;
	return c;
}



char SimvolFinder::newStatus(char X,bool isUP){
	if(!X)return (isUP?1:2);
	if(X>=3)return X;
	if(isUP){
		if(X==2)X = 3;
		}else{
		if(X==1)X = 3;
		}
	return X;
}




// [^:.0-5_]
// [^:.0-5--_]
// [^\S\s\D\d\W\w \f\n\r\t\v 1-7 \--\\ A-]
SimvolFinder* SimvolFinder::parse(const char*&s){
	const char*p = s;
	if(*p!='[')return NULL;
	++p;
	SimvolFinder*SF = new SimvolFinder();
	if(*p=='^'){
		SF->isInvert = 1;
		++p;
		}
	char c;
	bool isUP;
	while(*p){
		c = SpecialeSimvolParse(p);
		if(c){
			isUP = (c=='S');
			if(c=='s' || isUP)SF->S = newStatus(SF->S,isUP);
			isUP = (c=='D');
			if(c=='d' || isUP)SF->D = newStatus(SF->D,isUP);
			isUP = (c=='W');
			if(c=='w' || isUP)SF->W = newStatus(SF->W,isUP);
			continue;
			}
		c = StandartSimvolParse(p);
		if(!c){
			c = *p;
			if(c==']')break;
			++p;
			if(!*p)break;
			}
		if(*p!='-'){
			SF->nabor += c;
			continue;
			}
		++p;
		if(*p==']'){
			SF->nabor += c;
			SF->nabor += '-';
			break;
			}
		char a = c;
		c = SpecialeSimvolParse(p);
		if(c){
			c = 0;
			break;
			}
		c = StandartSimvolParse(p);
		if(!c)c = *p;
		if(!c)break;
		SF->OT += a;
		SF->DO += c;
		++p;
		}
	bool ok = (c && *p==']');
	if(!ok){
		delete SF;
		SF = NULL;
		}else{
		++p;
		s = p;
		}
	return SF;
}




string SimvolFinder::toEcraner(const char c){
	const char* standart = "fnrtv-\\";
	const char* chars = "\f\n\r\t\v-\\";
	int i;
	char s[2];
	s[1] = 0;
	for(i=0;chars[i];++i)
		if(c==chars[i]){
			s[0] = standart[i];
			return "\\" + string(s);
			}
	s[0] = c;
	return s;
}




string SimvolFinder::toString() const {
	if(isPsetON)return "[.]";
	string s;
	if(isInvert)s = "^";
	if(S&1)s += "\\S";
	if(S&2)s += "\\s";
	if(D&1)s += "\\D";
	if(D&2)s += "\\d";
	if(W&1)s += "\\W";
	if(W&2)s += "\\w";
	int size,i;
	size = OT.size();
	for(i=0;i<size;++i){
		s += toEcraner(OT[i]);
		s += "-";
		s += toEcraner(DO[i]);
		}
	size = nabor.size();
	for(i=0;i<size;++i)s += toEcraner(nabor[i]);
	s = "[" + s + "]";
	return s;
}





//-------------------------------------------------------------------------------------------------
PovtorFinder::PovtorFinder(){
	A = B = 0;
	PRE.type = 0;
	isLazy = isRevnive = 0;
}

PovtorFinder::~PovtorFinder(){
	PRE.clear();
}



PovtorFinder* PovtorFinder::parse(const char*&s){
	PovtorFinder*PF = new PovtorFinder();
	const char*p = s;
	bool ok = 0;
	if(*p=='?'){
		PF->A = 0;
		PF->B = 1;
		ok = 1;
		}
	if(*p=='+'){
		PF->A = 1;
		PF->B = -1;
		ok = 1;
		}
	if(*p=='*'){
		PF->A = 0;
		PF->B = -1;
		ok = 1;
		}
	if(*p=='{'){
		++p;
		PF->A = parseInt(p);
		if(*p==','){
			++p;
			if(*p=='}')PF->B = -1; else {
				PF->B = parseInt(p);
				}
			}else PF->B = PF->A;
		if(*p=='}')ok = 1;
		}
	if(ok){
		++p;
		const char c = *p;
		if(c=='?'){
			PF->isLazy = 1;
			++p;
			}
		if(c=='+'){
			PF->isRevnive = 1;
			++p;
			}
		s = p;
		}
	if(!ok){
		delete PF;
		PF = NULL;
		}
	return PF;
}



int PovtorFinder::parseInt(const char*&s){
	int x = 0;
	const char*p = s;
	bool isOK = 0;
	while(*p){
		char c = *p;
		bool ok = (c>='0' && c<='9');
		if(!ok)break;
		isOK = 1;
		x = x*10 + (c-'0');
		++p;
		}
	if(isOK)s = p;
	return x;
}



string PovtorFinder::toString() const {
	string s;
	if(A==0 && B==1)s = "?";
	if(A==1 && B<=0)s = "+";
	if(A==0 && B<0)s = "*";
	if(s.empty()){
		char txt[20];
		if(A==B){
			sprintf(txt,"{%d}",A);
			s = txt;
			}else{
			if(A>0){
				sprintf(txt,"%d",A);
				s = txt;
				}
			s += ",";
			if(B>=0){
				sprintf(txt,"%d",B);
				s += txt;
				}
			s = "{" + s + "}";
			}
		}
	if(isLazy)s += "?";
	if(isRevnive)s += "+";
	if(PRE.type==0)s = "NULL:" + s; else s = PRE.toString() + s;
	return s;
}



bool PovtorFinder::isValid() const {
	if(!PRE.type)return 0;
	if(PRE.type==3)return 0;
	if(PRE.type==1){
		int type = PRE.one.BF->type;
		if(!type || type==4 || type==20)return 0;
		if(type==1)if(PRE.one.BF->text.empty())return 0;
		if(type==2)if(PRE.one.BF->List.empty())return 0;
		if(type>=50 && type<=53 || type==100)return 0;
		//....
		}
	return 1;
}




//-------------------------------------------------------------------------------------------------
PointerRE::PointerRE(){
	type = 0;
	one.BF = NULL;
}



void PointerRE::clear(){
	if(type==1)if(one.BF){
		delete one.BF;
		one.BF = NULL;
		}
	if(type==2)if(one.SF){
		delete one.SF;
		one.SF = NULL;
		}
	if(type==3)if(one.PF){
		delete one.PF;
		one.PF = NULL;
		}
}



string PointerRE::toString() const {
	string text;
	if(type==1)if(one.BF)text+=one.BF->toString();
	if(type==2)if(one.SF)text+=one.SF->toString();
	if(type==3)if(one.PF)text+=one.PF->toString();
	return text;
}



//-------------------------------------------------------------------------------------------------
BaseFinder::~BaseFinder(){
	int i = 0, size = List.size();
	for(;i<size;++i)List[i].clear();
}




BaseFinder* BaseFinder::ConvertRowRE(const RowRE*RRE){
	BaseFinder*BF = new BaseFinder();
	BF->type = 2; // List
	bool isError = 0;
	bool isStartParse = 1;
	int i,size = RRE->Row.size();
	int BaseFinderType;
	for(i=0;i<size;++i){
		if(isError)break;
		const RowRE::OneTermRE*P = RRE->Row[i];
		string Buffer;
		if(P->isString){
			BaseFinderType = 1;
			char SFCode = 0;
			char NumberGrup = 0;
			int BFCode = 0;
			SimvolFinder*SFE = NULL;
			PovtorFinder*PFE = NULL;
			BaseFinder*SUBBF = NULL;
			const char* FromText = P->one.ps->c_str();
			bool isFirst = 1;
			for(;*FromText;++FromText){
				bool isNeed = 0, isEnd = 0;
				bool isOR = (*FromText=='|');
				if(!isOR){
					char c = *FromText;
					if(c=='{'){
						const char*p = FromText;
						++FromText;
						bool isStaticName = (*FromText==':');
						if(isStaticName)++FromText;
						string name = BaseFinder::parseName(FromText);
						bool su = (name.size() && *FromText=='}');
						if(su){
							c = 0;
							SUBBF = new BaseFinder();
							SUBBF->type = 101;
							SUBBF->text = name;
							if(isStaticName){
								if(name=="String")SUBBF->type = 110;
								if(name=="Number")SUBBF->type = 111;
								if(name=="Name")SUBBF->type = 112;
								if(SUBBF->type==101)isError = 1;
								}
							}else FromText = p;
						}
					if(c=='^' || (c=='$' && !isStartParse)){
						BFCode = (c=='^'?50:51);
						c = 0;
						}
					if(!isStartParse)
					if(c=='?' || c=='+' || c=='*' || c=='{'){
						c = 0;
						PFE = PovtorFinder::parse(FromText);
						if(!PFE)isError = 1;
						--FromText;
						}
					if(c=='['){
						c = 0;
						SFE = SimvolFinder::parse(FromText);
						if(!SFE)isError = 1;
						--FromText;
						}
					if(c=='.'){
						SFCode = c;
						c = 0;
						}
					const char nc = *(FromText+1);
					if(c=='\\' && nc!='Q'){
						bool isBb = (nc=='B' || nc=='b');
						if(isBb){
							BFCode = (nc=='b'?52:53);
							c = 0;
							++FromText;
							} else {
							bool isNumber = (nc>='0' && nc<='9');
							if(isNumber){
								++FromText;
								c = 0;
								NumberGrup = nc;
								}else{
								c = SimvolFinder::SpecialeSimvolParse(FromText);
								if(c){
									SFCode = c;
									c = 0;
									--FromText;
									}else{
									c = SimvolFinder::StandartSimvolParse(FromText);
									if(!c)isError = 1; else --FromText;
									}
								}
							}
						isFirst = 0;
						isStartParse = 0;
						}
					if(c)Buffer += c;
					if(!*(FromText+1))isNeed = 1;
					}
				if(isFirst){
					isFirst = 0;
					bool ok = (*FromText=='\\' && *(FromText+1)=='Q');
					if(ok){
						Buffer = FromText;
						Buffer = Buffer.substr(2,Buffer.size()-4);
						BaseFinderType = 1;
						isEnd = 1;
						}
					}
				if(isStartParse){
					isStartParse = 0;
					bool ok = (*FromText=='#' || (*FromText=='?' && *(FromText+1)=='#'));
					if(ok){
						Buffer = FromText;
						BF->type = 0;
						return BF;
						}
					ok = (*FromText=='?');
					if(ok){
						++FromText;
						if(!*FromText)BaseFinderType = 15;
						if(*FromText){
							Buffer = "";
							const char *mode = "-ismx";
							bool isGood = 0;
							while(1){
								const char*pos = find(mode,mode+5,*FromText);
								if(pos<mode+5){
									Buffer += *pos;
									++FromText;
									continue;
									}
								isGood = (*FromText==':' || !*FromText);
								if(!*FromText)--FromText;
								break;
								}
							if(Buffer.size()){
								if(isGood){
									BaseFinderType = 20;
									BF->text = Buffer;
									}else isError = 1;
								}
							}
						if(*FromText==':')BaseFinderType = 9;
						if(*FromText=='=')BaseFinderType = 10;
						if(*FromText=='!')BaseFinderType = 11;
						if(*FromText=='>')BaseFinderType = 14;
						if(*FromText=='<'){
							++FromText;
							if(*FromText=='=')BaseFinderType = 12;
							if(*FromText=='!')BaseFinderType = 13;
							}
						BF->type = BaseFinderType;
						BaseFinderType = 1;
						Buffer = "";
						}
					ok = (*FromText=='$') && !ok;
					if(ok){
						++FromText;
						string name = BaseFinder::parseName(FromText);
						bool isOK = (*FromText==':' && name.size());
						if(isOK){
							BF->type = 100; // ($name1:())
							BF->text = name;
							}else isError = 1;
						Buffer = "";
						}
					}
				if(isError)break;
				isNeed = SUBBF || BFCode || PFE || SFE || NumberGrup || SFCode || isEnd || isOR || isNeed;
				if(!isNeed)continue;
				if(PFE){
					int size = Buffer.size();
					if(size){
						const char c = Buffer[size-1];
						Buffer.erase(size-1,1);
						BaseFinder*BF2 = new BaseFinder();
						BF2->type = 1;
						BF2->text = c;
						PointerRE PX;
						PX.type = 1; // BaseFinder
						PX.one.BF = BF2;
						PFE->PRE = PX;
						}else{
						size = BF->List.size();
						if(size){
							PointerRE PX = BF->List[size-1];
							BF->List.pop_back();
							PFE->PRE = PX;
							}
						if(!PFE->isValid()){
							isEnd = 1;
							isError = 1;
							}
						}
					}
				if(!Buffer.empty()){
					BaseFinder*BF2 = new BaseFinder();
					BF2->type = BaseFinderType;
					BF2->text = Buffer;
					Buffer = "";
					PointerRE PX;
					PX.type = 1; // BaseFinder
					PX.one.BF = BF2;
					BF->List.push_back(PX);
					}
				if(PFE){
					PointerRE PX;
					PX.type = 3; // PovtorFinder
					PX.one.PF = PFE;
					BF->List.push_back(PX);
					PFE = NULL;
					}
				if(isOR){
					if(BF->type == 2)BF->type = 3; // 3-OR (A|B|C)
					BaseFinder*BF2 = new BaseFinder();
					BF2->type = 4;
					PointerRE PX;
					PX.type = 1; // BaseFinder
					PX.one.BF = BF2;
					BF->List.push_back(PX);
					if(BF->type == 100)isError = 1;
					}
				if(SFCode){
					SimvolFinder*SF = new SimvolFinder(SFCode);
					PointerRE PX;
					PX.type = 2; // SimvolFinder
					PX.one.SF = SF;
					BF->List.push_back(PX);
					SFCode = 0;
					}
				if(NumberGrup){
					BaseFinder*BF2 = new BaseFinder();
					BF2->type = 30;
					BF2->text = NumberGrup;
					PointerRE PX;
					PX.type = 1; // BaseFinder
					PX.one.BF = BF2;
					BF->List.push_back(PX);
					NumberGrup = 0;
					}
				if(SFE){
					PointerRE PX;
					PX.type = 2; // SimvolFinder
					PX.one.SF = SFE;
					BF->List.push_back(PX);
					SFE = NULL;
					}
				if(BFCode){
					BaseFinder*BF2 = new BaseFinder();
					BF2->type = BFCode;
					PointerRE PX;
					PX.type = 1; // BaseFinder
					PX.one.BF = BF2;
					BF->List.push_back(PX);
					BFCode = 0;
					}
				if(SUBBF){
					PointerRE PX;
					PX.type = 1; // BaseFinder
					PX.one.BF = SUBBF;
					BF->List.push_back(PX);
					SUBBF = NULL;
					}
				if(!*FromText || isEnd)break;
				}
			continue;
			}
		isStartParse = 0;
		BaseFinder*BF2 = ConvertRowRE(P->one.pr);
		if(!BF2){
			isError = 1;
			break;
			}
		if(BF2->List.size()==1 && BF2->type==2){ // (1) \1
			PointerRE*P = &BF2->List[0];
			if(P->type==1)
			if(P->one.BF->type==1)
			if(P->one.BF->text.size()==1){
				const char c = P->one.BF->text[0];
				if(c>='0' && c<='9')P->one.BF->type = 30;
				}
			if(P->one.BF->type==30){
				string num = P->one.BF->text;
				delete BF2;
				BF2 = new BaseFinder();
				BF2->type = 30;
				BF2->text = num;
				}
			}
		bool isTrenary = (BF2->type==15);
		if(BF2->type==3 || isTrenary || (BF2->type>=9 && BF2->type<=14)){
			BaseFinder*BF3 = new BaseFinder();
			BF3->type = BF2->type;
			BaseFinder*BFROW = NULL;
			int i,size = BF2->List.size();
			i=0;
			if(isTrenary){
				PointerRE*P = NULL;
				if(i<size)P = &BF2->List[i];
				int type = 0;
				if(P)if(P->type==1)type = P->one.BF->type;
				if(type==4 || !type){
					isError = 1;
					i = size;
					}
				if(!isError){
					BF3->List.push_back(*P);
					}
				++i;
				}
			for(;i<=size;++i){
				PointerRE*P = NULL;
				if(i<size)P = &BF2->List[i];
				int type = 0;
				if(P)if(P->type==1)type = P->one.BF->type;
				if(type==4 || !P){ // OR Separator (|)
					if(P)P->clear();
					if(!BFROW){
						isError = 1;
						break;
						}
					PointerRE PP;
					PP.type = 1;
					PP.one.BF = BFROW;
					BF3->List.push_back(PP);
					BFROW = NULL;
					}else{
					if(P){
						if(!BFROW){
							BFROW = new BaseFinder();
							BFROW->type = 2; // List
							}
						BFROW->List.push_back(*P);
						}
					}
				}
			if(!isError)if(isTrenary){
				int n = BF3->List.size();
				isError = !(n==3 || n==2);
				}
			if(isError){
				BF3->List.clear();
				delete BF3;
				delete BF2;
				break;
				}
			BF2->List.clear();
			BF3->text = BF2->text;
			delete BF2;
			BF2 = BF3;
			}
		PointerRE PX;
		PX.type = 1; // BaseFinder
		PX.one.BF = BF2;
		BF->List.push_back(PX);
		}
	if(isError){
		if(BF)delete BF;
		BF = NULL;
		}
	return BF;
}





string BaseFinder::toString() const {
	string text;
	if(type==0)text = "#REM";
	if(type==1)text = this->text;
	bool isList = (type==2 || type==100);
	bool isOR = (type>=9 && type<=14) || type==3;
	if(isList || isOR){
		if(type==9){
			text+="?";
			text+=this->text;
			text+=":";
			}
		if(type==10)text+="?=";
		if(type==11)text+="?!";
		if(type==12)text+="?<=";
		if(type==13)text+="?<!";
		if(type==14)text+="?>";
		int i,size = List.size();
		for(i=0;i<size;++i){
			if(i)if(!isList)text+="|";
			text+=List[i].toString();
			}
		}
	if(type==4)text = "|";
	if(type==15){
		text = "?";
		int i = 0;
		text += List[i].toString();
		++i;
		text += List[i].toString();
		text += "|";
		++i;
		text += List[i].toString();
		}
	if(type==20){
		text+="?";
		text+=this->text;
		}
	if(type==30)text += this->text;
	if(type==50)text = "^";
	if(type==51)text = "$";
	if(type==52)text = "\\b";
	if(type==53)text = "\\B";
	if(type==100)text = "$" + this->text + ":" + text;
	if(type==101)text = "{" + this->text + "}";
	if(type==110)text = "{:String}";
	if(type==111)text = "{:Number}";
	if(type==112)text = "{:Name}";
	text = "(" + text + ")";
	return text;
}




string BaseFinder::parseName(const char*&s){
	const char*p = s;
	string name;
	bool isFirst = 1;
	while(*p){
		char c = *p;
		bool isNumber = (c>='0' && c<='9');
		bool isLetter = ((c>='a' && c<='z') || (c>='A' && c<='Z'));
		bool ok = (isLetter || (isNumber && !isFirst));
		if(!ok)break;
		isFirst = 0;
		name += c;
		++p;
		}
	if(!isFirst)s = p;
	return name;
}






//==================================================================================================
FindersMahine::FindersMahine(const char*s){
	Expression = s;
	BF = NULL;
}


FindersMahine::~FindersMahine(){
	if(BF)delete BF;
}



void BaseFinder::RAnalize(FindersMahine*FM) const {
	if(type==100){
		bool povtor = FM->TableSubFinders.find(text) != FM->TableSubFinders.end();
		if(povtor){
			FM->ErrorMessage = "Redefine sub expression: ";
			FM->ErrorMessage += text;
			return;
			}
		FM->TableSubFinders[text] = this;
		}
	if(type==2 || type==3 || type==100 || (type>=9 && type<=15)){
		int i,size = List.size();
		for(i=0;i<size;++i){
			int t = List[i].type;
			if(t==1)List[i].one.BF->RAnalize(FM);
			if(t==3)List[i].one.PF->RAnalize(FM);
			}
		return;
		}
	if(type==101){
		FM->CallNames.push_back(text);
		}
}




void PovtorFinder::RAnalize(FindersMahine*FM) const {
	if(PRE.type!=1)return;
	PRE.one.BF->RAnalize(FM);
}





// 0-DANGER	1-OK	2-IGNORE
char PovtorFinder::isValidRecurse(FindersMahine*FM,V_S&VS) const {
	char R = 0;
	if(PRE.type==1)R = PRE.one.BF->isValidRecurse(FM,VS);
	if(PRE.type==2)R = 1;
	//if(PRE.type==3)R = PRE.one.PF->isValidRecurse(FM,VS); Never be
	bool isNeedConkretic = 0;
	if(isNeedConkretic){
		if(R==2){ // spor
			FM->ErrorMessage = "Invalid (whot this)? in sub expression: ";
			FM->ErrorMessage += *VS.rbegin();
			return 0;
			}
		}else if(R==2 || R==3)return 2;
	if(R==1){
		return (A>0?1:2);
		}
	return 0;
}




// 0-DANGER	1-OK	2-IGNORE	3-Empty
char BaseFinder::isValidRecurse(FindersMahine*FM,V_S&VS) const {
	if(!type)return 3;
	string&MeName = *VS.rbegin(); 
	bool isOR = (type==3);
	bool isSubExpression = (type==100);
	if(isSubExpression && MeName!=text)return 3;
	if(isSubExpression || type==2 || isOR || (type>=9 && type<=14)){
		int i,size = List.size();
		if(!size){
			FM->ErrorMessage = "Empty in sub expression: ";
			FM->ErrorMessage += text;
			return 0;
			}
		bool isOneOK = 0;
		for(i=0;i<size;++i){
			const PointerRE&PRE = List[i];
			char R = 0;
			if(PRE.type==1)R = PRE.one.BF->isValidRecurse(FM,VS);
			if(PRE.type==2)R = 1;
			if(PRE.type==3)R = PRE.one.PF->isValidRecurse(FM,VS);
			if(!R)return 0;
			if(R==1){
				if(!isOR)return 1;
				isOneOK = 1;
				}
			if(isOR && R==2)return 2;
			}
		return (isOneOK?1:2);
		}
	if(type>=110 && type<=112)return 1;
	if(type==101){
		bool isOK = (find(VS.begin(),VS.end(),text)==VS.end());
		if(!isOK){
			FM->ErrorMessage = "Cucle recursion in sub expression: ";
			FM->ErrorMessage += text;
			return 0;
			}
		const BaseFinder*BF = FM->TableSubFinders[text];
		VS.push_back(text);
		char c = BF->isValidRecurse(FM,VS);
		VS.pop_back();
		return c;
		}
	if(type==1){
		return (text.empty()?2:1);
		}
	if(type>=50 && type<=53 || type==20 || !type)return 2; // 2-IGNORE
	if(type==30)return 1;
	if(type==4){ // Never be
		FM->ErrorMessage = "Pronik OR (|) in sub expression: ";
		FM->ErrorMessage += text;
		return 0;
		}
	if(type==15){
		const PointerRE&PRE = List[0];
		char R = 0;
		if(PRE.type==1)R = PRE.one.BF->isValidRecurse(FM,VS);
		if(PRE.type==2)R = 1;
		if(PRE.type==3)R = PRE.one.PF->isValidRecurse(FM,VS);
		if(R==2){
			FM->ErrorMessage = "Invalid condition in sub expression: ";
			FM->ErrorMessage += text;
			return 0;
			}
		return R;
		}
	return 0;
}




bool FindersMahine::Build(){
	RowRE*R = RowRE::parse(Expression);
	if(!R){
		ErrorMessage = "Invalid skobki () or \\Q \\E";
		return 0;
		}
	BF = BaseFinder::ConvertRowRE(R);
	delete R;
	if(!BF){
		ErrorMessage = "Invalid Expression";
		return 0;
		}
	BF->RAnalize(this);
	if(!ErrorMessage.empty()){
		delete BF;
		BF = NULL;
		return 0;
		}
	int i,size = CallNames.size();
	for(i=0;i<size;++i){
		bool isE = TableSubFinders.find(CallNames[i])==TableSubFinders.end();
		if(isE)break;
		}
	if(i<size){
		ErrorMessage = "Undefined sub expression: ";
		ErrorMessage += CallNames[i];
		delete BF;
		BF = NULL;
		return 0;
		}
	bool testOK = 1;
	M_SBF::iterator it = TableSubFinders.begin();
	for(;it!=TableSubFinders.end();++it){
		string name = it->first;
		const BaseFinder*SUB = it->second;
		V_S VS;
		VS.push_back(name);
		char c = SUB->isValidRecurse(this,VS);
		if(!c){
			testOK = 0;
			if(ErrorMessage.empty())ErrorMessage = "Unknoun error in BaseFinder::isValidRecurse()";
			break;
			}
		if(c==2){
			testOK = 0;
			ErrorMessage = "Bred in: ";
			ErrorMessage += name;
			break;
			}
		}
	if(!testOK){
		delete BF;
		BF = NULL;
		}
	return testOK;
}




string FindersMahine::toString() const {
	string s = "Expression:\n";
	s += Expression;
	s += "\n\n";
	if(BF){
		s += "Sub Expressions:\n";
		M_SBF::const_iterator it = TableSubFinders.begin();
		for(;it!=TableSubFinders.end();++it){
			string name = it->first;
			const BaseFinder*SUB = it->second;
			s += name + ": ";
			s += SUB->toString();
			s += "\n";
			}
		}
	s += "\n";
	if(!ErrorMessage.empty()){
		s += "ErrrorMessage: ";
		s += ErrorMessage;
		s += "\n";
		}
	if(!BF)s += "Build: None.";
	if(BF){
		s += "Build: OK.";
		s += "\n";
		s += BF->toString();
		}
	s += "\n";

	return s;
}





//-------------------------------------------------------------------------------------------------
CTask::~CTask(){
	int i;
	if(SubTasks){
		for(i=0;i<size;++i)if(SubTasks[i])delete SubTasks[i];
		delete[] SubTasks;
		}
	int size = PovtorsList.size();
	for(i=0;i<size;++i)if(PovtorsList[i])delete PovtorsList[i];
}



string CTask::toString() const {
	if(!type || type==5 || type==6 || type==8)return "";
	if(type==1){
		char t[2];
		t[0] = c;
		t[1] = 0;
		return t;
		}
	if(type==2){
		string s;
		int i,size = PovtorsList.size();
		for(i=0;i<size;++i){
			s += PovtorsList[i]->toString();
			}
		return s;
		}
	if(type==3)return text;
	if(type==4 || type==5){
		if(!SubTasks)return "";
		string s;
		int i;
		for(i=0;i<size;++i){
			CTask*CT = SubTasks[i];
			if(!CT)continue;
			s += CT->toString();
			}
		return s;
		}
	if(type==7){
		if(!SubTasks)return "";
		CTask*CT = SubTasks[1];
		if(!CT)return "";
		return CT->toString();
		}
	if(type==9){
		if(!SubTasks)return "";
		CTask*CT = *SubTasks;
		if(!CT)return "";
		return CT->toString();
		}
	return "[??]";
}



RowRE* CTask::toExport() const {
	RowRE* RRE = NULL;
	if(!type || type==5 || type==6 || type==8)return RRE;
	if(type==4 || type==5){
		if(!SubTasks)return RRE;
		RRE = new RowRE();
		int i;
		for(i=0;i<size;++i){
			CTask*CT = SubTasks[i];
			if(!CT)continue;
			if(CT->type==1){
				char t[2];
				t[0] = CT->c;
				t[1] = 0;
				RRE->Row.push_back(new RowRE::OneTermRE(t));
				continue;
				}
			if(CT->type==3){
				RRE->Row.push_back(new RowRE::OneTermRE(CT->text.c_str()));
				continue;
				}
			RowRE* R = CT->toExport();
			if(!R)continue;
			RRE->Row.push_back(new RowRE::OneTermRE(R));
			}
		}
	if(type==2){
		RRE = new RowRE();
		int i,size = PovtorsList.size();
		string buf;
		for(i=0;i<size;++i){
			CTask*CT = PovtorsList[i];
			if(!CT)continue;
			if(CT->type==1){
				buf += CT->c;
				continue;
				}
			if(CT->type==3){
				if(buf.size()){
					RRE->Row.push_back(new RowRE::OneTermRE(buf.c_str()));
					buf = "";
					}
				RRE->Row.push_back(new RowRE::OneTermRE(CT->text.c_str()));
				continue;
				}
			RowRE* R = CT->toExport();
			if(!R)continue;
			if(buf.size()){
				RRE->Row.push_back(new RowRE::OneTermRE(buf.c_str()));
				buf = "";
				}
			RRE->Row.push_back(new RowRE::OneTermRE(R));
			}
		if(buf.size()){
			RRE->Row.push_back(new RowRE::OneTermRE(buf.c_str()));
			buf = "";
			}
		}
	if(type==1){
		RRE = new RowRE();
		char t[2];
		t[0] = c;
		t[1] = 0;
		RRE->Row.push_back(new RowRE::OneTermRE(t));
		}
	if(type==3){
		RRE = new RowRE();
		RRE->Row.push_back(new RowRE::OneTermRE(text.c_str()));
		}
	if(type==7){
		if(!SubTasks)return RRE;
		CTask*CT = SubTasks[1];
		if(!CT)return RRE;
		RRE = CT->toExport();
		}
	if(type==9){
		if(!SubTasks)return RRE;
		CTask*CT = *SubTasks;
		if(!CT)return RRE;
		RowRE* R = CT->toExport();
		if(!R)return RRE;
		string metext("SubExpression:");
		metext += text;
		RRE = new RowRE();
		RRE->Row.push_back(new RowRE::OneTermRE(metext.c_str()));
		RRE->Row.push_back(new RowRE::OneTermRE(R));
		}
	return RRE;
}




//-------------------------------------------------------------------------------------------------
RowRE* FindersMahine::RunParser(const char*data){
	if(!BF)return NULL;
	Data = data;
	const char*p = data;
	V_pCT Row;
	CTask* task = NULL;
	while(*p){
		task = new CTask();
		task->s = p;
		task->SubTasks = NULL;
		task->iterator = 0;
		task->FirstUPRow = NULL;
		task->ISMX = 2; // i0 s1 m0 x0
		bool isOK = BF->Scaner(this,task);
		bool toProgress = 1;
		if(isOK)
		if(p!=task->end){
			p = task->end;
			if(task->type!=6){
				Row.push_back(task);
				task = NULL;
				}
			toProgress = 0;
			}
		if(toProgress)++p;
		if(task)delete task;
		}
	if(Row.empty())return NULL;
	RowRE* Result = new RowRE();
	const char*pos = data;
	int i,size = Row.size();
	for(i=0;i<size;++i){
		task = Row[i];
		bool needText = (task->s>pos);
		if(needText){
			int i,size = task->s - pos;
			char* text = new char[size+1];
			memset(text,0,size+1);
			for(i=0;i<size;++i)text[i] = *(pos+i);
			RowRE* R = new RowRE();
			R->Row.push_back(new RowRE::OneTermRE("Text"));
			R->Row.push_back(new RowRE::OneTermRE(text));
			delete[] text;
			Result->Row.push_back(new RowRE::OneTermRE(R));
			pos = task->s;
			}
		if(pos==task->s){
			pos = task->end;
			RowRE* RRE = task->toExport();
			if(RRE){
				RowRE* R = new RowRE();
				R->Row.push_back(new RowRE::OneTermRE("Expression"));
				R->Row.push_back(new RowRE::OneTermRE(RRE));
				Result->Row.push_back(new RowRE::OneTermRE(R));
				}
			}
		}
	if(pos<p){
		int i,size = p - pos;
		char* text = new char[size+1];
		memset(text,0,size+1);
		for(i=0;i<size;++i)text[i] = *(pos+i);
		RowRE* R = new RowRE();
		R->Row.push_back(new RowRE::OneTermRE("Text"));
		R->Row.push_back(new RowRE::OneTermRE(text));
		delete[] text;
		Result->Row.push_back(new RowRE::OneTermRE(R));
		}
	for(i=0;i<size;++i)delete Row[i];
	return Result;
}





bool BaseFinder::Scaner(FindersMahine*FM,CTask*task) const {
	//if(!*task->s)return 0; // Dont use.
	if(!type || (type==100 && task->text!="NeedMe") || type==4){
		if(task->iterator)return 0;
		task->end = task->s;
		task->iterator = 1;
		task->type = 0;
		return 1;
		}
	if(type==1){
		if(task->iterator)return 0;
		task->iterator = 1;
		task->type = 3;
		const char* FText = text.c_str();
		const char* P =	task->s;
		int i;
		bool I = (task->ISMX&1);
		bool X = ((task->ISMX&8) != 0);
		if(!I && !X){
			for(i=0;FText[i];++i)if(FText[i]!=*(P+i))return 0;
			task->end = P+i;
			task->text = text;
			return 1;
			}
		string buf;
		int pp = 0;
		for(i=0;FText[i];++i){
			if(X)if(SimvolFinder::isProbelSimvol(FText[i]))continue;
			char c = *(P+pp);
			++pp;
			bool ok = (c==FText[i]);
			if(I){
				int a,b;
				a = b = -1;
				if(c>='A' && c<='Z')a = c-'A';
				if(c>='a' && c<='z')a = c-'a';
				char p = FText[i];
				if(p>='A' && p<='Z')b = p-'A';
				if(p>='a' && p<='z')b = p-'a';
				if(a>=0 && b>=0)ok = (a==b);
				}
			if(!ok)return 0;
			buf += c;
			}
		task->end = P+pp;
		task->text = buf;
		return 1;
		}
	if(type==2 || type==100){
		int i,size = List.size();
		task->size = size;
		char MeISMX = task->ISMX;
		const char* P =	task->s;
		if(!task->SubTasks){
			task->SubTasks = new (CTask*[size]);
			for(i=0;i<size;++i)task->SubTasks[i] = NULL;
			}
		i=0;
		if(task->iterator){
			i = size-1;
			}
		bool isDefect = 0;
		while(i<size){
			task->c = i;
			CTask*&MeTask = task->SubTasks[i];
			if(!MeTask){
				MeTask = new CTask();
				MeTask->s = P;
				MeTask->SubTasks = NULL;
				MeTask->iterator = 0;
				MeTask->FirstUPRow = task;
				MeTask->ISMX = MeISMX;
				}
			const PointerRE&PRE = List[i];
			bool ok = 0;
			if(PRE.type==1)ok = PRE.one.BF->Scaner(FM,MeTask);
			if(PRE.type==2)ok = PRE.one.SF->Scaner(FM,MeTask);
			if(PRE.type==3)ok = PRE.one.PF->Scaner(FM,MeTask);
			if(MeTask->type==6){
				if(ok)isDefect = 1;
				ok = 1;
				}
			if(ok){
				if(MeTask->type==8)MeISMX = MeTask->ISMX;
				P = MeTask->end;
				++i;
				continue;
				}
			MeISMX = MeTask->ISMX;
			delete MeTask;
			MeTask = NULL;
			--i;
			if(i<0)break;
			}
		task->end = P;
		task->iterator = size;
		task->type = ((type==2 || type==100)?4:5);
		if(i<0){
			task->end = task->s;
			return 0;
			}
		if(isDefect)task->type = 6;
		return 1;
		}
	// 20	-ismx
	bool isSetUP = (type==20);
	bool ist9 = (type==9); // 9	?:
	if(ist9 || isSetUP){
		if(isSetUP){
			if(task->iterator)return 0;
			task->iterator = 1;
			task->type = 8; // set up
			task->end = task->s;
			}
		char i,s,m,x; // 1-ON 2-OFF
		i = s = m = x = 0;
		bool isToON = 1;
		const char* ps = text.c_str();
		for(;*ps;++ps){
			if(*ps=='-')isToON = 0;
			if(*ps=='i')i = (isToON?1:2);
			if(*ps=='s')s = (isToON?1:2);
			if(*ps=='m')m = (isToON?1:2);
			if(*ps=='x')x = (isToON?1:2);
			}
		char ismx = task->ISMX;
		char I,S,M,X;
		I = (ismx&1) != 0;
		S = (ismx&2) != 0;
		M = (ismx&4) != 0;
		X = (ismx&8) != 0;
		if(i)I = (i==1);
		if(s)S = (s==1);
		if(m)M = (m==1);
		if(x)X = (x==1);
		task->ISMX = I|(S<<1)|(M<<2)|(X<<3);
		if(isSetUP)return 1;
		}
	bool isDontVozvrat = (type==14); // ?>
	bool isRavno = (type==10 || type==12); // ?= ?<=
	bool isDontRavno = (type==11 || type==13); // ?! ?<!
	bool isCompare = (isRavno || isDontRavno);
	if(type==3 || isDontVozvrat || isCompare || ist9){ // OR
		int i,size = List.size();
		task->size = 1;
		task->type = ((type==3 || isDontVozvrat || ist9)?4:5);
		if(task->iterator==size)return 0;
		const char* P =	task->s;
		if(!task->SubTasks){
			task->SubTasks = new (CTask*);
			*task->SubTasks = NULL;
			}else if(isDontVozvrat)return 0;
		CTask*&MeTask = *task->SubTasks;
		i = task->iterator;
		for(;i<size;++i){
			if(!MeTask){
				MeTask = new CTask();
				MeTask->s = P;
				MeTask->SubTasks = NULL;
				MeTask->iterator = 0;
				MeTask->FirstUPRow = task->FirstUPRow;
				MeTask->ISMX = task->ISMX;
				}
			const PointerRE&PRE = List[i];
			bool ok = 0;
			if(PRE.type==1)ok = PRE.one.BF->Scaner(FM,MeTask);
			if(PRE.type==2)ok = PRE.one.SF->Scaner(FM,MeTask);
			if(PRE.type==3)ok = PRE.one.PF->Scaner(FM,MeTask);
			if(ok)break;
			delete MeTask;
			MeTask = NULL;
			}
		task->iterator = i;
		bool isF = (i<size);
		if(isF){
			P = MeTask->end;
			}
		task->end = P;
		if(isDontRavno)isF = !isF;
		if(type==13){
			isF = !isF;
			task->type = 6;
			if(isF)task->end = MeTask->end;
			}
		return isF;
		}
	if(type==15){ // 15	?()A|B
		task->type = 7;
		if(task->iterator>=3)return 0;
		const char* P =	task->s;
		if(!task->SubTasks){
			task->SubTasks = new (CTask*[2]);
			*task->SubTasks = NULL;
			*(task->SubTasks+1) = NULL;
			task->size = 2;
			}
		if(!task->iterator){
			const PointerRE&PRE = List[0];
			CTask*&MeTask = *task->SubTasks;
			if(!MeTask){
				MeTask = new CTask();
				MeTask->s = P;
				MeTask->SubTasks = NULL;
				MeTask->iterator = 0;
				MeTask->FirstUPRow = task->FirstUPRow;
				MeTask->text = "NeedBool";
				MeTask->ISMX = task->ISMX;
				}
			bool ok = 0;
			if(PRE.type==1)ok = PRE.one.BF->Scaner(FM,MeTask);
			if(PRE.type==2)ok = PRE.one.SF->Scaner(FM,MeTask);
			if(PRE.type==3)ok = PRE.one.PF->Scaner(FM,MeTask);
			if(MeTask->type==6)ok = !ok;
			task->iterator = (ok?1:2);
			P = task->end = MeTask->end;
			}
		if(List.size()<3 && task->iterator==2){
			task->iterator = 3;
			return 1;
			}
		const PointerRE&PRE = List[task->iterator];
		CTask*&MeTask = task->SubTasks[1];
		if(!MeTask){
			MeTask = new CTask();
			MeTask->s = P;
			MeTask->SubTasks = NULL;
			MeTask->iterator = 0;
			MeTask->FirstUPRow = task->FirstUPRow;
			MeTask->ISMX = task->ISMX;
			}
		bool ok = 0;
		if(PRE.type==1)ok = PRE.one.BF->Scaner(FM,MeTask);
		if(PRE.type==2)ok = PRE.one.SF->Scaner(FM,MeTask);
		if(PRE.type==3)ok = PRE.one.PF->Scaner(FM,MeTask);
		task->end = MeTask->end;
		return ok;
		}
	if(type==30){ // (A)(1)
		if(task->iterator)return 0;
		task->type = 3;
		CTask*FirstUPRow = task->FirstUPRow;
		if(!FirstUPRow)return 0;
		char pos = text[0]-'0' - 1;
		bool isNotOK = ((FirstUPRow->c<=pos) || (pos>=FirstUPRow->size));
		if(isNotOK)return 0;
		CTask*E = FirstUPRow->SubTasks[pos];
		string ES = E->toString();
		task->iterator = 1;
		if(task->text=="NeedBool"){
			task->end = task->s;
			return !ES.empty();
			}
		const char* FText = ES.c_str();
		const char* P =	task->s;
		int i;
		for(i=0;FText[i];++i)if(FText[i]!=*(P+i))return 0;
		task->end = P+i;
		task->text = ES;
		return 1;
		}
	// 50	^
	// 51	$
	bool M = ((task->ISMX&4) != 0);
	if(type==50){
		if(task->iterator)return 0;
		task->iterator = 1;
		task->type = 3;
		task->end = task->s;
		if(M){
			char c = '\n';
			if(FM->Data<task->s)c = *(task->s - 1);
			return (c=='\n' || c=='\r');
			}
		return (FM->Data==task->s);
		}
	if(type==51){
		if(task->iterator)return 0;
		task->iterator = 1;
		task->type = 3;
		task->end = task->s;
		if(M)if(*task->s=='\n')return 1;
		return !(*task->s);
		}
	// 52	\b
	// 53	\B
	bool isPerepad = (type==52);
	if(type==53 || isPerepad){
		if(task->iterator)return 0;
		task->iterator = 1;
		task->type = 0;
		task->end = task->s;
		bool isStartPos = (FM->Data==task->s);
		bool beforeSymbolIsProbel = (isStartPos?1:SimvolFinder::isProbelSimvol(*(task->s-1)));
		bool thisSymbolIsProbel = SimvolFinder::isProbelSimvol(*task->s);
		bool isOdnakovo = (beforeSymbolIsProbel==thisSymbolIsProbel);
		return (isPerepad?!isOdnakovo:isOdnakovo);
		}
	// Sub Expression:
	if(type==101){
		task->type = 9;
		task->text = text;
		const BaseFinder* BF = FM->TableSubFinders[text];
		if(!task->SubTasks){
			task->SubTasks = new (CTask*);
			*task->SubTasks = NULL;
			task->size = 1;
			}
		CTask*&MeTask = *task->SubTasks;
		if(!MeTask){
			MeTask = new CTask();
			MeTask->s = task->s;
			MeTask->SubTasks = NULL;
			MeTask->iterator = 0;
			MeTask->FirstUPRow = NULL; // task->FirstUPRow;
			MeTask->ISMX = task->ISMX;
			MeTask->text = "NeedMe";
			}
		bool ok = BF->Scaner(FM,MeTask);
		task->end = MeTask->end;
		return ok;
		}
	// 110	{:String}
	// 111	{:Number}
	// 112	{:Name}
	if(type==110){ // " A \" B "
		if(task->iterator)return 0;
		task->iterator = 1;
		task->type = 3;
		const char*p = task->s;
		char start = *p;
		if(!(start=='"' || start=='\''))return 0;
		++p;
		while(*p){
			if(*p==start)break;
			if(*p=='\\')++p;
			++p;
			}
		if(*p!=start)return 0;
		++p;
		const char*i = task->s;
		for(;i<p;++i)task->text += *i;
		task->end = p;
		return 1;
		}
	if(type==111){ // -0.6738538E-73
		if(task->iterator)return 0;
		task->iterator = 1;
		task->type = 3;
		const char*p = task->s;
		if(*p=='-')++p;
		bool pset = 0;
		bool oneN = 0;
		while(*p){
			char c = *p;
			bool isD = (c>='0' && c<='9');
			bool isPset = (c=='.');
			if(!(isD||isPset))break;
			if(isD)oneN = 1;
			if(isPset){
				if(pset || !oneN)break;
				pset = 1;
				}
			++p;
			}
		if(!oneN)return 0;
		if(*p=='E' || *p=='e'){
			oneN = 0;
			++p;
			if(*p=='-')++p;
			while(*p){
				char c = *p;
				bool isD = (c>='0' && c<='9');
				if(!isD)break;
				oneN = 1;
				++p;
				}
			}
		if(!oneN)return 0;
		if(*(p-1)=='.')--p;
		const char*i = task->s;
		for(;i<p;++i)task->text += *i;
		task->end = p;
		return 1;
		}
	if(type==112){ // Name55
		if(task->iterator)return 0;
		task->iterator = 1;
		task->type = 3;
		const char*p = task->s;
		task->text = BaseFinder::parseName(p);
		task->end = p;
		return (p>task->s);
		}
	return 0;
}




bool SimvolFinder::Scaner(FindersMahine*FM,CTask*task) const {
	if(task->iterator)return 0;
	const char* P =	task->s;
	const char c = *P;
	if(!c)return 0;
	bool ok = 0;
	if(isPsetON){
		ok = 1;
		bool S = ((task->ISMX&2) != 0);
		if(!S)if(c=='\n' || c=='\r')ok = 0;
		}
	if(!ok){
		int pos = nabor.find(c);
		ok = (pos>=0);
		}
	if(!ok){
		int i,size = OT.size();
		for(i=0;i<size;++i){
			ok = (c>=OT[i] && c<=DO[i]);
			if(ok)break;
			}
		}
	if(!ok){
		bool isS,isD,isW;
		isS = isD = isW = 0;
		if(S){
			isS = SimvolFinder::isProbelSimvol(c);
			if(S==1)isS = !isS;
			if(S==3)isS = 1;
			}
		if(D){
			isD = (c>='0' && c<='9');
			if(D==1)isD = !isD;
			if(D==3)isD = 1;
			}
		if(W){
			isW = (c>='0' && c<='9') || (c>='a' && c<='z') || (c>='A' && c<='Z') || c=='_';
			if(W==1)isW = !isW;
			if(W==3)isW = 1;
			}
		ok = isS || isD || isW;
		}
	if(isInvert)ok = !ok;
	if(ok){
		task->c = c;
		task->end = ++P;
		task->iterator = 1;
		task->type = 1;
		}
	return ok;
}




bool PovtorFinder::Scaner(FindersMahine*FM,CTask*task) const {
	if(task->iterator==1)return 0;
	const char* P =	task->s;
	if(!A && !B){
		task->iterator = 1;
		task->end = P;
		task->type = 0;
		return 1;
		}
	task->type = 2;
	CTask* MeTask = NULL;
	if(task->iterator==2){
		if(isRevnive)return 0;
		int size = task->PovtorsList.size();
		if(!size)return 0;
		MeTask = *task->PovtorsList.rbegin();
		bool ok = 0;
		if(PRE.type==1)ok = PRE.one.BF->Scaner(FM,MeTask);
		if(PRE.type==2)ok = PRE.one.SF->Scaner(FM,MeTask);
		if(ok){
			task->end = MeTask->end;
			return 1;
			}
		task->PovtorsList.pop_back();
		--size;
		delete MeTask;
		MeTask = NULL;
		if(size<A)return 0;
		if(size)MeTask = *task->PovtorsList.rbegin();
		task->end = (MeTask?MeTask->end:task->s);
		return 1;
		}
	int i = task->PovtorsList.size();
	bool oneOK = !isLazy;
	if(i){
		MeTask = *task->PovtorsList.rbegin();
		P = MeTask->end;
		MeTask = NULL;
		}
	bool isStoper = 0;
	if(task->iterator!=3){
		if(isLazy)if(i>=A)oneOK = isStoper = 1;
		}
	if(!isStoper)
	while(*P){
		if(B>=0)if(i>=B)break;
		if(!MeTask){
			MeTask = new CTask();
			MeTask->s = P;
			MeTask->SubTasks = NULL;
			MeTask->iterator = 0;
			MeTask->FirstUPRow = task->FirstUPRow;
			MeTask->ISMX = task->ISMX;
			}
		bool ok = 0;
		if(PRE.type==1)ok = PRE.one.BF->Scaner(FM,MeTask);
		if(PRE.type==2)ok = PRE.one.SF->Scaner(FM,MeTask);
		//if(PRE.type==3)ok = PRE.one.PF->Scaner(FM,MeTask); // Never be
		if(!ok)break;
		if(MeTask->s == MeTask->end)return 0;
		++i;
		P = MeTask->end;
		task->PovtorsList.push_back(MeTask);
		MeTask = NULL;
		oneOK = 1;
		if(isLazy)if(i>=A)break;
		}
	if(MeTask)delete MeTask;
	if(!oneOK)return 0;
	task->end = P;
	task->iterator = (isLazy?3:2);
	return (i>=A);
}




//-------------------------------------------------------------------------------------------------
string FindersMahine::ExamplesExpression(string name){
	if(name=="XML"){
		string Expression;
		Expression += "($arg:{:Name}\\s*=\\s*{:String})";
		Expression += "($XML:<{:Name}\\s*({arg}\\s*)*>\\s*({XML}*|[^<]*)</\\2>\\s*)";
		Expression += "{XML}";
		return Expression;
		}
	if(name=="HTML"){
		string Expression;
		Expression += "($BR:<{:Name}\\s*({arg}\\s*)*/>)";
		Expression += "($arg:{:Name}\\s*=\\s*{:String})";
		Expression += "($HTML:<{:Name}\\s*({arg}\\s*)*>\\s*({BR}|{HTML}|[^<]+)*</\\2>\\s*)";
		Expression += "{HTML}";
		return Expression;
		}
	if(name=="JSON"){
		string Expression;
		Expression += "($array:\\[\\s*{JSON}?(?(3)(\\s*,\\s*{JSON})*)\\s*\\])";
		Expression += "($table:\\{\\s*({:String}\\s*:\\s*{JSON})?(?(3)(\\s*,\\s*{:String}\\s*:\\s*{JSON})*)\\s*\\})";
		Expression += "($JSON:({table}|{array}|{:String}|{:Number}|null|true|false))";
		Expression += "{JSON}";
		return Expression;
		}
	return "HTML XML JSON";
}




//	RegularExpressions.cpp	:-|