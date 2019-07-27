//
//  GameLayer.cpp
//  RockPaperScissors
//
//  Created by mini01 on 14-5-4.
//
//

#include "GameLayer.h"
#include "HelloWorldScene.h"

GameLayer::GameLayer()
{

}

GameLayer::~GameLayer()
{

}

CCScene* GameLayer::scene()
{
    CCScene* temp_scene = CCScene::create();
    
    GameLayer* layer = GameLayer::create();
    
	temp_scene->addChild(layer, 0);

	return temp_scene;
}

GameLayer* GameLayer::create()
{
	GameLayer* layer = new GameLayer();

	if(layer && layer->init())
	{
		layer->autorelease();
		return layer;
	}
	return NULL;
}

bool GameLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    //this->registerWithTouchDispatcher();
    this->setTouchEnabled(true);
      
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    
    pXmlparser = XMLParser::parseWithFile("userdata/loading.xml");
    
    //初始化随机种子
    srand((unsigned)time(NULL));
        
    CCLayerColor* laycolorBg = CCLayerColor::create(ccc4(251, 248, 241, 255), visibleSize.width, visibleSize.height);
    this->addChild(laycolorBg);
    
    stepCount = 0;
    CCSprite* stepBG = CCSprite::create("stepFrame.png");
    stepBG->setPosition(ccp(visibleSize.width - 20 - stepBG->getContentSize().width / 2, visibleSize.height * 0.79f));
    this->addChild(stepBG);
    
    labelStep = CCLabelTTF::create("0", "Arial", 50);
    labelStep->setPosition(stepBG->getPosition());
    this->addChild(labelStep);
    //获取用户选择的难度
    numDifficulty = UserData::sharedUserData()->getSelectDifficulty();
    //CCLog("%d++++++++++++++++++++++",numDifficulty);
    CCLog("width:%f,height:%f",visibleSize.width,visibleSize.height);


    createCardSprite(visibleSize);

    CCLog("1");
    //用于友盟统计进入关卡
    //MobClickCpp::startLevel(getStringFromInt(numDifficulty).c_str());
    
    CCLog("2");
    //播放背景音乐
    GSoundManager::sharedSoundManager()->playBackgroundMisc(BGMUSIC);

    CCLog("3");
    
    CCString* str = CCString::createWithFormat("%d X %d",numDifficulty,numDifficulty);
    
    CCSprite* bDiffBG = CCSprite::create("btnStartBG.png");
    bDiffBG->setPosition(ccp(20 + bDiffBG->getContentSize().width / 2, visibleSize.height * 0.78f));
    this->addChild(bDiffBG);
    
    labelDifficulty = CCMenuItemFont::create(str->getCString(), this, menu_selector(GameLayer::changeDifficulty));
    labelDifficulty->setPosition(bDiffBG->getPosition());
    
    CCSprite* bStartBG = CCSprite::create("btnStartBG.png");
    bStartBG->setPosition(ccp(30 + bDiffBG->getContentSize().width + bDiffBG->getPositionX(), visibleSize.height * 0.78f));
    this->addChild(bStartBG);
    
    btnStart = CCMenuItemFont::create(pXmlparser->getString("menu_start")->getCString(),this,menu_selector(GameLayer::startCallback));
    btnStart->setPosition(bStartBG->getPosition());
    
    //添加成就按钮
    CCMenuItemImage* btnRock = CCMenuItemImage::create("gameui/btnRock.png", "gameui/btnRock.png",this,menu_selector(GameLayer::achiveCallback));
    
    btnRock->setPosition(ccp(btnRock->getContentSize().width/2 + 20, visibleSize.height - 35 - btnRock->getContentSize().height/2 ));
    
    CCMenuItemImage* btnPaper = CCMenuItemImage::create("gameui/btnPapper.png", "gameui/btnPapper.png",this,menu_selector(GameLayer::achiveCallback));
    
    btnPaper->setPosition(ccp(btnRock->getPositionX() + 20 + btnPaper->getContentSize().width , visibleSize.height-35-btnPaper->getContentSize().height/2 ));
    
    CCMenuItemImage* btnScissors = CCMenuItemImage::create("gameui/btnScissors.png", "gameui/btnScissors.png",this,menu_selector(GameLayer::achiveCallback));
    
    btnScissors->setPosition(ccp(btnPaper->getPositionX()+20+btnPaper->getContentSize().width , visibleSize.height-35-btnScissors->getContentSize().height/2 ));
    
    //添加配置按钮
    CCMenuItemImage* btnConfig = CCMenuItemImage::create("gameui/btnMenu.png", "gameui/btnMenu.png",this,menu_selector(GameLayer::configCallback));
    
    btnConfig->setPosition(ccp(visibleSize.width - btnConfig->getContentSize().width / 2  - 20, visibleSize.height - 40 - btnConfig->getContentSize().height/2 ));
    CCLabelTTF* labelConfig = CCLabelTTF::create(pXmlparser->getString("menu_config")->getCString(),"Arial",32 );
    labelConfig->setPosition(ccp(btnConfig->getContentSize().width/2,btnConfig->getContentSize().height/2));
    btnConfig->addChild(labelConfig);
    
    //添加更改移动方式按钮
    CCMenuItemFont* changeMoveType = CCMenuItemFont::create("MoveForward", this, menu_selector(HelloWorld::changeMoveType));
    changeMoveType->setColor(ccRED);
    changeMoveType->setPosition(ccp(visibleSize.width / 2, visibleSize.height * 0.82));
    
    MyMenu* menu = MyMenu::create(1.5,labelDifficulty,btnStart,btnRock,btnPaper,btnScissors, btnConfig,changeMoveType, NULL);
    menu->setAnchorPoint(CCPointZero);
    menu->setPosition(CCPointZero);
    this->addChild(menu);

    CCLog("4");
    
    //创建2张自动生成卡牌
    autoCreateCardNumber();
    autoCreateCardNumber();

    CCLog("5");
    return true;
}

