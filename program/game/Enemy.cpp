#include "Enemy.h"
#include "define.h"
#include "../dxlib_ext/dxlib_ext.h"

Enemy::Enemy(
	float time,		// �o�����Ԃ��i�[
	double x,		// �X�|�[���ʒux
	double y,		// �X�|�[���ʒuy
	int type,		// �G�̎��
	int stype,		// �e�̎��
	int s_pattern,	// �V���b�g�p�^�[��
	int item,		// �A�C�e��
	int m_pattern,	// �s���p�^�[��
	float in_time,	// �o������
	float start_fire_time,	// ���C�J�n����
	float end_fire_time,		// ���C�I������
	float out_time,	// �A�Ҏ���
	double outX,	// �A�ҍ��WX
	double outY,		// �A�ҍ��WY	
	int moveK[ENEMY_MOVE_K]		// �s���p�^�[���W���i�[
	) {
	// --- �R���X�g���N�^ ---
	// - �G�̊�{�I�ȃp�����[�^ - 
	this->totalTime = time;
	this->x = x;
	this->y = y;
	gh = 0;				// Draw()�Ō�ɏ�����(DxLib_Init()����{�̂���)
	ghs[3] = {0};		// Draw()�Ō�ɏ�����
	width = 0;			// Draw()�Ō�ɏ�����
	height = 0;			// Draw()�Ō�ɏ�����
	rad = 0;	
	preRad = 0;
	refX = 0;
	refY = 0;
	// - �摜�����p�����[�^ - 
	drawInit = false;
	// - �G�̑����� - 
	this->type = type;
	this->stype = stype;
	this->m_pattern = m_pattern;
	this->s_pattern = s_pattern;
	this->item = item;
	// - ���ԁF�o���A�ˌ��J�n, �ˌ���~�A�A�Ҏ��� - 
	this->in_time = in_time;
	this->start_fire_time = start_fire_time;
	this->end_fire_time = end_fire_time;
	this->out_time = out_time;	
	damageAnimTime = 0;
	// - �A�ҍ��W - 
	this->outX = outX;
	this->outY = outY;
	// �{�X�t���O(�{�X�L�����ł����true)
	isBoss = false;
	isBossDead = false;
	isBossDeadInit = false;
	// - �}�V���p�����[�^: type�ɉ����ĎQ�Ƃ���}�V���p�����[�^�ω� - 
	GetMachine(type);
	// - �e�ۍ\���̂̏�����: stype�ɉ����ĎQ�Ƃ���e�ۃf�[�^�ω� -
	GetShot(stype);
	shotFlag = false;
	hormingFlag = false;
	waveShotFlag = false;
	waveShotRad = 0;
	waveShotCount = 0;
	// �G�̍s���p�^�[���W���@�i�[�\���� ������
	for (int i = 0; i < ENEMY_MOVE_K; i++) {
		this->moveK[i] = moveK[i];
	}
	// �ʒu����p�p�����[�^
	velX = 0;
	velY = 0;
	accX = 0;
	accY = 0;
	refPosX = 0;
	refPosY = 0;
	velMax = 0;
	accMax = 0;
	preErrorX = 0;
	preErrorY = 0;
	integralX = 0;
	integralY = 0;
	// ����
	reloadingTime = 0;
	// �t���O
	damageFlag = false;
	deadFlag = false;
	deadInitFlag = false;
	endFlag = false;
	deadMomentFlag = false;
	deadEffectEndFlag = false;
	damageEffectFlag = false;
	damageAnimFlag = false;
	fs_shotFlag = false;
	shootingFlag = false;
	// �_���[�W�v�Z�p
	damagedElement = 99;	// �K���Ȑ��������Ă���
	damagedNum = 0;
	fs_reflect = false;
	fs_damage = false;
	// �O���̏����i�[
	targetX = 0;
	targetY = 0;

	// ���S�G�t�F�N�g�N���X
	// -> �{�N���X���S���ɃC���X�^���X��.�b��I��nullpointer��^����
	deadEffect = nullptr;

	//// ** �f�o�b�O�p
	//// �C���X�^���X���ɋA�����Ԃ𒴂��Ă���ꍇ�A�{�C���X�^���X����������t���O�𗧂Ă�
	//if (totalTime >= out_time -10 ) {
	//	endFlag = true;
	//}
	//if (totalTime <= in_time) {
	//	endFlag = true;
	//}

}

Enemy::~Enemy() {
	// --- �f�X�g���N�^ ---
	if (deadEffect != NULL) {
		delete deadEffect;
	}
}

bool Enemy::All(float delta_time, double targetX, double targetY) {
	// --- �G�̑�����O������Ăяo�� ---

	// - �^�[�Q�b�g(��Ƀv���C���[���W)���擾
	SetPosition(targetX, targetY);
	// - �ڕW�ʒurefX, refY �X�V
	MoveControl(m_pattern);
	// -- nan�G���[����F�|�[�Y��ʎ� --
	if (!delta_time == 0) {
		// - �ڕW�ʒu�Ɍ������ē���
		Move(refX, refY, delta_time);
		// - �ˌ� - 
		Shot(delta_time);
	}
	// - �_���[�W�v�Z -
	// SceneManager���瑦���ɌĂяo���悤�ύX
	// DamageCalc();
	
	// - ���S��, ��x�������s����鏈���L�q -
	if (deadFlag) {
		if (!deadInitFlag) {
			// ���S�G�t�F�N�g�N���X�C���X�^���X��
			deadEffect = new DeadEffect(2, x, y, velX, velY, 0.8, 3.0, 2.0);
			// ���S���A1�t���[�������L���ȃt���O�𗧂Ă�
			deadMomentFlag = true;
			deadInitFlag = true;
		}
		else {
			// ���S���A1�t���[�������L���ȃt���O��܂�
			deadMomentFlag = false;
		}

		// ���S�G�t�F�N�g�N���X���s���Đ����ς񂾂�(true��Ԃ�)�폜
		deadEffectEndFlag = deadEffect->All(delta_time);
	}

	// - �`�� -
	Draw();
	// - ���ԊǗ� - 
	Time(delta_time);

	// �A�����Ԃ𒴂��Ă��違��ʊO�Ɉړ��������A�{�C���X�^���X����������t���O�𗧂Ă�
	if (totalTime >= out_time && CheckOutOfWindow() && !shootingFlag) {
		endFlag = true;
	}

	// - �C���X�^���X�����t���O��Ԃ�(shot()���ŊǗ�)
	return endFlag;
}

void Enemy::Move(double refX, double refY, float delta_time){
	// --- �G�̓��� --- 

	// -- �ʒu���� -- 
	this->refPosX = refX;		// �ڕW�ʒuX
	this->refPosY = refY;		// �ڕW�ʒuY

	// - �ʒu�Ǐ]������(�����) - 
	// ���x�������x����l�Q��
	velMax = machine.refVelMaxNormal;
	accMax = machine.refAccMaxNormal;
	float errorX = (refPosX - x);	// �ʒu�덷�v�Z
	float errorY = (refPosY - y);	
	// ������͐���
	integralX += (errorX + preErrorX) / 2 * delta_time;
	float ux = machine.kp * errorX +
		machine.ki * integralX +
		machine.kd * (errorX - preErrorX) / delta_time;
	integralY += (errorY + preErrorY) / 2 * delta_time;
	float uy = machine.kp * errorY +
		machine.ki * integralY +
		machine.kd * (errorY - preErrorY) / delta_time;


	// - ����Ώۂ֎w�ߒl�𔽉f - 
	// �����x
	accX += (ux / machine.mass) * delta_time;
	accY += (uy / machine.mass) * delta_time;
	// �����x����
	float accNorm = hypot(accX, accY);
	if (accNorm >= accMax) {
		accX = (accX / accNorm) * accMax;
		accY = (accY / accNorm) * accMax;
	}
	// ���x
	velX += accX;
	velY += accY;
	// ���x����
	float velNorm = hypot(velX, velY);
	if (velNorm >= velMax) {
		velX = (velX / velNorm) * velMax;
		velY = (velY / velNorm) * velMax;
	}
	// �ʒu
	x += velX;
	y += velY;

	// - ��������i�[ -
	preErrorX = errorX;
	preErrorY = errorY;


	// -- �p������ --
	// 1�t���[���O��rad���i�[(��ɕ΍��ˌ��Q�l�̂���)
	preRad = rad;

	// �^�[�Q�b�g�̕�������
	rad = atan2(targetY - y, targetX - x);

}

