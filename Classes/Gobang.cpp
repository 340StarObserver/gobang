#include"Gobang.h"
#include<cmath>
namespace Obito
{
	Gobang::Gobang()
	{
		//默认构造
		//棋盘清零
		for(int i=0;i<m_M;i++)
		{
			for(int j=0;j<m_M;j++)
			{
				m_qipan[i][j]=m_vacant;
				m_computer[i][j]=0;
				m_player[i][j]=0;
			}
		}
		m_current_x=0;
		m_current_y=0;
		//设置默认的电脑攻防系数
		m_k1=0.6;
		m_k2=0.4;
		//当前总棋子数
		m_current_numbers=0;
	}

	Gobang::~Gobang()
	{}

	Gobang::Gobang(const Gobang &t)
	{
		for(int i=0;i<m_M;i++)
		{
			for(int j=0;j<m_M;j++)
			{
				m_qipan[i][j]=t.m_qipan[i][j];
				m_computer[i][j]=t.m_computer[i][j];
				m_player[i][j]=t.m_player[i][j];
			}
		}
		m_current_numbers=t.m_current_numbers;
		m_current_x=t.m_current_x;
		m_current_y=t.m_current_y;
		m_k1=t.m_k1;
		m_k2=t.m_k2;
	}

	Gobang & Gobang::operator= (const Gobang &t)
	{
		for(int i=0;i<m_M;i++)
		{
			for(int j=0;j<m_M;j++)
			{
				m_qipan[i][j]=t.m_qipan[i][j];
				m_computer[i][j]=t.m_computer[i][j];
				m_player[i][j]=t.m_player[i][j];
			}
		}
		m_current_numbers=t.m_current_numbers;
		m_current_x=t.m_current_x;
		m_current_y=t.m_current_y;
		m_k1=t.m_k1;
		m_k2=t.m_k2;
		return *this;
	}

	void Gobang::reset_m_k1k2(double k1,double k2)
	{
		//自定义电脑的攻防系数
		//先进行攻防数据的合法性验证,是否都∈[0,1],且攻防系数之和为1
		if(k1>=0&&k1<=1&&k2>=0&&k2<=1&&std::fabs(k1+k2-1)<0.0001)
		{
			m_k1=k1;
			m_k2=k2;
		}
	}

	//获得当前总棋数
	int Gobang::get_m_current_numbers()const{ return m_current_numbers; }

	//获得棋盘的规格
	int Gobang::get_m_M()const{ return static_cast<int>(m_M); }

	//获得当前落点的行号
	int Gobang::get_m_current_x()const{ return m_current_x; }

	//获得当前落点的列号
	int Gobang::get_m_current_y()const{ return m_current_y; }

	//获得电脑的攻击系数
	double Gobang::get_m_k1()const{ return m_k1; }

	//获得电脑的防守系数
	double Gobang::get_m_k2()const{ return m_k2; }

	bool Gobang::player_luozi(int x,int y)
	{//玩家落子
		if(x<0||x>=m_M||y<0||y>=m_M)       //先验证落点是否越界
		{
			return false;
		}
		else if(m_qipan[x][y]!=m_vacant)   //再验证落点是否是可以落子的空位
		{
			return false;
		}
		else                               //若落点可以落子
		{
			//更新当前落点位置
			m_current_x=x;
			m_current_y=y;
			//更新棋盘
			m_qipan[m_current_x][m_current_y]=m_black;
			//更新总棋数
			m_current_numbers++;
			return true;
		}
	}

	bool Gobang::will_win(int &x,int &y)
	{
		//先保存当前落点(用于数据恢复)
		int tempx=m_current_x;
		int tempy=m_current_y;
		//遍历整个棋盘的空位,看落点在(i,j)是否电脑能立刻赢
		for(int i=0;i<m_M;i++)
		{
			for(int j=0;j<m_M;j++)    
			{
				if(m_qipan[i][j]==m_vacant) //是空位才进行判断
				{
					//尝试落点(更新棋盘,当前落点)
					m_qipan[i][j]=m_white;    
			    	m_current_x=i;
			    	m_current_y=j;
			    	if(judge_winorfail()==-1) //电脑赢即玩家输
			    	{
				    	//数据恢复(还原到原先的棋盘和当前落点)
				    	m_current_x=tempx;
				    	m_current_y=tempy;
				    	m_qipan[i][j]=m_vacant; 
				    	//记录最佳落点(i,j)
				    	x=i; y=j;
				    	return true;
			    	}
					//若该处不是制胜点,则还原该处为原先的空位,继续验证别的空位
			    	m_qipan[i][j]=m_vacant;
				}
			}
		}
		//若不存在 对于电脑来说的制胜点,则还原数据并return false
		m_current_x=tempx;
		m_current_y=tempy;
		return false;
	}

	bool Gobang::will_fail(int &x,int &y)
	{
		//先保存当前落点(用于数据恢复)
		int tempx=m_current_x;
		int tempy=m_current_y;
		//遍历整个棋盘的空位,看如果玩家落点在(i,j),电脑是否会立刻输
		for(int i=0;i<m_M;i++)
		{
			for(int j=0;j<m_M;j++)    
			{
				if(m_qipan[i][j]==m_vacant) //是空位才进行判断
				{
					//尝试落点(更新棋盘,当前落点)
					m_qipan[i][j]=m_black;
			    	m_current_x=i;
			    	m_current_y=j;
			    	if(judge_winorfail()==1) //电脑输即玩家赢
			    	{
						//数据恢复(还原到原先的棋盘和当前落点)
				    	m_current_x=tempx;
				    	m_current_y=tempy;
				    	m_qipan[i][j]=m_vacant; 
				    	//记录最佳落点(i,j)
				    	x=i; y=j;
				    	return true;
			    	}
					//若该处不是制败点,则还原该处为原先的空位,继续验证别的空位
			    	m_qipan[i][j]=m_vacant;
				}
			}
		}
		//若不存在 对于电脑来说的制败点,则还原数据并return false
		m_current_x=tempx;
		m_current_y=tempy;
		return false;
	}

