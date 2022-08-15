#pragma once

class SceneManagerOpening 
{
private:
	float totalTime;		// �o�ߎ���
	float animCount;		// �A�j���[�V�����ؑփJ�E���^
	int animFrame;			// �A�j���[�V�����t���[���w��
	bool drawInit;			// �摜�����������t���O
	int opImageGh[7];		// �I�[�v�j���O�A�j���摜�n���h��
	int opMessageGh[2];		// �I�[�v�j���O�\�����S�摜�n���h��
	int bgmVolume;			// BGM�{�����[��(0-100%)
	int seVolume;			// BGM�{�����[��(0-100%)


private:
	void Draw();
	void Time(float delta_time);

public:
	SceneManagerOpening();
	~SceneManagerOpening();
	bool All(float delta_time);
	void SetVolume(int bgmvolume, int sevolume);

};