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

static const c8 reg_be[4] =
{
  'e', 'f', 'g', 'h'
};

static const c8 reg_le[4] =
{
  'e', 'f', 'g', 'h'
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
  for (j = 0, i = vAddr; i <= (vAddr | 3); j++, i++)
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
  for (j = 0, i = vAddr; i >= (vAddr & ~3); j++, i--)
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
  for (j = 3, i = vAddr; i >= (vAddr & ~3); j--, i--)
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
  for (j = 3, i = vAddr; i <= (vAddr | 3); j--, i++)
  {
    reg[j] = mem[i];
  }
}

/* LWR + LWL */

static const c8 data_be[5][12] =
{
  { '0', '1', '2', '3', 'I', 'J', 'K', 'L', '8', '9', 'a', 'b' },
  { '0', '1', '2', '3', '4', 'I', 'J', 'K', 'L', '9', 'a', 'b' },
  { '0', '1', '2', '3', '4', '5', 'I', 'J', 'K', 'L', 'a', 'b' },
  { '0', '1', '2', '3', '4', '5', '6', 'I', 'J', 'K', 'L', 'b' },
  { '0', '1', '2', '3', '4', '5', '6', '7', 'I', 'J', 'K', 'L' }
};

static const c8 data_le[5][12] =
{
  { '0', '1', '2', '3', 'L', 'K', 'J', 'I', '8', '9', 'a', 'b' },
  { '0', '1', '2', '3', '4', 'L', 'K', 'J', 'I', '9', 'a', 'b' },
  { '0', '1', '2', '3', '4', '5', 'L', 'K', 'J', 'I', 'a', 'b' },
  { '0', '1', '2', '3', '4', '5', '6', 'L', 'K', 'J', 'I', 'b' },
  { '0', '1', '2', '3', '4', '5', '6', '7', 'L', 'K', 'J', 'I' }
};

static void lwr_lwl_be(c8 *reg, s32 EffAddr, const c8 *mem)
{
  lwr_be(reg, 3 + EffAddr, mem);
  lwl_be(reg, 0 + EffAddr, mem);
}

static void lwr_lwl_le(c8 *reg, s32 EffAddr, const c8 *mem)
{
  lwr_le(reg, 0 + EffAddr, mem);
  lwl_le(reg, 3 + EffAddr, mem);
}

/* TEST */

typedef void (*op_t)(c8 *reg, s32 vAddr, const c8 *mem);

static void do_op(const c8 *name, op_t op, const c8 *wanted, s32 vAddr, const c8 *mem, const c8 *reg)
{
  c8 r[4] = { reg[0], reg[1], reg[2], reg[3] };
  s32 i;
  op(r, vAddr, mem);
  printf("%s vAddr %d: %c %c %c %c\n", name, vAddr, r[0], r[1], r[2], r[3]);
  for (i = 0; i < 4; i++)
  {
    if (r[i] != wanted[i])
    {
      printf("error @ %d\n", i);
    }
  }
}

#define DO_OP(_o, _e)                                                                     \
  do_op(#_o "_" #_e, _o ## _ ## _e, reg_ ## _o ## _ ## _e[0], 0, mem_ ## _e, reg_ ## _e); \
  do_op(#_o "_" #_e, _o ## _ ## _e, reg_ ## _o ## _ ## _e[1], 1, mem_ ## _e, reg_ ## _e); \
  do_op(#_o "_" #_e, _o ## _ ## _e, reg_ ## _o ## _ ## _e[2], 2, mem_ ## _e, reg_ ## _e); \
  do_op(#_o "_" #_e, _o ## _ ## _e, reg_ ## _o ## _ ## _e[3], 3, mem_ ## _e, reg_ ## _e); \
  printf("\n");

typedef void (*op_rl_t)(c8 *reg, s32 EffAddr, const c8 *mem);

static void do_rl_op(const c8 *name, op_rl_t op, s32 EffAddr, const c8 *mem, const c8 *reg)
{
  const c8 wanted[4] = { 'I', 'J', 'K', 'L' };
  c8 r[4] = { reg[0], reg[1], reg[2], reg[3] };
  s32 i;
  op(r, EffAddr, mem);
  printf("%s EffAddr %d: %c %c %c %c\n", name, EffAddr, r[0], r[1], r[2], r[3]);
  for (i = 0; i < 4; i++)
  {
    if (r[i] != wanted[i])
    {
      printf("error @ %d\n", i);
    }
  }
}

void test_lwl_lwr(void)
{
  s32 i;

  DO_OP(lwl, le);
  DO_OP(lwl, be);
  DO_OP(lwr, le);
  DO_OP(lwr, be);

  for (i = 0; i < 5; i++)
  {
    do_rl_op("lwr_lwl_be", lwr_lwl_be, 4 + i, data_be[i], reg_be);
    do_rl_op("lwr_lwl_le", lwr_lwl_le, 4 + i, data_le[i], reg_le);
  }
  printf("\n");
}
