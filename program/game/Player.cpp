#include "Player.h"

Player::Player() {
	// --- �R���X�g���N�^ ---
	// �v���C���[
	x = 500;
	y = 650;
	life = true;
	machineType = MACHINE_RED;	// �����}�V���FRed
	pre_machineType = MACHINE_RED;
	rad = -tnl::PI/2;
	hp = PLAYER_INITIAL_HP;
	lifeNum = PLAYER_INITIAL_LIFENUM;
	deadPosX = 0;
	deadPosY = 0;
	aimX = 0;
	aimY = 0;
	aimGh = 0;
	additionalPower = 0;
	additionalSpeed = 0;
	additionalDefense = 0;
	pre_addPower = 0;
	pre_addSpeed = 0;
	pre_addDefense = 0;
	bombNum = PLAYER_INITIAL_BOMBNUM;
	bomb = {
		// �O���t�B�b�N�n���h���Fdraw()�ŏ�����
		.x = 0,
		.y = 0,
		.targetX = 0,
		.targetY = 0,
		.velMagunitude = BOMB_VEL_MAGNITUDE,
		.velX = 0,
		.velY = 0,
		.bomb_animTime = 0,
		.bomb_animFlame = 0,
		.bomb_explodeAnimTime = 0,
		.bomb_explodeFlame = 0,
		.bombFlag = false,
		.explodeFlag = false,
		.bomb_damage = BOMB_DAMAGE,
		.colision = BOMB_EXPLODE_COLLISION
	};
	fs_bombShot = false;
	fs_bombExplode = false;
	tr_animFlame = 0;
	tr_animTime = 0;
	tr_MaxTime = 0.3;			// �ό`���ԏ��
	tr_countTime = 0;
	tr_animFlag = false;	
	tr_seFlag = false;
	// ��ʏ����p�����[�^ 
	drawInit = false;

	// -- �@�̂̃p�����[�^�i�[ -- 
	// Red�@��
	machine[0].gh = 0;			// Draw()�Ō�ɏ����� (DxLib_Init()����{�̂���)
	machine[0].width = 0;		// Draw()�Ō�ɏ�����
	machine[0].height = 0;		// Draw()�Ō�ɏ�����
	machine[0].element = RED;
	machine[0].hp = 0;			// �v���C���[�̓}�V���\���̂�hp���Q�Ƃ��Ȃ�
	machine[0].defence = RED_INITIAL_DEFENSE;	
	machine[0].mass = 1.0;
	machine[0].refAccMaxNormal = RED_REF_ACC_MAX_N;
	machine[0].refAccMaxShifted = RED_REF_ACC_MAX_S;
	machine[0].refVelMaxNormal = RED_REF_VEL_MAX_N;
	machine[0].refVelMaxShifted = RED_REF_VEL_MAX_S;
	machine[0].kp = 15.0;
	machine[0].ki = 0.005;
	machine[0].kd = 0.2;
	machine[0].reloadTime = RED_RELOAD_TIME;
	machine[0].bulletSpeed = RED_BULLET_SPEED;
	machine[0].score = 0;		// (�G�@�p Player�ł͎g�p���Ȃ�)
	// Blue�@��
	machine[1].gh = 0;			// Draw()�Ō�ɏ�����
	machine[1].width = 0;		// Draw()�Ō�ɏ�����
	machine[1].height = 0;		// Draw()�Ō�ɏ�����
	machine[1].element = BLUE;
	machine[1].hp = 0;			// �v���C���[�̓}�V���\���̂�hp���Q�Ƃ��Ȃ�
	machine[1].defence = BLUE_INITIAL_DEFENSE;
	machine[1].mass = 1.8;
	machine[1].refAccMaxNormal = BLUE_REF_ACC_MAX_N;
	machine[1].refAccMaxShifted = BLUE_REF_ACC_MAX_S;
	machine[1].refVelMaxNormal = BLUE_REF_VEL_MAX_N;
	machine[1].refVelMaxShifted = BLUE_REF_VEL_MAX_S;
	machine[1].kp = 15.0;
	machine[1].ki = 0.005;
	machine[1].kd = 0.2;
	machine[1].reloadTime = BLUE_RELOAD_TIME;
	machine[1].bulletSpeed = BLUE_BULLET_SPEED;
	machine[1].score = 0;		// (�G�@�p Player�ł͎g�p���Ȃ�)
	// Yellow�@��
	machine[2].gh = 0;			// Draw()�Ō�ɏ�����
	machine[2].width = 0;		// Draw()�Ō�ɏ�����
	machine[2].height = 0;		// Draw()�Ō�ɏ�����
	machine[2].element = YELLOW;
	machine[2].mass = 0.3;
	machine[2].hp = 0;			// �v���C���[�̓}�V���\���̂�hp���Q�Ƃ��Ȃ�
	machine[2].defence = YELLOW_INITIAL_DEFENSE;
	machine[2].refAccMaxNormal = YELLOW_REF_ACC_MAX_N;
	machine[2].refAccMaxShifted = YELLOW_REF_ACC_MAX_S;
	machine[2].refVelMaxNormal = YELLOW_REF_VEL_MAX_N;
	machine[2].refVelMaxShifted = YELLOW_REF_VEL_MAX_S;
	machine[2].kp = 15.0;
	machine[2].ki = 0.005;
	machine[2].kd = 0.2;
	machine[2].reloadTime = YELLOW_RELOAD_TIME;
	machine[2].bulletSpeed = YELLOW_BULLET_SPEED;
	machine[2].score = 0;		// (�G�@�p Player�ł͎g�p���Ȃ�)
	// ���x����p�����[�^
	velX = 0;
	velY = 0;
	accX = 0;
	accY = 0;
	refVelY = 0;
	refVelX = 0;
	velMax = 0;
	accMax = 0;
	integralX = 0;
	integralY = 0;
	preErrorX = 0;
	preErrorY = 0;

	// -- �����n�̃p�����[�^ --
	// - �e�̃p�����[�^������ -
	for (int i = 0; i < MACHINE_NUM; i++){
		for (int k = 0; k < PSHOT_NUM; k++) {

			shot[i][k].gh[SHOT_FRAME_MAX] = { 0 };			// Draw()�Ō�ɏ�����
			shot[i][k].flag = false;
			shot[i][k].x = 0;
			shot[i][k].y = 0;
			shot[i][k].width = 0;		// Draw()�Ō�ɏ�����
			shot[i][k].heigh = 0;		// Draw()�Ō�ɏ�����
			shot[i][k].rad = 0;
			shot[i][k].speed = 0;
			shot[i][k].time = 0;
			shot[i][k].animFrame = 0;

			// �e�̎�ނɉ����ĕ���
			if (i == RED) {
				shot[i][k].element = RED;
				shot[i][k].damage = RED_INITIAL_BULLET_ATK;
			}
			else if (i == BLUE) {
				shot[i][k].element = BLUE;
				shot[i][k].damage = BLUE_INITIAL_BULLET_ATK;
			} 
			else if (i == YELLOW) {
				shot[i][k].element = YELLOW;
				shot[i][k].damage = YELLOW_INITIAL_BULLET_ATK;
			}

		}
	}
	shotCount = 0;
	// ���� 
	totalTime = 0;
	reloadingTime = 0;
	damageInvalidTime = 0;
	damageAnimCounter = 0;
	respawnTime = 0;
	// �t���O
	damageFlag = false;
	damageInvalidFlag = false;
	deadFlag = false;
	respawnFlag = false;
	redShotFlag = false;		// ���ʉ��Đ��t���O
	blueShotFlag = false;
	yellowShotFlag = false;
	deadEffectFlag = false;
	deadEffectInitFlag = false;
	drawBomb = false;
	fs_damage = false;
	fs_refrect = false;
	hs_shifted = false;
	penetrationFlag = false;
	// �_���[�W�v�Z�p -
	damagedElement = 0;		// ��e���̑���
	damagedNum = 0;			// ��e�_���[�W
	// ���ŃG�t�F�N�g�N���X
	//  -> All�֐����ŃC���X�^���X������. �b��I��nullpointer��^����
	deadeffect = nullptr;

}

