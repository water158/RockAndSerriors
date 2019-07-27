#include "GameCombatLayer.h"

/***************************
 * 初始化 构造 什么的
 ***************************/

CCScene* GameCombatLayer::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    GameCombatLayer *layer = GameCombatLayer::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameCombatLayer::init()
{
    if (!CCLayer::init())
    {
        return false;
    }
    
    this->setTouchEnabled(true);
    
    
    //**************
    //游戏数据初始化
    //**************
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    
    StringLibrary::sharedStringLibrary()->init();
    
    //获得上次用户选择的难度
    m_nDifficulty = UserData::sharedUserData()->getSelectDifficulty();
    m_nDifficultySelected = m_nDifficulty;
    
    //初始化棋盘的逻辑
    BoardLogic::sharedBoardLogic()->init(m_nDifficulty);
    
    //用于友盟统计进入关卡
    //MobClickCpp::startLevel(getStringFromInt(m_nDifficulty).c_str());
    
    //**************
    //游戏数据打印
    //**************
    CCLog("size width : %f,height : %f", visibleSize.width, visibleSize.height);
    
    CCLog("Difficulty = %d", m_nDifficulty);
    
    //**************
    //游戏UI初始化
    // 返回按钮 成就按钮 配置按钮
    // 步数显示 下一个显示
    //**************
    
    //**********
    //资源预加载
    //**********
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("gameui/hex/hex.plist");
    
    //**********
    //大背景
    //**********
    //添加背景图片
    CCSprite* backGround = CCSprite::create("gameui/mainBg.png");
    backGround->setPosition(_s_center);
    addChild(backGround);
    
    //**********
    //步数
    //**********
    //滑动步数背景
    CCSprite* stepBG = CCSprite::create("gameui/stepFrame.png");
    stepBG->setPosition(ccp(SP_STEPS_X, SP_STEPS_Y));
    this->addChild(stepBG);
    
    //步数显示的图标
    labelStep = CCLabelTTF::create("0", "Arial", 50);
    labelStep->setPosition(stepBG->getPosition());
    this->addChild(labelStep);
    
    //**********
    //下一个
    //**********
    spNextItem = CCSprite::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("hex_empty.png"));
    spNextItem->setPosition(ccp(SP_NEXTITEM_X, SP_NEXTITEM_Y));
    spNextItem->setScale(0.3);
    addChild(spNextItem);
    
    //**********
    //难度
    //**********
    //难度背景
    CCMenuItemImage* btnDiff = CCMenuItemImage::create("gameui/btnBG.png", "gameui/btnBG.png",this,menu_selector(GameCombatLayer::callbackChangeDifficulty));
    btnDiff->setPosition(ccp(BTN_DIF_X, BTN_DIF_Y));
    
    //添加难度显示
    CCString* str = CCString::createWithFormat("gameui/%d.png",m_nDifficulty);
    CCSprite* sprDifficulty = CCSprite::create(str->getCString());
    sprDifficulty->setPosition(btnDiff->getPosition());
    sprDifficulty->setTag(DIFFICULTYTAG);
    addChild(sprDifficulty,1);
    
    //**********
    //配置
    //**********
    CCMenuItemImage* btnConfig = CCMenuItemImage::create("gameui/btnBG.png", "gameui/btnBG.png",this,menu_selector(GameCombatLayer::callbackOpenConfig));
    btnConfig->setPosition(ccp(BTN_CONFIG_X, BTN_CONFIG_Y));
    
    //菜单字样
    CCSprite* labelConfig = CCSprite::create("gameui/label_menu.png");
    labelConfig->setPosition(btnConfig->getPosition());
    addChild(labelConfig, 1);
    
    //**********
    //开始
    //**********
    //开始按钮
    CCMenuItemImage* btnStart = CCMenuItemImage::create("gameui/btnStartBG.png", "gameui/btnStartBG.png",this,menu_selector(GameCombatLayer::callbackStart));
    btnStart->setPosition(ccp(BTN_BACK_X, BTN_BACK_Y));
    
    //**********
    //乾坤大挪移
    //**********
    int times_left = BoardLogic::sharedBoardLogic()->getMagicTimes();
    CCMenuItemFont* btnUseMagic = CCMenuItemFont::create(CCString::createWithFormat("乾坤大挪移 X %d", times_left)->getCString(), this, menu_selector(GameCombatLayer::callbackUseMagic));
    btnUseMagic->setColor(ccRED);
    btnUseMagic->setPosition(ccp(visibleSize.width / 2, 80));
    
    //**********
    //成就
    //**********
    //锤子
    btnRock = CCMenuItemImage::create("gameui/btnRock.png", "gameui/btnRock.png",this,menu_selector(GameCombatLayer::callbackOpenAchievement));
    btnRock->setPosition(ccp(BTN_ROCK_X,BTN_ACHIEVE_Y));
    
    //布
    btnPaper = CCMenuItemImage::create("gameui/btnPapper.png", "gameui/btnPapper.png",this,menu_selector(GameCombatLayer::callbackOpenAchievement));
    btnPaper->setPosition(ccp(BTN_PAPER_X,BTN_ACHIEVE_Y));
    
    //剪子
    btnScissors = CCMenuItemImage::create("gameui/btnScissors.png", "gameui/btnScissors.png",this,menu_selector(GameCombatLayer::callbackOpenAchievement));
    btnScissors->setPosition(ccp(BTN_SCISSORS_X,BTN_ACHIEVE_Y));
    
    //更新成就按钮信息
    this->updateAchieveBtn();
    
    //**********
    //把所有按钮放在Menu里面
    //**********
    MyMenu* menu = MyMenu::create(1.5, btnDiff, btnStart, btnRock, btnPaper, btnScissors, btnConfig, btnUseMagic, NULL);
    menu->setAnchorPoint(CCPointZero);
    menu->setPosition(CCPointZero);
    this->addChild(menu);
    
    //**********
    //棋盘背景
    //**********
    //创建棋盘的灰色格子区
    layerBackground = GameBackgroundLayer::create();
    layerBackground->setPosition(ccp(_s_left, _s_center.y + BACKGROUND_Y_OFFSET));
    this->addChild(layerBackground, kGameOrderBackground);
    
    //**********
    //棋盘
    //**********
    //创建棋盘
    layerPlay = GamePlayLayer::create();
    layerPlay->delegateGameUI = this;
    layerPlay->setPosition(ccp(_s_left, _s_center.y + BACKGROUND_Y_OFFSET));
    this->addChild(layerPlay, kGameOrderPlay);
    
    //随机添加两个单元
    //layerPlay->random();
    //layerPlay->random();
    
    return true;
}