void Enemy::Draw() {
	// --- �G�̕`�� --- 

	// -- ���������� -- 
	if (!drawInit) {
		
		// - �@�� - 
		// �^�C�v�ɉ����Ċe�X�̉摜�ǂݍ���(�ǁX�Acsv������ǂݍ��݂ł���悤�ɂ�����)
		switch (type) {
		case 100:
		case 200:
		case 300:
			LoadDivGraph("graphics/Enemy/Mob000.png", 3, 1, 3, 50, 50, ghs);

			break;

		case 101:
		case 201:
		case 301:
			LoadDivGraph("graphics/Enemy/Mob001.png", 3, 1, 3, 50, 50, ghs);

			break;

		case 102:
		case 202:
		case 302:
			LoadDivGraph("graphics/Enemy/Mob002.png", 3, 1, 3, 50, 85, ghs);

			break;

		case 103:
		case 203:
		case 303:
			LoadDivGraph("graphics/Enemy/Mob003.png", 3, 1, 3, 26, 50, ghs);

			break;

		case 104:
		case 204:
		case 304:
			LoadDivGraph("graphics/Enemy/Mob004.png", 3, 1, 3, 42, 64, ghs);

			break;

		case 105:
		case 205:
		case 305:
			LoadDivGraph("graphics/Enemy/Mob005.png", 3, 1, 3, 52, 60, ghs);

			break;

		case 106:
		case 206:
		case 306:
			LoadDivGraph("graphics/Enemy/Mob006.png", 3, 1, 3, 50, 56, ghs);

			break;

		case 107:
		case 207:
		case 307:
			LoadDivGraph("graphics/Enemy/Mob007.png", 3, 1, 3, 70, 60, ghs);

			break;

		case 108:
		case 208:
		case 308:
			LoadDivGraph("graphics/Enemy/Mob008.png", 3, 1, 3, 65, 76, ghs);

			break;

		case 109:
		case 209:
		case 309:
			LoadDivGraph("graphics/Enemy/Mob009.png", 3, 1, 3, 128, 68, ghs);

			break;

		case 110:
		case 210:
		case 310:
			LoadDivGraph("graphics/Enemy/Mob010.png", 3, 1, 3, 104, 120, ghs);

			break;

		case 111:
		case 211:
		case 311:
			LoadDivGraph("graphics/Enemy/Mob011.png", 3, 1, 3, 108, 102, ghs);

			break;

		

		}
		// �摜�̃T�C�Y���擾
		GetGraphSize(ghs[machine.element], &width, &height);

		// - �e - 
		// �e�̎�ނɉ����Ď擾�摜�ύX(�ǁX�Acsv������ǂݍ��݂ł���悤�ɂ�����)

		int temp[SHOT_FRAME_MAX];
		int w, h;
		switch (stype)
		{
		case 100:
			LoadDivGraph("graphics/Red/RedBullet001_6_84_14.png", 6, 6, 1, 84/6, 14, temp );
			GetGraphSize(temp[0], &w, &h);
			break;

		case 101:
			LoadDivGraph("graphics/Bullet/s_101.png", 6, 6, 1, 18, 16, temp);
			GetGraphSize(temp[0], &w, &h);
			break;

		case 102:
			LoadDivGraph("graphics/Bullet/s_102.png", 6, 6, 1, 10, 12, temp);
			GetGraphSize(temp[0], &w, &h);
			break;

		case 103:
			LoadDivGraph("graphics/Bullet/s_103.png", 6, 6, 1, 10, 21, temp);
			GetGraphSize(temp[0], &w, &h);
			break;

		case 104:
			LoadDivGraph("graphics/Bullet/s_104.png", 6, 6, 1, 22, 20, temp);
			GetGraphSize(temp[0], &w, &h);
			break;

		case 105:
			LoadDivGraph("graphics/Bullet/s_105.png", 6, 6, 1, 10, 16, temp);
			GetGraphSize(temp[0], &w, &h);
			break;

		case 106:
			LoadDivGraph("graphics/Bullet/s_106.png", 6, 6, 1, 16, 24, temp);
			GetGraphSize(temp[0], &w, &h);
			break;

		case 107:
			LoadDivGraph("graphics/Bullet/s_107.png", 6, 6, 1, 26, 26, temp);
			GetGraphSize(temp[0], &w, &h);
			break;

		case 108:
			LoadDivGraph("graphics/Bullet/s_108.png", 6, 6, 1, 22, 22, temp);
			GetGraphSize(temp[0], &w, &h);
			break;

		case 109:
			LoadDivGraph("graphics/Bullet/s_109.png", 6, 6, 1, 15, 18, temp);
			GetGraphSize(temp[0], &w, &h);
			break;

		case 110:
			LoadDivGraph("graphics/Bullet/s_110.png", 6, 6, 1, 33, 30, temp);
			GetGraphSize(temp[0], &w, &h);
			break;

		case 111:
			LoadDivGraph("graphics/Bullet/s_111.png", 6, 6, 1, 24, 36, temp);
			GetGraphSize(temp[0], &w, &h);
			break;

		case 200:
			LoadDivGraph("graphics/Blue/BlueBullet002_6_60_8.png", 6, 6, 1, 60 / 6, 8, temp);
			GetGraphSize(temp[0], &w, &h);
			break;

		case 201:
			LoadDivGraph("graphics/Bullet/s_201.png", 6, 6, 1, 18, 16, temp);
			GetGraphSize(temp[0], &w, &h);
			break;

		case 202:
			LoadDivGraph("graphics/Bullet/s_202.png", 6, 6, 1, 10, 12, temp);
			GetGraphSize(temp[0], &w, &h);
			break;

		case 203:
			LoadDivGraph("graphics/Bullet/s_203.png", 6, 6, 1, 10, 21, temp);
			GetGraphSize(temp[0], &w, &h);
			break;

		case 204:
			LoadDivGraph("graphics/Bullet/s_204.png", 6, 6, 1, 22, 20, temp);
			GetGraphSize(temp[0], &w, &h);
			break;

		case 205:
			LoadDivGraph("graphics/Bullet/s_205.png", 6, 6, 1, 10, 16, temp);
			GetGraphSize(temp[0], &w, &h);
			break;

		case 206:
			LoadDivGraph("graphics/Bullet/s_206.png", 6, 6, 1, 16, 24, temp);
			GetGraphSize(temp[0], &w, &h);
			break;

		case 207:
			LoadDivGraph("graphics/Bullet/s_207.png", 6, 6, 1, 26, 26, temp);
			GetGraphSize(temp[0], &w, &h);
			break;

		case 208:
			LoadDivGraph("graphics/Bullet/s_208.png", 6, 6, 1, 22, 22, temp);
			GetGraphSize(temp[0], &w, &h);
			break;

		case 209:
			LoadDivGraph("graphics/Bullet/s_209.png", 6, 6, 1, 15, 18, temp);
			GetGraphSize(temp[0], &w, &h);
			break;

		case 210:
			LoadDivGraph("graphics/Bullet/s_210.png", 6, 6, 1, 33, 30, temp);
			GetGraphSize(temp[0], &w, &h);
			break;

		case 211:
			LoadDivGraph("graphics/Bullet/s_211.png", 6, 6, 1, 24, 36, temp);
			GetGraphSize(temp[0], &w, &h);
			break;

		case 300:
			LoadDivGraph("graphics/Yellow/YellowBullet002_6_60_8.png", 6, 6, 1, 60 / 6, 8, temp);
			GetGraphSize(temp[0], &w, &h);
			break;

		case 301:
			LoadDivGraph("graphics/Bullet/s_301.png", 6, 6, 1, 18, 16, temp);
			GetGraphSize(temp[0], &w, &h);
			break;

		case 302:
			LoadDivGraph("graphics/Bullet/s_302.png", 6, 6, 1, 10, 12, temp);
			GetGraphSize(temp[0], &w, &h);
			break;

		case 303:
			LoadDivGraph("graphics/Bullet/s_303.png", 6, 6, 1, 10, 21, temp);
			GetGraphSize(temp[0], &w, &h);
			break;

		case 304:
			LoadDivGraph("graphics/Bullet/s_304.png", 6, 6, 1, 22, 20, temp);
			GetGraphSize(temp[0], &w, &h);
			break;

		case 305:
			LoadDivGraph("graphics/Bullet/s_305.png", 6, 6, 1, 10, 16, temp);
			GetGraphSize(temp[0], &w, &h);
			break;

		case 306:
			LoadDivGraph("graphics/Bullet/s_306.png", 6, 6, 1, 16, 24, temp);
			GetGraphSize(temp[0], &w, &h);
			break;

		case 307:
			LoadDivGraph("graphics/Bullet/s_307.png", 6, 6, 1, 26, 26, temp);
			GetGraphSize(temp[0], &w, &h);
			break;

		case 308:
			LoadDivGraph("graphics/Bullet/s_308.png", 6, 6, 1, 22, 22, temp);
			GetGraphSize(temp[0], &w, &h);
			break;

		case 309:
			LoadDivGraph("graphics/Bullet/s_309.png", 6, 6, 1, 15, 18, temp);
			GetGraphSize(temp[0], &w, &h);
			break;

		case 310:
			LoadDivGraph("graphics/Bullet/s_310.png", 6, 6, 1, 33, 30, temp);
			GetGraphSize(temp[0], &w, &h);
			break;

		case 311:
			LoadDivGraph("graphics/Bullet/s_311.png", 6, 6, 1, 24, 36, temp);
			GetGraphSize(temp[0], &w, &h);
			break;

		}

		// - �O���t�B�b�N�n���h���i�[���@�̂̕��������i�[
		for (int i = 0; i < ENEMY_SNUM; i++) {
			for (int k = 0; k < SHOT_FRAME_MAX; k++) {
				shot[i].gh[k] = temp[k];
			}
			shot[i].width = w;
			shot[i].height = h;
			
		}

		drawInit = true;
	}

	// -- �e�̕`�� --
	for (int i = 0; i < ENEMY_SNUM; i++) {
		if (shot[i].flag) {
			// - �e�A�j���ؑ� -
			if (shot[i].time >= ENEMY_BULLET_ANIM_SPAN) {
				shot[i].time = 0;
				shot[i].animFrame++;
				shot[i].animFrame %= SHOT_FRAME_MAX;
			}
			DrawRotaGraph(shot[i].x, shot[i].y, 1.5, shot[i].rad + tnl::PI / 2, shot[i].gh[shot[i].animFrame], true, false);
		}
	}


	// -- �G�̕`�� --
	// - ���S���Ă��Ȃ���Ε`�� - 
	if (!deadFlag) {

		// �_���[�W�L�F�_���[�W�A�j����\��
		if (damageAnimFlag) {
			switch (machine.element)
			{
			case RED:
				if (damageAnimTime < 0.1) {
					SetDrawBright(0, 0, 0);
					DrawRotaGraph(x, y, 1.0, rad + tnl::PI / 2, ghs[RED], true, false);
					SetDrawBright(255, 255, 255);
				}
				else {
					SetDrawBright(255, 0, 0);
					DrawRotaGraph(x, y, 1.0, rad + tnl::PI / 2, ghs[RED], true, false);
					SetDrawBright(255, 255, 255);
				}
				break;

			case BLUE:
				if (damageAnimTime < 0.1) {
					SetDrawBright(0, 0, 0);
					DrawRotaGraph(x, y, 1.0, rad + tnl::PI / 2, ghs[BLUE], true, false);
					SetDrawBright(255, 255, 255);
				}
				else {
					SetDrawBright(0, 0, 255);
					DrawRotaGraph(x, y, 1.0, rad + tnl::PI / 2, ghs[BLUE], true, false);
					SetDrawBright(255, 255, 255);
				}
				break;

			case YELLOW:
				if (damageAnimTime < 0.1) {
					SetDrawBright(0, 0, 0);
					DrawRotaGraph(x, y, 1.0, rad + tnl::PI / 2, ghs[YELLOW], true, false);
					SetDrawBright(255, 255, 255);
				}
				else {
					SetDrawBright(255, 255, 0);
					DrawRotaGraph(x, y, 1.0, rad + tnl::PI / 2, ghs[YELLOW], true, false);
					SetDrawBright(255, 255, 255);
				}
				break;

			}
		}
		else {
			// - �_���[�W���󂯂Ă��Ȃ����̕`��
			switch (machine.element)
			{
			case RED:
				DrawRotaGraph(x, y, 1.0, rad + tnl::PI / 2, ghs[RED], true, false);

				break;

			case BLUE:
				DrawRotaGraph(x, y, 1.0, rad + tnl::PI / 2, ghs[BLUE], true, false);
			
				break;

			case YELLOW:
				DrawRotaGraph(x, y, 1.0, rad + tnl::PI / 2, ghs[YELLOW], true, false);

				break;

			}

		}
	}

}

