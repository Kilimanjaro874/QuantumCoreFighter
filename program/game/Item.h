#pragma once

class Item
{
private:
	int type;						// アイテムの種類
	bool existeFlag;				// アイテム出現フラグ
	bool drawInit;					// 描画系初期化
	static int hpItemGh[3];
	static int powerItemGh[3];
	static int defenseItemGh[3];
	static int speedItemGh[3];
	static int bombItemGh[3];
	double x, y;
	int animFrame;					// アニメーションのフレーム
	float animCounter;				// アニメ切替時間カウンタ
	float animChangeTime;			// アニメーション切替時間
	float totalTime;				// 生存フラグが立ってからの時間
	
private:
	void Move(float delta_time);
	void Draw();
	void Time(float delta_time);
public:
	Item();
	void All(float delta_time);
	void Delete();
	int GetType();
	bool GetFlag();
	void GetPosition(double* x, double* y);
	void SetFlag(double x, double y, int data);
};