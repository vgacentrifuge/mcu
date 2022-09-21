#ifndef DMADRV_CONFIG_H
#define DMADRV_CONFIG_H

  // DMADRV DMA interrupt priority configuration option.
  // Set DMA interrupt priority. Range is 0..7, 0 is the highest priority.
#define EMDRV_DMADRV_DMA_IRQ_PRIORITY 4
  // DMADRV channel count configuration option.
  // A number of DMA channels to support. A lower DMA channel count will reduce
  // RAM footprint.
#define EMDRV_DMADRV_DMA_CH_COUNT 4
  // DMADRV native API configuration option.
  // Use the native emlib API of the DMA controller in addition to DMADRV
  // housekeeping functions, such as AllocateChannel/FreeChannel, and so on.
#define EMDRV_DMADRV_USE_NATIVE_API

#endif // DMADRV_CONFIG_H