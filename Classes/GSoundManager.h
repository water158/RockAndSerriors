//
//  GsoundManager.h
//  RockPaperScissors
//
//  Created by mini01 on 14-5-8.
//
//

#ifndef __RockPaperScissors__GsoundManager__
#define __RockPaperScissors__GsoundManager__

#include <iostream>
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "UserData.h"

#define SOUND_CLICK "buttonEffet.mp3"
#define SOUND_MOVE "buttonEffet.mp3"
#define BGMUSIC "bgMusic.mp3"


USING_NS_CC;

class GSoundManager : public CCNode
{
public:
    static GSoundManager* sharedSoundManager();
    //播放声音
    void playSound(const char* fileName);
    //停止播放音效
    void stopSound();
    //播放背景音乐
    void playBackgroundMisc(const char* fileName);
    //停止播放音乐
    void stopBackgroundMisc();
    
private:
    static GSoundManager* s_SharedGSoundManager;
    
    bool init();
    
    GSoundManager();
    ~GSoundManager();
};

#endif /* defined(__RockPaperScissors__GsoundManager__) */
