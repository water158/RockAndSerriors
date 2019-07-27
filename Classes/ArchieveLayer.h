//
//  ArchieveLayer.h
//  RockPaperScissors
//
//  Created by mini01 on 14-5-5.
//
//

#ifndef __RockPaperScissors__ArchieveLayer__
#define __RockPaperScissors__ArchieveLayer__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "Tools.h"
#include "AchievePage.h"
#include "MyXmlParser.h"
#include "GsoundManager.h"
#include "MyMenu.h"

using namespace cocos2d;

class AchieveLayer : public CCLayer,public extension::CCScrollViewDelegate
{
public:
    virtual void scrollViewDidScroll(extension::CCScrollView *view);
    virtual void scrollViewDidZoom(extension::CCScrollView* view);
public:
    virtual bool init();
    CREATE_FUNC(AchieveLayer);
    
    static CCScene* scene();    
    //触摸函数
    virtual bool ccTouchBegan(CCTouch* pTouch,CCEvent* pEvent);
    virtual void ccTouchMoved(CCTouch* pTouch,CCEvent* pEvent);
    virtual void ccTouchEnded(CCTouch* pTouch,CCEvent* pEvent);
    virtual void ccTouchCancelled(CCTouch* pTouch,CCEvent* pEvent);
    
    virtual void adjustScrollView(CCPoint beginOffest,CCPoint endOffset);
    virtual void onEnter();
    //返回回调
    void backCallback(CCObject* pSender);
    //分享回调
    void shareCallback(CCObject* pSender);
    //创建目录节点
    CCNode* getContentNode(int diff);
    //滚动到指定的页数
    void scrollToPage(int nPage,float delay);
    //获取当前页数
    int getCurrentPage();
    //计时器用于控制按钮的点击时间
    void update(float dt);
    
private:
    extension::CCScrollView* scrollView;
    //开始offset
    CCPoint m_beginOffset;
    //当前页数
    int currentPage;
    //按钮是否可点击
    bool isMenuEnabled;
    //scrollvew的滚动时间
    float moveTime;
    //滚动时间的标尺
    float curTime;
    //目录层
    CCLayer* contentlayer;
    
    XMLParser* pXmlpareser;
};


#endif /* defined(__RockPaperScissors__ArchieveLayer__) */
