/*
 * swl_swr
 */

#include "main.h"

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

static void swl_be(c8 *mem, s32 vAddr)
{
  s32 i, j;
  for (j = vAddr, i = 0; i <= (3 - vAddr); j++, i++)
  {
    mem[j] = reg_be[i];
  }
}

static const c8 mem_swl_le[4][4] =
{
  { 'i', 'j', 'k', 'E' },
  { 'i', 'j', 'E', 'F' },
  { 'i', 'E', 'F', 'G' },
  { 'E', 'F', 'G', 'H' }
};

static void swl_le(c8 *mem, s32 vAddr)
{
  s32 i, j;
  for (j = (3 - vAddr), i = 0; i <= vAddr; j++, i++)
  {
    mem[j] = reg_be[i];
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

static void swr_be(c8 *mem, s32 vAddr)
{
  s32 i, j;
  for (j = 0, i = (3 - vAddr); i <= 3; j++, i++)
  {
    mem[j] = reg_be[i];
  }
}

static const c8 mem_swr_le[4][4] =
{
  { 'E', 'F', 'G', 'H' },
  { 'F', 'G', 'H', 'l' },
  { 'G', 'H', 'k', 'l' },
  { 'H', 'j', 'k', 'l' }
};

static void swr_le(c8 *mem, s32 vAddr)
{
  s32 i, j;
  for (j = 0, i = vAddr; i <= 3; j++, i++)
  {
    mem[j] = reg_be[i];
  }
}

/* TEST */

typedef void (*op_t)(c8 *mem, s32 vAddr);

static void do_op(const c8 *name, op_t op, const c8 *wanted, s32 vAddr)
{
  c8 mem[4] = { 'i', 'j', 'k', 'l' };
  s32 i;
  op(mem, vAddr);
  printf("%s vAddr %d: %c %c %c %c\n", name, vAddr, mem[0], mem[1], mem[2], mem[3]);
  for (i = 0; i < 4; i++)
  {
    if (mem[i] != wanted[i])
    {
      printf("error @ %d\n", i);
    }
  }
}

#define DO_OP(_n)                  \
  do_op(#_n, _n, mem_ ## _n[0], 0); \
  do_op(#_n, _n, mem_ ## _n[1], 1); \
  do_op(#_n, _n, mem_ ## _n[2], 2); \
  do_op(#_n, _n, mem_ ## _n[3], 3); \
  printf("\n");

void test_swl_swr(void)
{
  DO_OP(swl_le);
  DO_OP(swl_be);
  DO_OP(swr_le);
  DO_OP(swr_be);
}
