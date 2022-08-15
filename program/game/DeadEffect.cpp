#include "DeadEffect.h"
//#include <stdlib.h>	// rand()に必要
//#include <time.h>	// time()に必要
#include "../dxlib_ext/dxlib_ext.h"


DeadEffect::DeadEffect(	int type, 
						double x, 
						double y, 
						double velocityX, 
						double velocityY,
						float rateInitial,
						float rateMax, 
						float vanishingTime ) {
	// --- コンストラクタ ---
	this->type = type;
	this->x = x;
	this->y = y;
	this->velocityX = velocityX;			// プレイヤー or 敵機破壊時の速度
	this->velocityY = velocityY;
	drawInit = false;
	gh[DEAD_EFFECT_FRAME_MAX] = { 0 };
	ghDivNum = 0;
	animFrame = 0;
	rad = 0;
	alpha = 0;
	rate = 0;
	this->rateInitial = rateInitial;
	this->rateMax = rateMax;
	elapsedTime = 0;
	animTime = 0;
	this->vanishingTime = vanishingTime;
	endFlag = false;
	srand((unsigned int)time(0));			// ランダム関数のシード値を得る
}

bool  DeadEffect::All(float delta_time) {
	// --- エフェクト
	// - 速度制御＆エフェクト管理 -
	if (delta_time != 0) {
		Move(delta_time);
	}
	// - 描画 -
	Draw();
	// - 時間管理 -
	TimeAndFlag(delta_time);
	// - 消滅フラグを返す -
	return endFlag;
	
}

void DeadEffect::Move(float delta_time) {
	// --- 移動の処理 ---

	// -- 係数更新＆移動系 -- 
	x += velocityX;
	y += velocityY;

	// - ランダムな角度保存(画像傾け用) -
	if (elapsedTime == 0) {
		rad = rand() % (int)(2 * tnl::PI);
	}
	// - 拡大率変化 -
	if (rate <= rateMax) {
		rate = rateInitial + rateMax * (elapsedTime / vanishingTime);	// (rateInitial + 1~100%)
	}
	else {
		rate = rateMax;
	}
	// - 透過率変化(elapsedTime == vanishingTimeの時0% -
	// 255 ~ 0 (elapsedTime == vanishingTimeの時0%)
	alpha =(int)(255 * (vanishingTime - elapsedTime) * (1 / vanishingTime));

}


void DeadEffect::Draw() {
	// --- 描画系 ---

	// -- 初期化処理 --
	if (!drawInit) {
		// - typeに応じて特定の画像読み込み -

		switch (type)
		{
		case 1:
			// デッドエフェクト001
			LoadDivGraph("graphics/Effect/DeadEffect001_10_560_48.png", 10, 10, 1, 560/10, 48, gh);
			ghDivNum = 10 - 1;
			break;

		case 2:
			// デッドエフェクト002
			LoadDivGraph("graphics/Effect/DeadEffect002_8_576_65.png", 8, 8, 1, 576 / 8, 65, gh);
			ghDivNum = 8 - 1;
			break;

		default:
			break;
		}

		drawInit = true;
	}

	// -- 描画処理 --
	
	// - 時間に応じて特定の画像描画
	if (vanishingTime / ghDivNum <= elapsedTime) {
		animTime = 0;
		animFrame++;
		if (animFrame >= ghDivNum)
			animFrame = ghDivNum;
	}

	// - 透過度適用 - 
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	// - 画像描画 -
	DrawRotaGraph(x, y, rate, rad, gh[animFrame], true);
	// - 透過度適用リセット -
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

}

void DeadEffect::TimeAndFlag(float delta_time) {
	// --- 時間＆フラグの管理 ---
	elapsedTime += delta_time;
	animTime += delta_time;
	if (elapsedTime >= vanishingTime) {
		endFlag = true;
	}
	else {
		endFlag = false;
	}
}