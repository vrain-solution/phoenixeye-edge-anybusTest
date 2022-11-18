#ifndef _GPIO_IF_H
#define _GPIO_IF_H

#ifdef __cplusplus
extern "C" {
#endif


void gpio_if_init(void);
void gpio_if_HWReset(void);
void gpio_if_HWReleaseReset(void);

#ifdef __cplusplus
}
#endif

#endif // _GPIO_IF_H