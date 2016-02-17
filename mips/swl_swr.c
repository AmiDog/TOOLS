/*
 * swl_swr
 */

#include "main.h"

static const c8 mem_be[4] =
{
  'i', 'j', 'k', 'l'
};

static const c8 mem_le[4] =
{
  'l', 'k', 'j', 'i'
};

static const c8 reg_be[4] =
{
  'E', 'F', 'G', 'H',
};

static const c8 reg_le[4] =
{
  'E', 'F', 'G', 'H',
};

/* SWL - EffAddr is the address of the most-significant of four consecutive
 * bytes forming a word in memory starting at an arbitrary byte boundary.
 */

static const c8 mem_swl_be[4][4] =
{
  { 'E', 'F', 'G', 'H' },
  { 'i', 'E', 'F', 'G' },
  { 'i', 'j', 'E', 'F' },
  { 'i', 'j', 'k', 'E' }
};

static void swl_be(c8 *mem, s32 vAddr, const c8 *reg)
{
  s32 i, j;
  for (j = 0, i = vAddr; i <= (vAddr | 3); j++, i++)
  {
    mem[i] = reg[j];
  }
}

static const c8 mem_swl_le[4][4] =
{
  { 'E', 'k', 'j', 'i' },
  { 'F', 'E', 'j', 'i' },
  { 'G', 'F', 'E', 'i' },
  { 'H', 'G', 'F', 'E' }
};

static void swl_le(c8 *mem, s32 vAddr, const c8 *reg)
{
  s32 i, j;
  for (j = 0, i = vAddr; i >= (vAddr & ~3); j++, i--)
  {
    mem[i] = reg[j];
  }
}

/* SWR - EffAddr is the address of the least-significant of four consecutive
 * bytes forming a word in memory starting at an arbitrary byte boundary.
 */

static const c8 mem_swr_be[4][4] =
{
  { 'H', 'j', 'k', 'l' },
  { 'G', 'H', 'k', 'l' },
  { 'F', 'G', 'H', 'l' },
  { 'E', 'F', 'G', 'H' }
};

static void swr_be(c8 *mem, s32 vAddr, const c8 *reg)
{
  s32 i, j;
  for (j = 3, i = vAddr; i >= (vAddr & ~3); j--, i--)
  {
    mem[i] = reg[j];
  }
}

static const c8 mem_swr_le[4][4] =
{
  { 'H', 'G', 'F', 'E' },
  { 'l', 'H', 'G', 'F' },
  { 'l', 'k', 'H', 'G' },
  { 'l', 'k', 'j', 'H' }
};

static void swr_le(c8 *mem, s32 vAddr, const c8 *reg)
{
  s32 i, j;
  for (j = 3, i = vAddr; i <= (vAddr | 3); j--, i++)
  {
    mem[i] = reg[j];
  }
}

/* SWR + SWL */

static const c8 data_be[5][12] =
{
  { '0', '1', '2', '3', 'E', 'F', 'G', 'H', '8', '9', 'a', 'b' },
  { '0', '1', '2', '3', '4', 'E', 'F', 'G', 'H', '9', 'a', 'b' },
  { '0', '1', '2', '3', '4', '5', 'E', 'F', 'G', 'H', 'a', 'b' },
  { '0', '1', '2', '3', '4', '5', '6', 'E', 'F', 'G', 'H', 'b' },
  { '0', '1', '2', '3', '4', '5', '6', '7', 'E', 'F', 'G', 'H' }
};

static const c8 data_le[5][12] =
{
  { '0', '1', '2', '3', 'H', 'G', 'F', 'E', '8', '9', 'a', 'b' },
  { '0', '1', '2', '3', '4', 'H', 'G', 'F', 'E', '9', 'a', 'b' },
  { '0', '1', '2', '3', '4', '5', 'H', 'G', 'F', 'E', 'a', 'b' },
  { '0', '1', '2', '3', '4', '5', '6', 'H', 'G', 'F', 'E', 'b' },
  { '0', '1', '2', '3', '4', '5', '6', '7', 'H', 'G', 'F', 'E' }
};

static void swr_swl_be(c8 *reg, s32 EffAddr, const c8 *mem)
{
  swr_be(reg, 3 + EffAddr, mem);
  swl_be(reg, 0 + EffAddr, mem);
}

static void swr_swl_le(c8 *reg, s32 EffAddr, const c8 *mem)
{
  swr_le(reg, 0 + EffAddr, mem);
  swl_le(reg, 3 + EffAddr, mem);
}

/* TEST */

typedef void (*op_t)(c8 *mem, s32 vAddr, const c8 *reg);

static void do_op(const c8 *name, op_t op, const c8 *wanted, s32 vAddr, const c8 *reg, const c8 *mem)
{
  c8 m[4] = { mem[0], mem[1], mem[2], mem[3] };
  s32 i;
  op(m, vAddr, reg);
  printf("%s vAddr %d: %c %c %c %c\n", name, vAddr, m[0], m[1], m[2], m[3]);
  for (i = 0; i < 4; i++)
  {
    if (m[i] != wanted[i])
    {
      printf("error @ %d\n", i);
    }
  }
}

#define DO_OP(_o, _e)                                                                     \
  do_op(#_o "_" #_e, _o ## _ ## _e, mem_ ## _o ## _ ## _e[0], 0, reg_ ## _e, mem_ ## _e); \
  do_op(#_o "_" #_e, _o ## _ ## _e, mem_ ## _o ## _ ## _e[1], 1, reg_ ## _e, mem_ ## _e); \
  do_op(#_o "_" #_e, _o ## _ ## _e, mem_ ## _o ## _ ## _e[2], 2, reg_ ## _e, mem_ ## _e); \
  do_op(#_o "_" #_e, _o ## _ ## _e, mem_ ## _o ## _ ## _e[3], 3, reg_ ## _e, mem_ ## _e); \
  printf("\n");

typedef void (*op_rl_t)(c8 *reg, s32 EffAddr, const c8 *mem);

static void do_rl_op(const c8 *name, op_rl_t op, s32 EffAddr, const c8 *wanted)
{
  const c8 reg[4] = { 'E', 'F', 'G', 'H' };
  c8 mem[12] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b' };
  s32 i;
  op(mem, EffAddr, reg);
  printf("%s EffAddr %d: %c %c %c %c %c %c %c %c %c %c %c %c\n", name, EffAddr, mem[0], mem[1], mem[2], mem[3], mem[4], mem[5], mem[6], mem[7], mem[8], mem[9], mem[10], mem[11]);
  for (i = 0; i < 12; i++)
  {
    if (mem[i] != wanted[i])
    {
      printf("error @ %d\n", i);
    }
  }
}

void test_swl_swr(void)
{
  s32 i;

  DO_OP(swl, le);
  DO_OP(swl, be);
  DO_OP(swr, le);
  DO_OP(swr, be);

  for (i = 0; i < 5; i++)
  {
    do_rl_op("swr_swl_be", swr_swl_be, 4 + i, data_be[i]);
    do_rl_op("swr_swl_le", swr_swl_le, 4 + i, data_le[i]);
  }
  printf("\n");
}
