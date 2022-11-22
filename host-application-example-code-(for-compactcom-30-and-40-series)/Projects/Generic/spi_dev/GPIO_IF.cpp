#include <JetsonGPIO.h>
#include "GPIO_IF.h"

void gpio_if_init(void){

    GPIO::setmode(GPIO::BOARD);
    GPIO::setup(13, GPIO::IN);    // MI0
    GPIO::setup(15, GPIO::IN);    // MI1

    GPIO::setup(16, GPIO::IN);                // IRQ
    GPIO::setup(18, GPIO::OUT, GPIO::LOW);    // RESET

    // RESETピンをHIGHにする
    //GPIO::output(18, GPIO::HIGH);
    GPIO::output(18, GPIO::LOW);
}

void gpio_if_HWReset(void)
{
    // RESETピンをLOWにする
    GPIO::output(18, GPIO::LOW);
}

void gpio_if_HWReleaseReset(void)
{
   // RESETピンをHIGHにする
   GPIO::output(18, GPIO::HIGH);
}

int gpio_if_ReadMI(void)
{
   int mi0 = GPIO::input(13); //MI0
   int mi1 = GPIO::input(15); //MI1

   int mi = (mi1 << 1) + mi0;

   return mi;

}

int gpio_if_ReadIRQ(void)
{
    return GPIO::input(16);
}