	int Gobang::judge_winorfail()const
	{
		//判断胜负(以玩家为标准.返回2:尚未分出胜负 1:玩家胜 0:平局 -1:玩家输)
		/* 
		  1:先看当前落点是黑棋(玩家)or白棋(电脑)or空位(说明游戏刚开始,则返回2)
		    若是黑棋:计算落点周围的四个方向上的连续黑棋数,若在某方向上满5,则返回1
			若是白棋:计算落点周围的四个方向上的连续白棋数,若在某方向上满5,则返回-1
			[四个方向:水平,竖直,45°,135°]
		  2:若棋盘满了,则平局返回0
		  3:若都不满足,则是未分胜负的状态,返回2
		*/
		if(m_qipan[m_current_x][m_current_y]==m_black) //若当前落点是黑棋
		{
			//先计算水平方向
			int number=1;
			for(int j=m_current_y-1;j>=0;j--)
			{
				if(m_qipan[m_current_x][j]==m_black)
					number++;
				else break;
			}
			for(int j=m_current_y+1;j<m_M;j++)
			{
				if(m_qipan[m_current_x][j]==m_black)
					number++;
				else break;
			}
			if(number>=5) return 1;
			//再计算竖直方向
			number=1;
			for(int i=m_current_x-1;i>=0;i--)
			{
				if(m_qipan[i][m_current_y]==m_black)
					number++;
				else break;
			}
			for(int i=m_current_x+1;i<m_M;i++)
			{
				if(m_qipan[i][m_current_y]==m_black)
					number++;
				else break;
			}
			if(number>=5) return 1;
			//再计算45°方向
			number=1;
			int i=m_current_x-1;
			int j=m_current_y-1;
			while(i>=0&&j>=0)
			{
				if(m_qipan[i][j]==m_black)
				{
					number++;
					i--; j--;
				}
				else break;
			}
			i=m_current_x+1;
			j=m_current_y+1;
			while(i<m_M&&j<m_M)
			{
				if(m_qipan[i][j]==m_black)
				{
					number++;
					i++; j++;
				}
				else break;
			}
			if(number>=5) return 1;
			//再计算135°方向
			number=1;
			i=m_current_x-1;
			j=m_current_y+1;
			while(i>=0&&j<m_M)
			{
				if(m_qipan[i][j]==m_black)
				{
					number++;
					i--; j++;
				}
				else break;
			}
			i=m_current_x+1;
			j=m_current_y-1;
			while(i<m_M&&j>=0)
			{
				if(m_qipan[i][j]==m_black)
				{
					number++;
					i++; j--;
				}
				else break;
			}
			if(number>=5) return 1;
		}
		else if(m_qipan[m_current_x][m_current_y]==m_white)//若当前落点是白棋
		{
			//先计算水平方向
			int number=1;
			for(int j=m_current_y-1;j>=0;j--)
			{
				if(m_qipan[m_current_x][j]==m_white)
					number++;
				else break;
			}
			for(int j=m_current_y+1;j<m_M;j++)
			{
				if(m_qipan[m_current_x][j]==m_white)
					number++;
				else break;
			}
			if(number>=5) return -1;
			//再计算竖直方向
			number=1;
			for(int i=m_current_x-1;i>=0;i--)
			{
				if(m_qipan[i][m_current_y]==m_white)
					number++;
				else break;
			}
			for(int i=m_current_x+1;i<m_M;i++)
			{
				if(m_qipan[i][m_current_y]==m_white)
					number++;
				else break;
			}
			if(number>=5) return -1;
			//再计算45°方向
			number=1;
			int i=m_current_x-1;
			int j=m_current_y-1;
			while(i>=0&&j>=0)
			{
				if(m_qipan[i][j]==m_white)
				{
					number++;
					i--; j--;
				}
				else break;
			}
			i=m_current_x+1;
			j=m_current_y+1;
			while(i<m_M&&j<m_M)
			{
				if(m_qipan[i][j]==m_white)
				{
					number++;
					i++; j++;
				}
				else break;
			}
			if(number>=5) return -1;
			//再计算135°方向
			number=1;
			i=m_current_x-1;
			j=m_current_y+1;
			while(i>=0&&j<m_M)
			{
				if(m_qipan[i][j]==m_white)
				{
					number++;
					i--; j++;
				}
				else break;
			}
			i=m_current_x+1;
			j=m_current_y-1;
			while(i<m_M&&j>=0)
			{
				if(m_qipan[i][j]==m_white)
				{
					number++;
					i++; j--;
				}
				else break;
			}
			if(number>=5) return -1;
		}
		else //当前落点是空位,说明游戏刚开始
			return 2;
		//接下来判断棋盘是否满了
		int not_vacant=0;
		for(int i=0;i<m_M;i++)
		{
			for(int j=0;j<m_M;j++)
			{
				if(m_qipan[i][j]!=m_vacant)
					not_vacant++;
			}
		}
		if(not_vacant==m_M*m_M) //棋盘满了
			return 0; //平局
		return 2; //上述都不满足,则是未分胜负的状态
	}

	void Gobang::origin_luozi()
	{
		//一开始棋子非常少的时候,电脑的落子:
		if(m_current_numbers==0)
		{//若是开局,则电脑直接在棋盘中间落子即可
			m_current_x=static_cast<int>(m_M)/2;
			m_current_y=m_current_x;
			m_qipan[m_current_x][m_current_y]=m_white;
			m_current_numbers++;
			return;
		}
		if(m_current_numbers<=3)
		{//若不是开局,且当前棋盘上棋子数很少(<=3),则让电脑在玩家落点的周围随机落子即可
			if(m_current_y-1>=0&&m_qipan[m_current_x][m_current_y-1]==m_vacant)
			{
				m_current_y-=1;
				m_qipan[m_current_x][m_current_y]=m_white;
				m_current_numbers++;
				return;
			}
			else if(m_current_y+1<m_M&&m_qipan[m_current_x][m_current_y+1]==m_vacant)
			{
				m_current_y+=1;
				m_qipan[m_current_x][m_current_y]=m_white;
				m_current_numbers++;
				return;
			}
			else if(m_current_x-1>=0&&m_qipan[m_current_x-1][m_current_y]==m_vacant)
			{
				m_current_x-=1;
				m_qipan[m_current_x][m_current_y]=m_white;
				m_current_numbers++;
				return;
			}
			else if(m_current_x+1<m_M&&m_qipan[m_current_x+1][m_current_y]==m_vacant)
			{
				m_current_x+=1;
				m_qipan[m_current_x][m_current_y]=m_white;
				m_current_numbers++;
				return;
			}
			else if(m_current_x-1>=0&&m_current_y-1>=0&&m_qipan[m_current_x-1][m_current_y-1]==m_vacant)
			{
				m_current_x-=1;
				m_current_y-=1;
				m_qipan[m_current_x][m_current_y]=m_white;
				m_current_numbers++;
				return;
			}
			else if(m_current_x+1<m_M&&m_current_y+1<m_M&&m_qipan[m_current_x+1][m_current_y+1]==m_vacant)
			{
				m_current_x+=1;
				m_current_y+=1;
				m_qipan[m_current_x][m_current_y]=m_white;
				m_current_numbers++;
				return;
			}
			else if(m_current_x-1>=0&&m_current_y+1<m_M&&m_qipan[m_current_x-1][m_current_y+1]==m_vacant)
			{
				m_current_x-=1;
				m_current_y+=1;
				m_qipan[m_current_x][m_current_y]=m_white;
				m_current_numbers++;
				return;
			}
			else if(m_current_x+1<m_M&&m_current_y-1>=0&&m_qipan[m_current_x+1][m_current_y-1]==m_vacant)
			{
				m_current_x+=1;
				m_current_y-=1;
				m_qipan[m_current_x][m_current_y]=m_white;
				m_current_numbers++;
				return;
			}
		}
	}

