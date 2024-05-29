// CPP.cpp
#include "main.h"


const char* CallFunc::m[]={
	"help",
	"trace",
	"tracex",
	"tracec",
	"MessageBeep",
	"system",
	"Sleep",
	"implode",
	"split",
	"size",
	"count",
	"outAllMemory",
	"outAllMTree",
	"rozpad",
	"define",
	"include",
	"uninclude",
	"isset",
	"eval",
	"reserve",
	"getFunction",
	"getstring",
	"getThisFunctionName",
	"getThisFileName",
	"amputace",

	"LoadDLL",
	"UnLoadDLL",
	"RunDLL",
	"getNamesAllDll",

	"echo",
	"rand",
	"typeof",
	"getValueInLaver",
	"setValueInLaver",
	"getThisProcessNumber",
	"getThisLaverNumber",
	"getThisModule",
	"getVariantov",
	"getValuesInLaver",
	"LaverOFF",
	"getListLaver",
	"setPrioritetProcess",
	"getTime",
	"timeFormat",
	"PROGRAM",
	"ControlOFF",
	"ClearPsetOFF",
	"getAllModules",
	"getAllModuleFunctions",
	"runFunction",
	"replaceSumbolHTML",
	"issystemf",
	"insertInclude",
	"eraseInclude",
	"Spirit",
	"Math",
	"sizeof",
	"Randomize",

	"readFile",
	"saveFile",
	"addInFile",
	"existFile",
	"existDir",
	"createDir",
	"deleteFile",
	"deleteDir",
	"FindFiles",
	"renameFile",
	"renameDir",

	"Ajax",
	"JSON",
	"Server",
	0
};


bool CallFunc::isCFunction(string s){
	int i;
	for(i=0;m[i];++i)if(s==m[i])return 1;
	return 0;
}


//--------------------------------------------------------------------------------------------------
void CallFunc::trace(MAIN*M, V_CVARIANT&VCV, char sposob){ // sposob: 2-color 1-ln 0-...
	EnterCriticalSection(&M->CriticalSection1);
	int i;
	if(sposob==2){
		int size = VCV.size();
		if(size){
			int color = 15;
			bool isSetOneColor = true;
			CVARIANT *CharColors = NULL;
			if(size>=2)CharColors = &VCV[1];
			VCV[0].TransformType("string");
			size = VCV[0].DATA.ps->size();
			char*str = new(char[size+1]);
			CharToOem(VCV[0].DATA.ps->c_str(),str);
			if(CharColors){
				isSetOneColor = CharColors->isType("int");
				if(isSetOneColor)color = CharColors->DATA.intVal;
				if(!CharColors->isType("vector"))CharColors = NULL;
				}
			int sizec = 0;
			if(CharColors)sizec = CharColors->DATA.vectorVal->size();
			i = 0;
			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			for(;i<size;++i){
				if(!isSetOneColor){
					if(i<sizec){
						CVARIANT *CC = *(CharColors->DATA.vectorVal->begin()+i);
						if(CC->isType("int"))color = CC->DATA.intVal;
						}else color = 15;
					}
				SetConsoleTextAttribute(hConsole, color);
				cout<<str[i]<<flush;
				}
			delete[] str;
			SetConsoleTextAttribute(hConsole, 7);
			}
		}else{
		for(i=0;i<VCV.size();++i){
			VCV[i].TransformType("string");
			char*str = new(char[VCV[i].DATA.ps->size()+1]);
			CharToOem(VCV[i].DATA.ps->c_str(),str);
			cout<<str;
			delete[] str;
			}
		}
	if(sposob)cout<<endl; else cout<<flush;
	LeaveCriticalSection(&M->CriticalSection1);
}


void CallFunc::Sleeep(V_CVARIANT&VCV,bool isSleep){
	if(!VCV.size())return;
	if(isSleep){
		VCV[0].TransformType("int");
		Sleep(VCV[0].DATA.intVal);
		return;
		}
	VCV[0].TransformType("string");
	system(VCV[0].DATA.ps->c_str());
}


void CallFunc::implode(V_CVARIANT&VCV,CVARIANT*&V){
	if(VCV.size()>1){// ("|",{1,2}) => 1|2
		CVARIANT*A,*B;
		if(VCV[1].isType("string")){A=&VCV[1];B=&VCV[0];}
			else{A=&VCV[0];B=&VCV[1];}
		A->TransformType("string");
		B->TransformType("vector");
		string str;
		int i;
		for(i=0;i<B->DATA.vectorVal->size();++i){
			if(i)str+=*A->DATA.ps;
			(*B->DATA.vectorVal)[i]->TransformType("string");
			str+=*(*B->DATA.vectorVal)[i]->DATA.ps;
			}
		V=new(CVARIANT);
		V->avtoSet("string");
		*V->DATA.ps=str;
		}
}


void CallFunc::split(V_CVARIANT&VCV,CVARIANT*&V){
	if(VCV.size()>1){// ("|","1|2") => {"1","2"}
		CVARIANT*A,*B;
		A=&VCV[0];
		B=&VCV[1];
		A->TransformType("string");
		B->TransformType("string");
		string*a=A->DATA.ps,*b=B->DATA.ps;
		int u;
		CVARIANT S;
		S.avtoSet("string");
		V=new(CVARIANT);
		V->avtoSet("vector");
		while(1){
			u=b->find(a->c_str());
			if(u<0)break;
			*S.DATA.ps=b->substr(0,u);
			CVARIANT*X = new CVARIANT();
			*X = S;
			V->DATA.vectorVal->push_back(X);
			*b=b->substr(u+a->length());
			}
		*S.DATA.ps=*b;
		CVARIANT*X = new CVARIANT();
		*X = S;
		V->DATA.vectorVal->push_back(X);
		}
}


void CallFunc::size(V_CVARIANT&VCV,CVARIANT*&V){
	if(VCV.size()>0){
		CVARIANT*A=&VCV[0];
		int n=0;
		if(A->isType("vector"))n=A->DATA.vectorVal->size();
		if(A->isType("set"))n=A->DATA.setVal->size();
		if(A->isType("map"))n=A->DATA.mapVal->size();
		V=new(CVARIANT);
		V->avtoSet("int");
		V->DATA.intVal=n;
		}
}


void CallFunc::outAllMemory(I*Pset,V_CVARIANT&VCV,CVARIANT*&V,MAIN*M){
	V_CVARIANT CELS;
	FRAGMENT*F=Pset->Fundament;
	while(F){
		V_pCVARIANT::reverse_iterator it=F->Memorys.rbegin();
		for(;it!=F->Memorys.rend();++it)CELS.push_back(**it);
		F=F->Prev;
		}
	string str;
	int i;
	for(i=CELS.size()-1;i>=0;--i){
		CVARIANT&A=CELS[i];
		M_SV*MC=A.DATA.mapVal;
		static const char*line="------------------------------------------------------------\n";
		static const char*line2="============================================================\n";
		if(MC->find("this-f")!=MC->end())str+=line2;else str+=line;
		str+=M->FragmentOneToString(MC);
		}
	V=new(CVARIANT);
	V->avtoSet("string");
	*V->DATA.ps=str;
}



void CallFunc::rozpad(I*Pset,V_CVARIANT&VCV,CVARIANT*&V,MAIN*M){
	if(VCV.size()>0){
		VCV[0].TransformType("vector");
		if(!VCV[0].DATA.vectorVal->size())return;
		Laver*L=M->tableLavers[Pset->Laver];
		int i;
		for(i=0;i<VCV[0].DATA.vectorVal->size();++i){
			I*U=new I(*Pset);
			L->IS.push_back(U);
			CVARIANT*CV=new(CVARIANT);
			*CV=*(*VCV[0].DATA.vectorVal)[i];
			U->sub->Bloki[this]=TRAVERS(CV,1);
			}
		V=new(CVARIANT);
		V->avtoSet("string");
		*V->DATA.ps="kill me";
		FRAGMENT*F=Pset->Fundament;
		Pset->Fundament=new(FRAGMENT);
		F->Next.push_back(Pset->Fundament);
		Pset->Fundament->Prev=F;
		Pset->off=1;
		L->ExtraExit=1;
		L->off=1;
		}
}


