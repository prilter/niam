#include <alsa/asoundlib.h>
#include <math.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SAMPLE_RATE 44100
#define PI          3.141593

const float FREQS[] = {
  261.63, /*  DO  */
  293.66, /*  RE  */
  329.63, /*  MI  */
  349.23, /*  FA  */
  392.00, /*  SOL */
  440.00, /*  LA  */
  493.88, /*  SI  */
  261.63, /*  DO  */
  293.66, /*  RE  */
  329.63, /*  MI  */
};

int
play(const float freq, float duration)
{
  snd_pcm_t *handle;
  float      buf[(int)(SAMPLE_RATE * duration) * sizeof(float)], t, envelope, sample;
  int        frames, i;

  /* OPEN PCM DEVICE */
  assert(snd_pcm_open(&handle, "default", SND_PCM_STREAM_PLAYBACK, 0) >= 0);

  /* SETTINGS */
  assert(snd_pcm_set_params(handle,
    SND_PCM_FORMAT_FLOAT,
    SND_PCM_ACCESS_RW_INTERLEAVED,
    1, /* MONO */
    SAMPLE_RATE,
    1,
    150000) >= 0); /* COLDOWN */

  /* ADSR parms */
  #define attack   0.03f /* 30 ms */
  #define release  0.08f /* 80 ms */

  frames = (int)(SAMPLE_RATE * duration);
  for (i = 0; frames-i;) {
    t = (float)i / SAMPLE_RATE;
    if (t < attack)
      envelope = t / attack;
    else if (t > duration - release)
      envelope = (duration - t) / release;

    sample = envelope * (                      \
      sinf(2.0f * PI * freq * t) +                 \
      0.5f * sinf(2.0f * PI * (2.0f * freq) * t) + \
      0.2f * sinf(2.0f * PI * (3.0f * freq) * t)   \
    );

    /* WHITE NOICE */
    sample += envelope * (((float)rand() / RAND_MAX) * 0.02f - 0.01f);
    buf[i++] = (sample > 1.0f) ? 1.0f:((sample < -1.0f) ? -1.0f:sample); /* sample - [-1;1] */
  }

  /* USE ALSA */
  assert(snd_pcm_writei(handle, buf, frames) >= 0);

  snd_pcm_drain(handle);
  snd_pcm_close(handle);

  return 0;
}

int
main(int argc, const char **argv)
{
  if (argc == 1)
    return 1;

  const char *n = *(argv+1);

  srand(time(NULL));
  for (;*n; n++) {
    if (*n == '.' || *n == ' ') 
      continue;
    printf("%c(%.2f) - %s\n", *n, FREQS[(int)(*n - '0')], n);
    play(FREQS[(int)(*n - '0')], ((rand() % 48) + 35) / 100.0f);
  }

  return 0;
}