	void Gobang::calc_qixing_data(int i,int j,char qizi_color,int data[4][2][5])
	{//计算某个位置(i,j),对于棋子颜色qizi_color来说,它周围的棋型数据
		/*
		一:
		data[0][0][0]: 水平方向上,(i,j)左侧,向左第一次遇到与qizi_color相同颜色的连续棋子数目
		data[0][0][1]: 水平方向上,(i,j)左侧,向左第一次遇到空位,连续空位的数目
		data[0][0][2]: 水平方向上,(i,j)左侧,向左第二次遇到与qizi_color相同颜色的连续棋子数目
		data[0][0][3]: 水平方向上,(i,j)左侧,向左第二次遇到空位,连续空位的数目
		data[0][0][4]: 水平方向上,(i,j)左侧,向左第三次遇到与qizi_color相同颜色的连续棋子数目
		data[0][1][0]~data[0][1][4] 则是水平方向(i,j)右侧向右的情况
		二:
		data[1][0][0]: 竖直方向上,(i,j)上侧,向上第一次遇到与qizi_color相同颜色的连续棋子数目
		data[1][0][1]: 竖直方向上,(i,j)上侧,向上第一次遇到空位,连续空位的数目
		data[1][0][2]: 竖直方向上,(i,j)上侧,向上第二次遇到与qizi_color相同颜色的连续棋子数目
		data[1][0][3]: 竖直方向上,(i,j)上侧,向上第二次遇到空位,连续空位的数目
		data[1][0][4]: 竖直方向上,(i,j)上侧,向上第三次遇到与qizi_color相同颜色的连续棋子数目
		data[1][1][0]~data[1][1][4] 则是竖直方向(i,j)下侧向下的情况
		三:
		data[2][0][0]: 平行主对角线方向上,(i,j)左上侧,向左上第一次遇到与qizi_color相同颜色的连续棋子数目
		data[2][0][1]: 平行主对角线方向上,(i,j)左上侧,向左上第一次遇到空位,连续空位的数目
		data[2][0][2]: 平行主对角线方向上,(i,j)左上侧,向左上第二次遇到与qizi_color相同颜色的连续棋子数目
		data[2][0][3]: 平行主对角线方向上,(i,j)左上侧,向左上第二次遇到空位,连续空位的数目
		data[2][0][4]: 平行主对角线方向上,(i,j)左上侧,向左上第三次遇到与qizi_color相同颜色的连续棋子数目
		data[2][1][0]~data[2][1][4] 则是平行主对角线方向(i,j)右下侧向右下的情况
		四:
		data[3][0][0]: 垂直主对角线方向上,(i,j)右上侧,向右上第一次遇到与qizi_color相同颜色的连续棋子数目
		data[3][0][1]: 垂直主对角线方向上,(i,j)右上侧,向右上第一次遇到空位,连续空位的数目
		data[3][0][2]: 垂直主对角线方向上,(i,j)右上侧,向右上第二次遇到与qizi_color相同颜色的连续棋子数目
		data[3][0][3]: 垂直主对角线方向上,(i,j)右上侧,向右上第二次遇到空位,连续空位的数目
		data[3][0][4]: 垂直主对角线方向上,(i,j)右上侧,向右上第三次遇到与qizi_color相同颜色的连续棋子数目
		data[3][1][0]~data[3][1][4] 则是垂直主对角线方向(i,j)左下侧向左下的情况
		*/
		if(m_qipan[i][j]!=m_vacant) return; //若不是空位则不计算
		//先清零
		for(int p=0;p<4;p++)
		{
			for(int q=0;q<2;q++)
			{
				for(int k=0;k<5;k++)
					data[p][q][k]=0;
			}
		}
		int tx=0,ty=0;
		//先计算水平方向
		ty=j-1;
		while(ty>=0)
		{
			if(m_qipan[i][ty]==qizi_color)
			{ data[0][0][0]++; ty--; }
			else break;
		}
		while(ty>=0)
		{
			if(m_qipan[i][ty]==m_vacant)
			{ data[0][0][1]++; ty--; }
			else break;
		}
		while(ty>=0)
		{
			if(m_qipan[i][ty]==qizi_color)
			{ data[0][0][2]++; ty--; }
			else break;
		}
		while(ty>=0)
		{
			if(m_qipan[i][ty]==m_vacant)
			{ data[0][0][3]++; ty--; }
			else break;
		}
		while(ty>=0)
		{
			if(m_qipan[i][ty]==qizi_color)
			{ data[0][0][4]++; ty--; }
			else break;
		}
		ty=j+1;
		while(ty<m_M)
		{
			if(m_qipan[i][ty]==qizi_color)
			{ data[0][1][0]++; ty++; }
			else break;
		}
		while(ty<m_M)
		{
			if(m_qipan[i][ty]==m_vacant)
			{ data[0][1][1]++; ty++; }
			else break;
		}
		while(ty<m_M)
		{
			if(m_qipan[i][ty]==qizi_color)
			{ data[0][1][2]++; ty++; }
			else break;
		}
		while(ty<m_M)
		{
			if(m_qipan[i][ty]==m_vacant)
			{ data[0][1][3]++; ty++; }
			else break;
		}
		while(ty<m_M)
		{
			if(m_qipan[i][ty]==qizi_color)
			{ data[0][1][4]++; ty++; }
			else break;
		}
		//再计算竖直方向
		tx=i-1;
		while(tx>=0)
		{
			if(m_qipan[tx][j]==qizi_color)
			{ data[1][0][0]++; tx--; }
			else break;
		}
		while(tx>=0)
		{
			if(m_qipan[tx][j]==m_vacant)
			{ data[1][0][1]++; tx--; }
			else break;
		}
		while(tx>=0)
		{
			if(m_qipan[tx][j]==qizi_color)
			{ data[1][0][2]++; tx--; }
			else break;
		}
		while(tx>=0)
		{
			if(m_qipan[tx][j]==m_vacant)
			{ data[1][0][3]++; tx--; }
			else break;
		}
		while(tx>=0)
		{
			if(m_qipan[tx][j]==qizi_color)
			{ data[1][0][4]++; tx--; }
			else break;
		}
		tx=i+1;
		while(tx<m_M)
		{
			if(m_qipan[tx][j]==qizi_color)
			{ data[1][1][0]++; tx++; }
			else break;
		}
		while(tx<m_M)
		{
			if(m_qipan[tx][j]==m_vacant)
			{ data[1][1][1]++; tx++; }
			else break;
		}
		while(tx<m_M)
		{
			if(m_qipan[tx][j]==qizi_color)
			{ data[1][1][2]++; tx++; }
			else break;
		}
		while(tx<m_M)
		{
			if(m_qipan[tx][j]==m_vacant)
			{ data[1][1][3]++; tx++; }
			else break;
		}
		while(tx<m_M)
		{
			if(m_qipan[tx][j]==qizi_color)
			{ data[1][1][4]++; tx++; }
			else break;
		}
		//再计算平行于主对角线方向
		tx=i-1; ty=j-1;
		while(tx>=0&&ty>=0)
		{
			if(m_qipan[tx][ty]==qizi_color)
			{ data[2][0][0]++; tx--; ty--; }
			else break;
		}
		while(tx>=0&&ty>=0)
		{
			if(m_qipan[tx][ty]==m_vacant)
			{ data[2][0][1]++; tx--; ty--; }
			else break;
		}
		while(tx>=0&&ty>=0)
		{
			if(m_qipan[tx][ty]==qizi_color)
			{ data[2][0][2]++; tx--; ty--; }
			else break;
		}
		while(tx>=0&&ty>=0)
		{
			if(m_qipan[tx][ty]==m_vacant)
			{ data[2][0][3]++; tx--; ty--; }
			else break;
		}
		while(tx>=0&&ty>=0)
		{
			if(m_qipan[tx][ty]==qizi_color)
			{ data[2][0][4]++; tx--; ty--; }
			else break;
		}
		tx=i+1; ty=j+1;
		while(tx<m_M&&ty<m_M)
		{
			if(m_qipan[tx][ty]==qizi_color)
			{ data[2][1][0]++; tx++; ty++; }
			else break;
		}
		while(tx<m_M&&ty<m_M)
		{
			if(m_qipan[tx][ty]==m_vacant)
			{ data[2][1][1]++; tx++; ty++; }
			else break;
		}
		while(tx<m_M&&ty<m_M)
		{
			if(m_qipan[tx][ty]==qizi_color)
			{ data[2][1][2]++; tx++; ty++; }
			else break;
		}
		while(tx<m_M&&ty<m_M)
		{
			if(m_qipan[tx][ty]==m_vacant)
			{ data[2][1][3]++; tx++; ty++; }
			else break;
		}
		while(tx<m_M&&ty<m_M)
		{
			if(m_qipan[tx][ty]==qizi_color)
			{ data[2][1][4]++; tx++; ty++; }
			else break;
		}
		//再计算垂直于主对角线方向
		tx=i-1; ty=j+1;
		while(tx>=0&&ty<m_M)
		{
			if(m_qipan[tx][ty]==qizi_color)
			{ data[3][0][0]++; tx--; ty++; }
			else break;
		}
		while(tx>=0&&ty<m_M)
		{
			if(m_qipan[tx][ty]==m_vacant)
			{ data[3][0][1]++; tx--; ty++; }
			else break;
		}
		while(tx>=0&&ty<m_M)
		{
			if(m_qipan[tx][ty]==qizi_color)
			{ data[3][0][2]++; tx--; ty++; }
			else break;
		}
		while(tx>=0&&ty<m_M)
		{
			if(m_qipan[tx][ty]==m_vacant)
			{ data[3][0][3]++; tx--; ty++; }
			else break;
		}
		while(tx>=0&&ty<m_M)
		{
			if(m_qipan[tx][ty]==qizi_color)
			{ data[3][0][4]++; tx--; ty++; }
			else break;
		}
		tx=i+1; ty=j-1;
		while(tx<m_M&&ty>=0)
		{
			if(m_qipan[tx][ty]==qizi_color)
			{ data[3][1][0]++; tx++; ty--; }
			else break;
		}
		while(tx<m_M&&ty>=0)
		{
			if(m_qipan[tx][ty]==m_vacant)
			{ data[3][1][1]++; tx++; ty--; }
			else break;
		}
		while(tx<m_M&&ty>=0)
		{
			if(m_qipan[tx][ty]==qizi_color)
			{ data[3][1][2]++; tx++; ty--; }
			else break;
		}
		while(tx<m_M&&ty>=0)
		{
			if(m_qipan[tx][ty]==m_vacant)
			{ data[3][1][3]++; tx++; ty--; }
			else break;
		}
		while(tx<m_M&&ty>=0)
		{
			if(m_qipan[tx][ty]==qizi_color)
			{ data[3][1][4]++; tx++; ty--; }
			else break;
		}
	}