void CallFunc::outAllMTree(I*Pset,CVARIANT*&V,MAIN*M){
	V=new(CVARIANT);
	V->avtoSet("string");
	Laver*L=M->tableLavers[Pset->Laver];
	*V->DATA.ps=L->printVariants();
}


void CallFunc::include(V_CVARIANT&VCV,CVARIANT*&V,MAIN*M){
	if(VCV.size()>0){
		VCV[0].TransformType("string");
		/*
		CVARIANT K;
		K.avtoSet("string");
		*K.DATA.ps="WAY";
		K=(*M->GlobalSpace.Map.DATA.mapVal)[K];
		K.TransformType("string");
		string WAY=*K.DATA.ps;
		WAY+="\\"+*VCV[0].DATA.ps;
		*/
		int i=M->IncludeFILE(VCV[0].DATA.ps->c_str());
		V=new(CVARIANT);
		V->avtoSet("int");
		V->DATA.intVal=i;
		}
}


void CallFunc::uninclude(V_CVARIANT&VCV,MAIN*M){
	if(VCV.size()>0){
		VCV[0].TransformType("string");
		/*
		CVARIANT K;
		K.avtoSet("string");
		*K.DATA.ps="WAY";
		K=(*M->GlobalSpace.Map.DATA.mapVal)[K];
		K.TransformType("string");
		string WAY=*K.DATA.ps;
		WAY+="\\"+*VCV[0].DATA.ps;
		*/
		M->unloadOverlay(VCV[0].DATA.ps->c_str());
		}
}


void CallFunc::isset(V_CVARIANT&VCV,CVARIANT*&V){
	bool b=1;
	int i;
	for(i=0;i<VCV.size();++i){
		if(VCV[i].isType("void"))b=0;
		}
	V=new(CVARIANT);
	V->avtoSet("bool");
	V->DATA.boolVal=b;
}


void CallFunc::eval(I*Pset,V_CVARIANT&VCV,CVARIANT*&V,MAIN*M){
	if(VCV.size()>0){
		Laver*L=M->tableLavers[Pset->Laver];
		Algorithm*A=NULL;
		if(VCV[0].isType("string")){
			if(Pset->sub->problemEval.find(this)!=Pset->sub->problemEval.end()){
				A=Pset->sub->problemEval[this];
				TRAVERS*T=A->Border(Pset,M);
				if(T){
					V=T->X;
					Pset->sub->Bloki.erase(A);
					Pset->sub->problemEval.erase(this);
					bool ok=1;
					V_II::iterator it=L->IS.begin();
					for(;it!=L->IS.end();++it)if((*it)->sub->problemEval.find(this)!=(*it)->sub->problemEval.end())ok=0;
					if(ok)delete A;
					return;
					}
				}
			if(!A){
				string error,s=*VCV[0].DATA.ps;
				s="{"+s+";}";
				int er=0;
				Sequence*S=NULL;
					{
					Assemble A(error,M,"");
					char*c=const_cast<char*>(s.c_str());
					S=A.getNabor(c,er);
					}
				if(er){
					if(S)delete S;
					V=new(CVARIANT);
					V->avtoSet("bool");
					V->DATA.boolVal=0;
					return;
					}
				if(S->nabor.size()==1){
					A=(*S->nabor.begin())->copy();
					delete S;
					}else A=S;
				A->initUP(NULL);
				}
			}
		if(VCV[0].isType("program")){
			if(Pset->sub->problemEval.find(this)!=Pset->sub->problemEval.end()){
				A=Pset->sub->problemEval[this];
				TRAVERS*T=A->Border(Pset,M);
				if(T){
					V=T->X;
					Pset->sub->Bloki.erase(A);
					Pset->sub->problemEval.erase(this);
					//delete A;
					return;
					}
				}
			if(!A){
				A=VCV[0].DATA.programVal->copy();
				A->initUP(NULL);
				}
			}
		if(A){
			if(A->isProsto()){
				subI*prevSub=Pset->sub;
				prevSub->problemEval[this]=A;
				int a=A->ZapuskTree(Pset,M,V);
				if(L->ExtraExit)return;
				prevSub->problemEval.erase(this);
				/*
				bool ok=0;
				V_II::iterator it=M->LL->IS.begin();
				for(;it!=M->LL->IS.end();++it)if((*it)->sub->problemEval.find(this)!=(*it)->sub->problemEval.end())ok=0;
				if(ok)delete A;
				*/
				if(!a)if(V)V=V->copy();
				if(a)Pset->sub->Bloki.erase(A);
				//Pset->sub->Bloki[this]=TRAVERS(V,1);
				}else{
				V=new(CVARIANT);
				V->avtoSet("bool");
				V->DATA.boolVal=1;
				Function*F=new Function();
				F->Body=A->copy();
				F->Body->initUP(NULL);
				L->ExtraExit=1;
				Pset->sub=new subI(F);
				Pset->sub->ActivF=F;
				Pset->posicse.push_back(Pset->sub);
				Pset->sub->Return=this;
				F->controlSxema();
				Pset->sub->pos=F->sxema->HeadEnd->down[0];
				Pset->sub->isFreeFunction=1;
				//Pset=NULL;
				L->activI=-1;
				}
			}
		}
}




void CallFunc::getFunction(I*Pset,V_CVARIANT&VCV,CVARIANT*&V,MAIN*M){
	if(VCV.size()>0){
		VCV[0].TransformType("string");
		string s=*VCV[0].DATA.ps;
		Function*F=NULL;
		if(VCV.size()>1){
			VCV[1].TransformType("string");
			string filename=*VCV[1].DATA.ps;
			File*fi=M->findModule(filename);
			if(fi)F=fi->FindFunction(s);
			}
		if(!F)F=M->getFunction(Pset,s);
		if(!F)return;
		F->controlSxema();
		V=new(CVARIANT);
		V->avtoSet("function");
		CFunction P(F);
		P.isOriginal=1;
		(*V->DATA.functionVal)=P;
		return;
		}
	Function*F = Pset->sub->ActivF;
	V=new(CVARIANT);
	V->avtoSet("function");
	CFunction P(F);
	P.isOriginal = 1;
	(*V->DATA.functionVal)=P;
}






void CallFunc::getstring(CVARIANT*&V){
	char string[1000];
	gets(string);
	V=new(CVARIANT);
	V->avtoSet("string");
	*V->DATA.ps=string;
}





void CallFunc::reserve(V_CVARIANT&VCV,CVARIANT*&V){
	int n=0;
	if(VCV.size()>0){
		VCV[0].TransformType("int");
		n=VCV[0].DATA.intVal;
		}
	CVARIANT U;
	V = new(CVARIANT);
	V->avtoSet("vector");
	if(VCV.size()>1)U=VCV[1];
	for(;n>0;--n){
		CVARIANT*W = U.copy();
		V->DATA.vectorVal->push_back(W);
		}
}


void CallFunc::getThisFunctionName(I*Pset,CVARIANT*&V,MAIN*M){
	string name;
	D_subI::reverse_iterator it=Pset->posicse.rbegin();
	for(;it!=Pset->posicse.rend();++it)
		if((*it)->ActivF)
		if(!(*it)->ActivF->name.empty()){name=(*it)->ActivF->name;break;}
	V=new(CVARIANT);
	V->avtoSet("string");
	*V->DATA.ps=name;
}






