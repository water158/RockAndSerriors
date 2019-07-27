#ifndef _STRING_LIBRARY_H_
#define _STRING_LIBRARY_H_

#include "cocos2d.h"
#include "tinyxml.h"
#include "Tools.h"
#include "GlobalEnum.h"

USING_NS_CC;
using namespace std;

class StringLibrary
{
public:
    StringLibrary(void);
	virtual ~StringLibrary(void);
    
    static StringLibrary* sharedStringLibrary(void);
    
    //初始化数据
    void init();
    
	//
	void preload();

	//
    CCString* getStringByKey(string key);
    
private:
    static StringLibrary* s_sharedStringLibrary;
    
    CC_SYNTHESIZE(CCDictionary*, dict_strings, DictStrings);
};

#endif