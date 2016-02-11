/*
 * mod.c
 */

#include "mod.h"

static c8 periods[4096][4];

static const struct
{
  s16 period;
  const c8 *name;
} octaves[] = {
  { 1712, "C-0" }, { 1616, "C#0" }, { 1525, "D-0" }, { 1440, "D#0" }, { 1357, "E-0" }, { 1281, "F-0" }, { 1209, "F#0" }, { 1141, "G-0" }, { 1077, "G#0" }, { 1017, "A-0" }, {  961, "A#0" }, {  907, "B-0" },
  {  856, "C-1" }, {  808, "C#1" }, {  762, "D-1" }, {  720, "D#1" }, {  678, "E-1" }, {  640, "F-1" }, {  604, "F#1" }, {  570, "G-1" }, {  538, "G#1" }, {  508, "A-1" }, {  480, "A#1" }, {  453, "B-1" },
  {  428, "C-2" }, {  404, "C#2" }, {  381, "D-2" }, {  360, "D#2" }, {  339, "E-2" }, {  320, "F-2" }, {  302, "F#2" }, {  285, "G-2" }, {  269, "G#2" }, {  254, "A-2" }, {  240, "A#2" }, {  226, "B-2" },
  {  214, "C-3" }, {  202, "C#3" }, {  190, "D-3" }, {  180, "D#3" }, {  170, "E-3" }, {  160, "F-3" }, {  151, "F#3" }, {  143, "G-3" }, {  135, "G#3" }, {  127, "A-3" }, {  120, "A#3" }, {  113, "B-3" },
  {  107, "C-4" }, {  101, "C#4" }, {   95, "D-4" }, {   90, "D#4" }, {   85, "E-4" }, {   80, "F-4" }, {   76, "F#4" }, {   71, "G-4" }, {   67, "G#4" }, {   64, "A-4" }, {   60, "A#4" }, {   57, "B-4" },
  {   -1, NULL }
};

