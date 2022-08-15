#pragma once
#include "../../program/dxlib_ext/dxlib_ext.h"	// DxLib���Q��
#include "define.h"
#include "DeadEffect.h"

class Player {

private:
	// --- �v���C���[���g�̃p�����[�^ --- 
	// -- ��{��� --
	double x, y;		// x, y���W
	bool life;			// �����t���O
	int machineType;	// �I�𒆂̃}�V��
	int pre_machineType;	// �ȑO�I�����Ă����}�V��
	float rad;			// �v���C���[�̌���
	int hp;				// �v���C���[��HP(�}�V���Ɋ֌W�������L)
	int lifeNum;		// �v���C���[�c�@
	double deadPosX;	// �v���C���[���S���W
	double deadPosY;	// 
	// -- �Ə� --
	int aimX;		// �Ə����W
	int aimY;
	int aimGh;			// �W���O���t�B�b�N�n���h��
	// -- �X�e�[�^�X�A�b�v --
	int additionalPower;		// �U���̓{�[�i�X
	int additionalSpeed;		// �X�s�[�h�{�[�i�X
	int additionalDefense;		// �h��̓{�[�i�X
	int pre_addPower;			// 1�t���[���O�̍U���̓{�[�i�X(�t���O���ĂɎg�p )
	int pre_addSpeed;			// 1�t���[���O�̃X�s�[�h�{�[�i�X
	int pre_addDefense;			// 1�t���[���O�̖h��̓{�[�i�X
	// -- �{�� --
	int bombNum;				// �{����
	Bomb bomb;					// �{���\����
	bool fs_bombShot;			// �{���V���b�g���t���O
	bool fs_bombExplode;		// �{�����j���t���O
	//  -- �ό`���[�V���� --
	int transformGh[12];		// �ό`���[�V�����O���t�B�b�N�n���h��
	int tr_animFlame;			// �ό`���[�V�����t���[��
	float tr_animTime;			// �ό`�A�j���[�V�������ԊԊu
	float tr_MaxTime;			// �ό`�A�j���[�V�����ő厞��
	float tr_countTime;			// �ό`���Ԋi�[
	bool tr_animFlag;			// �ό`�A�j���[�V�������s�t���O
	bool tr_seFlag;				// �ό`SE�t���O

	// - ��ʏ����p�����[�^ - 
	bool drawInit;		// �`�揉�������t���O

	// - ���x����p�p�����[�^[�@�̖��ɐ݂���]
	Machine machine[MACHINE_NUM];
	float velX;			// �����������x
	float velY;			// �����������x
	float accX;			// �������������x
	float accY;			// �������������x
	float refVelY;		// �����������x�ڕW�l
	float refVelX;		// �����������x�ڕW�l
	float velMax;		// ���x����l
	float accMax;		// �����x����l
	float integralX;	// ����덷�ώZ�lX
	float integralY;	// ����덷�ώZ�lY
	float preErrorX;	// 1�t���[���O�̑��x�덷�i�[
	float preErrorY;	// 1�t���[���O�̑��x�덷�i�[

	// - �����n�̃p�����[�^
	Shot shot[MACHINE_NUM][PSHOT_NUM];		// �e�@�̖��ɒe���Ǘ�����
	int shotCount;		// �e���˃J�E���g

	// - ���� - 
	float totalTime;			// �o�ߎ��Ԋi�[
	float reloadingTime;		// ���e���˂܂ł̎��ԊԊu�Ǘ�
	float damageInvalidTime;	// �_���[�W����������
	float damageAnimCounter;	// �_���[�W�A�j���[�V�����Đ��p�J�E���^
	float respawnTime;			// ���X�|�[������
	// - �t���O -
	bool damageFlag;		// ��_���[�W�t���O
	bool damageInvalidFlag;	// ���G�t���O
	bool deadFlag;			// ���S�t���O
	bool respawnFlag;		// ���X�|�[�����̃t���O
	bool redShotFlag;		// �V���b�g�t���O
	bool blueShotFlag;
	bool yellowShotFlag;	
	bool deadEffectFlag;	// ���S�G�t�F�N�g���{
	bool deadEffectInitFlag;	// ���ŃG�t�F�N�g�N���X�C���X�^���X�t���O
	bool drawBomb;			// ���j�G�t�F�N�g�`��t���O
	bool fs_damage;			// �_���[�W���ʉ��t���O
	bool fs_refrect;		// ���_���[�W���ʉ��t���O
	bool hs_shifted;		// �V�t�g�ō�����s�����ǂ����̃t���O
	bool penetrationFlag;	// �ђʒe���g�p�����ǂ����̃t���O

	// - �_���[�W�v�Z�p -
	int damagedElement;		// ��e���̑���
	int damagedNum;			// ��e�_���[�W

	// - ���ŃG�t�F�N�g�N���X
	DeadEffect* deadeffect;

private:
	void Move(float delta_time);
	void Draw();
	void Shot(float delta_time);
	void TimeAndFlag(float delta_time);
	void DamageCalc();
	void StateUp();

public:
	Player();
	~Player();
	void All(float delta_time);
	void GetPosition(double* x, double* y);
	bool GetShotPosition(int mechNum, int index, double* x, double* y);
	int GetShotElement(int mechNum, int index);
	int GetShotDamage(int mechNum, int index);
	int GetCollision();
	int GetBulletCollision();
	bool GetDamageFlag();
	int GetShotFlag();
	bool GetDamageInvalidFlag();
	bool GetLife();
	bool GetDeadFlag();
	int GetLifeNum();
	int GetHpNum();
	int GetPowerNum();
	int GetSpeedNum();
	int GetDefenseNum();
	int GetElement();
	int GetBombNum();
	bool GetBombFlag();
	bool GetBombExplodeFlag();
	void GetBombPosition(double* x, double* y);
	int GetBombCollision();
	int GetBombDamage();
	bool GetBombShotSoundFlag();
	bool GetBombExplodeSoundFlag();
	bool GetStateChange();	
	bool GetDamageSoundFlag();
	bool GetRefrectFlag();
	bool GetTranformFlag();
	bool GetShiftedFlying();
	bool GetPenetrationFlag();
	void SetShotFlag(int mechNum, int index, bool flag);
	void SetDamage(int element, int damage);
	void SetRespawn();
	void SetItem(int type);
	

};