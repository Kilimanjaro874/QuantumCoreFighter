#include"SelectCh.h"
#include "../dxlib_ext/dxlib_ext.h"

SelectCh::SelectCh() {
	// --- コンストラクタ ---
	drawInit = false;
	soundInit = false;
	chBackGh = 0;
	titleLogoGh = 0;
	selectLogoGh[TOTAL_CH] = { 0 };
	selectComentaryGh[TOTAL_CH] = { 0 };
	selectFrameGh = 0;
	guideLogo = 0;
	userChoice = 0;
	fs_choiceSE = false;
	hs_choiceSE = 0;
	fs_decisionSE = false;
	hs_decisionSE = 0;
	bgmVolume = 0;
	seVolume = 0;
	hb_chSelectBGM = 0;
}

SelectCh::~SelectCh() {
	// --- デストラクタ ---
	// -- 音楽停止 --
	if (CheckSoundMem(hb_chSelectBGM)) {
		StopSoundMem(hb_chSelectBGM);
	}
}

int SelectCh::All() {
	// --- チャプターセレクト操作 ---
	Move();
	Draw();
	SoundAll();

	return userChoice;
}

void SelectCh::Draw() {
	// --- 描画系 ---
	if (!drawInit) {
		// -- 描画初期処理 --
		chBackGh = LoadGraph("graphics/Title/TitleWindow.JPG");	// タイトル画面を流用
		titleLogoGh = LoadGraph("graphics/ChapterSelect/ChapterSelectTitle.png");
		LoadDivGraph("graphics/ChapterSelect/ChapterSelection_6_300_270.png", 6, 1, 6, 300, 270 / 6, selectLogoGh);
		LoadDivGraph("graphics/ChapterSelect/SelectComentary_6_526_2268.png", 6, 1, 6, 526, 2268/6, selectComentaryGh);
		selectFrameGh = LoadGraph("graphics/ChapterSelect/ChapterSelector.png");	
		guideLogo = LoadGraph("graphics/ChapterSelect/ControlsLogo.png");

		drawInit = true;
	}

	// -- 描画 --
	// - 背景描画 -
	DrawRotaGraph(WIDTH / 2, HEIGHT / 2, 0.7, 0, chBackGh, true, false);
	// - タイトルロゴ
	DrawRotaGraph(150, 90, 0.7, 0, titleLogoGh, true, false);
	// - チャプター解説表示 -
	for (int i = 0; i < TOTAL_CH; i++) {
		DrawRotaGraph((WIDTH / 2) + 275, 150 + i * 75, 1.2, 0,  selectLogoGh[i], true, false);
		if (i == userChoice) {
			// チャプター解説画像の表示
			DrawGraph(25, 150, selectComentaryGh[i], true);
			// 選択枠の表示
			DrawRotaGraph((WIDTH / 2) + 275, 150 + i * 75, 1.2, 0,  selectFrameGh, true, false);
		}
	}
	// - 操作方法のガイド表示
	DrawRotaGraph(800, 625, 1.0, 0, guideLogo, true, false);

}

void SelectCh::Move() {
	// --- 動作系 ---

	// -- 項目：チャプターの選択 -- 
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_DOWN)) {
		userChoice++;
		fs_choiceSE = true;
		if (userChoice >= TOTAL_CH) {
			userChoice = 0;
		}

	}
	else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_UP)) {
		userChoice--;
		fs_choiceSE = true;
		if (userChoice < 0) {
			userChoice = TOTAL_CH - 1;
		}
	}

	// 決定音を再生
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		fs_decisionSE = true;
	}

}

void SelectCh::SoundAll() {
	// --- 音楽系管理 ---
	if (!soundInit) {
		// -- SEロード --
		hb_chSelectBGM = LoadSoundMem("sound/ChapterSelect/ChapterSelectBGM.mp3");
		hs_choiceSE = LoadSoundMem("sound/Title/TitleCursorSE.mp3");
		hs_decisionSE = LoadSoundMem("sound/Title/TitleDecisionSE.mp3");
		// BGM 再生
		PlaySoundMem(hb_chSelectBGM, DX_PLAYTYPE_LOOP);

		soundInit = true;
	}

	// -- SE再生 --
	// - 選択肢変更SE - 
	if (fs_choiceSE) {
		PlaySoundMem(hs_choiceSE, DX_PLAYTYPE_BACK);

		fs_choiceSE = false;
	}


	// - 選択肢決定SE -
	if (fs_decisionSE) {
		PlaySoundMem(hs_decisionSE, DX_PLAYTYPE_BACK);

		fs_decisionSE = false;
	}

}

void SelectCh::SetSEVolume(int tempBGM, int tempSE) {
	// --- SEの音量をセット ---
	this->bgmVolume = tempBGM;
	this->seVolume = tempSE;
	// -- 各ハンドラに音量を適応させる --
	int tempBGMVol = (int)255 * (float)bgmVolume / 100.0;
	int tempSEVol = (int)255 * (float)seVolume / 100.0;
	// - BGM -
	ChangeVolumeSoundMem(tempBGMVol, hb_chSelectBGM);
	// - SE -
	ChangeVolumeSoundMem(tempSEVol, hs_choiceSE);
	ChangeVolumeSoundMem(tempSEVol, hs_decisionSE);

}