Player::~Player() {
	// --- �f�X�g���N�^ ---
	if (deadeffect != NULL) {
		delete deadeffect;
	}
}

void Player::All(float delta_time) {
	// --- �v���C���[�̑�����O������Ăяo�� ---
	// -- nan �G���[���: �|�[�Y��ʎ� --
	if (!delta_time == 0) {
		// �Ə��̈ʒu�̎擾
		GetMousePoint(&aimX, &aimY);
		// - ���x���� - 
		Move(delta_time);
		// - �ˌ� -
		Shot(delta_time);
	}
	// - ���S���̃t���O
	if (deadFlag) {
		
		// ���S�G�t�F�N�g�t���O�L����
		deadEffectInitFlag = true;
		deadEffectFlag = true;

		deadFlag = false;

	} 
	
	// - ���S�G�t�F�N�g���s
	if (deadEffectFlag) {
		if (deadEffectInitFlag) {
			deadeffect = new DeadEffect(1, deadPosX, deadPosY, velX, velY, 1.0, 10.0, 2.5);
			deadEffectInitFlag = false;
		}
		if (deadeffect->All(delta_time)) {
			deadEffectFlag = false;
		}
	}
	// - �_���[�W�v�Z - 
	DamageCalc();
	// - �A�C�e���ɂ��X�e�[�^�X�ω�������΁A�@�̂ɔ��f -
	if (GetStateChange()) {
		StateUp();
	}

	// - �`�� -
	Draw();
	// - ���ԁ��t���O�Ǘ� -
	TimeAndFlag(delta_time);

}


void Player::Move(float delta_time) {
	// --- �v���C���[�̓��� ---
	// �t���O���Z�b�g
	tr_seFlag = false;

	// *** �����Ă鎞�̏��� ****
	if (life) {
		// -- �}�V���̑I��(1, 2, 3 key) -- 
		if (tnl::Input::IsKeyDown(eKeys::KB_1)) {
			machineType = MACHINE_RED;
			tr_seFlag = true;
		}
		else if (tnl::Input::IsKeyDown(eKeys::KB_2)) {
			machineType = MACHINE_BLUE;
			tr_seFlag = true;
		}
		else if (tnl::Input::IsKeyDown(eKeys::KB_3)) {
			machineType = MACHINE_YELLOW;
			tr_seFlag = true;
		}
		// -- �}�V���̑I��(e key)
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_E)) {
			machineType++;
			if (machineType > MACHINE_YELLOW) {
				machineType = MACHINE_RED;
			}
			tr_seFlag = true;
		}

		// -- �}�V���̑I��(c key)
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_C)) {
			machineType--;
			if (machineType < MACHINE_RED) {
				machineType = MACHINE_YELLOW;
			}
			tr_seFlag = true;
		}

		// -- �ړ����� --
		// ��������
		if (x > (WIDTH - MENU_WIDTH)) {
			x = WIDTH - MENU_WIDTH;
		}
		else if (x < MARGINE) {
			x = MARGINE;
		}
		// ��������
		if (y > HEIGHT - MARGINE) {
			y = HEIGHT - MARGINE;
		}
		else if (y < MARGINE) {
			y = MARGINE;
		}

		// -- �v���C���[�̑��x���� --
		hs_shifted = false;		// �V�t�g�ō�����s�����ǂ����̃t���O
		// - Shift���͎��͑��x�������x������A�b�v������
		if (tnl::Input::IsKeyDown(eKeys::KB_LSHIFT)) {
			hs_shifted = true;		// �V�t�g�ō�����s�����ǂ����̃t���O
			velMax = machine[machineType].refVelMaxShifted;
			accMax = machine[machineType].refAccMaxShifted;
		}
		else {
			velMax = machine[machineType].refVelMaxNormal;
			accMax = machine[machineType].refAccMaxNormal;
		}

		// - ���x�ڕW�l��WASD or �����L�[���瓾�� - 
		// - �������� -
		if ((tnl::Input::IsKeyDown(eKeys::KB_W) && tnl::Input::IsKeyDown(eKeys::KB_S)) ||
			(tnl::Input::IsKeyDown(eKeys::KB_UP) && tnl::Input::IsKeyDown(eKeys::KB_DOWN))) {
			// ����������h�~
			refVelY = 0;

		}
		else if (tnl::Input::IsKeyDown(eKeys::KB_W) || tnl::Input::IsKeyDown(eKeys::KB_UP)) {
			refVelY = -1 * velMax;
		}
		else if (tnl::Input::IsKeyDown(eKeys::KB_S) || tnl::Input::IsKeyDown(eKeys::KB_DOWN)) {
			refVelY = velMax;
		}
		else {
			// �����쎞�̏���
			refVelY = 0;
		}

		// - �������� -
		if ((tnl::Input::IsKeyDown(eKeys::KB_D) && tnl::Input::IsKeyDown(eKeys::KB_A)) ||
			(tnl::Input::IsKeyDown(eKeys::KB_RIGHT) && tnl::Input::IsKeyDown(eKeys::KB_LEFT))) {
			// ����������h�~
			refVelX = 0;
		}
		else if (tnl::Input::IsKeyDown(eKeys::KB_D) || tnl::Input::IsKeyDown(eKeys::KB_RIGHT)) {
			refVelX = velMax;
		}
		else if (tnl::Input::IsKeyDown(eKeys::KB_A) || tnl::Input::IsKeyDown(eKeys::KB_LEFT)) {
			refVelX = -1 * velMax;
		}
		else {
			// �����쎞�̏���
			refVelX = 0;
		}


		if (delta_time == 0) {

		}
		// - ���x�Ǐ]������(�����)�FUx(F(N)) - 
		float errorX = (refVelX - velX);	// ���x�덷�v�Z
		float errorY = (refVelY - velY);
		// ������͐���
		integralX += (errorX + preErrorX) / 2 * delta_time;
		float ux = machine[machineType].kp * errorX +	// ������͌v�ZX
			machine[machineType].ki * integralX +
			machine[machineType].kd * (errorX - preErrorX) / delta_time;
		integralY += (errorY + preErrorY) / 2 * delta_time;
		float uy = machine[machineType].kp * errorY +	// ������͌v�ZY
			machine[machineType].ki * integralY +
			machine[machineType].kd * (errorY - preErrorY) / delta_time;

		// - ����Ώۂ֎w�ߒl�𔽉f - 
		// �����x
		accX += (ux / machine[machineType].mass) * delta_time;	// A = F/M����p
		accY += (uy / machine[machineType].mass) * delta_time;
		// �����x�̑傫���𐧌�
		float accNorm = hypot(accX, accY);
		if (accNorm >= accMax) {
			accX = (accX / accNorm) * accMax;
			accY = (accY / accNorm) * accMax;
		}

		// ���x
		velX += accX;
		velY += accY;
		// ���x�̑傫���𐧌�
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

		// -- �u�[�X�g�`���[�W�̎��� --
		

		// -- �v���C���[�̌������� --
		// �}�E�X�̌����ɉ����ăv���C���[�̊p�x���X�V����.
		int mouseX, mouseY;
		GetMousePoint(&mouseX, &mouseY);
		rad = atan2((double)mouseY - y, (double)mouseX - x);

		// -- �ό`���[�V�����̍쐬 --
		// - �I�𒆂̃}�V���ƈȑO�I�����Ă����}�V���̔�r���t���O�L����
		if (machineType != pre_machineType) {
			pre_machineType = machineType;
			tr_animFlag = true;		//	�t���O��������TimeAndFlag()�Ŏ��{
		}


	} else {


	// *** ���S���Ă��鎞�̏��� ****
	// -- ���X�|�[���ʒu�Ɉړ�����
	accX = 0;
	accY = 0;
	velX = 0;
	velY = 0;
	x = (WIDTH - MENU_WIDTH) / 2;
	y = HEIGHT + MARGINE + 10;
	rad = 0;
	// -- �c�@������΁A���X�|�[���t���O�𗧂Ă�
	if (lifeNum > 0) {
		respawnFlag = true;
	}
	// ���X�|�[���t���O�̊m�F
	else if (respawnFlag) {
		respawnFlag = false;
	}
	// - ���X�|�[���ڕW�ʒu�܂ňړ�
	double length = (HEIGHT + MARGINE + 10) - PLAYER_RESPAWN_POS_Y;
	double temp = HEIGHT - length * sin((tnl::PI / 2) * (respawnTime / PLAYER_RESPAWN_TIME));
	x = (WIDTH - MENU_WIDTH) / 2;
	y = temp;

	}
	
}