	void Gobang::calc_qixing_value(int i,int j,char qizi_color,const int data[4][2][5])
	{
		//根据某位置的之前计算好的周围的棋型数据,计算该位置的攻击系数or防御系数
		//若qizi_color为黑棋,则计算对于电脑来说,该点的防守价值
		//若qizi_color为白棋,则计算对于电脑来说,该点的攻击价值
		if(m_qipan[i][j]!=m_vacant) return; //若不是空位则不计算
		//先清零
		if(qizi_color==m_white) m_computer[i][j]=0;
		else                    m_player[i][j]=0;
		//然后计算四个方向上的总值
		for(int k=0;k<4;k++)  //用四次循环来表示四个方向
		{
			//定义不同的棋型数据代表了不同的攻防价值(可以自定义,此处我参考了一些实验数据并做了些修改)
			//此处可以尽情地扩展 (=v=)
			if((data[k][0][0]==3&&data[k][0][1]>=1&&data[k][1][1]>=1)||
				(data[k][0][1]>=1&&data[k][1][0]==3&&data[k][1][1]>=1))
			{
				if(qizi_color==m_white) m_computer[i][j]+=5000;
				else m_player[i][j]+=5000;
			}
			else if((data[k][0][0]==0&&data[k][0][1]==1&&data[k][0][2]==3&&data[k][0][3]>=1)||
				(data[k][1][0]==0&&data[k][1][1]==1&&data[k][1][2]==3&&data[k][1][3]>=1))
			{
				if(qizi_color==m_white) m_computer[i][j]+=700;
				else m_player[i][j]+=700;
			}
			else if((data[k][0][0]==3&&data[k][0][1]==0&&data[k][0][2]==0&&data[k][1][0]==0&&data[k][1][1]==1&&data[k][1][2]==0&&data[k][1][3]==0)||
				(data[k][0][0]==0&&data[k][0][1]==1&&data[k][0][2]==0&&data[k][0][3]==0&&data[k][1][0]==3&&data[k][1][1]==0&&data[k][1][2]==0))
			{
				if(qizi_color==m_white) m_computer[i][j]+=200;
				else m_player[i][j]+=200;
			}
			else if((data[k][0][0]==0&&data[k][0][1]==1&&data[k][0][2]==3&&data[k][0][3]==0&&data[k][0][4]==0&&data[k][1][0]==0&&data[k][1][1]==0)||
				(data[k][0][0]==0&&data[k][0][1]==0&&data[k][1][0]==0&&data[k][1][1]==1&&data[k][1][2]==3&&data[k][1][3]==0&&data[k][1][4]==0))
			{
				if(qizi_color==m_white) m_computer[i][j]+=200;
				else m_player[i][j]+=200;
			}
			else if((data[k][0][0]==1&&data[k][0][1]>=1&&data[k][1][0]==2&&data[k][1][1]>=1)||
				(data[k][0][0]==2&&data[k][0][1]>=1&&data[k][1][0]==1&&data[k][1][1]>=1))
			{
				if(qizi_color==m_white) m_computer[i][j]+=4900;
				else m_player[i][j]+=4900;
			}
			else if((data[k][0][0]==0&&data[k][0][1]==0&&data[k][1][0]==0&&data[k][1][1]==1&&data[k][1][2]==2&&data[k][1][3]==0&&data[k][1][4]==0)||
				(data[k][0][0]==0&&data[k][0][1]==1&&data[k][0][2]==2&&data[k][0][3]==0&&data[k][0][4]==0&&data[k][1][0]==0&&data[k][1][1]==0))
			{
				if(qizi_color==m_white) m_computer[i][j]+=350;
				else m_player[i][j]+=350;
			}
			else if((data[k][0][0]==1&&data[k][0][1]==1&&data[k][0][2]==0&&data[k][0][3]==0&&data[k][1][0]==2&&data[k][1][1]==0&&data[k][1][2]==0)||
				(data[k][0][0]==2&&data[k][0][1]==0&&data[k][0][2]==0&&data[k][1][0]==1&&data[k][1][1]==1&&data[k][1][2]==0&&data[k][1][3]==0))
			{
				if(qizi_color==m_white) m_computer[i][j]+=350;
				else m_player[i][j]+=350;
			}
			else if((data[k][0][0]==0&&data[k][0][1]==0&&data[k][1][0]==3&&data[k][1][1]==1&&data[k][1][2]==0&&data[k][1][3]==0)||
				(data[k][0][0]==3&&data[k][0][1]==1&&data[k][0][2]==0&&data[k][0][3]==0&&data[k][1][0]==0&&data[k][1][1]==0))
			{
				if(qizi_color==m_white) m_computer[i][j]+=350;
				else m_player[i][j]+=350;
			}
			else if((data[k][0][0]==0&&data[k][0][1]==0&&data[k][1][0]==2&&data[k][1][1]==1&&data[k][1][2]==1&&data[k][1][3]==0&&data[k][1][4]==0)||
				(data[k][0][0]==2&&data[k][0][1]==1&&data[k][0][2]==1&&data[k][0][3]==0&&data[k][0][4]==0&&data[k][1][0]==0&&data[k][1][1]==0))
			{
				if(qizi_color==m_white) m_computer[i][j]+=350;
				else m_player[i][j]+=350;
			}
			else if((data[k][0][0]==2&&data[k][0][1]==1&&data[k][0][2]==0&&data[k][0][3]==0&&data[k][1][0]==1&&data[k][1][1]==0&&data[k][1][2]==0)||
				(data[k][0][0]==1&&data[k][0][1]==0&&data[k][0][2]==0&&data[k][1][0]==2&&data[k][1][1]==1&&data[k][1][2]==0&&data[k][1][3]==0))
			{
				if(qizi_color==m_white) m_computer[i][j]+=350;
				else m_player[i][j]+=350;
			}
			else if((data[k][0][0]==1&&data[k][0][1]==0&&data[k][0][2]==0&&data[k][1][0]==0&&data[k][1][1]==1&&data[k][1][2]==2&&data[k][1][3]==0&&data[k][1][4]==0)||
				(data[k][0][0]==0&&data[k][0][1]==1&&data[k][0][2]==2&&data[k][0][3]==0&&data[k][0][4]==0&&data[k][1][0]==1&&data[k][1][1]==0&&data[k][1][2]==0))
			{
				if(qizi_color==m_white) m_computer[i][j]+=350;
				else m_player[i][j]+=350;
			}
			else if((data[k][0][0]==0&&data[k][0][1]==1&&data[k][0][2]==1&&data[k][0][3]==0&&data[k][0][4]==0&&data[k][1][0]==2&&data[k][1][1]==0&&data[k][1][2]==0)||
				(data[k][0][0]==2&&data[k][0][1]==0&&data[k][0][2]==0&&data[k][1][0]==0&&data[k][1][1]==1&&data[k][1][2]==1&&data[k][1][3]==0&&data[k][1][4]==0))
			{
				if(qizi_color==m_white) m_computer[i][j]+=350;
				else m_player[i][j]+=350;
			}
			else if((data[k][0][0]==1&&data[k][0][1]==0&&data[k][0][2]==0&&data[k][1][0]==1&&data[k][1][1]==1&&data[k][1][2]==1&&data[k][1][3]==0&&data[k][1][3]==0)||
				(data[k][0][0]==1&&data[k][0][1]==1&&data[k][0][2]==1&&data[k][0][3]==0&&data[k][0][4]==0&&data[k][1][0]==1&&data[k][1][1]==0&&data[k][1][2]==0))
			{
				if(qizi_color==m_white) m_computer[i][j]+=350;
				else m_player[i][j]+=350;
			}
			else if((data[k][0][0]==0&&data[k][0][1]==0&&data[k][1][0]==1&&data[k][1][1]==1&&data[k][1][2]==2&&data[k][1][3]==1&&data[k][1][4]==0)||
				(data[k][0][0]==1&&data[k][0][1]==1&&data[k][0][2]==2&&data[k][0][3]==1&&data[k][0][4]==0&&data[k][1][0]==0&&data[k][1][1]==0))
			{
				if(qizi_color==m_white) m_computer[i][j]+=400;
				else m_player[i][j]+=400;
			}
			else if((data[k][0][0]==2&&data[k][0][1]==1&&data[k][0][2]==1&&data[k][0][3]==1&&data[k][0][4]==0&&data[k][1][0]==0&&data[k][1][1]==0)||
				(data[k][0][0]==0&&data[k][0][1]==0&&data[k][1][0]==2&&data[k][1][1]==1&&data[k][1][2]==1&&data[k][1][3]==1&&data[k][1][4]==0))
			{
				if(qizi_color==m_white) m_computer[i][j]+=400;
				else m_player[i][j]+=400;
			}
			else if((data[k][0][0]==2&&data[k][0][1]>=2&&data[k][1][0]==0&&data[k][1][1]>=2)||
				(data[k][0][0]==0&&data[k][0][1]>=2&&data[k][1][0]==2&&data[k][1][1]>=2))
			{
				if(qizi_color==m_white) m_computer[i][j]+=1200;
				else m_player[i][j]+=1200;
			}
			else if((data[k][0][0]==0&&data[k][0][1]==1&&data[k][0][2]==2&&data[k][0][3]>=1&&data[k][1][0]==0&&data[k][1][1]>=1)||
				(data[k][0][0]==0&&data[k][0][1]>=1&&data[k][1][0]==0&&data[k][1][1]==1&&data[k][1][2]==2&&data[k][1][3]>=1))
			{
				if(qizi_color==m_white) m_computer[i][j]+=850;
				else m_player[i][j]+=850;
			}
			else if((data[k][0][0]==0&&data[k][0][1]==2&&data[k][0][2]==2&&data[k][0][3]>=1)||
				(data[k][1][0]==0&&data[k][1][1]==2&&data[k][1][2]==2&&data[k][1][3]>=1))
			{
				if(qizi_color==m_white) m_computer[i][j]+=600;
				else m_player[i][j]+=600;
			}
			else if((data[k][0][0]==2&&data[k][0][1]==0&&data[k][0][2]==0&&data[k][1][0]==0&&data[k][1][1]==2&&data[k][1][2]==0&&data[k][1][3]==0)||
				(data[k][0][0]==0&&data[k][0][1]==2&&data[k][0][2]==0&&data[k][0][3]==0&&data[k][1][0]==2&&data[k][1][1]==0&&data[k][1][2]==0))
			{
				if(qizi_color==m_white) m_computer[i][j]+=150;
				else m_player[i][j]+=150;
			}
			else if((data[k][0][0]==0&&data[k][0][1]==1&&data[k][0][2]==2&&data[k][0][3]==0&&data[k][0][4]==0&&data[k][1][0]==0&&data[k][1][1]==1&&data[k][1][2]==0&&data[k][1][3]==0)||
				(data[k][0][0]==0&&data[k][0][1]==1&&data[k][0][2]==0&&data[k][0][3]==0&&data[k][1][0]==0&&data[k][1][1]==1&&data[k][1][2]==2&&data[k][1][3]==0&&data[k][1][4]==0))
			{
				if(qizi_color==m_white) m_computer[i][j]+=150;
				else m_player[i][j]+=150;
			}
			else if((data[k][0][0]==0&&data[k][0][1]==2&&data[k][0][2]==2&&data[k][0][3]==0&&data[k][0][4]==0&&data[k][1][0]==0&&data[k][1][1]==0)||
				(data[k][0][0]==0&&data[k][0][1]==0&&data[k][1][0]==0&&data[k][1][1]==2&&data[k][1][2]==2&&data[k][1][3]==0&&data[k][1][4]==0))
			{
				if(qizi_color==m_white) m_computer[i][j]+=150;
				else m_player[i][j]+=150;
			}
			else if(data[k][0][0]==1&&data[k][0][1]>=2&&data[k][1][0]==1&&data[k][1][1]>=2)
			{
				if(qizi_color==m_white) m_computer[i][j]+=1200;
				else m_player[i][j]+=1200;
			}
			else if((data[k][0][0]==1&&data[k][0][1]==1&&data[k][0][2]==1&&data[k][0][3]>=2&&data[k][1][0]==0&&data[k][1][1]>=1)||
				(data[k][0][0]==0&&data[k][0][1]>=1&&data[k][1][0]==1&&data[k][1][1]==1&&data[k][1][2]==1&&data[k][1][3]>=2))
			{
				if(qizi_color==m_white) m_computer[i][j]+=850;
				else m_player[i][j]+=850;
			}
			else if((data[k][0][0]==0&&data[k][0][1]==1&&data[k][0][2]==1&&data[k][0][3]==1&&data[k][0][4]>=1&&data[k][1][0]==0)&&data[k][1][1]>=1||
				(data[k][0][0]==0&&data[k][0][1]>=1&&data[k][1][0]==0&&data[k][1][1]==1&&data[k][1][2]==1&&data[k][1][3]==1&&data[k][1][4]>=1))
			{
				if(qizi_color==m_white) m_computer[i][j]+=450;
				else m_player[i][j]+=450;
			}
			else if((data[k][0][0]==1&&data[k][0][1]==2&&data[k][0][2]==1&&data[k][0][3]>=1&&data[k][1][0]==0&&data[k][1][1]==0)||
				(data[k][0][0]==0&&data[k][0][1]==0&&data[k][1][0]==1&&data[k][1][1]==2&&data[k][1][2]==1&&data[k][1][3]>=1))
			{
				if(qizi_color==m_white) m_computer[i][j]+=380;
				else m_player[i][j]+=380;
			}
			else if((data[k][0][0]==1&&data[k][0][1]>=1&&data[k][1][0]==0&&data[k][1][1]==2&&data[k][1][2]==1&&data[k][1][3]>=1)||
				(data[k][0][0]==0&&data[k][0][1]==2&&data[k][0][2]==1&&data[k][0][3]>=1&&data[k][1][0]==1&&data[k][1][1]>=1))
			{
				if(qizi_color==m_white) m_computer[i][j]+=850;
				else m_player[i][j]+=850;
			}
		} //计算完 第k 方向
	}

