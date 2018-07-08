#ifndef __GPIO_IIC_H__
#define __GPIO_IIC_H__	

#include <stdint.h>
   	
void IIC_Init(void);                			//��ʼ��IIC��IO��				 
void IIC_Start(void);							//����IIC��ʼ�ź�
void IIC_Stop(void);	  						//����IICֹͣ�ź�
void IIC_Send_Byte(uint8_t data);				//IIC����һ���ֽ�
uint8_t IIC_Recv_Byte(uint8_t ack);				//IIC��ȡһ���ֽ�
uint8_t IIC_Wait_Ack(void); 					//IIC�ȴ�ACK�ź�
void IIC_Ack(void);								//IIC����ACK�ź�
void IIC_NAck(void);							//IIC������ACK�ź�

#endif


