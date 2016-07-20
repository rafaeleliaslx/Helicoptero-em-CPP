#include <SDL/SDL_mixer.h>
#include "audio.h"

// Indica se o sistema de áudio foi iniciado ou não bool audio_iniciado = false;
// Variável para armazenar a música a tocar no jogo Mix_Music *musica=0;
// Variável para armazenar os efeitos sonoros Mix_Chunk **efeitos=0; int num_efeitos=0;

// Esta função inicializa o sistema de áudio. Ela retorna true se funcionou
// e false se não conseguiu.

bool inicia_audio() {
// Tenta inicializar o sistema de áudio da SDL. Se der errado, retorna false if(SDL_InitSubSystem(SDL_INIT_AUDIO)!=0) audio_iniciado = false;
// Tenta iniciar a SDL_Mixer em estério, 16 bits, 44.1kHz, 4kb de buffer
// Se falhar, retorna false else if(Mix_OpenAudio(44100, AUDIO_S16SYS , 2, 4096)!=0) audio_iniciado = false; else
// Se chegou até aqui, deu certo. audio_iniciado = true;
// REtorna o valor de iniciado return audio_iniciado;
}
bool encerra_audio() {
// Iniciou o sistema de áudio??
	if(!audio_iniciado)
		return false;
// Verifica se existe uma música para apagar if(musica!=0)
	{
		Mix_HaltMusic();
		Mix_FreeMusic(musica);
		musica = 0;
	}
// Apaga os efeitos válidos if(efeitos!=0)
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
// Encera o áudio da SDL SDL_QuitSubSystem(SDL_INIT_AUDIO);
// Marca como não iniciado audio_iniciado = false;
// Indica sucesso return true;
}
bool carrega_musica(char * arquivo) {
	if(!audio_iniciado) {
		printf("Tentou carregar a música % s antes de iniciar o sistema de áudio.n", arquivo);
		return false;
	}
// Verifica se já existe uma música if(musica!=0)
	{
		Mix_HaltMusic();
		Mix_FreeMusic(musica);
		musica = 0;
	}
// Tenta carregar a música, se voltar zero, falhou
	if((musica = Mix_LoadMUS(arquivo)) == 0) {
		printf("Não conseguiu carregar o arquivo % s. Erro: % sn", arquivo, Mix_GetError());
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
// Se não consegui criar, dá erro if(efeitos==0) return false;
		num_efeitos = num;
// Tenta carregar os efeitos. Se falhar algum, deixa o efeito vazio for(int i=0;i
if((efeitos[i] = Mix_LoadWAV(lista[i])) == 0)
		printf("Falhou ao carregar o efeito % s.n",
			   lista[i]);
	return true;
}
void toca_musica() {
// Testa se iniciou o sistema de áudio corretamente ou se carregou a música if(!audio_iniciado || musica==0) return;
// Toca a música com repetição Mix_PlayMusic(musica, -1);
}
void fade_musica(int milisegundos) {
// Testa se iniciou o sistema de áudio corretamente ou se carregou a música if(!audio_iniciado || musica==0) return;
	if(milisegundos >= 0)
		Mix_FadeOutMusic(milisegundos);
	else
		Mix_HaltMusic();
}
void toca_efeito(int num) {
// Testa se iniciou o sistema de áudio corretamente ou se carregou a música if(!audio_iniciado || musica==0) return;
	if(efeitos != 0 && num >= 0 && num
			if(efeitos[num] != 0)
				Mix_PlayChannel(-1, efeitos[num], 0);
	}
