#pragma once

class GameOption 
{
private:
	bool drawInit;			// �摜�����������t���O
	bool soundInit;			// �����������t���O
	bool enable;			// �I�v�V�����L���t���O
	bool exitFlag;			// �I�v�V�����I���t���O
	int optionBackImageGh;	// �I�v�V�����w�i�n���h��
	int titleLogoGh;		// �I�v�V�����^�C�g�����S�n���h��
	int selectLogoGh[4];	// �I�����\�����S
	int gageGh;				// �{�����[���Q�[�W
	int barGh;				// �{�����[���o�[
	int numGh[10];			// �{�����[�����\���p
	int guideLogoSelectGh;	// �K�C�h���S�n���h��
	int guideLogoAdjustGh;	// �K�C�h���S�n���h��
	int exitLogoGh[2];		// exit���S�n���h��
	int selectedLogo;		// �I�𒆂̃��S
	int userChoice;			// ���[�U�̑I�����i�[
	int bgmVolumePercentage;	// BGM����(0-100%)
	int seVolumePercentage;		// SE����(0-100%)
	int preBgmVolumePercentage;	// 1�t���[���O��BGM����
	int preSeVolumePercentage;	// 1�t���[���O��SE����

	bool fs_choiceSE;		// ���[�U�I�����ύX���ʉ��t���O
	int hs_choiceSE;		// ���[�U�I�����ύX���ʉ��n���h��
	bool fs_decisionSE;		// ���[�U�I����������ʉ��t���O
	int hs_decisionSE;		// ���[�U�I����������ʉ��n���h��

private:
	void Draw();
	void Move();
	void Delete();
	void SoundAll();
	//void ChangeScrennSize();	// �o�O����. �]�T������Ύ�������
public:
	GameOption();
	bool All();
	void SetOption();
	bool GetVolumeFlag();
	void GetVolume(int* bgmVolume, int* seVolume);

};