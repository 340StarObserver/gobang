#ifndef _GAMESCENE_H
#define _GAMESCENE_H
#include "cocos2d.h"
#include "Gobang.h"
#include <string>
USING_NS_CC;
class GameScene : public cocos2d::Scene
{
private:
	enum{ defaultSize=15 };           //默认的棋盘规格是15*15
	Obito::Gobang gobang;             //使用Gobang对象--代理游戏内部逻辑
	int remainingTime;                //玩家剩余落子时间
	int blackNum;                     //当前黑棋数
	int whiteNum;                     //当前白棋数
	void luozi(cocos2d::Ref* pSender);//触发落子
	bool player_luozi(int i,int j);   //玩家落子(i,j)为选中的按钮的位置
	void computer_luozi();            //电脑落子
	void createTimeLabel();           //创建倒计时显示板
	void updateTimeLabel(float dt);   //更新倒计时显示板
	std::string numToString(int num)const;  //将数字转化为字符串
	void updateBlackNumLabel();       //更新黑棋数显示板
	void updateWhiteNumLabel();       //更新白棋数显示板
	void updateTotalNumLabel();       //更新总棋数显示板
	void updateExtension();           //更新进度条
public:
	static cocos2d::Scene* create();
	virtual bool init();
	void menuCloseCallback(cocos2d::Ref* pSender);
};
#endif