#include <JetsonGPIO.h>
#include "GPIO_IF.h"

void gpio_if_init(void){

    GPIO::setmode(GPIO::BOARD);
//   GPIO::setup(16, GPIO::IN);              // IRO
    GPIO::setup(18, GPIO::OUT, GPIO::LOW);    // RESET

    // RESETピンをHIGHにする
    GPIO::output(18, GPIO::HIGH);
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
