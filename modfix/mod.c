/*
 * mod.c
 */

#include "mod.h"

static mod_t main_mod;

static s32 get_word(u8 *ptr)
{
  return (ptr[0] << 8) | (ptr[1] << 0);
}

static void set_word(u8 *ptr, s32 value)
{
  ptr[0] = (value >> 8) & 0xff;
  ptr[1] = (value >> 0) & 0xff;
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

void mod_save(const c8 *filename)
{
  FILE *fp = fopen(filename, "wb");
  if (fp != NULL)
  {
    s32 i;

    printf("write %d bytes\n", fwrite(&main_mod.mod_data, 1, sizeof(mod_new_t), fp));

    fwrite(&main_mod.patterns[0], 1, main_mod.pattern_count * sizeof(mod_pattern_data_t), fp);

    for (i = 0; i < main_mod.sample_count; i++)
    {
      s32 sample_bytes = get_word(main_mod.mod_data.sample[i].length_in_words) * 2;
      if (sample_bytes > 0)
      {
        if ((s32)fwrite(&main_mod.samples[i], 1, sample_bytes, fp) != sample_bytes)
        {
          printf("write error\n");
        }
      }
    }

    fclose(fp);
  }
}

static void remove_loops(void)
{
  s32 i;
  for (i = 0; i < main_mod.sample_count; i++)
  {
    s32 repeat_length = get_word(main_mod.mod_data.sample[i].repeat_length);
    s32 repeat_offset = get_word(main_mod.mod_data.sample[i].repeat_offset);
    if (repeat_length > 1)
    {
      printf("removing loop for sample %d, length: %d, offset: %d\n", i, repeat_length, repeat_offset);
      set_word(main_mod.mod_data.sample[i].repeat_length, 1);
      set_word(main_mod.mod_data.sample[i].repeat_offset, 0);
    }
  }
}

void mod_process(void)
{
  remove_loops();
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

    main_mod.sample_count = SAMPLES_OLD;
    if ((main_mod.mod_data.type[0] == 'M') && (main_mod.mod_data.type[1] == '.') && (main_mod.mod_data.type[2] == 'K') && (main_mod.mod_data.type[3] == '.'))
    {
      main_mod.sample_count = SAMPLES_NEW;
    }
    else
    {
      printf("old type not supported\n");
      return;
    }

    printf("%s\n", main_mod.mod_data.title);
    for (i = 0; i < main_mod.sample_count; i++)
    {
      dump_sample(i, &main_mod.mod_data.sample[i]);
    }
    printf("song positions: %d\n", main_mod.mod_data.song_positions[0]);
    printf("always 127: %d\n", main_mod.mod_data.always_127[0]);

    main_mod.pattern_count = 0;
    for (i = 0; i < main_mod.mod_data.song_positions[0]; i++)
    {
      if (main_mod.mod_data.pattern_table[i] >= main_mod.pattern_count)
      {
        main_mod.pattern_count = main_mod.mod_data.pattern_table[i] + 1;
      }
      printf("%2d: pattern %d\n", i, main_mod.mod_data.pattern_table[i]);
    }
    printf("pattern count: %d\n", main_mod.pattern_count);

    fread(&main_mod.patterns[0], 1, main_mod.pattern_count * sizeof(mod_pattern_data_t), fp);

    printf("at file position %d\n", (s32)ftell(fp));
    for (i = 0; i < main_mod.sample_count; i++)
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
  }
}