//--------------------------------------------------------------------------------------------------
void CallFunc::define(I*Pset,V_CVARIANT&VCV,MAIN*M,CVARIANT*&V){
	Laver*L=M->tableLavers[Pset->Laver];
	L->ResultDEF.TransformType("vector");
	if(L->ResultDEF.DATA.vectorVal->size()){
		V=new(CVARIANT);
		*V=L->ResultDEF;
		L->ResultDEF.DATA.vectorVal->clear();
		return;
		}
	int x=0;
	Pset->stop=1;
	L->ExtraExit=1;
	CVARIANT*X=NULL,*List=NULL;
	if(VCV.size()>0){
		bool isSet=VCV[0].isType("set");
		if(isSet||VCV[0].isType("vector")){
			List=&VCV[0];
			if(isSet)List->TransformType("vector");
			}else X=&VCV[0];
		}
	if(VCV.size()>1){
		bool isSet=VCV[1].isType("set");
		if(isSet||VCV[1].isType("vector")){
			List=&VCV[1];
			if(isSet)List->TransformType("vector");
			}else X=&VCV[1];
		}
	if(X){
		X->TransformType("int");
		x=VCV[0].DATA.intVal;
		}
	Pset->numberDefine=x;
	if(List){
		V_pCVARIANT::iterator it=List->DATA.vectorVal->begin();
		for(;it!=List->DATA.vectorVal->end();++it)
			if((*it)->isType("string"))Pset->DefineNames.push_back(*(*it)->DATA.ps);
		}
}



void CallFunc::LoadDLL(V_CVARIANT&VCV,MAIN*M){
	if(VCV.size()>=2){
		VCV[0].TransformType("string");
		VCV[1].TransformType("string");
		string *NameDLL, *path;
		bool isNeed;
		NameDLL = VCV[0].DATA.ps;
		path = VCV[1].DATA.ps;
		isNeed = (M->CDLL.dlls.find(*NameDLL) == M->CDLL.dlls.end());
		if(isNeed)M->CDLL.Load(*NameDLL, *path);
		}
}


void CallFunc::unLoadDLL(V_CVARIANT&VCV,MAIN*M){
	if(VCV.size()>0){
		VCV[0].TransformType("string");
		M->CDLL.unLoad(*VCV[0].DATA.ps);
		}
}


void CallFunc::RunDLL(V_CVARIANT&VCV, CVARIANT*&V, MAIN*M){
	if(VCV.size()>=2){
		string *NameDLL, Data;
		VCV[0].TransformType("string");
		NameDLL = VCV[0].DATA.ps;
		int format = 1;
		bool isToString = false;
		::JSON objJSON;
		objJSON.getONE(&VCV[1], isToString);
		Data = objJSON.toString(format);
		Data = M->CDLL.Run(*NameDLL, Data);
		const char*str = Data.c_str();
		bool isOK = objJSON.parse(str);
		if(isOK){
			V = objJSON.toCVARIANT();
			return;
			}
		V = new(CVARIANT);
		V->avtoSet("string");
		*V->DATA.ps = Data;
		}
}




void CallFunc::getNamesAllDll(CVARIANT*&V,MAIN*M){
	CVARIANT A;
	A = (*M->GlobalSpace.Map.DATA.mapVal)["WAY"];
	string path = *A.DATA.ps+"\\SYS\\DLLS\\*.dll";
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	hFind = FindFirstFile(path.c_str(),&FindFileData);
	if(hFind == INVALID_HANDLE_VALUE)return;
	V=new(CVARIANT);
	V->avtoSet("vector");
	*A.DATA.ps = FindFileData.cFileName;
	V->DATA.vectorVal->push_back(A.copy());
	while (FindNextFile(hFind,&FindFileData)){
		*A.DATA.ps=FindFileData.cFileName;
		V->DATA.vectorVal->push_back(A.copy());
		}
	FindClose(hFind);
}



void CallFunc::echo(V_CVARIANT&VCV,MAIN*M){
	if(VCV.size()>0){
		VCV[0].TransformType("string");
		M->PHTML+=*VCV[0].DATA.ps;
		}
}



//--------------------------------------------------------------------------------------------------
void CallFunc::rand(V_CVARIANT&VCV,CVARIANT*&V){
	V=new(CVARIANT);
	if(VCV.size()>0){
		double x;
		bool isP=0;
		if(VCV[0].isType("float")){x=VCV[0].DATA.fltVal;isP=1;}
		if(VCV[0].isType("double")){x=*VCV[0].DATA.dblVal;isP=1;}
		if(isP && x<=1){
			double y=(double)(::rand()%10000)/10000;
			V->avtoSet("bool");
			V->DATA.boolVal=(y<x);
			return;
			}
		if(VCV[0].isType("vector")){
			V_D fragments;
			double sum=0;
			int i=0;
			for(;i<VCV[0].DATA.vectorVal->size();++i){
				CVARIANT*X=(*VCV[0].DATA.vectorVal)[i];
				X->TransformType("double");
				x = *X->DATA.dblVal;
				if(x<0)x=0;
				fragments.push_back(x);
				sum+=x;
				}
			double y=(sum*(::rand()%10000))/10000;
			sum=0;
			for(i=0;i<fragments.size();++i){
				sum+=fragments[i];
				if(y<sum)break;
				}
			V->avtoSet("int");
			V->DATA.intVal=i;
			return;
			}
		}
	V->avtoSet("int");
	V->DATA.intVal=::rand();
}


void CallFunc::typeof(V_CVARIANT&VCV,CVARIANT*&V){
	if(VCV.size()>0){
		V=new(CVARIANT);
		V->avtoSet("string");
		*V->DATA.ps=V->name[VCV[0].Ntype];
		}
}


void CallFunc::getValueInLaver(I*Pset,V_CVARIANT&VCV,CVARIANT*&V,MAIN*M){
	if(VCV.size()>2){
		VCV[0].TransformType("int");
		VCV[1].TransformType("int");
		VCV[2].TransformType("string");
		int nLaver=VCV[0].DATA.intVal,nProcess=VCV[1].DATA.intVal;
		if(M->tableLavers.find(nLaver)==M->tableLavers.end())return;
		Laver*L=M->tableLavers[nLaver];
		if(nProcess<0 || nProcess>=L->IS.size())return;
		I*me=L->IS[nProcess];//,*Prev;
		//Prev=Pset;
		Pset=me;
		CVARIANT*X=NULL;
		int pos;
		if(Base::findVar(Pset,M,X,*VCV[2].DATA.ps,pos))V=X->copy();
		//Pset=Prev;
		}
}


void CallFunc::setValueInLaver(I*Pset,V_CVARIANT&VCV,MAIN*M){
	if(VCV.size()>3){
		VCV[0].TransformType("int");
		VCV[1].TransformType("int");
		VCV[2].TransformType("string");
		int nLaver=VCV[0].DATA.intVal,nProcess=VCV[1].DATA.intVal;
		if(M->tableLavers.find(nLaver)==M->tableLavers.end())return;
		Laver*L=M->tableLavers[nLaver];
		if(nProcess<0 || nProcess>=L->IS.size())return;
		I*me=L->IS[nProcess];//,*Prev;
		//Prev=Pset;
		Pset=me;
		CVARIANT*X=NULL;
		int pos;
		if(Base::findVar(Pset,M,X,*VCV[2].DATA.ps,pos,1))*X=VCV[3];
		//Pset=Prev;
		}
}


void CallFunc::getThis(I*Pset,CVARIANT*&V,MAIN*M,int Process){
	Laver*L=M->tableLavers[Pset->Laver];
	if(!Process){
		V=new(CVARIANT);
		V->avtoSet("int");
		V->DATA.intVal=Pset->Laver;
		}
	if(Process==1){
		V=new(CVARIANT);
		V->avtoSet("int");
		V->DATA.intVal=L->activI;
		}
	if(Process==2){
		string name;
		D_subI::reverse_iterator it=Pset->posicse.rbegin();
		for(;it!=Pset->posicse.rend();++it)
			if((*it)->ActivF)
			if(!(*it)->ActivF->name.empty()){
				File*f=(*it)->ActivF->file;
				name=f->way+"\\"+f->name;
				break;
				}
		V=new(CVARIANT);
		V->avtoSet("string");
		*V->DATA.ps=name;
		}
	if(Process==3){
		Function*f=Pset->sub->ActivF;
		if(!f)return;
		V=new(CVARIANT);
		V->avtoSet("module");
		V->DATA.moduleVal->id=f->file->id;
		}
}



