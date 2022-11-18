#ifndef _SPI_H
#define _SPI_H

#ifdef __cplusplus
extern "C" {
#endif


class spi{
    public:
        spi();
        ~spi();

        int init(void);
        int xfer(uint8_t *p_tx_buffer, uint8_t tx_len, uint8_t *p_rx_buffer, uint8_t rx_len);
        int write(uint8_t *p_tx_buffer, uint8_t tx_len);
        int read(uint8_t *p_rx_buffer, uint8_t rx_len);

    private:
        int fd_;
};

#ifdef __cplusplus
}
#endif

#endif // _SPI_H