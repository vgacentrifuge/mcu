#include "spidrv.h"
#include "sl_spidrv_instances.h"
#include "sl_assert.h"


#include "sl_spidrv_sd_card_config.h"

SPIDRV_HandleData_t sl_spidrv_sd_card_handle_data;
SPIDRV_Handle_t sl_spidrv_sd_card_handle = &sl_spidrv_sd_card_handle_data;

SPIDRV_Init_t sl_spidrv_init_sd_card = {
  .port = SL_SPIDRV_SD_CARD_PERIPHERAL,
#if defined(_USART_ROUTELOC0_MASK)
  .portLocationTx = SL_SPIDRV_SD_CARD_TX_LOC,
  .portLocationRx = SL_SPIDRV_SD_CARD_RX_LOC,
  .portLocationClk = SL_SPIDRV_SD_CARD_CLK_LOC,
#if defined(SL_SPIDRV_SD_CARD_CS_LOC)
  .portLocationCs = SL_SPIDRV_SD_CARD_CS_LOC,
#endif
#elif defined(_GPIO_USART_ROUTEEN_MASK)
  .portTx = SL_SPIDRV_SD_CARD_TX_PORT,
  .portRx = SL_SPIDRV_SD_CARD_RX_PORT,
  .portClk = SL_SPIDRV_SD_CARD_CLK_PORT,
#if defined(SL_SPIDRV_SD_CARD_CS_PORT)
  .portCs = SL_SPIDRV_SD_CARD_CS_PORT,
#endif
  .pinTx = SL_SPIDRV_SD_CARD_TX_PIN,
  .pinRx = SL_SPIDRV_SD_CARD_RX_PIN,
  .pinClk = SL_SPIDRV_SD_CARD_CLK_PIN,
#if defined(SL_SPIDRV_SD_CARD_CS_PIN)
  .pinCs = SL_SPIDRV_SD_CARD_CS_PIN,
#endif
#else
  .portLocation = SL_SPIDRV_SD_CARD_ROUTE_LOC,
#endif
  .bitRate = SL_SPIDRV_SD_CARD_BITRATE,
  .frameLength = SL_SPIDRV_SD_CARD_FRAME_LENGTH,
  .dummyTxValue = 0xFF,
  .type = SL_SPIDRV_SD_CARD_TYPE,
  .bitOrder = SL_SPIDRV_SD_CARD_BIT_ORDER,
  .clockMode = SL_SPIDRV_SD_CARD_CLOCK_MODE,
  .csControl = SL_SPIDRV_SD_CARD_CS_CONTROL,
  .slaveStartMode = SL_SPIDRV_SD_CARD_SLAVE_START_MODE,
};

void sl_spidrv_init_instances(void) {
  SPIDRV_Init(sl_spidrv_sd_card_handle, &sl_spidrv_init_sd_card);
}
