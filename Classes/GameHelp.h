//
//  GameHelp.h
//  RockPaperScissors
//
//  Created by mini01 on 14-5-10.
//
//

#ifndef __RockPaperScissors__GameHelp__
#define __RockPaperScissors__GameHelp__

#include <iostream>
#include "cocos2d.h"
#include "MyXmlParser.h"
#include "GsoundManager.h"
#include "SmartRes.h"

using namespace cocos2d;

class GameHelp : public CCLayer
{
public:
    static CCScene* scene();
    
    virtual bool init();
    
    CREATE_FUNC(GameHelp);
    
    void backCallback(CCObject* obj);
private:
    XMLParser* pXmlparser;
};

#endif /* defined(__RockPaperScissors__GameHelp__) */
