
/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,Ұ��Ƽ�
 *     All rights reserved.
 *     �������ۣ�Ұ���ѧ��̳ http://www.chuxue123.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ����Ұ��Ƽ����У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣��Ұ��Ƽ��İ�Ȩ������
 *
 * @file       main.c
 * @brief      Ұ��K60 ƽ̨������
 * @author     Ұ��Ƽ�
 * @version    v5.0
 * @date       2013-08-28
 */

#include "common.h"
#include "include.h"
#include "other_parts.h"
#include  "MK60_it.h"



void  main(void)
{   
  DisableInterrupts;
  /*************�ж����ȼ�����*******************/
  set_priority_irq();
  LED_init();
  PWM();
  uart_init (UART4, 115200);  
  /*************����ͷ����жϳ�ʼ��*******************/
  init_camera();
  //DMAͨ��0��ʼ����PTA27����Դ(Ĭ��������)��Դ��ַΪPTE_B0_IN��Ŀ�ĵ�ַΪ��IMG_BUFF��ÿ�δ���1Byte
  /*************PIT����DMA�������ʼ��*******************/
  init_remain();
  EnableInterrupts;
  while(!nrf_init());
  set_vector_handler(PORTA_VECTORn,PORTA_IRQHandler);
  enable_irq(PORTA_IRQn); 
  speed_setting();
  LCD_Show_Number(22,1,speed_show);
  LCD_Show_Number(22,2,former_flag);
  DELAY_MS(500);       
  
  if(former_flag==1)
  {
    
   sprintf((char*)buff,"%s%d",str,1);
   nrf_tx(buff,DATA_PACKET);
      while(nrf_tx_state() == NRF_TXING);
   if( NRF_TX_OK == nrf_tx_state () )
   {  
   buff[0]=0;
   }
  }
  else
  {
   while(relen==0)
   {
     relen=nrf_rx(buff,DATA_PACKET);
   }
   buff[0]=0;
  }
    
  while(1)
  {  
     if(img_flag == 3)
    {
      img_flag = 4;      
      PORTC_ISFR = ~0;               //д1���жϱ�־λ(����ģ���Ȼ�ص���һ���жϾ����ϴ����ж�)
      enable_irq(PORTC_IRQn);
      img_flag = 1;
   // gpio_init (PTB22, GPO,1);
  //    LP_image();
    /********���ڷ��ͳ���*************/
  //   send_image();
   /*************Һ������ʾ����**********/
   //LCD_show_image(); 
  /**********ͼ�������***********/
     starting_count++;         
     sanjiaochaoche_send_message();
     bottom_line();
     tracking();
      starting_line(500,40,starting_line_get());     //ͣ���߳���
     shizijudge();
     otherjudge();
     chaoche_judge();
     error_steerout();
     LCD_Show();
  /**********����PID����***********/ 
     chaosheng_PID();
    }
  }
} 

