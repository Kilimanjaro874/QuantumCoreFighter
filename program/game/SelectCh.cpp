#include"SelectCh.h"
#include "../dxlib_ext/dxlib_ext.h"

SelectCh::SelectCh() {
	// --- �R���X�g���N�^ ---
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
	// --- �f�X�g���N�^ ---
	// -- ���y��~ --
	if (CheckSoundMem(hb_chSelectBGM)) {
		StopSoundMem(hb_chSelectBGM);
	}
}

int SelectCh::All() {
	// --- �`���v�^�[�Z���N�g���� ---
	Move();
	Draw();
	SoundAll();

	return userChoice;
}

void SelectCh::Draw() {
	// --- �`��n ---
	if (!drawInit) {
		// -- �`�揉������ --
		chBackGh = LoadGraph("graphics/Title/TitleWindow.JPG");	// �^�C�g����ʂ𗬗p
		titleLogoGh = LoadGraph("graphics/ChapterSelect/ChapterSelectTitle.png");
		LoadDivGraph("graphics/ChapterSelect/ChapterSelection_6_300_270.png", 6, 1, 6, 300, 270 / 6, selectLogoGh);
		LoadDivGraph("graphics/ChapterSelect/SelectComentary_6_526_2268.png", 6, 1, 6, 526, 2268/6, selectComentaryGh);
		selectFrameGh = LoadGraph("graphics/ChapterSelect/ChapterSelector.png");	
		guideLogo = LoadGraph("graphics/ChapterSelect/ControlsLogo.png");

		drawInit = true;
	}

	// -- �`�� --
	// - �w�i�`�� -
	DrawRotaGraph(WIDTH / 2, HEIGHT / 2, 0.7, 0, chBackGh, true, false);
	// - �^�C�g�����S
	DrawRotaGraph(150, 90, 0.7, 0, titleLogoGh, true, false);
	// - �`���v�^�[����\�� -
	for (int i = 0; i < TOTAL_CH; i++) {
		DrawRotaGraph((WIDTH / 2) + 275, 150 + i * 75, 1.2, 0,  selectLogoGh[i], true, false);
		if (i == userChoice) {
			// �`���v�^�[����摜�̕\��
			DrawGraph(25, 150, selectComentaryGh[i], true);
			// �I��g�̕\��
			DrawRotaGraph((WIDTH / 2) + 275, 150 + i * 75, 1.2, 0,  selectFrameGh, true, false);
		}
	}
	// - ������@�̃K�C�h�\��
	DrawRotaGraph(800, 625, 1.0, 0, guideLogo, true, false);

}

void SelectCh::Move() {
	// --- ����n ---

	// -- ���ځF�`���v�^�[�̑I�� -- 
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

	// ���艹���Đ�
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		fs_decisionSE = true;
	}

}

void SelectCh::SoundAll() {
	// --- ���y�n�Ǘ� ---
	if (!soundInit) {
		// -- SE���[�h --
		hb_chSelectBGM = LoadSoundMem("sound/ChapterSelect/ChapterSelectBGM.mp3");
		hs_choiceSE = LoadSoundMem("sound/Title/TitleCursorSE.mp3");
		hs_decisionSE = LoadSoundMem("sound/Title/TitleDecisionSE.mp3");
		// BGM �Đ�
		PlaySoundMem(hb_chSelectBGM, DX_PLAYTYPE_LOOP);

		soundInit = true;
	}

	// -- SE�Đ� --
	// - �I�����ύXSE - 
	if (fs_choiceSE) {
		PlaySoundMem(hs_choiceSE, DX_PLAYTYPE_BACK);

		fs_choiceSE = false;
	}


	// - �I��������SE -
	if (fs_decisionSE) {
		PlaySoundMem(hs_decisionSE, DX_PLAYTYPE_BACK);

		fs_decisionSE = false;
	}

}

void SelectCh::SetSEVolume(int tempBGM, int tempSE) {
	// --- SE�̉��ʂ��Z�b�g ---
	this->bgmVolume = tempBGM;
	this->seVolume = tempSE;
	// -- �e�n���h���ɉ��ʂ�K�������� --
	int tempBGMVol = (int)255 * (float)bgmVolume / 100.0;
	int tempSEVol = (int)255 * (float)seVolume / 100.0;
	// - BGM -
	ChangeVolumeSoundMem(tempBGMVol, hb_chSelectBGM);
	// - SE -
	ChangeVolumeSoundMem(tempSEVol, hs_choiceSE);
	ChangeVolumeSoundMem(tempSEVol, hs_decisionSE);

}