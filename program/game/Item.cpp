#include "Item.h"
#include "define.h"
#include "../dxlib_ext/dxlib_ext.h"

int Item::hpItemGh[3];
int Item::powerItemGh[3];
int Item::defenseItemGh[3];
int Item::speedItemGh[3];
int Item::bombItemGh[3];

Item::Item() {
	// --- コンストラクタ ---
	type = 0;		// アイテム種類セットはSetFlagで実施
	existeFlag = false;
	drawInit = false;
	hpItemGh[3] = { 0 };
	powerItemGh[3] = { 0 };
	defenseItemGh[3] = { 0 };
	speedItemGh[3] = { 0 };
	bombItemGh[3] = { 0 };
	x = 0;
	y = 0;
	animFrame = 0;
	animCounter = 0;
	animChangeTime = 0.1;
	totalTime = 0;

}

void Item::All(float delta_time) {
	// ---　外部からアイテムクラス実施 ---
	// -- 画面範囲外に移動したら、falseを返す
	// (又は、Delete()関数から削除を実施する
	Move(delta_time);
	Draw();
	Time(delta_time);

}

void Item::Move(float delta_time) {
	// --- 移動系 ---
	
	// -- 出現時間が1秒以内であれば、アイテム上昇
	if (totalTime <= 1.0) {
		y -= SCROLL_SPEED * delta_time;
	}
	else {
		// - 背景のスクロールスピードに合わせて移動 -
		y += SCROLL_SPEED * delta_time;
	}

	// -- 画面外に出た時、フラグを戻す
	if (y > HEIGHT) {
		Delete();
	}

}

void Item::Draw() {
	// --- 描画系 ---
	if (!drawInit) {
		// -- グラフィックハンドラへ参照格納 --
		LoadDivGraph("graphics/Item/HpItem_3_516_154.png", 3, 3, 1, 516 / 3, 154, hpItemGh);
		LoadDivGraph("graphics/Item/PowerItem_10_768_202.png", 3, 3, 1, 768 / 3, 202, powerItemGh);
		LoadDivGraph("graphics/Item/DefenseItem_3_606_200.png", 3, 3, 1, 606 / 3, 200, defenseItemGh);
		LoadDivGraph("graphics/Item/SpeedItem_3_858_284.png", 3, 3, 1, 858 / 3, 284, speedItemGh);
		LoadDivGraph("graphics/Item/BombItem_3_861_284.png", 3, 3, 1, 861 / 3, 284, bombItemGh);

		drawInit = true;
	}

	// -- 描画 --
	// - アニメーション切替 - 
	if (animCounter >= animChangeTime) {
		animCounter = 0;
		animFrame++;
		animFrame %= 3;
	}

	switch (type)
	{
	case ITEM_NONE:
		// アイテム無し
		break;

	case ITEM_HP:
		// 回復アイテム
		DrawRotaGraph(x, y, 0.24, 0, hpItemGh[animFrame], true, false);

		break;

	case ITEM_POWER:
		// パワーアップアイテム
		DrawRotaGraph(x, y, 0.24, 0, powerItemGh[animFrame], true, false);
		
		break;

	case ITEM_DEFENSE:
		// ディフェンスアイテム
		DrawRotaGraph(x, y, 0.23, 0, defenseItemGh[animFrame], true, false);

		break;

	case ITEM_SPEED:
		// スピードアイテム
		DrawRotaGraph(x, y, 0.23, 0, speedItemGh[animFrame], true, false);

		break;

	case ITEM_BOMB:
		// ボムアイテム
		DrawRotaGraph(x, y, 0.14, 0, bombItemGh[animFrame], true, false);

		break;

	}

}

void Item::Time(float delta_time) {
	// --- 時間＆フラグ管理 ---
	animCounter += delta_time;
	if (existeFlag) {
		totalTime += delta_time;
	}
	else {
		totalTime = 0;
	}

}

void Item::Delete() {
	// --- アイテム消滅フラグ ---
	totalTime = 0;
	existeFlag = false;
}

int  Item::GetType() {
	// --- 取得したアイテムの種類を返す ---
	return type;
}

bool Item::GetFlag() {
	// --- 生存フラグを返す ---
	return existeFlag;
}

void Item::GetPosition(double* x, double *y) {
	// --- アイテムの位置を参照渡し ---
	*x = this->x;
	*y = this->y;
}

void Item::SetFlag(double x, double y, int data) {
	// --- インスタンス済みのアイテムのフラグを立てる ---
	// -- アイテムの種類がNONEの時以外、有効化 --
	if (data != 0) {
		this->x = x;
		this->y = y;
		this->type = data;
		existeFlag = true;
	}
	else {
		// - 何もしない -

	}
}
