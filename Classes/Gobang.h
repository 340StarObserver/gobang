#ifndef Kirito_Gobang_h
#define Kirito_Gobang_h
namespace Obito
{
	class Gobang
	{
	private:
		enum  //类中常量
		{
			m_vacant=8, //空位     所使用的ASCIi码
			m_black=4,  //黑棋     所使用的ASCIi码
			m_white=79, //白棋     所使用的ASCIi码
			m_M=15      //默认棋盘规格是15*15
		};
	private:
		char m_qipan[m_M][m_M];       //棋盘
		double m_computer[m_M][m_M];  //电脑方棋型表(电脑落子决策时的攻击性表,即每一个位置的攻击效果)
		double m_player[m_M][m_M];    //玩家方棋型表(电脑落子决策时的防御性表,即每一个位置的防守效果)
		int m_current_x,m_current_y;  //当前落点
		int m_current_numbers;        //当期棋盘上总棋子数
		double m_k1,m_k2;             //电脑的攻防配置(攻击系数,防御系数)
	private:   //私有实现
		void origin_luozi();           //当棋盘上总棋子数<=3时,电脑在当前落点附近随机落子
		bool will_win(int &x,int &y);  //判断是否存在空位是直接制胜点(即落在该处立刻出现5连)
		bool will_fail(int &x,int &y); //判断是否存在空位是直接制败点(即该处是敌方的直接制胜点)

		void fill_m_computer(); //计算填写电脑方棋型表
		void fill_m_player();   //计算填写玩家方棋型表
		void calc_qixing_data(int i,int j,char qizi_color,int data[4][2][5]); //计算空位(i,j)周围棋型数据
		void calc_qixing_value(int i,int j,char qizi_color,const int data[4][2][5]);
		//为qizi_color所代表的棋型表的(i,j)处赋上棋型效果值(qizi_color填m_black or m_white)

		int single_live_four(int i,int j,char qizi_color);//返回某空位(i,j)处有几条方向上:将可以达到单活四(放上此点后)
		int single_live_three(int i,int j,char qizi_color);//返回某空位(i,j)处有几条方向上:将可以达到单活三(放上此点后)
		int single_dead_four(int i,int j,char qizi_color);//返回某空位(i,j)处有几条方向上:将可以达到单死四(放上此点后)
		bool double_live_four(int &x,int &y,char qizi_color);         //判断是否存在双活四
		bool live_four_and_live_three(int &x,int &y,char qizi_color); //判断是否存在活三四
		bool double_dead_four(int &x,int &y,char qizi_color);         //判断是否存在双死四
		bool dead_four_and_live_three(int &x,int &y,char qizi_color); //判断是否存在死四活三
		bool live_four(int &x,int &y,char qizi_color);                //判断是否存在单活四
		bool double_live_three(int &x,int&y,char qizi_color);         //判断是否存在双活三
	public:
		Gobang();
		~Gobang();
		Gobang(const Gobang &t);
		Gobang & operator = (const Gobang &t);
		//接口函数:
		void reset_m_k1k2(double k1=0.6,double k2=0.4); //设置电脑的攻击系数和防御系数
		int get_m_current_x()const;       //返回当前落点的行标∈[0,m_M-1]
		int get_m_current_y()const;       //返回当前落点的列标∈[0,m_M-1]
		int get_m_current_numbers()const; //返回当前棋子总数
		int get_m_M()const;               //返回棋盘的规格
		double get_m_k1()const;           //返回攻击系数
		double get_m_k2()const;           //返回防御系数
		//主要功能:
		int judge_winorfail()const;         //判断[胜,负,平,未分状态]
		bool player_luozi(int x=0,int y=0); //玩家落子
		void computer_luozi();              //电脑落子
	};
}
#endif