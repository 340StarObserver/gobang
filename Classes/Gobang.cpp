#include"Gobang.h"
#include<cmath>
namespace Obito
{
	Gobang::Gobang()
	{
		//Ĭ�Ϲ���
		//��������
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
		//����Ĭ�ϵĵ��Թ���ϵ��
		m_k1=0.6;
		m_k2=0.4;
		//��ǰ��������
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
		//�Զ�����ԵĹ���ϵ��
		//�Ƚ��й������ݵĺϷ�����֤,�Ƿ񶼡�[0,1],�ҹ���ϵ��֮��Ϊ1
		if(k1>=0&&k1<=1&&k2>=0&&k2<=1&&std::fabs(k1+k2-1)<0.0001)
		{
			m_k1=k1;
			m_k2=k2;
		}
	}

	//��õ�ǰ������
	int Gobang::get_m_current_numbers()const{ return m_current_numbers; }

	//������̵Ĺ��
	int Gobang::get_m_M()const{ return static_cast<int>(m_M); }

	//��õ�ǰ�����к�
	int Gobang::get_m_current_x()const{ return m_current_x; }

	//��õ�ǰ�����к�
	int Gobang::get_m_current_y()const{ return m_current_y; }

	//��õ��ԵĹ���ϵ��
	double Gobang::get_m_k1()const{ return m_k1; }

	//��õ��Եķ���ϵ��
	double Gobang::get_m_k2()const{ return m_k2; }

	bool Gobang::player_luozi(int x,int y)
	{//�������
		if(x<0||x>=m_M||y<0||y>=m_M)       //����֤����Ƿ�Խ��
		{
			return false;
		}
		else if(m_qipan[x][y]!=m_vacant)   //����֤����Ƿ��ǿ������ӵĿ�λ
		{
			return false;
		}
		else                               //������������
		{
			//���µ�ǰ���λ��
			m_current_x=x;
			m_current_y=y;
			//��������
			m_qipan[m_current_x][m_current_y]=m_black;
			//����������
			m_current_numbers++;
			return true;
		}
	}

	bool Gobang::will_win(int &x,int &y)
	{
		//�ȱ��浱ǰ���(�������ݻָ�)
		int tempx=m_current_x;
		int tempy=m_current_y;
		//�����������̵Ŀ�λ,�������(i,j)�Ƿ����������Ӯ
		for(int i=0;i<m_M;i++)
		{
			for(int j=0;j<m_M;j++)    
			{
				if(m_qipan[i][j]==m_vacant) //�ǿ�λ�Ž����ж�
				{
					//�������(��������,��ǰ���)
					m_qipan[i][j]=m_white;    
			    	m_current_x=i;
			    	m_current_y=j;
			    	if(judge_winorfail()==-1) //����Ӯ�������
			    	{
				    	//���ݻָ�(��ԭ��ԭ�ȵ����̺͵�ǰ���)
				    	m_current_x=tempx;
				    	m_current_y=tempy;
				    	m_qipan[i][j]=m_vacant; 
				    	//��¼������(i,j)
				    	x=i; y=j;
				    	return true;
			    	}
					//���ô�������ʤ��,��ԭ�ô�Ϊԭ�ȵĿ�λ,������֤��Ŀ�λ
			    	m_qipan[i][j]=m_vacant;
				}
			}
		}
		//�������� ���ڵ�����˵����ʤ��,��ԭ���ݲ�return false
		m_current_x=tempx;
		m_current_y=tempy;
		return false;
	}

	bool Gobang::will_fail(int &x,int &y)
	{
		//�ȱ��浱ǰ���(�������ݻָ�)
		int tempx=m_current_x;
		int tempy=m_current_y;
		//�����������̵Ŀ�λ,�������������(i,j),�����Ƿ��������
		for(int i=0;i<m_M;i++)
		{
			for(int j=0;j<m_M;j++)    
			{
				if(m_qipan[i][j]==m_vacant) //�ǿ�λ�Ž����ж�
				{
					//�������(��������,��ǰ���)
					m_qipan[i][j]=m_black;
			    	m_current_x=i;
			    	m_current_y=j;
			    	if(judge_winorfail()==1) //�����伴���Ӯ
			    	{
						//���ݻָ�(��ԭ��ԭ�ȵ����̺͵�ǰ���)
				    	m_current_x=tempx;
				    	m_current_y=tempy;
				    	m_qipan[i][j]=m_vacant; 
				    	//��¼������(i,j)
				    	x=i; y=j;
				    	return true;
			    	}
					//���ô������ưܵ�,��ԭ�ô�Ϊԭ�ȵĿ�λ,������֤��Ŀ�λ
			    	m_qipan[i][j]=m_vacant;
				}
			}
		}
		//�������� ���ڵ�����˵���ưܵ�,��ԭ���ݲ�return false
		m_current_x=tempx;
		m_current_y=tempy;
		return false;
	}

