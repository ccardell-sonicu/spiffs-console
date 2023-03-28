/*
 * SPDX-FileCopyrightText: 2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Unlicense OR CC0-1.0
 */
/* Console example — WiFi commands

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <stdio.h>
#include <string.h>
#include "esp_log.h"
#include "esp_console.h"
#include "argtable3/argtable3.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "esp_spiffs.h"
#include "cmd_spiffs.h"

static const char* TAG = "cmd_spiffs";

/** Arguments used by 'join' function */
static struct {
    struct arg_end *end;
} list_args;

static struct {
    struct arg_str *filename;
    struct arg_end *end;
} cat_args;

static int list_files(int argc, char **argv)
{
    int nerrors = arg_parse(argc, argv, (void **) &list_args);
    if (nerrors != 0) {
        arg_print_errors(stderr, list_args.end, argv[0]);
        return 1;
    }

    ESP_LOGI(__func__, "Listing files");

    /* list all files */

    return 0;
}

static int cat_file(int argc, char **argv)
{
    int nerrors = arg_parse(argc, argv, (void **) &cat_args);
    if (nerrors != 0) {
        arg_print_errors(stderr, cat_args.end, argv[1]);
        return 1;
    }

    ESP_LOGI(__func__, "Cat file %s", cat_args.filename->sval[0]);


    /* Check if file exists */


    /* Print contents of file */


    return 0;
}

void register_spiffs(void)
{

    ESP_LOGI(TAG, "Initializing SPIFFS");

    esp_vfs_spiffs_conf_t conf = {
      .base_path = "/spiffs",
      .partition_label = NULL,
      .max_files = 5,
      .format_if_mount_failed = false
    };

    // Use settings defined above to initialize and mount SPIFFS filesystem.
    // Note: esp_vfs_spiffs_register is an all-in-one convenience function.
    esp_err_t ret = esp_vfs_spiffs_register(&conf);

    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            ESP_LOGE(TAG, "Failed to mount or format filesystem");
        } else if (ret == ESP_ERR_NOT_FOUND) {
            ESP_LOGE(TAG, "Failed to find SPIFFS partition");
        } else {
            ESP_LOGE(TAG, "Failed to initialize SPIFFS (%s)", esp_err_to_name(ret));
        }
        return;
    }

    // register command line things
    list_args.end = arg_end(0);

    const esp_console_cmd_t list_cmd = {
        .command = "ls",
        .help = "List contents of directory",
        .hint = NULL,
        .func = &list_files,
        .argtable = &list_args
    };

    ESP_ERROR_CHECK( esp_console_cmd_register(&list_cmd) );


    cat_args.filename = arg_str0(NULL, NULL, "<filename>", "File name");
    cat_args.end = arg_end(1);

    const esp_console_cmd_t cat_cmd = {
        .command = "cat",
        .help = "Print the contects of a file",
        .hint = NULL,
        .func = &cat_file,
        .argtable = &cat_args
    };

    ESP_ERROR_CHECK( esp_console_cmd_register(&cat_cmd) );
}
