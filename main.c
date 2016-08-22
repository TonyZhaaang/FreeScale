
/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,野火科技
 *     All rights reserved.
 *     技术讨论：野火初学论坛 http://www.chuxue123.com
 *
 *     除注明出处外，以下所有内容版权均属野火科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留野火科技的版权声明。
 *
 * @file       main.c
 * @brief      野火K60 平台主程序
 * @author     野火科技
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
  /*************中断优先级更改*******************/
  set_priority_irq();
  LED_init();
  PWM();
  uart_init (UART4, 115200);  
  /*************摄像头相关中断初始化*******************/
  init_camera();
  //DMA通道0初始化，PTA27触发源(默认上升沿)，源地址为PTE_B0_IN，目的地址为：IMG_BUFF，每次传输1Byte
  /*************PIT及其DMA计脉冲初始化*******************/
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
      PORTC_ISFR = ~0;               //写1清中断标志位(必须的，不然回导致一开中断就马上触发中断)
      enable_irq(PORTC_IRQn);
      img_flag = 1;
   // gpio_init (PTB22, GPO,1);
  //    LP_image();
    /********串口发送程序*************/
  //   send_image();
   /*************液晶屏显示程序**********/
   //LCD_show_image(); 
  /**********图像处理程序***********/
     starting_count++;         
     sanjiaochaoche_send_message();
     bottom_line();
     tracking();
      starting_line(500,40,starting_line_get());     //停车线程序
     shizijudge();
     otherjudge();
     chaoche_judge();
     error_steerout();
     LCD_Show();
  /**********超声PID程序***********/ 
     chaosheng_PID();
    }
  }
} 