void Enemy::Time(float delta_time) {
	// --- ���Ԃ̊Ǘ� ---
	totalTime += delta_time;
	reloadingTime += delta_time;

	// --�@�_���[�W�A�j���[�V�����̂��߂̃J�E���g --
	if (damageAnimFlag) {
		damageAnimTime += delta_time;
		if (damageAnimTime >= ENEMY_DAMAGE_ANIMTIME) {
			damageAnimTime = 0;
			damageAnimFlag = false;
		}
	}

}

void Enemy::Shot(float delta_time) {
	// --- �G�̎ˌ����Ǘ� ---

	// ���C�t���O��܂�
	fs_shotFlag = false;
	// �ꎞ�ϐ�
	int num = 0;
	float deviationRad = 0;		// �΍��ˌ��p�x�v�Z�p

	// ���˃^�C�~���O�ɂȂ�����A�t���O�𗧂Ă�
	if (start_fire_time <= totalTime) {
		shotFlag = true;
	}

	// �t���O�������Ă��鎞�����ˌ�
	if (shotFlag) {

		if (!deadFlag) {
			// �����[�h�^�C���ɉ����Ďˌ����{
			if (reloadingTime >= machine.reloadTime) {
				// ���C�t���O�𗧂Ă�
				fs_shotFlag = true;
				switch (s_pattern)
				{
				// �v���C���[�Ɍ������ĒP���Ɏˌ�
				case 0:
					for (int i = 0; i < ENEMY_SNUM; i++) {
						// �t���O�������ĂȂ��e��T���āA���W�Z�b�g
						if (shot[i].flag == false) {
							shot[i].flag = true;
							shot[i].x = x;
							shot[i].y = y;
							shot[i].rad = rad;

							break;
						}
					}

					break;

				// �v���C���[�Ɍ������ĎU�e����
				case 1:
					for (int i = 0; i < ENEMY_SNUM; i++) {
						// �t���O�������ĂȂ��e��T���āA���W�Z�b�g
						if (shot[i].flag == false) {
							// �����擾
							if (num == 0) {
								srand((unsigned int)time(0));
							}
							if (num < 5) {
								shot[i].flag = true;
								shot[i].x = x;
								shot[i].y = y;
								shot[i].rad = rad - (60*tnl::PI/180) + ((rand()%120)*tnl::PI/180);
							} 
							num++;
						}
						if (num >= 5) {
							break;
						}
					}

					break;

				// �v���C���[�Ɍ�������30���̃����_���Ȋp�x�ɔ���
				case 2:
					for (int i = 0; i < ENEMY_SNUM; i++) {
						// �t���O�������ĂȂ��e��T���āA���W�Z�b�g
						if (shot[i].flag == false) {
							// �����擾
							
							shot[i].flag = true;
							shot[i].x = x;
							shot[i].y = y;
							shot[i].rad = rad - (60 * tnl::PI / 180) + ((rand() % 120) * tnl::PI / 180);
							
							break;
						}
					}

					break;
				
				// �΍��ˌ��̂悤�Ȏˌ����s��
				// �v���C���[�̈ړ������Ɍ����ăV���b�g
				case 3:
					for (int i = 0; i < ENEMY_SNUM; i++) {
						// �t���O�������ĂȂ��e��T���āA���W�Z�b�g
						if (shot[i].flag == false) {
							// �΍��p�x�����߂�
							deviationRad = rad - preRad;
							shot[i].flag = true;
							shot[i].x = x;
							shot[i].y = y;
							// �΍��p�x 
							//shot[i].rad = rad + deviationRad * (rand() % 30 * (tnl::PI/180));
							shot[i].rad = rad + deviationRad * (rand() % 40);
							
							break;
						}
					}

					break;

				// 3�����ˌ����s��(15�������炷)
				case 4:
					for (int i = 0; i < ENEMY_SNUM; i++) {
						// �t���O�������ĂȂ��e��T���āA���W�Z�b�g
						if (shot[i].flag == false) {
							
							if (num == 0) {
								shot[i].flag = true;
								shot[i].x = x;
								shot[i].y = y;
								shot[i].rad = rad -  15 * tnl::PI / 180;

							}

							if (num == 1) {
								shot[i].flag = true;
								shot[i].x = x;
								shot[i].y = y;
								shot[i].rad = rad;
							}

							if (num == 2) {
								shot[i].flag = true;
								shot[i].x = x;
								shot[i].y = y;
								shot[i].rad = rad + 15 * tnl::PI / 180;
							}

							num++;
						}
						if (num >= 3) {
							break;
						}
					}
					
					break;

				// �z�[�~���O�e��ł�
				case 5:
					// �z�[�~���O�e�t���O�𗧂Ă�
					hormingFlag = true;
					for (int i = 0; i < ENEMY_SNUM; i++) {
						// �t���O�������ĂȂ��e��T���āA���W�Z�b�g
						if (shot[i].flag == false) {
							shot[i].flag = true;
							shot[i].x = x;
							shot[i].y = y;
							shot[i].rad = rad;

							break;
						}
					}

					break;

				
				// 12�����ɐ��ˌ�
				case 6:
					for (int i = 0; i < ENEMY_SNUM; i++) {
						// �t���O�������ĂȂ��e��T���āA���W�Z�b�g
						if (shot[i].flag == false) {

							shot[i].flag = true;
							shot[i].x = x;
							shot[i].y = y;
							shot[i].rad = rad + ((-50 + 10 * num) * tnl::PI / 180);

							num++;
						}
						if (num >= 10) {
							break;
						}
					}

					break;

				// �΍��ˌ� + 3�����V���b�g
				case 7:
					deviationRad = rad - preRad;
					for (int i = 0; i < ENEMY_SNUM; i++) {
						// �t���O�������ĂȂ��e��T���āA���W�Z�b�g
						if (shot[i].flag == false) {
							// �΍��p�x�����߂�
							if (num == 0) {
								deviationRad *= (rand() % 40);
								shot[i].flag = true;
								shot[i].x = x;
								shot[i].y = y;
								// �΍��p�x 
								shot[i].rad = rad - (15 * tnl::PI / 180) + deviationRad;
							}

							if (num == 1) {
								shot[i].flag = true;
								shot[i].x = x;
								shot[i].y = y;
								// �΍��p�x 
								shot[i].rad = rad + deviationRad;
							}

							if (num == 2) {
								shot[i].flag = true;
								shot[i].x = x;
								shot[i].y = y;
								// �΍��p�x 
								shot[i].rad = rad + (15 * tnl::PI / 180) + deviationRad;
							}

							num++;

						}

						if( num >= 3){
							break;
						}

					}

				// �z�[�~���O�e + ����7����ł�
				case 8:
					// �z�[�~���O�e�t���O�𗧂Ă�
					hormingFlag = true;
					for (int i = 0; i < ENEMY_SNUM; i++) {
						// �t���O�������ĂȂ��e��T���āA���W�Z�b�g
						if (shot[i].flag == false) {


							shot[i].flag = true;
							shot[i].x = x;
							shot[i].y = y;
							shot[i].rad = rad + ((-60 + 20 * num) * tnl::PI / 180);

							num++;
						}
						if (num >= 7) {
							break;
						}
					}

					break;

					// �v���C���[�Ɍ������Ă����Ƒ�R�̎U�e����
				case 9:
					for (int i = 0; i < ENEMY_SNUM; i++) {
						// �t���O�������ĂȂ��e��T���āA���W�Z�b�g
						if (shot[i].flag == false) {
							// �����擾
							if (num == 0) {
								srand((unsigned int)time(0));
								num++;
							}
							else {
								shot[i].flag = true;
								shot[i].x = x;
								shot[i].y = y;
								shot[i].rad = rad - (80 * tnl::PI / 180) + ((rand() % 160) * tnl::PI / 180);
							}
							num++;
						}
						if (num >= 10) {
							break;
						}
					}

					break;


				
				// switch End 
				}

				// �����[�h�^�C���ĊJ
				reloadingTime = 0;
			}
		}

		// �t���O�������Ă���e�̐����`�F�b�N
		int shotCount = 0;

		// �t���O�������Ă���e�����A�e���ړ�������
		for (int i = 0; i < ENEMY_SNUM; i++) {
			if (shot[i].flag) {
				// �z�[�~���O�t���O�������Ă���΁A�z�[�~���O�e��ł�
				if (hormingFlag) {
					float deviationRad2 = rad - preRad;
					shot[i].rad += deviationRad2;
					shot[i].x += shot[i].speed * delta_time * cos(shot[i].rad);
					shot[i].y += shot[i].speed * delta_time * sin(shot[i].rad);
					shot[i].time += delta_time;
				}
				else {

					shot[i].x += shot[i].speed * delta_time * cos(shot[i].rad);
					shot[i].y += shot[i].speed * delta_time * sin(shot[i].rad);
					shot[i].time += delta_time;
				}

				// ��ʊO�ɒe���͂ݏo������A�t���O��߂�
				if ((shot[i].x > WIDTH - MENU_WIDTH) ||
					(shot[i].x < MARGINE) ||
					(shot[i].y > HEIGHT - MARGINE) ||
					(shot[i].y < MARGINE)) {

					shot[i].flag = false;
				}

				// �{�G�N���X�����˂��Ă���e���J�E���g
				shotCount++;
			}
		}

		// ���˒��̒e��0�ł��邩�ǂ����̃t���O��Ԃ�(All()�Ŏg�p)
		if (shotCount == 0) {
			shootingFlag = false;
		}
		else {
			shootingFlag = true;
		}

		// ���S�t���Otrue, ���˒��̒e��0�Ȃ�΁A�{�C���X�^���X������
		if (shotCount == 0 && deadFlag && deadEffectEndFlag) {
			endFlag = true;
		}

	}
}

