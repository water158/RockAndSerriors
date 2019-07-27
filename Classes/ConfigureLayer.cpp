//
//  ConfigureLayer.cpp
//  RockPaperScissors
//
//  Created by mini01 on 14-5-4.
//
//

#include "ConfigureLayer.h"

CCScene* ConfigureLayer::scene()
{
    CCScene* scene = CCScene::create();
    
    ConfigureLayer* layer = ConfigureLayer::create();
    
    scene->addChild(layer);
    
    return scene;
}

bool ConfigureLayer::init()
{
    if (!CCLayer::init())
    {
        return false;
    }
    
    pXmlParser = XMLParser::parseWithFile("userdata/loading.xml");
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    
    //添加背景图片
    CCSprite* backGround = CCSprite::create("gameui/mainBg.png");
    backGround->setPosition(_s_center);
    addChild(backGround);
    //中间框
    CCSprite* panel_frame = CCSprite::create("gameui/panel_game.png");
    panel_frame->setScaleX(1.2);
    panel_frame->setScaleY(1.5);
    panel_frame->setPosition(ccp(_s_center.x,_s_center.y - 80));
    addChild(panel_frame);
    //添加游戏lag
    CCSprite* flag = CCSprite::create("gameui/gameFlag.png");
    flag->setPosition(ccp(visibleSize.width / 2, visibleSize.height - 40 - flag->getContentSize().height / 2));
    flag->setScale(1.5f);
    this->addChild(flag);
    //添加音效字符
    CCLabelTTF* labelSound = CCLabelTTF::create(pXmlParser->getString("label_sound")->getCString(), "Arial", 40);
    labelSound->setPosition(ccp(visibleSize.width * 0.3f, visibleSize.height * 0.75f));
    labelSound->setColor(ccRED);
    this->addChild(labelSound);
    //添加音效控制按钮背景
    contrSoundBG = CCSprite::create("gameui/sliderBG.png");
    contrSoundBG->setPosition(ccp(visibleSize.width * 0.7f, visibleSize.height * 0.75f));
    this->addChild(contrSoundBG,0);
    //添加音效控制按钮
    contrSound = CCSprite::create("gameui/slider.png");
    if (UserData::sharedUserData()->getIsSoundEnable())
    {
        contrSound->setPosition(ccp(contrSound->getContentSize().width / 2, contrSound->getContentSize().height / 2));
        
    }
    else
    {
        contrSound->setPosition(ccp(contrSoundBG->getContentSize().width - contrSound->getContentSize().width / 2, contrSound->getContentSize().height / 2));
    }
    contrSoundBG->addChild(contrSound,1);
    //添加音乐字符
    CCLabelTTF* labelMsic = CCLabelTTF::create(pXmlParser->getString("label_music")->getCString(), "Arial", 40);
    labelMsic->setPosition(ccp(visibleSize.width * 0.3f, visibleSize.height * 0.6f));
    labelMsic->setColor(ccRED);
    this->addChild(labelMsic);
    //添加音乐控制按钮背景
    contrMsicBG = CCSprite::create("gameui/sliderBG.png");
    contrMsicBG->setPosition(ccp(visibleSize.width * 0.7f, visibleSize.height * 0.6f));
    this->addChild(contrMsicBG,0);
    //添加音乐控制按钮
    contrMsic = CCSprite::create("gameui/slider.png");
    if (UserData::sharedUserData()->getIsMusicEnable())
    {
        contrMsic->setPosition(ccp(contrMsic->getContentSize().width / 2, contrMsic->getContentSize().height / 2));
    }
    else
    {
        contrMsic->setPosition(ccp(contrMsicBG->getContentSize().width - contrMsic->getContentSize().width / 2, contrMsic->getContentSize().height / 2));
    }
    contrMsicBG->addChild(contrMsic,1);
    //添加返回按钮
    CCMenuItemImage* btnBack = CCMenuItemImage::create("gameui/btnBackBG.png", "gameui/btnBackBG.png",this,menu_selector(ConfigureLayer::backCallback));
    btnBack->setPosition(ccp(visibleSize.width / 2, visibleSize.height * 0.15f));
    btnBack->setScaleX(2.5);
    btnBack->setScaleY(1.5f);
    CCLabelTTF* labelBack = CCLabelTTF::create(pXmlParser->getString("menu_backgame")->getCString(), "Arial", 40);
    labelBack->setPosition(btnBack->getPosition());
    this->addChild(labelBack,1);
    //添加关于按钮
    CCMenuItemImage* about = CCMenuItemImage::create("gameui/btnBackBG.png", "gameui/btnBackBG.png", this,menu_selector(ConfigureLayer::helpAndAboutCallback));
    about->setPosition(ccp(visibleSize.width/2, visibleSize.height * 0.3f));
    about->setScaleX(2.5);
    about->setScaleY(1.5f);
    about->setTag(eAbout);
    CCLabelTTF* labelAbout = CCLabelTTF::create(pXmlParser->getString("menu_about")->getCString(), "Arial", 40);
    labelAbout->setPosition(about->getPosition());
    this->addChild(labelAbout,1);
    //添加帮助按钮
    CCMenuItemImage* help = CCMenuItemImage::create("gameui/btnBackBG.png", "gameui/btnBackBG.png",this,menu_selector(ConfigureLayer::helpAndAboutCallback));
    help->setPosition(ccp(visibleSize.width / 2, visibleSize.height * 0.45f));
    help->setScaleX(2.5);
    help->setScaleY(1.5f);
    help->setTag(eHelp);
    CCLabelTTF* labelHelp = CCLabelTTF::create(pXmlParser->getString("menu_help")->getCString(), "Arial", 40);
    labelHelp->setPosition(help->getPosition());
    this->addChild(labelHelp,1);
    
    MyMenu* menu = MyMenu::create(1.5,btnBack,about,help,NULL);
    menu->setAnchorPoint(CCPointZero);
    menu->setPosition(CCPointZero);
    this->addChild(menu,0);
    //开启触摸
    this->setTouchEnabled(true);
    return true;
}

