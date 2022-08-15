#include "SceneManagerTitle.h"
#include "../dxlib_ext/dxlib_ext.h"
#include "define.h"
#include "sceneDefine.h"

#define CHAPTER_SELECT 1	// �`���v�^�[�Z���N�g
#define OPTION_SELECT 2		// �I�v�V�����Z���N�g
#define END_SELECT 3		// �G���h�Z���N�g



SceneManagerTitle::SceneManagerTitle() {
	// --- �R���X�g���N�^ ---
	drawInit = false;
	soundInit = false;
	titleBackImageGh = 0;
	titleLogoImageGh = 0;
	selectedLogo = CHAPTER_SELECT;
	userChoice = 0;		
	hb_titleSound = 0;
	fs_choiceSE = false;
	hs_choiceSE = 0;
	fs_decisionSE = false;
	hs_decisionSE = 0;
	bgmVolume = BGM_INITIAL_VOLUME;
	seVolume = SE_INITIAL_VOLUME;


}

SceneManagerTitle::~SceneManagerTitle() {
	// --- �f�X�g���N�^ ---
	
	// -- ���y��~ --
	if (CheckSoundMem(hb_titleSound)) {
		StopSoundMem(hb_titleSound);
	}
}

bool SceneManagerTitle::All(float delta_time) {
	
	// --- �^�C�g���N���X���� ---
	Move(delta_time);
	Draw();
	SoundAll();
	return true;

	
	
}

void SceneManagerTitle::Draw() {
	// --- �`��n���� ---
	// 
	// -- �`�揉������ --
	if (!drawInit) {
		titleBackImageGh = LoadGraph("graphics/Title/TitleWindow.JPG");
		titleLogoImageGh = LoadGraph("graphics/Title/TitleLogo.png");
		LoadDivGraph("graphics/Title/SelectLogo_6_354_528.png", 6, 1, 6, 354, 528 / 6, selectLogoImageGh);
		drawInit = true;
	}

	// -- �`�揈�� --
	// - �w�i�`�� -
	DrawRotaGraph(WIDTH / 2, HEIGHT / 2, 0.7, 0, titleBackImageGh, true, false);
	// - �^�C�g�����S - 
	DrawRotaGraph(750,200, 0.5, 0, titleLogoImageGh, true, false);
	// - ���S�\��
	for (int i = 1; i < 4; i++) {
		DrawRotaGraph(800, 225 + i * 75, 0.7, 0, selectLogoImageGh[i-1], true, false);
	}
	// - �I�𒆂̃��S�\��
	if (selectedLogo != 0) {
		DrawRotaGraph(800, 225 + selectedLogo * 75, 0.7, 0, selectLogoImageGh[selectedLogo + (3 - 1)], true, false);
	}
}

void SceneManagerTitle::Move(float delta_time) {
	// --- �����̏��� ---
	// -- ���S�Z���N�g --
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_DOWN)) {
		selectedLogo++;
		fs_choiceSE = true;
		if (selectedLogo > END_SELECT) {
			selectedLogo = CHAPTER_SELECT;
		}
	}
	else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_UP)) {
		selectedLogo--;
		fs_choiceSE = true;
		if (selectedLogo < CHAPTER_SELECT) {
			selectedLogo = END_SELECT;
		}
	}
	
	// -- ���S�Z���N�g�ɉ����đI��������(sceneDefine.h�ƑΉ��t��)�@--
	switch (selectedLogo)
	{
	case CHAPTER_SELECT:
		userChoice = SCENE_CH_SELECT;

		break;
	
	case OPTION_SELECT:
		userChoice = SCENE_OPTION;

		break;
	case END_SELECT:
		userChoice = END_GAME;

		break;
	default:
		userChoice = SCENE_TITLE;
		break;
	}

}

void SceneManagerTitle::SoundAll() {
	// --- ���y�̊Ǘ� ---
	if (!soundInit) {
		// -- BGM���[�h���Đ� --
		hb_titleSound = LoadSoundMem("sound/Title/TitleBGM.mp3");
		PlaySoundMem(hb_titleSound, DX_PLAYTYPE_LOOP);
		// -- SE���[�h --
		hs_choiceSE = LoadSoundMem("sound/Title/TitleCursorSE.mp3");
		hs_decisionSE = LoadSoundMem("sound/Title/TitleDecisionSE.mp3");

		soundInit = true;
	}
	// -- BGM���ʕύX --
	ChangeVolumeSoundMem((int)255 * (float)bgmVolume / 100.0, hb_titleSound);

	// -- SE�Đ� --
	// - �I�����ύXSE -
	if (fs_choiceSE) {
		ChangeVolumeSoundMem((int)255 * (float)seVolume / 100.0, hs_choiceSE);
		PlaySoundMem(hs_choiceSE, DX_PLAYTYPE_BACK);
		fs_choiceSE = false;
	}
	// - �I��������SE -
	if (fs_decisionSE) {
		ChangeVolumeSoundMem((int)255 * (float)seVolume / 100.0, hs_decisionSE);
		PlaySoundMem(hs_decisionSE, DX_PLAYTYPE_BACK);
		fs_decisionSE = false;
	}

}

int SceneManagerTitle::GetUserChoice() {
	// --- ���[�U�̑I������Ԃ� ---
	// -- ���̎��A���艹SE������ --
	fs_decisionSE = true;
	return userChoice;
}

void SceneManagerTitle::SetVolume(int bgmVolume, int seVolume) {
	// --- BGM�����ʂ��Q�Ɠn�� ---
	this->bgmVolume = bgmVolume;
	this->seVolume = seVolume;
}