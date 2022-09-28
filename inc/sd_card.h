/*
 * sd_card.h
 *
 *  Created on: 28. sep. 2022
 *      Author: amatho
 */

#ifndef INC_SD_CARD_H_
#define INC_SD_CARD_H_

#define CMD0 0x40 // Reset
#define CMD1 0x41 // Init

void sd_card_init(void);

void sd_card_test(void);

#endif /* INC_SD_CARD_H_ */