void CallFunc::amputace(V_CVARIANT&VCV,MAIN*M){
	if(VCV.size()>0){
		VCV[0].TransformType("string");
		File*f=M->findModule(*VCV[0].DATA.ps);
		f->NeedSave=0;
		remove((*VCV[0].DATA.ps+".code").c_str());
		}
}


void CallFunc::getVariantov(V_CVARIANT&VCV,CVARIANT*&V,MAIN*M){
	if(VCV.size()>0){
		VCV[0].TransformType("int");
		int nLaver=VCV[0].DATA.intVal;
		if(M->tableLavers.find(nLaver)==M->tableLavers.end())return;
		Laver*L=M->tableLavers[nLaver];
		V=new(CVARIANT);
		V->avtoSet("int");
		V->DATA.intVal=L->adaptationMap.size();	//L->IS.size();
		}
}


void CallFunc::getValuesInLaver(I*Pset,V_CVARIANT&VCV,CVARIANT*&V,MAIN*M){
	if(VCV.size()>1){
		VCV[0].TransformType("int");
		VCV[1].TransformType("string");
		int nLaver=VCV[0].DATA.intVal,nProcess;
		if(M->tableLavers.find(nLaver)==M->tableLavers.end())return;
		Laver*L=M->tableLavers[nLaver];
		V=new(CVARIANT);
		V->avtoSet("vector");
		CVARIANT XV;
		XV.avtoSet("void");
		for(nProcess=0;nProcess<L->adaptationMap.size();++nProcess){
			I*me=L->IS[L->adaptationMap[nProcess]];//,*Prev;
			if(me->stop){
				CVARIANT*W = new CVARIANT();
				*W = XV;
				V->DATA.vectorVal->push_back(W);
				continue;
				}
			//Prev=Pset;
			Pset=me;
			CVARIANT*X=NULL;
			int pos;
			if(Base::findVar(Pset,M,X,*VCV[1].DATA.ps,pos)){
				CVARIANT*W = new CVARIANT();
				*W = *X;
				V->DATA.vectorVal->push_back(W);
				}else {
				CVARIANT*W = new CVARIANT();
				*W = XV;
				V->DATA.vectorVal->push_back(W);
				}
			//Pset=Prev;
			}
		}
}


void CallFunc::LaverOFF(V_CVARIANT&VCV,MAIN*M){
	if(VCV.size()>1){
		VCV[0].TransformType("int");
		VCV[1].TransformType("int");
		int nLaver=VCV[0].DATA.intVal,nProcess=VCV[1].DATA.intVal;
		if(M->tableLavers.find(nLaver)==M->tableLavers.end())return;
		Laver*L=M->tableLavers[nLaver];
		if(nProcess<0 || nProcess>=L->adaptationMap.size())return;
		I*me=L->IS[L->adaptationMap[nProcess]];
		me->off=1;
		}
}



void CallFunc::getListLaver(V_CVARIANT&VCV,CVARIANT*&V,MAIN*M){
	if(VCV.size()>0){
		VCV[0].TransformType("int");
		int nLaver=VCV[0].DATA.intVal,nProcess;
		if(M->tableLavers.find(nLaver)==M->tableLavers.end())return;
		Laver*L=M->tableLavers[nLaver];
		if(VCV.size()>1){
			VCV[1].TransformType("int");
			nProcess=VCV[1].DATA.intVal;
			}else
		for(nProcess=0;nProcess<L->adaptationMap.size();++nProcess){
			I*me=L->IS[L->adaptationMap[nProcess]];
			if(!me->stop)break;
			}
		FRAGMENT*H=L->IS[L->adaptationMap[nProcess]]->Fundament;
		V=new(CVARIANT);
		V->avtoSet("vector");
		while(H){
			int i;
			for(i=H->Memorys.size()-1;i>=0;--i){
				M_SV*map=H->Memorys[i]->DATA.mapVal;
				M_SV::iterator it=map->begin();
				for(;it!=map->end();++it)if(it->first=="this-f")H=NULL;else {
					CVARIANT *S = new CVARIANT();
					S->avtoSet("string");
					*S->DATA.ps=it->first;
					V->DATA.vectorVal->push_back(S);
					}
				if(!H)break;
				}
			if(!H)break;
			H=H->Prev;
			}
		}
}


void CallFunc::setPrioritetProcess(I*Pset,V_CVARIANT&VCV,MAIN*M){
	if(VCV.size()>0){
		int r=0;
		if(VCV[0].isType("string")){
			string s=*VCV[0].DATA.ps;
			if(s=="first")r=0;
			if(s=="random")r=1;
			if(s=="end")r=2;
			}else{
			VCV[0].TransformType("int");
			r=VCV[0].DATA.intVal;
			if(r<0)r=0;
			if(r>2)r=2;
			}
		M->tableLavers[Pset->Laver]->R=r;
		}
}





void CallFunc::getTime(CVARIANT*&V){
	V = new(CVARIANT);
	V->avtoSet("double");
	*V->DATA.dblVal = UPServac::gettimeofday();
}




void CallFunc::timeFormat(V_CVARIANT&VCV,CVARIANT*&V){
	if(VCV.size()>0){
		VCV[0].TransformType("double");
		double tx = *VCV[0].DATA.dblVal;
		VCV[0].TransformType("int");
		int n=VCV[0].DATA.intVal,p;
		tx-=n;
		string s,ss;
		p=n%60;
		n/=60;
		s=SCANER::toString((int)(tx*1000000));
		s+="ms";
		if(p>0){
			ss=s;
			s=SCANER::toString(p);
			s+="s "+ss;//secund
			}
		if(n>0){
			ss=s;
			p=n%60;
			n/=60;
			s=SCANER::toString(p);
			s+="m "+ss;
			}
		if(n>0){
			p=n%24;
			n/=24;
			ss=s;
			s=SCANER::toString(p);
			s+=":"+ss;
			}
		if(n>0){
			ss=s;
			s=SCANER::toString(n);
			s+="Dey "+ss;
			}
		V=new(CVARIANT);
		V->avtoSet("string");
		*V->DATA.ps=s;
		}
}



