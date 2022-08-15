#include "GameOption.h"
#include "../dxlib_ext/dxlib_ext.h"
#include "define.h"

// セレクター
#define BGM_SELECT 1
#define SE_SELECT 2 
#define EXIT_SELECT 3

GameOption::GameOption() {
	// --- コンストラクタ ---
	drawInit = false;
	soundInit = false;
	enable = false;
	exitFlag = false;
	selectedLogo = 0;
	userChoice = 0;
	bgmVolumePercentage = BGM_INITIAL_VOLUME;
	seVolumePercentage = SE_INITIAL_VOLUME;
	preBgmVolumePercentage = BGM_INITIAL_VOLUME;
	preSeVolumePercentage = SE_INITIAL_VOLUME;
	fs_choiceSE = false;
	hs_choiceSE = 0;
	fs_decisionSE = false;
	hs_decisionSE = 0;
}

bool GameOption::All() {
	// --- オプションクラス操作 ---
	exitFlag = false;
	Move();
	Draw();
	SoundAll();
	return exitFlag;
}

void GameOption::Draw() {
	// --- 描画系処理 ---
	// -- 描画初期処理 --
	if (!drawInit) {
		optionBackImageGh = LoadGraph("graphics/Option/OptionWindow.jpg");
		titleLogoGh = LoadGraph("graphics/Option/OptionTitle_1_281_136.png");
		LoadDivGraph("graphics/Option/OptionLogo_4_196_384.png", 4, 1, 4, 196, 384 / 4, selectLogoGh);
		gageGh = LoadGraph("graphics/HpGageBar/hpGreenGage.png");	// HPゲージを流用
		barGh = LoadGraph("graphics/HpGageBar/hpBarGreen.png");		// HPバーを流用
		LoadDivGraph("graphics/Score/ScoreNumber_10_300_38.png", 10, 10, 1, 300 / 10, 38, numGh);	// スコア数を流用
		guideLogoSelectGh = LoadGraph("graphics/Option/OptionLogoSelect_1_267_135.png");
		guideLogoAdjustGh = LoadGraph("graphics/Option/OptionLogoAdjust_1_267_135.png");
		LoadDivGraph("graphics/Option/OptionLogoExit_2_289_192.png", 2, 1, 2, 289, 192 / 2, exitLogoGh);

		drawInit = true;
	}

	// -- 描画処理 -- 
	// - 背景描画 -
	DrawRotaGraph(WIDTH / 2, HEIGHT / 2, 0.7, 0, optionBackImageGh, true, false);
	// - タイトルロゴ -
	DrawRotaGraph(100, 90, 0.7, 0, titleLogoGh, true, false);

	// - ロゴ表示 - 
	for (int i = 1; i < 3; i++) {
		DrawRotaGraph(100, 120 + i * 75, 0.7, 0, selectLogoGh[i - 1], true, false);
	}
	// Exitロゴ
	DrawRotaGraph(130, 500, 0.7, 0, exitLogoGh[0], true, false);

	// - 選択中のロゴを重ねて表示 -
	if (selectedLogo != 0) {
		switch (selectedLogo)
		{
		case BGM_SELECT:
			DrawRotaGraph(100, 120 + selectedLogo * 75, 0.7, 0, selectLogoGh[selectedLogo + (2 - 1)], true, false);
			break;
		
		case SE_SELECT:
			DrawRotaGraph(100, 120 + selectedLogo * 75, 0.7, 0, selectLogoGh[selectedLogo + (2 - 1)], true, false);
			break;
		case EXIT_SELECT:
			// Exitロゴ
			DrawRotaGraph(130, 500, 0.7, 0, exitLogoGh[1], true, false);
			break;
		}
		
	}
	


	// - 音量グラフの表示 -
	// BGMグラフ
	DrawExtendGraph(200, 173, 200 + (int)400 * (float)bgmVolumePercentage / 100.0, 173 + 40, barGh, true);
	DrawExtendGraph(200, 173, 600, 173+40, gageGh, true);
	// 数値の表示
	char buf[20];
	int num;
	num = sprintf(buf, "%d", bgmVolumePercentage);
	for (int i = 0; i < num; i++) {
		DrawGraph(625 + i * 20, 173, numGh[(buf[i] - '0')], true);
	}

	// SEグラフ
	DrawExtendGraph(200, 250, 200 + (int)400 * (float)seVolumePercentage / 100.0, 250 + 40, barGh, true);
	DrawExtendGraph(200, 250, 600, 250 + 40, gageGh, true);
	// 数値の表示
	num = sprintf(buf, "%d", seVolumePercentage);
	for (int i = 0; i < num; i++) {
		DrawGraph(625 + i * 20, 250, numGh[(buf[i] - '0')], true);
	}
	// ガイドアイコン表示
	DrawRotaGraph(530, 100, 0.7, 0, guideLogoSelectGh, true, false);
	DrawRotaGraph(730, 100, 0.7, 0, guideLogoAdjustGh, true, false);

}

