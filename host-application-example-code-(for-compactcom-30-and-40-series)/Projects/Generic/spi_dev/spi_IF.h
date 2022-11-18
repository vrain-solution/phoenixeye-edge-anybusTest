//https://www.servernote.net/article.cgi?id=use-cpp-class-from-c

#ifndef _SPI_IF_H
#define _SPI_IF_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"
#include "stdio.h"

void* spi_if_init(void);
int spi_if_xfer(const void *handle, uint8_t *p_tx_buffer, uint8_t tx_len, uint8_t *p_rx_buffer, uint8_t rx_len);

#ifdef __cplusplus
}
#endif

#endif // _SPI_IF_H