	void Gobang::fill_m_computer()
	{
		//计算填写 对于电脑来说的攻击性价值表
		int DATA[4][2][5];
		for(int i=0;i<m_M;i++)
		{
			for(int j=0;j<m_M;j++)
			{
				m_computer[i][j]=0; //先清零
				if(m_qipan[i][j]==m_vacant) //是空位才计算
				{
					//先计算(i,j)位置周围 相对于白棋来说的 棋型数据
					calc_qixing_data(i,j,m_white,DATA);
					//然后计算(i,j)位置 相对于白棋(即电脑)来说的 攻击价值
					calc_qixing_value(i,j,m_white,DATA);
				} 
			}
		}
	}

	void Gobang::fill_m_player()
	{
		//计算填写 对于电脑来说的防守性价值表(即可以理解为玩家方的攻击价值表)
		int DATA[4][2][5];
		for(int i=0;i<m_M;i++)
		{
			for(int j=0;j<m_M;j++)
			{
				m_player[i][j]=0; //先清零
				if(m_qipan[i][j]==m_vacant) //是空位才计算
				{
					//先计算(i,j)位置周围 相对于黑棋来说的 棋型数据
					calc_qixing_data(i,j,m_black,DATA);
					//然后计算(i,j)位置 相对于黑棋(即玩家)来说的攻击价值
					//也即相对于白棋(电脑)来说的防守价值
					calc_qixing_value(i,j,m_black,DATA);
				}
			}
		}
	}

