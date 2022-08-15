#pragma once
#include "define.h"

class Back
{
private:
	static const int backNum = 3;
	double x[backNum], y[backNum];		// 座標
	int backGh[backNum];		// グラフィックハンドラ
	bool drawInit;		// 描画初期処理フラグ
	int width[backNum];			// 取得背景の横幅
	int height[backNum];			// 取得背景の縦幅
	bool xScrool[backNum];			// x方向の画像を左右に動作させるためのフラグ
	char backName[NAME_SIZE];		// 背景名
	char effect1[NAME_SIZE];		// エフェクト名1
	char effect2[NAME_SIZE];		// エフェクト名2

private:
	void Draw();					// 背景描画
	void Move(float delta_time);	// 背景スクロール

public:
	void All(float delta_time);
	Back(char back[NAME_SIZE], char effect1[NAME_SIZE], char effect2[NAME_SIZE]);

};