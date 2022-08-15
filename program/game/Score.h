#pragma once

enum SCOREDATA {
	CURRENT_SCORE,
	LIFE,
	BOMB,
	HP,
	POWER,
	DEFENSE,
	SPEED,
	ELEMENT
};

class Score 
{
private:
	int currentScore;		// 現在のスコア
	int bombNum;			// 残りのボム数
	int hpNum;				// 現在のHP
	int hpNumP;				// 現在のHP(バーに対する割合%)
	int lifeNum;			// 残りのライフ数
	int stateMaxNum;		// ステータス上昇上限値
	int powerNum;			// 現在のパワー(%)
	int speedNum;			// 現在のスピード(%)
	int defenseNum;			// 現在の防御力(%)
	int currentElement;		// 現在選択中のエレメント
	int elementGh[3];		// エレメント画像ハンドラ
	int messageGh[8];		// スコアでの文字画像ハンドラ
	int numGh[10];			// 数字の画像ハンドラ
	int lifeNumGh[3];		// 残機画像ハンドラ 
	int bombGh;				// ボム画像ハンドラ
	int gageMax;			// hpゲージの横幅最大値
	int redGage;			// hp赤ゲージ
	int orangeGage;
	int greenGage;
	int redBar;				// hp赤バー
	int orangeBar;
	int greenBar;
	int statusGageGh;		// ステータスゲージ画像ハンドラ
	int powerBarGh;			// パワーバー画像ハンドラ
	int speedBarGh;			// スピードバー画像ハンドラ
	int defenseBarGh;		// ディフェンスバー画像ハンドラ
	bool drawInit;			// 描画初期化フラグ

private:
	void Draw();

public:
	void SetScore(SCOREDATA data, int val);
	int GetScore(SCOREDATA data);
	void All();
	Score();

};