	int Gobang::single_live_four(int i,int j,char qizi_color)
	{//计算某位置活四的数目
		//若该点不是空位,则直接跳过
		if(m_qipan[i][j]!=m_vacant) return 0;
		int result=0;
		int DATA[4][2][5];
		//先计算该位置 相对于qizi_color颜色的棋来说的 棋型数据
		calc_qixing_data(i,j,qizi_color,DATA);
		//接下来(水平,竖直,45°,135°方向上),有几个方向上是存在活四的
		for(int k=0;k<4;k++)
		{
			if((DATA[k][0][0]==3&&DATA[k][0][1]>=1&&DATA[k][1][0]==0&&DATA[k][1][1]>=1)||
				(DATA[k][0][0]==0&&DATA[k][0][1]>=1&&DATA[k][1][0]==3&&DATA[k][1][1]>=1))
				result++;
			else if((DATA[k][0][0]==2&&DATA[k][0][1]>=1&&DATA[k][1][0]==1&&DATA[k][1][1]>=1)||
				(DATA[k][0][0]==1&&DATA[k][0][1]>=1&&DATA[k][1][0]==2&&DATA[k][1][1]>=1))
				result++;
		}
		return result;
	}

	int Gobang::single_live_three(int i,int j,char qizi_color)
	{//计算某位置活三的数目
		//若该点不是空位,则直接跳过
		if(m_qipan[i][j]!=m_vacant) return 0;
		int result=0;
		int DATA[4][2][5];
		//先计算该位置 相对于qizi_color颜色的棋来说的 棋型数据
		calc_qixing_data(i,j,qizi_color,DATA);
		//接下来(水平,竖直,45°,135°方向上),有几个方向上是存在活三的
		for(int k=0;k<4;k++)
		{
			if((DATA[k][0][0]==0&&DATA[k][0][1]>=2&&DATA[k][1][0]==2&&DATA[k][1][1]>=2)||
				(DATA[k][0][0]==2&&DATA[k][0][1]>=2&&DATA[k][1][0]==0&&DATA[k][1][1]>=2))
				result++;
			else if(DATA[k][0][0]==1&&DATA[k][0][1]>=2&&DATA[k][1][0]==1&&DATA[k][1][1]>=2)
				result++;
			else if((DATA[k][0][0]==0&&DATA[k][0][1]>=1&&DATA[k][1][0]==0&&DATA[k][1][1]==1&&DATA[k][1][2]==2&&DATA[k][1][3]>=1)||
				(DATA[k][0][0]==0&&DATA[k][0][1]==1&&DATA[k][0][2]==2&&DATA[k][0][3]>=1&&DATA[k][1][0]==0&&DATA[k][1][1]>=1))
				result++;
			else if((DATA[k][0][0]==0&&DATA[k][0][1]==1&&DATA[k][0][2]==1&&DATA[k][0][3]>=1&&DATA[k][1][0]==1&&DATA[k][1][1]>=1)||
				(DATA[k][0][0]==1&&DATA[k][0][1]>=1&&DATA[k][1][0]==0&&DATA[k][1][1]==1&&DATA[k][1][2]==1&&DATA[k][1][3]>=1))
				result++;
			else if((DATA[k][0][0]==1&&DATA[k][0][1]==1&&DATA[k][0][2]==1&&DATA[k][0][3]>=1&&DATA[k][1][0]==0&&DATA[k][1][1]>=1)||
				(DATA[k][0][0]==0&&DATA[k][0][1]>=1&&DATA[k][1][0]==1&&DATA[k][1][1]==1&&DATA[k][1][2]==1&&DATA[k][1][3]>=1))
				result++;
		}
		return result;
	}

