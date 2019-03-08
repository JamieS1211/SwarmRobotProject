/*
 * File:   interrupts.c
 * Author: Jamie
 *
 * Created on 30 November 2018, 16:45
 */


#include <xc.h>

#include "interrupts.h"
#include "i2C.h"
#include "vL53L0X.h"

// Interrupts - Pages 97 - 110 in data sheet

/*
 * Function sets up all interrupt settings.
 */
void interrupts_Setup(void) {

    //INTCON [Interrupt Control Register 1] - page 99
    INTCONbits.GIE = 1;     //Global Interrupt Enable bit
    INTCONbits.PEIE = 0;    //Peripheral Interrupt Enable bit
    INTCONbits.TMR0IE = 1;  //TMR0 Overflow Interrupt Enable bit
    INTCONbits.INT0IE = 0;  //INT0 External Interrupt Enable bit
    INTCONbits.RBIE = 0;    //RB Port Change Interrupt Enable bit
    
    //INTCON2 [Interrupt Control Register 2] - page 100
    INTCON2bits.RBPU = 1;       //PORTB Pull-up Enable bit
    INTCON2bits.INTEDG0 = 1;    //External Interrupt 0 Edge Select bit 
    INTCON2bits.INTEDG1 = 1;    //External Interrupt 1 Edge Select bit 
    INTCON2bits.INTEDG2 = 1;    //External Interrupt 2 Edge Select bit 
    INTCON2bits.TMR0IP = 1;     //TMR0 Overflow Interrupt Priority bit
    INTCON2bits.RBIP = 1;       //RB Port Change Interrupt Priority bit
    
    //INTCON3 [Interrupt Control Register 3] - page 101
    INTCON3bits.INT2IP = 1;     //INT2 External Interrupt Priority bit
    INTCON3bits.INT1IP = 1;     //INT1 External Interrupt Priority bit
    INTCON3bits.INT2IE = 0;     //INT2 External Interrupt Enable bit
    
    //Peripheral Interrupt Request Registers - When IPEN = 0, the PEIE bit must be set to enable any of these peripheral interrupts
    //PIE1 [Peripheral Interrupt Request (Enable) Register 1] - Page 104
    PIE1bits.ADIE = 0;      //A/D Converter Interrupt Enable bit 
    PIE1bits.RCIE = 0;      //EUSART Receive Interrupt Enable bit 
    PIE1bits.TXIE = 0;      //EUSART Transmit Interrupt Enable bit 
    PIE1bits.SSPIE = 0;     //Master Synchronous Serial Port Interrupt Enable bit
    PIE1bits.CCP1IE = 0;    //CCP1 Interrupt Enable bit
    PIE1bits.TMR2IE = 0;    //TMR2 to PR2 Match Interrupt Enable bit
    PIE1bits.TMR1IE = 0;    //TMR1 Overflow Interrupt Enable bit 
    
    //PIE2 [Peripheral Interrupt Request (Enable) Register 2] - Page 105
    PIE2bits.OSCFIE = 0;    //Oscillator Fail Interrupt Enable bit
    PIE2bits.CMIE = 0;      //Comparator Interrupt Enable bit
    PIE2bits.USBIE = 0;     //USB Interrupt Enable bit
    PIE2bits.EEIE = 0;      //Data EEPROM/Flash Write Operation Interrupt Enable bit 
    PIE2bits.BCLIE = 0;     //Bus Collision Interrupt Enable bit 
    PIE2bits.HLVDIE = 0;    //High/Low-Voltage Detect Interrupt Enable bit 
    PIE2bits.TMR3IE = 0;    //TMR3 Overflow Interrupt Enable bit
    PIE2bits.CCP2IE = 0;    //CCP2 Interrupt Enable bit
    
    
    
    //Interrupt Priority Registers - Using the priority bits requires that the Interrupt Priority Enable (IPEN) bit be set
    //RPI1 [Peripheral Interrupt Priority Register 1] - page 106
    IPR1bits.ADIP = 1;      //A/D Converter Interrupt Priority bit
    IPR1bits.RCIP = 1;      //EUSART Receive Interrupt Priority bit 
    IPR1bits.TXIP = 1;      //EUSART Transmit Interrupt Priority bit 
    IPR1bits.SSPIP = 1;     //Master Synchronous Serial Port Interrupt Priority bit 
    IPR1bits.CCP1IP = 1;    //CP1 Interrupt Priority bit 
    IPR1bits.TMR2IP = 1;    //TMR2 to PR2 Match Interrupt Priority bit 
    IPR1bits.TMR1IP = 1;    //TMR1 Overflow Interrupt Priority bit
    
    //RPI2 [Peripheral Interrupt Priority Register 2] - page 107
    IPR2bits.OSCFIP = 1;    //Oscillator Fail Interrupt Priority bit
    IPR2bits.CMIP = 1;      //Comparator Interrupt Priority bit
    IPR2bits.USBIP = 1;     //USB Interrupt Priority bit
    IPR2bits.EEIP = 1;      //Data EEPROM/Flash Write Operation Interrupt Priority bit
    IPR2bits.BCLIP = 1;     //Bus Collision Interrupt Priority bit
    IPR2bits.HLVDIP = 1;    //High/Low-Voltage Detect Interrupt Priority bit
    IPR2bits.TMR3IP = 1;    //TMR3 Overflow Interrupt Priority bit
    IPR2bits.CCP2IP = 1;    //CCP2 Interrupt Priority bit
}



