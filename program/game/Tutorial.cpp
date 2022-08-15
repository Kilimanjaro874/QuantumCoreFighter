#include "Tutorial.h"
#include "../dxlib_ext/dxlib_ext.h"
#include "define.h"

Tutorial::Tutorial() {
	// --- コンストラクタ ---
	tutorialGh[5] = { 0 };
	count = 0;
	tutorialEndFlag = false;
	drawInit = false;
	soundInit = false;

}

bool Tutorial::All() {
	// --- チュートリアルクラスを外部から操作 ---
	Move();
	Draw();
	return tutorialEndFlag;
}

void Tutorial::Draw() {
	// --- 描画系 ---
	if (!drawInit) {
		// -- 描画初期処理
		tutorialGh[0] = LoadGraph("graphics/Tutorial/Controls.png");
		tutorialGh[1] = LoadGraph("graphics/Tutorial/Technic1.png");
		tutorialGh[2] = LoadGraph("graphics/Tutorial/Technic2.png");
		tutorialGh[3] = LoadGraph("graphics/Tutorial/Technic3.png");
		tutorialGh[4] = LoadGraph("graphics/Tutorial/Technic4.png");

		drawInit = true;
	}
	
	// -- チュートリアル描画 --
	DrawRotaGraph(WIDTH / 2, HEIGHT / 2, 1.0, 0, tutorialGh[count], true, false);

}

void Tutorial::Move() {
	// --- 動作系 ---

	// -- チュートリアルのカウント
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		count++;
	}
	if (count >= 5) {
		count = 4;
		tutorialEndFlag = true;
	}
}