void GameCombatLayer::callbackChangeMapType(CCObject *pSender)
{
    /*
    //更改逻辑层中的地图样式信息
    GameMapType curMapType = BoardLogic::sharedBoardLogic()->getMapType();
    if (curMapType == kMapTypeHex)
    {
        BoardLogic::sharedBoardLogic()->setMapType(kMapTypeRect);
        if (btnChangMapType->getChildByTag(10))
        {
            btnChangMapType->removeChildByTag(10);
            CCLabelTTF* mapTypeText = CCLabelTTF::create("Rect", "Arial", 40);
            mapTypeText->setPosition(ccp(50, 30));
            btnChangMapType->addChild(mapTypeText,0,10);
        }
        
    }
    else
    {
        BoardLogic::sharedBoardLogic()->setMapType(kMapTypeHex);
        if (btnChangMapType->getChildByTag(10))
        {
            btnChangMapType->removeChildByTag(10);
            CCLabelTTF* mapTypeText = CCLabelTTF::create("Hex", "Arial", 40);
            mapTypeText->setPosition(ccp(50, 30));
            btnChangMapType->addChild(mapTypeText,0,10);
        }
    }
     */
}

void GameCombatLayer::updateAchieveBtn()
{
    //按钮纹理 分为初始 铜 银 金 白金
    //石头
    CCSpriteFrame* spFrameRock_ori = CCSpriteFrame::create("gameui/btnRock.png", CCRectMake(0, 0, BTN_ACHIEVE_WIDTH,BTN_ACHIEVE_HEIGHT));
    CCSpriteFrame* spFrameRock_brone = CCSpriteFrame::create("gameui/broneRock.png", CCRectMake(0, 0, BTN_ACHIEVE_WIDTH,BTN_ACHIEVE_HEIGHT));
    CCSpriteFrame* spFrameRock_sliver = CCSpriteFrame::create("gameui/silverRock.png", CCRectMake(0, 0, BTN_ACHIEVE_WIDTH,BTN_ACHIEVE_HEIGHT));
    CCSpriteFrame* spFrameRock_gold = CCSpriteFrame::create("gameui/GoldRock.png", CCRectMake(0, 0, BTN_ACHIEVE_WIDTH,BTN_ACHIEVE_HEIGHT));
    CCSpriteFrame* spFrameRock_Pt = CCSpriteFrame::create("gameui/PtRock.png", CCRectMake(0, 0, BTN_ACHIEVE_WIDTH,BTN_ACHIEVE_HEIGHT));
    //剪刀
    CCSpriteFrame* spFrameScissors_ori = CCSpriteFrame::create("gameui/btnScissors.png", CCRectMake(0, 0, BTN_ACHIEVE_WIDTH,BTN_ACHIEVE_HEIGHT));
    CCSpriteFrame* spFrameScissors_brone = CCSpriteFrame::create("gameui/broneScissors.png", CCRectMake(0, 0, BTN_ACHIEVE_WIDTH,BTN_ACHIEVE_HEIGHT));
    CCSpriteFrame* spFrameScissors_sliver = CCSpriteFrame::create("gameui/silverScissors.png", CCRectMake(0, 0, BTN_ACHIEVE_WIDTH,BTN_ACHIEVE_HEIGHT));
    CCSpriteFrame* spFrameScissors_gold = CCSpriteFrame::create("gameui/GoldScissors.png", CCRectMake(0, 0, BTN_ACHIEVE_WIDTH,BTN_ACHIEVE_HEIGHT));
    CCSpriteFrame* spFrameScissors_Pt = CCSpriteFrame::create("gameui/PtScissors.png", CCRectMake(0, 0, BTN_ACHIEVE_WIDTH,BTN_ACHIEVE_HEIGHT));
    //布
    CCSpriteFrame* spFramePaper_ori = CCSpriteFrame::create("gameui/btnPapper.png", CCRectMake(0, 0, BTN_ACHIEVE_WIDTH,BTN_ACHIEVE_HEIGHT));
    CCSpriteFrame* spFramePaper_brone = CCSpriteFrame::create("gameui/bronePapper.png", CCRectMake(0, 0, BTN_ACHIEVE_WIDTH,BTN_ACHIEVE_HEIGHT));
    CCSpriteFrame* spFramePaper_sliver = CCSpriteFrame::create("gameui/silverPapper.png", CCRectMake(0, 0, BTN_ACHIEVE_WIDTH,BTN_ACHIEVE_HEIGHT));
    CCSpriteFrame* spFramePaper_gold = CCSpriteFrame::create("gameui/GoldPapper.png", CCRectMake(0, 0, BTN_ACHIEVE_WIDTH,BTN_ACHIEVE_HEIGHT));
    CCSpriteFrame* spFramePaper_Pt = CCSpriteFrame::create("gameui/PtPapper.png", CCRectMake(0, 0, BTN_ACHIEVE_WIDTH,BTN_ACHIEVE_HEIGHT));
    //通过次数
    int times = UserData::sharedUserData()->getTimesByKey(m_nDifficulty, eRock);
    if (times < BRONZE_MEDAL)
    {
        btnRock->setNormalSpriteFrame(spFrameRock_ori);
        btnRock->setSelectedSpriteFrame(spFrameRock_ori);
    }
    else if (times < SILVER_MEDAL)
    {
        btnRock->setNormalSpriteFrame(spFrameRock_brone);
        btnRock->setSelectedSpriteFrame(spFrameRock_brone);
    }
    else if (times < GOLD_MEDAL)
    {
        btnRock->setNormalSpriteFrame(spFrameRock_sliver);
        btnRock->setSelectedSpriteFrame(spFrameRock_sliver);
    }
    else if (times < PLATINUM_MEDAL)
    {
        btnRock->setNormalSpriteFrame(spFrameRock_gold);
        btnRock->setSelectedSpriteFrame(spFrameRock_gold);
    }
    else
    {
        btnRock->setNormalSpriteFrame(spFrameRock_Pt);
        btnRock->setSelectedSpriteFrame(spFrameRock_Pt);
    }
    
    times = UserData::sharedUserData()->getTimesByKey(m_nDifficulty, eScissors);
    if (times < BRONZE_MEDAL)
    {
        btnScissors->setNormalSpriteFrame(spFrameScissors_ori);
        btnScissors->setSelectedSpriteFrame(spFrameScissors_ori);
    }
    else if (times < SILVER_MEDAL)
    {
        btnScissors->setNormalSpriteFrame(spFrameScissors_brone);
        btnScissors->setSelectedSpriteFrame(spFrameScissors_brone);
    }
    else if (times < GOLD_MEDAL)
    {
        btnScissors->setNormalSpriteFrame(spFrameScissors_sliver);
        btnScissors->setSelectedSpriteFrame(spFrameScissors_sliver);
    }
    else if (times < PLATINUM_MEDAL)
    {
        btnScissors->setNormalSpriteFrame(spFrameScissors_gold);
        btnScissors->setSelectedSpriteFrame(spFrameScissors_gold);
    }
    else
    {
        btnScissors->setNormalSpriteFrame(spFrameScissors_Pt);
        btnScissors->setSelectedSpriteFrame(spFrameScissors_Pt);
    }
    times = UserData::sharedUserData()->getTimesByKey(m_nDifficulty, ePaper);
    if (times < BRONZE_MEDAL)
    {
        btnPaper->setNormalSpriteFrame(spFramePaper_ori);
        btnPaper->setSelectedSpriteFrame(spFramePaper_ori);
    }
    else if (times < SILVER_MEDAL)
    {
        btnPaper->setNormalSpriteFrame(spFramePaper_brone);
        btnPaper->setSelectedSpriteFrame(spFramePaper_brone);
    }
    else if (times < GOLD_MEDAL)
    {
        btnPaper->setNormalSpriteFrame(spFramePaper_sliver);
        btnPaper->setSelectedSpriteFrame(spFramePaper_sliver);
    }
    else if (times < PLATINUM_MEDAL)
    {
        btnPaper->setNormalSpriteFrame(spFramePaper_gold);
        btnPaper->setSelectedSpriteFrame(spFramePaper_gold);
    }
    else
    {
        btnPaper->setNormalSpriteFrame(spFramePaper_Pt);
        btnPaper->setSelectedSpriteFrame(spFramePaper_Pt);
    }
}

