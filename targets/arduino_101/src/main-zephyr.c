/* Copyright 2016 Samsung Electronics Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <misc/shell.h>

int jerryscript_entry (int argc, char *argv[]);
int jerryscript_cmd (int argc, char *argv[]);
int jerryscript_test (int argc, char *argv[]);

#if defined(CONFIG_STDOUT_CONSOLE)
#include <stdio.h>
#define PRINT           printf
#else
#include <misc/printk.h>
#define PRINT           printk
#endif

void
print_version()
{
    printf("Version " __DATE__ __TIME__ " ");
}

void
print_help()
{
    printf("Help\n");
}

static void
shell_cmd_help(int argc, char* argv[])
{
    print_help();
}

static void
shell_cmd_version(int argc, char* argv[])
{
    print_version();
}

static void
shell_cmd_execute(int argc, char* argv[])
{
    printf("START %d\n", argc);
    for (int t=0; t<argc; t++) {
        printf("%d[%s]\n",t, argv[t]);
    }

    if (argc<1)
        return;

   jerryscript_cmd (argc, argv);
   printf("END\n");
}

static void
shell_cmd_test(int argc, char* argv[])
{
    jerryscript_test(argc, argv);
}

const struct shell_cmd commands[] = { { "syntax", shell_cmd_help },
                                      { "version", shell_cmd_version },
                                      { "js", shell_cmd_execute },
                                      { "test", shell_cmd_test },
                                      { NULL, NULL } };

void main(void) {
    shell_init("jerryscript> ", commands);
    jerryscript_entry(0,NULL);
}

void __attribute__ ((noreturn)) abort() {
    while(1) {

    };
}

void __attribute__ ((noreturn)) exit(int value) {
    while(1) {

    };
}
