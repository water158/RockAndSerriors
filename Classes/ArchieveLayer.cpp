//
//  ArchieveLayer.cpp
//  RockPaperScissors
//
//  Created by mini01 on 14-5-5.
//
//

#include "ArchieveLayer.h"
#include "C2DXShareSDK/C2DXShareSDK.h"

USING_NS_CC_EXT;
using namespace cn::sharesdk;

//分享结果回调
void shareResultHandler(C2DXResponseState state, C2DXPlatType platType, CCDictionary *shareInfo, CCDictionary *error)
{
    switch (state) {
        case C2DXResponseStateSuccess:
            CCLog("分享成功");
            break;
        case C2DXResponseStateFail:
            CCLog("分享失败");
            break;
        default:
            break;
    }
}

CCScene* AchieveLayer::scene()
{
    CCScene* scene =  CCScene::create();
    
    AchieveLayer* layer = AchieveLayer::create();
    
    scene->addChild(layer);
    
    return scene;
}

void AchieveLayer::onEnter()
{
    CCLayer::onEnter();
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 1, false);
}

bool AchieveLayer::init()
{
    if (!CCLayer::init())
    {
        return false;
    }
    //开启触摸
    this->setTouchEnabled(true);
    
    pXmlpareser = XMLParser::parseWithFile("userdata/loading.xml");
 
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    //添加主背景颜色层
    CCLayerColor* layerColorBG = CCLayerColor::create(ccc4(251, 248, 241, 255));
    this->addChild(layerColorBG,0);
    
    //添加游戏图标 和flag
    CCSprite* flag = CCSprite::create("gameui/gameTittle.png");
    flag->setPosition(ccp(visibleSize.width / 2, visibleSize.height - 20 - flag->getContentSize().height / 2));
    this->addChild(flag);
    
    //添加返回和分享按钮
    CCMenuItemImage* btnBack = CCMenuItemImage::create("gameui/btnBackBG.png", "gameui/btnBackBG.png",this,menu_selector(AchieveLayer::backCallback));
    btnBack->setPosition(ccp(20 + btnBack->getContentSize().width / 2, 20 + btnBack->getContentSize().height / 2));
    //添加返回字符
    CCLabelTTF* labelBack = CCLabelTTF::create(pXmlpareser->getString("menu_back")->getCString(), "Arial", 40);
    labelBack->setPosition(btnBack->getPosition());
    this->addChild(labelBack,1);
    
    CCMenuItemImage* btnShare = CCMenuItemImage::create("gameui/btnShare.png", "gameui/btnShare.png",this,menu_selector(AchieveLayer::shareCallback));
    btnShare->setPosition(ccp(visibleSize.width - 20 - btnShare->getContentSize().width / 2, 20 + btnShare->getContentSize().height / 2));
    //添加分享字符
    CCLabelTTF* labelShare = CCLabelTTF::create(pXmlpareser->getString("menu_share")->getCString(), "Arial", 40);
    labelShare->setPosition(btnShare->getPosition());
    this->addChild(labelShare,1);
    
    MyMenu* menu = MyMenu::create(1.5,btnBack,btnShare,NULL);
    menu->setAnchorPoint(CCPointZero);
    menu->setPosition(CCPointZero);
    this->addChild(menu,0);
    isMenuEnabled = true;
    
    scrollView = CCScrollView::create();
    CCLayer* containerLayer = CCLayer::create();
    
    //设置显示的区域
    scrollView->setViewSize(visibleSize);
    scrollView->setContentOffset(CCPointZero);
    containerLayer->setContentSize(CCSizeMake(visibleSize.width * 8, visibleSize.height));
    
    //设置滑动区域的实际大小
    scrollView->setContentSize(CCSizeMake(visibleSize.width * 8, visibleSize.height));
    scrollView->setContainer(containerLayer);
    //初始化当前页数
    currentPage = 0;
    //自己实现触摸
    scrollView->setTouchEnabled(true);
    scrollView->setDirection(kCCScrollViewDirectionHorizontal);
    scrollView->setDelegate(this);
    this->addChild(scrollView);
    //添加首页目录界面
    /*
    CCNode* content = getContentNode();
    content->setPosition(CCPointZero);
    containerLayer->addChild(content);
     */
    contentlayer = CCLayer::create();
    contentlayer->setContentSize(visibleSize);
    contentlayer->setPosition(CCPointZero);
    containerLayer->addChild(contentlayer,10086);
    //为目录层添加目录节点
    for (int i = 3; i <= 9; i++)
    {
        CCNode* content =  getContentNode(i);
        content->setPosition(ccp(visibleSize.width/2, visibleSize.height * (12 - i) * 0.09f - 80));
        //设置tag值为该成就详细界面的page值
        content->setTag(i - 2);
        contentlayer->addChild(content);
    }
    
    //添加9个难度的成就详细界面
    for (int i = 3; i <= 9; i++)
    {
        AchievePage* page = AchievePage::createWithDifficulty(i);
        page->setPosition(ccp((i - 2) * visibleSize.width, 0));
        containerLayer->addChild(page);
    }
    
    return true;
}

