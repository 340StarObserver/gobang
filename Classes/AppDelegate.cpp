#include "AppDelegate.h"
#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
AppDelegate::AppDelegate() {
}
AppDelegate::~AppDelegate() 
{
}
void AppDelegate::initGLContextAttrs()
{
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};
    GLView::setGLContextAttrs(glContextAttrs);
}
bool AppDelegate::applicationDidFinishLaunching() {
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::create("Gobang Game");
        director->setOpenGLView(glview);
    }
    director->setAnimationInterval(1.0 / 60);
	auto scene=HelloWorld::create();
    director->runWithScene(scene);
    return true;
}
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();
	CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
	
}
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();
	CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