	int Gobang::single_dead_four(int i,int j,char qizi_color)
	{//计算某位置死四的数目
		//若该点不是空位,则直接跳过
		if(m_qipan[i][j]!=m_vacant) return 0;
		int result=0;
		int DATA[4][2][5];
		//先计算该位置 相对于qizi_color颜色的棋来说的 棋型数据
		calc_qixing_data(i,j,qizi_color,DATA);
		//接下来(水平,竖直,45°,135°方向上),有几个方向上是存在死四的
		for(int k=0;k<4;k++)
		{
			if((DATA[k][0][0]==0&&DATA[k][0][1]==0&&DATA[k][1][0]==3&&DATA[k][1][1]>=1)||
				(DATA[k][0][0]==3&&DATA[k][0][1]>=1&&DATA[k][1][0]==0&&DATA[k][1][1]==0))
				result++;
			else if((DATA[k][0][0]==1&&DATA[k][0][1]==0&&DATA[k][0][2]==0&&DATA[k][1][0]==2&&DATA[k][1][1]>=1)||
				(DATA[k][0][0]==2&&DATA[k][0][1]>=1&&DATA[k][1][0]==1&&DATA[k][1][1]==0&&DATA[k][1][2]==0))
				result++;
			else if((DATA[k][0][0]==2&&DATA[k][0][1]==0&&DATA[k][0][2]==0&&DATA[k][1][0]==1&&DATA[k][1][1]>=1)||
				(DATA[k][0][0]==1&&DATA[k][0][1]>=1&&DATA[k][1][0]==2&&DATA[k][1][1]==0&&DATA[k][1][2]==0))
				result++;
			else if((DATA[k][0][0]==3&&DATA[k][0][1]==0&&DATA[k][0][2]==0&&DATA[k][1][0]==0&&DATA[k][1][1]>=1)||
				(DATA[k][0][0]==0&&DATA[k][0][1]>=1&&DATA[k][1][0]==3&&DATA[k][1][1]==0&&DATA[k][1][2]==0))
				result++;
			else if((DATA[k][0][0]==0&&DATA[k][0][1]==1&&DATA[k][0][2]==3)||
				(DATA[k][1][0]==0&&DATA[k][1][1]==1&&DATA[k][1][2]==3))
				result++;
			else if((DATA[k][0][0]==0&&DATA[k][0][1]==1&&DATA[k][0][2]>=1&&DATA[k][1][0]>=2)||
				(DATA[k][0][0]>=2&&DATA[k][1][0]==0&&DATA[k][1][1]==1&&DATA[k][1][2]>=1))
				result++;
			else if((DATA[k][0][0]==1&&DATA[k][0][1]==1&&DATA[k][0][2]>=1&&DATA[k][1][0]>=1)||
				(DATA[k][0][0]>=1&&DATA[k][1][0]==1&&DATA[k][1][1]==1&&DATA[k][1][2]>=1))
				result++;
			else if((DATA[k][0][0]==2&&DATA[k][0][1]==1&&DATA[k][0][2]>=1)||
				(DATA[k][1][0]==2&&DATA[k][1][1]==1&&DATA[k][1][2]>=1))
				result++;
			else if((DATA[k][0][0]==1&&DATA[k][0][1]==1&&DATA[k][0][2]>=2)||
				(DATA[k][1][0]==1&&DATA[k][1][1]==1&&DATA[k][1][2]>=2))
				result++;
			else if((DATA[k][0][0]>=1&&DATA[k][1][0]==0&&DATA[k][1][1]==1&&DATA[k][1][2]>=2)||
				(DATA[k][0][0]==0&&DATA[k][0][1]==1&&DATA[k][0][2]>=2&&DATA[k][1][0]>=1))
				result++;
		}
		return result;
	}

	bool Gobang::double_live_four(int &x,int &y,char qizi_color)
	{//判断是否存在某个空位,在该处落子能产生双活四的效果,参数x,y用来记录这样的位置
		for(int i=0;i<m_M;i++)
		{
			for(int j=0;j<m_M;j++)
			{
				//如果(i,j)是空位且能够满足双活四的条件
				if((m_qipan[i][j]==m_vacant)&&single_live_four(i,j,qizi_color)>=2) 
				{
					x=i;
					y=j;
					return true;
				}
			}
		}
		return false;
	}


	bool Gobang::live_four_and_live_three(int &x,int &y,char qizi_color)
	{//判断是否存在某个空位,在该处落子能产生活三四的效果,参数x,y用来记录这样的位置
		for(int i=0;i<m_M;i++)
		{
			for(int j=0;j<m_M;j++)
			{
				//如果(i,j)是空位且能够满足活四和活三的条件
				if((m_qipan[i][j]==m_vacant)&&single_live_four(i,j,qizi_color)>=1&&single_live_three(i,j,qizi_color)>=1)
				{
					x=i;
					y=j;
					return true;
				}
			}
		}
		return false;
	}

	bool Gobang::double_dead_four(int &x,int &y,char qizi_color)
	{//判断是否存在某个空位,在该处落子能产生双死四的效果,参数x,y用来记录这样的位置
		for(int i=0;i<m_M;i++)
		{
			for(int j=0;j<m_M;j++)
			{
				//如果(i,j)是空位且能够满足双死四的条件
				if((m_qipan[i][j]==m_vacant)&&single_dead_four(i,j,qizi_color)>=2) 
				{
					x=i;
					y=j;
					return true;
				}
			}
		}
		return false;
	}

