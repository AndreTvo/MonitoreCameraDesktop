/**********************************************************************************
// Audio (Arquivo de Cabe�alho)
// 
// Cria��o:		14 Out 2011
// Atualiza��o:	11 Jun 2014
// Compilador:	Visual C++ 12
//
// Descri��o:	Classe para reproduzir �udio
//
**********************************************************************************/

#ifndef _DESENVJOGOS_AUDIO_H_
#define _DESENVJOGOS_AUDIO_H_

// ---------------------------------------------------------------------------------

#include "Sound.h"											// guarda o som no formato WAVE
#include "Types.h"											// tipos espec�ficos da engine
#include <XAudio2.h>										// XAudio2 API
#include <unordered_map>									// tabela de dispers�o
#include <string>											// tipo string da STL
using std::unordered_map;
using std::string;

// ---------------------------------------------------------------------------------

class Audio
{
private:
	IXAudio2* audioEngine;							// sistema de �udio (engine)
	IXAudio2MasteringVoice* masterVoice;			// dispositivo principal de �udio
	
	unordered_map<uint, Sound*> sounds;

public:
	Audio();												// construtor
	~Audio();												// destrutor

	void Add(uint id, string filename, uint nVoices = 1);	// adiciona arquivo ao conjunto de sons
	void Play(uint id, bool repeat = false);				// inicia a reprodu��o do som
	void Stop(uint id);										// para a reprodu��o do som
	void Volume(uint id, float level);						// ajusta volume do som
	void Frequency(uint id, float level);					// ajusta a frequencia do som
}; 

// ---------------------------------------------------------------------------------

#endif