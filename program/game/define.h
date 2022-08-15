#pragma once
// -- ���ݒ� -- 
#define WIDTH 1024			// �Q�[����ʉ���
#define HEIGHT 768			// �Q�[����ʏc��
#define MENU_WIDTH 300		// ���j���[��ʉ���	 
#define MENU_HEIGHT 720		// ���j���[��ʏc��	
#define SCROLL_SPEED 200	// �w�i�̃X�N���[���X�s�[�h
// -- �w�i --
#define NAME_SIZE 50		// �w�i���T�C�Y

// -- �萔��` --
#define MARGINE 15		// �v���C���[�̈ړ�����
#define TOTAL_CH 6		// �`���v�^�[����

// -- 3�����ݒ�` --
#define RED 0			// �ԑ���
#define BLUE 1			// ����
#define YELLOW 2		// ������
#define NON_ELEMENT 3	// ������

// -- BGM&SE�{�����[�� --
#define BGM_INITIAL_VOLUME 50
#define SE_INITIAL_VOLUME 50

// -- �v���C���[�̃p�����[�^����` -- 
#define PLAYER_INITIAL_LIFENUM 3		// �v���C���[�����c�@��
#define PLAYER_INITIAL_HP 100			// �v���C���[�����̗�
#define PLAYER_MAX_HP 100				// �v���C���[�̗͏��
#define PLAYER_RESPAWN_TIME 3			// �v���C���[���X�|�[���^�C��
#define PLAYER_RESPAWN_POS_X (WIDTH - MENU_WIDTH) / 2	// �v���C���[���X�|�[���ʒuX
#define PLAYER_RESPAWN_POS_Y (HEIGHT - MARGINE - 70)
#define DAMAGE_INVALID_TIME_MAX 1.5		// ��_���[�W��̖��G����(s)
#define RED_BULLET_COLLISION 3
#define BLUE_BULLET_COLLISION 4
#define YELLOW_BULLET_COLLISION 3
#define RED_BULLET_FRAME_MAX 3			// �ԋ@�̒e�摜�t���[����
#define BLUE_BULLET_FRAME_MAX 6			// �@�̒e�摜�t���[����
#define YELLOW_BULLET_FRAME_MAX 4		// ���@�̒e�摜�t���[����
#define RED_BULLET_ANIM_SPAN 0.1		// �ԋ@�̒e�A�j���ؑ֎���
#define BLUE_BULLET_ANIM_SPAN 0.1		// �@�̒e�A�j���ؑ֎���
#define YELLOW_BULLET_ANIM_SPAN 0.1		// ���@�̒e�A�j���ؑ֎���
#define PLAYER_INITIAL_BOMBNUM 3		// �v���C���[�����{����
#define PLAYER_MAX_BOMBNUM 3			// �v���C���[�{�������
#define PLAYER_STATUS_UP_MAX 8			// �X�e�[�^�X�A�b�v����l

