#pragma once
#include "define.h"

class SelectCh
{
private:
	bool drawInit;		// 画像処理初期化フラグ
	bool soundInit;		// 音声切替処理フラグ
	int chBackGh;		// 背景用グラフィックハンドラ
	int titleLogoGh;		// タイトルロゴグラフィックハンドラ
	int selectLogoGh[TOTAL_CH];			// チャプター総数ロゴハンドラ
	int selectComentaryGh[TOTAL_CH];	// チャプター解説画像ハンドラ
	int selectFrameGh;					// チャプター選択枠
	int guideLogo;			// 操作方法説明用ロゴ
	int userChoice;						// ユーザー選択肢
	bool fs_choiceSE;	// ユーザ選択肢変更効果音フラグ 
	int hs_choiceSE;	// ユーザ選択肢変更効果音ハンドラ
	bool fs_decisionSE;	// ユーザ選択肢決定効果音フラグ
	int hs_decisionSE;	// ユーザ選択肢決定効果音ハンドラ
	int bgmVolume;		// BGM音量
	int seVolume;		// 効果音量
	int hb_chSelectBGM;	// チャプターセレクトBGMハンドラ

private:
	void Draw();
	void Move();
	void SoundAll();
public:
	SelectCh();
	~SelectCh();
	int All();
	void SetSEVolume(int tempBGM, int tempSE);

};