/*
 * SPDX-FileCopyrightText: 2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Unlicense OR CC0-1.0
 */
/* Console example — declarations of command registration functions.

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

// Register SPIFFS functions
void register_spiffs(void);

#ifdef __cplusplus
}
#endif


// TODO

/**
 * Check integrity of SPIFFS
 *
 * @param partition_label  Same label as passed to esp_vfs_spiffs_register
 * @return
 *          - ESP_OK                  if successful
 *          - ESP_ERR_INVALID_STATE   if not mounted
 *          - ESP_FAIL                on error
 */
// esp_err_t esp_spiffs_check(const char* partition_label);