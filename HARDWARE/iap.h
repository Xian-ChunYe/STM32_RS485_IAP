#ifndef __IAP_H__
#define __IAP_H__
#include "sys.h"  

typedef  void (*iapfun)(void);				//定义一个函数类型的参数.

#define FLASH_APP_Info_ADDR		0X0800FC00      //APP属性放置区地址
#define FLASH_APP_ADDR		    0x08010000  	//第一个应用程序起始地址(存放在FLASH)
#define APPLICATION_ADDRESS	FLASH_APP_ADDR									//保留0X08000000~0X0800FFFF的空间为IAP使用

#define USER_FLASH_LAST_PAGE_ADDRESS  0x0800F800
#define USER_FLASH_END_ADDRESS        0x08040000 /* 64 KBytes */
/* define the user application size */
#define USER_FLASH_SIZE   (USER_FLASH_END_ADDRESS - APPLICATION_ADDRESS + 1)


void iap_load_app(u32 appxaddr);			//执行flash里面的app程序
void iap_load_appsram(u32 appxaddr);		//执行sram里面的app程序
void iap_write_appbin(u32 appxaddr,u8 *appbuf,u32 applen);	//在指定地址开始,写入bin
#endif







































