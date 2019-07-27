#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "GameLayer.h"

using namespace cocos2d;
using namespace CocosDenshion;

void HelloWorld::onEnter()
{
    CCLayer::onEnter();
}

void HelloWorld::onExit()
{
    CCLayer::onExit();
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();
    
    //设定移动方式为后移动的时候为false  前移动的时候为true
    CCUserDefault::sharedUserDefault()->setBoolForKey("isMoveTypeForward", false);
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
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
    
    CCSprite* stepBG = CCSprite::create("gameui/stepFrame.png");
    stepBG->setPosition(ccp(visibleSize.width - 20 - stepBG->getContentSize().width / 2, visibleSize.height * 0.79f));
    this->addChild(stepBG);
    
    labelStep = CCLabelTTF::create("0", "Arial", 50);
    labelStep->setPosition(stepBG->getPosition());
    this->addChild(labelStep);
    
    numDifficulty = UserData::sharedUserData()->getSelectDifficulty();
    
    createCardSprite(visibleSize);
    
    CCLog("size width : %f,height : %f",visibleSize.width,visibleSize.height);
    
    //用于友盟统计进入关卡
    //MobClickCpp::startLevel(getStringFromInt(numDifficulty).c_str());
    
    CCSprite* bDiffBG = CCSprite::create("gameui/btnStartBG.png");
    bDiffBG->setPosition(ccp(20 + bDiffBG->getContentSize().width / 2, visibleSize.height * 0.78f));
    this->addChild(bDiffBG);
    
    CCString* str = CCString::createWithFormat("%d X %d",numDifficulty,numDifficulty);
    
    labelDifficulty = CCMenuItemFont::create(str->getCString(), this, menu_selector(HelloWorld::changeDifficulty));
    labelDifficulty->setPosition(bDiffBG->getPosition());
    
    CCSprite* bStartBG = CCSprite::create("gameui/btnStartBG.png");
    bStartBG->setPosition(ccp(30 + bDiffBG->getContentSize().width + bDiffBG->getPositionX(), visibleSize.height * 0.78f));
    this->addChild(bStartBG);
    
    btnStart = CCMenuItemFont::create("start",this,menu_selector(HelloWorld::startCallback));
    btnStart->setPosition(bStartBG->getPosition());
    
    //添加成就按钮
    CCMenuItemImage* btnRock = CCMenuItemImage::create("gameui/btnRock.png", "gameui/btnRock.png",this,menu_selector(HelloWorld::achiveCallback));
    
    btnRock->setPosition(ccp(btnRock->getContentSize().width/2 + 20, visibleSize.height - 35 - btnRock->getContentSize().height/2 ));
    
    CCMenuItemImage* btnPaper = CCMenuItemImage::create("gameui/btnPapper.png", "gameui/btnPapper.png",this,menu_selector(HelloWorld::achiveCallback));
    
    btnPaper->setPosition(ccp(btnRock->getPositionX() + 20 + btnPaper->getContentSize().width , visibleSize.height-35-btnPaper->getContentSize().height/2 ));
    
    CCMenuItemImage* btnScissors = CCMenuItemImage::create("gameui/btnScissors.png", "gameui/btnScissors.png",this,menu_selector(HelloWorld::achiveCallback));
    
    btnScissors->setPosition(ccp(btnPaper->getPositionX()+20+btnPaper->getContentSize().width , visibleSize.height-35-btnScissors->getContentSize().height/2 ));
    
    //添加配置按钮
    CCMenuItemImage* btnConfig = CCMenuItemImage::create("gameui/btnMenu.png", "gameui/btnMenu.png",this,menu_selector(HelloWorld::configCallback));
    
    btnConfig->setPosition(ccp(visibleSize.width - btnConfig->getContentSize().width / 2  - 20, visibleSize.height - 40 - btnConfig->getContentSize().height/2 ));
    CCLabelTTF* labelConfig = CCLabelTTF::create(pXmlparser->getString("menu_config")->getCString(),"Arial",32 );
    labelConfig->setPosition(ccp(btnConfig->getContentSize().width/2,btnConfig->getContentSize().height/2));
    btnConfig->addChild(labelConfig);
    
    //添加更改移动方式按钮
    CCMenuItemFont* changeMoveType = CCMenuItemFont::create("MoveBehind", this, menu_selector(HelloWorld::changeMoveType));
    changeMoveType->setColor(ccRED);
    changeMoveType->setPosition(ccp(visibleSize.width / 2, visibleSize.height * 0.82));
    
    MyMenu* menu = MyMenu::create(1.5,labelDifficulty,btnStart,btnRock,btnPaper,btnScissors, btnConfig,changeMoveType, NULL);
    menu->setAnchorPoint(CCPointZero);
    menu->setPosition(CCPointZero);
    this->addChild(menu);
    
    
    
    //创建2张自动生成卡牌
    autoCreateCardNumber();
    autoCreateCardNumber();
    return true;
}