static const c8 hex[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

void mod_setup(void)
{
  s32 i;
  memset(periods, 0, sizeof(periods));
  for (i = 1; i < 4096; i++)
  {
    strncpy(periods[i], "???", 3);
  }
  for (i = 0; octaves[i].name != NULL; i++)
  {
    strncpy(periods[octaves[i].period], octaves[i].name, 3);
  }
}

static mod_t main_mod;

static u32 get_long(u8 *ptr)
{
  return (ptr[0] << 24) | (ptr[1] << 16) | (ptr[2] << 8) | (ptr[3] << 0);
}

static u32 get_word(u8 *ptr)
{
  return (ptr[0] << 8) | (ptr[1] << 0);
}

static void set_word(u8 *ptr, u32 value)
{
  ptr[0] = (value >> 8) & 0xff;
  ptr[1] = (value >> 0) & 0xff;
}

static void dump_channel(mod_channel_data_t *channel)
{
  mod_channel_data_t chn;
  chn.data = get_long((u8 *)&channel->data);
  s32 sample = (chn.n.sample_hi << 4) | (chn.n.sample_lo << 0);
  printf("| [%-3s] %2d [%c%c%c] |", periods[chn.n.period], sample,
    hex[(chn.n.effect >> 8) & 15], hex[(chn.n.effect >> 4) & 15], hex[(chn.n.effect >> 0) & 15]);
}

static void dump_division(mod_division_data_t *division)
{
  s32 i;
  for (i = 0; i < 4; i++)
  {
    dump_channel(&division->channel[i]);
  }
}

static void dump_pattern(mod_pattern_data_t *pattern)
{
  s32 i;
  for (i = 0; i < 64; i++)
  {
    printf("%2d: ", i);
    dump_division(&pattern->division[i]);
    printf("\n");
  }
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

static s32 append_loop(s32 entry, s32 repeat_length, s32 repeat_offset, s32 length_in_words)
{
  s32 i;
  for (i = 0; i < repeat_length; i++)
  {
    main_mod.samples[entry][(length_in_words * 2) + 0] = main_mod.samples[entry][(repeat_offset * 2) + (i * 2) + 0];
    main_mod.samples[entry][(length_in_words * 2) + 1] = main_mod.samples[entry][(repeat_offset * 2) + (i * 2) + 1];
    length_in_words++;
  }
  return length_in_words;
}

static s32 fade_loop(s32 entry, s32 repeat_length, s32 repeat_offset, s32 length_in_words, s32 fade_length)
{
  s32 i;
#if 0
  for (i = -50; i < 0; i++)
  {
    printf("end %d\n", main_mod.samples[entry][(length_in_words * 2) + i]);
  }
  for (i = -50; i < 0; i++)
  {
    printf("xxx %d\n", main_mod.samples[entry][((repeat_offset + repeat_length) * 2) + i]);
  }
  for (i = 0; i < 50; i++)
  {
    printf("rep %d\n", main_mod.samples[entry][(repeat_offset * 2) + i]);
  }
#endif
#if 0
  s32 j;
  for (i = 0, j = 0; i < fade_length; i++)
  {
    main_mod.samples[entry][(length_in_words * 2) + 0] = (main_mod.samples[entry][(repeat_offset * 2) + (j * 2) + 0] * (fade_length - i)) / fade_length;
    main_mod.samples[entry][(length_in_words * 2) + 1] = (main_mod.samples[entry][(repeat_offset * 2) + (j * 2) + 1] * (fade_length - i)) / fade_length;
    j++;
    if (j >= repeat_length)
    {
      j = 0;
    }
    //printf("fade: %d, %d, %d: %d: %d, %d\n", max, i, (max - i), ((100 * (max - i)) / max), main_mod.samples[entry][(length_in_words * 2) + 0], main_mod.samples[entry][(length_in_words * 2) + 1]);
    length_in_words++;
  }
#else
  for (i = 0; i < repeat_length; i++)
  {
    main_mod.samples[entry][(length_in_words * 2) + 0] = (main_mod.samples[entry][(repeat_offset * 2) + (i * 2) + 0] * (repeat_length - i)) / repeat_length;
    main_mod.samples[entry][(length_in_words * 2) + 1] = (main_mod.samples[entry][(repeat_offset * 2) + (i * 2) + 1] * (repeat_length - i)) / repeat_length;
    //printf("fade: %d, %d, %d: %d: %d, %d\n", max, i, (max - i), ((100 * (max - i)) / max), main_mod.samples[entry][(length_in_words * 2) + 0], main_mod.samples[entry][(length_in_words * 2) + 1]);
    length_in_words++;
  }
#endif
  return length_in_words;
  (void)fade_length;
}

static void remove_loops(void)
{
  s32 i;
  for (i = 0; i < main_mod.sample_count; i++)
  {
    s32 repeat_length = get_word(main_mod.mod_data.sample[i].repeat_length);
    s32 repeat_offset = get_word(main_mod.mod_data.sample[i].repeat_offset);
    s32 length_in_words = get_word(main_mod.mod_data.sample[i].length_in_words);
    if (repeat_length > 1)
    {
      printf("removing loop for sample %d, repeat length: %d, repeat offset: %d, sample length %d\n", i, repeat_length, repeat_offset, length_in_words);
      set_word(main_mod.mod_data.sample[i].repeat_length, 1);
      set_word(main_mod.mod_data.sample[i].repeat_offset, 0);

      // docs says to play full sample once, but that causes clicks (and makes sample look weird). this fixes it.does fix those clicks.
      length_in_words = (repeat_offset + repeat_length);
#if 1
      while (length_in_words < 8192)
      {
        length_in_words = append_loop(i, repeat_length, repeat_offset, length_in_words);
      }
#endif

#if 1
      length_in_words = fade_loop(i, repeat_length, repeat_offset, length_in_words, 4096);
#endif

      set_word(main_mod.mod_data.sample[i].length_in_words, length_in_words);
    }
  }
  (void)append_loop;
  (void)fade_loop;
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
#if 1
    dump_pattern(&main_mod.patterns[0]);
#endif

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
