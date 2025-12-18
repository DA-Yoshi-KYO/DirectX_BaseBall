
#include "Main.h"
#include "Audio.h"

std::map<std::string,IXAudio2SourceVoice*>  CAudio::m_pSourceVoice{};
IXAudio2* CAudio::m_pXAudio = nullptr;
IXAudio2MasteringVoice* CAudio::m_pMasteringVoice = nullptr;

void CAudio::InitMaster()
{
	HRESULT hr;
	// COM初期化
	hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if (FAILED(hr))
	{
		MessageBox(NULL, "AudioInitialize", "Error", MB_OK);
	}

	// XAudio生成
	hr = XAudio2Create(&m_pXAudio, 0);
	if (FAILED(hr))
	{
		MessageBox(NULL, "AudioCreate", "Error", MB_OK);
	}
	// マスタリングボイス生成
	hr = m_pXAudio->CreateMasteringVoice(&m_pMasteringVoice);
	if (FAILED(hr))
	{
		MessageBox(NULL, "MasteringVoiceCreate", "Error", MB_OK);
	}
}

void CAudio::UninitMaster()
{
	for (auto itr : m_pSourceVoice)
	{
		if (itr.second)
		{
			delete itr.second;
			itr.second = nullptr;
		}
	}
	m_pMasteringVoice->DestroyVoice();
	m_pXAudio->Release();
	CoUninitialize();
}

void CAudio::Load(const char* inPath)
{

	// サウンドデータ読込
	WAVEFORMATEX wfx = { 0 };

	{
		HMMIO hmmio = NULL;
		MMIOINFO mmioinfo = { 0 };
		MMCKINFO riffchunkinfo = { 0 };
		MMCKINFO datachunkinfo = { 0 };
		MMCKINFO mmckinfo = { 0 };
		UINT32 buflen;
		LONG readlen;


		hmmio = mmioOpen((LPSTR)inPath, &mmioinfo, MMIO_READ);
		assert(hmmio);

		riffchunkinfo.fccType = mmioFOURCC('W', 'A', 'V', 'E');
		mmioDescend(hmmio, &riffchunkinfo, NULL, MMIO_FINDRIFF);

		mmckinfo.ckid = mmioFOURCC('f', 'm', 't', ' ');
		mmioDescend(hmmio, &mmckinfo, &riffchunkinfo, MMIO_FINDCHUNK);

		if (mmckinfo.cksize >= sizeof(WAVEFORMATEX))
		{
			mmioRead(hmmio, (HPSTR)&wfx, sizeof(wfx));
		}
		else
		{
			PCMWAVEFORMAT pcmwf = { 0 };
			mmioRead(hmmio, (HPSTR)&pcmwf, sizeof(pcmwf));
			memset(&wfx, 0x00, sizeof(wfx));
			memcpy(&wfx, &pcmwf, sizeof(pcmwf));
			wfx.cbSize = 0;
		}
		mmioAscend(hmmio, &mmckinfo, 0);

		datachunkinfo.ckid = mmioFOURCC('d', 'a', 't', 'a');
		mmioDescend(hmmio, &datachunkinfo, &riffchunkinfo, MMIO_FINDCHUNK);



		buflen = datachunkinfo.cksize;
		m_pSoundData = new unsigned char[buflen];
		readlen = mmioRead(hmmio, (HPSTR)m_pSoundData, buflen);


		m_nLength = readlen;
		m_nPlayLength = readlen / wfx.nBlockAlign;


		mmioClose(hmmio, 0);
	}


	// サウンドソース生成
	m_pXAudio->CreateSourceVoice(&m_pSourceVoice[m_sKey], &wfx);
	assert(m_pSourceVoice[m_sKey]);
}

void CAudio::Uninit()
{
	m_pSourceVoice[m_sKey]->Stop();
	m_pSourceVoice[m_sKey]->DestroyVoice();
}

void CAudio::Play(bool Loop)
{
	m_pSourceVoice[m_sKey]->Stop();
	m_pSourceVoice[m_sKey]->FlushSourceBuffers();


	// バッファ設定
	XAUDIO2_BUFFER bufinfo;

	memset(&bufinfo, 0x00, sizeof(bufinfo));
	bufinfo.AudioBytes = m_nLength;
	bufinfo.pAudioData = m_pSoundData;
	bufinfo.PlayBegin = 0;
	bufinfo.PlayLength = m_nPlayLength;

	// ループ設定
	if (Loop)
	{
		bufinfo.LoopBegin = 0;
		bufinfo.LoopLength = m_nPlayLength;
		bufinfo.LoopCount = XAUDIO2_LOOP_INFINITE;
	}

	m_pSourceVoice[m_sKey]->SubmitSourceBuffer(&bufinfo, NULL);


	// 再生
	m_pSourceVoice[m_sKey]->Start();

}

void CAudio::Stop()
{
    m_pSourceVoice[m_sKey]->Stop();
}

void CAudio::SetVolume(float inVolume)
{
    inVolume = std::clamp(inVolume, 0.0f, 1.0f);
    m_pSourceVoice[m_sKey]->SetVolume(inVolume);
}

bool CAudio::IsPlay()
{
    XAUDIO2_VOICE_STATE state;
    m_pSourceVoice[m_sKey]->GetState(&state);
	return state.BuffersQueued > 0;
}