void CallFunc::PROGRAM(V_CVARIANT&VCV,CVARIANT*&V,MAIN*M){
	if(VCV.size()>0){
		if(VCV[0].isType("map")){
			string type,condition,body,eelse;
			V_pCVARIANT*VBody=NULL,*VElse=NULL;
			Algorithm*Condition=NULL,*Body=NULL,*Else=NULL;
			M_SV*table=VCV[0].DATA.mapVal;
			CVARIANT *Va;
			string K;
			K="type";
			Va=NULL;
			if(table->find(K)!=table->end())Va=&(*table)[K];
			if(Va)if(Va->isType("string"))type=*Va->DATA.ps;
			Va=NULL;
			K="condition";
			if(table->find(K)!=table->end())Va=&(*table)[K];
			if(Va){
				if(Va->isType("string"))condition=*Va->DATA.ps;
				if(Va->isType("program"))Condition=Va->DATA.programVal;
				}
			Va=NULL;
			K="body";
			if(table->find(K)!=table->end())Va=&(*table)[K];else{
				K="then";
				if(table->find(K)!=table->end())Va=&(*table)[K];
				}
			if(Va){
				if(Va->isType("string"))body=*Va->DATA.ps;
				if(Va->isType("program"))Body=Va->DATA.programVal;
				if(Va->isType("vector"))VBody=Va->DATA.vectorVal;
				}
			Va=NULL;
			K="else";
			if(table->find(K)!=table->end())Va=&(*table)[K];
			if(Va){
				if(Va->isType("string"))eelse=*Va->DATA.ps;
				if(Va->isType("program"))Else=Va->DATA.programVal;
				if(Va->isType("vector"))VElse=Va->DATA.vectorVal;
				}
			int nou=0,nou2=0,nou3=0;
			if(!Condition)Condition=Assemble::getComand(M,condition);
			int ok=0;
			if(!Condition)ok=1;else if(!Condition->isProsto())ok=1;
			if(ok){
				Base*B=new(Base);
				B->text="#";
				B->type=5;
				Condition=B;
				nou=1;
				}
			if(!Body && body.size()){
				Body=Assemble::getComand(M,body);
				nou2=1;
				}
			if(!Body && VBody){
				Sequence*SS=new Sequence();
				int i;
				for(i=0;i<VBody->size();++i){
					CVARIANT*P=(*VBody)[i];
					if(P->isType("program"))SS->nabor.push_back(P->DATA.programVal->copy());
					if(P->isType("string")){
						Algorithm*A=Assemble::getComand(M,*P->DATA.ps);
						if(A)SS->nabor.push_back(A);
						}
					}
				Body=SS;
				nou2=1;
				}
			if(!Else && eelse.size()){
				Else=Assemble::getComand(M,eelse);
				nou3=1;
				}
			if(!Else && VElse){
				Sequence*SS=new Sequence();
				int i;
				for(i=0;i<VBody->size();++i){
					CVARIANT*P=(*VBody)[i];
					if(P->isType("program"))SS->nabor.push_back(P->DATA.programVal->copy());
					if(P->isType("string")){
						Algorithm*A=Assemble::getComand(M,*P->DATA.ps);
						if(A)SS->nabor.push_back(A);
						}
					}
				Else=SS;
				nou3=1;
				}
			if(type=="if"){
				IF*iif=new IF();
				iif->X=nou?Condition:Condition->copy();
				if(Body)iif->A=nou2?Body:Body->copy();
				if(Else)iif->E=nou3?Else:Else->copy();
				iif->initUP(NULL);
				V=new(CVARIANT);
				V->avtoSet("program");
				if(V->DATA.programVal)delete V->DATA.programVal;
				V->DATA.programVal=iif;
				return;
				}
			if(Else)if(nou3)delete Else;
			if(type=="while" || type=="do"){
				WHILE*W=new WHILE();
				W->X=nou?Condition:Condition->copy();
				if(Body)W->A=nou2?Body:Body->copy();
				W->n=(type=="do"?2:1);
				W->initUP(NULL);
				V=new(CVARIANT);
				V->avtoSet("program");
				if(V->DATA.programVal)delete V->DATA.programVal;
				V->DATA.programVal=W;
				return;
				}
			if(Body)if(nou2)delete Body;
			if(Condition)if(nou)delete Condition;
			return;
			}
		VCV[0].TransformType("string");
		if(VCV[0].isType("string")){
			string error,s=*VCV[0].DATA.ps;
			s="{"+s+";}";
			int er=0;
			Algorithm*A;//Assemble::getComand(M,*VCV[0].DATA.ps);
			Sequence*S=NULL;
				{
				Assemble A(error,M,"");
				char*c=const_cast<char*>(s.c_str());
				S=A.getNabor(c,er);
				}
			if(er){
				if(S)delete S;
				V=new(CVARIANT);
				V->avtoSet("bool");
				V->DATA.boolVal=0;
				return;
				}
			if(S->nabor.size()==1){
				A=(*S->nabor.begin())->copy();
				delete S;
				}else A=S;
			A->initUP(NULL);
			V=new(CVARIANT);
			V->avtoSet("program");
			if(V->DATA.programVal)delete V->DATA.programVal;
			V->DATA.programVal=A;
			return;
			}
		}
}



void CallFunc::ControlOFF(I*Pset,MAIN*M,CVARIANT*&V){
	FRAGMENT*FR=Pset->Fundament;
	while(FR){
		if(!FR->Memorys.empty())break;
		FR=FR->Prev;
		}
	if(!FR)return;
	CVARIANT*X=*FR->Memorys.rbegin();
	CVARIANT E;
	string S;
	E.avtoSet("int");
	S="stopoff*";
	V=new(CVARIANT);
	V->avtoSet("bool");
	if(X->DATA.mapVal->find(S)==X->DATA.mapVal->end()){
		Function*f=Pset->sub->ActivF;
		f->sxema->controlTable();
		E.DATA.intVal=Pset->sub->pos->accessNumber;
		(*X->DATA.mapVal)[S]=E;
		V->DATA.boolVal=1;
		return;
		}
	V->DATA.boolVal=0;
}



void CallFunc::ClearPsetOFF(I*Pset,MAIN*M){
	if(Pset->Fundament->Memorys.empty())return;
	CVARIANT*X=*Pset->Fundament->Memorys.rbegin();
	string S;
	S="stopoff*";
	if(X->DATA.mapVal->find(S)==X->DATA.mapVal->end())return;
	X->DATA.mapVal->erase(S);
}


void CallFunc::getAllModules(MAIN*M,CVARIANT*&V){
	V=new(CVARIANT);
	V->avtoSet("vector");
	M_iFile::iterator it=M->Files.begin();
	for(;it!=M->Files.end();++it){
		string s=it->second->way+"\\"+it->second->name;
		CVARIANT *S = new CVARIANT();
		S->avtoSet("string");
		*S->DATA.ps=s;
		V->DATA.vectorVal->push_back(S);
		}
}


void CallFunc::getAllModuleFunctions(V_CVARIANT&VCV,MAIN*M,CVARIANT*&V){
	if(VCV.size()>0){
		VCV[0].TransformType("string");
		File*f=M->findModule(*VCV[0].DATA.ps);
		if(!f)return;
		V=new(CVARIANT);
		V->avtoSet("vector");
		int i;
		for(i=0;i<f->functions.size();++i){
			Function*fun=f->functions[i];
			string full=fun->name;
			CLASS*C=fun->Space;
			while(C){
				full=C->name+"::"+full;
				C=C->Space;
				}
			CVARIANT *S = new CVARIANT();
			S->avtoSet("string");
			*S->DATA.ps=full;
			V->DATA.vectorVal->push_back(S);
			}
		}
}


void CallFunc::runFunction(I*Pset,V_CVARIANT&VCV,MAIN*M,CVARIANT*&V){
	if(VCV.size()>1){
		VCV[0].TransformType("string");//file
		VCV[1].TransformType("string");//function
		File*f=M->findModule(*VCV[0].DATA.ps);
		if(!f)return;
		Function*fun=f->FindFunction(*VCV[1].DATA.ps);
		if(!fun)return;
		Laver*L=M->tableLavers[Pset->Laver];
		L->ExtraExit=1;
		Pset->sub->Bloki[this]=TRAVERS(NULL,0);
		Pset->sub=new subI(fun);
		Pset->sub->ActivF=fun;
		fun->controlSxema();
		Pset->posicse.push_back(Pset->sub);
		Pset->sub->Return=this;
		Pset->sub->pos=fun->sxema->HeadEnd->down[0];
		CVARIANT NF;
		string K;
		NF.avtoSet("string");
		CVARIANT*CEL=new(CVARIANT);
		CEL->avtoSet("map");
		K="this-f";
		*NF.DATA.ps="f:"+fun->name;
		(*CEL->DATA.mapVal)[K]=NF;
		V_pCVARIANT H;
		if(VCV.size()>2){
			VCV[2].TransformType("vector");
			H=*VCV[2].DATA.vectorVal;
			}
		int i;
		for(i=0;i<fun->names.size()&&i<H.size();++i){
			K=fun->names[i];
			(*CEL->DATA.mapVal)[K]=*H[i];
			}
		Pset->Fundament->Memorys.push_back(CEL);
		}
}


void CallFunc::replaceSumbolHTML(V_CVARIANT&VCV,CVARIANT*&V){
	if(VCV.size()>0){
		VCV[0].TransformType("string");
		V=new(CVARIANT);
		V->avtoSet("string");
		*V->DATA.ps=SCANER::replaceSpecSumbolHTML(VCV[0].DATA.ps->c_str());
		}
}


void CallFunc::issystemf(V_CVARIANT&VCV,CVARIANT*&V){
	V=new(CVARIANT);
	if(VCV.size()>0){
		VCV[0].TransformType("string");
		bool x=0;
		int i;
		for(i=0;m[i];++i)if(*VCV[0].DATA.ps==m[i]){x=1;break;}
		V->avtoSet("bool");
		V->DATA.boolVal=x;
		return;
		}
	V->avtoSet("vector");
	int i;
	for(i=0;m[i];++i){
		CVARIANT *S = new CVARIANT();
		S->avtoSet("string");
		*S->DATA.ps=m[i];
		V->DATA.vectorVal->push_back(S);
		}
}


