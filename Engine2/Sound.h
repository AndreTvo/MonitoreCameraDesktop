/**********************************************************************************
// Sound (Arquivo de Cabe�alho)
//
// Cria��o:		14 Out 2011
// Atualiza��o:	11 Jun 2014
// Compilador:	Visual C++ 12
//
// Descri��o:	Representa um som no formato WAVE
//
**********************************************************************************/

#ifndef _DESENVJOGOS_SOUND_H_
#define _DESENVJOGOS_SOUND_H_

// ---------------------------------------------------------------------------------

#include "Types.h"
#include <xaudio2.h> 
#include <string>										
using std::string;

// ---------------------------------------------------------------------------------

class Sound
{
private:
	WAVEFORMATEXTENSIBLE soundFormat;					// formato do arquivo .wav
	XAUDIO2_BUFFER       soundBuffer;					// buffer com os dados do som
	float                soundVolume;					// volume do som
	float				 soundFrequency;				// freq��ncia do som

	uint				voicesCount;					// n�mero de vozes para este som
	uint				currentVoice;					// voz selecionada
	IXAudio2SourceVoice  ** voices;						// vetor de ponteiros para vozes

	HRESULT FindChunk(HANDLE hFile,
		DWORD fourcc,
		DWORD & dwChunkSize,
		DWORD & dwChunkDataPosition);		// localiza blocos no arquivo RIFF

	HRESULT ReadChunkData(HANDLE hFile,
		void * buffer,
		DWORD buffersize,
		DWORD bufferoffset);			// l� blocos do arquivo para um buffer

	friend class Audio;

public:
	Sound(string fileName, uint nVoices);				// construtor
	~Sound();											// destrutor
};

// ---------------------------------------------------------------------------------


#endif