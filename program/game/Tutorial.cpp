#include "Tutorial.h"
#include "../dxlib_ext/dxlib_ext.h"
#include "define.h"

Tutorial::Tutorial() {
	// --- �R���X�g���N�^ ---
	tutorialGh[5] = { 0 };
	count = 0;
	tutorialEndFlag = false;
	drawInit = false;
	soundInit = false;

}

bool Tutorial::All() {
	// --- �`���[�g���A���N���X���O�����瑀�� ---
	Move();
	Draw();
	return tutorialEndFlag;
}

void Tutorial::Draw() {
	// --- �`��n ---
	if (!drawInit) {
		// -- �`�揉������
		tutorialGh[0] = LoadGraph("graphics/Tutorial/Controls.png");
		tutorialGh[1] = LoadGraph("graphics/Tutorial/Technic1.png");
		tutorialGh[2] = LoadGraph("graphics/Tutorial/Technic2.png");
		tutorialGh[3] = LoadGraph("graphics/Tutorial/Technic3.png");
		tutorialGh[4] = LoadGraph("graphics/Tutorial/Technic4.png");

		drawInit = true;
	}
	
	// -- �`���[�g���A���`�� --
	DrawRotaGraph(WIDTH / 2, HEIGHT / 2, 1.0, 0, tutorialGh[count], true, false);

}

void Tutorial::Move() {
	// --- ����n ---

	// -- �`���[�g���A���̃J�E���g
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		count++;
	}
	if (count >= 5) {
		count = 4;
		tutorialEndFlag = true;
	}
}


