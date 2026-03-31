#include<p18f4520.h>
#include"header.h"
#include <timers.h>
//--------------------------------------------------------------------------------------------
//variabili globali
int ContaTempo_ds;

//prototipi
void InterruptHandlerHigh (void);
//--------------------------------------------------------------------------------------------
//main
void main (void)
{
    int stato = STATO_ATTESA_P1;
    //configura porte utilizzate (A,B)
    TRISA = PORTA_TRIS;
    PORTA = PORTA_DEFAULT;
    
    TRISB = PORTB_TRIS;
    PORTB = PORTB_DEFAULT;
    
    //supponiamo un oscillatore a 4Mzh
    //4.000.000 / 4= 1.000.000 /2 prescaler = 500.000
    //configura TIMER0 (16 bit, interrupt enabled)
    Opentimer0 (TIMER_INT_ON & T0_16BIT & T0_SOURCE_INT & T0_DEGE_FALL & T0_PS_1_2);
    WriteTimer0 (TIMER0_VALUE_100ms);
    while (1)
    {
        switch (stato)
        {
        case STATO_ATTESA_P1: //verifica se è stato premuto il pulsante 1
            if (PULSANTE1 == 0)
            {
                USCITA = 1;
                ContaTempo_ds = DURATA_IMPULSO1_ds;
                stato = STATO_IMPULSO1;
            }
            break;
        case STATO_IMPULSO1: //verifica se  esaurito il tempo per l'impulso 1
            if (ContaTmpo_ds ==0)
            {
                USCITA = 0;
                stato = STATO_ATTESAS_P2;
            }
            break;
        case STATO_ATTESA_P2://verifica se è stato premuto il pulsante 2
            if (PULSANTE2 ==0)
            {
                USCITA = 1;
                ContaTempo_ds = DURATA_IMPULSO2_ds;
                stato = STATO_IMPULSO2;
            }
            break;
        case STATO_IMPULSO2://verifica se  esaurito il tempo per l'impulso 2
            if (ContaTmpo_ds ==0)
            {
                USCITA = 0;
                stato = STATO_ATTESAS_P1;
            }
            break;
        }
    }
}

#pragma code InterruptVectorHigh = 0x08
void InterruptVectorHigh (void)
{
    _asm
    goto InterruptVectorHigh
    _endasm
}

#pragma code
#pragma interrupt InterruptVectorHigh

void InterruptVectorHigh ()
{
    if (INTCONbits.TMR0IF)
    {
        INTCONbits.TMR0IF=0;
        WriteTimer0(TIMER0_VALUE_100ms);
        if (ContaTempo_ds > 0)
        {
            ContaTempo_ds--;
        }
    }
}
