#pragma once

class SceneManagerTitle
{
private:
	bool drawInit;			// �摜�����������t���O
	bool soundInit;			// �������������t���O
	int titleBackImageGh;	// �^�C�g���w�i
	int titleLogoImageGh;	// �^�C�g�����S
	int selectLogoImageGh[6];		// �I�����\��
	int selectedLogo;		// �I�𒆂̃��S
	int userChoice;			// ���[�U-�̑I����
	int hb_titleSound;		// �^�C�g��BGM�n���h��
	bool fs_choiceSE;		// ���[�U�I�����ύX���ʉ��t���O
	int hs_choiceSE;		// ���[�U�I�����ύX���ʉ��n���h��
	bool fs_decisionSE;		// ���[�U�I����������ʉ��t���O
	int hs_decisionSE;		// ���[�U�I����������ʉ��n���h��
	int bgmVolume;			// BGM�{�����[��(0-100%)
	int seVolume;			// SE�{�����[��(0-100%)

private:
	void Draw();
	void Move(float delta_time);
	void SoundAll();
public:
	SceneManagerTitle();
	~SceneManagerTitle();
	bool All(float delta_time);
	void SetVolume(int bgmVolume, int seVolume);
	int GetUserChoice();
};