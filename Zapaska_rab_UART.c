#include <1886VE2d.h>
#include "int17xxx.h"
#pragma origin 0x8
#define ID_UPS 0x88 // ID SKS

/////////////inclusion variable
uns8 CountWR;
uns8 LanWr;
uns8 CountRead;
uns8 BufRs[40];
uns8 BufWrRs[40];

uns8 flag;
bit f_ReadRs @ flag.0;
bit f_Per_ser @ flag.1;

bank0 char sPRODL, sPRODH, sTBLPTRH, sTBLPTRL, sFSR0, sFSR1;
/////////////

/////////////inlusion function
void setup(void);
void Delay_us(const unsigned int Delay);
void Delay_ms(const unsigned int Delay);
/////////////


interrupt iServer(void)
{
    multi_interrupt_entry_and_save
    
    PERIPHERAL_service:
                     
    #asm
		MOVFP PRODL, sPRODL
		MOVFP PRODH, sPRODH
		MOVFP TBLPTRH, sTBLPTRH
		MOVFP TBLPTRL, sTBLPTRL
		MOVFP FSR0, sFSR0
		MOVFP FSR1, sFSR1
 	#endasm   
      
    
        
    if (f_Per_ser == 0 )
    {
        f_Per_ser = 1; // FLAG PERIPHERAL_service


         //transmit RS
        if (TX1IF)  
        {
		    if ( CountWR <= LanWr)
		    {
			    TXREG1=BufWrRs[CountWR];
			    CountWR=CountWR+1;
		    }
		else if (TRMT1 == 1)
		    {
                LanWr = 0;
			    TX1IE = 0;	   
         	    TXEN1 = 0;
		    }
        }
    
	    // read RS
 	    if (RC1IF)  
        {
            if(FERR1)
            {
		 	    BufRs[0] = RCREG1;
			    CountRead = 0;
            }
            else
            {	            	
                BufRs[CountRead] = RCREG1;
	            if(OERR1)
	            {
	                CREN1=0;
                    CREN1=1;
                }  
                ++CountRead;
			    f_ReadRs = 1; 
		    }
 	    }

		f_Per_ser = 0;  // FLAG PERIPHERAL_service 
    }                
    goto RESTORE_and_return;               
 TMR0_service:
    // save on demand: PRODL,PRODH,TBLPTRH,TBLPTRL,FSR0,FSR1
    /* process Timer 0 interrupt */
    // T0IF is automatically cleared when the CPU vectors to 0x10
    // restore on demand: PRODL,PRODH,TBLPTRH,TBLPTRL,FSR0,FSR1
 T0CKI_service:
 INT_service:
             
 RESTORE_and_return:
	#asm
		MOVFP sFSR1, FSR1
		MOVFP sFSR0, FSR0
		MOVFP sTBLPTRL, TBLPTRL
		MOVFP sTBLPTRH, TBLPTRH
		MOVFP sPRODH, PRODH
		MOVFP sPRODL, PRODL
	#endasm

    interrupt_exit_and_restore
}
/******************/
void main(void)
{
    GLINTD = 1;
    setup();
    GLINTD = 0; 
    uns8 i=0;
    
    /////////////////// TEST
    //GLINTD = 1;
    PORTC = 0x00;//LED ON
    Delay_ms(100);
	PORTC = 0x08;//LED OFF
	

	/////////////////// Test Transmitting
	//TXREG1=0x44;
	PORTF = 0x05;// PORTF TRANSMIT
	LanWr = 2;
	f_Per_ser = 0; // FLAG PERIPHERAL_service
	BufWrRs[0] = 0x14;
    BufWrRs[1] = 0x14;
    BufWrRs[2] = 0x88;
	
    Delay_ms(100);
	CountWR = 0;          	
    TXEN1 = 1;
    TX1IE = 1;
	//GLINTD = 0;
	//////////////////// END TEST
          
    while (1)
    {   
        if( f_ReadRs == 1)
            {   
                
				PORTC = 0x00; //LED ON				 
        		f_ReadRs = 0;
				f_Per_ser = 0; // FLAG PERIPHERAL_service
				LanWr = 2; // = CountRead;
				CountRead = 0;
                
				if (BufRs[1]==0x55)	
				{  
            		BufWrRs[0] = ID_UPS;
            		BufWrRs[1] = 0x15; //spi_data_0;
            		BufWrRs[2] = 0x40; //spi_data_1;
				} 
				else
				{
            		BufWrRs[0] = ID_UPS;
            		BufWrRs[1] = 0x14;
            		BufWrRs[2] = 0x88;
				}
               
				PORTF = 0x05; // PORTF TRANSMIT  
                i=0; 
			    Delay_ms(100);
                CountWR = 0;
                GLINTD = 1;          	
                TXEN1 = 1;
                TX1IE = 1;		         
                GLINTD = 0;
					
            }
		i = 0; 
		Delay_ms(100);		
		PORTC = 0x08;// LED OFF
		f_Per_ser = 0; // FLAG PERIPHERAL_service
        PORTF = 0x06;// PORTF READ
	}
}

void setup()
{
    //FlagInit
   	flag = 0x00;

    //CountInit   	
   	CountWR = 0;       
    f_Per_ser = 0;
   	CountRead = 0;
   	LanWr = 0;

   	//InterruptsInit Register INSTA
   	PEIE = 1;
   	T0CKIE = 0; 
	INTE = 0; 
	T0IE = 0;
	
	//LEDInit 

   	DDRC=0xf7; 
   	PORTC=0x08;	
   	
   	//Usart1Init

	DDRF = 0xf8;
	PORTF = 0x06;

   	SPBRG1 = 25;
	TXSTA1 = 0x00; 
	RCSTA1 = 0x90; 
	PIE1 = 1;   
     
}
 
void Delay_us(const unsigned int Delay)
{
	unsigned int i;	
	
    for(i = 0; i < Delay; i++){
		#asm
	        nop // 241 * 10^-9 �� , 1 ��
	    #endasm
    }
}
void Delay_ms(const unsigned int Delay)
{
	unsigned int i;
	
    for(i = 0; i < Delay; i++) Delay_us(4000);
}
