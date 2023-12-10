#include "boot.h"

#include "bsp_qspi.h"
#include "bsp_led.h"
#include "bsp_usart.h"
#include "bsp_sd.h" 
#include "bsp_sdram.h" 





 void JumpToBootloader(void)
 {
 uint32_t i=0;
 void (*SysMemBootJump)(void); /* 声明一个函数指针 */
 __IO uint32_t BootAddr = 0x1FF09800; /* STM32H7 的系统 BootLoader 地址 */

 /* 关闭全局中断 */
 __disable_irq();
	 

 /* 关闭滴答定时器，复位到默认值 */
 SysTick->CTRL = 0;
 SysTick->LOAD = 0;
 SysTick->VAL = 0;

 /* 设置所有时钟到默认状态，使用 HSI 时钟 */
 HAL_RCC_DeInit();

 /* 关闭所有中断，清除所有中断挂起标志 */
 for (i = 0; i < 8; i++)
 {
 NVIC->ICER[i]=0xFFFFFFFF;
 NVIC->ICPR[i]=0xFFFFFFFF;
 }

 /* 使能全局中断 */
 __enable_irq();

 /* 跳转到系统 BootLoader，首地址是 MSP，地址+4 是复位中断服务程序地址 */
 SysMemBootJump = (void (*)(void)) (*((uint32_t *) (BootAddr + 4)));

 /* 设置主堆栈指针 */
 __set_MSP(*(uint32_t *)BootAddr);

 /* 在 RTOS 工程，这条语句很重要，设置为特权级模式，使用 MSP 指针 */
 __set_CONTROL(0);

 /* 跳转到系统 BootLoader */
 SysMemBootJump(); 


  while(1){
	
	}
	
}





