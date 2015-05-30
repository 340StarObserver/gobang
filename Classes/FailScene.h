#ifndef _FAILSCENE_H
#define _FAILSCENE_H
#include "cocos2d.h"
USING_NS_CC;
class FailScene : public cocos2d::Scene
{//Ê§°Ü³¡¾°:
public:
	static cocos2d::Scene* create();
	virtual bool init();
	void menuCloseCallback(cocos2d::Ref* pSender);
};
#endif