void Player::Draw() {
	// --- �`�揈�� --- 
	
	// -- ���������� -- 
	if (!drawInit) {

		// - �@�̂̉摜������ - 
		machine[0].gh = LoadGraph("graphics/RedPlaneFront00.png");		// Red�@��
		machine[1].gh = LoadGraph("graphics/BluePlaneFront00.png");		// Blue�@��
		machine[2].gh = LoadGraph("graphics/YellowPlaneFront00.png");	// Yellow�@��
		for (int i = 0; i < MACHINE_NUM; i++) {
			// �摜�T�C�Y���\���̂Ɋi�[
			GetGraphSize(machine[i].gh, &machine[i].width, &machine[i].height);
		}
		// - �Ə��̏���
		aimGh = LoadGraph("graphics/Aim.png");
		// - �{���̏��� - 
		// bomb�A�C�e���摜�𗬗p
		LoadDivGraph("graphics/Item/BombItem_3_861_284.png", 3, 3, 1, 861 / 3, 284, bomb.gh);
		LoadDivGraph("graphics/Effect/DeadEffect001_10_560_48.png", 10, 10, 1, 568 / 10, 48, bomb.effectGh);

		// - �ό`���[�V�����摜�ǂݍ���
		LoadDivGraph("graphics/QuantumCore_12_120_10.png", 12, 12, 1, 120 / 12, 10, transformGh, true, false);
		// - �e�̏��� - 
		int tempRed[RED_BULLET_FRAME_MAX];
		int tempBlue[BLUE_BULLET_FRAME_MAX];
		int tempYellow[YELLOW_BULLET_FRAME_MAX];

		LoadDivGraph("graphics/Red/RedBullet002_3_48_16.png", 3, 3, 1, 48 / 3, 16, tempRed);
		LoadDivGraph("graphics/Blue/BlueBullet002_6_60_21.png", 6, 6, 1, 60 / 6, 21, tempBlue);
		LoadDivGraph("graphics/Yellow/YellowBullet001_4_64_26.png", 4, 4, 1, 64 / 4, 26, tempYellow);

		int w, h;
		for (int i = 0; i < MACHINE_NUM; i++) {
			// �@�̂̎�ނɉ����ď�������
			switch (i)
			{
			case RED:
				// �摜�̃T�C�Y���擾
				GetGraphSize(tempRed[0], &w, &h);
				// �O���t�B�b�N�n���h�����摜�T�C�Y��e�\���̂֓n��
				for (int k = 0; k < PSHOT_NUM; k++) {
					for (int n = 0; n < RED_BULLET_FRAME_MAX; n++) {
						shot[i][k].gh[n] = tempRed[n];
					}
					shot[i][k].width = w;
					shot[i][k].heigh = h;
				}
				break;

			case BLUE:
				// �摜�̃T�C�Y���擾
				GetGraphSize(tempBlue[0], &w, &h);
				// �O���t�B�b�N�n���h�����摜�T�C�Y��e�\���̂֓n��
				for (int k = 0; k < PSHOT_NUM; k++) {
					for (int n = 0; n < BLUE_BULLET_FRAME_MAX; n++) {
						shot[i][k].gh[n] = tempBlue[n];
					}
					shot[i][k].width = w;
					shot[i][k].heigh = h;
				}
				break;

			case YELLOW:
				// �摜�̃T�C�Y���擾
				GetGraphSize(tempYellow[0], &w, &h);
				// �O���t�B�b�N�n���h�����摜�T�C�Y��e�\���̂֓n��
				for (int k = 0; k < PSHOT_NUM; k++) {
					for (int n = 0; n < YELLOW_BULLET_FRAME_MAX; n++) {
						shot[i][k].gh[n] = tempYellow[n];
					}
					shot[i][k].width = w;
					shot[i][k].heigh = h;
				}
				break;

			default:
				OutputDebugString("�G���[�F���@�e�̉摜�����������A�}�V�������m�F���ĉ�����");
				break;
			}
		}

		drawInit = true;
	}

	// -- �`�揈�� -- 

	// - �e�ۂ̕`�� - 
	for (int i = 0; i < MACHINE_NUM; i++) {
		for (int k = 0; k < PSHOT_NUM; k++) {
			if (shot[i][k].flag) {
				switch (i)
				{
				case RED:
					// �e�A�j���[�V�����Đ�
					if (shot[i][k].time >= RED_BULLET_ANIM_SPAN) {
						shot[i][k].time = 0;
						shot[i][k].animFrame++;
						shot[i][k].animFrame %= RED_BULLET_FRAME_MAX;
					}
					DrawRotaGraph(shot[i][k].x,	shot[i][k].y, 1.3, shot[i][k].rad + tnl::PI / 2, 
									shot[i][k].gh[shot[i][k].animFrame], true, false);
					break;

				case BLUE:
					// �e�A�j���[�V�����Đ�
					if (shot[i][k].time >= BLUE_BULLET_ANIM_SPAN) {
						shot[i][k].time = 0;
						shot[i][k].animFrame++;
						shot[i][k].animFrame %= BLUE_BULLET_FRAME_MAX;
					}
					DrawRotaGraph(shot[i][k].x, shot[i][k].y, 1.8, shot[i][k].rad + tnl::PI / 2,
						shot[i][k].gh[shot[i][k].animFrame], true, false);

					break;

				case YELLOW:
					// �e�A�j���[�V�����Đ�
					if (shot[i][k].time >= YELLOW_BULLET_ANIM_SPAN) {
						shot[i][k].time = 0;
						shot[i][k].animFrame++;
						shot[i][k].animFrame %= YELLOW_BULLET_FRAME_MAX;
					}
					DrawRotaGraph(shot[i][k].x, shot[i][k].y, 1.3, shot[i][k].rad + tnl::PI / 2,
						shot[i][k].gh[shot[i][k].animFrame], true, false);

					break;

				default:
					OutputDebugString("�G���[�F���@�e�̃A�j���[�V��������");
					break;
				}

			}
		}
	}

	// - �{���̕`�� -
	if (bomb.bombFlag) {
		// �{���ړ����̏���
		if (bomb.bomb_animTime >= 0.2) {
			bomb.bomb_animTime = 0;
			bomb.bomb_animFlame++;
			bomb.bomb_animFlame %= 3;
		}
		DrawRotaGraph(bomb.x, bomb.y, 0.1, bomb.rad + tnl::PI / 2, bomb.gh[bomb.bomb_animFlame], true, false);
	}
	else if (bomb.explodeFlag) {
		drawBomb = true;
	}
	if(drawBomb){
		// �{�����j��̏���
		if (bomb.bomb_explodeAnimTime >= 0.1) {
			bomb.bomb_explodeAnimTime = 0;
			bomb.bomb_explodeFlame++;
			if (bomb.bomb_explodeFlame >= 9) {
				// �{�����j�A�j���[�V�����̓��[�v���Ȃ�
				bomb.bomb_explodeFlame = 9;
				drawBomb = false;
			}

			DrawRotaGraph(bomb.x, bomb.y, 7.0, bomb.rad + tnl::PI / 2, bomb.effectGh[bomb.bomb_explodeFlame], true, false);
		}

	}


	// - �v���C���[�̕`�� -
	if (life) {
		// * �v���C���[�������Ă���Ƃ� * 
		
		// �ό`�A�j���摜�n���h���ύX
		if (tr_animTime >= 0.05) {
			tr_animTime = 0;
			tr_animFlame++;
			tr_animFlame %= 12;
		}

		if (!damageInvalidFlag) {
			// ���G���ԂŖ������̃v���C���[�`��
			if (!tr_animFlag) {
				DrawRotaGraph(x, y, 1.0, rad + tnl::PI / 2, machine[machineType].gh, true, false);
			}
			else {
				// �ό`�A�j���[�V�����Đ�
				DrawRotaGraph(x, y, 4.0, rad + tnl::PI / 2, transformGh[tr_animFlame], true, false);
			}
		}
		else {
			// ���G���Ԓ��̃v���C���[�̕`��
			// ���G���Ԃ̌o�ߎ��Ԋ������󂯎��
			int temp = (int)(100 * ((float)DAMAGE_INVALID_TIME_MAX - damageInvalidTime) / (float)DAMAGE_INVALID_TIME_MAX);
			if (temp % 10 < 5){
				if (!tr_animFlag) {
					// �v���C���[�𔖂��\�� 
					SetDrawBlendMode(DX_BLENDMODE_ALPHA, 140);
					DrawRotaGraph(x, y, 1.0, rad + tnl::PI / 2, machine[machineType].gh, true, false);
					SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
				}
				else {
					// �ό`��Ԃ̃v���C���[�𔖂��\��
					SetDrawBlendMode(DX_BLENDMODE_ALPHA, 140);
					DrawRotaGraph(x, y, 4.0, rad + tnl::PI / 2, transformGh[tr_animFlame], true, false);
					SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
				}
			}
			else {
				if (!tr_animFlag) {
					// �v���C���[���X�ɔ����\��
					SetDrawBlendMode(DX_BLENDMODE_ALPHA, 80);
					DrawRotaGraph(x, y, 1.0, rad + tnl::PI / 2, machine[machineType].gh, true, false);
					SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
				}
				else {
					// �ό`��Ԃ̃v���C���[���X�ɔ����\��
					SetDrawBlendMode(DX_BLENDMODE_ALPHA, 80);
					DrawRotaGraph(x, y, 4.0, rad + tnl::PI / 2, transformGh[tr_animFlame], true, false);
					SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

				}
			}		
		}
		// �Ə��̕`��
		DrawRotaGraph(aimX, aimY, 0.8, rad + tnl::PI / 2, aimGh, true, true);

	} else if (respawnFlag) {
		// * ���X�|�[���� *
		if ((int)(totalTime) % 2 == 0) {
			// �����b�̎�
			// �v���C���[�𔖂��\�� 
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 140);
			DrawRotaGraph(x, y, 1.0, rad, machine[machineType].gh, true, false);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
		else {
			// ��b�̎�
			// �v���C���[���X�ɔ����\��
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 80);
			DrawRotaGraph(x, y, 1.0, rad, machine[machineType].gh, true, false);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}

	}

}

