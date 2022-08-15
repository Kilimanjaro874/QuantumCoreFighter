#pragma once

#include "define.h"

class DeadEffect 
{
private:
	int type;			// 表示するエフェクトの種類
	double x, y;		// 座標
	double velocityX;	// 速度
	double velocityY;
	bool drawInit;		// 描画初期化処理
	int gh[DEAD_EFFECT_FRAME_MAX];	// グラフィックハンドラ最大数
	int ghDivNum;		// 実際のグラフィックハンドラ分割数格納
	int animFrame;		// アニメーションフレーム
	float rad;			// 画像回転
	int alpha;			// 画像透明度
	float rate;			// 画像拡大率
	float rateInitial;	// 画像拡大率初期値
	float rateMax;		// 画像拡大率上限
	float elapsedTime;	// 生成からの経過時間格納
	float animTime;		// グラフィックハンドラ切替タイミング管理
	float vanishingTime;	// エフェクト生成から消滅するまでの時間
	bool endFlag;		// 破壊フラグ

private:
	void Move(float delta_time);
	void Draw();
	void TimeAndFlag(float delta_time);

public:
	DeadEffect(	int type, double x, 
				double y, 
				double velocityX, 
				double velocityY,
				float rateInitial,
				float rateMax,
				float vanishingTime);
	bool All(float delta_time);

};