void CallFunc::EIInclude(V_CVARIANT&VCV,MAIN*M,bool inc){
	if(VCV.size()>1){
		VCV[0].TransformType("string");
		VCV[1].TransformType("string");
		string str=*VCV[1].DATA.ps;
		File*f=M->findModule(*VCV[0].DATA.ps);
		if(!f)return;
		V_S::iterator it=find(f->modules.begin(),f->modules.end(),str);
		bool b=it==f->modules.end();
		if(inc){
			if(b)f->modules.push_back(str);
			}else if(!b)f->modules.erase(it);
		}
}


void CallFunc::Spirit(I*Pset,V_CVARIANT&VCV,MAIN*M){
	bool kill=1;
	Laver*L=M->tableLavers[Pset->Laver];
	if(VCV.size()>0){
		kill=0;
		if(VCV[0].isType("string")){
			Function*f=M->getFunction(Pset,*VCV[0].DATA.ps);
			CVARIANT NF,*U;
			string K;
			NF.avtoSet("string");
			U=new(CVARIANT);
			U->avtoSet("bool");
			U->DATA.boolVal=1;
			if(f){
				L->ExtraExit=1;
				TRAVERS*T=&Pset->sub->Bloki[L->SpiritA];
				if(T->free)delete T->X;
				Pset->sub->Bloki[L->SpiritA]=TRAVERS(U,1);
				Pset->sub=new subI(f);
				Pset->sub->ActivF=f;
				f->controlSxema();
				Pset->posicse.push_back(Pset->sub);
				Pset->sub->Return=L->SpiritA;
				Pset->sub->pos=f->sxema->HeadEnd->down[0];
				CVARIANT*CEL;
				CEL=new(CVARIANT);
				CEL->avtoSet("map");
				K="this-f";
				*NF.DATA.ps="f:"+f->getPsetWay()+*VCV[0].DATA.ps;
				(*CEL->DATA.mapVal)[K]=NF;
				int i;
				for(i=0;i<f->names.size() && i+1<VCV.size();++i){
					K=f->names[i];
					(*CEL->DATA.mapVal)[K]=VCV[i+1];
					}
				Pset->Fundament->Memorys.push_back(CEL);
				}
			}else{
			VCV[0].TransformType("bool");
			if(VCV[0].DATA.boolVal)kill=1;
			}
		}
	if(kill){
		if(Pset->sub->Bloki.find(L->SpiritA)!=Pset->sub->Bloki.end()){
			TRAVERS*T=&Pset->sub->Bloki[L->SpiritA];
			if(T->free)delete T->X;
			Pset->sub->Bloki.erase(L->SpiritA);
			}
		Bloc*B=Pset->sub->pos;
		if(B->A)delete B->A;
		B->A=NULL;
		Function*f=Pset->sub->ActivF;
		if(f->file)f->file->NeedSave=1;
		L->ExtraExit=1;
		}
}



void CallFunc::Match(V_CVARIANT&VCV,MAIN*M,CVARIANT*&V){
	if(VCV.empty())return;
	VCV[0].TransformType("string");
	string comand=*VCV[0].DATA.ps;
	V=new(CVARIANT);
	V->avtoSet("double");
	*V->DATA.dblVal = 0;
	if(VCV.size()==1){
		if(comand=="pi" || comand=="PI")*V->DATA.dblVal = 3.1415926535897932384626433;
		if(comand=="e" || comand=="E")*V->DATA.dblVal = 2.7182818284590452353602874;
		return;
		}
	double a;
	VCV[1].TransformType("double");
	a = *VCV[1].DATA.dblVal;
	if(VCV.size()>=3){
		double b;
		VCV[2].TransformType("double");
		b = *VCV[2].DATA.dblVal;
		if(comand=="pow" || comand=="Pow")*V->DATA.dblVal = pow(a,b);
		if(comand=="fmod" || comand=="mod")*V->DATA.dblVal = fmod(a,b);
		return;
		}
	if(comand=="abs" || comand=="fabs")*V->DATA.dblVal = fabs(a);
	if(comand=="cos")*V->DATA.dblVal = cos(a);
	if(comand=="sin")*V->DATA.dblVal = sin(a);
	if(comand=="tan")*V->DATA.dblVal = tan(a);
	if(comand=="acos")*V->DATA.dblVal = acos(a);
	if(comand=="asin")*V->DATA.dblVal = asin(a);
	if(comand=="atan")*V->DATA.dblVal = atan(a);
	if(comand=="sqrt")*V->DATA.dblVal = sqrt(a);
	if(comand=="ceil")*V->DATA.dblVal = ceil(a);
	if(comand=="exp")*V->DATA.dblVal = exp(a);
	if(comand=="floor")*V->DATA.dblVal = floor(a);
	if(comand=="ln")*V->DATA.dblVal = log(a);
	if(comand=="log10")*V->DATA.dblVal = log10(a);
}





void CallFunc::Help(V_CVARIANT&VCV,CVARIANT*&V){
	V=new(CVARIANT);
	if(VCV.size()<1){
		V->avtoSet("vector");
		CVARIANT *X;
		X = new CVARIANT();
		X->avtoSet("string");
		*X->DATA.ps="author";
		V->DATA.vectorVal->push_back(X);
		X = new CVARIANT();
		X->avtoSet("string");
		*X->DATA.ps="ver";
		V->DATA.vectorVal->push_back(X);
		X = new CVARIANT();
		X->avtoSet("string");
		*X->DATA.ps="functions";
		V->DATA.vectorVal->push_back(X);
		return;
		}
	VCV[0].TransformType("string");
	string s=*VCV[0].DATA.ps;
	if(s=="author"){
		V->avtoSet("string");
		*V->DATA.ps="Довгополый Вадим";
		}
	if(s=="ver"){
		V->avtoSet("string");
		*V->DATA.ps="2019";
		}
	if(s=="functions"){
		V->avtoSet("vector");
		CVARIANT *X;
		int i;
		for(i=0;m[i];++i){
			X = new CVARIANT();
			X->avtoSet("string");
			*X->DATA.ps=m[i];
			V->DATA.vectorVal->push_back(X);
			}
		}
}