void Player::Shot(float delta_time) {
	// --- �e�ۂ̔��ˁA�O���̌v�Z ---

	// �ꎞ�ϐ�
	int num = 0;
	// -- �e�ۂ̔���(�}�V�����ɈႤ�e���g�p) -- 
	// - �����Ă��鎞�����ˌ� -
	if (life) {
		// - ��_���[�W��(�܂薳�G���Ԓ�)�͔��˂��Ȃ�
		if (!damageInvalidFlag) {
			// ** �ʏ�e�̏��� * //
			if (tnl::Input::IsMouseDown(eMouse::LEFT) &&
				reloadingTime >= machine[machineType].reloadTime) {

				if (machineType == RED) {
					// * �����X�e�[�^�X�ɉ����Ĕ��˂���e�̕ύX�������� * //
					for (int i = 0; i < PSHOT_NUM; i++) {
						if (shot[machineType][i].flag == false) {

							// 1. additonalPower < 4�̎��̃V���b�g
							if (additionalPower < 3) {
								// �ђʒe����
								penetrationFlag = false;
								shot[machineType][i].flag = true;
								shot[machineType][i].x = x;
								shot[machineType][i].y = y;
								shot[machineType][i].rad = rad;
								shot[machineType][i].speed = machine[machineType].bulletSpeed;
								shot[machineType][i].time = 0;

								break;
							}
							// 2. additionalPower >= 3�̎��̃V���b�g
							else if (additionalPower >= 3 && additionalPower < 6) {
								// �ђʒe����
								penetrationFlag = false;
								if (num == 0) {
									shot[machineType][i].flag = true;
									shot[machineType][i].x = x;
									shot[machineType][i].y = y;
									shot[machineType][i].rad = rad - tnl::PI * 2 / 360 * 2;
									shot[machineType][i].speed = machine[machineType].bulletSpeed;
									shot[machineType][i].time = 0;
								}
								else if (num == 1) {
									shot[machineType][i].flag = true;
									shot[machineType][i].x = x;
									shot[machineType][i].y = y;
									shot[machineType][i].rad = rad + tnl::PI * 2 / 360 * 2;
									shot[machineType][i].speed = machine[machineType].bulletSpeed;
									shot[machineType][i].time = 0;
								}
								num++;
								if (num >= 2) {
									break;
								}

							}
							// 3. additonalPower >= 6�̎��̃V���b�g
							else if (additionalPower >= 6) {
								// �ђʒe����
								penetrationFlag = false;
								if (num == 0) {
									shot[machineType][i].flag = true;
									shot[machineType][i].x = x;
									shot[machineType][i].y = y;
									shot[machineType][i].rad = rad;
									shot[machineType][i].speed = machine[machineType].bulletSpeed;
									shot[machineType][i].time = 0;
								}
								else if (num == 1) {
									shot[machineType][i].flag = true;
									shot[machineType][i].x = x;
									shot[machineType][i].y = y;
									shot[machineType][i].rad = rad + tnl::PI * 2 / 360 * 10;
									shot[machineType][i].speed = machine[machineType].bulletSpeed;
									shot[machineType][i].time = 0;
								}
								else if (num == 2) {
									shot[machineType][i].flag = true;
									shot[machineType][i].x = x;
									shot[machineType][i].y = y;
									shot[machineType][i].rad = rad - tnl::PI * 2 / 360 * 10;
									shot[machineType][i].speed = machine[machineType].bulletSpeed;
									shot[machineType][i].time = 0;
								}

								num++;
								if (num >= 3) {
									break;
								}
							}
						}
					}
				}

				if (machineType == BLUE) {
					for (int i = 0; i < PSHOT_NUM; i++) {
						if (shot[machineType][i].flag == false) {
							// 1. additonalPower < 4�̎��̃V���b�g
							if (additionalPower < 3) {
								// �ђʒe����
								penetrationFlag = false;
								shot[machineType][i].flag = true;
								shot[machineType][i].x = x;
								shot[machineType][i].y = y;
								shot[machineType][i].rad = rad;
								shot[machineType][i].speed = machine[machineType].bulletSpeed;
								shot[machineType][i].time = 0;

								break;
							}
							// 2. additonalPower >= 3�̎��̃V���b�g
							if (additionalPower >= 3 && additionalPower < 6) {
								// �ђʒe�L����
								penetrationFlag = true;
								shot[machineType][i].flag = true;
								shot[machineType][i].x = x;
								shot[machineType][i].y = y;
								shot[machineType][i].rad = rad;
								shot[machineType][i].speed = machine[machineType].bulletSpeed;
								shot[machineType][i].time = 0;

								break;
							}
							// 3. additonalPower >= 6�̎��̃V���b�g
							if (additionalPower >= 6) {
								// �ђʒe�L����
								penetrationFlag = true;
								shot[machineType][i].flag = true;
								shot[machineType][i].x = x;
								shot[machineType][i].y = y;
								shot[machineType][i].rad = rad;
								shot[machineType][i].speed = machine[machineType].bulletSpeed;
								shot[machineType][i].time = 0;

								break;
							}

						}
					}
				}

				if (machineType == YELLOW) {
					for (int i = 0; i < PSHOT_NUM; i++) {
						if (shot[machineType][i].flag == false) {

							// 1. additonalPower < 4�̎��̃V���b�g
							if (additionalPower < 3) {
								// �ђʒe����
								penetrationFlag = false;
								shot[machineType][i].flag = true;
								shot[machineType][i].x = x;
								shot[machineType][i].y = y;
								shot[machineType][i].rad = rad;
								shot[machineType][i].speed = machine[machineType].bulletSpeed;
								shot[machineType][i].time = 0;

								break;
							}

							// 2. additonalPower >= 3�̎��̃V���b�g
							// 30�����̊p�x��4���̎U�e���ˁA�ꔭ�͐��ʂɔ��˂���
							if (additionalPower >= 3 && additionalPower < 6) {
								// �ђʒe����
								penetrationFlag = false;
								// �����擾
								if (num == 0) {
									srand((unsigned int)time(0));
									// �ꔭ�͐��ʂɔ���
									shot[machineType][i].flag = true;
									shot[machineType][i].x = x;
									shot[machineType][i].y = y;
									shot[machineType][i].rad = rad;
									shot[machineType][i].speed = machine[machineType].bulletSpeed;
									shot[machineType][i].time = 0;
								}
								if (num < 4) {
									shot[machineType][i].flag = true;
									shot[machineType][i].x = x;
									shot[machineType][i].y = y;
									shot[machineType][i].rad = rad  - (15 * tnl::PI / 180) + ((rand() % 30) * tnl::PI / 180);
									shot[machineType][i].speed = machine[machineType].bulletSpeed;
									shot[machineType][i].time = 0;
								}
								num++;

								if (num >= 4) {
									break;
								}
							}

							// 3. additonalPower >= 6�̎��̃V���b�g
							if (additionalPower >= 6) {
								// �ђʒe����
								penetrationFlag = false;
								// �����擾
								if (num == 0) {
									srand((unsigned int)time(0));
									// �ꔭ�͐��ʂɔ���
									shot[machineType][i].flag = true;
									shot[machineType][i].x = x;
									shot[machineType][i].y = y;
									shot[machineType][i].rad = rad;
									shot[machineType][i].speed = machine[machineType].bulletSpeed;
									shot[machineType][i].time = 0;
								}
								if (num < 8) {
									shot[machineType][i].flag = true;
									shot[machineType][i].x = x;
									shot[machineType][i].y = y;
									shot[machineType][i].rad = rad - (15 * tnl::PI / 180) + ((rand() % 30) * tnl::PI / 180);
									shot[machineType][i].speed = machine[machineType].bulletSpeed;
									shot[machineType][i].time = 0;
								}
								num++;

								if (num >= 8) {
									break;
								}
							}
						}
					}
				}
						
				// - �����[�h�^�C���̃��Z�b�g -
				reloadingTime = 0;

				// - �V���b�g���ʉ��Đ��t���O�𗧂Ă� -
				switch (machineType)
				{
				case RED:
					redShotFlag = true;
					break;
				case BLUE:
					blueShotFlag = true;
					break;
				case YELLOW:
					yellowShotFlag = true;
					break;
				}

			}
			// * �{���̏��� * //
			// �{�����j�t���O��܂�
			bomb.explodeFlag = false;
			// �{���V���b�g���t���O��܂�
			fs_bombShot = false;
			// �{���͉�ʂɈ�����\��������
			if (tnl::Input::IsMouseDown(eMouse::RIGHT) && bombNum > 0 && bomb.bombFlag == false && bomb.explodeFlag == false && drawBomb == false) {
				// �{���V���b�g���t���O�𗧂Ă�
				fs_bombShot = true;
				// ����������
				bomb.bomb_animTime = 0;
				bomb.bomb_animFlame = 0;
				bomb.bomb_explodeAnimTime = 0;
				bomb.bomb_explodeFlame = 0;
				
				bombNum--;
				if (bombNum < 0) {
					bombNum = 0;
				}
				// �W�I�܂ł̊p�x���擾
				bomb.rad = rad;
				// �v���C���[�̈ʒu���i�[
				bomb.x = x;
				bomb.y = y;
				// �}�E�X�J�[�\���ʒu��ڕW�ʒu�Ƃ��Ċi�[
				int mouseX, mouseY;
				GetMousePoint(&mouseX, &mouseY);
				bomb.targetX = mouseX;
				bomb.targetY = mouseY;
				// �v���C���[�̌����ɉ����ă{�����������v�Z
				bomb.velX = bomb.velMagunitude * delta_time * cos(bomb.rad);
				bomb.velY = bomb.velMagunitude * delta_time * sin(bomb.rad);
				// ���˃t���O�L����
				bomb.bombFlag = true;

			}
		}
	}

	// -- �t���O�̗������S�Ă̒e�ۂ̈ʒu�X�V -- 
	// - �}�V���S�@�̔��˒e�ۂ����� - 
	for (int i = 0; i < MACHINE_NUM; i++) {
		for (int k = 0; k < PSHOT_NUM; k++) {
			
			if(shot[i][k].flag) {

				shot[i][k].x += shot[i][k].speed * delta_time * cos(shot[i][k].rad);
				shot[i][k].y += shot[i][k].speed * delta_time * sin(shot[i][k].rad);
				shot[i][k].time += delta_time;
			}

			// ��ʊO�ɒe���͂ݏo������A�t���O��߂�
			if((shot[i][k].x > WIDTH - MENU_WIDTH) ||
				(shot[i][k].x < MARGINE) ||
				(shot[i][k].y > HEIGHT - MARGINE) ||
				(shot[i][k].y < MARGINE)) {

				shot[i][k].flag = false;
			}

		}
	}

	// - �{�����ˌ�̏������{ - 
	// �{�����j���t���O��܂�
	fs_bombExplode = false;
	if (bomb.bombFlag) {
		// �{�������j����܂ňړ�������
		bomb.x += bomb.velX;
		bomb.y += bomb.velY;
		bomb.bomb_animTime += delta_time;	// �A�j���[�V�����J�E���^�X�V

		// - ��ʊO�ɒe���͂ݏo�� or �ڕW�ʒu�ɓ��B�����ꍇ�A���j�t���O��L���� -
		if ((bomb.x > WIDTH - MENU_WIDTH) ||
			(bomb.x < MARGINE) ||
			(bomb.y > HEIGHT - MARGINE) ||
			(bomb.y < MARGINE)) {
			bomb.bombFlag = false;
			bomb.explodeFlag = true;
			// �{�����j���t���O�𗧂Ă�
			fs_bombExplode = true;

		}
		else if (((double)bomb.targetX > bomb.x - 2.0 && (double)bomb.targetX < bomb.x + 2.0) ||
			((double)bomb.targetY > bomb.y - 2.0 && (double)bomb.targetY < bomb.y + 2.0)) {
			bomb.bombFlag = false;
			bomb.explodeFlag = true;
			// �{�����j���t���O�𗧂Ă�
			fs_bombExplode = true;
		}

	}

	if (drawBomb) {
		// ������ړ���~�A�A�j���[�V�����J�E���^�͍X�V
		bomb.bomb_explodeAnimTime += delta_time;
	}
	
	
}

