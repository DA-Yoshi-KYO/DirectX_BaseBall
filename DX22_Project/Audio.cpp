
#include "Main.h"
#include "Audio.h"

std::map<std::string, CAudio::SoundData>  CAudio::m_tSoundData{};
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
    for (auto itr : m_tSoundData)
    {
        if (itr.second.m_pSourceVoice)
        {
            itr.second.m_pSourceVoice->DestroyVoice();
        }
        if (itr.second.m_pSoundData)
        {
            delete itr.second.m_pSoundData;
        }
    }
    m_pMasteringVoice->DestroyVoice();
    m_pXAudio->Release();
    CoUninitialize();
}

void CAudio::Load(const char* inPath)
{
    m_sKey = inPath;

    if (m_tSoundData.find(m_sKey) != m_tSoundData.end())
    {
        return;
    }

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
        m_tSoundData[m_sKey].m_pSoundData = new unsigned char[buflen];
        readlen = mmioRead(hmmio, (HPSTR)m_tSoundData[m_sKey].m_pSoundData, buflen);


        m_tSoundData[m_sKey].m_nLength = readlen;
        m_tSoundData[m_sKey].m_nPlayLength = readlen / wfx.nBlockAlign;


        mmioClose(hmmio, 0);
    }


    // サウンドソース生成
    m_pXAudio->CreateSourceVoice(&m_tSoundData[m_sKey].m_pSourceVoice, &wfx);
    assert(m_tSoundData[m_sKey].m_pSourceVoice);
}

void CAudio::Uninit()
{
    m_tSoundData[m_sKey].m_pSourceVoice->Stop();
}

void CAudio::Play(bool Loop)
{
    m_tSoundData[m_sKey].m_pSourceVoice->Stop();
    m_tSoundData[m_sKey].m_pSourceVoice->FlushSourceBuffers();


    // バッファ設定
    XAUDIO2_BUFFER bufinfo;

    memset(&bufinfo, 0x00, sizeof(bufinfo));
    bufinfo.AudioBytes = m_tSoundData[m_sKey].m_nLength;
    bufinfo.pAudioData = m_tSoundData[m_sKey].m_pSoundData;
    bufinfo.PlayBegin = 0;
    bufinfo.PlayLength = m_tSoundData[m_sKey].m_nPlayLength;

    // ループ設定
    if (Loop)
    {
        bufinfo.LoopBegin = 0;
        bufinfo.LoopLength = m_tSoundData[m_sKey].m_nPlayLength;
        bufinfo.LoopCount = XAUDIO2_LOOP_INFINITE;
    }

    m_tSoundData[m_sKey].m_pSourceVoice->SubmitSourceBuffer(&bufinfo, NULL);


    // 再生
    m_tSoundData[m_sKey].m_pSourceVoice->Start();

}

void CAudio::Stop()
{
    m_tSoundData[m_sKey].m_pSourceVoice->Stop();
}

void CAudio::SetVolume(float inVolume)
{
    inVolume = std::clamp(inVolume, 0.0f, 1.0f);
    m_tSoundData[m_sKey].m_pSourceVoice->SetVolume(inVolume);
}

bool CAudio::IsPlay()
{
    XAUDIO2_VOICE_STATE state;
    m_tSoundData[m_sKey].m_pSourceVoice->GetState(&state);
    return state.BuffersQueued > 0;
}
