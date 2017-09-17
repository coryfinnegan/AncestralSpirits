#include "stdafx.h"
#include "SDL.h" 
#include "SDL_audio.h"
#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include "GameManager.h"

#define NUM_SOUNDS 2
struct sample {
    Uint8 *data;
    Uint32 dpos;
    Uint32 dlen;
} sounds[NUM_SOUNDS];
bool soundDone = 0;

    int index;
    SDL_AudioSpec wave;
    Uint8 *wavedata;
    Uint32 wavedlen;
	Uint32 dummy[20];
    SDL_AudioCVT *cvt;
	Uint8 *hackarama;
	int numSounds = 0;

void mixaudio(void *unused, Uint8 *stream, int len)
{
    int i;
    Uint32 amount;
	Uint8 dummy[8] = {0,0,0,0,0,0,0,0};

	    if (len == 0 )
		{
				return;
		}

		
/* Empty any old data in the buffer */
    SDL_memset(stream, 0, len);


    for ( i=0; i<NUM_SOUNDS; ++i ) {
 //   for ( i=0; i<1; ++i ) {
        amount = (sounds[i].dlen-sounds[i].dpos);
        if ( amount > len ) {
            amount = len;
        }
		if (amount == 0)
		{
			soundDone = 1;
			SDL_MixAudio(stream, dummy, 1, SDL_MIX_MAXVOLUME);
		}
		else
		{
		SDL_MixAudio(stream, &sounds[i].data[sounds[i].dpos], amount, SDL_MIX_MAXVOLUME);
		sounds[i].dpos += amount;
		}
   }
}

int LoadSound(char *file, GameManager* pGM)
{

	SDL_AudioSpec obtained = pGM->getAudioSpec();

	index = numSounds++;

    /* Look for an empty (or finished) sound slot 
    for ( index=0; index<NUM_SOUNDS; ++index ) {
        if ( sounds[index].dpos == sounds[index].dlen ) {
            break;
        }
    }
	*/

    if ( index == NUM_SOUNDS )
        return 0;

    /* Load the sound file and convert it to 16-bit stereo at 22kHz */
    if (( SDL_LoadWAV(file, &wave, &wavedata, &wavedlen) == NULL ) || (wavedata == NULL)) {
		 printf("SDL_LoadWAV failed: %s\n", SDL_GetError());
        printf( "Couldn't load %s: %s\n", file, SDL_GetError());
        return 0;
    }


	hackarama = (Uint8*) malloc(sizeof(SDL_AudioCVT) + 20);
	cvt = (SDL_AudioCVT*) hackarama;  // Avoid memory corruption caused by SDL_BuildAudioCVT.  Wheeee!!!

    int ret = SDL_BuildAudioCVT(cvt, wave.format, wave.channels, wave.freq,
			obtained.format, obtained.channels, obtained.freq);

	/*
	cvt.buf = NULL;
	cvt.dst_format = 32784;
	cvt.src_format = 32784;
	cvt.filters[0] = NULL;
	cvt.filter_index = 0;
	cvt.len = 0;
	cvt.len_mult = 1;
	cvt.len_ratio = 1.0;
	cvt.needed = 0;
	cvt.rate_incr = 1.0;
	*/

      //                      AUDIO_S16,   2,             22050);
	
	
	if(ret==-1){
	printf( "Couldn't build converter!\n");
	SDL_CloseAudio();
	// free(wave);
	 SDL_FreeWAV(wavedata);
	 return 0;
	}
	

    cvt->buf = (Uint8*) malloc(wavedlen*cvt->len_mult);

	
	if(cvt->buf== NULL){
	fprintf(stderr, "Couldn't malloc!\n");
	SDL_CloseAudio();
	// free(wave);
	 SDL_FreeWAV(wavedata);
	 return 0;
	}

    memcpy(cvt->buf, wavedata, wavedlen);
    cvt->len = wavedlen;
    SDL_ConvertAudio(cvt);
	// printf("SDL_ConvertAudio: %s\n", SDL_GetError());
    SDL_FreeWAV(wavedata);


    //* Put the sound data in the slot (it starts playing immediately) 
    if ( sounds[index].data ) {
        free(sounds[index].data);
    }


	SDL_LockAudio();
    sounds[index].data = cvt->buf;
    sounds[index].dlen = cvt->len_cvt;
    sounds[index].dpos = sounds[index].dlen;
    SDL_UnlockAudio();

	return index;
}

int PlaySound(int soundIndex)
{

	SDL_LockAudio();
    sounds[soundIndex].dpos = 0;
    SDL_UnlockAudio();

	return 1;
}


int StopSound(int soundIndex)
{

	SDL_LockAudio();
    sounds[soundIndex].dpos = sounds[soundIndex].dlen;
    SDL_UnlockAudio();

	return 1;
}

int SoundDone(int soundIndex)
{

	SDL_LockAudio();
    if (sounds[soundIndex].dpos >= sounds[soundIndex].dlen) return 1;
    SDL_UnlockAudio();

	return 0;
}


/*
int updateSound(int index)
{
	
	if (soundDone)
	{
		//SDL_PauseAudio(0);
		SDL_LockAudio();
		sounds[index].data = NULL;
		sounds[index].dlen = 0;
		sounds[index].dpos = 0;
		SDL_UnlockAudio();
	}
	
	return 1;
}
*/