/***************************
 * UI 回调
 ***************************/
void GameCombatLayer::callbackStart(CCObject *pSender)
{
    CCLog("callbackStart");
    
    m_nDifficulty = m_nDifficultySelected;
    
    //保存到userdata
    UserData::sharedUserData()->setSelectDifficulty(m_nDifficulty);
    UserData::sharedUserData()->save();
    
    
    BoardLogic::sharedBoardLogic()->clear(m_nDifficulty);
    
    layerBackground->removeFromParentAndCleanup(true);
    
    layerBackground = GameBackgroundLayer::create();
    layerBackground->setPosition(ccp(_s_left, _s_center.y + BACKGROUND_Y_OFFSET));
    this->addChild(layerBackground, kGameOrderBackground);
    
    layerPlay->removeFromParentAndCleanup(true);
    
    layerPlay = GamePlayLayer::create();
    layerPlay->delegateGameUI = this;
    layerPlay->setPosition(ccp(_s_left, _s_center.y + BACKGROUND_Y_OFFSET));
    this->addChild(layerPlay, kGameOrderPlay);
}

void GameCombatLayer::callbackChangeDifficulty(CCObject *pSender)
{
    //播放点击音效
    GSoundManager::sharedSoundManager()->playSound(SOUND_CLICK);
    
    //将难度系数+1  如果难度系数大于9 则从2开始
    m_nDifficultySelected ++;
    if (m_nDifficultySelected > 6)
    {
        m_nDifficultySelected = 3;
    }
    if (this->getChildByTag(DIFFICULTYTAG))
    {
        removeChildByTag(DIFFICULTYTAG);
    }
    //将难度系数显示在界面上
    CCString* textDif = CCString::createWithFormat("gameui/%d.png",m_nDifficultySelected);
    CCSprite* labelDif = CCSprite::create(textDif->getCString());
    labelDif->setPosition(ccp(BTN_DIF_X, BTN_DIF_Y));
    labelDif->setTag(DIFFICULTYTAG);
    addChild(labelDif,1);
}

