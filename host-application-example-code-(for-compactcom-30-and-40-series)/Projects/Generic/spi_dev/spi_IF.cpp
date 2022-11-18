#include "spi_IF.h"
#include "spi.h"


static spi spi_;

void* spi_if_init(void){
    int retval = 0;

    retval = spi_.init();

    if(retval == -1){
        return NULL;
    } else {
        return &spi_;
    }
}

int spi_if_xfer(const void *handle, uint8_t *p_tx_buffer, uint8_t tx_len, uint8_t *p_rx_buffer, uint8_t rx_len){
    return static_cast<spi*>(const_cast<void *>(handle))->xfer(p_tx_buffer, tx_len, p_rx_buffer, rx_len);
}