// -- �v���C���[�̏����X�e�[�^�X..�܂��͏����Ă��� --
// �X�e�[�^�X�ω��������炷���߁A�����̒l���Q�Ƃ������B�Ȃ̂Œ�`�B
#define RED_INITIAL_BULLET_ATK 18		// �ԋ@�̒ʏ�e�����U����
#define BLUE_INITIAL_BULLET_ATK 45		// �@�̒ʏ�e�����U����
#define YELLOW_INITIAL_BULLET_ATK 65	// ���@�̒ʏ�e�����U����
#define RED_INITIAL_DEFENSE 15			// �ԋ@�̏����h���
#define BLUE_INITIAL_DEFENSE 20			// �@�̏����h���
#define YELLOW_INITIAL_DEFENSE 12		// ���@�̏����h���
#define RED_REF_ACC_MAX_N 0.3			// �ԋ@�̏����ʏ�����x
#define RED_REF_ACC_MAX_S 0.6			// �ԋ@�̏����V�t�g�����x
#define RED_REF_VEL_MAX_N 3.5			// �ԋ@�̏����ʏ푬�x
#define RED_REF_VEL_MAX_S 5.5			// �ԋ@�̏����V�t�g���x
#define BLUE_REF_ACC_MAX_N 0.2			// �@�̏����ʏ�����x
#define BLUE_REF_ACC_MAX_S 0.4			// �@�̏����V�t�g�����x
#define BLUE_REF_VEL_MAX_N 3.2			// �@�̏����ʏ푬�x
#define BLUE_REF_VEL_MAX_S 4.5			// �@�̏����V�t�g���x
#define YELLOW_REF_ACC_MAX_N 0.4		// ���@�̏����ʏ�����x
#define YELLOW_REF_ACC_MAX_S 0.7		// ���@�̏����V�t�g�����x
#define YELLOW_REF_VEL_MAX_N 4.8		// ���@�̏����ʏ푬�x
#define YELLOW_REF_VEL_MAX_S 7.5		// ���@�̏����V�t�g���x
#define RED_RELOAD_TIME 0.08			// �ԋ@�̏��������[�h�^�C��
#define BLUE_RELOAD_TIME 0.4			// �@�̏��������[�h�^�C��
#define YELLOW_RELOAD_TIME 0.6			// ���@�̏��������[�h�^�C��
#define RED_BULLET_SPEED 800			// �ԋ@�̒e�������l
#define BLUE_BULLET_SPEED 1800			// �@�̒e�������l
#define YELLOW_BULLET_SPEED 600			// ���@�̒e�������l

// -- �X�R�A�{�[�h�̃p�����[�^����` -- 
#define MENU_ORIGINE_X  WIDTH - MENU_WIDTH			// ���j���[��ʌ��_�w
#define MENU_ORIGINE_Y  MARGINE					// ���j���[��ʌ��_Y
#define ELEMENT_X MENU_ORIGINE_X + MENU_WIDTH * 50/100			// �G�������g�\���ʒuX
#define ELEMENT_Y MENU_ORIGINE_Y + MENU_HEIGHT * 85/100		// �G�������g�\���ʒuY

// -- �A�C�e���̏�� --
#define ITEM_NUM 30						// ��ʏ�̃A�C�e�������
#define ITEM_COLLISION 16				// �A�C�e���擾�͈�
#define ITEM_NONE 0						// �A�C�e������
#define ITEM_HP 1						// �񕜃A�C�e��
#define ITEM_POWER 2					// �p���[�A�b�v�A�C�e��
#define ITEM_DEFENSE 3					// �f�B�t�F���X�A�b�v�A�C�e��
#define ITEM_SPEED 4					// �X�s�[�h�A�b�v�A�C�e��
#define ITEM_BOMB 5						// �{���������A�b�v�A�C�e��

// - �A�C�e���̌��� -
#define ITEM_HP_RECOVERY_AMOUNT 30		// �A�C�e���ł�HP�񕜗�

// - �{���\���� -
struct Bomb {
	int gh[3];				// �O���t�B�b�N�n���h��
	int effectGh[10];		// ���j�G�t�F�N�g�n���h��
	double x;				// ���W
	double y;
	int targetX;			// �{�����B�ڕW�l
	int targetY;			// 
	float rad;				// �{�����B�ڕW�p�x
	float velMagunitude;	// �{������
	float velX;				// �{������x����
	float velY;				// �{������y����
	float bomb_animTime;	// �{���A�j���[�V�����ύX���ԊԊu
	int bomb_animFlame;		// �{���ړ��A�j���[�V�����t���[��
	float bomb_explodeAnimTime;		// �{���A�j���[�V�����ύX���ԊԊu
	int bomb_explodeFlame;	// �{�����j�A�j���[�V�����t���[��
	bool bombFlag;			// �{�������t���O
	bool explodeFlag;		// �{�������t���O
	int bomb_damage;		// �{���̃_���[�W
	int colision;			// �{���̃R���W����
};
#define BOMB_VEL_MAGNITUDE 300			// �{�����ˏ����x
#define BOMB_EXPLODE_COLLISION	150		// �{�����j�R���C�_�[
#define BOMB_DAMAGE 900					// �{���U���� 

