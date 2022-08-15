#include "Item.h"
#include "define.h"
#include "../dxlib_ext/dxlib_ext.h"

int Item::hpItemGh[3];
int Item::powerItemGh[3];
int Item::defenseItemGh[3];
int Item::speedItemGh[3];
int Item::bombItemGh[3];

Item::Item() {
	// --- �R���X�g���N�^ ---
	type = 0;		// �A�C�e����ރZ�b�g��SetFlag�Ŏ��{
	existeFlag = false;
	drawInit = false;
	hpItemGh[3] = { 0 };
	powerItemGh[3] = { 0 };
	defenseItemGh[3] = { 0 };
	speedItemGh[3] = { 0 };
	bombItemGh[3] = { 0 };
	x = 0;
	y = 0;
	animFrame = 0;
	animCounter = 0;
	animChangeTime = 0.1;
	totalTime = 0;

}

void Item::All(float delta_time) {
	// ---�@�O������A�C�e���N���X���{ ---
	// -- ��ʔ͈͊O�Ɉړ�������Afalse��Ԃ�
	// (���́ADelete()�֐�����폜�����{����
	Move(delta_time);
	Draw();
	Time(delta_time);

}

void Item::Move(float delta_time) {
	// --- �ړ��n ---
	
	// -- �o�����Ԃ�1�b�ȓ��ł���΁A�A�C�e���㏸
	if (totalTime <= 1.0) {
		y -= SCROLL_SPEED * delta_time;
	}
	else {
		// - �w�i�̃X�N���[���X�s�[�h�ɍ��킹�Ĉړ� -
		y += SCROLL_SPEED * delta_time;
	}

	// -- ��ʊO�ɏo�����A�t���O��߂�
	if (y > HEIGHT) {
		Delete();
	}

}

void Item::Draw() {
	// --- �`��n ---
	if (!drawInit) {
		// -- �O���t�B�b�N�n���h���֎Q�Ɗi�[ --
		LoadDivGraph("graphics/Item/HpItem_3_516_154.png", 3, 3, 1, 516 / 3, 154, hpItemGh);
		LoadDivGraph("graphics/Item/PowerItem_10_768_202.png", 3, 3, 1, 768 / 3, 202, powerItemGh);
		LoadDivGraph("graphics/Item/DefenseItem_3_606_200.png", 3, 3, 1, 606 / 3, 200, defenseItemGh);
		LoadDivGraph("graphics/Item/SpeedItem_3_858_284.png", 3, 3, 1, 858 / 3, 284, speedItemGh);
		LoadDivGraph("graphics/Item/BombItem_3_861_284.png", 3, 3, 1, 861 / 3, 284, bombItemGh);

		drawInit = true;
	}

	// -- �`�� --
	// - �A�j���[�V�����ؑ� - 
	if (animCounter >= animChangeTime) {
		animCounter = 0;
		animFrame++;
		animFrame %= 3;
	}

	switch (type)
	{
	case ITEM_NONE:
		// �A�C�e������
		break;

	case ITEM_HP:
		// �񕜃A�C�e��
		DrawRotaGraph(x, y, 0.24, 0, hpItemGh[animFrame], true, false);

		break;

	case ITEM_POWER:
		// �p���[�A�b�v�A�C�e��
		DrawRotaGraph(x, y, 0.24, 0, powerItemGh[animFrame], true, false);
		
		break;

	case ITEM_DEFENSE:
		// �f�B�t�F���X�A�C�e��
		DrawRotaGraph(x, y, 0.23, 0, defenseItemGh[animFrame], true, false);

		break;

	case ITEM_SPEED:
		// �X�s�[�h�A�C�e��
		DrawRotaGraph(x, y, 0.23, 0, speedItemGh[animFrame], true, false);

		break;

	case ITEM_BOMB:
		// �{���A�C�e��
		DrawRotaGraph(x, y, 0.14, 0, bombItemGh[animFrame], true, false);

		break;

	}

}

void Item::Time(float delta_time) {
	// --- ���ԁ��t���O�Ǘ� ---
	animCounter += delta_time;
	if (existeFlag) {
		totalTime += delta_time;
	}
	else {
		totalTime = 0;
	}

}

void Item::Delete() {
	// --- �A�C�e�����Ńt���O ---
	totalTime = 0;
	existeFlag = false;
}

int  Item::GetType() {
	// --- �擾�����A�C�e���̎�ނ�Ԃ� ---
	return type;
}

bool Item::GetFlag() {
	// --- �����t���O��Ԃ� ---
	return existeFlag;
}

void Item::GetPosition(double* x, double *y) {
	// --- �A�C�e���̈ʒu���Q�Ɠn�� ---
	*x = this->x;
	*y = this->y;
}

void Item::SetFlag(double x, double y, int data) {
	// --- �C���X�^���X�ς݂̃A�C�e���̃t���O�𗧂Ă� ---
	// -- �A�C�e���̎�ނ�NONE�̎��ȊO�A�L���� --
	if (data != 0) {
		this->x = x;
		this->y = y;
		this->type = data;
		existeFlag = true;
	}
	else {
		// - �������Ȃ� -

	}
}
