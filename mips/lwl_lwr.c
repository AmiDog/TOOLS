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

static const c8 reg_be[4][4] =
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
  for (j = 0, i = vAddr; i < 4; j++, i++)
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

static const c8 reg_le[4][4] =
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

int main(void)
{
  printf("le\n");
  lwl_le(0);
  lwl_le(1);
  lwl_le(2);
  lwl_le(3);

  printf("be\n");
  lwl_be(0);
  lwl_be(1);
  lwl_be(2);
  lwl_be(3);

  return 0;
}
