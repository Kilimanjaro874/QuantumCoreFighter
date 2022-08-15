#pragma once
#include "define.h"

class Back
{
private:
	static const int backNum = 3;
	double x[backNum], y[backNum];		// ���W
	int backGh[backNum];		// �O���t�B�b�N�n���h��
	bool drawInit;		// �`�揉�������t���O
	int width[backNum];			// �擾�w�i�̉���
	int height[backNum];			// �擾�w�i�̏c��
	bool xScrool[backNum];			// x�����̉摜�����E�ɓ��삳���邽�߂̃t���O
	char backName[NAME_SIZE];		// �w�i��
	char effect1[NAME_SIZE];		// �G�t�F�N�g��1
	char effect2[NAME_SIZE];		// �G�t�F�N�g��2

private:
	void Draw();					// �w�i�`��
	void Move(float delta_time);	// �w�i�X�N���[��

public:
	void All(float delta_time);
	Back(char back[NAME_SIZE], char effect1[NAME_SIZE], char effect2[NAME_SIZE]);

};