void HelloWorld::changeMoveType(CCObject* pSender)
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

void HelloWorld::configCallback(cocos2d::CCObject *pSender)
{
    //播放点击音效
    GSoundManager::sharedSoundManager()->playSound(SOUND_CLICK);
    
    CCScene* pScene = ConfigureLayer::scene();
    
    CCDirector::sharedDirector()->pushScene(pScene);
    
}

void HelloWorld::achiveCallback(cocos2d::CCObject *pSender)
{
    //播放点击音效
    GSoundManager::sharedSoundManager()->playSound(SOUND_CLICK);
    CCScene* pScene = AchieveLayer::scene();
    CCDirector::sharedDirector()->pushScene(pScene);
}

void HelloWorld::startCallback(cocos2d::CCObject *pSender)
{
    CCLog("start call back");
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

void HelloWorld::changeDifficulty(cocos2d::CCObject *pSender)
{
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
bool HelloWorld::ccTouchBegan(cocos2d::CCTouch *touch, cocos2d::CCEvent *unused_event)
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
void HelloWorld::ccTouchEnded(cocos2d::CCTouch *touch, cocos2d::CCEvent *unused_event)
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
                autoCreateCardNumber();
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
                autoCreateCardNumber();
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
            if (isdo) {
                stepCount++;
                labelStep->setString(CCString::createWithFormat("%d",stepCount)->getCString());
                
                autoCreateCardNumber();
                doCheckGameOver();
            }
        }
        else if(endY - 40 > 0)
        {
            //向上
            bool isdo = doUp();
            if (isdo) {
                stepCount++;
                labelStep->setString(CCString::createWithFormat("%d",stepCount)->getCString());
                autoCreateCardNumber();
                doCheckGameOver();
            }
        }
        
    }
}

void HelloWorld::ShiftByDir(Shift dir)
{
    switch (dir)
    {
        case eLeft:
            for (int y=0; y<numDifficulty; y++)
            {
                for (int x = 0; x<numDifficulty; x++)
                {
                    for (int x1 = x + 1; x1<numDifficulty; x1++)
                    {
                        if (cardArr[x1][y]->getNumber() != NOTHING)
                        {
                            if (cardArr[x][y]->getNumber() == NOTHING)
                            {
                                cardArr[x][y]->setNumber(cardArr[x1][y]->getNumber(), false, false);
                                cardArr[x1][y]->setNumber(NOTHING, false, false);
                                isCreated = true;
                            }
                        }
                    }
                }
            }
            break;
        case eRight:
            for (int y = 0; y < numDifficulty; y++)
            {
                for (int x = numDifficulty - 1; x >= 0; x--)
                {
                    for (int x1 = x - 1; x1 >= 0; x1--)
                    {
                        if (cardArr[x1][y]->getNumber() != NOTHING)
                        {
                            if (cardArr[x][y]->getNumber() == NOTHING)
                            {
                                cardArr[x][y]->setNumber(cardArr[x1][y]->getNumber(), false, false);
                                cardArr[x1][y]->setNumber(NOTHING, false, false);
                                isCreated = true;
                            }
                        }
                    }
                }
            }
            break;
        case eUp:
            for (int x = 0; x < numDifficulty; x++)
            {
                for (int y = numDifficulty - 1; y >= 0; y--)
                {
                    for (int y1 = y - 1; y1 >= 0; y1--)
                    {
                        if (cardArr[x][y1]->getNumber() != NOTHING)
                        {
                            if (cardArr[x][y]->getNumber() == NOTHING)
                            {
                                cardArr[x][y]->setNumber(cardArr[x][y1]->getNumber(), false, false);
                                cardArr[x][y1]->setNumber(NOTHING, false, false);
                                isCreated = true;
                            }
                        }
                    }
                }
            }
            break;
        case eDown:
            for (int x = 0; x < numDifficulty; x++)
            {
                for (int y = 0; y < numDifficulty; y++)
                {
                    for (int y1 = y + 1; y1 < numDifficulty; y1++)
                    {
                        if (cardArr[x][y1]->getNumber() != NOTHING)
                        {
                            if (cardArr[x][y]->getNumber() == NOTHING)
                            {
                                cardArr[x][y]->setNumber(cardArr[x][y1]->getNumber(), false, false);
                                cardArr[x][y1]->setNumber(NOTHING, false, false);
                                isCreated = true;
                            }
                        }
                    }
                }
            }
            break;
        default:
            break;
    }
}

