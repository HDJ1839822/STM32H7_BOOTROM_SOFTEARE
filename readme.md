## STM32 bootloader

STM32H7的启动方式分为两种，从FLASH启动，从BOOROM启动,由外部BOOT引脚配置启动，由于我使用的这一块开发板没有引出BOOT引脚，没办法硬件启动，所以使用一个boot程序跳转到BOOTROM区域启动

- FLASH启动最常用，即地址0x0800 0000
- BOOROM启动主要用于下载程序：该起始地址为：0x1FF09800（不同系列对应地址不同）

BOOROM的启动地址可查看手册：AN2606，该手册包含了STM32所有系列自举程序说明



该引导程序适用于STM32H750XB，由与该芯片只falsh有一个扇区，将引导程序下载至该扇区从而跳转到BOOTROM的地址上

通过cubeprogrammer完成USB/USART的下载（覆盖引导程序扇区）因此该引导程序只能生效一次

![image-20231210173030162](https://newbie-typora.oss-cn-shenzhen.aliyuncs.com/TyporaJPG/image-20231210173030162.png)于该flash只有一个





AN2606如下：

- 需要注意起始地址以及适配的下载引脚

![image-20231210172610055](https://newbie-typora.oss-cn-shenzhen.aliyuncs.com/TyporaJPG/image-20231210172610055.png)![image-20231210172628474](https://newbie-typora.oss-cn-shenzhen.aliyuncs.com/TyporaJPG/image-20231210172628474.png)![image-20231210172650092](https://newbie-typora.oss-cn-shenzhen.aliyuncs.com/TyporaJPG/image-20231210172650092.png)![image-20231210172720696](https://newbie-typora.oss-cn-shenzhen.aliyuncs.com/TyporaJPG/image-20231210172720696.png)

此处省略部分内容

![image-20231210172802056](https://newbie-typora.oss-cn-shenzhen.aliyuncs.com/TyporaJPG/image-20231210172802056.png)

下表描述了选择机制，注意下载时候如果同时连接USART和USB优先选择USART,所以如果要使用USB，确保无其他连接

![image-20231210180432988](https://newbie-typora.oss-cn-shenzhen.aliyuncs.com/TyporaJPG/image-20231210180432988.png)



展示：

- 下载boot程序后复位即可开启USB

![image-20231210175602168](https://newbie-typora.oss-cn-shenzhen.aliyuncs.com/TyporaJPG/image-20231210175602168.png)

- 下载boot程序后复位即可开启USART

![image-20231210180252704](https://newbie-typora.oss-cn-shenzhen.aliyuncs.com/TyporaJPG/image-20231210180252704.png)



下载APP后复位LED闪烁，此时boot程序被覆盖失效





跳转程序如下：

```c

 void JumpToBootloader(void)
 {
 uint32_t i=0;
 void (*SysMemBootJump)(void); /* 声明一个函数指针 */
 __IO uint32_t BootAddr = 0x1FF09800; /* STM32H74xxx/75xxx 的系统 BootLoader 地址 */

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

```



