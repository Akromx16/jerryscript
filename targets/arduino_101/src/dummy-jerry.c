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
#include <stdarg.h>

#include "jerry.h"
#include "jrt/jrt.h"

/**
 * memcmp
 *
 * @return 0, if areas are equal;
 *         <0, if first area's content is lexicographically less, than second area's content;
 *         >0, otherwise
 */
int
memcmp (const void *s1, /**< first area */
        const void *s2, /**< second area */
        size_t n) /**< area size */
{
  const uint8_t *area1_p = (uint8_t *) s1, *area2_p = (uint8_t *) s2;
  while (n--)
  {
    int diff = ((int) *area1_p++) - ((int) *area2_p++);
    if (diff)
    {
      return diff;
    }
  }

  return 0;
} /* memcmp */

/**
 * State of pseudo-random number generator
 */
static uint32_t libc_random_gen_state[4] = { 1455997910, 1999515274, 1234451287, 1949149569 };

/**
 * Generate pseudo-random integer
 *
 * Note:
 *      The function implements George Marsaglia's XorShift random number generator
 *
 * @return integer in range [0; RAND_MAX]
 */
int
rand (void)
{
  uint32_t intermediate = libc_random_gen_state[0] ^ (libc_random_gen_state[0] << 11);
  intermediate ^= intermediate >> 8;

  libc_random_gen_state[0] = libc_random_gen_state[1];
  libc_random_gen_state[1] = libc_random_gen_state[2];
  libc_random_gen_state[2] = libc_random_gen_state[3];

  libc_random_gen_state[3] ^= libc_random_gen_state[3] >> 19;
  libc_random_gen_state[3] ^= intermediate;

  return libc_random_gen_state[3] % (RAND_MAX + 1u);
} /* rand */

typedef uint64_t jmp_buf[14];

/**
 * The module interface routine
 */
int jerryscript_entry (int argc, char *argv[]);
int jerryscript_cmd (int argc, char *argv[]);
int jerryscript_test (int argc, char *argv[]);

/**
 * Maximum command line arguments number
 */
#define JERRY_MAX_COMMAND_LINE_ARGS (16)

/**
 * Standalone Jerry exit codes
 */
#define JERRY_STANDALONE_EXIT_CODE_OK   (0)
#define JERRY_STANDALONE_EXIT_CODE_FAIL (1)

int jerryscript_test (int argc, char *argv[]) {
    if (argc<=0 || argv==NULL) return 0;

    const char script [] = "print ('Hi JS!');";
    //const char script [] = "1+1;";
    jerry_completion_code_t ret_code;

    ret_code = jerry_run_simple ((jerry_api_char_t *) script, strlen(script), JERRY_FLAG_EMPTY);
    return ret_code;
}

int jerryscript_cmd (int argc, char *argv[])
{
    printf("jerryscript_cmd \n");

    if (argc<=0 || argv == NULL) {
        printf("Not enough params\n");
    }

    /*
    jerry_api_object_t *error_obj_p = NULL;
    jerry_completion_code_t ret_code;

    printf("Parse [%s]\n", argv[1]);
    jerry_parse (argv[1], strlen (argv[1]), &error_obj_p);
    printf("Run \n");
    jerry_run (&error_obj_p);
    printf("Cleanup \n");
    jerry_cleanup ();
    */
    return 0;
}

int jerryscript_entry (int argc, char *argv[])
{
  if (argc >= JERRY_MAX_COMMAND_LINE_ARGS)
  {
    JERRY_ERROR_MSG ("Too many command line arguments. Current maximum is %d\n",
             JERRY_MAX_COMMAND_LINE_ARGS);

    return JERRY_STANDALONE_EXIT_CODE_FAIL;
  }

  //const char *file_names[JERRY_MAX_COMMAND_LINE_ARGS];
  int i;
  //int files_counter = 0;

  for (i = 1; i < argc; i++)
  {
    printf ("PARAM %d : [%s]\n", i, argv[i]);
  }

  jerry_flag_t flags = JERRY_FLAG_EMPTY;
  jerry_init (flags);

  printf("Init jerryscript_entry\n");

  //jerry_flag_t flags = JERRY_FLAG_EMPTY;
/*
  for (i = 1; i < argc; i++)
  {
    if (!strcmp ("-v", argv[i]))
    {
      printf ("Build date: \t%s\n", jerry_build_date);
      printf ("Commit hash:\t%s\n", jerry_commit_hash);
      printf ("Branch name:\t%s\n", jerry_branch_name);
    }
    else if (!strcmp ("--mem-stats", argv[i]))
    {
      flags |= JERRY_FLAG_MEM_STATS;
    }
    else if (!strcmp ("--mem-stats-separate", argv[i]))
    {
      flags |= JERRY_FLAG_MEM_STATS_SEPARATE;
    }
    else if (!strcmp ("--parse-only", argv[i]))
    {
      flags |= JERRY_FLAG_PARSE_ONLY;
    }
    else if (!strcmp ("--show-opcodes", argv[i]))
    {
      flags |= JERRY_FLAG_SHOW_OPCODES;
    }
    else if (!strcmp ("--abort-on-fail", argv[i]))
    {
      flags |= JERRY_FLAG_ABORT_ON_FAIL;
    }
    else if (!strcmp ("--log-level", argv[i]))
    {
      flags |= JERRY_FLAG_ENABLE_LOG;
      if (++i < argc && strlen (argv[i]) == 1 && argv[i][0] >='0' && argv[i][0] <= '3')
      {
#ifdef JERRY_ENABLE_LOG
        jerry_debug_level = argv[i][0] - '0';
#endif
      }
      else
      {
        JERRY_ERROR_MSG ("Error: wrong format or invalid argument\n");
        return JERRY_STANDALONE_EXIT_CODE_FAIL;
      }
    }
    else
    {
      file_names[files_counter++] = argv[i];
    }
  }

  if (files_counter == 0)
  {
    printf ("Jerry: file count 0\n");
    return JERRY_STANDALONE_EXIT_CODE_OK;
  }

  size_t source_size;
  char *source_p = read_sources (file_names, files_counter, &source_size);

  if (source_p == NULL)
  {
    JERRY_ERROR_MSG ("JERRY_STANDALONE_EXIT_CODE_FAIL\n");
    return JERRY_STANDALONE_EXIT_CODE_FAIL;
  }

  jerry_completion_code_t ret_code;

  ret_code = jerry_run_simple ((jerry_api_char_t *) source_p, source_size, flags);

  free(source_p);

  if (ret_code != JERRY_COMPLETION_CODE_OK)
  {
    return JERRY_STANDALONE_EXIT_CODE_FAIL;
  }
*/
  return JERRY_STANDALONE_EXIT_CODE_OK;
}

/**
 * Provide log message to filestream implementation for the engine.
 */
int jerry_port_logmsg (FILE* stream, const char* format, ...)
{
  va_list args;
  int count;
  va_start (args, format);
  count = vfprintf (stream, format, args);
  va_end (args);
  return count;
}

/**
 * Provide error message to console implementation for the engine.
 */
int jerry_port_errormsg (const char* format, ...)
{
  va_list args;
  int count;
  va_start (args, format);
  count = vfprintf (stderr, format, args);
  va_end (args);
  return count;
}

/**
 * Provide output character to console implementation for the engine.
 */
int jerry_port_putchar (int c)
{
    printf("%c",c);
    return 1;
  //return putchar(c);
}