void GameCombatLayer::callbackUseMagic(CCObject* pSender)
{
    CCMenuItemFont* pMenu = (CCMenuItemFont*)pSender;
    
    if (BoardLogic::sharedBoardLogic()->getMagicTimes() > 0)
    {
        BoardLogic::sharedBoardLogic()->useMagic();
        
        int times_left = BoardLogic::sharedBoardLogic()->getMagicTimes();
        
        pMenu->setString(CCString::createWithFormat("乾坤大挪移 X %d", times_left)->getCString());
        
        layerPlay->refresh();
    }
}

void GameCombatLayer::callbackOpenConfig(CCObject *pSender)
{
    //播放点击音效
    GSoundManager::sharedSoundManager()->playSound(SOUND_CLICK);
    
    //显示
    CCScene* pScene = ConfigureLayer::scene();
    CCDirector::sharedDirector()->pushScene(pScene);
}

void GameCombatLayer::callbackOpenAchievement(CCObject *pSender)
{
    //播放点击音效
    GSoundManager::sharedSoundManager()->playSound(SOUND_CLICK);
    
    //显示
    CCScene* pScene = AchieveLayer::scene();
    CCDirector::sharedDirector()->pushScene(pScene);
}

void GameCombatLayer::callbackWinSure(CCObject *pSender)
{
    this->removeChild(layerShield);
}