void HelloWorld::KillByDir(Shift dir)
{
    switch (dir)
    {
        case eLeft:
            for (int y=0; y<numDifficulty; y++)
            {
                for (int x = numDifficulty-1; x>=0; x--)
                {
                    int x1 = x-1;
                    if(x1>=0)
                    {
                        if (((cardArr[x][y]->getNumber() - cardArr[x1][y]->getNumber())) == ROCK - SCISSOS)
                        {
                            cardArr[x1][y]->setNumber(2,true,false);
                            cardArr[x][y]->setNumber(0,false,false);
                            isCreated = true;
                            x--;
                        }
                        else if (cardArr[x][y]->getNumber() != 0 && (cardArr[x][y]->getNumber() - cardArr[x1][y]->getNumber()) == SCISSOS - PAPPER)
                        {
                            cardArr[x1][y]->setNumber(5,true,false);
                            cardArr[x][y]->setNumber(0,false,false);
                            isCreated = true;
                            x--;
                        }
                        else if (((cardArr[x][y]->getNumber() - cardArr[x1][y]->getNumber())) == PAPPER - ROCK)  //-8
                        {
                            cardArr[x1][y]->setNumber(10,true,false);
                            cardArr[x][y]->setNumber(0,false,false);
                            isCreated = true;
                            x--;
                        }
                    }
                }
            }
            break;
        case eRight:
            for (int y = 0; y < numDifficulty; y++)
            {
                for (int x = 0; x < numDifficulty; x++)
                {
                    int x1 = x + 1;
                    if (x1 <= numDifficulty - 1)
                    {
                        if (((cardArr[x][y]->getNumber() - cardArr[x1][y]->getNumber())) == ROCK - SCISSOS)
                        {
                            cardArr[x1][y]->setNumber(ROCK,true,false);
                            cardArr[x][y]->setNumber(NOTHING,false,false);
                            isCreated = true;
                            x++;
                        }
                        else if (cardArr[x][y]->getNumber() != 0 && (cardArr[x][y]->getNumber() - cardArr[x1][y]->getNumber()) == SCISSOS - PAPPER)
                        {
                            cardArr[x1][y]->setNumber(SCISSOS,true,false);
                            cardArr[x][y]->setNumber(NOTHING,false,false);
                            isCreated = true;
                            x++;
                        }
                        else if (((cardArr[x][y]->getNumber() - cardArr[x1][y]->getNumber())) == PAPPER - ROCK)  //-8
                        {
                            cardArr[x1][y]->setNumber(PAPPER,true,false);
                            cardArr[x][y]->setNumber(NOTHING,false,false);
                            isCreated = true;
                            x++;
                        }
                    }
                }
            }
            break;
        case eUp:
            for (int x = 0; x < numDifficulty; x++)
            {
                for (int y = 0; y < numDifficulty; y++)
                {
                    int y1 = y + 1;
                    if (y1 < numDifficulty)
                    {
                        if (((cardArr[x][y]->getNumber() - cardArr[x][y1]->getNumber())) == ROCK - SCISSOS)
                        {
                            cardArr[x][y]->setNumber(ROCK,true,false);
                            cardArr[x][y1]->setNumber(NOTHING,false,false);
                            isCreated = true;
                            y++;
                        }
                        else if (cardArr[x][y]->getNumber() != 0 && (cardArr[x][y]->getNumber() - cardArr[x][y1]->getNumber()) == SCISSOS - PAPPER)
                        {
                            cardArr[x][y]->setNumber(SCISSOS,true,false);
                            cardArr[x][y1]->setNumber(NOTHING,false,false);
                            isCreated = true;
                            y++;
                        }
                        else if (((cardArr[x][y]->getNumber() - cardArr[x][y1]->getNumber())) == PAPPER - ROCK)  //-8
                        {
                            cardArr[x][y]->setNumber(PAPPER,true,false);
                            cardArr[x][y1]->setNumber(NOTHING,false,false);
                            isCreated = true;
                            y++;
                        }
                    }
                }
            }
            break;
        case eDown:
            for (int x = 0; x < numDifficulty; x++)
            {
                for (int y = numDifficulty - 1; y >= 0; y--)
                {
                    int y1 = y - 1;
                    if (y1 >= 0)
                    {
                        if (((cardArr[x][y]->getNumber() - cardArr[x][y1]->getNumber())) == ROCK - SCISSOS)
                        {
                            cardArr[x][y1]->setNumber(ROCK,true,false);
                            cardArr[x][y]->setNumber(NOTHING,false,false);
                            isCreated = true;
                            y--;
                        }
                        else if (cardArr[x][y]->getNumber() != 0 && (cardArr[x][y]->getNumber() - cardArr[x][y1]->getNumber()) == SCISSOS - PAPPER)
                        {
                            cardArr[x][y1]->setNumber(SCISSOS,true,false);
                            cardArr[x][y]->setNumber(NOTHING,false,false);
                            isCreated = true;
                            y--;
                        }
                        else if (((cardArr[x][y]->getNumber() - cardArr[x][y1]->getNumber())) == PAPPER - ROCK)  //-8
                        {
                            cardArr[x][y1]->setNumber(PAPPER,true,false);
                            cardArr[x][y]->setNumber(NOTHING,false,false);
                            isCreated = true;
                            y--;
                        }
                    }
                }
            }
            break;
        default:
            break;
    }
}