void GameLayer::changeMoveType(CCObject* pSender)
{
    CCLog("change move type is clicked!");
    
    CCMenuItemFont* pMenu = (CCMenuItemFont*)pSender;
    
    if (CCUserDefault::sharedUserDefault()->getBoolForKey("isMoveTypeForward"))
    {
        CCLabelTTF* pLabel = CCLabelTTF::create("MoveBehind", "Arial", 40);
        pLabel->setColor(ccRED);
        pMenu->setLabel(pLabel);
        CCUserDefault::sharedUserDefault()->setBoolForKey("isMoveTypeForward", false);
    }
    else
    {
        CCLabelTTF* pLabel = CCLabelTTF::create("MoveForward", "Arial", 40);
        pLabel->setColor(ccRED);
        pMenu->setLabel(pLabel);
        CCUserDefault::sharedUserDefault()->setBoolForKey("isMoveTypeForward", true);
    }
}

void GameLayer::configCallback(cocos2d::CCObject *pSender)
{
    //播放点击音效
    GSoundManager::sharedSoundManager()->playSound(SOUND_CLICK);
    
    CCScene* pScene = ConfigureLayer::scene();
    
    CCDirector::sharedDirector()->pushScene(pScene);
    
}

void GameLayer::achiveCallback(cocos2d::CCObject *pSender)
{
    //播放点击音效
    GSoundManager::sharedSoundManager()->playSound(SOUND_CLICK);
    CCScene* pScene = AchieveLayer::scene();
    CCDirector::sharedDirector()->pushScene(pScene);
}

void GameLayer::startCallback(cocos2d::CCObject *pSender)
{
    //播放点击音效
    GSoundManager::sharedSoundManager()->playSound(SOUND_CLICK);
    CCScene* scene = (CCScene*)this->getParent();
    if (scene)
    {
        this->removeFromParentAndCleanup(true);
        if (CCUserDefault::sharedUserDefault()->getBoolForKey("isMoveTypeForward"))
        {
            GameLayer* layer = GameLayer::create();
            scene->addChild(layer);
        }
        else
        {
            HelloWorld* layer = HelloWorld::create();
            scene->addChild(layer);
        }
    }
}