void GameCombatLayer::callbackChangeNextItem()
{
    switch (BoardLogic::sharedBoardLogic()->getNextRandomItemType())
    {
        case ePaper:
            spNextItem->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("hex_paper.png"));
            break;
        case eScissors:
            spNextItem->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("hex_scissors.png"));
            break;
        case eRock:
            spNextItem->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("hex_rock.png"));
            break;
    }
}

/***************************
 * GameUIDelegate 回调
 ***************************/

void GameCombatLayer::callbackRefreshSteps()
{
    labelStep->setString(getStringFromInt(BoardLogic::sharedBoardLogic()->getSteps()).c_str());
}

void GameCombatLayer::callbackCheckGameOver()
{
    //用于友盟统计
    //MobClickCpp::finishLevel(getStringFromInt(m_nDifficulty).c_str());
    ItemType typeWin;
    
    if (!BoardLogic::sharedBoardLogic()->isGameOver(typeWin))
    {
        return;
    }
    
    //保存数据
    AchieveRank* rank = AchieveRank::create();
    rank->setStep(BoardLogic::sharedBoardLogic()->getSteps());
    rank->setDate(getCurtime());
    
    //其他信息
    int difficulty = UserData::sharedUserData()->getSelectDifficulty();
    ItemType type = typeWin;
    
    int ret = UserData::sharedUserData()->addUserRankInfo(difficulty, type, rank);
    CCLog("save result is %d", ret);
    
    //保存成就通过的次数
    UserData::sharedUserData()->addTimesByKey(difficulty, type);
    
    //打印成就通过的次数
    CCLog("have win %d times!", UserData::sharedUserData()->getTimesByKey(difficulty, type));
    
    //用于友盟统计
    //MobClickCpp::event(getStringFromInt(m_nDifficulty * 10 + info->type).c_str());
    
    //显示胜利信息
    CCMenuItemImage* winBG = CCMenuItemImage::create("gameui/stepFrame.png",
                                                     "gameui/stepFrame.png",
                                                     this,
                                                     menu_selector(GameCombatLayer::callbackWinSure));
    winBG->setPosition(ccp(_s_width / 2, _s_height / 2));
    
    
    CCLabelTTF* label_sure = CCLabelTTF::create(StringLibrary::sharedStringLibrary()->getStringByKey("menu_sure")->getCString(), "Arial", 30);
    label_sure->setPosition(winBG->getPosition());
    
    
    //添加游戏胜利提示
    CCLabelTTF* label_win = CCLabelTTF::create(StringLibrary::sharedStringLibrary()->getStringByKey("label_win")->getCString(), "Arial", 40);
    label_win->setPosition(ccp(_s_width / 2,_s_height / 2 + 80));
    label_win->setColor(ccRED);
    
    MyMenu* menu = MyMenu::create(1.5, winBG, NULL);
    menu->setAnchorPoint(CCPointZero);
    menu->setPosition(CCPointZero);
    
    layerShield = MyShieldLayer::create(_s_width, _s_height, CCPointZero);
    layerShield->addChild(label_sure, 1);
    layerShield->addChild(menu, 0);
    layerShield->addChild(label_win);
    this->addChild(layerShield, kGameOrderShield);
}


