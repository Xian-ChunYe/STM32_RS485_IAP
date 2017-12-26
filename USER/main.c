#include "sys.h"

struct
{
    u16 is_boot;
    u8 file_name[256];
    u16 file_len;
}boot_info;

extern u16 USART_RX_CNT;
void Main_Menu(void);
void SerialDownload(void);

uint8_t tab_1024[1024] ={0};
  
int main(void)
{	
    uint8_t Number[10] = {0};
    int8_t delay_time=8;
    uint8_t key = 0; 
    u16 UpDate_flsh=0;
    
    delay_init();	    //延时函数初始化	  
    uart_init(115200);
    
    STMFLASH_Read(FLASH_APP_Info_ADDR,&boot_info.is_boot,1); 
    
    if(boot_info.is_boot == 0X5555) 
    {
       if(((*(vu32*)(FLASH_APP_ADDR+4))&0xFF000000)==0x08000000)//判断是否为0X08XXXXXX.
       {	 
          iap_load_app(FLASH_APP_ADDR);//执行FLASH APP代码
       }
    }
    else if(boot_info.is_boot == 0XAAAA)
    {
      Serial_PutString("\r\n\r\n");
      while (Receive_Byte(&key, 1000) != 0)  //等待选择
      {
          Int2Str(Number, delay_time);
          Serial_PutString("System will be start after ");
          Serial_PutString(Number);
          Serial_PutString("s\r\r\r");
          delay_ms(1000);
          delay_time--;
          if(delay_time==-1) break;
      }
      
      if(delay_time==-1)
      {
          Serial_PutString("\r\nStart The New Program ......");
          if(((*(vu32*)(FLASH_APP_ADDR+4))&0xFF000000)==0x08000000)//判断是否为0X08XXXXXX.
          {	 
              iap_load_app(FLASH_APP_ADDR);//执行FLASH APP代码
          }
          else 
          {
              Serial_PutString("\r\nIllegal FLASH APP!");	   
          }
      }
  
        SerialDownload();     /* Download user application in the Flash */
        Serial_PutString("\r\nStart The New Program ......");
        if(((*(vu32*)(FLASH_APP_ADDR+4))&0xFF000000)==0x08000000)//判断是否为0X08XXXXXX.
        {	 
            iap_load_app(FLASH_APP_ADDR);//执行FLASH APP代码
        }else 
        {
            Serial_PutString("\r\nIllegal FLASH APP!");	   
        }
    }
    else
    {
        Main_Menu();  
    }
  
	while(1);
}
 
void Main_Menu(void)
{
  int8_t delay_time=5;
  uint8_t key = 0; 
    
  Serial_PutString("\r\n======================================================================");
  Serial_PutString("\r\n=                    (C) AX-END                                      =");
  Serial_PutString("\r\n=                                                                    =");
  Serial_PutString("\r\n=   In-Application Programming Application  (Version 1.0.0)          =");
  Serial_PutString("\r\n=                                                                    =");
  Serial_PutString("\r\n=                                                                    =");
  Serial_PutString("\r\n======================================================================");
  Serial_PutString("\r\n\r\n");


  while (Receive_Byte(&key, 1000) != 0)  //等待选择
  {
      Serial_PutString("System will be start after %ds\r\r\r");
      delay_ms(1000);
      delay_time--;
      if(delay_time==-1) break;
  }
  
  if(delay_time==-1)
  {
      Serial_PutString("\r\nStart The New Program ......");
      if(((*(vu32*)(FLASH_APP_ADDR+4))&0xFF000000)==0x08000000)//判断是否为0X08XXXXXX.
      {	 
          iap_load_app(FLASH_APP_ADDR);//执行FLASH APP代码
      }
      else 
      {
          Serial_PutString("\r\nIllegal FLASH APP!");	   
      }
  }
  
  while (1)
  {
      Serial_PutString("\r\n================== Main Menu ============================\r\n\n");
      Serial_PutString("  Download Image To the Internal Flash -------------------- 1\r\n\n");
      Serial_PutString("  Execute The New Program --------------------------------- 2\r\n\n");
      Serial_PutString("  Auto Update APP ----------------------------------------- 3\r\n\n");
      Serial_PutString("=============================================================\r\n\n");
      Serial_PutString("Please Enter the Number:");
      while (Receive_Byte(&key, 1000) != 0);  //等待选择
      Serial_PutString(&key);
      
      switch(key)
      {
          case 0x31:     
              SerialDownload();     /* Download user application in the Flash */
              break;
          
          case 0x32:
                Serial_PutString("\r\nStart The New Program ......");
                if(((*(vu32*)(FLASH_APP_ADDR+4))&0xFF000000)==0x08000000)//判断是否为0X08XXXXXX.
                {	 
                    iap_load_app(FLASH_APP_ADDR);//执行FLASH APP代码
                }else 
                {
                    Serial_PutString("\r\nIllegal FLASH APP!");	   
                }
              break;
          
          case 0x33:
              SerialDownload();     /* Download user application in the Flash */
               Serial_PutString("\r\nStart The New Program ......");
                if(((*(vu32*)(FLASH_APP_ADDR+4))&0xFF000000)==0x08000000)//判断是否为0X08XXXXXX.
                {	 
                    iap_load_app(FLASH_APP_ADDR);//执行FLASH APP代码
                }else 
                {
                    Serial_PutString("\r\nIllegal FLASH APP!");	   
                }
              break;
          
          default:
              Serial_PutString("\r\nInvalid Number ! ==> The number should be either 1, 2 or 3\r\n");
              break;      
      }
  }
}

void SerialDownload(void)
{
  uint8_t Number[10] = {0};
  int32_t Size = 0;

  Serial_PutString("Waiting for the file to be sent ... (press 'a' to abort)\n\r");
  Size = Ymodem_Receive(&tab_1024[0]);
  
  delay_ms(30);
  
  if (Size > 0)
  {
    Serial_PutString("\n\n\r Programming Completed Successfully!\n\r--------------------------------\r\n Name: ");
    Serial_PutString(FileName);
    Int2Str(Number, Size);
    Serial_PutString("\n\r Size: ");
    Serial_PutString(Number);
    Serial_PutString(" Bytes\r\n");
    Serial_PutString("-------------------\n");
  }
  else if (Size == -1)
  {
    Serial_PutString("\n\n\rThe image size is higher than the allowed space memory!\n\r");
  }
  else if (Size == -2)
  {
    Serial_PutString("\n\n\rVerification failed!\n\r");
  }
  else if (Size == -3)
  {
    Serial_PutString("\r\n\nAborted by user.\n\r");
  }
  else
  {
    Serial_PutString("\n\rFailed to receive the file!\n\r");
  }
}









