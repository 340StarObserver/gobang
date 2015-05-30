#ifndef _PINGSCENE_H
#define _PINGSCENE_H
#include "cocos2d.h"
USING_NS_CC;
class PingScene : public cocos2d::Scene
{//Æ½¾Ö³¡¾°:
public:
	static cocos2d::Scene* create();
	virtual bool init();
	void menuCloseCallback(cocos2d::Ref* pSender);
};
#endif