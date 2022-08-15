#pragma once

class SceneManagerOpening 
{
private:
	float totalTime;		// 経過時間
	float animCount;		// アニメーション切替カウンタ
	int animFrame;			// アニメーションフレーム指定
	bool drawInit;			// 画像処理初期化フラグ
	int opImageGh[7];		// オープニングアニメ画像ハンドラ
	int opMessageGh[2];		// オープニング表示ロゴ画像ハンドラ
	int bgmVolume;			// BGMボリューム(0-100%)
	int seVolume;			// BGMボリューム(0-100%)


private:
	void Draw();
	void Time(float delta_time);

public:
	SceneManagerOpening();
	~SceneManagerOpening();
	bool All(float delta_time);
	void SetVolume(int bgmvolume, int sevolume);

};