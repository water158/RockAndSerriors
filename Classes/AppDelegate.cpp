//
//  RockPaperScissorsAppDelegate.cpp
//  RockPaperScissors
//
//  Created by mini01 on 14-5-4.
//  Copyright __MyCompanyName__ 2014年. All rights reserved.
//

#include "AppDelegate.h"

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "GameLayer.h"
#include "SmartRes.h"
#include "HelloWorldScene.h"
#include "GameCombatLayer.h"


USING_NS_CC;
using namespace CocosDenshion;
using namespace cn::sharesdk;


AppDelegate::AppDelegate()
{

}

AppDelegate::~AppDelegate()
{
}

bool AppDelegate::applicationDidFinishLaunching()
{
    //用于友盟
    //MobClickCpp::startWithAppkey("53703d3356240b68020ce0f0");
    
    //初始化ShareSDK
    C2DXShareSDK::open(CCString::create("1c846ffaae5a"), false);
    
    //初始化社交平台信息
    this -> initPlatformConfig();
    
    // initialize director
    CCDirector *pDirector = CCDirector::sharedDirector();
    pDirector->setOpenGLView(CCEGLView::sharedOpenGLView());

	SmartRes::sharedRes()->setVirtualScreenHeight(960.0f);
    // turn on display FPS
    pDirector->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    CCScene *pScene = GameCombatLayer::scene();

    // run
    pDirector->runWithScene(pScene);
    


    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    CCDirector::sharedDirector()->stopAnimation();
    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
    SimpleAudioEngine::sharedEngine()->pauseAllEffects();
    //用于友盟
    //MobClickCpp::applicationDidEnterBackground();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    CCDirector::sharedDirector()->startAnimation();
    SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
    SimpleAudioEngine::sharedEngine()->resumeAllEffects();
    //MobClickCpp::applicationWillEnterForeground();
}


void AppDelegate::initPlatformConfig()
{
    //短信
    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeSMS, NULL);
    
    //QQ空间
    CCDictionary *qzConfigDict = CCDictionary::create();
    qzConfigDict -> setObject(CCString::create("100371282"), "app_id");
    qzConfigDict -> setObject(CCString::create("aed9b0303e3ed1e27bae87c33761161d"), "app_key");
    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeQZone, qzConfigDict);
    
    //微信
    CCDictionary *wcConfigDict = CCDictionary::create();
    wcConfigDict -> setObject(CCString::create("wxe107521dbb2791cf"), "app_id");
    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeWeixiSession, wcConfigDict);
    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeWeixiTimeline, wcConfigDict);
    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeWeixiFav, wcConfigDict);
    
    //QQ
    CCDictionary *qqConfigDict = CCDictionary::create();
    qqConfigDict -> setObject(CCString::create("100371282"), "app_id");
    C2DXShareSDK::setPlatformConfig(C2DXPlatTypeQQ, qqConfigDict);
    
}