void Player::TimeAndFlag(float delta_time) {
	// --- ���ԁ��t���O�̊Ǘ� --- 
	totalTime += delta_time;		// �o�ߎ��Ԃ��X�V
	reloadingTime += delta_time;	// �e�̃����[�h���ԍX�V
	tr_animTime += delta_time;		// �ό`�A�j���[�V�������ԍX�V
	if (tr_animFlag) {
		// �ό`���L���̍ہA�ό`���ԍX�V
		tr_countTime += delta_time;
		if (tr_countTime >= tr_MaxTime) {
			tr_countTime = 0;
			tr_animFlag = false;
		}
	}

	// -- ���G���Ԃ̊Ǘ� --
	if (damageInvalidFlag) {
		// - ��莞�Ԗ��G -
		damageInvalidTime += delta_time;
		if (damageInvalidTime >= DAMAGE_INVALID_TIME_MAX) {
			// ���G�t���O�����ԃ��Z�b�g
			damageInvalidFlag = false;
			damageInvalidTime = 0;
		}
	}

	// -- ���X�|�[�����Ԃ̊Ǘ� --
	if (respawnFlag) {
		respawnTime += delta_time;
		if (respawnTime >= PLAYER_RESPAWN_TIME) {
			// - ���X�|�[�� -
			respawnTime = 0;
			respawnFlag = false;
			life = true;
			hp = PLAYER_INITIAL_HP;
			// - ���X�|�[����A�����h�~�̂��ߖ��G���ԃZ�b�g - 
			damageInvalidFlag = true;
		}
	}

}