// -- �G�̃p�����[�^����` --
#define EFFECT_EDEADNUM	25				// �G�j��G�t�F�N�g�N���X��`���i��ʏ�̃G�t�F�N�g�����)
#define ENEMY_BULLET_ANIM_SPAN 0.1		// �G�@�̒e�A�j���ؑ֎���
#define ENEMY_DAMAGE_ANIMTIME 0.2		// �G�@��e�A�j���\������

// - �@�̖��̓��������Â���p�����[�^�\���� - 
struct Machine {
	int gh;				// �O���t�B�b�N�n���h��
	int width;			// �@�̂̉���
	int height;			// �@�̂̏c��
	int element;		// �@�̂̑���
	int hp;				// �@�̗̂̑�
	int defence;		// �@�̖̂h���
	float mass;			// �@�̎���
	float refAccMaxNormal;		// �ʏ��s���̉����x���
	float refAccMaxShifted;		// ������s���̉����x���
	float refVelMaxNormal;		// �ʏ��s���̑��x���
	float refVelMaxShifted;		// ������s���̑��x���
	float kp;					// ���萔
	float ki;					// �ϕ��萔
	float kd;					// �����萔
	float reloadTime;			// ���e���˂܂ł̎���
	float bulletSpeed;			// �e�ۂ̔��ˑ��x
	int score;					// (�G�@�p : ���j�X�R�A)
};
#define MACHINE_NUM 3			// �}�V����
#define MACHINE_RED	0			// �}�V���̎�ށFRed
#define MACHINE_BLUE 1			// �}�V���̎�ށFBlue
#define MACHINE_YELLOW 2		// �}�V���̎�ށFYellow

// - �e�̍\���� - 
#define PSHOT_NUM 50	// �e������
#define SHOT_FRAME_MAX 6	// �e�摜�t���[�����
struct Shot {
	int gh[SHOT_FRAME_MAX];				// �O���t�B�b�N�n���h��
	bool flag;			// �e�����˒����ǂ���
	double x;			// ���W
	double y;
	int width, heigh;	// �摜�̕��ƍ���
	float rad;			// �e�̔��ˊp�x
	float speed;		// �e�̑��x
	int element;		// �e�̑���
	float damage;		// �e�̃_���[�W
	float time;			// �X�|�[�����Ă���̌o�ߎ���
	int animFrame;		// �e�A�j���[�V�����̃t���[����
};


// -- �G�̃p�����[�^����` -- 
#define ENEMY_NUM 100	// �G�̑���
// - �G�̒e�\���� -
#define ENEMY_SNUM 100	// �G�̒e�̏��������
struct EShot {
	int type;			// �e�̎��
	int gh[SHOT_FRAME_MAX];				// �O���t�B�b�N�n���h��
	bool flag;			// �e�����˒����ǂ���
	double x;			// ���W
	double y;
	int width, height;	// �摜�̕��ƍ���
	float rad;			// �e�̔��ˊp�x
	float speed;		// �e�̃X�s�[�h
	int element;		// �e�̑���
	float damage;		// �_���[�W
	float time;			// �X�|�[�����Ă���̌o�ߎ���
	int animFrame;		// �e�A�j���[�V�����̃t���[����
};

// - �G�̃f�[�^�\����(�t�@�C���R�s�[�̂���)
#define ENEMY_MOVE_K 13	// �G�̍s���p�^�[���W���i�[�����
struct EnemyData {
	int x;		// �X�|�[���ʒux
	int y;		// �X�|�[���ʒuy
	int type;		// �G�̎��
	int stype;		// �e�̎��
	int s_pattern;	// �V���b�g�p�^�[��
	int item;		// �A�C�e��
	int m_pattern;	// �s���p�^�[��
	float in_time;	// �o������
	float start_fire_time;	// ���C�J�n����
	float end_fire_time;		// ���C�I������
	float out_time;	// �A�Ҏ���
	int outX;		// �A�ҍ��WX
	int outY;		// �A�ҍ��WY	
	int moveK[ENEMY_MOVE_K];		// �s���p�^�[���W���i�[
	int colision;					// �{���̃R���W����
};

// - �j��G�t�F�N�g - 
#define DEAD_EFFECT_FRAME_MAX 10	// �j��A�j���[�V�����̃t���[�����

