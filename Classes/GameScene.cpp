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
	//�����˳���ť
	auto closeItem=MenuItemImage::create(CloseNormalPng,CloseSelectedPng,
		CC_CALLBACK_1(GameScene::menuCloseCallback,this));
		closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));
    auto closemenu = Menu::create(closeItem, NULL);
    closemenu->setPosition(Vec2::ZERO);
    this->addChild(closemenu,1);
	//four sprites:
	auto sprite_1=Sprite::create(BlackNumPng);   //������ ͼ��
	auto sprite_2=Sprite::create(WhiteNumPng);   //������ ͼ��
	auto sprite_3=Sprite::create(TotalNumPng);   //������ ͼ��
	auto sprite_4=Sprite::create(RemainTimePng); //ʣ���������� ͼ��
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
	//�������Ӱ�ť:
	int size=(int)defaultSize*(int)defaultSize;
	auto batchnode_1=CCSpriteBatchNode::create(QiziNormalPng,size);   //��λû�б�����ʱ��ͼ��
	auto batchnode_2=CCSpriteBatchNode::create(QiziSelectedPng,size); //��λ�����µ�ʱ���ͼ��
	//����������ÿ����λ�����Ӱ�ť
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
	//��������ʱ���岢���õ�����:
	createTimeLabel();
	//����һЩ����:
	updateBlackNumLabel();
	updateWhiteNumLabel();
	updateTotalNumLabel();
	//��������������������:
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
{//���½����ϵ�ĳ����λ---���Ӵ�������
	//���Ȼ�ð��µ���λ��tag���
	int tag=dynamic_cast<Node *>(pSender)->getTag();
	//���ð��µ���λ��tag��������㴥���İ�ť��λ��(i,j):
	int i=tag/defaultSize;
	int j=tag-i*defaultSize;
	if(player_luozi(i,j))    
	{
		//��������ӳɹ�,���ֵ�����������
		computer_luozi();
		//���������Ӻ�,�ֵ��������,��ʱҪ���� ��ҵ�ʣ������ʱ��Ϊ60��
		createTimeLabel();
	}
}
bool GameScene::player_luozi(int i,int j)
{
	//���̽����(x,y)�ǻ���cocos2dx����ϵ��,����ӳ�䵽��ά���������(i,j)��һ������任
	//(x,y)��Ӧ��ά�������е�(size-1-y,x)
	if(!gobang.player_luozi(defaultSize-1-j,i))
		return false;//�ô���������,����ʧ��,�����ֲ�������
	//��������ӳɹ�,����ʱɾ�� ʣ������ʱ�� �������ʱ
	this->removeChildByName("timeLabel");
	//����������ʱ�ĵ�����
	this->unschedule(schedule_selector(GameScene::updateTimeLabel));
	//��������ӵĽ����ϵ�λ�÷���һ�����徫��
	float x=50+40*i;
   	float y=50+40*j;
	auto sprite_1=Sprite::create(BlackChessPng);
	sprite_1->setPosition(x,y);
	this->addChild(sprite_1,3);//���ú���
	//Ȼ����½����ϵĺ�����Ŀ��ʾ��ǩ
	updateBlackNumLabel();
	//�������̵�ǰ�������Ľ�����
	updateExtension();
	//��ִ����������ӵ����в����Ժ�,Ҫ�жϵ�ǰ��ֵ�ʤ�������:
	int k=gobang.judge_winorfail();
	if(k==1) //���Ӯ��
	{
		//������ʤλ�õ���˸���� �� �л���ʤ�������Ķ��� ����
		auto blink=Blink::create(2.5,5);
		auto sequence=Sequence::create(blink,CallFunc::create(
			[](){
				auto scene=WinScene::create();
		       	auto transition=TransitionFadeBL::create(1.5,scene);
		       	Director::getInstance()->replaceScene(transition);
		}),NULL);
		//����ʤλ�õ����Ӿ���ȥִ�������������
		sprite_1->runAction(sequence);
		return false;//�ѷ�ʤ��,����Ҳ��������(��  void GameScene::luozi(Ref*)  )
	}
	else if(k==0)//ƽ��
	{
		//�л���ƽ�ֳ���
		auto scene=PingScene::create();
		auto transition=TransitionPageTurn::create(1.5,scene,false);
		Director::getInstance()->replaceScene(transition);
		return false;//�ѳ�ƽ��,����Ҳ��������
	}
	return true;//������ӳɹ���δ��ʤ��,���ֵ���������(��  void GameScene::luozi(Ref*)  )
}
void GameScene::computer_luozi()
{
	//ʹ��Gobang����������������ӵ��ڲ��߼�
	gobang.computer_luozi();
	//��õ������ӵ�λ��(i,j)--�����ϵ�ԭ�������·�
	//get_m_current_x, get_m_current_y�Ƕ�ά�����е����к�
	int i=gobang.get_m_current_y();
	int j=defaultSize-1-gobang.get_m_current_x();
	//����������Ӷ�Ӧ�������ϵ�����
	float x=50+40*i;
   	float y=50+40*j;
	//�ڽ���Ķ�Ӧ���괦���һ�����徫��
	auto sprite_2=Sprite::create(WhiteChessPng);
	sprite_2->setPosition(x,y);
	this->addChild(sprite_2,3);
	//���½����ϵİ�����Ŀ��ǩ
	updateWhiteNumLabel();
	//���½����ϵ���������ǩ
	updateExtension();
	//��ִ����������ӵ����в����,���жϵ�ǰ��ֵ�ʤ�������:
	int k=gobang.judge_winorfail();
	if(k==-1)//����Ӯ��
	{
		//�����ưܵ���˸ �� �л���ʧ�ܳ��� �Ķ�������
		auto blink=Blink::create(2.5,5);
		auto sequence=Sequence::create(blink,CallFunc::create(
			[](){
				auto scene=FailScene::create();
		       	auto transition=TransitionMoveInT::create(1.5,scene);
		       	Director::getInstance()->replaceScene(transition);
		}),NULL);
		//���ưܵ����� ȥִ�� �����������
		sprite_2->runAction(sequence);
		return;
	}
	else if(k==0)//ƽ��
	{
		//�л���ƽ�ֳ���
		auto scene=PingScene::create();
		auto transition=TransitionPageTurn::create(1.5,scene,false);
		Director::getInstance()->replaceScene(transition);
		return;
	}
}
std::string GameScene::numToString(int num)const
{
	//�˴���num���������̹��15*15
	std::string str;
	if(num>=100)       //num��[100,255]
	{
		char temp[4];
		temp[2]=num%10+'0'; num=num/10;
		temp[1]=num%10+'0'; num=num/10;
		temp[0]=num%10+'0'; 
		temp[3]='\0';
		str=temp;
	}
	else if(num>=10)   //num��[10,99]
	{
		char temp[3];
		temp[1]=num%10+'0';
		temp[0]=num/10+'0';
		temp[2]='\0';
		str=temp;
	}
	else               //num��[0,9]
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
	//���� ���ʣ�����������ĵ���ʱ ��ǩ
	remainingTime=60;
	std::string str=numToString(remainingTime);
	auto timeLabel=LabelTTF::create(str,"����",25);
	timeLabel->setName("timeLabel");
	Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
	float x0=origin.x+visibleSize.width*0.85;
	float y0=origin.y+visibleSize.height*0.85;
	timeLabel->setPosition(x0,y0-300);
	this->addChild(timeLabel,1);
	//��������ʱ������,ÿ1�����һ�µ���ʱ��ǩ
	this->schedule(schedule_selector(GameScene::updateTimeLabel),1);
}
void GameScene::updateTimeLabel(float dt)
{
	//���µ���ʱ��ǩ
	remainingTime--;
	if(remainingTime>0)
	{
		//����ʣ��ʱ��,��ʣ������-1
		//�������Ƴ�ԭ��ǩ,�ٴ���һ���±�ǩ���ڳ�����
		this->removeChildByName("timeLabel");
		std::string str=numToString(remainingTime);
		auto timeLabel=LabelTTF::create(str,"����",25);
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
		//�����ʣ������ʱ��Ϊ0,����Ϊ�ûغ������Ȩ����
		//����ɾ�� ����ʱ��ǩ,����ֹ����ʱ������
		this->removeChildByName("timeLabel");
		this->unschedule(schedule_selector(GameScene::updateTimeLabel));
		//Ȼ���õ���ִ��һ������
		computer_luozi();
		//����������,���ֵ����������,��ʱ�ٴ��� ����ʱ��ǩ
		createTimeLabel();
	}
}
void GameScene::updateBlackNumLabel()
{
	//���º�������ǩ
	blackNum++;
	this->removeChildByName("blackNumLabel");
	Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
	float x0=origin.x+visibleSize.width*0.85;
	float y0=origin.y+visibleSize.height*0.85;
	std::string str=numToString(blackNum);
	auto blackNumLabel=LabelTTF::create(str,"����",25);
	blackNumLabel->setName("blackNumLabel");
	blackNumLabel->setPosition(x0,y0);
	this->addChild(blackNumLabel,1);
	updateTotalNumLabel();
}
void GameScene::updateWhiteNumLabel()
{
	//���°�������ǩ
	whiteNum++;
	this->removeChildByName("whiteNumLabel");
	Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
	float x0=origin.x+visibleSize.width*0.85;
	float y0=origin.y+visibleSize.height*0.85;
	std::string str=numToString(whiteNum);
	auto whiteNumLabel=LabelTTF::create(str,"����",25);
	whiteNumLabel->setName("whiteNumLabel");
	whiteNumLabel->setPosition(x0,y0-100);
	this->addChild(whiteNumLabel,1);
	updateTotalNumLabel();
}
void GameScene::updateTotalNumLabel()
{
	//������������ǩ
	this->removeChildByName("totalNumLabel");
	Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
	float x0=origin.x+visibleSize.width*0.85;
	float y0=origin.y+visibleSize.height*0.85;
	std::string str=numToString(blackNum+whiteNum);
	auto totalNumLabel=LabelTTF::create(str,"����",25);
	totalNumLabel->setName("totalNumLabel");
	totalNumLabel->setPosition(x0,y0-200);
	this->addChild(totalNumLabel,1);
}
void GameScene::updateExtension()
{
	//������Ϸ������ ��Ϸ�����뵱ǰ�����ϵ������� ������
	float total=(float)defaultSize*(float)defaultSize;
	float percent=((float)(blackNum+whiteNum))/total*100;
	auto gameExtension=this->getChildByName("gameExtension");
	((ProgressTimer *)gameExtension)->setPercentage(percent);
}