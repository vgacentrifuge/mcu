#ifndef PIN_CONFIG_H
#define PIN_CONFIG_H

// $[ACMP0]
// [ACMP0]$

// $[ACMP1]
// [ACMP1]$

// $[ADC0]
// [ADC0]$

// $[BU]
// [BU]$

// $[CMU]
// [CMU]$

// $[DAC0]
// [DAC0]$

// $[DBG]
// DBG SWO on PF2
#define DBG_SWO_PORT                             gpioPortF
#define DBG_SWO_PIN                              2
#define DBG_ROUTE_LOC                            0

// [DBG]$

// $[EBI]
// [EBI]$

// $[ETM]
// [ETM]$

// $[GPIO]
// [GPIO]$

// $[HFXO]
// [HFXO]$

// $[I2C0]
// [I2C0]$

// $[I2C1]
// I2C1 SCL on PC5
#define I2C1_SCL_PORT                            gpioPortC
#define I2C1_SCL_PIN                             5
#define I2C1_ROUTE_LOC                           0

// I2C1 SDA on PC4
#define I2C1_SDA_PORT                            gpioPortC
#define I2C1_SDA_PIN                             4

// [I2C1]$

// $[LCD]
// [LCD]$

// $[LESENSE]
// [LESENSE]$

// $[LETIMER0]
// [LETIMER0]$

// $[LEUART0]
// [LEUART0]$

// $[LEUART1]
// [LEUART1]$

// $[LFXO]
// [LFXO]$

// $[PCNT0]
// [PCNT0]$

// $[PCNT1]
// [PCNT1]$

// $[PCNT2]
// [PCNT2]$

// $[PRS.CH0]
// [PRS.CH0]$

// $[PRS.CH1]
// [PRS.CH1]$

// $[PRS.CH2]
// [PRS.CH2]$

// $[PRS.CH3]
// [PRS.CH3]$

// $[TIMER0]
// [TIMER0]$

// $[TIMER1]
// [TIMER1]$

// $[TIMER2]
// [TIMER2]$

// $[TIMER3]
// [TIMER3]$

// $[UART0]
// [UART0]$

// $[UART1]
// [UART1]$

// $[USART0]
// USART0 CLK on PE12
#define USART0_CLK_PORT                          gpioPortE
#define USART0_CLK_PIN                           12
#define USART0_ROUTE_LOC                         0

// USART0 CS on PE13
#define USART0_CS_PORT                           gpioPortE
#define USART0_CS_PIN                            13

// USART0 RX on PE11
#define USART0_RX_PORT                           gpioPortE
#define USART0_RX_PIN                            11

// USART0 TX on PE10
#define USART0_TX_PORT                           gpioPortE
#define USART0_TX_PIN                            10

// [USART0]$

// $[USART1]
// USART1 CLK on PB7
#define USART1_CLK_PORT                          gpioPortB
#define USART1_CLK_PIN                           7
#define USART1_ROUTE_LOC                         0

// USART1 CS on PB8
#define USART1_CS_PORT                           gpioPortB
#define USART1_CS_PIN                            8

// USART1 RX on PC1
#define USART1_RX_PORT                           gpioPortC
#define USART1_RX_PIN                            1

// USART1 TX on PC0
#define USART1_TX_PORT                           gpioPortC
#define USART1_TX_PIN                            0

// [USART1]$

// $[USART2]
// [USART2]$

// $[USB]
// [USB]$

// $[CUSTOM_PIN_NAME]
#define FPGA_PROGRAM_B_PORT                      gpioPortA
#define FPGA_PROGRAM_B_PIN                       0

#define FPGA_INIT_B_PORT                         gpioPortA
#define FPGA_INIT_B_PIN                          1

#define GPIO_EXTRA_4_PORT                        gpioPortA
#define GPIO_EXTRA_4_PIN                         3

#define GPIO_EXTRA_8_PORT                        gpioPortA
#define GPIO_EXTRA_8_PIN                         5

