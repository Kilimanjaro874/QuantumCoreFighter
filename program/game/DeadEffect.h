#pragma once

#include "define.h"

class DeadEffect 
{
private:
	int type;			// �\������G�t�F�N�g�̎��
	double x, y;		// ���W
	double velocityX;	// ���x
	double velocityY;
	bool drawInit;		// �`�揉��������
	int gh[DEAD_EFFECT_FRAME_MAX];	// �O���t�B�b�N�n���h���ő吔
	int ghDivNum;		// ���ۂ̃O���t�B�b�N�n���h���������i�[
	int animFrame;		// �A�j���[�V�����t���[��
	float rad;			// �摜��]
	int alpha;			// �摜�����x
	float rate;			// �摜�g�嗦
	float rateInitial;	// �摜�g�嗦�����l
	float rateMax;		// �摜�g�嗦���
	float elapsedTime;	// ��������̌o�ߎ��Ԋi�[
	float animTime;		// �O���t�B�b�N�n���h���ؑփ^�C�~���O�Ǘ�
	float vanishingTime;	// �G�t�F�N�g����������ł���܂ł̎���
	bool endFlag;		// �j��t���O

private:
	void Move(float delta_time);
	void Draw();
	void TimeAndFlag(float delta_time);

public:
	DeadEffect(	int type, double x, 
				double y, 
				double velocityX, 
				double velocityY,
				float rateInitial,
				float rateMax,
				float vanishingTime);
	bool All(float delta_time);

};