void Player::DamageCalc() {
	// --- ��e�_���[�W���v�Z ---
	// �_���[�W�̎󂯎���SetDamage()�Ŏ��{
	// �_���[�W���ʉ��t���O��܂�
	fs_damage = false;
	fs_refrect = false;
	// �_���[�W���󂯂������ɉ����ă_���[�W�ω�
	int tempElement = machine[machineType].element;
	int calcType;

	if (life) {
		if (damageFlag) {
			// - ����񂯂�A���S���Y�����Q�l - 

			int temp = (tempElement - damagedElement + 3) % 3;
			if (temp == 0) {
				// ������
				calcType = 0;
			}
			else if (temp == 1) {
				// ����(���ʂ͂��܂���)
				calcType = 1;
			}
			else {
				// ����(��_��˂��ꂽ)
				calcType = 2;
			}

			// - �W�����P���̌��ʂɉ����ă_���[�W�v�Z - 
			switch (calcType)
			{
			case 0:
				// ������
				if (machine[machineType].defence - damagedNum <= 0) {
					hp += (int)machine[machineType].defence - damagedNum;
					damageInvalidFlag = true;
					fs_damage = true;
				} else {
					// �m�[�_���[�W
					fs_refrect = true;
				}
				break;

			case 1:
				// ����(�_���[�W1/2)
				if (machine[machineType].defence - damagedNum / 2 <= 0) {
					hp += (int)machine[machineType].defence - damagedNum / 2;
					damageInvalidFlag = true;
					fs_damage = true;
				} else {
					// �m�[�_���[�W
					fs_refrect = true;
				}
				break;
			case 2:
				// ����(�_���[�W2�{)
				if (machine[machineType].defence - damagedNum * 2 <= 0) {
					hp += (int)machine[machineType].defence - damagedNum * 2;
					damageInvalidFlag = true;
					fs_damage = true;
				} else {
					// �m�[�_���[�W
					fs_refrect = true;
				}
				break;
			}


			// - hp��0�ȉ��Ȃ�΁A���C�t�����炷 -
			if (hp <= 0) {
				hp = 0;
				life = false;
				deadFlag = true;
				deadPosX = x;
				deadPosY = y;
				lifeNum--;
				// �X�e�[�^�X�̃��Z�b�g
				additionalPower = 0;
				additionalDefense = 0;
				additionalSpeed = 0;
				if (lifeNum <= 0) {
					lifeNum = 0;
				}

			}

		}
	}
	// - �_���[�W�t���O��߂� - 
	damageFlag = false;

}

