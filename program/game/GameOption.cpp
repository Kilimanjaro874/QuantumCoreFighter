#include "GameOption.h"
#include "../dxlib_ext/dxlib_ext.h"
#include "define.h"

// �Z���N�^�[
#define BGM_SELECT 1
#define SE_SELECT 2 
#define EXIT_SELECT 3

GameOption::GameOption() {
	// --- �R���X�g���N�^ ---
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
	// --- �I�v�V�����N���X���� ---
	exitFlag = false;
	Move();
	Draw();
	SoundAll();
	return exitFlag;
}

void GameOption::Draw() {
	// --- �`��n���� ---
	// -- �`�揉������ --
	if (!drawInit) {
		optionBackImageGh = LoadGraph("graphics/Option/OptionWindow.jpg");
		titleLogoGh = LoadGraph("graphics/Option/OptionTitle_1_281_136.png");
		LoadDivGraph("graphics/Option/OptionLogo_4_196_384.png", 4, 1, 4, 196, 384 / 4, selectLogoGh);
		gageGh = LoadGraph("graphics/HpGageBar/hpGreenGage.png");	// HP�Q�[�W�𗬗p
		barGh = LoadGraph("graphics/HpGageBar/hpBarGreen.png");		// HP�o�[�𗬗p
		LoadDivGraph("graphics/Score/ScoreNumber_10_300_38.png", 10, 10, 1, 300 / 10, 38, numGh);	// �X�R�A���𗬗p
		guideLogoSelectGh = LoadGraph("graphics/Option/OptionLogoSelect_1_267_135.png");
		guideLogoAdjustGh = LoadGraph("graphics/Option/OptionLogoAdjust_1_267_135.png");
		LoadDivGraph("graphics/Option/OptionLogoExit_2_289_192.png", 2, 1, 2, 289, 192 / 2, exitLogoGh);

		drawInit = true;
	}

	// -- �`�揈�� -- 
	// - �w�i�`�� -
	DrawRotaGraph(WIDTH / 2, HEIGHT / 2, 0.7, 0, optionBackImageGh, true, false);
	// - �^�C�g�����S -
	DrawRotaGraph(100, 90, 0.7, 0, titleLogoGh, true, false);

	// - ���S�\�� - 
	for (int i = 1; i < 3; i++) {
		DrawRotaGraph(100, 120 + i * 75, 0.7, 0, selectLogoGh[i - 1], true, false);
	}
	// Exit���S
	DrawRotaGraph(130, 500, 0.7, 0, exitLogoGh[0], true, false);

	// - �I�𒆂̃��S���d�˂ĕ\�� -
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
			// Exit���S
			DrawRotaGraph(130, 500, 0.7, 0, exitLogoGh[1], true, false);
			break;
		}
		
	}
	


	// - ���ʃO���t�̕\�� -
	// BGM�O���t
	DrawExtendGraph(200, 173, 200 + (int)400 * (float)bgmVolumePercentage / 100.0, 173 + 40, barGh, true);
	DrawExtendGraph(200, 173, 600, 173+40, gageGh, true);
	// ���l�̕\��
	char buf[20];
	int num;
	num = sprintf(buf, "%d", bgmVolumePercentage);
	for (int i = 0; i < num; i++) {
		DrawGraph(625 + i * 20, 173, numGh[(buf[i] - '0')], true);
	}

	// SE�O���t
	DrawExtendGraph(200, 250, 200 + (int)400 * (float)seVolumePercentage / 100.0, 250 + 40, barGh, true);
	DrawExtendGraph(200, 250, 600, 250 + 40, gageGh, true);
	// ���l�̕\��
	num = sprintf(buf, "%d", seVolumePercentage);
	for (int i = 0; i < num; i++) {
		DrawGraph(625 + i * 20, 250, numGh[(buf[i] - '0')], true);
	}
	// �K�C�h�A�C�R���\��
	DrawRotaGraph(530, 100, 0.7, 0, guideLogoSelectGh, true, false);
	DrawRotaGraph(730, 100, 0.7, 0, guideLogoAdjustGh, true, false);

}

void GameOption::Move() {
	// --- �����̏��� ---

	// -- ���ڂ̑I�� --
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

	// -- ���ʒ���, Option�I������ --
	switch (selectedLogo)
	{
	case BGM_SELECT:
		// - ���E�L�[�ŉ��ʒ���
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
		// - ���E�L�[�ŉ��ʒ���
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
		// �G���^�[���͂ŃI�v�V������ʏI��
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
			fs_decisionSE = true;
			// �I�v�V�����I���֐����s
			Delete();
		}

	default:
		// - �������Ȃ� -
		break;
	}


	// - ESCAPE�ł�OPTION�I�� -
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_ESCAPE)) {
		fs_decisionSE = true;
		// �I�v�V�����I���֐����s
		Delete();
	}

}

void GameOption::Delete() {
	// --- �I�v�V�����N���X��\���t���O ---
	selectedLogo = 0;
	exitFlag = true;
}

void GameOption::SoundAll() {
	// --- ���y�̊Ǘ� ---
	if (!soundInit) {
		// -- SE���[�h --
		hs_choiceSE = LoadSoundMem("sound/Title/TitleCursorSE.mp3");
		hs_decisionSE = LoadSoundMem("sound/Title/TitleDecisionSE.mp3");

		soundInit = true;
	}


	// -- SE�Đ� --
	// - �I�����ύXSE -
	if (fs_choiceSE) {
		int temp = (int)255 * (float)seVolumePercentage / 100.0;
		ChangeVolumeSoundMem((int)255 * (float)seVolumePercentage / 100.0, hs_choiceSE);
		PlaySoundMem(hs_choiceSE, DX_PLAYTYPE_BACK);
		fs_choiceSE = false;
	}
	// - �I��������SE -
	if (fs_decisionSE) {
		ChangeVolumeSoundMem((int)255 * (float)seVolumePercentage / 100.0, hs_decisionSE);
		PlaySoundMem(hs_decisionSE, DX_PLAYTYPE_BACK);
		fs_decisionSE = false;
	}
}


void GameOption::SetOption() {
	// --- �I�v�V�����Ăяo���t���O
	enable = true;
}

bool GameOption::GetVolumeFlag() {
	// --- ���ʕύX����������true, ��������false��Ԃ� ---
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
	// --- BGM&SE���ʂ��Q�Ɠn�� ---
	*bgmVolume = this->bgmVolumePercentage;
	*seVolume = this->seVolumePercentage;
}