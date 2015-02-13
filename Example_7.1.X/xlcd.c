
#if defined(__18CXX)
#include <xlcd.h>
#include <delays.h>
#elif defined(__XC)
#include <plib\xlcd.h>
#include <plib\delays.h>
#endif
#include "xlcd_config.h"

void WriteCmdXLCD(unsigned char cmd)
{
#ifdef BIT8                                                                 /* 8-bit interface                                  */
        TRIS_DATA_PORT = 0;                                                 /* Data port output                                 */
        DATA_PORT = cmd;                                                    /* Write command to data port                       */
        RW_PIN_ = 0;                                                         /* Set the control signals                          */
        RS_PIN_ = 0;                                                         /* Register select input=0 for sending a command    */
        DelayFor18TCY();
        E_PIN_ = 1;                                                          /* Clock the command input                          */
        DelayFor18TCY();                                                    /* Invokung delay                                   */
        E_PIN_ = 0;
        DelayFor18TCY();
        TRIS_DATA_PORT = 0xff;                                              /* Data port direction as input                     */

#endif
        return;
}

void WriteDataXLCD(char data)
{
#ifdef BIT8                                                                 /* 8-bit interface                                  */
        TRIS_DATA_PORT = 0;                                                 /* Data port output                                 */
        DATA_PORT = data;                                                   /* Write data to data port                          */
        RS_PIN_ = 1;                                                         /* Set control bits                                 */
        RW_PIN_ = 0;
        DelayFor18TCY();
        E_PIN_ = 1;                                                          /* Clock data into LCD                              */
        DelayFor18TCY();
        E_PIN_ = 0;
        DelayFor18TCY();
        RS_PIN_ = 0;                                                         /* clear register select bit                        */
        TRIS_DATA_PORT = 0xff;                                              /* Make data port direction as input                */
#endif
        return;
}

void OpenXLCD_(unsigned char lcdtype)
    {
        #ifdef BIT8                                                         /* 8-bit mode                                       */
        DATA_PORT = 0;                                                      /* clearing data port                               */
        TRIS_DATA_PORT = 0x00;                                              /* Direction of data port set to output             */

#endif
        TRIS_RW_ = 0;                                                        /* Direction of all control bits set as outputs     */
        TRIS_RS_ = 0;
        TRIS_E_ = 0;
        RW_PIN_ = 0;                                                         /* Write/Read pin made low (Write mode)             */
        RS_PIN_ = 0;                                                         /* Register select pin made low (command mode)      */
        E_PIN_ = 0;                                                          /* Clock pin cleared                                */

        // Delay for 15ms to allow for LCD Power on reset
        DelayPORXLCD();
 //-------------------reset procedure through software-------------------------------------
		 WriteCmdXLCD(0x30);
         Delay10KTCYx(0x05);
		 WriteCmdXLCD(0x30);
		 Delay10KTCYx(0x01);
		 WriteCmdXLCD(0x32);
		while( BusyXLCD() );
//------------------------------------------------------------------------------------------

        // Set data interface width, # lines, font
        while(BusyXLCD());                                                  /* Wait if LCD busy                                  */
        WriteCmdXLCD(lcdtype);                                              /* Writing code for lcdtype                          */

        /* Turn the display on then off */
        while(BusyXLCD());                                                  /* Wait if LCD busy                                  */
        WriteCmdXLCD(DOFF&CURSOR_OFF&BLINK_OFF);                            /* Write command for Display OFF/cursor Blink OFF    */
        while(BusyXLCD());                                                  /* Wait if LCD busy                                  */
        WriteCmdXLCD(DON);                                                  /* Write command for Display ON                      */

        /* Clear display            */
        while(BusyXLCD());                                                  /* Wait if LCD busy                                  */
        WriteCmdXLCD(0x01);                                                 /* Clear display                                     */
        /* Set DD Ram address to 0  */
        while(BusyXLCD());                                                  /* Wait if LCD busy                                  */
        SetDDRamAddr(0x80);                                                 /* Set Display data ram address to 0                 */
        return;
    }
