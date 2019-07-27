//
//  GameAbout.h
//  RockPaperScissors
//
//  Created by mini01 on 14-5-10.
//
//

#ifndef __RockPaperScissors__GameAbout__
#define __RockPaperScissors__GameAbout__

#include <iostream>
#include "cocos2d.h"
#include "MyXmlParser.h"
#include "GsoundManager.h"
#include "MyMenu.h"
#include "SmartRes.h"

using namespace cocos2d;

class GameAbout : public CCLayer
{
public:
    static CCScene* scene();
    
    virtual bool init();
    
    CREATE_FUNC(GameAbout);
    
    void backCallback(CCObject* obj);
private:
    XMLParser* pXmlparser;
    
};

#endif /* defined(__RockPaperScissors__GameAbout__) */
