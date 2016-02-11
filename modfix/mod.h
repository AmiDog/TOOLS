/*
 * mod.h
 */

#ifndef _MOD_H_
#define _MOD_H_

#include "types.h"

typedef union mod_channel_data_u mod_channel_data_t;
typedef struct mod_division_data_s mod_division_data_t;
typedef struct mod_pattern_data_s mod_pattern_data_t;

typedef struct mod_new_s mod_new_t;
typedef struct mod_old_s mod_old_t;
typedef struct mod_sample_s mod_sample_t;
typedef struct mod_s mod_t;

#define SAMPLES_OLD (15)
#define SAMPLES_NEW (31)

union mod_channel_data_u
{
  struct
  {
#if 0
    u32 sample_hi :  4;
    u32 period    : 12;
    u32 sample_lo :  4;
    u32 effect    : 12;
#else
    u32 effect    : 12;
    u32 sample_lo :  4;
    u32 period    : 12;
    u32 sample_hi :  4;
#endif
  } n;
  u32 data;
};

struct mod_division_data_s
{
  mod_channel_data_t channel[4];
};

struct mod_pattern_data_s
{
  mod_division_data_t division[64];
};

struct mod_sample_s
{
  c8 name[22];
  u8 length_in_words[2];
  u8 finetune[1];
  u8 volume[1];
  u8 repeat_offset[2];
  u8 repeat_length[2];
};

struct mod_new_s
{
  c8 title[20];
  mod_sample_t sample[SAMPLES_NEW];
  u8 song_positions[1];
  u8 always_127[1];
  u8 pattern_table[128];
  c8 type[4];
};

struct mod_old_s
{
  c8 title[20];
  mod_sample_t sample[SAMPLES_OLD];
  u8 song_positions[1];
  u8 always_127[1];
  u8 pattern_table[128];
};

struct mod_s
{
  mod_new_t mod_data;
  mod_pattern_data_t patterns[128];
  s8 samples[32][2 * 65536];
  s32 sample_count;
  s32 pattern_count;
};

extern void mod_setup(void);
extern void mod_load(const c8 *filename);
extern void mod_process(void);
extern void mod_save(const c8 *filename);

#endif /* _MOD_H_ */
