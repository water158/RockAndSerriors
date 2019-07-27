#ifndef _MYPINGBILAYER_H_
#define _MYPINGBILAYER_H_

#include "cocos2d.h"
using namespace cocos2d;
class MyShieldLayer : public CCLayer
{
public:
	MyShieldLayer();
	bool init();
	static MyShieldLayer *create(float width, float height,CCPoint bpos);
	virtual bool ccTouchBegan(CCTouch *pTouches, CCEvent *pEvent);
	void destroy();
	void setColorLayer(ccColor3B color,unsigned char opacity);

	float layerWidth;
	float layerHeight;
	CCPoint begainPos;
};


#endif