void AchieveLayer::scrollViewDidScroll(extension::CCScrollView *view)
{
    return;
}

void AchieveLayer::scrollViewDidZoom(extension::CCScrollView *view)
{
    return;
}

CCNode* AchieveLayer::getContentNode(int diff)
{
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    
    CCNode* content = CCNode::create();

    content->setContentSize(CCSize(500, 64));
    content->setAnchorPoint(ccp(0.5, 0.5));
    //向节点添加难度标志
    CCString* str = CCString::createWithFormat("%d X %d",diff,diff);
    CCLabelTTF* labelDiff = CCLabelTTF::create(str->getCString(), "Arial", 40);
    labelDiff->setColor(ccRED);
    labelDiff->setPosition(ccp(labelDiff->getContentSize().width/2,labelDiff->getContentSize().height / 2 + 5));
    content->addChild(labelDiff);
    //添加石头标志
    //获取该难度下石头的通过次数
    CCSprite* iconRock = NULL;
    
    //string key_rock = RankDifficulty::makeKey(diff, eRock, 1);
    int times = UserData::sharedUserData()->getTimesByKey(diff, eRock);
    if (times < BRONZE_MEDAL)
    {
        iconRock = CCSprite::create("gameui/btnRock.png");
    }
    else if (times < SILVER_MEDAL)
    {
        iconRock = CCSprite::create("gameui/rock_bronze.png");
    }
    else if (times < GOLD_MEDAL)
    {
        iconRock = CCSprite::create("gameui/rock_silver.png");
    }
    else if (times < PLATINUM_MEDAL)
    {
        iconRock = CCSprite::create("gameui/rock_gold.png");
    }
    else
    {
        iconRock = CCSprite::create("gameui/rock_platinum.png");
    }
    iconRock->setPosition(ccp(150, 32));
    content->addChild(iconRock);
    
    //添加剪刀标志
    //获取该难度下剪刀通过次数
    //string key_scissors = RankDifficulty::makeKey(diff, eScissors, 1);
    CCSprite* iconScissors = NULL;
    times = UserData::sharedUserData()->getTimesByKey(diff, eScissors);
    if (times < BRONZE_MEDAL)
    {
        iconScissors = CCSprite::create("gameui/btnScissors.png");
    }
    else if (times < SILVER_MEDAL)
    {
        iconScissors = CCSprite::create("gameui/scissors_bronze.png");
    }
    else if (times < GOLD_MEDAL)
    {
        iconScissors = CCSprite::create("gameui/scissors_silver.png");
    }
    else if (times < PLATINUM_MEDAL)
    {
        iconScissors = CCSprite::create("gameui/scissors_gold.png");
    }
    else
    {
        iconScissors = CCSprite::create("gameui/scissors_platinum.png");
    }
    iconScissors->setPosition(ccp(300, 32));
    content->addChild(iconScissors);
    
    //添加布标志
    //添加该难度下布通过的次数
    //string key_paper = RankDifficulty::makeKey(diff, ePaper, 1);
    CCSprite* iconPaper = NULL;
    times = UserData::sharedUserData()->getTimesByKey(diff, ePaper);
    if (times < BRONZE_MEDAL)
    {
        iconPaper = CCSprite::create("gameui/btnPaper.png");
    }
    else if (times < SILVER_MEDAL)
    {
        iconPaper = CCSprite::create("gameui/paper_bronze.png");
    }
    else if (times < GOLD_MEDAL)
    {
        iconPaper = CCSprite::create("gameui/paper_silver.png");
    }
    else if (times < PLATINUM_MEDAL)
    {
        iconPaper = CCSprite::create("gameui/paper_gold.png");
    }
    else
    {
        iconPaper = CCSprite::create("gameui/paper_platinum.png");
    }
    iconPaper->setPosition(ccp(450, 32));
    content->addChild(iconPaper);
    
    return content;
}


