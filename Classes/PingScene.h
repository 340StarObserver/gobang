#ifndef _PINGSCENE_H
#define _PINGSCENE_H
#include "cocos2d.h"
USING_NS_CC;
class PingScene : public cocos2d::Scene
{//ƽ�ֳ���:
public:
	static cocos2d::Scene* create();
	virtual bool init();
	void menuCloseCallback(cocos2d::Ref* pSender);
};
#endif