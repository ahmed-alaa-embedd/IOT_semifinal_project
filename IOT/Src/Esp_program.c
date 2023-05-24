///***************************/
///*Author : ahmed alaa	*/
///*date : 23/3/2023 		*/
///*version : v04			*/
///***************************/

//Liberary
#include <stdio.h>
#include <string.h>

#include "BIT_MATH.h"
#include "STD_TYPES.h"


#include "Esp_config.h"
#include "Esp_interface.h"
#include "Esp_private.h"


#include "USART_interface.h"

u8 Quotes	[]= "\""	;
u8 Comma 	[]	 = {","};

void HEsp_voidInit()
{
	u8 Local_u8Result =0;

	//check for ok
	while(Local_u8Result == 0)
	{
		//Echo disable
		MUSART1_voidTransmit("ATE0\r\n");
		//if recieve OK then break while loop
		Local_u8Result = u8EspValidateCmd(ECHO_TIMEOUT);

	}

	Local_u8Result =0;

	//check for ok
	while(Local_u8Result== 0)
	{
		//set station mode
		MUSART1_voidTransmit("AT+CWMODE=1\r\n");
		//if recieve OK then break while loop
		Local_u8Result = u8EspValidateCmd(MODE_TIMEOUT);
	}

}
void HEsp_voidConnectOnWifi(u8 *Copy_u8Name, u8 *Copy_u8Password)	//AT+CWJAP_CUR="Name","Pass"
{
	u8 Local_u8Result = 0;
	u8 Local_u8Data[100] = {0};

	sprintf(Local_u8Data,"AT+CWJAP_CUR=%s%s%s,%s%s%s\r\n",Quotes,Copy_u8Name,Quotes,Quotes,Copy_u8Password,Quotes);


	//check for ok
	while(Local_u8Result== 0)
	{
		//send Name and pass
		MUSART1_voidTransmit(Local_u8Data);
		//if recieve OK then break while loop
		Local_u8Result = u8EspValidateCmd(ROUTER_TIMEOUT);
	}


}
void HEsp_voidConnectOnServer(u8 *Copy_u8IP)//AT+CIPSTART="TCP","162.253.155.226",80
{
	u8 Local_u8Result = 0;
	u8 Local_u8Data[100] = {0};

	sprintf(Local_u8Data,"AT+CIPSTART=%sTCP%s,%s%s%s,80\r\n",Quotes,Quotes,Quotes,Copy_u8IP,Quotes);


	//check for ok
	while(Local_u8Result== 0)
	{
		//send IP
		MUSART1_voidTransmit(Local_u8Data);
		//if recieve OK then break while loop
		Local_u8Result = u8EspValidateCmd(SERVER_TIMEOUT);
	}


}
/*AT+CIPSEND=45
GET http://ahmedarm.freevar.com/status.txt
=43 letter + enter %ctr(j)=44
* */
u8 HEsp_voidSendData(u8 Copy_u8DataSize,u8 *Copy_u8WebSite)
{
	u8 Local_u8Result = 0;
	u8 Local_u8Data[100] = {0};

	sprintf(Local_u8Data,"AT+CIPSEND=%d\r\n",Copy_u8DataSize);


	//check for ok
	while(Local_u8Result== 0)
	{
		//send data size
		MUSART1_voidTransmit(Local_u8Data);
		//if recieve OK then break while loop
		Local_u8Result = u8EspValidateCmd(PREREQUEST_TIMEOUT);
	}


	//delete saved data to write a new one
	for(int i=0;i<80;i++)
	{
		Local_u8Data[i]=0;
	}

	//send website
	sprintf(Local_u8Data,"GET http://%s/status.txt\r\n\r",Copy_u8WebSite);
	Local_u8Result=0;

	//check for ok
	while(Local_u8Result== 0)
	{
		//send data size
		MUSART1_voidTransmit(Local_u8Data);
		//if recieve OK then break while loop
		Local_u8Result = u8EspValidateCmd(REQUEST_TIMEOUT);
	}
	return Local_u8Result;
}
// delete it and put it in the above section


u8 u8EspValidateCmd(u32 Copy_u32timeout) //private func
{
	u8 Local_u8Response[100] ={0};
	u8 i=0;
	u8 Local_u8Dummy = 0;
	u8 Local_u8Result = 0;

	while(Local_u8Dummy != 255)
	{
		Local_u8Dummy=MUSART1_u8Receive(Copy_u32timeout);
		Local_u8Response[i] = Local_u8Dummy;
		i++;
	}

	//check for ok
	for(i=0;i<99;i++)
	{
		if(Local_u8Response[i] == 'O' && Local_u8Response[i+1] == 'K' )
		{
			Local_u8Result = i;
			break;
		}
		//add check for error on putty
		else if(Local_u8Response[i] == 'E' && Local_u8Response[i+1] == 'R'&& Local_u8Response[i+2] == 'R')
		{
			Local_u8Result = i;
			break;
		}
		else
		{
			Local_u8Result = i;
		}

	}
	for(i=0;i<99;i++)
	{
		if(Local_u8Response[i] == '+' && Local_u8Response[i+1] == 'I' )
		{
			if(Local_u8Response[i+7] == '1')
			{
				Local_u8Result = 1;
				break;
			}
			else if(Local_u8Response[i+7] == '0')
			{
				Local_u8Result = 35;
				break;
			}

		}
		else
		{
			Local_u8Result = i;
		}
	}
	return Local_u8Result;
}

