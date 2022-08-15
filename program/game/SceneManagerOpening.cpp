#include "SceneManagerOpening.h"
#include "define.h"
#include "../dxlib_ext/dxlib_ext.h"

SceneManagerOpening::SceneManagerOpening() {
	// --- �R���X�g���N�^ ---
	totalTime = 0;
	animCount = 0;
	animFrame = 0;
	drawInit = false;
	opImageGh[7] = { 0 };

}

bool SceneManagerOpening::All(float delta_time) {
	// --- �I�[�v�j���O�N���X���s ---
	Draw();
	Time(delta_time);

	// -- 5�b�ȏ㗧������V�[���I�� --
	if (totalTime >= 6) {
		return false;
	}
	else {
		return true;
	}

}

SceneManagerOpening::~SceneManagerOpening() {

}

void SceneManagerOpening::Draw() {
	// --- �`�揈�� ---
	if (!drawInit) {
		// -- ���������� --
		LoadDivGraph("graphics/Opening/FlogAnimation_7_1862_271.png", 7, 7, 1, 1862 / 7, 271, opImageGh);
		opMessageGh[0] = LoadGraph("graphics/Opening/TitleMessage1.png");
		opMessageGh[1] = LoadGraph("graphics/Opening/TitleLogo.png");

		drawInit = true;
	}
	// -- �`�� --
	
	// �I�[�v�j���O�\��(
	if (totalTime >= 1.5) {
		// �����\��
		DrawRotaGraph(WIDTH / 2, HEIGHT * 20 / 100, 0.4, 0, opMessageGh[0], true, false);
		
		// �A�j���[�V�����Đ�
		if (animCount >= 0.3) {
			animCount = 0;
			animFrame++;
			animFrame %= 7;
		}
		DrawRotaGraph(WIDTH / 2, HEIGHT * 45 / 100, 0.9, 0, opImageGh[animFrame], true, false);
		// ���S�\��
		DrawRotaGraph(WIDTH / 2, HEIGHT * 70 / 100, 0.6, 0, opMessageGh[1], true, false);
	}

}

void SceneManagerOpening::Time(float delta_time) {
	// --- ���ԊǗ� ----
	totalTime += delta_time;
	animCount += delta_time;

}
