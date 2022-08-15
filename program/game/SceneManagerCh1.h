#pragma once

// -- ����N���X�p�� --
#include "Player.h"
#include "Back.h"
#include "Enemy.h"
#include "DeadEffect.h"
#include "Score.h"
#include "Item.h"
#include "define.h"
#include "Tutorial.h"

class SceneManagerCh1
{
private:

	// -- �ϐ� ---
	// - �I�������`���v�^�[�ԍ�
	int chapterNum;
	// - ���� - 
	float totalTime;			// �o�ߎ���
	// - ���W��� -
	double playerX, playerY;	// �v���C���[�̈ʒu
	// - �G�̍s���p�^�[���W���i�[
	int moveK[ENEMY_MOVE_K];
	// - �G���^�C���e�[�u���̏ォ�珇�ɃX�|�[��. ���̃J�E���g
	int enemySpawnCount;
	bool spawnEndFlag;			// �G�X�|�[�������t���O
	// - All���������t���O -
	bool allInit;				// All()�֐����s���A����݂̂̏������s�̂��߂̃t���O
	// -- BGM�n���h�� --
	int hb_stageSound;			// �X�e�[�WBGM
	int hb_bossSound;			// �{�X�퓬BGM
	// -- �|�[�Y��� --
	bool pauseDrawInit;			// �|�[�Y��ʕ`�揉�������t���O
	bool pauseFlag;				// �{�[�Y��ʎ��s�t���O
	float pauseWaitTime;		// �L�[���̓`���^�����O�h�~�p�̃J�E���^
	int pauseBackGh;			// �|�[�Y��ʔw�i
	int pauseTitleLogoGh;		// �|�[�Y��ʃ��S�O���t�B�b�N�n���h��
	int pauseSelectLogoGh[8];	// �|�[�Y��ʑI�������S�O���t�B�b�N�n���h��
	int guideGh;				// ����K�C�h�O���t�B�b�N�n���h��
	int pauseSelectLogo;		// �|�[�Y��ʑI����
	int pauseHelpLogo;			// �|�[�Y�w���v���S�O���t�B�b�N�n���h��
	bool pauseHelpFlag;		// �|�[�Y���A��������摜���J���Ă��邩�̃t���O
	int controlGuideGh;		// ������@�摜�̃O���t�B�b�N�n���h���i�[

	// -- �Q�[���I�[�o�[��� -- 
	bool gameOverFlag;			// �Q�[���I�[�o�[�t���O
	float gameOverWindowTime;	// �Q�[���I�[�o�[�ɂȂ��Ă���A�Q�[���I�[�o�[��ʂ��o���܂ł̎���
	bool gameOverDrawInit;		// �Q�[���I�[�o�[��ʕ`�揉�������t���O
	int gameOverBackGh;			// �Q�[���I�[�o�[��ʔw�i
	int gameOverTitle;			// �Q�[���I�[�o�[��ʃ^�C�g�����S�O���t�B�b�N�n���h��
	int gameOverLogoGh[6];		// �Q�[���I�[�o�[��ʃ��S�O���t�B�b�N�n���h��
	int gameOverSelectLogo;		// �Q�[���I�[�o�[��ʑI����
	// -- ���U���g��� --
	bool resultFlag;			// ���U���g��ʕ\���t���O
	bool resultInit;			// ���U���g��ʏ��������t���O
	int resultBackGh;			// ���U���g��ʔw�i
	int resultTitleGh;			// ���U���g��ʃ^�C�g�����S�O���t�B�b�N�n���h��
	int resultLogo1Gh;			// ���U���g��ʃ��S�O���t�B�b�N�n���h���P
	int resultLogo2Gh;			// ���U���g��ʃ��S�O���t�B�b�N�n���h���Q
	int numGh[10];				// �����̉摜�n���h��
	int rankGh[5];				// �����N(S-D)�̉摜�n���h��
	float resultWindowTime;		// ���U���g��ʕ\���f�B���C�J�E���^
	int hs_winBGM;				// ����BGM

