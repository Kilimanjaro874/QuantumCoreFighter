#pragma once
#include "define.h"

class SelectCh
{
private:
	bool drawInit;		// �摜�����������t���O
	bool soundInit;		// �����ؑ֏����t���O
	int chBackGh;		// �w�i�p�O���t�B�b�N�n���h��
	int titleLogoGh;		// �^�C�g�����S�O���t�B�b�N�n���h��
	int selectLogoGh[TOTAL_CH];			// �`���v�^�[�������S�n���h��
	int selectComentaryGh[TOTAL_CH];	// �`���v�^�[����摜�n���h��
	int selectFrameGh;					// �`���v�^�[�I��g
	int guideLogo;			// ������@�����p���S
	int userChoice;						// ���[�U�[�I����
	bool fs_choiceSE;	// ���[�U�I�����ύX���ʉ��t���O 
	int hs_choiceSE;	// ���[�U�I�����ύX���ʉ��n���h��
	bool fs_decisionSE;	// ���[�U�I����������ʉ��t���O
	int hs_decisionSE;	// ���[�U�I����������ʉ��n���h��
	int bgmVolume;		// BGM����
	int seVolume;		// ���ʉ���
	int hb_chSelectBGM;	// �`���v�^�[�Z���N�gBGM�n���h��

private:
	void Draw();
	void Move();
	void SoundAll();
public:
	SelectCh();
	~SelectCh();
	int All();
	void SetSEVolume(int tempBGM, int tempSE);

};