void Enemy::DamageCalc() {
	// --- ��e�_���[�W���v�Z�F�O������Ăяo���B ---
	// �_���[�W�̎󂯎���SetDamage�Ŏ��{
	// �_���[�W���󂯂������ɉ����ă_���[�W�ω�
	int tempElement = machine.element;
	int calcType;
	// �e��e�����_���[�W�t���O��߂�
	fs_reflect = false;
	fs_damage = false;
	if (damageFlag) {
		// -- �������U���̏ꍇ�̌v�Z -- 
		if (damagedElement == NON_ELEMENT) {
			if (machine.defence - damagedNum <= 0) {
				machine.hp += (int)machine.defence - damagedNum;
				damageEffectFlag = true;
				damageAnimFlag = true;
			}
			else {
				// �m�[�_���[�W
				fs_reflect = true;
			}
		}
		else {
		// -- �����U���̏ꍇ�̌v�Z
			// - ����񂯂�A���S���Y�����Q�l -
			int temp = (tempElement - damagedElement + 3) % 3;
			if (temp == 0) {
				// ������
				calcType = 0;
			}
			else if (temp == 1) {
				// �����i���ʂ͂��܂���)
				calcType = 1;
			}
			else {
				// ���� (��_��˂��ꂽ)
				calcType = 2;
			}

			// - ����񂯂�̌��ʂɉ����ă_���[�W�v�Z -
			switch (calcType) {
			case 0:
				// ������
				if (machine.defence - damagedNum <= 0) {
					machine.hp += (int)machine.defence - damagedNum;
					damageEffectFlag = true;
					damageAnimFlag = true;
					fs_damage = true;
				}
				else {
					// �m�[�_���[�W
					fs_reflect = true;
				}
				break;

			case 1:
				// ����(�_���[�W 1/2)
				if (machine.defence - damagedNum / 2 <= 0) {
					// �_���[�W�G�t�F�N�g�L����
					machine.hp += (int)machine.defence - damagedNum / 2;
					damageEffectFlag = true;
					damageAnimFlag = true;
					fs_damage = true;
				}
				else {
					// �m�[�_���[�W
					fs_reflect = true;
				}
				break;
			case 2:
				// ����(�_���[�W2�{)
				if (machine.defence - damagedNum * 2 <= 0) {
					machine.hp += (int)machine.defence - damagedNum * 2;
					damageEffectFlag = true;
					damageAnimFlag = true;
					fs_damage = true;
				}
				else {
					// �m�[�_���[�W
					fs_reflect = true;
				}
				break;
			}
		}

		// - �_���[�W�t���O��߂� -//
		damageFlag = false;

		// - hp��0�ȉ��Ȃ�΁A���S�t���O�𗧂Ă� -
		if (machine.hp <= 0) {
			machine.hp = 0;
			deadFlag = true;
			// �{�X�L�����ł������ꍇ�A�{�X�L�������S�t���O�𗧂Ă�
			// (1�t���[���̂ݗL���BGetBossDead()���ŏ���������)
			if (isBoss) {
				isBossDead = true;
			}
		}
	}
}

