#include "GameScene.h"
#include "WinScene.h"
#include "FailScene.h"
#include "PingScene.h"
#include "SimpleAudioEngine.h"
#include "SourceStore.h"
Scene* GameScene::create()
{
	auto scene=new GameScene();
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
void GameScene::menuCloseCallback(cocos2d::Ref * pSender)
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
bool GameScene::init()
{
	if(!Scene::init())
		return false;
	blackNum=-1;
	whiteNum=-1;
	//background music:
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic(GameMusicMp3.c_str());
	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(GameMusicMp3.c_str(),true);
	//background sprite:
	Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto background=Sprite::create(GameScenePng);
	background->setPosition(Vec2(origin.x+visibleSize.width/2,origin.y+visibleSize.height/2));
	this->addChild(background,0);
	//创建退出按钮
	auto closeItem=MenuItemImage::create(CloseNormalPng,CloseSelectedPng,
		CC_CALLBACK_1(GameScene::menuCloseCallback,this));
		closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));
    auto closemenu = Menu::create(closeItem, NULL);
    closemenu->setPosition(Vec2::ZERO);
    this->addChild(closemenu,1);
	//four sprites:
	auto sprite_1=Sprite::create(BlackNumPng);   //黑棋数 图标
	auto sprite_2=Sprite::create(WhiteNumPng);   //白棋数 图标
	auto sprite_3=Sprite::create(TotalNumPng);   //总棋数 图标
	auto sprite_4=Sprite::create(RemainTimePng); //剩余落子秒数 图标
	float x0=origin.x+visibleSize.width*0.75;
	float y0=origin.y+visibleSize.height*0.85;
	sprite_1->setPosition(x0,y0);
	sprite_2->setPosition(x0,y0-100);
	sprite_3->setPosition(x0,y0-200);
	sprite_4->setPosition(x0,y0-300);
	this->addChild(sprite_1,1);
	this->addChild(sprite_2,1);
	this->addChild(sprite_3,1);
	this->addChild(sprite_4,1);
	//创建棋子按钮:
	int size=(int)defaultSize*(int)defaultSize;
	auto batchnode_1=CCSpriteBatchNode::create(QiziNormalPng,size);   //棋位没有被按下时的图像
	auto batchnode_2=CCSpriteBatchNode::create(QiziSelectedPng,size); //棋位被按下的时候的图像
	//创建棋盘上每个棋位的落子按钮
	for(int i=0;i<defaultSize;i++)
	{
		for(int j=0;j<defaultSize;j++)
		{
			auto menuitem=MenuItemSprite::create(Sprite::createWithTexture(batchnode_1->getTexture()),
				Sprite::createWithTexture(batchnode_2->getTexture()),
				CC_CALLBACK_1(GameScene::luozi,this));
	    	menuitem->setPosition(Vec2(50+40*i,50+40*j));
			menuitem->setTag(defaultSize*i+j);
	    	auto qizimenu=Menu::create(menuitem,NULL);
			qizimenu->setPosition(Point::ZERO);
			this->addChild(qizimenu,2);
		}
	}
	//创建倒计时画板并设置调度器:
	createTimeLabel();
	//创建一些进度:
	updateBlackNumLabel();
	updateWhiteNumLabel();
	updateTotalNumLabel();
	//创建横向棋子数进度条:
	auto gameExtension=ProgressTimer::create(Sprite::create(GameExtensionPng));
	gameExtension->setBarChangeRate(Vec2(1,0));	
	gameExtension->setType(ProgressTimer::Type::BAR);
	gameExtension->setMidpoint(Vec2(0,0));
	gameExtension->setPosition(x0+100,y0-350);
	gameExtension->setPercentage(0.0);
	gameExtension->setName("gameExtension");
	this->addChild(gameExtension,1);
	return true;
}
void GameScene::luozi(cocos2d::Ref* pSender)
{//按下界面上的某个棋位---落子触发函数
	//首先获得按下的棋位的tag标记
	int tag=dynamic_cast<Node *>(pSender)->getTag();
	//利用按下的棋位的tag标记来计算触发的按钮的位置(i,j):
	int i=tag/defaultSize;
	int j=tag-i*defaultSize;
	if(player_luozi(i,j))    
	{
		//若玩家落子成功,则轮到电脑来落子
		computer_luozi();
		//电脑落完子后,轮到玩家落子,这时要更新 玩家的剩余落子时间为60秒
		createTimeLabel();
	}
}
bool GameScene::player_luozi(int i,int j)
{
	//棋盘界面的(x,y)是基于cocos2dx坐标系的,所以映射到二维数组的行列(i,j)有一个坐标变换
	//(x,y)对应二维数组行列的(size-1-y,x)
	if(!gobang.player_luozi(defaultSize-1-j,i))
		return false;//该处已有棋子,落子失败,电脑轮不到落子
	//若玩家落子成功,则暂时删除 剩余落子时间 这个倒计时
	this->removeChildByName("timeLabel");
	//并撤销倒计时的调度器
	this->unschedule(schedule_selector(GameScene::updateTimeLabel));
	//在玩家落子的界面上的位置放置一个黑棋精灵
	float x=50+40*i;
   	float y=50+40*j;
	auto sprite_1=Sprite::create(BlackChessPng);
	sprite_1->setPosition(x,y);
	this->addChild(sprite_1,3);//放置黑棋
	//然后更新界面上的黑棋数目提示标签
	updateBlackNumLabel();
	//更新棋盘当前总棋数的进度条
	updateExtension();
	//当执行完玩家落子的所有步骤以后,要判断当前棋局的胜负情况了:
	int k=gobang.judge_winorfail();
	if(k==1) //玩家赢了
	{
		//创建制胜位置的闪烁动作 和 切换到胜利场景的动作 序列
		auto blink=Blink::create(2.5,5);
		auto sequence=Sequence::create(blink,CallFunc::create(
			[](){
				auto scene=WinScene::create();
		       	auto transition=TransitionFadeBL::create(1.5,scene);
		       	Director::getInstance()->replaceScene(transition);
		}),NULL);
		//让制胜位置的棋子精灵去执行这个动作序列
		sprite_1->runAction(sequence);
		return false;//已分胜负,电脑也不用落子(见  void GameScene::luozi(Ref*)  )
	}
	else if(k==0)//平局
	{
		//切换到平局场景
		auto scene=PingScene::create();
		auto transition=TransitionPageTurn::create(1.5,scene,false);
		Director::getInstance()->replaceScene(transition);
		return false;//已成平局,电脑也不用落子
	}
	return true;//玩家落子成功且未分胜负,则轮到电脑落子(见  void GameScene::luozi(Ref*)  )
}
void GameScene::computer_luozi()
{
	//使用Gobang对象来代理电脑落子的内部逻辑
	gobang.computer_luozi();
	//获得电脑落子的位置(i,j)--界面上的原点在左下方
	//get_m_current_x, get_m_current_y是二维数组中的行列号
	int i=gobang.get_m_current_y();
	int j=defaultSize-1-gobang.get_m_current_x();
	//计算电脑落子对应到界面上的坐标
	float x=50+40*i;
   	float y=50+40*j;
	//在界面的对应坐标处添加一个白棋精灵
	auto sprite_2=Sprite::create(WhiteChessPng);
	sprite_2->setPosition(x,y);
	this->addChild(sprite_2,3);
	//更新界面上的白棋数目标签
	updateWhiteNumLabel();
	//更新界面上的总棋数标签
	updateExtension();
	//当执行完电脑落子的所有步骤后,该判断当前棋局的胜负情况了:
	int k=gobang.judge_winorfail();
	if(k==-1)//电脑赢了
	{
		//创建制败点闪烁 和 切换到失败场景 的动作序列
		auto blink=Blink::create(2.5,5);
		auto sequence=Sequence::create(blink,CallFunc::create(
			[](){
				auto scene=FailScene::create();
		       	auto transition=TransitionMoveInT::create(1.5,scene);
		       	Director::getInstance()->replaceScene(transition);
		}),NULL);
		//让制败点棋子 去执行 这个动作序列
		sprite_2->runAction(sequence);
		return;
	}
	else if(k==0)//平局
	{
		//切换到平局场景
		auto scene=PingScene::create();
		auto transition=TransitionPageTurn::create(1.5,scene,false);
		Director::getInstance()->replaceScene(transition);
		return;
	}
}
std::string GameScene::numToString(int num)const
{
	//此处的num最大就是棋盘规格15*15
	std::string str;
	if(num>=100)       //num∈[100,255]
	{
		char temp[4];
		temp[2]=num%10+'0'; num=num/10;
		temp[1]=num%10+'0'; num=num/10;
		temp[0]=num%10+'0'; 
		temp[3]='\0';
		str=temp;
	}
	else if(num>=10)   //num∈[10,99]
	{
		char temp[3];
		temp[1]=num%10+'0';
		temp[0]=num/10+'0';
		temp[2]='\0';
		str=temp;
	}
	else               //num∈[0,9]
	{
		char temp[2];
		temp[0]=num+'0';
		temp[1]='\0';
		str=temp;
	}
	return str;
}
void GameScene::createTimeLabel()
{
	//创建 玩家剩余落子秒数的倒计时 标签
	remainingTime=60;
	std::string str=numToString(remainingTime);
	auto timeLabel=LabelTTF::create(str,"宋体",25);
	timeLabel->setName("timeLabel");
	Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
	float x0=origin.x+visibleSize.width*0.85;
	float y0=origin.y+visibleSize.height*0.85;
	timeLabel->setPosition(x0,y0-300);
	this->addChild(timeLabel,1);
	//创建倒计时调度器,每1秒更新一下倒计时标签
	this->schedule(schedule_selector(GameScene::updateTimeLabel),1);
}
void GameScene::updateTimeLabel(float dt)
{
	//更新倒计时标签
	remainingTime--;
	if(remainingTime>0)
	{
		//若还剩余时间,则将剩余秒数-1
		//采用先移除原标签,再创建一个新标签挂在场景上
		this->removeChildByName("timeLabel");
		std::string str=numToString(remainingTime);
		auto timeLabel=LabelTTF::create(str,"宋体",25);
		timeLabel->setName("timeLabel");
    	Size visibleSize = Director::getInstance()->getVisibleSize();
        Vec2 origin = Director::getInstance()->getVisibleOrigin();
    	float x0=origin.x+visibleSize.width*0.85;
    	float y0=origin.y+visibleSize.height*0.85;
    	timeLabel->setPosition(x0,y0-300);
    	this->addChild(timeLabel,1);
	}
	else
	{
		//若玩家剩余落子时间为0,则视为该回合玩家弃权落子
		//则先删除 倒计时标签,并终止倒计时调度器
		this->removeChildByName("timeLabel");
		this->unschedule(schedule_selector(GameScene::updateTimeLabel));
		//然后让电脑执行一次落子
		computer_luozi();
		//电脑落子完,该轮到玩家落子了,此时再创建 倒计时标签
		createTimeLabel();
	}
}
void GameScene::updateBlackNumLabel()
{
	//更新黑棋数标签
	blackNum++;
	this->removeChildByName("blackNumLabel");
	Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
	float x0=origin.x+visibleSize.width*0.85;
	float y0=origin.y+visibleSize.height*0.85;
	std::string str=numToString(blackNum);
	auto blackNumLabel=LabelTTF::create(str,"宋体",25);
	blackNumLabel->setName("blackNumLabel");
	blackNumLabel->setPosition(x0,y0);
	this->addChild(blackNumLabel,1);
	updateTotalNumLabel();
}
void GameScene::updateWhiteNumLabel()
{
	//更新白棋数标签
	whiteNum++;
	this->removeChildByName("whiteNumLabel");
	Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
	float x0=origin.x+visibleSize.width*0.85;
	float y0=origin.y+visibleSize.height*0.85;
	std::string str=numToString(whiteNum);
	auto whiteNumLabel=LabelTTF::create(str,"宋体",25);
	whiteNumLabel->setName("whiteNumLabel");
	whiteNumLabel->setPosition(x0,y0-100);
	this->addChild(whiteNumLabel,1);
	updateTotalNumLabel();
}
void GameScene::updateTotalNumLabel()
{
	//更新总棋数标签
	this->removeChildByName("totalNumLabel");
	Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
	float x0=origin.x+visibleSize.width*0.85;
	float y0=origin.y+visibleSize.height*0.85;
	std::string str=numToString(blackNum+whiteNum);
	auto totalNumLabel=LabelTTF::create(str,"宋体",25);
	totalNumLabel->setName("totalNumLabel");
	totalNumLabel->setPosition(x0,y0-200);
	this->addChild(totalNumLabel,1);
}
void GameScene::updateExtension()
{
	//更新游戏进度条 游戏进度与当前棋盘上的总棋数 成正比
	float total=(float)defaultSize*(float)defaultSize;
	float percent=((float)(blackNum+whiteNum))/total*100;
	auto gameExtension=this->getChildByName("gameExtension");
	((ProgressTimer *)gameExtension)->setPercentage(percent);
}