void GameLayer::changeDifficulty(cocos2d::CCObject *pSender)
{
    CCLog("change difficulty call back!");
    //播放点击音效
    GSoundManager::sharedSoundManager()->playSound(SOUND_CLICK);
    int dif = UserData::sharedUserData()->getSelectDifficulty();
    //将难度系数+1  如果难度系数大于9 则从3开始
    dif++;
    if (dif > 9)
    {
        dif = 3;
    }
    //将难度系数显示在界面上 并保存到userdata
    CCString* str = CCString::createWithFormat("%d X %d",dif,dif);
    CCLabelTTF* label = CCLabelTTF::create(str->getCString(), "",30);
    labelDifficulty->setLabel(label);
    UserData::sharedUserData()->setSelectDifficulty(dif);
    UserData::sharedUserData()->save();
}

//事件监听回调：触摸开始
bool GameLayer::ccTouchBegan(cocos2d::CCTouch *touch, cocos2d::CCEvent *unused_event)
{
    //获取触摸的X轴和Y轴
    CCPoint touchPoint = touch->getLocation(); //获取OpenGL坐标（即cocos2d-x坐标，原点在左下角）
    touch->getLocationInView();
    firstX=touchPoint.x;
    firstY=touchPoint.y;
    
    CCLog("x:%d,y:%d",firstX,firstY);
    
    return true;
}

//事件监听回调：触摸结束
void GameLayer::ccTouchEnded(cocos2d::CCTouch *touch, cocos2d::CCEvent *unused_event)
{
    //获取X轴和Y轴的移动范围
    CCPoint touchPoint=touch->getLocation(); //获取OpenGL坐标（即cocos2d-x坐标，原点在左下角）
    endX=touchPoint.x - firstX;
    endY=touchPoint.y - firstY;
    
    //判断X轴和Y轴的移动距离，如果X轴的绝对值大，则向左右滑动，如果Y轴的绝对值大，则向上下滑动
    if(abs(endX) > abs(endY) + 40)
    {
        //手势向左右
        //判断向左还是向右
        if(endX+40<0)
        {
            //向左
            bool isdo = doLeft();
            if (isdo) {
                stepCount++;
                labelStep->setString(CCString::createWithFormat("%d",stepCount)->getCString());
                doCheckGameOver();
            }
        }
        else if(endX - 40 > 0)
        {
            //向右
            bool isdo = doRight();
            if (isdo) {
                stepCount++;
                labelStep->setString(CCString::createWithFormat("%d",stepCount)->getCString());
                doCheckGameOver();
            }
        }
    }
    else
    {
        //手势向上下
        //判断手势向上还是向下
        if(endY+40<0)
        {
            //向下
            bool isdo = doDown();
            if (isdo)
            {
                //播放点击音效
                GSoundManager::sharedSoundManager()->playSound(SOUND_CLICK);
                stepCount++;
                labelStep->setString(CCString::createWithFormat("%d",stepCount)->getCString());
                doCheckGameOver();
                
            }
        }
        else if(endY - 40 > 0)
        {
            //向上
            bool isdo = doUp();
            if (isdo)
            {
                //播放点击音效
                GSoundManager::sharedSoundManager()->playSound(SOUND_CLICK);
                stepCount++;
                labelStep->setString(CCString::createWithFormat("%d",stepCount)->getCString());
                doCheckGameOver();
            }
        }
        
    }
}