void AchieveLayer::adjustScrollView(CCPoint beginOffset,CCPoint endOffset)
{
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();

    int nPage = abs((int) beginOffset.x / (int)visibleSize.width);
    
    int nAdjustPage = 0;
    
    int nDis = endOffset.x - beginOffset.x;
    
    if (nDis < -visibleSize.width / 5)
    {
        nAdjustPage = nPage + 1;
    }
    else if (nDis > visibleSize.width / 5)
    {
        nAdjustPage = nPage - 1;
    }
    else
    {
        nAdjustPage = nPage;
    }
    //确保值不能超出范围
    nAdjustPage = MIN(nAdjustPage, 7);
    
    nAdjustPage = MAX(nAdjustPage, 0);
    
    scrollToPage(nAdjustPage , 0.2f);
}

int AchieveLayer::getCurrentPage()
{
    return currentPage;
}

bool AchieveLayer::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    m_beginOffset = scrollView->getContentOffset();
    return true;
}

void AchieveLayer::ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{

}

void AchieveLayer::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    CCPoint endOffset = scrollView->getContentOffset();
    CCPoint pos = pTouch->getLocation();
    
    if (m_beginOffset.equals(endOffset) && currentPage == 0)
    {
        CCLayer* containerLayer = (CCLayer*)scrollView->getContainer();
        for (int i = 1; i <= 7; i++)
        {
            CCNode* node = (CCNode*)contentlayer->getChildByTag(i);
            if (node->boundingBox().containsPoint(pos))
            {
                scrollToPage(i , 0);
            }
        }
        return;
    }
    adjustScrollView(m_beginOffset,endOffset);
}

void AchieveLayer::ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    CCPoint endOffset = scrollView->getContentOffset();
    adjustScrollView(m_beginOffset,endOffset);
}

void AchieveLayer::scrollToPage(int nPage,float delay)
{
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    //关闭ccscrollview中的自调整
    scrollView->unscheduleAllSelectors();
    CCPoint oOffset = scrollView->getContentOffset();
    // 调整位置
    CCPoint adjustPos=ccp(-visibleSize.width * nPage,0);
    
    // 调整动画时间
    float adjustAnimDelay = delay;
    
    moveTime = adjustAnimDelay + 0.2f;
    //开始移动前将按钮可点击状态设置为false
    isMenuEnabled = false;
    // 调整位置
    scrollView->setContentOffsetInDuration(adjustPos, adjustAnimDelay);
    //开启定时器
    //this->scheduleUpdate();
    //将当前页数设置为page
    currentPage = nPage;
}

void AchieveLayer::update(float dt)
{
    /*
    curTime += dt;
    if (curTime >= moveTime)
    {
        isMenuEnabled = true;
        curTime = 0;
        this->unscheduleUpdate();
    }
     */
}

void AchieveLayer::backCallback(cocos2d::CCObject *pSender)
{
    //播放点击音效
    GSoundManager::sharedSoundManager()->playSound(SOUND_CLICK);
    CCDirector::sharedDirector()->popScene();
}

void AchieveLayer::shareCallback(cocos2d::CCObject *pSender)
{
    //获取当前屏幕截图
    CCSize size = CCDirector::sharedDirector()->getVisibleSize();
    CCRenderTexture* screen = CCRenderTexture::create(size.width, size.height);
    CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
    screen->begin();
    scene->visit();   //访问当前运行场景
    screen->end();
    screen->saveToFile("ShareScene.png",kCCImageFormatPNG);
    //获取当前场景图片的存放路径
    std::string fullpath = CCFileUtils::sharedFileUtils()->getWritablePath() + "ShareScene.png";
    
    //播放点击音效
    GSoundManager::sharedSoundManager()->playSound(SOUND_CLICK);
    
    CCDictionary *content = CCDictionary::create();
    content -> setObject(CCString::create("这是一条测试内容"), "content");
    content -> setObject(CCString::create(fullpath.c_str()), "image");
    content -> setObject(CCString::create("布布锤心"), "title");
    content -> setObject(CCString::create("测试描述"), "description");
    content -> setObject(CCString::create("www.baidu.com"), "url");
    //content -> setObject(CCString::createWithFormat("%d", C2DXContentTypeNews), "type");
    content -> setObject(CCString::createWithFormat("%d", C2DXContentTypeImage), "type");
    content -> setObject(CCString::create("baidu.com"), "siteUrl");
    content -> setObject(CCString::create("www"), "site");
    content -> setObject(CCString::create("http://mp3.mwap8.com/destdir/Music/2009/20090601/ZuiXuanMinZuFeng20090601119.mp3"), "musicUrl");
    content -> setObject(CCString::create("extInfo"), "extInfo");
    
    C2DXShareSDK::showShareMenu(NULL, content, CCPointMake(100, 100), C2DXMenuArrowDirectionLeft, shareResultHandler);
}







