# DMA

## DREQ

```
DREQ DREQ Channel     DREQ DREQ Channel     DREQ DREQ Channel     DREQ DREQ Channel
0    DREQ_PIO0_TX0    10   DREQ_PIO1_TX2    20  DREQ_UART0_TX     30   DREQ_PWM_WRAP6
1    DREQ_PIO0_TX1    11   DREQ_PIO1_TX3    21  DREQ_UART0_RX     31   DREQ_PWM_WRAP7
2    DREQ_PIO0_TX2    12   DREQ_PIO1_RX0    22  DREQ_UART1_TX     32   DREQ_I2C0_TX
3    DREQ_PIO0_TX3    13   DREQ_PIO1_RX1    23  DREQ_UART0_RX     33   DREQ_I2C0_RX
4    DREQ_PIO0_RX0    14   DREQ_PIO1_RX2    24  DREQ_PWM_WRAP0    34   DREQ_I2C1_TX
5    DREQ_PIO0_RX1    15   DREQ_PIO1_RX3    25  DREQ_PWM_WRAP1    35   DREQ_I2C1_RX
6    DREQ_PIO0_RX2    16   DREQ_SPI0_TX     26  DREQ_PWM_WRAP2    36   DREQ_ADC
7    DREQ_PIO0_RX3    17   DREQ_SPI0_RX     27  DREQ_PWM_WRAP3    37   DREQ_XIP_STREAM
8    DREQ_PIO1_TX0    18   DREQ_SPI1_TX     28  DREQ_PWM_WRAP4    38   DREQ_XIP_SSITX
9    DREQ_PIO1_TX1    19   DREQ_SPI1_RX     29  DREQ_PWM_WRAP5    39   DREQ_XIP_SSIRX
```

[Source](https://people.ece.cornell.edu/land/courses/ece4760/RP2040/index_rp2040_testing.html)

E.g. TREQ source 25 (DREQ\_PWM\_WRAP1) is used to trigger DMA channel 0 (I think he means 1).


`void channel_config_set_dreq(ma_channel_config *c, uint dreq)`

`dreq` can either be a DREQ or TREQ.

TREQs are:
0x3b : Select Timer 0 as TREQ
0x3c : Select Timer 1 as TREQ
0x3d : Select Timer 2 as TREQ ("Optional")
0x3e : Select Timer 3 as TREQ ("Optional")
0x3f : Permanent request, for unpaced transfers
