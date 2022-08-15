#pragma once
#include "define.h"
#include "DeadEffect.h"

class Enemy
{
private:
	// - �G�̊�{�I�ȃp�����[�^ - 
	double x, y;		// x, y���W
	int gh;				// �O���t�B�b�N�n���h��(�ꖇ�G)
	int ghs[3];			// �O���t�B�b�N�n���h��(3���G)
	int width, height;	// ����, ����
	float rad;			// ����
	double refX, refY;	// ���B�ڕW�ʒu(�{���W��ڎw���Ĉړ�����)
	float preRad;		// 1�t���[���O�̌���

	// - ��ʏ����p�����[�^ - 
	bool drawInit;		// �`�揉�������t���O

	// - �G�̑����� -
	int type;			// ���
	int stype;			// �e�̎��
	int s_pattern;		// �V���b�g�p�^�[��
	int item;			// �h���b�v�A�C�e��
	int m_pattern;		// �ړ��p�^�[��
	// - ���� -
	float in_time;			// �o������
	float start_fire_time;	// ���C�J�n����
	float end_fire_time;	// ���C�I������
	float out_time;			// �A�Ҏ���
	float damageAnimTime;	// �_���[�W�A�j���p�J�E���^
	// - �A�ҍ��W -
	double outX;
	double outY;
	// - �}�V���p�����[�^(�Ǎ��ݐ�p) - 
	Machine machine;

	// - �e�ۓ� -
	EShot shot[ENEMY_SNUM];
	bool shotFlag;			// �e�����Ă�悤�ɂȂ������̃t���O
	bool hormingFlag;		// �z�[�~���O�e�ɂ���ꍇ�̃t���O
	bool waveShotFlag;		// �E�F�[�u�V���b�g����ꍇ�̃t���O
	float waveShotRad;		// 
	int waveShotCount;		// 

	// - �G�̍s���p�^�[���W�� �i�[�\���� - 
	int moveK[ENEMY_MOVE_K];

	// - �ʒu����p�p�����[�^ -
	float velX;			// �����������x
	float velY;			// �����������x
	float accX;			// �������������x
	float accY;			// �������������x
	float refPosX;		// ���������ʒu�ڕW�l
	float refPosY;		// ���������ʒu�ڕW�l
	float velMax;		// ���x����l
	float accMax;		// �����x����l
	float preErrorX;	// 1�t���[���O�̈ʒu�덷�i�[X
	float preErrorY;	// 1�t���[���O�̈ʒu�덷�i�[Y
	float integralX;	// �덷�ώZ�lX
	float integralY;	// �덷�ϕ��lY
	
	// - ���� -
	float totalTime;		// �o�ߎ��Ԋi�[
	float reloadingTime;	// ���e���˂܂ł̎��ԊԊu�Ǘ�
	
	// - �t���O -
	bool damageFlag;		// �_���[�W�t���O
	bool deadFlag;			// �G���Ńt���O
	bool deadInitFlag;		// ���S���A��x�������s�����֐��̃t���O
	bool endFlag;			// �G�N���X���Ńt���O
	bool damageEffectFlag;	// ��_���[�W���̃G�t�F�N�g�t���O;
	bool deadMomentFlag;	// �j��t���O(�j�󎞂�1�t���[���̂ݗL����)
	bool deadEffectEndFlag;	// ���S�G�t�F�N�g�̍Đ����I��������true;
	bool damageAnimFlag;	// �_���[�W�A�j���t���O
	bool fs_shotFlag;		// ���C�������̃t���O
	bool shootingFlag;		// ��ʂɔ��˒��̒e�����݂��鎞true;		

	// - �_���[�W�v�Z�p -
	int damagedElement;	// ��e���̑���
	int damagedNum;		// ��e�_���[�W
	bool fs_reflect;	// �m�[�_���[�W�t���O
	bool fs_damage;		// �_���[�W�t���O
	// - �O���̏����i�[ -
	double targetX, targetY;	// �W�I(��Ƀv���C���[)�̈ʒu���i�[

	// - �{�X�t���O -
	bool isBoss;			// �{�X�ł��邩
	bool isBossDead;		// �{�X���S�t���O
	bool isBossDeadInit;	// �{�X���S���������t���O
	//- ���S�G�t�F�N�g�N���X
	DeadEffect* deadEffect;

private:
	void Move(double refX, double refY, float delta_time);
	void Draw();
	void Time(float delta_time);
	void Shot(float delta_time);
	void GetMachine(int type);
	void GetShot(int stype);
	void MoveControl(int m_pattern);
	bool CircleCollision(double cir1, double cir2, double cirX1, double cirX2, double CirY1, double CirY2);
	bool CheckOutOfWindow();

public:
	void DamageCalc();
	void GetPosition(double* x, double* y);
	void SetPosition(double x, double y);
	bool GetDeadFlag();
	int GetCollision();
	int GetShotCollision(int index);
	bool GetShotPosition(int index, double* x, double* y);
	int GetShotElement(int index);
	int GetShotDamage(int index);
	bool GetDeadMoment();
	void GetVelocity(double* vx, double* vy);
	bool GetShotFlag();
	int GetScore(float totalTime);
	int GetItem();
	bool GetBossExist();
	bool GetBossDead();
	bool GetRefrectFlag();
	bool GetDamageFlag();
	void SetShotFlag(int index, bool flag);
	void SetDamage(int element, int damage);
	void SetOutTime(float outTime);
	bool All(float delta_time, double targetX, double targetY);
	Enemy(
		float time,		// �o�����Ԃ��i�[
		double x,		// �X�|�[���ʒux
		double y,		// �X�|�[���ʒuy
		int type,		// �G�̎��
		int stype,		// �e�̎��
		int s_pattern,	// �V���b�g�p�^�[��
		int item,		// �A�C�e��
		int m_pattern,	// �s���p�^�[��
		float in_time,	// �o������
		float start_fire_time,		// ���C�J�n����
		float end_fire_time,		// ���C�I������
		float out_time,				// �A�Ҏ���
		double outX,	// �A�ҍ��WX
		double outY,	// �A�ҍ��WY	
		int moveK[ENEMY_MOVE_K]		// �s���p�^�[���W���i�[
		);
	~Enemy();
};