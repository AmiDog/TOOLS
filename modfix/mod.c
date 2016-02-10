/*
 * mod.c
 */

#include "mod.h"

static mod_t main_mod;

static s32 get_word(u8 *ptr)
{
  return (ptr[0] << 8) | ptr[1];
}

static void dump_sample(s32 entry, mod_sample_t *sample)
{
  printf("%2d: '%22s' length %d words, finetune %d, volumne %d, repeat offset %d, repeat length %d words\n",
    entry,
    sample->name,
    get_word(sample->length_in_words),
    ((s8)(sample->finetune[0] << 4)) >> 4,
    sample->volume[0],
    get_word(sample->repeat_offset),
    get_word(sample->repeat_length));
}

static void mod_save(const c8 *filename, mod_t * mod)
{
  (void)filename;
  (void)mod;
}

static void remove_loops(s32 sample_count)
{
  s32 i;
  for (i = 0; i < sample_count; i++)
  {
    s32 repeat_length = get_word(main_mod.mod_data.sample[i].repeat_length);
    if (repeat_length > 1)
    {
      printf("removing loop for sample %d\n", i);
    }
  }
}

void mod_load(const c8 *filename)
{
  FILE *fp = fopen(filename, "rb");
  if (fp != NULL)
  {
    s32 i;

    memset(&main_mod, 0, sizeof(main_mod));

    printf("read %d bytes\n", fread(&main_mod.mod_data, 1, sizeof(mod_new_t), fp));
    printf("%c%c%c%c\n", main_mod.mod_data.type[0], main_mod.mod_data.type[1], main_mod.mod_data.type[2], main_mod.mod_data.type[3]);

    s32 sample_count = SAMPLES_OLD;
    if ((main_mod.mod_data.type[0] == 'M') && (main_mod.mod_data.type[1] == '.') && (main_mod.mod_data.type[2] == 'K') && (main_mod.mod_data.type[3] == '.'))
    {
      sample_count = SAMPLES_NEW;
    }
    else
    {
      printf("old type not supported\n");
      return;
    }

    printf("%s\n", main_mod.mod_data.title);
    for (i = 0; i < sample_count; i++)
    {
      dump_sample(i, &main_mod.mod_data.sample[i]);
    }
    printf("song positions: %d\n", main_mod.mod_data.song_positions[0]);
    printf("always 127: %d\n", main_mod.mod_data.always_127[0]);
    s32 pattern_count = 0;
    for (i = 0; i < main_mod.mod_data.song_positions[0]; i++)
    {
      if (main_mod.mod_data.pattern_table[i] >= pattern_count)
      {
        pattern_count = main_mod.mod_data.pattern_table[i] + 1;
      }
      printf("%2d: pattern %d\n", i, main_mod.mod_data.pattern_table[i]);
    }
    printf("pattern count: %d\n", pattern_count);

    fread(&main_mod.patterns[0], 1, pattern_count * sizeof(mod_pattern_data_t), fp);

    printf("at file position %d\n", (s32)ftell(fp));
    for (i = 0; i < sample_count; i++)
    {
      s32 sample_bytes = get_word(main_mod.mod_data.sample[i].length_in_words) * 2;
      if (sample_bytes > 0)
      {
        if ((s32)fread(&main_mod.samples[i], 1, sample_bytes, fp) != sample_bytes)
        {
          printf("read error\n");
          return;
        }
        printf("at file position %d\n", (s32)ftell(fp));
      }
    }

    fclose(fp);

    remove_loops(sample_count);
    mod_save("../../test.mod", &main_mod);
  }
}
