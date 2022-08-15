#include "sceneDefine.h"
#include "../dxlib_ext/dxlib_ext.h"

#include "SceneManagerOpening.h"
#include "SceneManagerTitle.h"
#include "SelectCh.h"
#include "SceneManagerCh1.h"
#include "GameOption.h"

bool sceneInit = false;			// �V�[���������t���O
int tempScene = 0;				// �b��̃V�[�����ł̏�������p�ϐ�
// 
int choiceChapter = 0;			// ���[�U�̃`���v�^�[�Z���N�g���ʂ��i�[

// -- ����N���X������(���ӁF�K��endGame()�ō폜���鎖) -- 

// - Opening -
SceneManagerOpening* sceneManagerOpening = nullptr;
// - Title -
SceneManagerTitle* sceneManagerTitle = nullptr;
// - ChapterSelect -
SelectCh* selectCh;
// - Ch. 1 - 
SceneManagerCh1* sceneManagerCh1 = nullptr;

// - �I�v�V�����N���X(�ǁX�B�ꉻ������) -
GameOption gameOption;

// -- �Q�[���I�����A�C���X�^���X��j�󂷂�֐� -- 
void endGame() {

	// - Opening -
	if(sceneManagerOpening != NULL)
		delete sceneManagerOpening;
	// - Title - 
	if (sceneManagerTitle != NULL)
		delete sceneManagerTitle;
	// - Chapter Select -
	if (selectCh != NULL) {
		delete selectCh;
	}
	// - Ch.1 - 
	if (sceneManagerCh1 != NULL) {
		delete sceneManagerCh1;
	}

}


void sceneOpening(float delta_time) {
	// -- �I�[�v�j���O��� -- 
	// - �V�[���Ǘ��N���X�̃C���X�^���X�쐬 -
	if (!sceneInit) {
		sceneManagerOpening = new SceneManagerOpening();
		sceneInit = true;
	}

	// - �I�[�v�j���O�N���X���s -
	// �I���t���O�ɉ����ăV�[���ؑ�
	if (!sceneManagerOpening->All(delta_time)) {
		// �^�C�g���Ɉړ�
		scene = SCENE_TITLE;
		endSceneOp();
	}

	// - �G���^�[���͂ŃI�[�v�j���O�X�L�b�v - 
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		scene = SCENE_TITLE;
		endSceneOp();
	}
	
}


void sceneTitle(float delta_time) {

	// -- �^�C�g����� --
	if (!sceneInit) {
		sceneManagerTitle = new SceneManagerTitle();
		sceneInit = true;
		tempScene = SCENE_TITLE;
	}
	
	// -- �V�[���N���X���s --
	sceneManagerTitle->All(delta_time);
	// - BGM, SE���ʕύX -
	if (gameOption.GetVolumeFlag()) {
		int tempBgm, tempSe;
		gameOption.GetVolume(&tempBgm, &tempSe);
		sceneManagerTitle->SetVolume(tempBgm, tempSe);
	}

	// -- tempScene�ɉ����đI�������� --
	switch (tempScene)
	{
	case SCENE_TITLE:
		// �^�C�g���V�[�� 
		// - �I�����ɉ����ăV�[���ؑ�(Enter or Space ����) -
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN) || tnl::Input::IsKeyDownTrigger(eKeys::KB_SPACE)) {
			tempScene = sceneManagerTitle->GetUserChoice();
		}

		break;

	case SCENE_CH_SELECT:
		// �`���v�^�[�Z���N�g�Ɉړ�
		scene = SCENE_CH_SELECT;
		endSceneTitle();
		break;

	case SCENE_OPTION:
		// �V�[���J�ڂ͍s�킸�A�Q�[���I�v�V�������J��
		if (!gameOption.All()) {

		}
		else {
			tempScene = SCENE_TITLE;
		}

		break;

	case END_GAME:
		// �Q�[���I��
		scene = END_GAME;
		break;

	default:
		// �G���[
		break;
	}
	
}


void sceneChSelect() {
	// - �`���v�^�[�Z���N�g��ʂ���e�`���v�^�[�� - 
	if (!sceneInit) {
		selectCh = new SelectCh();
		//  - BGM, SE���ʂ�K�p -
		selectCh->All();
		int tempSE, tempBGM;
		gameOption.GetVolume(&tempBGM, &tempSE);
		selectCh->SetSEVolume(tempBGM, tempSE);

		sceneInit = true;
	}

	// - �`���v�^�[�Z���N�g�����s
	selectCh->All();
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		// �I�������`���v�^�[���i�[
		choiceChapter = selectCh->All();
		scene = SCENE_CH_1;
		endSceneChSelect();
	}

}


