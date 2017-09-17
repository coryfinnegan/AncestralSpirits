



void mixaudio(void *unused, Uint8 *stream, int len);


int LoadSound(char *file, GameManager* pGM);

int PlaySound(int soundIndex);

int StopSound(int soundIndex);

int SoundDone(int soundIndex);

//int updateSound(int index);