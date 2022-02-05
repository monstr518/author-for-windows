//LoaderCode.h

#ifdef DEF_PCONS_INCLUDE
	#include <fstream.h>
#endif



#ifdef DEF_PCONS_CONTENT
//--------------------------------------------------------------------------------------------------
class LOADER{
	void load(const char*);
public:
	void delete_komentary();
	char*text;
	LOADER(const char*,bool on_komentaru=1);
	LOADER();
	~LOADER();
	void out(char*);
	static char*copy(std::string&);
};

#endif
//LoaderCode.h