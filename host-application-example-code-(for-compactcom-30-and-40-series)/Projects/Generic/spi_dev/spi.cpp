#include <iostream>
#include <memory.h>

#include <fcntl.h>
#include <unistd.h>

// ファイル操作 open/close
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

#include "spi.h"

spi::spi(){
    fd_ = -1;   // openに成功すると0以上の値が設定されるため、エラー値である-1を初期値に設定しておく
}
spi::~spi(){
    if(fd_ >= 0){
        close(fd_);
    }
}

int spi::init(void){
    int ret = 0;
    fd_ = open("/dev/spidev0.0", O_RDWR);
    if(fd_ < 0){
        std::cout << "can not open /dev/spidev0.0" << std::endl;
        return -1;
    }

    // anybus用のパラメータ
    uint8_t mode          = SPI_MODE_0;     // anybusモジュールはモード0で動作
    uint8_t bits_per_word = 8;             // ABCC_CFG_SPI_MSG_FRAG_LENより大きい値に設定する必要がありそう          
    uint32_t speed        = 5000000;        // 1000000 Hz (1000 KHz)
    uint16_t delay        = 0;

    // spi mode
    ret = ioctl(fd_, SPI_IOC_WR_MODE, &mode);
    if(ret == -1){
        std::cout << "can not write spi mode" << std::endl;
        return -1;
    }

    ret = ioctl(fd_, SPI_IOC_RD_MODE, &mode);
    if(ret == -1){
        std::cout << "can not read spi mode" << std::endl;
        return -1;
    }

    // bits per word
    ret = ioctl(fd_, SPI_IOC_WR_BITS_PER_WORD, &bits_per_word);
    if(ret == -1){
        std::cout << "can not write bits per word" << std::endl;
        return -1;
    }

    ret = ioctl(fd_, SPI_IOC_RD_BITS_PER_WORD, &bits_per_word);
    if(ret == -1){
        std::cout << "can not read bits per word" << std::endl;
        return -1;
    }

    // max speed hs
    ret = ioctl(fd_, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
    if(ret == -1){
        std::cout << "can not write max speed hz" << std::endl;
        return -1;
    }

    ret = ioctl(fd_, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
    if(ret == -1){
        std::cout << "can not read max speed hz" << std::endl;
        return -1;

    }
    // 成功
    return 0;
}

int spi::xfer(uint8_t *p_tx_buffer, uint8_t tx_len, uint8_t *p_rx_buffer, uint8_t rx_len){
#if 1
    int ret;
    struct spi_ioc_transfer tr[1];

    memset(tr, 0, sizeof(tr));

    tr[0].tx_buf = (unsigned long)p_tx_buffer; 
    tr[0].rx_buf = (unsigned long)p_rx_buffer;
    tr[0].len = tx_len;

    ret = ioctl(fd_, SPI_IOC_MESSAGE(1), tr);
    if(ret == -1){
        std::cout << "can not send message" << std::endl;
        return -1;
    }
#else
    int ret;
    struct spi_ioc_transfer tr[1];
    uint8_t tx_buffer[1024];
    uint8_t rx_buffer[1024];

    memset(tx_buffer, 0, sizeof(tx_buffer));
    memset(rx_buffer, 0, sizeof(rx_buffer));

    if(p_tx_buffer != NULL ){
        memcpy(tx_buffer, p_tx_buffer, tx_len);
    }
    if(p_rx_buffer != NULL){
        memcpy(rx_buffer, p_rx_buffer, rx_len);
    }

    memset(tr, 0, sizeof(tr));

    tr[0].tx_buf = (unsigned long)tx_buffer;
    tr[0].rx_buf = (unsigned long)rx_buffer;
    tr[0].len = tx_len;
    tr[0].delay_usecs = 1000000;
    tr[0].word_delay_usecs = 1000000;

    ret = ioctl(fd_, SPI_IOC_MESSAGE(1), tr);
    if(ret == -1){
        std::cout << "can not send message" << std::endl;
        return -1;
    }
#endif

    return 0;
}

int spi::write(uint8_t *p_tx_buffer, uint8_t tx_len){
    int ret;
    struct spi_ioc_transfer tr[1];

    memset(tr, 0, sizeof(tr));

    tr[0].tx_buf = (unsigned long)p_tx_buffer; 
    tr[0].len = tx_len;

    ret = ioctl(fd_, SPI_IOC_MESSAGE(1), tr);
    if(ret == -1){
        std::cout << "can not send message" << std::endl;
        return -1;
    }

    return 0;
}


int spi::read(uint8_t *p_rx_buffer, uint8_t rx_len) {
    int ret;
    struct spi_ioc_transfer tr[1];

    memset(tr, 0, sizeof(tr));

    tr[0].rx_buf = (unsigned long)p_rx_buffer;
    tr[0].len = rx_len;

    ret = ioctl(fd_, SPI_IOC_MESSAGE(1), tr);
    if(ret == -1){
        std::cout << "can not send message" << std::endl;
        return -1;
    }

    return 0;
}