//滑向上下左右的方法
bool GameLayer::doUp()
{
    CCLog("doUp");
    bool isdo = false;
    //移动的是 剪刀石头  2 和 5
    bool isMove2 = false;
    //移动的是 剪刀和布  5 和 10
    bool isMove5 = false;
    //移动的是 石头和布  2 和 10
    bool isMove10 = false;
    for (int x = 0; x < numDifficulty; x++) {
        for (int y = numDifficulty-1; y >= 0; y--) {
            
            for (int y1 = y - 1; y1 >= 0; y1--) {
                if (cardArr[x][y1]->getNumber() > 0) {
                    if (cardArr[x][y]->getNumber() <= 0) {
                        cardArr[x][y]->setNumber(cardArr[x][y1]->getNumber(),false,false);
                        cardArr[x][y1]->setNumber(0,false,false);
                        y++;
                        isdo = true;
                    }
                    else
                    {
                        if ((cardArr[x][y]->getNumber()-cardArr[x][y1]->getNumber()) == 3 ) {
                            cardArr[x][y]->setNumber(2,true,false);
                            cardArr[x][y1]->setNumber(0,false,false);
                            isdo = true;
                            isMove2 = true;
                        }
                        else if ((cardArr[x][y]->getNumber()-cardArr[x][y1]->getNumber()) == 5)
                        {
                            cardArr[x][y]->setNumber(5,true,false);
                            cardArr[x][y1]->setNumber(0,false,false);
                            isdo = true;
                            isMove5 = true;
                        }
                        else if ((cardArr[x][y]->getNumber()-cardArr[x][y1]->getNumber()) == -8)
                        {
                            cardArr[x][y]->setNumber(10,true,false);
                            cardArr[x][y1]->setNumber(0,false,false);
                            isdo = true;
                            isMove10 = true;
                        }
                    }
                    break;
                }
            }
            
        }
    }
    //如果有消除 则随机生成卡片
    if (isdo)
    {
        autoCreateCardNumber();
    }
    return isdo;
}

bool GameLayer::doDown()
{
    CCLog("doDown");
    bool isdo = false;
    //移动的是 剪刀石头  2 和 5
    bool isMove2 = false;
    //移动的是 剪刀和布  5 和 10
    bool isMove5 = false;
    //移动的是 石头和布  2 和 10
    bool isMove10 = false;
    for (int x = 0; x < numDifficulty; x++) {
        for (int y = 0; y < numDifficulty; y++) {
            
            for (int y1 = y + 1; y1 < numDifficulty; y1++) {
                if (cardArr[x][y1]->getNumber() > 0) {
                    if (cardArr[x][y]->getNumber() <= 0) {
                        cardArr[x][y]->setNumber(cardArr[x][y1]->getNumber(),false,false);
                        cardArr[x][y1]->setNumber(0,false,false);
                        y--;
                        isdo = true;
                    }
                    else
                    {
                        if ((cardArr[x][y]->getNumber()-cardArr[x][y1]->getNumber()) == 3 ) {
                            cardArr[x][y]->setNumber(2,true,false);
                            cardArr[x][y1]->setNumber(0,false,false);
                            isMove2 = true;
                            isdo = true;
                        }
                        else if ((cardArr[x][y]->getNumber()-cardArr[x][y1]->getNumber()) == 5)
                        {
                            cardArr[x][y]->setNumber(5,true,false);
                            cardArr[x][y1]->setNumber(0,false,false);
                            isMove5 = true;
                            isdo = true;
                        }
                        else if ((cardArr[x][y]->getNumber()-cardArr[x][y1]->getNumber()) == -8)
                        {
                            cardArr[x][y]->setNumber(10,true,false);
                            cardArr[x][y1]->setNumber(0,false,false);
                            isMove10 = true;
                            isdo = true;
                        }
                    }
                    break;
                }
            }
            
        }
    }
    //如果有消除 则随机生成卡片
    if (isdo)
    {
        autoCreateCardNumber();
    }
    return isdo;
}

