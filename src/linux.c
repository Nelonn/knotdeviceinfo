/*
 * This file is part of knotdeviceinfo.
 *
 * For license and copyright information please follow this link:
 * https://github.com/noseam-env/knotdeviceinfo/blob/master/LEGAL
 */

#include "os.h"

#if defined(OS_LINUX)

#include <knot/deviceinfo.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define MAX_LINE_LENGTH 256
#define MAX_KEY_LENGTH 64
#define MAX_VALUE_LENGTH 128

/*

Example output in Debian 12:

$ cat /etc/os-release
PRETTY_NAME="Debian GNU/Linux 12 (bookworm)"
NAME="Debian GNU/Linux"
VERSION_ID="12"
VERSION="12 (bookworm)"
VERSION_CODENAME=bookworm
ID=debian
HOME_URL="https://www.debian.org/"
SUPPORT_URL="https://www.debian.org/support"
BUG_REPORT_URL="https://bugs.debian.org/"

*/

void KNDeviceInfoFetch(KNDeviceInfo* info) {
    info->platform = strdup("Linux");

    FILE *infile = fopen("/etc/os-release", "r");
    if (infile == NULL) {
      return;
    }

    char line[MAX_LINE_LENGTH];
    char key[MAX_KEY_LENGTH];
    char value[MAX_VALUE_LENGTH];

    while (fgets(line, sizeof(line), infile) != NULL) {
        char *delimiter_pos = strchr(line, '=');
        if (delimiter_pos != NULL) {
            *delimiter_pos = '\0';
            strcpy(key, line);
            strcpy(value, delimiter_pos + 1);

            // Remove newline character from value
            char *newline_pos = strchr(value, '\n');
            if (newline_pos != NULL) {
                *newline_pos = '\0';
            }

            // Remove double quotes from value
            char *quote_pos = strchr(value, '"');
            if (quote_pos != NULL) {
                memmove(quote_pos, quote_pos + 1, strlen(quote_pos));
                quote_pos = strchr(value, '"');
                if (quote_pos != NULL) {
                    *quote_pos = '\0';
                }
            }

            if (strcmp(key, "NAME") == 0) {
                info->system_name = strdup(value);
            } else if (strcmp(key, "VERSION_ID") == 0) {
                info->system_version = strdup(value);
            }
        }
    }
    fclose(infile);

    char hostname[MAX_LINE_LENGTH];
    if (gethostname(hostname, sizeof(hostname)) == 0) {
        info->name = strdup(hostname);
    }
}

#endif
