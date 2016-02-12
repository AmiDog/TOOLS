/*
 * lwl_lwr
 */

#include "types.h"

static const c8 data_be[5][12] =
{
  { '0', '1', '2', '3', 'I', 'J', 'K', 'L', '4', '5', '6', '7' },
  { '0', '1', '2', '3', '4', 'I', 'J', 'K', 'L', '5', '6', '7' },
  { '0', '1', '2', '3', '4', '5', 'I', 'J', 'K', 'L', '6', '7' },
  { '0', '1', '2', '3', '4', '5', '6', 'I', 'J', 'K', 'L', '7' },
  { '0', '1', '2', '3', '4', '5', '6', '7', 'I', 'J', 'K', 'L' }
};

static const c8 reg_lwl_be[4][4] =
{
  { 'I', 'J', 'K', 'L' },
  { 'J', 'K', 'L', 'h' },
  { 'K', 'L', 'g', 'h' },
  { 'L', 'f', 'g', 'h' }
};

static void lwl_be(s32 vAddr)
{
  c8 reg[4] = { 'e', 'f', 'g', 'h' };
  s32 i, j;
  for (j = 0, i = vAddr; i <= 3; j++, i++)
  {
    reg[j] = data_be[0][4 + i];
  }
  printf("vAddr %d: %c %c %c %c\n", vAddr, reg[0], reg[1], reg[2], reg[3]);
}

static const c8 reg_lwr_be[4][4] =
{
  { 'e', 'f', 'g', 'I' },
  { 'e', 'f', 'I', 'J' },
  { 'e', 'I', 'J', 'K' },
  { 'I', 'J', 'K', 'L' }
};

static void lwr_be(s32 vAddr)
{
  c8 reg[4] = { 'e', 'f', 'g', 'h' };
  s32 i, j;
  for (j = (3 - vAddr), i = 0; i <= vAddr; j++, i++)
  {
    reg[j] = data_be[0][4 + i];
  }
  printf("vAddr %d: %c %c %c %c\n", vAddr, reg[0], reg[1], reg[2], reg[3]);
}

static const c8 data_le[5][12] =
{
  { '0', '1', '2', '3', 'L', 'K', 'J', 'I', '4', '5', '6', '7' },
  { '0', '1', '2', '3', '4', 'L', 'K', 'J', 'I', '5', '6', '7' },
  { '0', '1', '2', '3', '4', '5', 'L', 'K', 'J', 'I', '6', '7' },
  { '0', '1', '2', '3', '4', '5', '6', 'L', 'K', 'J', 'I', '7' },
  { '0', '1', '2', '3', '4', '5', '6', '7', 'L', 'K', 'J', 'I' }
};

static const c8 reg_lwl_le[4][4] =
{
  { 'L', 'f', 'g', 'h' },
  { 'K', 'L', 'g', 'h' },
  { 'J', 'K', 'L', 'h' },
  { 'I', 'J', 'K', 'L' }
};

static void lwl_le(s32 vAddr)
{
  c8 reg[4] = { 'e', 'f', 'g', 'h' };
  s32 i, j;
  for (j = 0, i = vAddr; i >= 0; j++, i--)
  {
    reg[j] = data_le[0][4 + i];
  }
  printf("vAddr %d: %c %c %c %c\n", vAddr, reg[0], reg[1], reg[2], reg[3]);
}

static const c8 reg_lwr_le[4][4] =
{
  { 'I', 'J', 'K', 'L' },
  { 'e', 'I', 'J', 'K' },
  { 'e', 'f', 'I', 'J' },
  { 'e', 'f', 'g', 'I' }
};

static void lwr_le(s32 vAddr)
{
  c8 reg[4] = { 'e', 'f', 'g', 'h' };
  s32 i, j;
  for (j = vAddr, i = 3; i >= vAddr; j++, i--)
  {
    reg[j] = data_le[0][4 + i];
  }
  printf("vAddr %d: %c %c %c %c\n", vAddr, reg[0], reg[1], reg[2], reg[3]);
}

int main(void)
{
  printf("lwl le\n");
  lwl_le(0);
  lwl_le(1);
  lwl_le(2);
  lwl_le(3);

  printf("lwl be\n");
  lwl_be(0);
  lwl_be(1);
  lwl_be(2);
  lwl_be(3);

  printf("lwr le\n");
  lwr_le(0);
  lwr_le(1);
  lwr_le(2);
  lwr_le(3);

  printf("lwr be\n");
  lwr_be(0);
  lwr_be(1);
  lwr_be(2);
  lwr_be(3);

  return 0;
}