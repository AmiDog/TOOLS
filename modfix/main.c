/*
 * main.c
 */

#include "mod.h"

#define _SIZEOF(_a) \
  printf("size of %s: %d\n", #_a, sizeof(_a))

int main(int argc, char **argv)
{
  _SIZEOF(mod_channel_data_t);
  _SIZEOF(mod_division_data_t);
  _SIZEOF(mod_pattern_data_t);
  if (argc == 2)
  {
    mod_setup();
    mod_load(argv[1]);
    mod_process();
    mod_save("../../test.mod");
  }
  return 0;
}