void GameOption::Move() {
	// --- 動きの処理 ---

	// -- 項目の選択 --
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_DOWN)) {
		selectedLogo++;
		fs_choiceSE = true;
		if (selectedLogo > EXIT_SELECT) {
			selectedLogo = BGM_SELECT;
		}
	}
	else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_UP)) {
		selectedLogo--;
		fs_choiceSE = true;
		if (selectedLogo < BGM_SELECT) {
			selectedLogo = EXIT_SELECT;
		}

	}

	// -- 音量調整, Option終了処理 --
	switch (selectedLogo)
	{
	case BGM_SELECT:
		// - 左右キーで音量調節
		if (tnl::Input::IsKeyDown(eKeys::KB_RIGHT)) {
			bgmVolumePercentage++;
			fs_choiceSE = true;
			if (bgmVolumePercentage > 100) {
				bgmVolumePercentage = 100;
			}
		}
		else if (tnl::Input::IsKeyDown(eKeys::KB_LEFT)) {
			bgmVolumePercentage--;
			fs_choiceSE = true;
			if (bgmVolumePercentage < 0) {
				bgmVolumePercentage = 0;
			}
		}

		break;

	case SE_SELECT:
		// - 左右キーで音量調節
		if (tnl::Input::IsKeyDown(eKeys::KB_RIGHT)) {
			seVolumePercentage++;
			fs_choiceSE = true;
			if (seVolumePercentage > 100) {
				seVolumePercentage = 100;
			}
		}
		else if (tnl::Input::IsKeyDown(eKeys::KB_LEFT)) {
			seVolumePercentage--;
			fs_choiceSE = true;
			if (seVolumePercentage < 0) {
				seVolumePercentage = 0;
			}
		}

		break;

	case EXIT_SELECT:
		// エンター入力でオプション画面終了
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
			fs_decisionSE = true;
			// オプション終了関数実行
			Delete();
		}

	default:
		// - 何もしない -
		break;
	}


	// - ESCAPEでもOPTION終了 -
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_ESCAPE)) {
		fs_decisionSE = true;
		// オプション終了関数実行
		Delete();
	}

}

void GameOption::Delete() {
	// --- オプションクラス非表示フラグ ---
	selectedLogo = 0;
	exitFlag = true;
}

void GameOption::SoundAll() {
	// --- 音楽の管理 ---
	if (!soundInit) {
		// -- SEロード --
		hs_choiceSE = LoadSoundMem("sound/Title/TitleCursorSE.mp3");
		hs_decisionSE = LoadSoundMem("sound/Title/TitleDecisionSE.mp3");

		soundInit = true;
	}


	// -- SE再生 --
	// - 選択肢変更SE -
	if (fs_choiceSE) {
		int temp = (int)255 * (float)seVolumePercentage / 100.0;
		ChangeVolumeSoundMem((int)255 * (float)seVolumePercentage / 100.0, hs_choiceSE);
		PlaySoundMem(hs_choiceSE, DX_PLAYTYPE_BACK);
		fs_choiceSE = false;
	}
	// - 選択肢決定SE -
	if (fs_decisionSE) {
		ChangeVolumeSoundMem((int)255 * (float)seVolumePercentage / 100.0, hs_decisionSE);
		PlaySoundMem(hs_decisionSE, DX_PLAYTYPE_BACK);
		fs_decisionSE = false;
	}
}


void GameOption::SetOption() {
	// --- オプション呼び出しフラグ
	enable = true;
}

bool GameOption::GetVolumeFlag() {
	// --- 音量変更があったらtrue, 無かったfalseを返す ---
	if (bgmVolumePercentage != preBgmVolumePercentage ||
		seVolumePercentage != preSeVolumePercentage) {
		preBgmVolumePercentage = bgmVolumePercentage;
		preSeVolumePercentage = seVolumePercentage;
		return true;
	}
	else {
		return false;
	}

}

void GameOption::GetVolume(int* bgmVolume, int* seVolume) {
	// --- BGM&SE音量を参照渡し ---
	*bgmVolume = this->bgmVolumePercentage;
	*seVolume = this->seVolumePercentage;
}