/*
 * main.c
 */

#include "mod.h"

int main(int argc, char **argv)
{
  if (argc == 2)
  {
    mod_load(argv[1]);
    mod_process();
    mod_save("../../test.mod");
  }
  return 0;
}
