#include "main.h"

/*全局变量的定义*/
int CNT1=0,CNT2=0,Time=0;
int16_t EncoderA=0,EncoderB=0;   //10MS内的脉冲数
int16_t S_EncoderA=0,S_EncoderB=0;//脉冲累加
/*全局变量的引用（extern）*/

/*
函数功能：HAL库定时器读取编码器的值
入口参数：无
返回值：无
*/
void Reading(int16_t *L_Val,int16_t *R_Val)
{
	*R_Val=TIM5->CNT; //读取计数
	TIM5->CNT = 0;
	*L_Val=-TIM4->CNT;
	TIM4->CNT = 0;
}

/*
函数功能：HAL库定时器的回调函数
入口参数：无
返回值：无
*/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance==TIM6)   //10ms
	{
		CNT1++;
//		Reading(&EncoderA,&EncoderB);   //读取完成，自动清零
//		S_EncoderB+=EncoderB;
//		S_EncoderA+=EncoderA;
//		if(keyflag==1)
//		{
//			speed_pid();
//			addPID.target_val = 10;	
//			add2PID.target_val = 10;
//			printf("左轮：%d\r\n",EncoderA);
//			printf("右轮：%d\r\n",EncoderB);
//		}
//		if(keyflag==2)
//		{
//			speed_pid();
//			PWMA_Set(0);
//			PWMB_Set(0);
//			printf("左轮：%d\r\n",EncoderA);
//			printf("右轮：%d\r\n",EncoderB);
//		}
//		if(keyflag==3)
//		{
//			Motor_Control_ClosedLoop();
//			printf("左轮：%d\r\n",S_EncoderA);
//			printf("右轮：%d\r\n",S_EncoderB);
//		}
	}
	
	if(htim->Instance==TIM7)   //200ms
	{
		CNT2++;
		ADC_Vol=(double)StartAndGetOneResult()*3.3*11/4096;//读取转换的AD值
		ADC_Vol1=ADC_Vol*100;
		ADC_Vol2=(int)ADC_Vol1%100;
		//OLED_Show();
		if(CNT2==5)
		{
			Time++;
			CNT2=0;
		}
	}
  if(htim->Instance==TIM8)   //控制步进电机的定时器
  {
     Step_Run_Flag = 1;                             /* 标志位置一 */
		 if(StepMotor_Num==1)    //判断是哪一个步进电机
		 {
				Pulse_Add.StepMotor1++;   //脉冲的累加数
		 }
		 else if(StepMotor_Num==2)
		 {
				Pulse_Add.StepMotor2++;   //脉冲的累加数
		 }
		 else if(StepMotor_Num==3)
		 {
				Pulse_Add.StepMotor3++;   //脉冲的累加数
		 }
		 else if(StepMotor_Num==4)
		 {
				Pulse_Add.StepMotor4++;   //脉冲的累加数
		 }
		 
     g_stepper.pulse_count--;                    /* 每一个完整的脉冲就-- */
     if(g_stepper.dir == CW)
     {
        g_stepper.add_pulse_count++;             /* 绝对位置++ */
     }else
     {
        g_stepper.add_pulse_count--;             /* 绝对位置-- */
     }

     if(g_stepper.pulse_count<=0 && StepMotor_Num == 1)                /* 当脉冲数等于0的时候 代表需要发送的脉冲个数已完成，停止定时器输出 */
     {
         printf("累计旋转的角度:%d\r\n",(int)(g_stepper.add_pulse_count*MAX_STEP_ANGLE));  /* 打印累计转动了多少角度 */
         stepper_stop(STEPPER_MOTOR_1);          /* 停止接口一输出 */
         Step_Run_Flag = 0;
     }
     else if(g_stepper.pulse_count<=0 && StepMotor_Num == 2)                /* 当脉冲数等于0的时候 代表需要发送的脉冲个数已完成，停止定时器输出 */
     {
         printf("累计旋转的角度:%d\r\n",(int)(g_stepper.add_pulse_count*MAX_STEP_ANGLE));  /* 打印累计转动了多少角度 */
         stepper_stop(STEPPER_MOTOR_2);          /* 停止接口一输出 */
         Step_Run_Flag = 0;
     }
     else if(g_stepper.pulse_count<=0 && StepMotor_Num == 3)                /* 当脉冲数等于0的时候 代表需要发送的脉冲个数已完成，停止定时器输出 */
     {
         printf("累计旋转的角度:%d\r\n",(int)(g_stepper.add_pulse_count*MAX_STEP_ANGLE));  /* 打印累计转动了多少角度 */
         stepper_stop(STEPPER_MOTOR_3);          /* 停止接口一输出 */
         Step_Run_Flag = 0;
     }
     else if(g_stepper.pulse_count<=0 && StepMotor_Num == 4)                /* 当脉冲数等于0的时候 代表需要发送的脉冲个数已完成，停止定时器输出 */
     {
         printf("累计旋转的角度:%d\r\n",(int)(g_stepper.add_pulse_count*MAX_STEP_ANGLE));  /* 打印累计转动了多少角度 */
         stepper_stop(STEPPER_MOTOR_4);          /* 停止接口一输出 */
         Step_Run_Flag = 0;
     }
   } 	
}
