#include "millis.h"
#include "MS51_16K.h"

#define TH0_INIT 0xA2
#define TL0_INIT 0x42

static xdata unsigned long millis;
unsigned long uptime_millis(void)
{
    return millis;
}
unsigned long uptime_sec(void)
{
    return (millis/1000);
}
void uptime_init(void)
{
    ENABLE_TIMER0_MODE1;
    TIMER0_FSYS;

    TH0 = TH0_INIT;
    TL0 = TL0_INIT;

    ENABLE_TIMER0_INTERRUPT; // enable Timer0 interrupt
    ENABLE_GLOBAL_INTERRUPT; // enable interrupts

    set_TCON_TR0;
}
xdata unsigned long int ms = 0;
void Timer0_ISR(void) interrupt 1 // interrupt address is 0x000B
{
    _push_(SFRS);

    TH0 = TH0_INIT;
    TL0 = TL0_INIT;
    TF0 = 0;
    P14 = ~P14; // GPIO1 toggle when interrupt
    millis++;
    if (millis > 0xffffffff)
    {
        millis = 0;
    }
    _pop_(SFRS);
}