void Player::StateUp() {
	// --- �A�C�e���擾���ʂɂ�鉶�b���@�̂ɗ^���� ---
	
	// -- �@�� �� �e�̃p�����[�^�ύX -
	for (int i = 0; i < MACHINE_NUM; i++) {
		if (i == RED) {
			// �h���UP
			machine[i].defence = RED_INITIAL_DEFENSE + additionalDefense;
			// �X�s�[�h�A�b�v
			machine[i].refAccMaxNormal = RED_REF_ACC_MAX_N + (float)additionalSpeed / 10.0;
			machine[i].refAccMaxShifted = RED_REF_ACC_MAX_S + (float)additionalSpeed / 10.0;
			machine[i].refAccMaxNormal = RED_REF_VEL_MAX_N + (float)additionalSpeed / 10.0;
			machine[i].refAccMaxShifted = RED_REF_VEL_MAX_S + (float)additionalSpeed / 10.0;
			// �����[�h���xUP
			machine[i].reloadTime = RED_RELOAD_TIME - (float)additionalSpeed / 50;
			// �e��UP
			machine[i].bulletSpeed = RED_BULLET_SPEED + (float)additionalSpeed * 20;

		}
		else if (i == BLUE) {
			// �h���UP
			machine[i].defence = BLUE_INITIAL_DEFENSE + additionalDefense;
			// �X�s�[�h�A�b�v
			machine[i].refAccMaxNormal = BLUE_REF_ACC_MAX_N + (float)additionalSpeed / 5.0;
			machine[i].refAccMaxShifted = BLUE_REF_ACC_MAX_S + (float)additionalSpeed / 5.0;
			machine[i].refAccMaxNormal = BLUE_REF_VEL_MAX_N + (float)additionalSpeed / 5.0;
			machine[i].refAccMaxShifted = BLUE_REF_VEL_MAX_S + (float)additionalSpeed / 5.0;
			// �����[�h���xUP
			machine[i].reloadTime = BLUE_RELOAD_TIME - (float)additionalSpeed / 50;
			// �e��UP
			machine[i].bulletSpeed = BLUE_BULLET_SPEED + (float)additionalSpeed * 20;
		}
		else if (i == YELLOW) {
			// �h���UP
			machine[i].defence = YELLOW_INITIAL_DEFENSE + additionalDefense;
			// �X�s�[�h�A�b�v
			machine[i].refAccMaxNormal = YELLOW_REF_ACC_MAX_N + (float)additionalSpeed / 5.0;
			machine[i].refAccMaxShifted = YELLOW_REF_ACC_MAX_S + (float)additionalSpeed / 5.0;
			machine[i].refAccMaxNormal = YELLOW_REF_VEL_MAX_N + (float)additionalSpeed / 5.0;
			machine[i].refAccMaxShifted = YELLOW_REF_VEL_MAX_S + (float)additionalSpeed / 5.0;
			// �����[�h���xUP
			machine[i].reloadTime = YELLOW_RELOAD_TIME - (float)additionalSpeed / 50;
			// �e��UP
			machine[i].bulletSpeed = YELLOW_BULLET_SPEED + (float)additionalSpeed * 20;
		}


		for (int k = 0; k < PSHOT_NUM; k++) {
			if (i == RED) {
				// �U����UP
				shot[i][k].damage = RED_INITIAL_BULLET_ATK + additionalPower;
			}
			else if (i == BLUE) {
				// �U����UP
				shot[i][k].damage = BLUE_INITIAL_BULLET_ATK + additionalPower;
			}
			else if (i == YELLOW) {
				shot[i][k].damage = YELLOW_INITIAL_BULLET_ATK + additionalPower;
			}

		}
	}

	machine[0].refAccMaxNormal = 0.3;
	machine[0].refAccMaxShifted = 0.7;
	machine[0].refVelMaxNormal = 4.0;
	machine[0].refVelMaxShifted = 6.0;
	machine[0].kp = 15.0;
	machine[0].ki = 0.005;
	machine[0].kd = 0.2;
	machine[0].reloadTime = 0.08;
	machine[0].bulletSpeed = 800;


}

void Player::GetPosition(double* x, double* y) {
	// --- �v���C���[�̈ʒu��^���� ---
	*x = this->x;
	*y = this->y;
}

bool Player::GetShotPosition(int mechNum, int index, double* x, double* y) {
	// --- �v���C���[�̔��˂����e�̍��W���Q�ƃR�s�[ ---
	if (shot[mechNum][index].flag) {
		*x = shot[mechNum][index].x;
		*y = shot[mechNum][index].y;
		return true;
	}
	else {
		return false;
	}
}

int Player::GetShotElement(int mechNum, int index) {
	// --- �w�肵���e�̑����FElement��Ԃ�
	return shot[mechNum][index].element;
}

int Player::GetShotDamage(int mechNum, int index) {
	// --- �w�肵���e�̃_���[�W��Ԃ� ---
	return shot[mechNum][index].damage;
}

int Player::GetCollision() {
	// --- �@�̂̓����蔻�蔼�a�FCollision��Ԃ� ---
	// �摜�T�C�Y�̉����������̕��� / 2��Ԃ�
	int collision = (machine[machineType].width / 2 + this->machine[machineType].height / 2) / 2;
	return collision;
}

int Player::GetBulletCollision() {
	// -- ���@���˒e�̓����蔻�蔼�a�FCollision��Ԃ� ---
	// �摜�T�C�Y�̉����������̕��� / 2��Ԃ�
	int collision = (shot[machineType][0].width / 2 + shot[machineType][0].heigh / 2) / 2;
	return collision;
}

bool Player::GetDamageSoundFlag() {
	// --- ��e���̉��̃t���O��Ԃ� ---
	return fs_damage;
}

bool Player::GetRefrectFlag() {
	// --- �m�[�_���[�W���̃t���O��Ԃ� ---
	return fs_refrect;
}

bool Player::GetDamageFlag() {
	// --- ��e���ł��邩�̃t���O��Ԃ� ---
	return damageFlag;
}

int Player::GetShotFlag() {
	// --- �V���b�g���ʉ��̍Đ��t���O��Ԃ� ---
	// �t���O��������� -1 ��Ԃ�
	if (redShotFlag) {
		redShotFlag = false;
		return RED;
	}
	else if (blueShotFlag) {
		blueShotFlag = false;
		return BLUE;
	}
	else if(yellowShotFlag) {
		yellowShotFlag = false;
		return YELLOW;
	}
	else {
		return -1;
	}
}

bool Player::GetDamageInvalidFlag() {
	// --- �v���C���[�����G��Ԃł��邩�̃t���O��Ԃ� ---
	return damageInvalidFlag;
}

bool Player::GetLife() {
	// --- �v���C���[�̐����t���O��Ԃ� ---
	return life;
}

bool Player::GetDeadFlag() {
	// --- �v���C���[�̔j��t���O��Ԃ� ---
	return deadFlag;
}

int Player::GetLifeNum() {
	// --- �v���C���[�̎c�@��Ԃ� ---
	return lifeNum;
}

int Player::GetHpNum() {
	// --- �v���C���[��HP��Ԃ� ---
	return hp;
}

int Player::GetPowerNum() {
	// --- �v���C���[�̃{�[�i�X�U���͂�Ԃ� ---
	return additionalPower;
}

int Player::GetSpeedNum() {
	// --- �v���C���[�̃{�[�i�X���x��Ԃ� ---
	return additionalSpeed;
}

int Player::GetDefenseNum() {
	// --- �v���C���[�̃{�[�i�X�h��͂�Ԃ� ---
	return additionalDefense;
}

int Player::GetElement() {
	// --- �v���C���[�̑I�𒆂̑�����Ԃ� ---
	return machineType;
}

int Player::GetBombNum() {
	// --- �v���C���[�̏����{������Ԃ� ---
	return bombNum;
}

