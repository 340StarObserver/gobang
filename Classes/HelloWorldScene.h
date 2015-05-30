#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__
#include "cocos2d.h"
USING_NS_CC;
class HelloWorld : public cocos2d::Scene
{
public:
    static cocos2d::Scene* create();
    virtual bool init();
    void menuCloseCallback(cocos2d::Ref* pSender);
};
#endif 
