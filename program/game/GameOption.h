#pragma once

class GameOption 
{
private:
	bool drawInit;			// 画像処理初期化フラグ
	bool soundInit;			// 音声初期化フラグ
	bool enable;			// オプション有効フラグ
	bool exitFlag;			// オプション終了フラグ
	int optionBackImageGh;	// オプション背景ハンドラ
	int titleLogoGh;		// オプションタイトルロゴハンドラ
	int selectLogoGh[4];	// 選択肢表示ロゴ
	int gageGh;				// ボリュームゲージ
	int barGh;				// ボリュームバー
	int numGh[10];			// ボリューム数表示用
	int guideLogoSelectGh;	// ガイドロゴハンドラ
	int guideLogoAdjustGh;	// ガイドロゴハンドラ
	int exitLogoGh[2];		// exitロゴハンドラ
	int selectedLogo;		// 選択中のロゴ
	int userChoice;			// ユーザの選択肢格納
	int bgmVolumePercentage;	// BGM音量(0-100%)
	int seVolumePercentage;		// SE音量(0-100%)
	int preBgmVolumePercentage;	// 1フレーム前のBGM音量
	int preSeVolumePercentage;	// 1フレーム前のSE音量

	bool fs_choiceSE;		// ユーザ選択肢変更効果音フラグ
	int hs_choiceSE;		// ユーザ選択肢変更効果音ハンドラ
	bool fs_decisionSE;		// ユーザ選択肢決定効果音フラグ
	int hs_decisionSE;		// ユーザ選択肢決定効果音ハンドラ

private:
	void Draw();
	void Move();
	void Delete();
	void SoundAll();
	//void ChangeScrennSize();	// バグ発生. 余裕があれば実装する
public:
	GameOption();
	bool All();
	void SetOption();
	bool GetVolumeFlag();
	void GetVolume(int* bgmVolume, int* seVolume);

};