bool GameLayer::doLeft()
{
    CCLog("doLeft");
    bool isdo = false;
    //移动的是 剪刀石头  2 和 5
    bool isMove2 = false;
    //移动的是 剪刀和布  5 和 10
    bool isMove5 = false;
    //移动的是 石头和布  2 和 10
    bool isMove10 = false;
    for (int y = 0; y< numDifficulty; y++)
    {
        for (int x = 0; x < numDifficulty; x++)
        {
            for (int x1 = x+1; x1<numDifficulty; x1++)
            {
                if (cardArr[x1][y]->getNumber() > 0)
                {
                    if (cardArr[x][y]->getNumber()<=0)
                    {
                        cardArr[x][y]->setNumber(cardArr[x1][y]->getNumber(),false,false);
                        cardArr[x1][y]->setNumber(0,false,false);
                        x--;
                        isdo = true;
                    }
                    else
                    {
                        if ((cardArr[x][y]->getNumber()-cardArr[x1][y]->getNumber()) == 3 ) {
                            cardArr[x][y]->setNumber(2,true,false);
                            cardArr[x1][y]->setNumber(0,false,false);
                            isMove2 = true;
                            isdo = true;
                        }
                        else if ((cardArr[x][y]->getNumber()-cardArr[x1][y]->getNumber()) == 5)
                        {
                            cardArr[x][y]->setNumber(5,true,false);
                            cardArr[x1][y]->setNumber(0,false,false);
                            isMove5 = true;
                            isdo = true;
                        }
                        else if ((cardArr[x][y]->getNumber()-cardArr[x1][y]->getNumber()) == -8)
                        {
                            cardArr[x][y]->setNumber(10,true,false);
                            cardArr[x1][y]->setNumber(0,false,false);
                            isMove10 = true;
                            isdo = true;
                        }
                    }
                    break;
                }
            }
        }
    }
    //如果有消除  随机生成卡片
    if (isdo)
    {
        autoCreateCardNumber();
    }
    return isdo;
}

bool GameLayer::doRight()
{
    CCLog("doRight");
    bool isdo = false;
    //移动的是 剪刀石头  2 和 5
    bool isMove2 = false;
    //移动的是 剪刀和布  5 和 10
    bool isMove5 = false;
    //移动的是 石头和布  2 和 10
    bool isMove10 = false;
    for (int y = 0; y < numDifficulty; y++) {
        for (int x = numDifficulty-1; x >= 0; x--) {
            
            for (int x1 = x - 1; x1 >= 0; x1--) {
                if (cardArr[x1][y]->getNumber() > 0) {
                    if (cardArr[x][y]->getNumber() <= 0) {
                        cardArr[x][y]->setNumber(cardArr[x1][y]->getNumber(),false,false);
                        cardArr[x1][y]->setNumber(0,false,false);
                        x++;
                        isdo = true;
                    }
                    else
                    {
                        if ((cardArr[x][y]->getNumber()-cardArr[x1][y]->getNumber()) == 3 ) {
                            cardArr[x][y]->setNumber(2,true,false);
                            cardArr[x1][y]->setNumber(0,false,false);
                            isMove2 = true;
                            isdo = true;
                        }
                        else if ((cardArr[x][y]->getNumber()-cardArr[x1][y]->getNumber()) == 5)
                        {
                            cardArr[x][y]->setNumber(5,true,false);
                            cardArr[x1][y]->setNumber(0,false,false);
                            isMove5 = true;
                            isdo = true;
                        }
                        else if ((cardArr[x][y]->getNumber()-cardArr[x1][y]->getNumber()) == -8)
                        {
                            cardArr[x][y]->setNumber(10,true,false);
                            cardArr[x1][y]->setNumber(0,false,false);
                            isMove10 = true;
                            isdo = true;
                        }
                    }
                    
                    break;
                }
            }
            
        }
    }
    //如果有消除 则随机生成新的卡片
    if (isdo)
    {
        autoCreateCardNumber();
    }
    return isdo;
}

void GameLayer::ccTouchMoved(cocos2d::CCTouch *touch, cocos2d::CCEvent *unused_event)
{
    
}

void GameLayer::ccTouchCancelled(cocos2d::CCTouch *touch, cocos2d::CCEvent *unused_event)
{
    
}

void GameLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}

void GameLayer::createCardSprite(cocos2d::CCSize size)
{
    //初始化棋盘
    int uniteSize = (size.width - 28) / numDifficulty;
    
    for (int i=0; i<numDifficulty; i++)
    {
        for (int j=0;j<numDifficulty; j++)
        {
            CardSprite* card = CardSprite::createCardSprite(0, uniteSize, uniteSize, uniteSize*i + 20, uniteSize*j + 20);
            card->difficulty = numDifficulty;
            cardArr[i][j] = card;
            this->addChild(card);
        }
    }
}

