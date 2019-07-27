//
//  GsoundManager.cpp
//  RockPaperScissors
//
//  Created by mini01 on 14-5-8.
//
//

#include "GsoundManager.h"
using namespace CocosDenshion;

GSoundManager* GSoundManager::s_SharedGSoundManager = NULL;

GSoundManager::GSoundManager()
{

}

GSoundManager::~GSoundManager()
{
    if (s_SharedGSoundManager)
    {
        delete s_SharedGSoundManager;
    }
}

GSoundManager* GSoundManager::sharedSoundManager()
{
    if (!s_SharedGSoundManager)
    {
        s_SharedGSoundManager = new GSoundManager();
        s_SharedGSoundManager->init();
    }
    return s_SharedGSoundManager;
}

bool GSoundManager::init()
{
    return true;
}

void GSoundManager::playSound(const char *fileName)
{
    if (UserData::sharedUserData()->getIsSoundEnable())
    {
        SimpleAudioEngine::sharedEngine()->playEffect(fileName);
    }
}

void GSoundManager::playBackgroundMisc(const char *fileName)
{
    if (UserData::sharedUserData()->getIsMusicEnable())
    {
        SimpleAudioEngine::sharedEngine()->playBackgroundMusic(fileName, true);
    }
}

void GSoundManager::stopBackgroundMisc()
{
    SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
}

void GSoundManager::stopSound()
{
    SimpleAudioEngine::sharedEngine()->stopAllEffects();
}