bool Enemy::CircleCollision(double cir1, double cir2, double cirX1, double cirX2, double cirY1, double cirY2) {
	// --- �~�`�����蔻�� (��ɏ��񊮗��m�F���Ɏg�p ---
	// cir1, cir2 : �e�X�̉~�̔��a
	// cirNn : �e�X�̉~�̈ʒuX,Y
	double length = cir1 + cir2;
	double xLenght = cirX1 - cirX2;
	double yLength = cirY1 - cirY2;

	if (length * length >= xLenght * xLenght + yLength * yLength) {
		return true;
	}
	else {
		return false;
	}
}

bool Enemy::CheckOutOfWindow() {
	// --- �G�@����ʊO�ɑ��݂��邩���`�F�b�N ---
	if ((x - width > WIDTH - MENU_WIDTH) ||
		(x + width < MARGINE) ||
		(y - height > HEIGHT - MARGINE) ||
		(y + height < MARGINE)) {

		return true;
	}
	else {
		return false;
	}
}

void Enemy::GetPosition(double* x, double* y) {
	// --- ���W���O���ɗ^���� --- 
	*x = this->x;
	*y = this->y;
}

void Enemy::SetPosition(double x, double y) {
	// --- �W�I(��Ƀv���C���[)�̈ʒu���i�[ --- 
	this->targetX = x;
	this->targetY = y;
}

bool Enemy::GetDeadFlag() {
	// --- �j�󂳂ꂽ���ۂ��̃t���O��Ԃ� ---
	return deadFlag;
}

int Enemy::GetCollision() {
	// --- �@�̂̓����蔻�蔼�a�FCollision��Ԃ� ---
	// �摜�T�C�Y�̉������c���̕��� / 2 ��Ԃ�
	int collision = (this->width / 2 + this->height / 2) / 2;
	return collision;
}

int Enemy::GetShotCollision(int index) {
	// --- ����̃C���f�b�N�X�̒e��Collision��Ԃ� ---
	// �摜�T�C�Y�̉������c���̕��� / 2 ��Ԃ�
	int collision = (this->shot[index].width / 2 + this->shot[index].height / 2) / 2;
	return collision;
}


bool Enemy::GetShotPosition(int index, double* x, double* y) {
	// --- ����̃C���f�b�N�X�̒e�̈ʒu��Ԃ� ---
	// �t���O�� true : �t�B�[���h��ɑ��݂���e�݈̂ʒu��Ԃ�
	if (shot[index].flag) {
		*x = shot[index].x;
		*y = shot[index].y;
		return true;
	}
	else {
		return false;
	}
}

int Enemy::GetShotElement(int index) {
	// --- ����̃C���f�b�N�X�̒e�̑�����Ԃ� ---
	return shot[index].element;
}

int Enemy::GetShotDamage(int index) {
	// --- �w�肵���e�̃_���[�W��Ԃ� ---
	return shot[index].damage;
}

bool Enemy::GetDeadMoment() {
	// --- ���S���ʉ���炷�t���O---
	return deadMomentFlag;
}

void Enemy::GetVelocity(double* vx, double* vy) {
	// --- ���x�����Q�Ɠn�� ---
	*vx = this->velX;
	*vy = this->velY;

}

bool Enemy::GetShotFlag() {
	// --- ���C�������̃t���O��Ԃ� --- 
	return fs_shotFlag;
}

int Enemy::GetScore(float totalTime) {
	// --- �X�R�A��Ԃ� ---
	// �v�Z���F�G�𑁂��|�����A��{�X�R�A��葽���l��Ԃ�(�ő�30%)
	int temp = machine.score;
	float tempTime = out_time - in_time;
	if (totalTime < (in_time + tempTime * 0.1 )) {
		temp += (int)temp * 0.3;
	
	}
	else if (totalTime < in_time + tempTime * 0.2) {
		temp += (int)temp * 0.25;
	}
	else if (totalTime < in_time + tempTime * 0.3) {
		temp += (int)temp * 0.225;

	}
	else if (totalTime < in_time + tempTime * 0.4) {
		temp += (int)temp * 0.2;

	}
	else if (totalTime < in_time + tempTime * 0.5) {
		temp += (int)temp * 0.15;
	
	}
	else if (totalTime < in_time + tempTime * 0.7) {
		temp += (int)temp * 0.1;
	
	}
	
	return temp;

}

int Enemy::GetItem() {
	// --- �G�ɃZ�b�g�����A�C�e����Ԃ� ---
	return item;
}

bool Enemy::GetBossExist() {
	// --- �{�X�t���O��Ԃ� ---
	return isBoss;
}

bool Enemy::GetBossDead() {
	// --- �{�X�L���������S�����u��(1�t���[��)�Atrue��Ԃ� ---
	if (isBossDead) {
		if (!isBossDeadInit) {
			isBossDeadInit = true;
			return true;
		}
		return false;
	}
	else {
		return false;
	}

}

bool Enemy::GetRefrectFlag() {
	// --- �G���e��e�������ǂ����̃t���O��Ԃ� ---
	return fs_reflect;
}

bool Enemy::GetDamageFlag() {
	// --- �G���_���[�W���󂯂����ǂ����̃t���O��Ԃ� ---
	return fs_damage;
}

void Enemy::SetShotFlag(int index, bool flag) {
	// --- �w�肵���e�̃t���O��ύX ---
	// (��ɒe���������������Ɏg�p)
	shot[index].flag = flag;
}

void Enemy::SetDamage(int element, int damage) {
	// --- ��e���ɌĂяo���B�e�̑������_���[�W�Z�b�g ---
	this->damagedElement = element;
	this->damagedNum = damage;
	damageFlag = true;
}

void Enemy::MoveControl(int m_pattern) {
	// --- �G�̍s��(�ڕW�ʒu)���s���p�^�[���ɉ����čX�V����֐� ---
	// �ꎞ�ϐ�
	int t_count = 0;
	int t_collision = 0;
	int t_point = 0;
	switch (m_pattern)
	{

	case 0:
		// -- ���U --
		// �v���C���[�Ɍ������ēːi�𑱂���
		refX = this->targetX;
		refY = this->targetY;
		if (!isBoss) {
			if (out_time <= totalTime) {
				// �A�Ҏ��ԂɂȂ�����A�A�҃|�C���g�ֈړ�
				refX = outX;
				refY = outY;
			}
		}

		break;

	case 1:
		// -- �������W����(���[�v����) --
		t_point = moveK[0];		// ���񂷂�|�C���g��
		// k3:X1, k4: Y1, k5:X2, ...��ڕW�n�_�Ƃ��A
		// ���J��ɏ��s������
		if (moveK[1] == 0) {
			// ����������
			moveK[1] = 3;
		}

		// ���s����
		t_count = moveK[1];
		t_collision = moveK[2];
		refX = (double)moveK[t_count];
		refY = (double)moveK[t_count + 1];
		// �ڕW�ʒu�֓��B�������̔���
		if (CircleCollision(t_collision, t_collision, refX, x, refY, y)) {
			// ���̖ڕW�ʒu�֐i��
			moveK[1] += 2;
			// �ŏI�ڕW�ʒu�֓���������A�ҋ@
			if (moveK[1] >= 11) {
				moveK[1] = 11;
			}

		}
		else if (moveK[1] > 3 + t_point * 2) {
			// ���񂷂�|�C���g�����
			moveK[1] = 3 + t_point * 2;
		}

		if (!isBoss) {
			if (out_time <= totalTime) {
				// �A�Ҏ��ԂɂȂ�����A�A�҃|�C���g�ֈړ�
				refX = outX;
				refY = outY;
			}
		}

		break;

	case 2: 
		// - �������W���s(���[�v�L)
		// -- �������W����(���[�v����) --
		t_point = moveK[0];		// ���񂷂�|�C���g��
		// k3:X1, k4: Y1, k5:X2, ...��ڕW�n�_�Ƃ��A
		// ���J��ɏ��s������
		if (moveK[1] == 0) {
			// ����������
			moveK[1] = 3;
		}

		// ���s����
		t_count = moveK[1];
		t_collision = moveK[2];
		refX = (double)moveK[t_count];
		refY = (double)moveK[t_count + 1];
		// �ڕW�ʒu�֓��B�������̔���
		if (CircleCollision(t_collision, t_collision, refX, x, refY, y)) {
			// ���̖ڕW�ʒu�֐i��
			moveK[1] += 2;
			// �ŏI�ڕW�ʒu�֓���������A�ŏ��̍��W�ɖ߂�(���[�v����)
			if (moveK[1] >= 11) {
				moveK[1] = 3;
			}

		}
		else if (moveK[1] > 3 + t_point * 2) {
			// ���񂷂�|�C���g�����
			moveK[1] = 3 + t_point * 2;
		}

		if (!isBoss) {
			if (out_time <= totalTime) {
				// �A�Ҏ��ԂɂȂ�����A�A�҃|�C���g�ֈړ�
				refX = outX;
				refY = outY;
			}
		}

		break;
	}
}

