/***************************************************************************//**
 * @file
 * @brief Functions for configuring the two ADCs using their reset lines and I2C
 ******************************************************************************/

#pragma once

// Initializes both ADCs by changing their reset to inactive,
// and using I2C to configure control registers.
// Both ADCs are connected to the same I2C1 bus, but have different addresses.
void adcs_init();
