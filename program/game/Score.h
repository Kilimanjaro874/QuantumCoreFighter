#pragma once

enum SCOREDATA {
	CURRENT_SCORE,
	LIFE,
	BOMB,
	HP,
	POWER,
	DEFENSE,
	SPEED,
	ELEMENT
};

class Score 
{
private:
	int currentScore;		// ���݂̃X�R�A
	int bombNum;			// �c��̃{����
	int hpNum;				// ���݂�HP
	int hpNumP;				// ���݂�HP(�o�[�ɑ΂��銄��%)
	int lifeNum;			// �c��̃��C�t��
	int stateMaxNum;		// �X�e�[�^�X�㏸����l
	int powerNum;			// ���݂̃p���[(%)
	int speedNum;			// ���݂̃X�s�[�h(%)
	int defenseNum;			// ���݂̖h���(%)
	int currentElement;		// ���ݑI�𒆂̃G�������g
	int elementGh[3];		// �G�������g�摜�n���h��
	int messageGh[8];		// �X�R�A�ł̕����摜�n���h��
	int numGh[10];			// �����̉摜�n���h��
	int lifeNumGh[3];		// �c�@�摜�n���h�� 
	int bombGh;				// �{���摜�n���h��
	int gageMax;			// hp�Q�[�W�̉����ő�l
	int redGage;			// hp�ԃQ�[�W
	int orangeGage;
	int greenGage;
	int redBar;				// hp�ԃo�[
	int orangeBar;
	int greenBar;
	int statusGageGh;		// �X�e�[�^�X�Q�[�W�摜�n���h��
	int powerBarGh;			// �p���[�o�[�摜�n���h��
	int speedBarGh;			// �X�s�[�h�o�[�摜�n���h��
	int defenseBarGh;		// �f�B�t�F���X�o�[�摜�n���h��
	bool drawInit;			// �`�揉�����t���O

private:
	void Draw();

public:
	void SetScore(SCOREDATA data, int val);
	int GetScore(SCOREDATA data);
	void All();
	Score();

};