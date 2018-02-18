#include "speed.h"

float speedL , speedR ,speed_error,speed_need=300,speed_now; 
double speed_I=0;
float I_MOVE=1;
float PWM_SPEED,PWM_SPEED_OUT,PWM_SPEED_AGO;
float MOVE=12;
extern int16 Angle_Need;
extern uint8 cnt;
extern u8 P_S,I_S;
extern float PWM_ANGLE;

//�ٶȻ�:һ���Ƕȶ�Ӧһ���ٶȣ��ٶȻ�ͨ�����������ٶȲ����Ƕȣ��Ӷ�������Ӧ�ٶ� ��PWM������ĵ�ѹ��Чֵ�����Ƶ��Ǽ��ٶȣ��������ٶȣ�
/**************e������****************/
/*�����ֱ�� 42.80mm ������ 104��
 *���������� ֱ�� 16mm ������ 40��
*��ֱ̥��63.5mm piȡ3.1415 �ܳ�Ϊ0.19949m
 *���������� 500��ŷķ�� ȡ��ʱ�� 100ms
 */
/***********************************/
void duty_speed()                                  //�����¼��ٽ׶�ʱ�䣬��ʱ�䳣��T1
{
	speedL = ftm_count_get(ftm0);  //�õ�����������
  speedR = ftm_count_get(ftm1);  //�õ�����������
  ftm_count_clean(ftm0);         //��������
  ftm_count_clean(ftm1);
          
	if(gpio_get(DIR_L)==0)        //�жϷ���
		speedL = -speedL;
	if(gpio_get(DIR_R)==1)
		speedR = -speedR;
	
	speed_now = (speedL + speedR)/2;  //�õ���ǰ������
	speed_error = (speed_need - speed_now);    //�õ��ٶ�ƫ��
        
	//���ñ��ٻ���ϵ��
if((speed_error>=0?speed_error:-speed_error)<I_MIN)
		I_MOVE = 1;
	else if(((speed_error>=0?speed_error:-speed_error)>=I_MIN) && ((speed_error>=0?speed_error:-speed_error)<I_MAX))
		I_MOVE = (I_MAX-(speed_error>=0?speed_error:-speed_error))/(I_MAX-I_MIN);
	else if((speed_error>=0?speed_error:-speed_error)>=I_MAX)
		I_MOVE = 0;
	/*if(speed_error>25)
			I_MOVE = 0;*/	
	
//	if((flag_I==0)||(PWM>0&&speed_I<0)||(PWM<0&&speed_I>0))   //��û�б��ͻ�����֣����ۼӻ�����
		speed_I += speed_error*I_MOVE;          //���ٻ���          
	
	if(speed_I>1000)                          //�����޷�
		speed_I = 1000;
	else if(speed_I<-1000)
		speed_I = -1000;
	
	PWM_SPEED_AGO = PWM_SPEED;
	//20��������������޸�ԭ������������Ϊб������
	PWM_SPEED = P_SPEED*speed_error*0.01 + I_SPEED*speed_I*0.001; //��������ٶȣ������Ƕȣ���	���ȵ�I�����P
	PWM_SPEED_OUT = (PWM_SPEED-PWM_SPEED_AGO)*cnt/20 + PWM_SPEED_AGO;
	
	//�޷��ٶȻ�������Ϊ�ǶȻ���MOVE��
	if(PWM_SPEED_OUT >MOVE*(PWM_ANGLE>0?PWM_ANGLE:-PWM_ANGLE))
		PWM_SPEED_OUT = MOVE*(PWM_ANGLE>0?PWM_ANGLE:-PWM_ANGLE);
	else if(PWM_SPEED_OUT < -MOVE*(PWM_ANGLE>0?PWM_ANGLE:-PWM_ANGLE))
		PWM_SPEED_OUT = -MOVE*(PWM_ANGLE>0?PWM_ANGLE:-PWM_ANGLE);

}