bool Player::GetBombFlag() {
	// --- �{�����˒����ǂ�����Ԃ� ---
	return bomb.bombFlag;
}

bool Player::GetBombExplodeFlag() {
	// --- �{�����������ǂ�����Ԃ� ---
	return bomb.explodeFlag;
}

void Player::GetBombPosition(double* x, double* y) {
	// --- �{���̈ʒu��Ԃ� ---
	*x = this->bomb.x;
	*y = this->bomb.y;
}

int Player::GetBombCollision() {
	// --- �{���̃R���W������Ԃ� ---
	return bomb.colision;
}

int Player::GetBombDamage() {
	// --- �{���̃_���[�W��Ԃ� ---
	return bomb.bomb_damage;
}

bool Player::GetBombShotSoundFlag() {
	// --- �{���V���b�g���t���O��Ԃ� ---
	return fs_bombShot;
}

bool Player::GetBombExplodeSoundFlag() {
	// --- �{�����j���t���O��Ԃ� ---
	return fs_bombExplode;
}

bool Player::GetStateChange() {
	// --- �v���C���[�ɃX�e�[�^�X�ω����������ꍇ�Ftrue��Ԃ� ---
	if (additionalPower != pre_addPower ||
		additionalSpeed != pre_addSpeed ||
		additionalDefense != pre_addDefense) {
		// 1�t���[���O�̏����C������
		pre_addPower = additionalPower;
		pre_addSpeed = additionalSpeed;
		pre_addDefense = additionalDefense;

		// -- �X�e�[�^�X�ω��ɉ����Ēe�̉摜�؂�ւ� -- 
		if (additionalPower < 3) {
			
			// -- �ԋ@�̂̏��� --
			// �������Ȃ�

			// -- �@�̂̏��� --
			// - �e�̉摜��؂�ւ� -
			int tempGh[BLUE_BULLET_FRAME_MAX];// �ꎞ�I�ȃO���t�B�b�N�n���h��
			int w, h;
			LoadDivGraph("graphics/Blue/BlueBullet002_6_60_21.png", 6, 6, 1, 60 / 6, 21, tempGh);
			GetGraphSize(tempGh[0], &w, &h);
			// �O���t�B�b�N�n���h�����摜�T�C�Y��e�\���̂֓n��
			for (int i = 0; i < PSHOT_NUM; i++) {
				for (int k = 0; k < BLUE_BULLET_FRAME_MAX; k++) {
					shot[BLUE][i].gh[k] = tempGh[k];
				}
				shot[BLUE][i].width = w;
				shot[BLUE][i].heigh = h;
			}

			// -- ���@�̂̏��� -- 
			// �������Ȃ�


		}

		if (pre_addPower >= 3) {
			
			// -- �ԋ@�̂̏��� --
			// �������Ȃ�

			// -- �@�̂̏��� --
			// - �e�̉摜��؂�ւ� -
			int tempGh[BLUE_BULLET_FRAME_MAX];// �ꎞ�I�ȃO���t�B�b�N�n���h��
			int w, h;
			LoadDivGraph("graphics/Blue/BlueBullet003_6_90_35.png", 6, 6, 1, 90/6, 35, tempGh);
			GetGraphSize(tempGh[0], &w, &h);
			// �O���t�B�b�N�n���h�����摜�T�C�Y��e�\���̂֓n��
			for (int i = 0; i < PSHOT_NUM; i++) {
				for (int k = 0; k < BLUE_BULLET_FRAME_MAX; k++) {
					shot[BLUE][i].gh[k] = tempGh[k];
				}
				shot[BLUE][i].width = w;
				shot[BLUE][i].heigh = h;
			}
			
			// -- ���@�̂̏��� --
			// �������Ȃ�


		}
		if (pre_addPower >= 6) {
			
			// -- �ԋ@�̂̏��� --
			// �������Ȃ�

			// -- �@�̂̏��� --
			// - �e�̉摜��؂�ւ� -
			int tempGh[BLUE_BULLET_FRAME_MAX];// �ꎞ�I�ȃO���t�B�b�N�n���h��
			int w, h;
			LoadDivGraph("graphics/Blue/BlueBullet004_6_120_56.png", 6, 6, 1, 120/6, 56, tempGh);
			GetGraphSize(tempGh[0], &w, &h);
			// �O���t�B�b�N�n���h�����摜�T�C�Y��e�\���̂֓n��
			for (int i = 0; i < PSHOT_NUM; i++) {
				for (int k = 0; k < BLUE_BULLET_FRAME_MAX; k++) {
					shot[BLUE][i].gh[k] = tempGh[k];
				}
				shot[BLUE][i].width = w;
				shot[BLUE][i].heigh = h;
			}

			// -- ���@�̂̏��� --
			// �������Ȃ�

		}

		return true;
	}
	else {
		return false;
	}
}

bool Player::GetTranformFlag() {
	// --- �v���C���[�̕ό`�t���O��Ԃ� ---
	return tr_seFlag;
}

bool Player::GetShiftedFlying() {
	// --- �V�t�g�ō�����s�����ǂ�����Ԃ� ---
	return hs_shifted;
}

bool Player::GetPenetrationFlag() {
	// --- �v���C���[���ђʒe�g�p�����ǂ�����Ԃ� ---
	return penetrationFlag;

}

void Player::SetShotFlag(int mechNum, int index, bool flag) {
	// --- �w�肵���e�̃t���O��ύX ---
	// (��ɒe���������������Ɏg�p)
	shot[mechNum][index].flag = flag;
}

void Player::SetDamage(int element, int damage) {
	// --- ��e���ɌĂяo���B�e�̑������_���[�W�Z�b�g
	this->damagedElement = element;
	this->damagedNum = damage;
	damageFlag = true;
}

void Player::SetRespawn() {
	// --- �v���C���[�����X�|�[��������(�R���e�B�j���[�I��)
	respawnFlag = true;
	lifeNum = PLAYER_INITIAL_LIFENUM;
}

void Player::SetItem(int type) {
	// --- �A�C�e���̌��ʃZ�b�g�p�֐� ---
	switch (type)
	{
	case ITEM_HP:
		// HP��
		hp += ITEM_HP_RECOVERY_AMOUNT;
		if (hp >= PLAYER_MAX_HP) {
			hp = PLAYER_MAX_HP;
		}
		break;

	case ITEM_POWER:
		// �U���̓A�b�v
		additionalPower += 1;
		if (additionalPower >= PLAYER_STATUS_UP_MAX) {
			additionalPower = PLAYER_STATUS_UP_MAX;
		}
		break;

	case ITEM_DEFENSE:
		// �h��̓A�b�v
		additionalDefense += 1;
		if (additionalDefense >= PLAYER_STATUS_UP_MAX) {
			additionalDefense = PLAYER_STATUS_UP_MAX;
		}
		break;

	case ITEM_SPEED:
		// �X�s�[�h�A�b�v
		additionalSpeed += 1;
		if (additionalSpeed >= PLAYER_STATUS_UP_MAX) {
			additionalSpeed = PLAYER_STATUS_UP_MAX;
		}
		break;

	case ITEM_BOMB:
		bombNum += 1;
		if (bombNum >= PLAYER_MAX_BOMBNUM) {
			bombNum = PLAYER_MAX_BOMBNUM;
		}
		break;

	}
}