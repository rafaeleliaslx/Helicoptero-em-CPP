#define AUDIO

bool inicia_audio();
bool encerra_audio();

bool carrega_musica(char *arquivo);
bool carrega_efeitos(char **lista, int num);

void toca_musica();
void fade_musica(int milisegundos);
void toca_efeito(int num);