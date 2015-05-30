#include "PingScene.h"
#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "SourceStore.h"
Scene * PingScene::create()
{
	auto scene=new PingScene();
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
bool PingScene::init()
{
	if(!Scene::init())
		return false;
	//background music:
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic(PingMusicMp3.c_str());
	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(PingMusicMp3.c_str(),true);
	//background:
	Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto background=Sprite::create(PingJuPng);
	background->setPosition(Vec2(origin.x+visibleSize.width/2,origin.y+visibleSize.height/2));
	this->addChild(background,0);
	//创建退出按钮
	auto closeItem=MenuItemImage::create(CloseNormalPng,CloseSelectedPng,
		CC_CALLBACK_1(PingScene::menuCloseCallback,this));
		closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));
    auto closemenu = Menu::create(closeItem, NULL);
    closemenu->setPosition(Vec2::ZERO);
    this->addChild(closemenu,1);
	//创建文字标签:
	auto label=Label::create("PingJu>_<","宋体",40);
	label->setPosition(Vec2(origin.x+visibleSize.width/2,origin.y+visibleSize.height/2+50));
	this->addChild(label,1);
	//创建文字按钮--重新游戏:
	auto label_1=Label::create("Game Again","宋体",25);
	auto menuitem_1=MenuItemLabel::create(label_1,CC_CALLBACK_1(PingScene::menuCloseCallback,this));
	menuitem_1->setPosition(origin.x+visibleSize.width/2,origin.y+visibleSize.height/2-50);
	menuitem_1->setTag(1);
	//创建文字按钮--退出游戏:
	auto label_2=Label::create("Exit Game","宋体",25);
	auto menuitem_2=MenuItemLabel::create(label_2,CC_CALLBACK_1(PingScene::menuCloseCallback,this));
	menuitem_2->setPosition(origin.x+visibleSize.width/2,origin.y+visibleSize.height/2-150);
	menuitem_2->setTag(2);
	Vector<MenuItem *> vect;
	vect.pushBack(menuitem_1);
	vect.pushBack(menuitem_2);
	auto menu=Menu::createWithArray(vect);
	menu->setPosition(Point::ZERO);
	this->addChild(menu,1);
	return true;
}
void PingScene::menuCloseCallback(cocos2d::Ref* pSender)
{
	int tag=dynamic_cast<Node *>(pSender)->getTag();
	if(tag==1)
	{
		auto scene=GameScene::create();
		auto transition=TransitionFade::create(1.5,scene);
		Director::getInstance()->replaceScene(transition);
		return;
	}
	#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
    MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
    #endif
	Director::getInstance()->end();
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
    #endif
}