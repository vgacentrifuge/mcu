/***************************************************************************//**
 * @file
 * @brief Top level application functions
 ******************************************************************************/

#pragma once

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void);

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void);

void home_menu(void);

void save_state(void);

void load_state(void);

void hotkeys_menu(void);

void use_image_menu(void);

void transition_menu(void);

void slideshow_menu(void);
