#include <SDL/SDL_mixer.h>
#include "audio.h"

// Indica se o sistema de �udio foi iniciado ou n�o bool audio_iniciado = false;
// Vari�vel para armazenar a m�sica a tocar no jogo Mix_Music *musica=0;
// Vari�vel para armazenar os efeitos sonoros Mix_Chunk **efeitos=0; int num_efeitos=0;

// Esta fun��o inicializa o sistema de �udio. Ela retorna true se funcionou
// e false se n�o conseguiu.

bool inicia_audio() {
// Tenta inicializar o sistema de �udio da SDL. Se der errado, retorna false if(SDL_InitSubSystem(SDL_INIT_AUDIO)!=0) audio_iniciado = false;
// Tenta iniciar a SDL_Mixer em est�rio, 16 bits, 44.1kHz, 4kb de buffer
// Se falhar, retorna false else if(Mix_OpenAudio(44100, AUDIO_S16SYS , 2, 4096)!=0) audio_iniciado = false; else
// Se chegou at� aqui, deu certo. audio_iniciado = true;
// REtorna o valor de iniciado return audio_iniciado;
}
bool encerra_audio() {
// Iniciou o sistema de �udio??
	if(!audio_iniciado)
		return false;
// Verifica se existe uma m�sica para apagar if(musica!=0)
	{
		Mix_HaltMusic();
		Mix_FreeMusic(musica);
		musica = 0;
	}
// Apaga os efeitos v�lidos if(efeitos!=0)
	{
		Mix_HaltChannel(-1);
		for(int i = 0; i
				if(efeitos[i] != 0)
					Mix_FreeChunk(efeitos[i]);
// apaga o vetor de efeitos delete efeitos;
					efeitos = 0;
					num_efeitos = 0;
		}
// Encerra o mixer Mix_CloseAudio();
// Encera o �udio da SDL SDL_QuitSubSystem(SDL_INIT_AUDIO);
// Marca como n�o iniciado audio_iniciado = false;
// Indica sucesso return true;
}
bool carrega_musica(char * arquivo) {
	if(!audio_iniciado) {
		printf("Tentou carregar a m�sica % s antes de iniciar o sistema de �udio.n", arquivo);
		return false;
	}
// Verifica se j� existe uma m�sica if(musica!=0)
	{
		Mix_HaltMusic();
		Mix_FreeMusic(musica);
		musica = 0;
	}
// Tenta carregar a m�sica, se voltar zero, falhou
	if((musica = Mix_LoadMUS(arquivo)) == 0) {
		printf("N�o conseguiu carregar o arquivo % s. Erro: % sn", arquivo, Mix_GetError());
		return false;
	}
	return true;
}
bool carrega_efeitos(char * * lista, int num) {
// Apaga os efeitos anteriores
	if(efeitos != 0) {
		Mix_HaltChannel(-1);
		for(int i = 0; i
				if(efeitos[i] != 0)
					Mix_FreeChunk(efeitos[i]);
// apaga o vetor de efeitos delete efeitos;
// Valores default efeitos=0;
					num_efeitos = 0;
		}
// Cria o vetor de efeitos efeitos = new Mix_Chunk*[num];
// Se n�o consegui criar, d� erro if(efeitos==0) return false;
		num_efeitos = num;
// Tenta carregar os efeitos. Se falhar algum, deixa o efeito vazio for(int i=0;i
if((efeitos[i] = Mix_LoadWAV(lista[i])) == 0)
		printf("Falhou ao carregar o efeito % s.n",
			   lista[i]);
	return true;
}
void toca_musica() {
// Testa se iniciou o sistema de �udio corretamente ou se carregou a m�sica if(!audio_iniciado || musica==0) return;
// Toca a m�sica com repeti��o Mix_PlayMusic(musica, -1);
}
void fade_musica(int milisegundos) {
// Testa se iniciou o sistema de �udio corretamente ou se carregou a m�sica if(!audio_iniciado || musica==0) return;
	if(milisegundos >= 0)
		Mix_FadeOutMusic(milisegundos);
	else
		Mix_HaltMusic();
}
void toca_efeito(int num) {
// Testa se iniciou o sistema de �udio corretamente ou se carregou a m�sica if(!audio_iniciado || musica==0) return;
	if(efeitos != 0 && num >= 0 && num
			if(efeitos[num] != 0)
				Mix_PlayChannel(-1, efeitos[num], 0);
	}