#define GPIO_EXTRA_6_PORT                        gpioPortA
#define GPIO_EXTRA_6_PIN                         6

#define GPIO_LED0_PORT                           gpioPortA
#define GPIO_LED0_PIN                            14

#define FPGA_CONFIG_DONE_PORT                    gpioPortA
#define FPGA_CONFIG_DONE_PIN                     15

#define GPIO_EXTRA_7_PORT                        gpioPortB
#define GPIO_EXTRA_7_PIN                         1

#define GPIO_EXTRA_5_PORT                        gpioPortB
#define GPIO_EXTRA_5_PIN                         5

#define GPIO_EXTRA_3_PORT                        gpioPortB
#define GPIO_EXTRA_3_PIN                         6

#define ADC1_DDC_SDA_PORT                        gpioPortC
#define ADC1_DDC_SDA_PIN                         6

#define ADC1_DDC_SCL_PORT                        gpioPortC
#define ADC1_DDC_SCL_PIN                         7

#define ADC2_RESET_PORT                          gpioPortD
#define ADC2_RESET_PIN                           0

#define ADC1_RESET_PORT                          gpioPortD
#define ADC1_RESET_PIN                           1

#define ADC2_DDC_SDA_PORT                        gpioPortD
#define ADC2_DDC_SDA_PIN                         6

#define ADC2_DDC_SCL_PORT                        gpioPortD
#define ADC2_DDC_SCL_PIN                         7

#define GPIO_EXTRA_10_PORT                       gpioPortD
#define GPIO_EXTRA_10_PIN                        10

#define GPIO_EXTRA_11_PORT                       gpioPortE
#define GPIO_EXTRA_11_PIN                        14

#define GPIO_EXTRA_12_PORT                       gpioPortE
#define GPIO_EXTRA_12_PIN                        15


#define GPIO_EXTRA_9_PORT                        gpioPortF
#define GPIO_EXTRA_9_PIN                         7




#define DEVBOARD                                 1
#ifdef DEVBOARD

// Defined to work with the devboard exposed pins

#define KEYPAD_ROW0_PORT                         gpioPortD
#define KEYPAD_ROW0_PIN                          2

#define KEYPAD_ROW1_PORT                         gpioPortD
#define KEYPAD_ROW1_PIN                          3

#define KEYPAD_ROW2_PORT                         gpioPortD
#define KEYPAD_ROW2_PIN                          4

#define KEYPAD_ROW3_PORT                         gpioPortD
#define KEYPAD_ROW3_PIN                          5

#define KEYPAD_COL0_PORT                         gpioPortD
#define KEYPAD_COL0_PIN                          8

#define KEYPAD_COL1_PORT                         gpioPortD
#define KEYPAD_COL1_PIN                          13

#define KEYPAD_COL2_PORT                         gpioPortD
#define KEYPAD_COL2_PIN                          14

#define KEYPAD_COL3_PORT                         gpioPortD
#define KEYPAD_COL3_PIN                          15

#else

#define KEYPAD_COL3_PORT                         gpioPortE
#define KEYPAD_COL3_PIN                          4

#define KEYPAD_COL1_PORT                         gpioPortE
#define KEYPAD_COL1_PIN                          5

#define KEYPAD_ROW2_PORT                         gpioPortF
#define KEYPAD_ROW2_PIN                          5

#define KEYPAD_ROW1_PORT                         gpioPortF
#define KEYPAD_ROW1_PIN                          6

#define KEYPAD_ROW3_PORT                         gpioPortF
#define KEYPAD_ROW3_PIN                          8

#define KEYPAD_COL2_PORT                         gpioPortF
#define KEYPAD_COL2_PIN                          10

#define KEYPAD_COL0_PORT                         gpioPortF
#define KEYPAD_COL0_PIN                          11

#define KEYPAD_ROW0_PORT                         gpioPortF
#define KEYPAD_ROW0_PIN                          12

#endif

// [CUSTOM_PIN_NAME]$

#endif // PIN_CONFIG_H