	// -- ���ʉ��n���h���� --�@
	bool soundInit;
	int hs_playerRedShot01;		// ���@�W���e���ˉ��n���h��
	int hs_playerBlueShot01;		// ���@�W���e���ˉ��n���h��
	int hs_playerYellowShot01;		// ���@�W���e���ˉ��n���h��
	int hs_playerDead;			// �v���C���[�j��
	int hs_playerTransform;	// �v���C���[�ό`���n���h��
	bool fs_playerDamaged;		// �v���C���[�_���[�W�t���O
	int hs_playerDamaged;		// �v���C���[�_���[�W���n���h��
	bool fs_playerBombShot;		// �v���C���[�{���V���b�g���t���O
	int hs_playerBombShot;		// �v���C���[�{���V���b�g���n���h��
	bool fs_playerBombExplode;	// �v���C���[�{���������t���O
	int hs_playerBombExplode;	// �v���C���[�{���������n���h��
	bool fs_playerRefrect;		// �v���C���[�m�[�_���[�W���t���O
	int hs_playerRefrect;		// �v���C���[�m�[�_���[�W���n���h��
	float fs_shiftedFlying;		// �V�t�g�ō����ړ������𔻒�
	int hs_playerFlying;		// �v���C���[��s���n���h��
	int hs_playerShiftedFlying;	// �v���C���[������s���n���h��(�V�t�g�L�[����)
	bool fs_enemyDead;			// �G���Ńt���O
	int hs_enemyDead;			// �G���ŉ��n���h��
	bool fs_enemyDamaged;		// �G�_���[�W�t���O
	int hs_enemyDamaged;		// �G�_���[�W���n���h��
	bool fs_itemGot;			// �A�C�e���擾�t���O
	int hs_itemGot;				// �A�C�e���擾���n���h��
	bool fs_enemyShot01;		// �G���C�v���O
	int hs_enemySho01;			// �G���C���n���h��
	bool fs_enemyReflect;		// �G�@�m�[�_���[�W���t���O
	int hs_enemyReflect;		// �G�@�m�[�_���[�W���n���h��
	// ���j���[���
	bool fs_choiceSE;			// �I�������ʉ��t���O
	int hs_choiceSE;			// �I�������ʉ��n���h��
	bool fs_decisionSE;			// �I�������艹�t���O
	int hs_decisionSE;			// �I�������艹�n���h��
	// -- BGM, SE���� --
	int bgmVolume;				// BGM����(0-100%)
	int seVolume;				// BGM����(0-100%)
	// -- ��ʑJ�ڐ�i�[ --
	int tempScene;				// SceneDefine.h�Q��
	// -- �{�X�t���O�֌W --
	bool bossExistFlag;			// �{�X�o���t���O
	bool bossExistInit;			// �{�X�o�����������t���O
	int deadBossCounter;		// �{�X���j���J�E���^
	int bossNumMax;				// �{�X������ݒ�
	// -- �`���[�g���A�� --
	bool tutorialFlag;			// �`���[�g���A�����{�t���O
	bool tutorialEndFlag;		// �`���[�g���A���I���t���O

	// - �G�̃^�C���e�[�u���p��
	EnemyData edata[ENEMY_NUM];

	// -- �N���X ---
	// �v���C���[�N���X
	Player* player;
	// �w�i�`��N���X
	Back* back;
	// �G�N���X
	Enemy* enemy[ENEMY_NUM];
	// �X�R�A�{�[�h�N���X
	Score* score;
	// �A�C�e���N���X
	Item* item[ITEM_NUM];
	// �`���[�g���A���N���X
	Tutorial tutorial;

private:
	void SoundAll();
	void CollisionAll();
	bool CircleCollision(double cir1, double cir2, double cirX1, double cirX2, double cirY1, double cirY2);
	void GetPlayerPosition();
	void Time(float delta_time);
	void ScoreUpdate();
	int PauseWindow(bool pauseFlag);
	int GameOverWindow(bool gameOverFlag);
	int ResultWindow(bool resultFlag);
	bool BossExistFlag();
	bool BossDefeatFlag();

public:
	SceneManagerCh1(int chapterNum, char backName[NAME_SIZE], char effect1[NAME_SIZE], char effect2[NAME_SIZE], char enemyData[NAME_SIZE]);
	~SceneManagerCh1();
	int  All(float delta_time);
	void SetVolume(int bgmVolume, int seVolume);
};