void ConfigureLayer::changeConfigCallback(cocos2d::CCObject *pSender)
{

}

void ConfigureLayer::helpAndAboutCallback(cocos2d::CCObject *pSender)
{
    GSoundManager::sharedSoundManager()->playSound(SOUND_CLICK);
    CCMenuItemImage* item = (CCMenuItemImage*)pSender;
    if (item->getTag() == eHelp)
    {
        CCScene* scene = GameHelp::scene();
        CCDirector::sharedDirector()->pushScene(scene);
    }
    else if (item->getTag() == eAbout)
    {
        CCScene* scene = GameAbout::scene();
        CCDirector::sharedDirector()->pushScene(scene);
    }
}

void ConfigureLayer::backCallback(cocos2d::CCObject *pSender)
{
    GSoundManager::sharedSoundManager()->playSound(SOUND_CLICK);
    CCDirector::sharedDirector()->popScene();
}

void ConfigureLayer::onEnter()
{
    CCLayer::onEnter();
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 1, false);
}

bool ConfigureLayer::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    CCPoint pos = pTouch->getLocation();
    //获取音效的点击框   扩大点击选取范围
    CCRect soundRec = contrSoundBG->boundingBox();
    
    if (CCRect(soundRec.getMinX() * 0.95, soundRec.getMinY() * 0.95, soundRec.getMaxX() * 1.05, soundRec.getMaxY() * 1.05).containsPoint(pos))
    {
        //如果音效是打开状态  播放按钮向右动画 关闭音效  否则向左滑动 打开音效
        if (UserData::sharedUserData()->getIsSoundEnable())
        {
            contrSound->runAction(CCMoveTo::create(0.1, ccp(contrSoundBG->getContentSize().width - contrSound->getContentSize().width / 2,contrSound->getContentSize().height/2)));
            UserData::sharedUserData()->setIsSoundEnable(false);
            UserData::sharedUserData()->save();
            GSoundManager::sharedSoundManager()->stopSound();
        }
        else
        {
            contrSound->runAction(CCMoveTo::create(0.1, ccp(contrSound->getContentSize().width / 2,contrSound->getContentSize().height/2)));
            UserData::sharedUserData()->setIsSoundEnable(true);
            UserData::sharedUserData()->save();
        }
        return true;
    }
    //获取音乐的点击框   扩大点击选取范围
    CCRect musicRec = contrMsicBG->boundingBox();
    if (CCRect(musicRec.getMinX() * 0.95, musicRec.getMinY() * 0.95, musicRec.getMaxX() * 1.05, musicRec.getMaxY() * 1.05).containsPoint(pos))
    {
        //如果音乐是打开状态 播放按钮向右动画 关闭音乐 否则向左滑动 打开音乐
        if (UserData::sharedUserData()->getIsMusicEnable())
        {
            contrMsic->runAction(CCMoveTo::create(0.1, ccp(contrMsicBG->getContentSize().width - contrMsic->getContentSize().width / 2,contrMsic->getContentSize().height/2)));
            UserData::sharedUserData()->setIsMusicEnable(false);
            UserData::sharedUserData()->save();
            GSoundManager::sharedSoundManager()->stopBackgroundMisc();
        }
        else
        {
            contrMsic->runAction(CCMoveTo::create(0.1, ccp(contrMsic->getContentSize().width / 2,contrMsic->getContentSize().height/2)));
            UserData::sharedUserData()->setIsMusicEnable(true);
            UserData::sharedUserData()->save();
            GSoundManager::sharedSoundManager()->playBackgroundMisc(BGMUSIC);
        }
        return true;
    }
    return false;
}

void ConfigureLayer::ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{

}

void ConfigureLayer::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    
}