void Enemy::SetOutTime(float outTime) {
	// --- �G�̋A�����Ԃ�ύX���� ---
	this->out_time = outTime;
}

void Enemy::GetMachine(int type) {
	// --- type �ɉ����ăR�s�[����G�@(machine)���擾 ---
	// �� �摜�̏�������Draw()���Ŏ��{(DxLib_Init()����{�̂���)
	// (�ǁX�Acsv������ǂݍ��݂ł���悤�ɂ�����)
	switch (type)
	{
		// ���ʂ̏���
		machine = {
			.gh = 0, .width = 0, .height = 0
		};

	// -- RED�@��(100 ~ 199) --
	case 100:
		// - ���ʋ@, �ʒu���ߐ���, 
		machine = {
			.element = RED,
			.hp = 100,
			.defence = 10,
			.mass = 10,
			.refAccMaxNormal = 0.1,
			.refAccMaxShifted = 0.2,
			.refVelMaxNormal = 2.0,
			.refVelMaxShifted = 3.0,
			.kp = 16.0,
			.ki = 0.01,
			.kd = 3.5,
			.reloadTime = 0.5,
			.score = 100
		};
		break;

	case 101:
		// - Mob1, 
		machine = {
			.element = RED,
			.hp = 50,
			.defence = 5,
			.mass = 3,
			.refAccMaxNormal = 0.3,
			.refAccMaxShifted = 0.3,
			.refVelMaxNormal = 1.5,
			.refVelMaxShifted = 1.5,
			.kp = 2.0,
			.ki = 0.0001,
			.kd = 0.001,
			.reloadTime = 0.8,
			.score = 80
		};
		break;

	case 102:
		// - Mob2, 
		machine = {
			.element = RED,
			.hp = 350,
			.defence = 20,
			.mass = 20,
			.refAccMaxNormal = 0.1,
			.refAccMaxShifted = 0.1,
			.refVelMaxNormal = 0.8,
			.refVelMaxShifted = 0.8,
			.kp = 15.0,
			.ki = 0.01,
			.kd = 3.5,
			.reloadTime = 0.8,
			.score = 150
		};
		break;

	case 103:
		// - Mob3, 
		machine = {
			.element = RED,
			.hp = 100,
			.defence = 10,
			.mass = 10,
			.refAccMaxNormal = 0.1,
			.refAccMaxShifted = 0.2,
			.refVelMaxNormal = 2.0,
			.refVelMaxShifted = 3.0,
			.kp = 16.0,
			.ki = 0.01,
			.kd = 3.5,
			.reloadTime = 0.5,
			.score = 100
		};
		break;

	case 104:
		// - Mob4
		machine = {
			.element = RED,
			.hp = 100,
			.defence = 10,
			.mass = 10,
			.refAccMaxNormal = 0.1,
			.refAccMaxShifted = 0.2,
			.refVelMaxNormal = 2.0,
			.refVelMaxShifted = 3.0,
			.kp = 16.0,
			.ki = 0.01,
			.kd = 3.5,
			.reloadTime = 0.5,
			.score = 100
		};
		break;

	case 105:
		// - ���ʋ@, �ʒu���ߐ���, 
		machine = {
			.element = RED,
			.hp = 250,
			.defence = 10,
			.mass = 10,
			.refAccMaxNormal = 0.5,
			.refAccMaxShifted = 0.5,
			.refVelMaxNormal = 3.0,
			.refVelMaxShifted = 3.0,
			.kp = 16.0,
			.ki = 0.01,
			.kd = 3.5,
			.reloadTime = 0.15,
			.score = 120
		};
		break;

	case 106:
		// - Mob6
		machine = {
			.element = RED,
			.hp = 100,
			.defence = 10,
			.mass = 10,
			.refAccMaxNormal = 0.1,
			.refAccMaxShifted = 0.2,
			.refVelMaxNormal = 2.0,
			.refVelMaxShifted = 3.0,
			.kp = 16.0,
			.ki = 0.01,
			.kd = 3.5,
			.reloadTime = 0.5,
			.score = 100
		};
		break;

	case 107:
		// - Mob7, 
		machine = {
			.element = RED,
			.hp = 100,
			.defence = 10,
			.mass = 10,
			.refAccMaxNormal = 0.1,
			.refAccMaxShifted = 0.2,
			.refVelMaxNormal = 2.0,
			.refVelMaxShifted = 3.0,
			.kp = 16.0,
			.ki = 0.01,
			.kd = 3.5,
			.reloadTime = 0.5,
			.score = 100
		};
		break;

	case 108:
		// - ���ʋ@, �ʒu���ߐ���, 
		machine = {
			.element = RED,
			.hp = 500,
			.defence = 10,
			.mass = 10,
			.refAccMaxNormal = 0.2,
			.refAccMaxShifted = 0.2,
			.refVelMaxNormal = 3.5,
			.refVelMaxShifted = 3.5,
			.kp = 16.0,
			.ki = 0.01,
			.kd = 3.5,
			.reloadTime = 3.0,
			.score = 500
		};
		break;

	case 109:
		// - ���ʋ@, �ʒu���ߐ���, 
		machine = {
			.element = RED,
			.hp = 4000,
			.defence = 10,
			.mass = 25,
			.refAccMaxNormal = 0.2,
			.refAccMaxShifted = 0.2,
			.refVelMaxNormal = 1.5,
			.refVelMaxShifted = 1.5,
			.kp = 16.0,
			.ki = 0.01,
			.kd = 3.5,
			.reloadTime = 3.5,
			.score = 2000
		};
		isBoss = true;		// �{�X�L�����t���O�𗧂Ă�
		break;

	case 110:
		// - ���ʋ@, �ʒu���ߐ���, 
		machine = {
			.element = RED,
			.hp = 4000,
			.defence = 10,
			.mass = 25,
			.refAccMaxNormal = 0.2,
			.refAccMaxShifted = 0.2,
			.refVelMaxNormal = 1.5,
			.refVelMaxShifted = 1.5,
			.kp = 16.0,
			.ki = 0.01,
			.kd = 3.5,
			.reloadTime = 3.5,
			.score = 2000
		};
		isBoss = true;		// �{�X�L�����t���O�𗧂Ă�
		break;

	case 111:
		// - Ch.1 �{�X(1/3�́FRED)
		machine = {
			.element = RED,
			.hp = 4000,
			.defence = 5,
			.mass = 10,
			.refAccMaxNormal = 0.5,
			.refAccMaxShifted = 0.5,
			.refVelMaxNormal = 4.0,
			.refVelMaxShifted = 4.0,
			.kp = 16.0,
			.ki = 0.01,
			.kd = 3.5,
			.reloadTime = 3.5,
			.score = 2000
		};
		isBoss = true;		// �{�X�L�����t���O�𗧂Ă�
		break;

	// -- BLUE�@��(200~299) --
	case 200:
		// - ���ʋ@, �ʒu���ߐ���, 
		machine = {
			.element = BLUE,
			.hp = 100,
			.defence = 10,
			.mass = 12,
			.refAccMaxNormal = 0.1,
			.refAccMaxShifted = 0.2,
			.refVelMaxNormal = 1.8,
			.refVelMaxShifted = 3.0,
			.kp = 16.0,
			.ki = 0.01,
			.kd = 3.5,
			.reloadTime = 1.8,
			.score = 100
		};
		break;

	case 201:
		// - Mob1
		machine = {
			.element = BLUE,
			.hp = 100,
			.defence = 10,
			.mass = 12,
			.refAccMaxNormal = 0.1,
			.refAccMaxShifted = 0.2,
			.refVelMaxNormal = 1.8,
			.refVelMaxShifted = 3.0,
			.kp = 16.0,
			.ki = 0.01,
			.kd = 3.5,
			.reloadTime = 1.8,
			.score = 100
		};
		break;

	case 202:
		// - Mob2 
		machine = {
			.element = BLUE,
			.hp = 350,
			.defence = 20,
			.mass = 20,
			.refAccMaxNormal = 0.1,
			.refAccMaxShifted = 0.1,
			.refVelMaxNormal = 0.8,
			.refVelMaxShifted = 0.8,
			.kp = 15.0,
			.ki = 0.01,
			.kd = 3.5,
			.reloadTime = 0.8,
			.score = 150
		};
		break;

	case 203:
		// - Mob3
		machine = {
			.element = BLUE,
			.hp = 50,
			.defence = 5,
			.mass = 3.0,
			.refAccMaxNormal = 0.3,
			.refAccMaxShifted = 0.3,
			.refVelMaxNormal = 2.5,
			.refVelMaxShifted = 2.5,
			.kp = 6.0,
			.ki = 0.001,
			.kd = 0.001,
			.reloadTime = 1.8,
			.score = 100
		};
		break;

	case 204:
		// - Mob4, 
		machine = {
			.element = BLUE,
			.hp = 350,
			.defence = 25,
			.mass = 30,
			.refAccMaxNormal = 0.05,
			.refAccMaxShifted = 0.05,
			.refVelMaxNormal = 2.0,
			.refVelMaxShifted = 2.0,
			.kp = 16,
			.ki = 0.01,
			.kd = 3.5,
			.reloadTime = 1.5,
			.score = 150
		};
		break;

	case 205:
		// - Mob5 
		machine = {
			.element = BLUE,
			.hp = 100,
			.defence = 10,
			.mass = 12,
			.refAccMaxNormal = 0.1,
			.refAccMaxShifted = 0.2,
			.refVelMaxNormal = 1.8,
			.refVelMaxShifted = 3.0,
			.kp = 16.0,
			.ki = 0.01,
			.kd = 3.5,
			.reloadTime = 1.8,
			.score = 100
		};
		break;

	case 206:
		// - Mob6
		machine = {
			.element = BLUE,
			.hp = 100,
			.defence = 10,
			.mass = 12,
			.refAccMaxNormal = 0.1,
			.refAccMaxShifted = 0.2,
			.refVelMaxNormal = 1.8,
			.refVelMaxShifted = 3.0,
			.kp = 16.0,
			.ki = 0.01,
			.kd = 3.5,
			.reloadTime = 1.8,
			.score = 100
		};
		break;

	case 207:
		// - Mob7
		machine = {
			.element = BLUE,
			.hp = 100,
			.defence = 10,
			.mass = 12,
			.refAccMaxNormal = 0.1,
			.refAccMaxShifted = 0.2,
			.refVelMaxNormal = 1.8,
			.refVelMaxShifted = 3.0,
			.kp = 16.0,
			.ki = 0.01,
			.kd = 3.5,
			.reloadTime = 1.8,
			.score = 100
		};
		break;

	case 208:
		// - Mob8
		machine = {
			.element = BLUE,
			.hp = 500,
			.defence = 10,
			.mass = 10,
			.refAccMaxNormal = 0.2,
			.refAccMaxShifted = 0.2,
			.refVelMaxNormal = 3.5,
			.refVelMaxShifted = 3.5,
			.kp = 16.0,
			.ki = 0.01,
			.kd = 3.5,
			.reloadTime = 3.0,
			.score = 500
		};
		break;

	case 209:
		// - M0b9
		machine = {
			.element = BLUE,
			.hp = 4000,
			.defence = 10,
			.mass = 25,
			.refAccMaxNormal = 0.2,
			.refAccMaxShifted = 0.2,
			.refVelMaxNormal = 1.5,
			.refVelMaxShifted = 1.5,
			.kp = 16.0,
			.ki = 0.01,
			.kd = 3.5,
			.reloadTime = 3.5,
			.score = 2000
		};
		isBoss = true;		// �{�X�L�����t���O�𗧂Ă�
		break;

	case 210:
		// - Mob10
		machine = {
			.element = BLUE,
			.hp = 4000,
			.defence = 10,
			.mass = 25,
			.refAccMaxNormal = 0.2,
			.refAccMaxShifted = 0.2,
			.refVelMaxNormal = 1.5,
			.refVelMaxShifted = 1.5,
			.kp = 16.0,
			.ki = 0.01,
			.kd = 3.5,
			.reloadTime = 3.5,
			.score = 2000
		};
		isBoss = true;		// �{�X�L�����t���O�𗧂Ă�
		break;

	case 211:
		// - Mob11
		machine = {
			.element = BLUE,
			.hp = 4000,
			.defence = 20,
			.mass = 10,
			.refAccMaxNormal = 0.5,
			.refAccMaxShifted = 0.5,
			.refVelMaxNormal = 4.0,
			.refVelMaxShifted = 4.0,
			.kp = 16.0,
			.ki = 0.01,
			.kd = 3.5,
			.reloadTime = 3.5,
			.score = 2000
		};
		isBoss = true;		// �{�X�L�����t���O�𗧂Ă�
		break;

		// -- YELLOW�@��(300~399) --
	case 300:
		// - ���ʋ@, �ʒu���ߐ���, 
		machine = {
			.element = YELLOW,
			.hp = 100,
			.defence = 10,
			.mass = 5,
			.refAccMaxNormal = 0.2,
			.refAccMaxShifted = 0.2,
			.refVelMaxNormal = 2.5,
			.refVelMaxShifted = 3.0,
			.kp = 16.0,
			.ki = 0.01,
			.kd = 3.5,
			.reloadTime = 1.0,
			.score = 100
		};
		break;

	case 301:
		// - M0b1
		machine = {
			.element = YELLOW,
			.hp = 100,
			.defence = 10,
			.mass = 5,
			.refAccMaxNormal = 0.2,
			.refAccMaxShifted = 0.2,
			.refVelMaxNormal = 2.5,
			.refVelMaxShifted = 3.0,
			.kp = 16.0,
			.ki = 0.01,
			.kd = 3.5,
			.reloadTime = 1.0,
			.score = 100
		};
		break;

	case 302:
		// - M0b2
		machine = {
			.element = YELLOW,
			.hp = 350,
			.defence = 20,
			.mass = 20,
			.refAccMaxNormal = 0.1,
			.refAccMaxShifted = 0.1,
			.refVelMaxNormal = 0.8,
			.refVelMaxShifted = 0.8,
			.kp = 15,
			.ki = 0.01,
			.kd = 3.5,
			.reloadTime = 0.8,
			.score = 150
		};
		break;

	case 303:
		// - M0b3
		machine = {
			.element = YELLOW,
			.hp = 100,
			.defence = 10,
			.mass = 5,
			.refAccMaxNormal = 0.2,
			.refAccMaxShifted = 0.2,
			.refVelMaxNormal = 2.5,
			.refVelMaxShifted = 3.0,
			.kp = 6.01,
			.ki = 0.0001,
			.kd = 3.5,
			.reloadTime = 1.0,
			.score = 100
		};
		break;

	case 304:
		// - M0b4
		machine = {
			.element = YELLOW,
			.hp = 350,
			.defence = 25,
			.mass = 30,
			.refAccMaxNormal = 0.05,
			.refAccMaxShifted = 0.05,
			.refVelMaxNormal = 2.0,
			.refVelMaxShifted = 2.0,
			.kp = 16.0,
			.ki = 0.01,
			.kd = 3.5,
			.reloadTime = 1.5,
			.score = 150
		};
		break;

	case 305:
		// - M0b5
		machine = {
			.element = YELLOW,
			.hp = 250,
			.defence = 10,
			.mass = 10,
			.refAccMaxNormal = 0.5,
			.refAccMaxShifted = 0.5,
			.refVelMaxNormal = 3.0,
			.refVelMaxShifted = 3.0,
			.kp = 16.0,
			.ki = 0.01,
			.kd = 3.5,
			.reloadTime = 0.15,
			.score = 120
		};
		break;

	case 306:
		// - M0b6
		machine = {
			.element = YELLOW,
			.hp = 100,
			.defence = 10,
			.mass = 20,
			.refAccMaxNormal = 0.5,
			.refAccMaxShifted = 0.5,
			.refVelMaxNormal = 2.5,
			.refVelMaxShifted = 2.5,
			.kp = 5.0,
			.ki = 0.001,
			.kd = 0.005,
			.reloadTime = 1.0,
			.score = 100
		};
		break;

	case 307:
		// - M0b7
		machine = {
			.element = YELLOW,
			.hp = 700,
			.defence = 15,
			.mass = 25,
			.refAccMaxNormal = 0.2,
			.refAccMaxShifted = 0.2,
			.refVelMaxNormal = 1.5,
			.refVelMaxShifted = 1.5,
			.kp = 15.0,
			.ki = 0.01,
			.kd = 3.5,
			.reloadTime = 0.8,
			.score = 300
		};
		break;

	case 308:
		// - M0b8
		machine = {
			.element = YELLOW,
			.hp = 500,
			.defence = 10,
			.mass = 10,
			.refAccMaxNormal = 0.2,
			.refAccMaxShifted = 0.2,
			.refVelMaxNormal = 3.5,
			.refVelMaxShifted = 3.5,
			.kp = 16.0,
			.ki = 0.01,
			.kd = 3.5,
			.reloadTime = 3.0,
			.score = 300
		};
		break;

	case 309:
		// - M0b9
		machine = {
			.element = YELLOW,
			.hp = 4000,
			.defence = 10,
			.mass = 25,
			.refAccMaxNormal = 0.2,
			.refAccMaxShifted = 0.2,
			.refVelMaxNormal = 1.5,
			.refVelMaxShifted = 1.5,
			.kp = 16.0,
			.ki = 0.01,
			.kd = 3.5,
			.reloadTime = 3.5,
			.score = 3000
		};
		isBoss = true;
		break;

	case 310:
		// - M0b1
		machine = {
			.element = YELLOW,
			.hp = 4000,
			.defence = 10,
			.mass = 25,
			.refAccMaxNormal = 0.2,
			.refAccMaxShifted = 0.2,
			.refVelMaxNormal = 1.5,
			.refVelMaxShifted = 1.0,
			.kp = 16.0,
			.ki = 0.01,
			.kd = 3.5,
			.reloadTime = 3.5,
			.score = 3000
		};
		isBoss = true;
		break;

	case 311:
		// - Ch.2 �{�X(3/3�́FYellow)
		machine = {
			.element = YELLOW,
			.hp = 4000,
			.defence = 5,
			.mass = 10,
			.refAccMaxNormal = 0.5,
			.refAccMaxShifted = 0.5,
			.refVelMaxNormal = 4.0,
			.refVelMaxShifted = 4.0,
			.kp = 16.0,
			.ki = 0.01,
			.kd = 3.5,
			.reloadTime = 3.5,
			.score = 3000
		};
		isBoss = true;		// �{�X�L�����t���O�𗧂Ă�
		break;

	default:
		machine = {
			.element = RED,
			.hp = 100,
			.defence = 10,
			.mass = 10,
			.refAccMaxNormal = 0.1,
			.refAccMaxShifted = 0.2,
			.refVelMaxNormal = 2.0,
			.refVelMaxShifted = 3.0,
			.kp = 16.0,
			.ki = 0.01,
			.kd = 3.5,
			.reloadTime = 0.5,
			.score = 100
		};
		isBoss = true;		// �{�X�L�����t���O�𗧂Ă�
		break;
	}
}