void CallFunc::FileAccess(string&name,V_CVARIANT&VCV,CVARIANT*&V){
	if(name=="existFile")if(VCV.size()>=1){
		VCV[0].TransformType("string");
		const char*fileName = VCV[0].DATA.ps->c_str();
		FILE *file = fopen(fileName, "r");
		bool ok = (file!=NULL);
		if(ok)fclose(file);
		V=new(CVARIANT);
		V->avtoSet("bool");
		V->DATA.boolVal = ok;
		return;
		}
	if(name=="existDir")if(VCV.size()>=1){
		VCV[0].TransformType("string");
		const char*fileName = VCV[0].DATA.ps->c_str();
		DWORD attribs = ::GetFileAttributesA(fileName);
		DWORD INVALID_FILE_ATTRIBUTES = ((DWORD)-1);
		bool ok = true;
		if(attribs == INVALID_FILE_ATTRIBUTES)ok = false;
		if(ok)ok=((attribs & FILE_ATTRIBUTE_DIRECTORY)!=0);
		V=new(CVARIANT);
		V->avtoSet("bool");
		V->DATA.boolVal = ok;
		return;
		}
	if(name=="readFile")if(VCV.size()>=1){
		VCV[0].TransformType("string");
		const char*fileName = VCV[0].DATA.ps->c_str();
		::ifstream t;
		int length;
		t.open(fileName);			// open input file
		t.seekg(0, ios::end);    // go to the end
		length = t.tellg();           // report location (this is the length)
		t.seekg(0, ios::beg);    // go back to the beginning
		char*buffer = new char[length+1];    // allocate memory for a buffer of appropriate dimension
		::memset(buffer,0,length+1);
		t.read(buffer, length);       // read the whole file into the buffer
		t.close();                    // close file handle
		V=new(CVARIANT);
		V->avtoSet("string");
		*V->DATA.ps = buffer;
		delete[] buffer;
		return;
		}
	if(name=="saveFile")if(VCV.size()>=2){
		VCV[0].TransformType("string");
		VCV[1].TransformType("string");
		const char*fileName = VCV[0].DATA.ps->c_str();
		::ofstream out(fileName);
		out << VCV[1].DATA.ps->c_str();
		out.close();
		return;
		}
	if(name=="createDir")if(VCV.size()>=1){
		VCV[0].TransformType("string");
		const char *DirName = VCV[0].DATA.ps->c_str();
		mkdir(DirName);
		return;
		}
	if(name=="addInFile")if(VCV.size()>=2){
		VCV[0].TransformType("string");
		VCV[1].TransformType("string");
		const char*fileName = VCV[0].DATA.ps->c_str();
		::ofstream out(fileName, ios::app);
		out << VCV[1].DATA.ps->c_str();
		out.close();
		return;
		}
	if(name=="FindFiles")if(VCV.size()>=1){
		VCV[0].TransformType("string");
		const char*fileName = VCV[0].DATA.ps->c_str();
		int attr = 3, p;
		if(VCV.size()>=2){
			if(VCV[1].isType("int")){
				p = VCV[1].DATA.intVal;
				if(p==1 || p==2)attr = p;
				}else{
				VCV[1].TransformType("string");
				const string *s = VCV[1].DATA.ps;
				if(*s=="File" || *s=="file" || *s=="Files" || *s=="files")attr=1;
				if(*s=="Dir" || *s=="dir" || *s=="Dirs" || *s=="dirs")attr=2;
				}
			}
		V = new(CVARIANT);
		V->avtoSet("vector");
		WIN32_FIND_DATA FindFileData;
		HANDLE hf;
		hf = FindFirstFile(fileName,&FindFileData);
		if(hf != INVALID_HANDLE_VALUE){
			do{
				p = (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)?2:1;
				if(attr&p){
					string ps = FindFileData.cFileName;
					if(ps!="." && ps!=".."){
						CVARIANT *X = new CVARIANT();
						X->avtoSet("string");
						*X->DATA.ps = ps;
						V->DATA.vectorVal->push_back(X);
						}
					}
				}while(FindNextFile(hf,&FindFileData) != 0);
			FindClose(hf);
			}
		return;
		}
	if(name=="deleteFile")if(VCV.size()>=1){
		VCV[0].TransformType("string");
		const char*fileName = VCV[0].DATA.ps->c_str();
		remove(fileName);
		return;
		}
	if(name=="deleteDir")if(VCV.size()>=1){
		VCV[0].TransformType("string");
		const char*fileName = VCV[0].DATA.ps->c_str();
		int result;
        result = rmdir(fileName);
		V = new(CVARIANT);
		V->avtoSet("bool");
		V->DATA.boolVal = (result == 0);//ok
		return;
		}
	if(name=="renameFile" || name=="renameDir")if(VCV.size()>=2){
		VCV[0].TransformType("string");
		VCV[1].TransformType("string");
		const char*fileName = VCV[0].DATA.ps->c_str();
		const char*fileName2 = VCV[1].DATA.ps->c_str();
		int result;
		result = rename(fileName,fileName2);
		V = new(CVARIANT);
		V->avtoSet("bool");
		V->DATA.boolVal = (result == 0);//ok
		return;
		}
}




void CallFunc::AJAX(V_CVARIANT&VCV,CVARIANT*&V){
	if(VCV.size()!=1)return;
	if(!VCV[0].isType("map"))return;
	M_SV*table = VCV[0].DATA.mapVal;
	CVARIANT *Value;
	string X;

	X="method";
	string method = "GET";
	if(table->find(X) != table->end()){
		Value = &(*table)[X];
		Value->TransformType("string");
		method = *Value->DATA.ps;
		}

	X="url";
	if(table->find(X) == table->end())return;
	string host_name;
	Value = &(*table)[X];
	Value->TransformType("string");
	host_name = *Value->DATA.ps;

	X="port";
	int port = 80;
	if(table->find(X) != table->end()){
		Value = &(*table)[X];
		Value->TransformType("int");
		port = Value->DATA.intVal;
		}

	string context;
	X="context";
	if(table->find(X) != table->end()){
		Value = &(*table)[X];
		Value->TransformType("string");
		context = *Value->DATA.ps;
		} else {
		X="text";
		if(table->find(X) != table->end()){
			Value = &(*table)[X];
			Value->TransformType("string");
			context = *Value->DATA.ps;
			}
		}

	string headers;
	X="headers";
	if(table->find(X) != table->end()){
		Value = &(*table)[X];
		Value->TransformType("string");
		headers = *Value->DATA.ps;
		}

	string text;
	bool isOK = Ajax(host_name,port,method,headers,context,text);
	V = new CVARIANT();
	if(!isOK){
		//printf("%s", text.c_str());
		V->avtoSet("vector");
		CVARIANT *S;
		S = new CVARIANT();
		S->avtoSet("string");
		*S->DATA.ps = "Error:";
		V->DATA.vectorVal->push_back(S);
		S = new CVARIANT();
		S->avtoSet("string");
		*S->DATA.ps = text;
		V->DATA.vectorVal->push_back(S);
		return;
		}

	V->avtoSet("string");
	*V->DATA.ps = text;
	int pos = text.find("\r\n\r\n");
	if(pos<0)return;
	string body = text.substr(pos+4);
	int posx = text.find("\r\n");
	if(posx<0)return;
	string fstr = text.substr(0,posx);
	string heads = text.substr(posx+2,pos-(posx+2));
	V->avtoSet("vector");
	CVARIANT *S;
	S = new CVARIANT();
	S->avtoSet("string");
	*S->DATA.ps = fstr;
	V->DATA.vectorVal->push_back(S);
	S = new CVARIANT();
	S->avtoSet("string");
	*S->DATA.ps = heads;
	V->DATA.vectorVal->push_back(S);
	S = new CVARIANT();
	S->avtoSet("string");
	*S->DATA.ps = body;
	V->DATA.vectorVal->push_back(S);
}



void CallFunc::JSON(V_CVARIANT&VCV,CVARIANT*&V){
	if(VCV.size()<2)return;
	VCV[0].TransformType("string");
	if(*VCV[0].DATA.ps=="parse"){
		VCV[1].TransformType("string");
		string*textJSON = VCV[1].DATA.ps;
		::JSON objJSON;
		const char*str = textJSON->c_str();
		objJSON.parse(str);
		V=objJSON.toCVARIANT();
		}
	if(*VCV[0].DATA.ps=="stringify"){
		int format = 1;
		bool isToString = false;
		if(VCV.size()>=3){
			VCV[2].TransformType("int");
			format = VCV[2].DATA.intVal;
			isToString = (format>1);
			if(format)format = 1;
			}
		::JSON objJSON;
		objJSON.getONE(&VCV[1],isToString);
		V = new CVARIANT;
		V->avtoSet("string");
		*V->DATA.ps = objJSON.toString(format);
		}
}




void CallFunc::Server(MAIN*M,V_CVARIANT&VCV,CVARIANT*&V){
	int sizeVCV = VCV.size();
	if(sizeVCV<1)return;
	VCV[0].TransformType("string");
	string* ps = VCV[0].DATA.ps;
	if(*ps=="Listen")if(sizeVCV>=2){
		int port;
		VCV[1].TransformType("int");
		port = VCV[1].DATA.intVal;
		if(M->p_Servac){
			V = new CVARIANT();
			V->avtoSet("string");
			*V->DATA.ps = "NeedStopServer";
			return;
			}
		M->p_Servac = new UPServac(port);
		M->p_Servac->RunServer();
		V = new CVARIANT();
		V->avtoSet("string");
		*V->DATA.ps = M->p_Servac->getDoclad();
		return;
		}
	if(*ps=="Stop"){
		delete M->p_Servac;
		M->p_Servac = NULL;
		return;
		}
	if(*ps=="getCountWaitUsers"){
		int n = 0;
		if(M->p_Servac)n = M->p_Servac->getCountWaitUsers();
		V = new CVARIANT();
		V->avtoSet("int");
		V->DATA.intVal = n;
		return;
		}
	if(*ps=="WaitUser"){
		string message;
		if(M->p_Servac){
			M->p_Servac->WaitUser();
			message = M->p_Servac->GetBuffer();
			}else message = "NeedStartServer";
		V = new CVARIANT();
		V->avtoSet("string");
		*V->DATA.ps = message;
		return;
		}
	if(*ps=="SendAndClose")if(sizeVCV>=2){
		VCV[1].TransformType("string");
		const char* s = VCV[1].DATA.ps->c_str();
		if(M->p_Servac)M->p_Servac->SendAndClose(s);
		return;
		}
}