	int Gobang::judge_winorfail()const
	{
		//�ж�ʤ��(�����Ϊ��׼.����2:��δ�ֳ�ʤ�� 1:���ʤ 0:ƽ�� -1:�����)
		/* 
		  1:�ȿ���ǰ����Ǻ���(���)or����(����)or��λ(˵����Ϸ�տ�ʼ,�򷵻�2)
		    ���Ǻ���:���������Χ���ĸ������ϵ�����������,����ĳ��������5,�򷵻�1
			���ǰ���:���������Χ���ĸ������ϵ�����������,����ĳ��������5,�򷵻�-1
			[�ĸ�����:ˮƽ,��ֱ,45��,135��]
		  2:����������,��ƽ�ַ���0
		  3:����������,����δ��ʤ����״̬,����2
		*/
		if(m_qipan[m_current_x][m_current_y]==m_black) //����ǰ����Ǻ���
		{
			//�ȼ���ˮƽ����
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
			//�ټ�����ֱ����
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
			//�ټ���45�㷽��
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
			//�ټ���135�㷽��
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
		else if(m_qipan[m_current_x][m_current_y]==m_white)//����ǰ����ǰ���
		{
			//�ȼ���ˮƽ����
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
			//�ټ�����ֱ����
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
			//�ټ���45�㷽��
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
			//�ټ���135�㷽��
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
		else //��ǰ����ǿ�λ,˵����Ϸ�տ�ʼ
			return 2;
		//�������ж������Ƿ�����
		int not_vacant=0;
		for(int i=0;i<m_M;i++)
		{
			for(int j=0;j<m_M;j++)
			{
				if(m_qipan[i][j]!=m_vacant)
					not_vacant++;
			}
		}
		if(not_vacant==m_M*m_M) //��������
			return 0; //ƽ��
		return 2; //������������,����δ��ʤ����״̬
	}

	void Gobang::origin_luozi()
	{
		//һ��ʼ���ӷǳ��ٵ�ʱ��,���Ե�����:
		if(m_current_numbers==0)
		{//���ǿ���,�����ֱ���������м����Ӽ���
			m_current_x=static_cast<int>(m_M)/2;
			m_current_y=m_current_x;
			m_qipan[m_current_x][m_current_y]=m_white;
			m_current_numbers++;
			return;
		}
		if(m_current_numbers<=3)
		{//�����ǿ���,�ҵ�ǰ����������������(<=3),���õ��������������Χ������Ӽ���
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
	{//����ĳ��λ��(i,j),����������ɫqizi_color��˵,����Χ����������
		/*
		һ:
		data[0][0][0]: ˮƽ������,(i,j)���,�����һ��������qizi_color��ͬ��ɫ������������Ŀ
		data[0][0][1]: ˮƽ������,(i,j)���,�����һ��������λ,������λ����Ŀ
		data[0][0][2]: ˮƽ������,(i,j)���,����ڶ���������qizi_color��ͬ��ɫ������������Ŀ
		data[0][0][3]: ˮƽ������,(i,j)���,����ڶ���������λ,������λ����Ŀ
		data[0][0][4]: ˮƽ������,(i,j)���,���������������qizi_color��ͬ��ɫ������������Ŀ
		data[0][1][0]~data[0][1][4] ����ˮƽ����(i,j)�Ҳ����ҵ����
		��:
		data[1][0][0]: ��ֱ������,(i,j)�ϲ�,���ϵ�һ��������qizi_color��ͬ��ɫ������������Ŀ
		data[1][0][1]: ��ֱ������,(i,j)�ϲ�,���ϵ�һ��������λ,������λ����Ŀ
		data[1][0][2]: ��ֱ������,(i,j)�ϲ�,���ϵڶ���������qizi_color��ͬ��ɫ������������Ŀ
		data[1][0][3]: ��ֱ������,(i,j)�ϲ�,���ϵڶ���������λ,������λ����Ŀ
		data[1][0][4]: ��ֱ������,(i,j)�ϲ�,���ϵ�����������qizi_color��ͬ��ɫ������������Ŀ
		data[1][1][0]~data[1][1][4] ������ֱ����(i,j)�²����µ����
		��:
		data[2][0][0]: ƽ�����Խ��߷�����,(i,j)���ϲ�,�����ϵ�һ��������qizi_color��ͬ��ɫ������������Ŀ
		data[2][0][1]: ƽ�����Խ��߷�����,(i,j)���ϲ�,�����ϵ�һ��������λ,������λ����Ŀ
		data[2][0][2]: ƽ�����Խ��߷�����,(i,j)���ϲ�,�����ϵڶ���������qizi_color��ͬ��ɫ������������Ŀ
		data[2][0][3]: ƽ�����Խ��߷�����,(i,j)���ϲ�,�����ϵڶ���������λ,������λ����Ŀ
		data[2][0][4]: ƽ�����Խ��߷�����,(i,j)���ϲ�,�����ϵ�����������qizi_color��ͬ��ɫ������������Ŀ
		data[2][1][0]~data[2][1][4] ����ƽ�����Խ��߷���(i,j)���²������µ����
		��:
		data[3][0][0]: ��ֱ���Խ��߷�����,(i,j)���ϲ�,�����ϵ�һ��������qizi_color��ͬ��ɫ������������Ŀ
		data[3][0][1]: ��ֱ���Խ��߷�����,(i,j)���ϲ�,�����ϵ�һ��������λ,������λ����Ŀ
		data[3][0][2]: ��ֱ���Խ��߷�����,(i,j)���ϲ�,�����ϵڶ���������qizi_color��ͬ��ɫ������������Ŀ
		data[3][0][3]: ��ֱ���Խ��߷�����,(i,j)���ϲ�,�����ϵڶ���������λ,������λ����Ŀ
		data[3][0][4]: ��ֱ���Խ��߷�����,(i,j)���ϲ�,�����ϵ�����������qizi_color��ͬ��ɫ������������Ŀ
		data[3][1][0]~data[3][1][4] ���Ǵ�ֱ���Խ��߷���(i,j)���²������µ����
		*/
		if(m_qipan[i][j]!=m_vacant) return; //�����ǿ�λ�򲻼���
		//������
		for(int p=0;p<4;p++)
		{
			for(int q=0;q<2;q++)
			{
				for(int k=0;k<5;k++)
					data[p][q][k]=0;
			}
		}
		int tx=0,ty=0;
		//�ȼ���ˮƽ����
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
		//�ټ�����ֱ����
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
		//�ټ���ƽ�������Խ��߷���
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
		//�ټ��㴹ֱ�����Խ��߷���
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
		//����ĳλ�õ�֮ǰ����õ���Χ����������,�����λ�õĹ���ϵ��or����ϵ��
		//��qizi_colorΪ����,�������ڵ�����˵,�õ�ķ��ؼ�ֵ
		//��qizi_colorΪ����,�������ڵ�����˵,�õ�Ĺ�����ֵ
		if(m_qipan[i][j]!=m_vacant) return; //�����ǿ�λ�򲻼���
		//������
		if(qizi_color==m_white) m_computer[i][j]=0;
		else                    m_player[i][j]=0;
		//Ȼ������ĸ������ϵ���ֵ
		for(int k=0;k<4;k++)  //���Ĵ�ѭ������ʾ�ĸ�����
		{
			//���岻ͬ���������ݴ����˲�ͬ�Ĺ�����ֵ(�����Զ���,�˴��Ҳο���һЩʵ�����ݲ�����Щ�޸�)
			//�˴����Ծ������չ (=v=)
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
		} //������ ��k ����
	}

	void Gobang::fill_m_computer()
	{
		//������д ���ڵ�����˵�Ĺ����Լ�ֵ��
		int DATA[4][2][5];
		for(int i=0;i<m_M;i++)
		{
			for(int j=0;j<m_M;j++)
			{
				m_computer[i][j]=0; //������
				if(m_qipan[i][j]==m_vacant) //�ǿ�λ�ż���
				{
					//�ȼ���(i,j)λ����Χ ����ڰ�����˵�� ��������
					calc_qixing_data(i,j,m_white,DATA);
					//Ȼ�����(i,j)λ�� ����ڰ���(������)��˵�� ������ֵ
					calc_qixing_value(i,j,m_white,DATA);
				} 
			}
		}
	}

	void Gobang::fill_m_player()
	{
		//������д ���ڵ�����˵�ķ����Լ�ֵ��(���������Ϊ��ҷ��Ĺ�����ֵ��)
		int DATA[4][2][5];
		for(int i=0;i<m_M;i++)
		{
			for(int j=0;j<m_M;j++)
			{
				m_player[i][j]=0; //������
				if(m_qipan[i][j]==m_vacant) //�ǿ�λ�ż���
				{
					//�ȼ���(i,j)λ����Χ ����ں�����˵�� ��������
					calc_qixing_data(i,j,m_black,DATA);
					//Ȼ�����(i,j)λ�� ����ں���(�����)��˵�Ĺ�����ֵ
					//Ҳ������ڰ���(����)��˵�ķ��ؼ�ֵ
					calc_qixing_value(i,j,m_black,DATA);
				}
			}
		}
	}

	int Gobang::single_live_four(int i,int j,char qizi_color)
	{//����ĳλ�û��ĵ���Ŀ
		//���õ㲻�ǿ�λ,��ֱ������
		if(m_qipan[i][j]!=m_vacant) return 0;
		int result=0;
		int DATA[4][2][5];
		//�ȼ����λ�� �����qizi_color��ɫ������˵�� ��������
		calc_qixing_data(i,j,qizi_color,DATA);
		//������(ˮƽ,��ֱ,45��,135�㷽����),�м����������Ǵ��ڻ��ĵ�
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
	{//����ĳλ�û�������Ŀ
		//���õ㲻�ǿ�λ,��ֱ������
		if(m_qipan[i][j]!=m_vacant) return 0;
		int result=0;
		int DATA[4][2][5];
		//�ȼ����λ�� �����qizi_color��ɫ������˵�� ��������
		calc_qixing_data(i,j,qizi_color,DATA);
		//������(ˮƽ,��ֱ,45��,135�㷽����),�м����������Ǵ��ڻ�����
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
	{//����ĳλ�����ĵ���Ŀ
		//���õ㲻�ǿ�λ,��ֱ������
		if(m_qipan[i][j]!=m_vacant) return 0;
		int result=0;
		int DATA[4][2][5];
		//�ȼ����λ�� �����qizi_color��ɫ������˵�� ��������
		calc_qixing_data(i,j,qizi_color,DATA);
		//������(ˮƽ,��ֱ,45��,135�㷽����),�м����������Ǵ������ĵ�
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
	{//�ж��Ƿ����ĳ����λ,�ڸô������ܲ���˫���ĵ�Ч��,����x,y������¼������λ��
		for(int i=0;i<m_M;i++)
		{
			for(int j=0;j<m_M;j++)
			{
				//���(i,j)�ǿ�λ���ܹ�����˫���ĵ�����
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
	{//�ж��Ƿ����ĳ����λ,�ڸô������ܲ��������ĵ�Ч��,����x,y������¼������λ��
		for(int i=0;i<m_M;i++)
		{
			for(int j=0;j<m_M;j++)
			{
				//���(i,j)�ǿ�λ���ܹ�������ĺͻ���������
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
	{//�ж��Ƿ����ĳ����λ,�ڸô������ܲ���˫���ĵ�Ч��,����x,y������¼������λ��
		for(int i=0;i<m_M;i++)
		{
			for(int j=0;j<m_M;j++)
			{
				//���(i,j)�ǿ�λ���ܹ�����˫���ĵ�����
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
	{//�ж��Ƿ����ĳ����λ,�ڸô������ܲ������Ļ�����Ч��,����x,y������¼������λ��
		for(int i=0;i<m_M;i++)
		{
			for(int j=0;j<m_M;j++)
			{
				//���(i,j)�ǿ�λ���ܹ��������ĺͻ���������
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
	{//�ж��Ƿ����ĳ����λ,�ڸô������ܲ��������ĵ�Ч��,����x,y������¼������λ��
		for(int i=0;i<m_M;i++)
		{
			for(int j=0;j<m_M;j++)
			{
				//���(i,j)�ǿ�λ���ܹ�������ĵ�����
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
	{//�ж��Ƿ����ĳ����λ,�ڸô������ܲ���˫������Ч��,����x,y������¼������λ��
		for(int i=0;i<m_M;i++)
		{
			for(int j=0;j<m_M;j++)
			{
				//���(i,j)�ǿ�λ���ܹ�����˫����������
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
	{//����ִ�����ӵ��߼�����
		/*
		(�жϵ��Ⱥ�˳����������):

		��������������3,�ڵ�ǰ�����Χ����

		������  ֱ����ʤ��          ,������������
		������  ֱ���ưܵ�          ,������������

		������  ����˫����    ��λ��,������������
		������  ����������    ��λ��,������������
		������  ����˫����    ��λ��,������������
		������  �������Ļ���  ��λ��,������������
		������  ����������    ��λ��,������������

		������  �з�˫����    ��λ��,������������
		������  �з�������    ��λ��,������������
		������  �з�˫����    ��λ��,������������
		������  �з����Ļ���  ��λ��,������������
		������  �з�������    ��λ��,������������

		������  ����˫����    ��λ��,������������

		������  �з�˫����    ��λ��,������������

		�����������������,��������̵Ĺ�����ֵ��and���ؼ�ֵ��,
		�ٸ��ݹ���ϵ��,�������Ȩƽ���������Ǹ�λ��,������������

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