void Enemy::GetShot(int stype) {
	// --- stype�ɉ����ăR�s�[����e��(Eshot)���擾 ---
	// �� �摜�̏�������Draw()���Ŏ��{(DxLib_Init()����{�̂���)
	// (�ǁX�Acsv������ǂݍ��݂ł���悤�ɂ�����)
	EShot tempEShot;
	// -- ���ʂ̏��������{ --
	for (int i = 0; i < SHOT_FRAME_MAX; i++) {
		tempEShot.gh[i] = 0;
	}
	tempEShot = {
		.type = 0, .flag = false, .x = 0, .y = 0, .width = 0, .height = 0, .rad = 0, .time = 0, .animFrame = 0
	};

	switch (stype) {

	// -- RED(100 ~ 199) -- 
	case 100:
		tempEShot = {
			.speed = 400,
			.element = RED,
			.damage = 20,
		};
		break;

	case 101:
		tempEShot = {
			.speed = 200,
			.element = RED,
			.damage = 15,
		};
		break;

	case 102:
		tempEShot = {
			.speed = 300,
			.element = RED,
			.damage = 18,
		};
		break;

	case 103:
		tempEShot = {
			.speed = 500,
			.element = RED,
			.damage = 22,
		};
		break;
	
	case 104:
		tempEShot = {
			.speed = 350,
			.element = RED,
			.damage = 18,
		};
		break;

	case 105:
		tempEShot = {
			.speed = 450,
			.element = RED,
			.damage = 30,
		};
		break;

	case 106:
		tempEShot = {
			.speed = 250,
			.element = RED,
			.damage = 28,
		};
		break;

	case 107:
		tempEShot = {
			.speed = 300,
			.element = RED,
			.damage = 18,
		};
		break;

	case 108:
		tempEShot = {
			.speed = 400,
			.element = RED,
			.damage = 35,
		};
		break;

	case 109:
		tempEShot = {
			.speed = 500,
			.element = RED,
			.damage = 40,
		};
		break;

	case 110:
		tempEShot = {
			.speed = 250,
			.element = RED,
			.damage = 35,
		};
		break;

	case 111:
		tempEShot = {
			.speed = 300,
			.element = RED,
			.damage = 30,
		};
		break;

	// -- BLUE(200 ~ 299) --
	case 200:
		tempEShot = {
			.speed = 600,
			.element = BLUE,
			.damage = 30,
		};
		break;

	case 201:
		tempEShot = {
			.speed = 200,
			.element = BLUE,
			.damage = 15,
		};
		break;

	case 202:
		tempEShot = {
			.speed = 300,
			.element = BLUE,
			.damage = 18,
		};
		break;

	case 203:
		tempEShot = {
			.speed = 500,
			.element = BLUE,
			.damage = 22,
		};
		break;

	case 204:
		tempEShot = {
			.speed = 350,
			.element = BLUE,
			.damage = 18,
		};
		break;

	case 205:
		tempEShot = {
			.speed = 450,
			.element = BLUE,
			.damage = 30,
		};
		break;

	case 206:
		tempEShot = {
			.speed = 250,
			.element = BLUE,
			.damage = 28,
		};
		break;

	case 207:
		tempEShot = {
			.speed = 300,
			.element = BLUE,
			.damage = 35,
		};
		break;

	case 208:
		tempEShot = {
			.speed = 400,
			.element = BLUE,
			.damage = 35,
		};
		break;

	case 209:
		tempEShot = {
			.speed = 500,
			.element = BLUE,
			.damage = 40,
		};
		break;

	case 210:
		tempEShot = {
			.speed = 250,
			.element = BLUE,
			.damage = 35,
		};
		break;

	case 211:
		tempEShot = {
			.speed = 300,
			.element = BLUE,
			.damage = 30,
		};
		break;
	
	// -- YELLOW(300 ~ 399) --
	case 300:
		tempEShot = {
			.speed = 300,
			.element = YELLOW,
			.damage = 40,
		};

	case 301:
		tempEShot = {
			.speed = 200,
			.element = YELLOW,
			.damage = 15,
		};
		break;

	case 302:
		tempEShot = {
			.speed = 300,
			.element = YELLOW,
			.damage = 18,
		};
		break;

	case 303:
		tempEShot = {
			.speed = 700,
			.element = YELLOW,
			.damage = 22,
		};
		break;

	case 304:
		tempEShot = {
			.speed = 350,
			.element = YELLOW,
			.damage = 18,
		};
		break;

	case 305:
		tempEShot = {
			.speed = 450,
			.element = YELLOW,
			.damage = 30,
		};
		break;

	case 306:
		tempEShot = {
			.speed = 250,
			.element = YELLOW,
			.damage = 28,
		};
		break;

	case 307:
		tempEShot = {
			.speed = 300,
			.element = YELLOW,
			.damage = 35,
		};
		break;

	case 308:
		tempEShot = {
			.speed = 225,
			.element = YELLOW,
			.damage = 35,
		};
		break;

	case 309:
		tempEShot = {
			.speed = 500,
			.element = YELLOW,
			.damage = 40,
		};
		break;

	case 310:
		tempEShot = {
			.speed = 250,
			.element = YELLOW,
			.damage = 35,
		};
		break;

	case 311:
		tempEShot = {
			.speed = 300,
			.element = YELLOW,
			.damage = 30,
		};
		break;

	}
	// temp��S�Ă̒e�ɓK�p
	for (int i = 0; i < ENEMY_SNUM; i++) {
		shot[i] = tempEShot;
	}

}