//--------------------------------------------------------------------------------------------------
void CallFunc::fc(I*Pset,string&name,V_CVARIANT&VCV,CVARIANT*&V,MAIN*M){
	if(name=="help")Help(VCV,V);
	if(name=="trace" || name=="tracex")trace(M,VCV,name=="trace");
	if(name=="tracec")trace(M,VCV,2);
	if(name=="Sleep" || name=="system")Sleeep(VCV,name=="Sleep");
	if(name=="implode")implode(VCV,V);
	if(name=="split")split(VCV,V);
	if(name=="count"||name=="size")size(VCV,V);
	if(name=="outAllMemory")outAllMemory(Pset,VCV,V,M);
	if(name=="outAllMTree")outAllMTree(Pset,V,M);
	if(name=="rozpad")rozpad(Pset,VCV,V,M);
	if(name=="define")define(Pset,VCV,M,V);
	if(name=="include")include(VCV,V,M);
	if(name=="uninclude")uninclude(VCV,M);
	if(name=="isset")isset(VCV,V);
	if(name=="eval")eval(Pset,VCV,V,M);
	if(name=="getFunction")getFunction(Pset,VCV,V,M);
	if(name=="getstring")getstring(V);
	if(name=="reserve")reserve(VCV,V);
	if(name=="getThisFunctionName")getThisFunctionName(Pset,V,M);

	if(name=="LoadDLL")LoadDLL(VCV,M);
	if(name=="UnLoadDLL")unLoadDLL(VCV,M);
	if(name=="RunDLL")RunDLL(VCV,V,M);
	if(name=="getNamesAllDll")getNamesAllDll(V,M);

	if(name=="echo")echo(VCV,M);
	if(name=="rand")rand(VCV,V);
	if(name=="typeof")typeof(VCV,V);
	if(name=="getValueInLaver")getValueInLaver(Pset,VCV,V,M);
	if(name=="setValueInLaver")setValueInLaver(Pset,VCV,M);
	if(name=="getThisLaverNumber")getThis(Pset,V,M,0);
	if(name=="getThisProcessNumber")getThis(Pset,V,M,1);
	if(name=="getThisFileName")getThis(Pset,V,M,2);
	if(name=="getThisModule")getThis(Pset,V,M,3);
	if(name=="amputace")amputace(VCV,M);
	if(name=="getVariantov")getVariantov(VCV,V,M);
	if(name=="getValuesInLaver")getValuesInLaver(Pset,VCV,V,M);
	if(name=="LaverOFF")LaverOFF(VCV,M);
	if(name=="getListLaver")getListLaver(VCV,V,M);
	if(name=="setPrioritetProcess")setPrioritetProcess(Pset,VCV,M);
	if(name=="getTime")getTime(V);
	if(name=="timeFormat")timeFormat(VCV,V);
	if(name=="PROGRAM")PROGRAM(VCV,V,M);
	if(name=="ControlOFF")ControlOFF(Pset,M,V);
	if(name=="ClearPsetOFF")ClearPsetOFF(Pset,M);
	if(name=="getAllModules")getAllModules(M,V);
	if(name=="getAllModuleFunctions")getAllModuleFunctions(VCV,M,V);
	if(name=="runFunction")runFunction(Pset,VCV,M,V);
	if(name=="replaceSumbolHTML")replaceSumbolHTML(VCV,V);
	if(name=="issystemf")issystemf(VCV,V);
	if(name=="insertInclude" || name=="eraseInclude")EIInclude(VCV,M,name=="insertInclude");
	if(name=="Spirit")Spirit(Pset,VCV,M);
	if(name=="Math")Match(VCV,M,V);
	if(
		name=="readFile" || name=="saveFile" || name=="addInFile" || 
		name=="existFile" || name=="existDir" || 
		name=="deleteFile" || name=="deleteDir" || name=="FindFiles" ||
		name=="renameFile" || name=="renameDir" || name=="createDir"
		)FileAccess(name,VCV,V);
	if(name=="Ajax")AJAX(VCV,V);
	if(name=="JSON")JSON(VCV,V);
	if(name=="Server")Server(M,VCV,V);
	if(name=="sizeof"){
		int size = 0;
		if(VCV.size())size = VCV[0].getSizeOf();
		V = new CVARIANT();
		V->avtoSet("int");
		V->DATA.intVal = size;
		return;
		}
	if(name=="Randomize"){
		int code;
		if(VCV.size()){
			VCV[0].TransformType("int");
			code = VCV[0].DATA.intVal;
			}else{
			code = UPServac::gettimeofday()*1111;
			}
		srand(code);
		return;
		}
	if(name=="MessageBeep"){
		int NubberBeep = 0;
		int HZ, time = 0;
		int si = VCV.size();
		if(si){
			VCV[0].TransformType("int");
			HZ = VCV[0].DATA.intVal;
			NubberBeep = HZ % 4;
			if(si>=2)if(VCV[1].isType("int"))time = VCV[1].DATA.intVal;
			}
		if(time<10){
			long n[] = {
				MB_ICONASTERISK,
				MB_ICONEXCLAMATION,
				MB_ICONHAND,
				MB_ICONQUESTION // none
				};
			MessageBeep(n[NubberBeep]);
			return;
			}
		Beep(HZ, time);
		return;
		}
	
}






//--------------------------------------------------------------------------------------------------
void CallFunc::mc(I*Pset,string&tt,string&name,V_CVARIANT&VCV,CVARIANT*&V,MAIN*M){
	string t=tt;
	CVARIANT*thisVal=NULL;
	Algorithm*A=NULL;
	int kill=0;
	int p=t.find("&:");
	if(!p){
		t=t.substr(2);
		int n=0;
		sscanf(t.c_str(),"%d",&n);
		A=Pset->sub->tableLinks[n];
		thisVal=Pset->sub->Bloki[A].X;
		kill=Pset->sub->Bloki[A].free;
		}
	t=tt;
	bool ok;
	p=t.find("*");
	if(!p){
		t=t.substr(1);
		thisVal=M->getUnLink(Pset,t,ok);
		}
	if(!thisVal)return;
	if(thisVal->isType("char"))forchar(thisVal,name,V,VCV);
	if(thisVal->isType("interval"))forinterval(thisVal,name,V,VCV);
	if(thisVal->isType("string"))forstring(thisVal,name,V,VCV);
	if(thisVal->isType("pointer"))forpointer(Pset,thisVal,name,V,VCV,M);
	if(thisVal->isType("vector"))forvector(thisVal,name,V,VCV);
	if(thisVal->isType("set"))forset(thisVal,name,V,VCV);
	if(thisVal->isType("map"))formap(thisVal,name,V,VCV);
	if(thisVal->isType("function"))forfunction(Pset,thisVal,name,V,VCV,M);
	if(thisVal->isType("program"))forprogram(thisVal,name,V,VCV);
	if(thisVal->isType("graph"))forgraf(thisVal,name,V,VCV,M);
	if(thisVal->isType("digit"))fordigit(thisVal,name,V,VCV);
	if(thisVal->isType("module"))formodule(thisVal,name,V,VCV,M);
	if(kill){
		Laver*L=M->tableLavers[Pset->Laver];
		if(L->ExtraExit)return;
		Pset->sub->Bloki.erase(A);
		delete thisVal;
		}
}



// CPP.cpp	:-|