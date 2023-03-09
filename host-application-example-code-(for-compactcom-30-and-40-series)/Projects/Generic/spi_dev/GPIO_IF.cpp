#include <JetsonGPIO.h>
#include <unistd.h>
#include "GPIO_IF.h"

void gpio_if_init(void){
    printf("gpio_if_init()\n");

    GPIO::setmode(GPIO::BOARD);

    GPIO::setup(13, GPIO::IN);    // MI0
    GPIO::setup(15, GPIO::IN);    // MI1

    GPIO::setup(16, GPIO::IN);                // IRQ
    GPIO::setup(18, GPIO::OUT, GPIO::LOW);    // RESET

    // RESETピンをHIGHにする
    GPIO::output(18, GPIO::LOW);
}

void gpio_if_cleanup(void){
    printf("gpio_if_cleanup()\n");
    GPIO::setmode(GPIO::BOARD);   
    GPIO::cleanup();
}

void gpio_if_HWReset(void)
{
    // RESETピンをLOWにする
    printf("gpio_if_HWReset()\n");
    GPIO::setmode(GPIO::BOARD);   
    GPIO::setup(18, GPIO::OUT, GPIO::LOW);    // RESET
    GPIO::output(18, GPIO::LOW);
//    usleep(100000);
}

void gpio_if_HWReleaseReset(void)
{
    // RESETピンをHIGHにする
    printf("gpio_if_HWReleaseReset()\n");
    GPIO::setmode(GPIO::BOARD);   
    GPIO::setup(18, GPIO::OUT, GPIO::LOW);    // RESET
    GPIO::output(18, GPIO::HIGH);
//    usleep(100000);
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
    // printf("gpio_if_ReadIRQ()\n");
    return GPIO::input(16);
}
