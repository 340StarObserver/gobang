#include "HelloWorldScene.h"
#include "GameScene.h"
#include "SourceStore.h"
Scene* HelloWorld::create()
{
	auto scene=new HelloWorld();
	if(!scene->init())
	{
		delete scene;
		return nullptr;
	}
	else
	{
		scene->autorelease();
		return scene;
	}
}
bool HelloWorld::init()
{
    if ( !Scene::init() )
        return false;
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
	//创建退出按钮
    auto closeItem = MenuItemImage::create(
                                           CloseNormalPng,
                                           CloseSelectedPng,
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));
    auto closemenu = Menu::create(closeItem, NULL);
    closemenu->setPosition(Vec2::ZERO);
    this->addChild(closemenu, 1);
	//创建底图
	auto sprite = Sprite::create(HelloWorldPng);
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    this->addChild(sprite, 0);
	//创建标签按钮(进入游戏,退出游戏)
	auto label_1=Label::create("Enter Game","宋体",28);
	auto label_2=Label::create("Exit Game","宋体",28);
	auto menuItem_1=MenuItemLabel::create(label_1,CC_CALLBACK_1(HelloWorld::menuCloseCallback,this));
	menuItem_1->setPosition(Vec2(120,150));
	menuItem_1->setTag(1);
	auto menuItem_2=MenuItemLabel::create(label_2,CC_CALLBACK_1(HelloWorld::menuCloseCallback,this));
	menuItem_2->setPosition(Vec2(120,100));
	menuItem_2->setTag(2);
	Vector<MenuItem *> vect;
	vect.pushBack(menuItem_1);
	vect.pushBack(menuItem_2);
	auto menu=Menu::createWithArray(vect);
	menu->setPosition(Point::ZERO);
	this->addChild(menu,1);
    return true;
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
	int tag=dynamic_cast<Node *>(pSender)->getTag();
	if(tag==1)
	{
		//若按到Enter Game标签按钮,则切换到游戏场景
		auto scene=GameScene::create();
		auto transition=TransitionPageTurn::create(0.5,scene,true);
		Director::getInstance()->replaceScene(transition);
	}
	else
	{
		#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
    	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
        return;
        #endif
		Director::getInstance()->end();
        #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		exit(0);
        #endif
	}
}
