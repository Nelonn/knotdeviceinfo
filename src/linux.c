/*
 * This file is part of knotdeviceinfo.
 *
 * For license and copyright information please follow this link:
 * https://github.com/noseam-env/knotdeviceinfo/blob/master/LEGAL
 */

#include "target.h"

#if defined(OS_LINUX)

#include <knot/deviceinfo.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/utsname.h>

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
    memset(info, 0, sizeof(KNDeviceInfo));

    info->platform = KN_PLATFORM_LINUX;

    struct utsname uts = {0};
    if (uname(&uts) == 0) {
        if (strlen(uts.release) > 0) { // 6.14.2-arch1-1
            info->kernel_version = strdup(uts.release);
        }
        if (strlen(uts.version) > 0) { // #1 SMP PREEMPT_DYNAMIC Thu, 10 Apr 2025 18:43:59 +0000
            info->kernel_build = strdup(uts.version);
        }
    }

    FILE* infile = fopen("/etc/os-release", "r");
    if (infile) {
        char line[MAX_LINE_LENGTH];
        char key[MAX_KEY_LENGTH];
        char value[MAX_VALUE_LENGTH];

        while (fgets(line, sizeof(line), infile) != NULL) {
            char* delimiter_pos = strchr(line, '=');
            if (!delimiter_pos) continue;
            *delimiter_pos = '\0';
            strncpy(key, line, sizeof(key)-1);
            strncpy(value, delimiter_pos + 1, sizeof(value)-1);

            // Remove newline character from value
            char* newline_pos = strchr(value, '\n');
            if (newline_pos != NULL) {
                *newline_pos = '\0';
            }

            // Remove double quotes from value
            char* quote_pos = strchr(value, '"');
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
        fclose(infile);
    }

    infile = fopen("/sys/class/dmi/id/product_name", "r");
    if (infile) {
        char buffer[MAX_LINE_LENGTH];
        if (fgets(buffer, sizeof(buffer), infile)) {
            size_t len = strlen(buffer);
            if (len && buffer[len - 1] == '\n') {
               buffer[len - 1] = '\0';
            }
            if (strcmp(buffer, "To Be Filled By O.E.M.") != 0) {
                info->model_raw = strdup(buffer);
            }
        }
        fclose(infile);
    }

    char hostname[MAX_LINE_LENGTH];
    if (gethostname(hostname, sizeof(hostname)) == 0) {
        info->name = strdup(hostname);
    }
}

#endif
