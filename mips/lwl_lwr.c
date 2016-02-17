/*
 * lwl_lwr
 */

#include "main.h"

static const c8 mem_be[4] =
{
  'I', 'J', 'K', 'L'
};

static const c8 mem_le[4] =
{
  'L', 'K', 'J', 'I'
};

/* LWL - EffAddr is the address of the most-significant of four consecutive
 * bytes forming a word in memory starting at an arbitrary byte boundary.
 */

static const c8 reg_lwl_be[4][4] =
{
  { 'I', 'J', 'K', 'L' },
  { 'J', 'K', 'L', 'h' },
  { 'K', 'L', 'g', 'h' },
  { 'L', 'f', 'g', 'h' }
};

static void lwl_be(c8 *reg, s32 vAddr, const c8 *mem)
{
  s32 i, j;
  for (j = 0, i = vAddr; i <= 3; j++, i++)
  {
    reg[j] = mem[i];
  }
}

static const c8 reg_lwl_le[4][4] =
{
  { 'L', 'f', 'g', 'h' },
  { 'K', 'L', 'g', 'h' },
  { 'J', 'K', 'L', 'h' },
  { 'I', 'J', 'K', 'L' }
};

static void lwl_le(c8 *reg, s32 vAddr, const c8 *mem)
{
  s32 i, j;
  for (j = 0, i = vAddr; i >= 0; j++, i--)
  {
    reg[j] = mem[i];
  }
}

/* LWR - EffAddr is the address of the least-significant of four consecutive
 * bytes forming a word in memory starting at an arbitrary byte boundary.
 */

static const c8 reg_lwr_be[4][4] =
{
  { 'e', 'f', 'g', 'I' },
  { 'e', 'f', 'I', 'J' },
  { 'e', 'I', 'J', 'K' },
  { 'I', 'J', 'K', 'L' }
};

static void lwr_be(c8 *reg, s32 vAddr, const c8 *mem)
{
  s32 i, j;
  for (j = (3 - vAddr), i = 0; i <= vAddr; j++, i++)
  {
    reg[j] = mem[i];
  }
}

static const c8 reg_lwr_le[4][4] =
{
  { 'I', 'J', 'K', 'L' },
  { 'e', 'I', 'J', 'K' },
  { 'e', 'f', 'I', 'J' },
  { 'e', 'f', 'g', 'I' }
};

static void lwr_le(c8 *reg, s32 vAddr, const c8 *mem)
{
  s32 i, j;
  for (j = vAddr, i = 3; i >= vAddr; j++, i--)
  {
    reg[j] = mem[i];
  }
}

/* TEST */

typedef void (*op_t)(c8 *reg, s32 vAddr, const c8 *mem);

static void do_op(const c8 *name, op_t op, const c8 *wanted, s32 vAddr, const c8 *mem)
{
  c8 reg[4] = { 'e', 'f', 'g', 'h' };
  s32 i;
  op(reg, vAddr, mem);
  printf("%s vAddr %d: %c %c %c %c\n", name, vAddr, reg[0], reg[1], reg[2], reg[3]);
  for (i = 0; i < 4; i++)
  {
    if (reg[i] != wanted[i])
    {
      printf("error @ %d\n", i);
    }
  }
}

#define DO_OP(_o, _e)                                                         \
  do_op(#_o "_" #_e, _o ## _ ## _e, reg_ ## _o ## _ ## _e[0], 0, mem_ ## _e); \
  do_op(#_o "_" #_e, _o ## _ ## _e, reg_ ## _o ## _ ## _e[1], 1, mem_ ## _e); \
  do_op(#_o "_" #_e, _o ## _ ## _e, reg_ ## _o ## _ ## _e[2], 2, mem_ ## _e); \
  do_op(#_o "_" #_e, _o ## _ ## _e, reg_ ## _o ## _ ## _e[3], 3, mem_ ## _e); \
  printf("\n");

void test_lwl_lwr(void)
{
  DO_OP(lwl, le);
  DO_OP(lwl, be);
  DO_OP(lwr, le);
  DO_OP(lwr, be);
}
