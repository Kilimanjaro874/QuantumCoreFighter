#include "Score.h"
#include "define.h"
#include "../dxlib_ext/dxlib_ext.h"

Score::Score() {
	// --- コンストラクタ ---
	currentScore = 0;
	bombNum = PLAYER_INITIAL_BOMBNUM;
	hpNum = PLAYER_INITIAL_HP;
	hpNumP = 0;
	lifeNum = PLAYER_INITIAL_LIFENUM;
	powerNum = 1;
	speedNum = 1;
	defenseNum = 1;
	currentElement = RED;
	bombGh = 0;
	gageMax = 250;
	redGage = 0;
	orangeGage = 0;
	greenGage = 0;
	redBar = 0;
	orangeBar = 0;
	greenBar = 0;
	drawInit = false;
	statusGageGh = 0;
	powerBarGh = 0;
	speedBarGh = 0;
	defenseBarGh = 0;

}


void Score::All() {
	Draw();
}

void Score::Draw() {
	// --- 描画処理 ---

	// -- 初期化処理 --
	if (!drawInit) {

		// - メッセージ画像取得 -
		LoadDivGraph("graphics/Score/ScoreMessage2_8_205_528.png", 8, 1, 8, 205, 528 / 8, messageGh);
		// - 数字の画像取得 -
		LoadDivGraph("graphics/Score/ScoreNumber_10_300_38.png", 10, 10, 1, 300/10, 38, numGh);
		// - 残機の画像取得 - 
		LoadDivGraph("graphics/Score/QuantumCoreLife_3_30_10.png", 3, 3, 1, 30 / 3, 10, lifeNumGh);
		// - ボム画像取得 -
		bombGh = LoadGraph("graphics/bombDot.png");
		// - HPゲージ＆バー
		redGage = LoadGraph("graphics/HpGageBar/hpRedGage.png");
		orangeGage = LoadGraph("graphics/HpGageBar/hpOrangeGage.png");
		greenGage = LoadGraph("graphics/HpGageBar/hpGreenGage.png");
		redBar = LoadGraph("graphics/HpGageBar/hpBarRed.png");
		orangeBar = LoadGraph("graphics/HpGageBar/hpBarOrange.png");
		greenBar = LoadGraph("graphics/HpGageBar/hpBarGreen.png");
		// - ステータスゲージ＆バー
		statusGageGh = LoadGraph("graphics/StatusBar/StatusGage.png");	//
		powerBarGh = LoadGraph("graphics/StatusBar/PowerBar.png");
		speedBarGh = LoadGraph("graphics/StatusBar/SpeedBar.png");
		defenseBarGh = LoadGraph("graphics/StatusBar/DefenseBar.png");
		// - エレメント画像取得 -
		elementGh[0] = LoadGraph("graphics/Score/RedElementSelected01.png");
		elementGh[1] = LoadGraph("graphics/Score/BlueElementSelected01.png");
		elementGh[2] = LoadGraph("graphics/Score/YellowElementSelected01.png");


		drawInit = true;
	}

	// -- 描画処理 --

	// - 文字の表示(追々、マジックナンバーを修正したい) -
	int m_width, m_height;
	GetGraphSize(messageGh[0], &m_width, &m_height);
	int x_span = 20;
	int y_span = 75;

	for (int i = 0; i < 8; i++) {
		// 各項目表示
		// BOMB
		if (i == 1) {
			DrawExtendGraph(MENU_ORIGINE_X + 150, MENU_ORIGINE_Y + (int)y_span * i,
				MENU_ORIGINE_X + 150 + (int)m_width / 2, MENU_ORIGINE_Y + (int)y_span * i + (int)m_height / 2,
				messageGh[i], true);
		}
		// LIFE
		else if (i == 7) {
			DrawExtendGraph(MENU_ORIGINE_X + x_span, MENU_ORIGINE_Y + (int)y_span * 1,
				MENU_ORIGINE_X + (int)m_width / 2, MENU_ORIGINE_Y + (int)y_span * 1 + (int)m_height / 2,
				messageGh[i], true);
		}
		else {


			DrawExtendGraph(MENU_ORIGINE_X + x_span, MENU_ORIGINE_Y + (int)y_span * i,
				MENU_ORIGINE_X + (int)m_width / 2, MENU_ORIGINE_Y + (int)y_span * i + (int)m_height / 2,
				messageGh[i], true);
		}
	}

	// - 現在のスコア表示(数値) -
	char buf[20];
	int num;
	num = sprintf(buf, "%d", currentScore);
	for (int i = 0; i < num; i++) {
		DrawGraph(MENU_ORIGINE_X + x_span*2 + i*20, MENU_ORIGINE_Y + 30, numGh[(buf[i] - '0')], true);
	}

	// - 残機の表示(画像)
	for (int i = 0; i < lifeNum; i++) {
		DrawRotaGraph(MENU_ORIGINE_X + 5 + x_span * 1 + i * 30, MENU_ORIGINE_Y + 128, 2.5, 0, lifeNumGh[currentElement], true, false);
		
	}

	// - 残りのボム数表示(画像) -
	for (int i = 0; i < bombNum; i++) {
		if (bombNum > 0) {
			DrawRotaGraph(MENU_ORIGINE_X + 100 + x_span * 3 + i * 30, MENU_ORIGINE_Y + 128, 0.7, 0, bombGh, true, false);
		}
	}

	// - 現在のＨＰ表示(グラフ) -
	hpNumP = (int)hpNum * (float)gageMax / 100;
	if (hpNum >= 45) {
		// 緑のバー＆ゲージを表示
		DrawExtendGraph(MENU_ORIGINE_X + x_span * 1, MENU_ORIGINE_Y + 190, MENU_ORIGINE_X + x_span * 1 + hpNumP, MENU_ORIGINE_Y + 190 + 25,
			greenBar, true);
		DrawExtendGraph(MENU_ORIGINE_X + x_span * 1, MENU_ORIGINE_Y + 190, MENU_ORIGINE_X + x_span * 1 + gageMax, MENU_ORIGINE_Y + 190 + 25,
			greenGage, true);

	}
	else if (45 > hpNum && hpNum >= 20) {
		// オレンジのバー＆ゲージを表示
		DrawExtendGraph(MENU_ORIGINE_X + x_span * 1, MENU_ORIGINE_Y + 190, MENU_ORIGINE_X + x_span * 1 + hpNumP, MENU_ORIGINE_Y + 190 + 25,
			orangeBar, true);
		DrawExtendGraph(MENU_ORIGINE_X + x_span * 1, MENU_ORIGINE_Y + 190, MENU_ORIGINE_X + x_span * 1 + gageMax, MENU_ORIGINE_Y + 190 + 25,
			orangeGage, true);
	}
	else {
		// 赤のバー＆ゲージを表示
		DrawExtendGraph(MENU_ORIGINE_X + x_span * 1, MENU_ORIGINE_Y + 190, MENU_ORIGINE_X + x_span * 1 + hpNumP, MENU_ORIGINE_Y + 190 + 25,
			redBar, true);
		DrawExtendGraph(MENU_ORIGINE_X + x_span * 1, MENU_ORIGINE_Y + 190, MENU_ORIGINE_X + x_span * 1 + gageMax, MENU_ORIGINE_Y + 190 + 25,
			redGage, true);
	}

	// - 現在のパワーゲージ＆バー表示(グラフ) -
	int tempPosX = x_span + 34;
	for (int i = 0; i < powerNum; i++) {
		DrawRotaGraph(MENU_ORIGINE_X + tempPosX +(int)25*(i), MENU_ORIGINE_Y + 265 + 13, 0.65, 0, powerBarGh, true, false);
	}

	DrawExtendGraph(MENU_ORIGINE_X + x_span * 1, MENU_ORIGINE_Y + 265, 
					MENU_ORIGINE_X + x_span * 1 + 243, MENU_ORIGINE_Y + 265 + 26,
		statusGageGh, true);

	// - 現在のスピード表示(グラフ) - 
	for (int i = 0; i < speedNum; i++) {
		DrawRotaGraph(MENU_ORIGINE_X + tempPosX + (int)25 * (i), MENU_ORIGINE_Y + 340 + 13, 0.65, 0, speedBarGh, true, false);
	}

	DrawExtendGraph(MENU_ORIGINE_X + x_span * 1, MENU_ORIGINE_Y + 340,
		MENU_ORIGINE_X + x_span * 1 + 243, MENU_ORIGINE_Y + 340 + 26,
		statusGageGh, true);

	// - 現在のディフェンス表示(グラフ) - 
	for (int i = 0; i < defenseNum; i++) {
		DrawRotaGraph(MENU_ORIGINE_X + tempPosX + (int)25 * (i), MENU_ORIGINE_Y + 415 + 13, 0.65, 0, defenseBarGh, true, false);
	}

	DrawExtendGraph(MENU_ORIGINE_X + x_span * 1, MENU_ORIGINE_Y + 415,
		MENU_ORIGINE_X + x_span * 1 + 243, MENU_ORIGINE_Y + 415 + 26,
		statusGageGh, true);

	// -エレメント表示
	switch (currentElement)
	{
	case RED:
		DrawRotaGraph(ELEMENT_X, ELEMENT_Y, 0.30, 0, elementGh[0], true, false);
		break;
	case BLUE:
		DrawRotaGraph(ELEMENT_X, ELEMENT_Y, 0.30, 0, elementGh[1], true, false);
		break;
	case YELLOW:
		DrawRotaGraph(ELEMENT_X, ELEMENT_Y, 0.30, 0, elementGh[2], true, false);
		break;
	default:
		break;
	}


}

void Score::SetScore(SCOREDATA data, int val) {
	// --- スコアボードの値セット ---
	switch (data)
	{
	case CURRENT_SCORE:
		currentScore += val;
		if (currentScore < 0) {
			currentScore = 0;
		}
		break;

	case LIFE:
		lifeNum = val;
		break;

	case BOMB:
		bombNum = val;
		break;

	case HP:
		hpNum = val;
		break;

	case POWER:
		powerNum = val;
		break;

	case SPEED:
		speedNum = val;
		break;

	case DEFENSE:
		defenseNum = val;
		break;

	case ELEMENT:
		currentElement = val;

	default:
		break;
	}

}

int Score::GetScore(SCOREDATA data) {
	// --- スコアボードの値を返す ---
	switch (data)
	{
	case CURRENT_SCORE:
		return currentScore;
		break;

	case LIFE:
		return lifeNum;
		break;

	case BOMB:
		return bombNum;
		break;
		

	case HP:
		return  hpNum;
		break;

	case POWER:
		return powerNum;
		break;

	case DEFENSE:
		return defenseNum;
		break;

	case SPEED:
		return speedNum;
		break;

	case ELEMENT:
		return currentElement;
		break;

	default:
		break;
	}
}