//滑向上下左右的方法
bool HelloWorld::doUp()
{
    CCLog("doUp");
    
    ShiftByDir(eUp);
    KillByDir(eUp);
    ShiftByDir(eUp);
    return isCreated;
}

bool HelloWorld::doDown()
{
    CCLog("doDown");
    
    ShiftByDir(eDown);
    KillByDir(eDown);
    ShiftByDir(eDown);
    return isCreated;
}

bool HelloWorld::doLeft()
{
    CCLog("doLeft");
    //先做移动
    ShiftByDir(eLeft);
    //再作消除
    KillByDir(eLeft);
    //再作移动
    ShiftByDir(eLeft);
    return isCreated;
}

bool HelloWorld::doRight()
{
    CCLog("doRight");
    //先做移动
    ShiftByDir(eRight);
    //再作消除
    KillByDir(eRight);
    //再作移动
    ShiftByDir(eRight);
    
    return isCreated;
}

void HelloWorld::ccTouchMoved(cocos2d::CCTouch *touch, cocos2d::CCEvent *unused_event)
{
    
}

void HelloWorld::ccTouchCancelled(cocos2d::CCTouch *touch, cocos2d::CCEvent *unused_event)
{
    
}

void HelloWorld::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}

void HelloWorld::createCardSprite(cocos2d::CCSize size)
{
    int uniteSize = (size.width - 48) / numDifficulty;
    
    for (int i=0; i<numDifficulty; i++)
    {
        for (int j=0;j<numDifficulty; j++)
        {
            CardSprite* card = CardSprite::createCardSprite(0, uniteSize, uniteSize, uniteSize*i + 30, uniteSize*j + _s_height * 0.1);
            card->difficulty = numDifficulty;
            cardArr[i][j] = card;
            this->addChild(card);
        }
    }
}

void HelloWorld::autoCreateCardNumber()
{
    int i=CCRANDOM_0_1()*numDifficulty;
    int j = CCRANDOM_0_1()*numDifficulty;
    
    //判断是否这个位置已存在卡牌
    if (cardArr[i][j]->getNumber()>0)
    {
        autoCreateCardNumber();
    }
    else
    {
        
        float randNum = CCRANDOM_0_1();
        
        if (randNum < 0.33f)
        {
            
            cardArr[i][j]->setNumber(ROCK,false,true);
        }
        else if(randNum > 0.66f)
        {
            
            cardArr[i][j]->setNumber(SCISSOS,false,true);
        }
        else
        {
            
            cardArr[i][j]->setNumber(PAPPER,false,true);
        }
    }
    //将是否产生置为false
    isCreated = false;
}


void HelloWorld::doCheckGameOver()
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
        CCMenuItemImage* winBG = CCMenuItemImage::create("gameui/stepFrame.png","gameui/stepFrame.png",this,menu_selector(HelloWorld::sureCallback));
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

void HelloWorld::sureCallback(cocos2d::CCObject *pSender)
{
    this->removeChildByTag(10086);
}
