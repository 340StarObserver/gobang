#ifndef _GAMESCENE_H
#define _GAMESCENE_H
#include "cocos2d.h"
#include "Gobang.h"
#include <string>
USING_NS_CC;
class GameScene : public cocos2d::Scene
{
private:
	enum{ defaultSize=15 };           //Ĭ�ϵ����̹����15*15
	Obito::Gobang gobang;             //ʹ��Gobang����--������Ϸ�ڲ��߼�
	int remainingTime;                //���ʣ������ʱ��
	int blackNum;                     //��ǰ������
	int whiteNum;                     //��ǰ������
	void luozi(cocos2d::Ref* pSender);//��������
	bool player_luozi(int i,int j);   //�������(i,j)Ϊѡ�еİ�ť��λ��
	void computer_luozi();            //��������
	void createTimeLabel();           //��������ʱ��ʾ��
	void updateTimeLabel(float dt);   //���µ���ʱ��ʾ��
	std::string numToString(int num)const;  //������ת��Ϊ�ַ���
	void updateBlackNumLabel();       //���º�������ʾ��
	void updateWhiteNumLabel();       //���°�������ʾ��
	void updateTotalNumLabel();       //������������ʾ��
	void updateExtension();           //���½�����
public:
	static cocos2d::Scene* create();
	virtual bool init();
	void menuCloseCallback(cocos2d::Ref* pSender);
};
#endif