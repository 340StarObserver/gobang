#ifndef _WINSCENE_H
#define _WINSCENE_H
#include "cocos2d.h"
USING_NS_CC;
class WinScene : public cocos2d::Scene
{//Ê¤Àû³¡¾°:
public:
	static cocos2d::Scene* create();
	virtual bool init();
	void menuCloseCallback(cocos2d::Ref* pSender);
};
#endif