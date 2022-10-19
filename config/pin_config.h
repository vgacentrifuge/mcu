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
// USART0 CLK on PE5
#define USART0_CLK_PORT                          gpioPortE
#define USART0_CLK_PIN                           5
#define USART0_ROUTE_LOC                         1

// USART0 CS on PE4
#define USART0_CS_PORT                           gpioPortE
#define USART0_CS_PIN                            4

// USART0 RX on PE6
#define USART0_RX_PORT                           gpioPortE
#define USART0_RX_PIN                            6

// USART0 TX on PE7
#define USART0_TX_PORT                           gpioPortE
#define USART0_TX_PIN                            7

// [USART0]$

// $[USART1]
// USART1 CLK on PD2
#define USART1_CLK_PORT                          gpioPortD
#define USART1_CLK_PIN                           2
#define USART1_ROUTE_LOC                         1

// USART1 CS on PD3
#define USART1_CS_PORT                           gpioPortD
#define USART1_CS_PIN                            3

// USART1 RX on PD1
#define USART1_RX_PORT                           gpioPortD
#define USART1_RX_PIN                            1

// USART1 TX on PD0
#define USART1_TX_PORT                           gpioPortD
#define USART1_TX_PIN                            0

// [USART1]$

// $[USART2]
// [USART2]$

// $[USB]
// [USB]$

// $[CUSTOM_PIN_NAME]
#define KEYPAD_ROW0_PORT                         gpioPortA
#define KEYPAD_ROW0_PIN                          12

#define KEYPAD_ROW1_PORT                         gpioPortA
#define KEYPAD_ROW1_PIN                          13

#define KEYPAD_ROW2_PORT                         gpioPortA
#define KEYPAD_ROW2_PIN                          14

#define KEYPAD_ROW3_PORT                         gpioPortB
#define KEYPAD_ROW3_PIN                          15

#define KEYPAD_COL0_PORT                         gpioPortE
#define KEYPAD_COL0_PIN                          0

#define KEYPAD_COL1_PORT                         gpioPortE
#define KEYPAD_COL1_PIN                          1

#define KEYPAD_COL2_PORT                         gpioPortE
#define KEYPAD_COL2_PIN                          2

#define KEYPAD_COL3_PORT                         gpioPortE
#define KEYPAD_COL3_PIN                          3

// [CUSTOM_PIN_NAME]$

#endif // PIN_CONFIG_H