void GameLayer::autoCreateCardNumber()
{
    int i = (int)(CCRANDOM_0_1()*1000) % numDifficulty;
    int j = (int)(CCRANDOM_0_1()*1000) % numDifficulty;
   
    //判断是否这个位置已存在卡牌
    if (cardArr[i][j]->getNumber()>0)
    {
        autoCreateCardNumber();
    }
    else
    {
        
        int randNum = (int)(CCRANDOM_0_1()*1000) % 3;
        
        if (randNum == 0)
        {
            
            cardArr[i][j]->setNumber(ROCK,false,true);
        }
        else if(randNum == 1)
        {
            
            cardArr[i][j]->setNumber(SCISSOS,false,true);
        }
        else if(randNum == 2)
        {
            
            cardArr[i][j]->setNumber(PAPPER,false,true);
        }
        
    }
}


void GameLayer::doCheckGameOver()
{
    //判断游戏是否还能继续
    
    bool isGameOver = true;
    
    int value = cardArr[0][0]->getNumber();
    
    for (int y = 0; y < numDifficulty; y++) {
        for (int x = 0; x < numDifficulty; x++) {
            if (value == NOTHING || cardArr[x][y]->getNumber() != value)
            {
                isGameOver = false;
            }
        }
    }
    
    if (isGameOver) {
        //用于友盟统计
        //MobClickCpp::finishLevel(getStringFromInt(numDifficulty).c_str());
        //保存数据
        RankDifficulty* info = new RankDifficulty();
        info->step = stepCount;
        info->date = getCurtime();
        //确定该成就的类型信息
        if (cardArr[0][0]->getNumber() == ROCK)
        {
            info->type = eRock;
        }
        else if (cardArr[0][0]->getNumber() == SCISSOS)
        {
            info->type = eScissors;
        }
        else if (cardArr[0][0]->getNumber() == PAPPER)
        {
            //类型值设置为8 是方便在userdata里面读取和保存时候的循环
            info->type = ePaper;
        }        
        info->difficulty = UserData::sharedUserData()->getSelectDifficulty();
        info->pos = 0;
        int x = UserData::sharedUserData()->addUserRankInfo(info);
        //保存成就通过的次数
        int key = info->difficulty * 100 + info->type * 10 + 1;
        UserData::sharedUserData()->addTimesByKey(key);
        //打印成就通过的次数
        CCLog("have win %d times!",UserData::sharedUserData()->getTimesByKey(key));
        //用于友盟统计
        //MobClickCpp::event(getStringFromInt(numDifficulty * 10 + info->type).c_str());
        CCLog("save result is %d",x);
        //游戏结束，重新开始游戏
        //CCMessageBox("游戏胜利", "end");
        
        pXmlparser = XMLParser::parseWithFile("userdata/loading.xml");
        MyShieldLayer* shielder = MyShieldLayer::create(_s_width, _s_height, CCPointZero);
        CCMenuItemImage* winBG = CCMenuItemImage::create("gameui/stepFrame.png","gameui/stepFrame.png",this,menu_selector(GameLayer::sureCallback));
        winBG->setPosition(ccp(_s_width / 2, _s_height / 2));
        CCLabelTTF* label_sure = CCLabelTTF::create(pXmlparser->getString("menu_sure")->getCString(), "Arial", 30);
        label_sure->setPosition(winBG->getPosition());
        shielder->addChild(label_sure,1);
        //添加游戏胜利提示
        CCLabelTTF* label_win = CCLabelTTF::create(pXmlparser->getString("label_win")->getCString(), "Arial", 40);
        label_win->setPosition(ccp(_s_width / 2,_s_height / 2 + 80));
        label_win->setColor(ccRED);
        shielder->addChild(label_win);
        MyMenu* menu = MyMenu::create(1.5,winBG,NULL);
        menu->setAnchorPoint(CCPointZero);
        menu->setPosition(CCPointZero);
        shielder->addChild(menu,0);
        this->addChild(shielder,0,10086);
        
    }
    
}

void GameLayer::sureCallback(cocos2d::CCObject *pSender)
{
    this->removeChildByTag(10086);
}
