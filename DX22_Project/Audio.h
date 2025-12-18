#pragma once

#include <xaudio2.h>
#include <map>
#include <string>

/// <summary>
/// 音を扱うコンポーネントクラス
/// </summary>
class CAudio : public CComponent
{
public:
	using CComponent::CComponent;
	static void InitMaster();       // XAudio関係の初期化処理
	static void UninitMaster();     // XAudio関係の終了処理
	void Uninit();

	/// <summary>
	/// 指定されたパスから音データを読み込む
	/// </summary>
	/// <param name="inPath">
	/// 読み込むデータのファイルパス
	/// </param>
	void Load(const char* inPath);

	/// <summary>
	/// サウンドを再生
	/// </summary>
	/// <param name="inLoop">
	/// true:ループ再生 false:一回きり再生
	/// </param>
	void Play(bool inLoop = false);

	/// <summary>
	/// サウンドの停止
	/// </summary>
	void Stop();

	/// <summary>
	/// ボリュームを設定
	/// </summary>
	/// <param name="inVolume">
	/// ボリュームを0.0f～1.0fで指定
	/// </param>
	void SetVolume(float inVolume);

	/// <summary>
	/// サウンドがプレイ中かどうかを取得
	/// </summary>
	/// <returns>
	/// true:再生中 false:停止中
	/// </returns>
	bool IsPlay();

private:
	static IXAudio2* m_pXAudio;
	static IXAudio2MasteringVoice* m_pMasteringVoice;

	static std::map<std::string,IXAudio2SourceVoice*> m_pSourceVoice;
	std::string m_sKey = "";
	BYTE* m_pSoundData{};

	int	m_nLength{};
	int	m_nPlayLength{};

};
