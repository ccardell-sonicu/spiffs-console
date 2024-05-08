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
#include <dirent.h> 
#include "esp_log.h"
#include "esp_console.h"
#include "argtable3/argtable3.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "esp_vfs.h"
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

static struct {
    struct arg_str *line;
    struct arg_str *write_type;
    struct arg_str *filename;
    struct arg_end *end;
} echo_args;

typedef enum {
	WRITE_TYPE_UNDEFINED,
    WRITE_TYPE_OVEWRWRITE,
    WRITE_TYPE_APPEND
} write_type_t;

static int list_files(int argc, char **argv)
{
    int nerrors = arg_parse(argc, argv, (void **) &list_args);
    if (nerrors != 0) {
        arg_print_errors(stderr, list_args.end, argv[0]);
        return 1;
    }

    ESP_LOGI(__func__, "Listing files");

    /* list all files */
    // FILE * fp;
    // fp = fopen("/spiffs", "r");
    // if (fp == NULL) {
    //     ESP_LOGE(TAG, "Failed to open /spiffs");
    // }

    DIR *d;
    struct dirent *dir;
    d = opendir("/spiffs");
    if (d) {
        while ((dir = readdir(d)) != NULL) {
        printf("%s\n", dir->d_name);
        }
        closedir(d);
    }

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
    FILE* ptr;
    int ch;
 
    char filename[64] = { "\0" };
    sprintf(filename, "/spiffs/%s", cat_args.filename->sval[0]);

    // Opening file in reading mode
    ptr = fopen(filename, "r");
 
    if (NULL == ptr) {
        printf("file can't be opened \n");
        return -1;
    }
  
    // Printing what is written in file
    // character by character using loop.
    do {
        ch = fgetc(ptr);
        printf("%c", ch);
 
        // Checking if character is not EOF.
        // If it is EOF stop reading.
    } while (ch != EOF);
 
    // Closing the file
    fclose(ptr);

    return 0;
}

static int echo_file(int argc, char **argv)
{
    int nerrors = arg_parse(argc, argv, (void **) &echo_args);
    if (nerrors != 0) {
        arg_print_errors(stderr, echo_args.end, argv[1]);
        return 1;
    }

    // ESP_LOGI(__func__, "line %s", echo_args.line->sval[0]);
    // ESP_LOGI(__func__, "write_type %s", echo_args.write_type->sval[0]);
    // ESP_LOGI(__func__, "filename %s", echo_args.filename->sval[0]);

    /* Check write type */ 
    if (echo_args.write_type->sval[0][0] != '>') {
        ESP_LOGE(__func__, "Invalid write type. Must be '>' or '>>'.");
        return 1;
    }

    write_type_t write_type = WRITE_TYPE_UNDEFINED;

    if (echo_args.write_type->sval[0][1] == '\0') {
        write_type = WRITE_TYPE_OVEWRWRITE;
    } else if ((echo_args.write_type->sval[0][1] == '>') && (echo_args.write_type->sval[0][2] == '\0')) {
        write_type = WRITE_TYPE_APPEND;
    } else {
        printf("Invalid write type. Must be '>' or '>>'.");
        return 1;
    }

    /* Check if file exists */
    FILE* ptr = NULL;
 
    char filename[64] = { "\0" };
    sprintf(filename, "/spiffs/%s", echo_args.filename->sval[0]);

    // Opening file
    if (WRITE_TYPE_OVEWRWRITE == write_type) {
        ESP_LOGI(__func__, "Overwriting file %s", filename);
        ptr = fopen(filename, "w+");
    } else if (WRITE_TYPE_APPEND == write_type) {
        ESP_LOGI(__func__, "Appending to file %s", filename);
        ptr = fopen(filename, "a+");
    } else {
        printf("I shouldn't be reachable\n");
        return 1;
    }
 
    if (NULL == ptr) {
        printf("file can't be opened \n");
        return -1;
    }
 
    fprintf(ptr, "%s\n", echo_args.line->sval[0]);

    // Closing the file
    fclose(ptr);

    return 0;
}

void register_spiffs(void)
{

    ESP_LOGI(TAG, "Initializing SPIFFS");

    esp_vfs_spiffs_conf_t conf = {
      .base_path = "/spiffs",
      .partition_label = NULL,
      .max_files = 5,
      .format_if_mount_failed = true
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
        .help = "Print the contents of a file",
        .hint = NULL,
        .func = &cat_file,
        .argtable = &cat_args
    };

    ESP_ERROR_CHECK( esp_console_cmd_register(&cat_cmd) );

    echo_args.line = arg_str0(NULL, NULL, "<line>", "The line of text being written to the file");
    echo_args.write_type = arg_str0(NULL, NULL, "<write type>", "'>>' appends, '>' overwrites");
    echo_args.filename = arg_str0(NULL, NULL, "<filename>", "File name");
    echo_args.end = arg_end(1);

    const esp_console_cmd_t echo_cmd = {
        .command = "echo",
        .help = "Echo implemented to write to files.",
        .hint = NULL,
        .func = &echo_file,
        .argtable = &echo_args
    };

    ESP_ERROR_CHECK( esp_console_cmd_register(&echo_cmd) );

}
