//
//  GameHelp.cpp
//  RockPaperScissors
//
//  Created by mini01 on 14-5-10.
//
//

#include "GameHelp.h"
#include "MyMenu.h"

CCScene* GameHelp::scene()
{
    CCScene* scene = CCScene::create();
    
    GameHelp* layer = GameHelp::create();
    
    scene->addChild(layer);
    
    return scene;
}

bool GameHelp::init()
{
    if (!CCLayer::init())
    {
        return false;
    }
    
    pXmlparser = XMLParser::parseWithFile("userdata/loading.xml");
    
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    //添加背景图片
    CCSprite* backGround = CCSprite::create("gameui/mainBg.png");
    backGround->setPosition(_s_center);
    addChild(backGround);
    //标题背景
    CCSprite* titleBg = CCSprite::create("gameui/btnShare.png");
    titleBg->setScale(2);
    titleBg->setPosition(ccp(visibleSize.width / 2, visibleSize.height * 0.87));
    this->addChild(titleBg);
    //标题label
    CCLabelTTF* label_title = CCLabelTTF::create(pXmlparser->getString("menu_help")->getCString(), "Arial", 80);
    label_title->setPosition(titleBg->getPosition());
    this->addChild(label_title);
    //文档背景
    CCSprite* textBG = CCSprite::create("gameui/panel_game.png");
    textBG->setPosition(_s_center);
    textBG->setScale(1.2f);
    addChild(textBG);
    //文档内容
    CCLabelTTF* labelText = CCLabelTTF::create(pXmlparser->getString("help")->getCString(), "Arial", 40);
    labelText->setDimensions(CCSize(visibleSize.width * 0.75, visibleSize.height * 0.65));
    labelText->setPosition(ccp(visibleSize.width/2, visibleSize.height/2.5));
    this->addChild(labelText);
    
    //添加返回按钮
    CCMenuItemImage* btnBack = CCMenuItemImage::create("gameui/btnShare.png", "gameui/btnShare.png",this,menu_selector(GameHelp::backCallback));
    btnBack->setPosition(ccp(40 + btnBack->getContentSize().width / 2, 20 + btnBack->getContentSize().height / 2));
    CCLabelTTF* labelBack = CCLabelTTF::create(pXmlparser->getString("menu_back")->getCString(), "Arial", 40);
    labelBack->setPosition(btnBack->getPosition());
    this->addChild(labelBack,1);
    
    MyMenu* menu = MyMenu::create(1.5,btnBack,NULL);
    menu->setAnchorPoint(CCPointZero);
    menu->setPosition(CCPointZero);
    this->addChild(menu);
    
    return true;
}

void GameHelp::backCallback(CCObject* obj)
{
    //播放点击音效
    GSoundManager::sharedSoundManager()->playSound(SOUND_CLICK);
    CCDirector::sharedDirector()->popScene();
}