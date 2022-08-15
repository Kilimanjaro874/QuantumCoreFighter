#include "DeadEffect.h"
//#include <stdlib.h>	// rand()�ɕK�v
//#include <time.h>	// time()�ɕK�v
#include "../dxlib_ext/dxlib_ext.h"


DeadEffect::DeadEffect(	int type, 
						double x, 
						double y, 
						double velocityX, 
						double velocityY,
						float rateInitial,
						float rateMax, 
						float vanishingTime ) {
	// --- �R���X�g���N�^ ---
	this->type = type;
	this->x = x;
	this->y = y;
	this->velocityX = velocityX;			// �v���C���[ or �G�@�j�󎞂̑��x
	this->velocityY = velocityY;
	drawInit = false;
	gh[DEAD_EFFECT_FRAME_MAX] = { 0 };
	ghDivNum = 0;
	animFrame = 0;
	rad = 0;
	alpha = 0;
	rate = 0;
	this->rateInitial = rateInitial;
	this->rateMax = rateMax;
	elapsedTime = 0;
	animTime = 0;
	this->vanishingTime = vanishingTime;
	endFlag = false;
	srand((unsigned int)time(0));			// �����_���֐��̃V�[�h�l�𓾂�
}

bool  DeadEffect::All(float delta_time) {
	// --- �G�t�F�N�g
	// - ���x���䁕�G�t�F�N�g�Ǘ� -
	if (delta_time != 0) {
		Move(delta_time);
	}
	// - �`�� -
	Draw();
	// - ���ԊǗ� -
	TimeAndFlag(delta_time);
	// - ���Ńt���O��Ԃ� -
	return endFlag;
	
}

void DeadEffect::Move(float delta_time) {
	// --- �ړ��̏��� ---

	// -- �W���X�V���ړ��n -- 
	x += velocityX;
	y += velocityY;

	// - �����_���Ȋp�x�ۑ�(�摜�X���p) -
	if (elapsedTime == 0) {
		rad = rand() % (int)(2 * tnl::PI);
	}
	// - �g�嗦�ω� -
	if (rate <= rateMax) {
		rate = rateInitial + rateMax * (elapsedTime / vanishingTime);	// (rateInitial + 1~100%)
	}
	else {
		rate = rateMax;
	}
	// - ���ߗ��ω�(elapsedTime == vanishingTime�̎�0% -
	// 255 ~ 0 (elapsedTime == vanishingTime�̎�0%)
	alpha =(int)(255 * (vanishingTime - elapsedTime) * (1 / vanishingTime));

}


void DeadEffect::Draw() {
	// --- �`��n ---

	// -- ���������� --
	if (!drawInit) {
		// - type�ɉ����ē���̉摜�ǂݍ��� -

		switch (type)
		{
		case 1:
			// �f�b�h�G�t�F�N�g001
			LoadDivGraph("graphics/Effect/DeadEffect001_10_560_48.png", 10, 10, 1, 560/10, 48, gh);
			ghDivNum = 10 - 1;
			break;

		case 2:
			// �f�b�h�G�t�F�N�g002
			LoadDivGraph("graphics/Effect/DeadEffect002_8_576_65.png", 8, 8, 1, 576 / 8, 65, gh);
			ghDivNum = 8 - 1;
			break;

		default:
			break;
		}

		drawInit = true;
	}

	// -- �`�揈�� --
	
	// - ���Ԃɉ����ē���̉摜�`��
	if (vanishingTime / ghDivNum <= elapsedTime) {
		animTime = 0;
		animFrame++;
		if (animFrame >= ghDivNum)
			animFrame = ghDivNum;
	}

	// - ���ߓx�K�p - 
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	// - �摜�`�� -
	DrawRotaGraph(x, y, rate, rad, gh[animFrame], true);
	// - ���ߓx�K�p���Z�b�g -
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

}

void DeadEffect::TimeAndFlag(float delta_time) {
	// --- ���ԁ��t���O�̊Ǘ� ---
	elapsedTime += delta_time;
	animTime += delta_time;
	if (elapsedTime >= vanishingTime) {
		endFlag = true;
	}
	else {
		endFlag = false;
	}
}