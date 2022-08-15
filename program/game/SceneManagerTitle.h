#pragma once

class SceneManagerTitle
{
private:
	bool drawInit;			// 画像処理初期化フラグ
	bool soundInit;			// 音声初期処理フラグ
	int titleBackImageGh;	// タイトル背景
	int titleLogoImageGh;	// タイトルロゴ
	int selectLogoImageGh[6];		// 選択肢表示
	int selectedLogo;		// 選択中のロゴ
	int userChoice;			// ユーザ-の選択肢
	int hb_titleSound;		// タイトルBGMハンドラ
	bool fs_choiceSE;		// ユーザ選択肢変更効果音フラグ
	int hs_choiceSE;		// ユーザ選択肢変更効果音ハンドラ
	bool fs_decisionSE;		// ユーザ選択肢決定効果音フラグ
	int hs_decisionSE;		// ユーザ選択肢決定効果音ハンドラ
	int bgmVolume;			// BGMボリューム(0-100%)
	int seVolume;			// SEボリューム(0-100%)

private:
	void Draw();
	void Move(float delta_time);
	void SoundAll();
public:
	SceneManagerTitle();
	~SceneManagerTitle();
	bool All(float delta_time);
	void SetVolume(int bgmVolume, int seVolume);
	int GetUserChoice();
};