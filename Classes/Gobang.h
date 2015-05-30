#ifndef Kirito_Gobang_h
#define Kirito_Gobang_h
namespace Obito
{
	class Gobang
	{
	private:
		enum  //���г���
		{
			m_vacant=8, //��λ     ��ʹ�õ�ASCIi��
			m_black=4,  //����     ��ʹ�õ�ASCIi��
			m_white=79, //����     ��ʹ�õ�ASCIi��
			m_M=15      //Ĭ�����̹����15*15
		};
	private:
		char m_qipan[m_M][m_M];       //����
		double m_computer[m_M][m_M];  //���Է����ͱ�(�������Ӿ���ʱ�Ĺ����Ա�,��ÿһ��λ�õĹ���Ч��)
		double m_player[m_M][m_M];    //��ҷ����ͱ�(�������Ӿ���ʱ�ķ����Ա�,��ÿһ��λ�õķ���Ч��)
		int m_current_x,m_current_y;  //��ǰ���
		int m_current_numbers;        //������������������
		double m_k1,m_k2;             //���ԵĹ�������(����ϵ��,����ϵ��)
	private:   //˽��ʵ��
		void origin_luozi();           //����������������<=3ʱ,�����ڵ�ǰ��㸽���������
		bool will_win(int &x,int &y);  //�ж��Ƿ���ڿ�λ��ֱ����ʤ��(�����ڸô����̳���5��)
		bool will_fail(int &x,int &y); //�ж��Ƿ���ڿ�λ��ֱ���ưܵ�(���ô��ǵз���ֱ����ʤ��)

		void fill_m_computer(); //������д���Է����ͱ�
		void fill_m_player();   //������д��ҷ����ͱ�
		void calc_qixing_data(int i,int j,char qizi_color,int data[4][2][5]); //�����λ(i,j)��Χ��������
		void calc_qixing_value(int i,int j,char qizi_color,const int data[4][2][5]);
		//Ϊqizi_color����������ͱ��(i,j)����������Ч��ֵ(qizi_color��m_black or m_white)

		int single_live_four(int i,int j,char qizi_color);//����ĳ��λ(i,j)���м���������:�����Դﵽ������(���ϴ˵��)
		int single_live_three(int i,int j,char qizi_color);//����ĳ��λ(i,j)���м���������:�����Դﵽ������(���ϴ˵��)
		int single_dead_four(int i,int j,char qizi_color);//����ĳ��λ(i,j)���м���������:�����Դﵽ������(���ϴ˵��)
		bool double_live_four(int &x,int &y,char qizi_color);         //�ж��Ƿ����˫����
		bool live_four_and_live_three(int &x,int &y,char qizi_color); //�ж��Ƿ���ڻ�����
		bool double_dead_four(int &x,int &y,char qizi_color);         //�ж��Ƿ����˫����
		bool dead_four_and_live_three(int &x,int &y,char qizi_color); //�ж��Ƿ�������Ļ���
		bool live_four(int &x,int &y,char qizi_color);                //�ж��Ƿ���ڵ�����
		bool double_live_three(int &x,int&y,char qizi_color);         //�ж��Ƿ����˫����
	public:
		Gobang();
		~Gobang();
		Gobang(const Gobang &t);
		Gobang & operator = (const Gobang &t);
		//�ӿں���:
		void reset_m_k1k2(double k1=0.6,double k2=0.4); //���õ��ԵĹ���ϵ���ͷ���ϵ��
		int get_m_current_x()const;       //���ص�ǰ�����б��[0,m_M-1]
		int get_m_current_y()const;       //���ص�ǰ�����б��[0,m_M-1]
		int get_m_current_numbers()const; //���ص�ǰ��������
		int get_m_M()const;               //�������̵Ĺ��
		double get_m_k1()const;           //���ع���ϵ��
		double get_m_k2()const;           //���ط���ϵ��
		//��Ҫ����:
		int judge_winorfail()const;         //�ж�[ʤ,��,ƽ,δ��״̬]
		bool player_luozi(int x=0,int y=0); //�������
		void computer_luozi();              //��������
	};
}
#endif