	bool Gobang::dead_four_and_live_three(int &x,int &y,char qizi_color)
	{//判断是否存在某个空位,在该处落子能产生死四活三的效果,参数x,y用来记录这样的位置
		for(int i=0;i<m_M;i++)
		{
			for(int j=0;j<m_M;j++)
			{
				//如果(i,j)是空位且能够满足死四和活三的条件
				if((m_qipan[i][j]==m_vacant)&&single_dead_four(i,j,qizi_color)>=1&&single_live_three(i,j,qizi_color)>=1) 
				{
					x=i;
					y=j;
					return true;
				}
			}
		}
		return false;
	}

	bool Gobang::live_four(int &x,int &y,char qizi_color)
	{//判断是否存在某个空位,在该处落子能产生单活四的效果,参数x,y用来记录这样的位置
		for(int i=0;i<m_M;i++)
		{
			for(int j=0;j<m_M;j++)
			{
				//如果(i,j)是空位且能够满足活四的条件
				if((m_qipan[i][j]==m_vacant)&&single_live_four(i,j,qizi_color)>=1) 
				{
					x=i;
					y=j;
					return true;
				}
			}
		}
		return false;
	}

	bool Gobang::double_live_three(int &x,int &y,char qizi_color)
	{//判断是否存在某个空位,在该处落子能产生双活三的效果,参数x,y用来记录这样的位置
		for(int i=0;i<m_M;i++)
		{
			for(int j=0;j<m_M;j++)
			{
				//如果(i,j)是空位且能够满足双活三的条件
				if((m_qipan[i][j]==m_vacant)&&single_live_three(i,j,qizi_color)>=2) 
				{
					x=i;
					y=j;
					return true;
				}
			}
		}
		return false;
	}

	void Gobang::computer_luozi()
	{//电脑执行落子的逻辑决策
		/*
		(判断的先后顺序依次如下):

		若棋子总数不超3,在当前落点周围落子

		若存在  直接制胜点          ,则在那里落子
		若存在  直接制败点          ,则在那里落子

		若存在  本方双活四    的位置,则在那里落子
		若存在  本方活三四    的位置,则在那里落子
		若存在  本方双死四    的位置,则在那里落子
		若存在  本方死四活三  的位置,则在那里落子
		若存在  本方单活四    的位置,则在那里落子

		若存在  敌方双活四    的位置,则在那里落子
		若存在  敌方活三四    的位置,则在那里落子
		若存在  敌方双死四    的位置,则在那里落子
		若存在  敌方死四活三  的位置,则在那里落子
		若存在  敌方单活四    的位置,则在那里落子

		若存在  本方双活三    的位置,则在那里落子

		若存在  敌方双活三    的位置,则在那里落子

		若上述情况都不存在,则计算棋盘的攻击价值表and防守价值表,
		再根据攻防系数,计算出加权平均数最大的那个位置,则在那里落子

		*/
		int tx=0,ty=0;
		if(m_current_numbers<=3)
		{
			origin_luozi();
			return;
		}
		else if(will_win(tx,ty))
		{
			m_current_x=tx;
			m_current_y=ty;
			m_qipan[m_current_x][m_current_y]=m_white;
			m_current_numbers++;
			return;
		}
		else if(will_fail(tx,ty))
		{
			m_current_x=tx;
			m_current_y=ty;
			m_qipan[m_current_x][m_current_y]=m_white;
			m_current_numbers++;
			return;
		}
		else if(double_live_four(tx,ty,m_white))
		{
			m_current_x=tx;
			m_current_y=ty;
			m_qipan[m_current_x][m_current_y]=m_white;
			m_current_numbers++;
			return;
		}
		else if(live_four_and_live_three(tx,ty,m_white))
		{
			m_current_x=tx;
			m_current_y=ty;
			m_qipan[m_current_x][m_current_y]=m_white;
			m_current_numbers++;
			return;
		}
		else if(double_dead_four(tx,ty,m_white))
		{
			m_current_x=tx;
			m_current_y=ty;
			m_qipan[m_current_x][m_current_y]=m_white;
			m_current_numbers++;
			return;
		}
		else if(dead_four_and_live_three(tx,ty,m_white))
		{
			m_current_x=tx;
			m_current_y=ty;
			m_qipan[m_current_x][m_current_y]=m_white;
			m_current_numbers++;
			return;
		}
		else if(live_four(tx,ty,m_white))
		{
			m_current_x=tx;
			m_current_y=ty;
			m_qipan[m_current_x][m_current_y]=m_white;
			m_current_numbers++;
			return;
		}
		else if(double_live_four(tx,ty,m_black))
		{
			m_current_x=tx;
			m_current_y=ty;
			m_qipan[m_current_x][m_current_y]=m_white;
			m_current_numbers++;
			return;
		}
		else if(live_four_and_live_three(tx,ty,m_black))
		{
			m_current_x=tx;
			m_current_y=ty;
			m_qipan[m_current_x][m_current_y]=m_white;
			m_current_numbers++;
			return;
		}
		else if(double_dead_four(tx,ty,m_black))
		{
			m_current_x=tx;
			m_current_y=ty;
			m_qipan[m_current_x][m_current_y]=m_white;
			m_current_numbers++;
			return;
		}
		else if(dead_four_and_live_three(tx,ty,m_black))
		{
			m_current_x=tx;
			m_current_y=ty;
			m_qipan[m_current_x][m_current_y]=m_white;
			m_current_numbers++;
			return;
		}
		else if(live_four(tx,ty,m_black))
		{
			m_current_x=tx;
			m_current_y=ty;
			m_qipan[m_current_x][m_current_y]=m_white;
			m_current_numbers++;
			return;
		}
		else if(double_live_three(tx,ty,m_white))
		{
			m_current_x=tx;
			m_current_y=ty;
			m_qipan[m_current_x][m_current_y]=m_white;
			m_current_numbers++;
			return;
		}
		else if(double_live_three(tx,ty,m_black))
		{
			m_current_x=tx;
			m_current_y=ty;
			m_qipan[m_current_x][m_current_y]=m_white;
			m_current_numbers++;
			return;
		}
		else
		{
			fill_m_computer();
			fill_m_player();
			tx=0; ty=0;
			double max_value=m_k1*m_computer[tx][ty]+m_k2*m_player[tx][ty];
			for(int i=0;i<m_M;i++)
			{
				for(int j=0;j<m_M;j++)
				{
					if(max_value<m_k1*m_computer[i][j]+m_k2*m_player[i][j])
					{
						tx=i; ty=j;
						max_value=m_k1*m_computer[tx][ty]+m_k2*m_player[tx][ty];
					}
				}
			}
			m_current_x=tx;
			m_current_y=ty;
			m_qipan[tx][ty]=m_white;
			m_current_numbers++;
			return;
		}
	}
//end
}