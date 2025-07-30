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
  523.25  /*  DO' */
};

int
play(const float freq, float duration)
{
  /* INIT */
  snd_pcm_t *handle;
  float buf[(int)(SAMPLE_RATE * duration)];
  int err, frames, i;

  /* SETTING A SOUND */
  assert((err = snd_pcm_open(&handle, "default", SND_PCM_STREAM_PLAYBACK, 0)) >= 0); /* OPEN THE PCM DEVICE BY DEFAULT FOR PLAYING */
  assert((err = snd_pcm_set_params(handle,
          SND_PCM_FORMAT_FLOAT,
          SND_PCM_ACCESS_RW_INTERLEAVED,
          1,              /* MONO CHANNEL */
          SAMPLE_RATE,
          1,              /* TURN ON FLOAT */
          150000)) >= 0); /* COLDOWN(0.15 sec) */

  /* MAKE SINUSOID */
  frames = (int)(SAMPLE_RATE * duration);
  for (i = 0; frames - i;)
    buf[i] = sinf((2.0f * PI * freq * i++) / SAMPLE_RATE);
  assert((err = snd_pcm_writei(handle, buf, frames)) >= 0); /* LOADING AUDIO */

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
    puts(n); 
    play(FREQS[(int)(*n - '0')], ((rand() % 5) + 3) / 10.0f);
  }

  return 0;
}