// -- �`���v�^�[1�v���C�֐� --
// - Start -
void scenePlayCh1(float delta_time) {

	// - choiceChapter�̌��ʂɉ����ēǂݍ��ޔw�i�E�X�|�[������G�@���X�g�ω�
	if (!sceneInit) {
		char backName[NAME_SIZE];
		char effect1[NAME_SIZE];
		char effect2[NAME_SIZE];

		switch (choiceChapter)
		{
		case 0:

			strcpy(backName, "graphics/Back/City2.png");
			strcpy(effect1, "graphics/Back/wave.png");
			strcpy(effect2, "graphics/Back/Cloud2.png");

			break;

		case 1:
			strcpy(backName, "graphics/Back/Sea.png");
			strcpy(effect1, "graphics/Back/wave.png");
			strcpy(effect2, "graphics/Back/Cloud2.png");

			break;

		case 2:
			strcpy(backName, "graphics/Back/Sea.png");
			strcpy(effect1, "graphics/Back/wave.png");
			strcpy(effect2, "graphics/Back/Cloud2.png");

			break;

		case 4:
			strcpy(backName, "graphics/Back/Sea.png");
			strcpy(effect1, "graphics/Back/wave.png");
			strcpy(effect2, "graphics/Back/Cloud2.png");

			break;

		case 5:
			strcpy(backName, "graphics/Back/Sea.png");
			strcpy(effect1, "graphics/Back/wave.png");
			strcpy(effect2, "graphics/Back/Cloud2.png");

			break;

		default:
			break;
		}

		sceneManagerCh1->All(delta_time);				// ��x�`�恕������������������������
		// - BGM,SE���ʂ�K�p
		int tempBGM, tempSE;
		gameOption.GetVolume(&tempBGM, &tempSE);
		sceneManagerCh1->SetVolume(tempBGM, tempSE);

		sceneInit = true;
	}


	//// - �N���X�̃C���X�^���X���쐬 - 
	//if (!sceneInit) {
	//	sceneManagerCh1 = new SceneManagerCh1();
	//	sceneManagerCh1->All(delta_time);				// ��x�`�恕������������������������
	//	// - BGM,SE���ʂ�K�p
	//	int tempBGM, tempSE;
	//	gameOption.GetVolume(&tempBGM, &tempSE);
	//	sceneManagerCh1->SetVolume(tempBGM, tempSE);

	//	sceneInit = true;
	//}

	// - Ch. 1���Đ� - 
	if (tempScene != SCENE_OPTION) {
		tempScene = sceneManagerCh1->All(delta_time);
	}

	// -- tempScene�ɉ����đI�������� --
	switch (tempScene)
	{
	case SCENE_NON_CHANGE:
		// �������Ȃ�

		break;

	case SCENE_CH_SELECT:
		// �`���v�^�[�Z���N�g�Ɉړ�
		scene = SCENE_CH_SELECT;
		endScenePlayCh1();
		break;

	case SCENE_TITLE:
		// �^�C�g����ʂɈړ�
		scene = SCENE_TITLE;
		endScenePlayCh1();
		break;

	case SCENE_OPTION:
		// �V�[���J�ڂ͍s�킸�A�Q�[���I�v�V�������J��
		if (!gameOption.All()) {
			tempScene = SCENE_OPTION;
			// ���ʐݒ�𔽉f����
			int tempBGM, tempSE;
			gameOption.GetVolume(&tempBGM, &tempSE);
			sceneManagerCh1->SetVolume(tempBGM, tempSE);

		}
		else {
			tempScene = SCENE_NON_CHANGE;
		}

		break;

	case SCENE_CH_1:
		// ��Ԃ����������čăg���C
		endScenePlayCh1();

		break;

	case END_GAME:
		// �Q�[���I��
		scene = END_GAME;
		break;

	default:
		// �G���[
		break;
	}


	
}
// -- End -- 
// �V�[���N���X��j��
void endSceneOp() {
	sceneInit = false;
	delete sceneManagerOpening;
	sceneManagerOpening = nullptr;
}

void endSceneTitle() {
	sceneInit = false;
	delete sceneManagerTitle;
	sceneManagerTitle = nullptr;
}

void endSceneChSelect() {
	sceneInit = false;
	delete selectCh;
	selectCh = nullptr;
}

void endScenePlayCh1() {
	sceneInit = false;
	delete sceneManagerCh1;
	sceneManagerCh1 = nullptr;
}


