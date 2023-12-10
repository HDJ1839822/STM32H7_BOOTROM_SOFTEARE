#include "boot.h"

#include "bsp_qspi.h"
#include "bsp_led.h"
#include "bsp_usart.h"
#include "bsp_sd.h" 
#include "bsp_sdram.h" 





 void JumpToBootloader(void)
 {
 uint32_t i=0;
 void (*SysMemBootJump)(void); /* ����һ������ָ�� */
 __IO uint32_t BootAddr = 0x1FF09800; /* STM32H7 ��ϵͳ BootLoader ��ַ */

 /* �ر�ȫ���ж� */
 __disable_irq();
	 

 /* �رյδ�ʱ������λ��Ĭ��ֵ */
 SysTick->CTRL = 0;
 SysTick->LOAD = 0;
 SysTick->VAL = 0;

 /* ��������ʱ�ӵ�Ĭ��״̬��ʹ�� HSI ʱ�� */
 HAL_RCC_DeInit();

 /* �ر������жϣ���������жϹ����־ */
 for (i = 0; i < 8; i++)
 {
 NVIC->ICER[i]=0xFFFFFFFF;
 NVIC->ICPR[i]=0xFFFFFFFF;
 }

 /* ʹ��ȫ���ж� */
 __enable_irq();

 /* ��ת��ϵͳ BootLoader���׵�ַ�� MSP����ַ+4 �Ǹ�λ�жϷ�������ַ */
 SysMemBootJump = (void (*)(void)) (*((uint32_t *) (BootAddr + 4)));

 /* ��������ջָ�� */
 __set_MSP(*(uint32_t *)BootAddr);

 /* �� RTOS ���̣�����������Ҫ������Ϊ��Ȩ��ģʽ��ʹ�� MSP ָ�� */
 __set_CONTROL(0);

 /* ��ת��ϵͳ BootLoader */
 SysMemBootJump(); 


  while(1){
	
	}
	
}





