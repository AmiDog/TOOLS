/*
 * swl_swr
 */

#include "main.h"

static const c8 reg_be[4] =
{
  'E', 'F', 'G', 'H',
};

static const c8 mem_swl_be[4][4] =
{
  { 'E', 'F', 'G', 'H' },
  { 'i', 'E', 'F', 'G' },
  { 'i', 'j', 'E', 'F' },
  { 'i', 'j', 'k', 'E' }
};

static void swl_be(s32 vAddr)
{
  c8 mem[4] = { 'i', 'j', 'k', 'l' };
  s32 i, j;
  for (j = vAddr, i = 0; i <= (3 - vAddr); j++, i++)
  {
    mem[j] = reg_be[i];
  }
  printf("vAddr %d: %c %c %c %c\n", vAddr, mem[0], mem[1], mem[2], mem[3]);
}

static const c8 mem_swr_be[4][4] =
{
  { 'H', 'j', 'k', 'l' },
  { 'G', 'H', 'k', 'l' },
  { 'F', 'G', 'H', 'l' },
  { 'E', 'F', 'G', 'H' }
};

static void swr_be(s32 vAddr)
{
  c8 mem[4] = { 'i', 'j', 'k', 'l' };
  s32 i, j;
  for (j = 0, i = (3 - vAddr); i <= 3; j++, i++)
  {
    mem[j] = reg_be[i];
  }
  printf("vAddr %d: %c %c %c %c\n", vAddr, mem[0], mem[1], mem[2], mem[3]);
}

static const c8 reg_le[4] =
{
  'E', 'F', 'G', 'H',
};

static const c8 mem_swl_le[4][4] =
{
  { 'i', 'j', 'k', 'E' },
  { 'i', 'j', 'E', 'F' },
  { 'i', 'E', 'F', 'G' },
  { 'E', 'F', 'G', 'H' }
};

static void swl_le(s32 vAddr)
{
  c8 mem[4] = { 'i', 'j', 'k', 'l' };
  s32 i, j;
  for (j = (3 - vAddr), i = 0; i <= vAddr; j++, i++)
  {
    mem[j] = reg_be[i];
  }
  printf("vAddr %d: %c %c %c %c\n", vAddr, mem[0], mem[1], mem[2], mem[3]);
}

static const c8 mem_swr_le[4][4] =
{
  { 'E', 'F', 'G', 'H' },
  { 'F', 'G', 'H', 'l' },
  { 'G', 'H', 'k', 'l' },
  { 'H', 'j', 'k', 'l' }
};

static void swr_le(s32 vAddr)
{
  c8 mem[4] = { 'i', 'j', 'k', 'l' };
  s32 i, j;
  for (j = 0, i = vAddr; i <= 3; j++, i++)
  {
    mem[j] = reg_be[i];
  }
  printf("vAddr %d: %c %c %c %c\n", vAddr, mem[0], mem[1], mem[2], mem[3]);
}

void test_swl_swr(void)
{
  printf("swl le\n");
  swl_le(0);
  swl_le(1);
  swl_le(2);
  swl_le(3);

  printf("swl be\n");
  swl_be(0);
  swl_be(1);
  swl_be(2);
  swl_be(3);

  printf("swr le\n");
  swr_le(0);
  swr_le(1);
  swr_le(2);
  swr_le(3);

  printf("swr be\n");
  swr_be(0);
  swr_be(1);
  swr_be(2);
  swr_be(3);
}