/*
 * Function should be called on an interrupt.
 * Contains code for each type of interrupt and clears interrupt after handling.
 */
void __interrupt() interrupts_Event(void) {
//__interrupt(low_priority)
//__interrupt(high_priority)
    
    if (INTCONbits.TMR0IE && INTCONbits.TMR0IF == 1) {
        //TMR0 Overflow Interrupt 
        
        //<TEST CODE>
        unsigned char slave_address = 0x52;     //This is correctly shifted so last bit can be read or wright (0x29)
        unsigned char device_register = 0x14;   //This should be register with result 
        unsigned char value = vl53l0x_I2C_ReceiveData(slave_address, device_register);
        
        if (value < 60) {
            PORTBbits.RB7 = 1;
            PORTBbits.RB6 = 1;
            PORTBbits.RB4 = 1;  
            PORTBbits.RB3 = 1;  
            PORTBbits.RB2 = 1;                
        } else if (value < 70) {            
            PORTBbits.RB7 = 0;
            PORTBbits.RB6 = 1;
            PORTBbits.RB4 = 1;
            PORTBbits.RB3 = 1;  
            PORTBbits.RB2 = 1;   
        } else if (value < 80) {            
            PORTBbits.RB7 = 0;
            PORTBbits.RB6 = 0;
            PORTBbits.RB4 = 1;
            PORTBbits.RB3 = 1;  
            PORTBbits.RB2 = 1;   
        } else if (value < 90) {            
            PORTBbits.RB7 = 0;
            PORTBbits.RB6 = 0;
            PORTBbits.RB4 = 0;
            PORTBbits.RB3 = 1;  
            PORTBbits.RB2 = 1;   
        } else {            
            PORTBbits.RB7 = 0;
            PORTBbits.RB6 = 0;
            PORTBbits.RB4 = 0;
            PORTBbits.RB3 = 0;  
            PORTBbits.RB2 = 1;   
        }
        //</TEST CODE/>
        
        INTCONbits.TMR0IF = 0;
    } else if(INTCONbits.INT0IE && INTCONbits.INT0IF == 1) {
        //INT0 External Interrupt
        
        INTCONbits.INT0IF = 0;
    } else if(INTCONbits.RBIE && INTCONbits.RBIF == 1) {
        //RB Port Change Interrupt
        
        INTCONbits.RBIF = 0;
    } else if(INTCON3bits.INT2IE && INTCON3bits.INT2IF == 1) {
        //INT2 External Interrupt Flag bit
        
        INTCON3bits.INT2IF = 0;
    } else if(INTCON3bits.INT1IE && INTCON3bits.INT1IF == 1) {
        //INT1 External Interrupt Flag bit
        
        INTCON3bits.INT1IF = 0;
    } else if(PIE1bits.ADIE && PIR1bits.ADIF == 1) {
        //A/D Converter Interrupt Flag bit
        
        PIR1bits.ADIF = 0;
    } else if(PIE1bits.RCIE && PIR1bits.RCIF == 1) {
        //[READ ONLY] EUSART Receive Interrupt
        
        PIR1bits.RCIF = 0;
    } else if(PIE1bits.TXIE && PIR1bits.TXIF == 1) {
        //[READ ONLY] EUSART Transmit Interrupt
        
        PIR1bits.TXIF = 0;
    } else if(PIR1bits.SSPIF && PIR1bits.SSPIF == 1) {
        //Master Synchronous Serial Port Interrupt
        
        PIR1bits.SSPIF = 0;
    } else if(PIE1bits.CCP1IE && PIR1bits.CCP1IF == 1) {
        //CCP1 Interrupt
        
        PIR1bits.CCP1IF = 0;
    } else if(PIE1bits.TMR2IE && PIR1bits.TMR2IF == 1) {
        //TMR2 to PR2 Match Interrupt
        
        PIR1bits.TMR2IF = 0;
    } else if(PIE1bits.TMR1IE && PIR1bits.TMR1IF == 1) {
        //TMR1 Overflow Interrupt
        
        PIR1bits.TMR1IF = 0;
    } else if(PIE2bits.OSCFIE && PIR2bits.OSCFIF == 1) {
        //Oscillator Fail Interrupt
        
        PIR2bits.OSCFIF = 0;
    } else if(PIE2bits.CMIE && PIR2bits.CMIF == 1) {
        //Comparator Interrupt
        
        PIR2bits.CMIF = 0;
    } else if(PIE2bits.USBIE && PIR2bits.USBIF == 1) {
        //USB Interrupt
        
        PIR2bits.USBIF = 0;
    } else if(PIE2bits.EEIE && PIR2bits.EEIF == 1) {
        //Data EEPROM/Flash Write Operation Interrupt
        
        PIR2bits.EEIF = 0;
    } else if(PIE2bits.BCLIE && PIR2bits.BCLIF == 1) {
        //Bus Collision Interrupt
        
        PIR2bits.BCLIF = 0;
    } else if(PIE2bits.HLVDIE && PIR2bits.HLVDIF == 1) {
        //High/Low-Voltage Detect Interrupt
        
        PIR2bits.HLVDIF = 0;
    } else if(PIE2bits.TMR3IE && PIR2bits.TMR3IF == 1) {
        //TMR3 Overflow Interrupt
        
        PIR2bits.TMR3IF = 0;
    } else if(PIE2bits.CCP2IE && PIR2bits.CCP2IF == 1) {  
        //CCP2 Interrupt 
        
        PIR2bits.CCP2IF = 0;
    }
}