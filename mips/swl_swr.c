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
  { 'i', 'j', 'k', 'l' },
  { 'i', 'j', 'k', 'l' },
  { 'i', 'j', 'k', 'l' },
  { 'i', 'j', 'k', 'l' }
};

static void swl_be(s32 vAddr)
{
  c8 reg[4] = { 'e', 'f', 'g', 'h' };
  s32 i, j;
  for (j = 0, i = vAddr; i <= 3; j++, i++)
  {
    reg[j] = reg_be[i];
  }
  printf("vAddr %d: %c %c %c %c\n", vAddr, reg[0], reg[1], reg[2], reg[3]);
}

static const c8 mem_swr_be[4][4] =
{
  { 'i', 'j', 'k', 'l' },
  { 'i', 'j', 'k', 'l' },
  { 'i', 'j', 'k', 'l' },
  { 'i', 'j', 'k', 'l' }
};

static void swr_be(s32 vAddr)
{
  c8 reg[4] = { 'e', 'f', 'g', 'h' };
  s32 i, j;
  for (j = (3 - vAddr), i = 0; i <= vAddr; j++, i++)
  {
    reg[j] = reg_be[i];
  }
  printf("vAddr %d: %c %c %c %c\n", vAddr, reg[0], reg[1], reg[2], reg[3]);
}

static const c8 reg_le[4] =
{
  'E', 'F', 'G', 'H',
};

static const c8 mem_swl_le[4][4] =
{
  { 'i', 'j', 'k', 'l' },
  { 'i', 'j', 'k', 'l' },
  { 'i', 'j', 'k', 'l' },
  { 'i', 'j', 'k', 'l' }
};

static void swl_le(s32 vAddr)
{
  c8 reg[4] = { 'e', 'f', 'g', 'h' };
  s32 i, j;
  for (j = 0, i = vAddr; i >= 0; j++, i--)
  {
    reg[j] = reg_le[i];
  }
  printf("vAddr %d: %c %c %c %c\n", vAddr, reg[0], reg[1], reg[2], reg[3]);
}

static const c8 mem_swr_le[4][4] =
{
  { 'i', 'j', 'k', 'l' },
  { 'i', 'j', 'k', 'l' },
  { 'i', 'j', 'k', 'l' },
  { 'i', 'j', 'k', 'l' }
};

static void swr_le(s32 vAddr)
{
  c8 reg[4] = { 'e', 'f', 'g', 'h' };
  s32 i, j;
  for (j = vAddr, i = 3; i >= vAddr; j++, i--)
  {
    reg[j] = reg_le[i];
  }
  printf("vAddr %d: %c %c %c %c\n", vAddr, reg[0], reg[1], reg[2], reg[3]);
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
