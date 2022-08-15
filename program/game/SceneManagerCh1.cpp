#include "SceneManagerCh1.h"
#include "define.h"
#include "sceneDefine.h"


// - ���j���[��ʁF�I������`

// �|�[�Y���
#define P_CONTINUE_SELECT 0
#define P_OPTION_SELECT 1
#define P_CHAPTER_SELECT 2
#define P_TITLE_SELECT 3

// �Q�[���I�[�o�[���
#define CONTINUE_SELECT 0			// �R���e�B�j���[�Z���N�g
#define RETRY 1						// �`���v�^�[�����[�h
#define TITLE_SELECT 2				// �^�C�g���Z���N�g

SceneManagerCh1::SceneManagerCh1(int chapterNum, char backName[NAME_SIZE], char effect1[NAME_SIZE], char effect2[NAME_SIZE], char enemyData[NAME_SIZE]) {
	// --- �R���X�g���N�^ --- 
	
	// -- �ϐ��������� --
	// - �I�������`���v�^�[�ԍ� -
	this->chapterNum = chapterNum;
	// - ���� - 
	totalTime = 0;
	// - ���W��� --
	playerX = 0;		// �v���C���[���W
	playerY = 0;
	// - �X�|�[�� -
	enemySpawnCount = 0;
	spawnEndFlag = false;
	// - All���������t���O -
	allInit = false;
	// - �|�[�Y��� -
	pauseDrawInit = false;
	pauseFlag = false;
	pauseWaitTime = 0;
	pauseBackGh = 0;
	pauseTitleLogoGh = 0;
	pauseSelectLogo = 0;
	pauseHelpLogo = 0;
	pauseHelpFlag = false;
	// - �Q�[���I�[�o�[���
	gameOverDrawInit = false;
	gameOverFlag = false;
	gameOverWindowTime = 0;
	gameOverTitle = 0;
	gameOverSelectLogo = 0;
	// ���j���[���
	fs_choiceSE = false;
	hs_choiceSE = 0;
	fs_decisionSE = false;
	hs_decisionSE = 0;
	// - ���U���g��� - 
	resultFlag = false;
	resultInit = false;
	resultBackGh = 0;
	resultTitleGh = 0;
	resultLogo1Gh = 0;
	resultLogo2Gh = 0;
	numGh[10] = { 0 };
	resultWindowTime = 0;
	// - ���ʉ��n���h���� -
	soundInit = false;			// SoundAll()���ŏ�����		
	hb_stageSound = 0;
	hb_bossSound = 0;
	hs_playerRedShot01 = 0;
	hs_playerBlueShot01 = 0;
	hs_playerYellowShot01 = 0;
	hs_playerDead = 0;
	hs_playerTransform = 0;
	fs_playerDamaged = false;
	hs_playerDamaged = 0;
	fs_playerBombShot = false;
	hs_playerBombShot = 0;		
	fs_playerBombExplode = false;	
	hs_playerBombExplode = 0;	
	fs_playerRefrect = false;
	hs_playerRefrect = 0;
	fs_shiftedFlying = false;
	hs_playerFlying = 0;
	hs_playerShiftedFlying = 0;
	fs_enemyDead = false;
	hs_enemyDead = 0;
	fs_enemyDamaged = false;
	hs_enemyDamaged = 0;
	fs_itemGot = false;
	hs_itemGot = 0;
	fs_enemyShot01 = false;
	hs_enemySho01 = 0;
	fs_enemyReflect = false;
	hs_enemyReflect = 0;
	// - BGM,SE���� -
	bgmVolume = BGM_INITIAL_VOLUME;
	seVolume = SE_INITIAL_VOLUME;
	// - ��ʑJ�ڐ�i�[ -
	tempScene = SCENE_NON_CHANGE;
	// - �{�X�t���O�֌W -
	bossExistFlag = false;
	bossExistInit = false;
	deadBossCounter = 0;
	// �`���v�^�[�ԍ��ɉ����ă{�X�̍ő吔�ω�
	switch (chapterNum)
	{
	case 1:
		bossNumMax = 3;
		break;

	default:
		bossNumMax = 1;
		break;
	}
	// �`���[�g���A��
	tutorialFlag = false;
	tutorialEndFlag = false;
	// -- �C���X�^���X�̍쐬 -- 
	player = new Player();		// �v���C���[
	back = new Back(backName, effect1, effect2);			// �w�i
	score = new Score();		// �X�R�A�{�[�h
	for (int i = 0; i < ITEM_NUM; i++) {
		// �A�C�e���N���X
		item[i] = new Item;
	}

	// -- �G�f�[�^�̓ǂݍ���(enemyControlData.csv �Q��) --
	// �� �C���X�^���X��All()���ŊǗ�
	FILE* fp;
	char buff[100];
	int c1;
	int col = 1;
	int row = 0;
	int err;
	memset(buff, 0, sizeof(buff));
	// �t�@�C����ǂݎ��ŊJ��
	//err = fopen_s(&fp, "enemyControlDataCh1.csv", "r");
	err = fopen_s(&fp, enemyData, "r");
	if (err != 0) {
		DrawStringEx(200, 200, -1, "file not open!\n");
	}

	// �w�b�_�t�@�C���ǂݔ�΂�(3�s�ǂݔ�΂�)
	while (fgetc(fp) != '\n');
	while (fgetc(fp) != '\n');
	while (fgetc(fp) != '\n');

	while (true) {
		while (true) {
			c1 = fgetc(fp);
			// �����Ȃ烋�[�v�𔲂���
			if (c1 == EOF) {
				goto out;
			}
			// �J���}, ���s�Ŗ�����Ε����Ƃ��ĂȂ���
			if (c1 != ',' && c1 != '\n')
				strcat(buff, (const char*)&c1);
			else
				break;
		}
		// 1�Z�����̕�������f�[�^�Ɋi�[���Ă���
		switch (col)
		{
		case 1:	 edata[row].x				= atoi(buff); break;
		case 2:  edata[row].y				= atoi(buff); break;
		case 3:  edata[row].type			= atoi(buff); break;
		case 4:  edata[row].stype			= atoi(buff); break;
		case 5:  edata[row].s_pattern		= atoi(buff); break;
		case 6:  edata[row].item			= atoi(buff); break;
		case 7:  edata[row].m_pattern		= atoi(buff); break;
		case 8:  edata[row].in_time			= atof(buff); break;
		case 9:  edata[row].start_fire_time = atof(buff); break;
		case 10: edata[row].end_fire_time	= atof(buff); break;
		case 11: edata[row].out_time		= atof(buff); break;
		case 12: edata[row].outX			= atoi(buff); break;
		case 13: edata[row].outY			= atoi(buff); break;
		case 14: edata[row].moveK[0]		= atoi(buff); break;
		case 15: edata[row].moveK[1]		= atoi(buff); break;
		case 16: edata[row].moveK[2]		= atoi(buff); break;
		case 17: edata[row].moveK[3]		= atoi(buff); break;
		case 18: edata[row].moveK[4]		= atoi(buff); break;
		case 19: edata[row].moveK[5]		= atoi(buff); break;
		case 20: edata[row].moveK[6]		= atoi(buff); break;
		case 21: edata[row].moveK[7]		= atoi(buff); break;
		case 22: edata[row].moveK[8]		= atoi(buff); break;
		case 23: edata[row].moveK[9]		= atoi(buff); break;
		case 24: edata[row].moveK[10]		= atoi(buff); break;
		case 25: edata[row].moveK[11]		= atoi(buff); break;
		case 26: edata[row].moveK[12]		= atoi(buff); break;
		}
		// �o�b�t�@������
		memset(buff, 0, sizeof(buff));
		// �񐔂𑫂�
		++col;
		// �����ǂݍ��񂾕��������s�ł���΁A�񐔏��������s������
		if (c1 == '\n') {
			col = 1;
			++row;
		}
		
	}
out:
	fclose(fp);	// �t�@�C�������
	// - �G�f�[�^�ǂݍ��ݏI�� -
	// ->�N���X��All�֐����ŃC���X�^���X������. �b��G��nullpointer��^����
	for (int i = 0; i < ENEMY_NUM; i++) {
		enemy[i] = nullptr;
	}


}

SceneManagerCh1::~SceneManagerCh1() {
	// --- �f�X�g���N�^ --- 

	// -- BGM��~ --
	if (CheckSoundMem(hb_stageSound)) {
		StopSoundMem(hb_stageSound);
	}

	if (CheckSoundMem(hb_bossSound)) {
		StopSoundMem(hb_bossSound);
	}

	// -- ��s����~ --
	if (CheckSoundMem(hs_playerFlying)) {
		StopSoundMem(hs_playerFlying);
	}
	if (CheckSoundMem(hs_playerShiftedFlying)) {
		StopSoundMem(hs_playerShiftedFlying);
	}
	// -- �C���X�^���X�j�� --
	delete player;
	delete back;
	delete score;
	for (int i = 0; i < enemySpawnCount; i++) {
		if (enemy[i] != NULL) {
			delete enemy[i];
		}
	}
	for (int i = 0; i < ITEM_NUM; i++) {
		delete item[i];
	}

}

int SceneManagerCh1::All(float delta_time) {
	// ---  �V�[�����I�u�W�F�N�g�̊Ǘ� ---
	
	// -- �f�o�b�O�p�@�\ --
	if (tnl::Input::IsKeyDown(eKeys::KB_LCONTROL) && tnl::Input::IsKeyDown(eKeys::KB_T)) {
		delta_time = delta_time * 5;
	}
	if (tnl::Input::IsKeyDown(eKeys::KB_LCONTROL) && tnl::Input::IsKeyDown(eKeys::KB_P)) {
		player->SetRespawn();
		for (int i = 0; i < 9; i++) {
			for (int k = 0; k < 5; k++) {
				player->SetItem(k);
			}
		}
	}
	if (tnl::Input::IsKeyDown(eKeys::KB_LCONTROL) && tnl::Input::IsKeyDown(eKeys::KB_K)) {
		for (int i = 0; i < enemySpawnCount; i++) {
			if (enemy[i] != NULL) {
				enemy[i]->SetDamage(NON_ELEMENT, 9999);
				enemy[i]->DamageCalc();
			}
		}
	}

	// -- �`���[�g���A��(��ʕ\���͌�Ɏ��s) --
	if (chapterNum == 0 && !tutorialEndFlag) {
		tutorialFlag = true;
		// �|�[�Y���s
		delta_time = 0;
	}

	// -- �������� --
	if (!allInit) {
		// - �}�E�X�J�[�\���ʒu��\��
		SetMouseDispFlag(false);
		allInit = true;
	}

	// -- �{�X�o�� --
	if (BossExistFlag()) {
		bossExistFlag = true;
	}
	
	// - �{�X���j���J�E���g -
	if (BossDefeatFlag()) {
		deadBossCounter++;
	}

	// -- �Q�[���N���A�m�F(���U���g��ʕ\���͌�Ɏ��s) --
	if (deadBossCounter >= bossNumMax && !resultFlag) {
		// ���U���g��ʕ\��
		resultFlag = true;
		// ���U���g��ʕ\���f�B���C�J�E���^
		resultWindowTime = 1.5;
	}
	if (resultFlag && resultWindowTime <= 0) {
		// - �|�[�Y���s
		delta_time = 0;
	}


	// -- �Q�[���I�[�o�[����(�Q�[���I�[�o�[��ʕ\���͌�Ɏ��s --
	if (player->GetLifeNum() <= 0 && !gameOverFlag) {
		// �v���C���[�̃��C�t�����Ȃ��Ȃ�����
		gameOverFlag = true;
		// - �E�B���h�E�\���f�B���C�J�E���^ -
		gameOverWindowTime = 2.0;
	}
	
	if (gameOverFlag && gameOverWindowTime <= 0) {
		// - �|�[�Y���s
		delta_time = 0;
	}

	// -- �{�[�Y���s(�{�[�Y��ʕ\���͌�Ɏ��s) -- 
	if (!pauseFlag) {
		// - ESC���͂Ń{�[�Y�t���O�𗧂Ă�(�Q�[���I�[�o�[�������U���g���͎��{�o���Ȃ�) -
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_ESCAPE) && !gameOverFlag && !resultFlag) {
			pauseFlag = true;
			// - �`���^�����O�h�~�p�J�E���^ -
			pauseWaitTime = 1.0;
			// - ��������摜�\���t���O��܂�
			pauseHelpFlag = false;
		}
	} else {
		// - �Q�[�����ݎ��Ԃ�0�ɂ��� - 
		delta_time = 0;
		pauseWaitTime -= 0.1;
		if (pauseWaitTime <= 0) {
			pauseWaitTime = 0;
		}
		// - ESC���͂ŃQ�[�����s -
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_ESCAPE) && pauseWaitTime <= 0) {
			pauseFlag = false;
		}
	}

	// -- �V�[���Ǘ� --
	// - �`��̈�w�� - 
	SetDrawArea(MARGINE, MARGINE, WIDTH - MENU_WIDTH, HEIGHT - MARGINE);

	// -- �I�u�W�F�N�g�Ǘ� --
	// �w�i�`��
	back->All(delta_time);

	// �v���C���[���W���擾
	GetPlayerPosition();

	// �G����
	// - �^�C���e�[�u���̏ォ�珇�ɓG���X�|�[������
	if (!spawnEndFlag) {
		if (edata[enemySpawnCount].in_time <= totalTime) {
			// �o�ߎ��Ԃɉ����ēG���X�|�[��
			enemy[enemySpawnCount] = new Enemy(
				totalTime,
				(double)edata[enemySpawnCount].x,
				(double)edata[enemySpawnCount].y,
				edata[enemySpawnCount].type,
				edata[enemySpawnCount].stype,
				edata[enemySpawnCount].s_pattern,
				edata[enemySpawnCount].item,
				edata[enemySpawnCount].m_pattern,
				edata[enemySpawnCount].in_time,
				edata[enemySpawnCount].start_fire_time,
				edata[enemySpawnCount].end_fire_time,
				edata[enemySpawnCount].out_time,
				(double)edata[enemySpawnCount].outX,
				(double)edata[enemySpawnCount].outY,
				edata[enemySpawnCount].moveK
			);
			enemySpawnCount++;

			if (enemySpawnCount >= ENEMY_NUM) {
				// �^�C���e�[�u���̓G��S�ăX�|�[������
				enemySpawnCount = ENEMY_NUM;
				spawnEndFlag = true;
			}
		}
	}

	// - �G�𓮂����B��ڂ��I�����G�͏������� - 
	for (int i = 0; i < enemySpawnCount; i++) {
		if (enemy[i] != NULL) {

			// �G�����S�����u�ԁi1�t���[��)�̃t���O�m�F���������{
			if (enemy[i]->GetDeadMoment()) {
				// �G���S���ʉ��t���O��L����
				fs_enemyDead = true;
				// �X�R�A�ɓ��_�����Z����
				score->SetScore(CURRENT_SCORE, enemy[i]->GetScore(totalTime));
				// �A�C�e���o��
				for (int k = 0; k < ITEM_NUM; k++) {
					// �t���O���Z�b�g�̃A�C�e���������Z�b�g
					if (!item[k]->GetFlag()) {
						double enemyItemX, enemyItemY;
						enemy[i]->GetPosition(&enemyItemX, &enemyItemY);
						item[k]->SetFlag(enemyItemX, enemyItemY, enemy[i]->GetItem());
						break;
					}
				}

			}

			// �G��All�֐����s�B�����ɁA���S�t���O�����˒e�̖����G�͏�������
			if (enemy[i]->All(delta_time, this->playerX, this->playerY)) {
				delete enemy[i];
				enemy[i] = NULL;
			}

		}
	}

	// - �v���C���[���S�̊ԁA�G�͓P�ނ��� -
	if (!player->GetLife()) {
		for (int i = 0; i < enemySpawnCount; i++) {
			if (enemy[i] != NULL) {
				enemy[i]->SetOutTime(totalTime);
			}
		}
	}

	// �v���C���[����
	player->All(delta_time);

	// - �����蔻�� - //
	CollisionAll();

	// - �����E���ʉ��Đ� - //
	SoundAll();

	// - ���ԊǗ� - 
	Time(delta_time);

	// - �A�C�e�� -
	for (int i = 0; i < ITEM_NUM; i++) {
		if (item[i]->GetFlag()) {
			item[i]->All(delta_time);
		}
	}

	// -- �X�R�A�{�[�h -- 
	// - �`��̈���w�� 
	SetDrawArea(0, 0, WIDTH - MARGINE, HEIGHT - MARGINE);
	// - �X�R�A���e�X�V -
	ScoreUpdate();
	// - �X�R�A�`�� -
	score->All();


	// �Q�[���I�[�o�[��ʎ��s�֐�(��ʕ\���n�F�ǁX�N���X��������) - 
	if (gameOverFlag && gameOverWindowTime <= 0) {
		GameOverWindow(gameOverFlag);
		// �G���^�[���͎��A�R���e�B�j���[�ȊO��I���̏ꍇ�A�V�[���؂�ւ������{
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
			tempScene = GameOverWindow(gameOverFlag);

			// ���[�U�[���R���e�B�j���[��I�񂾏ꍇ
			if (tempScene == SCENE_NON_CHANGE) {
				// �v���C���[����
				player->SetRespawn();
				// �X�R�A���_
				score->SetScore(CURRENT_SCORE, -2000);
				// �Q�[���I�[�o�[�t���O��܂�
				gameOverFlag = false;
			}
		}
		else {
			tempScene = SCENE_NON_CHANGE;
		}
		
	}

	// ���U���g��ʎ��s�֐� - 
	if (resultFlag && resultWindowTime <=0) {
		ResultWindow(resultFlag);
		// �G���^�[���͎��A�V�[���؂�ւ������{
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
			tempScene = ResultWindow(resultFlag);
		}
		else {
			tempScene = SCENE_NON_CHANGE;
		}
	}

	// - �`���[�g���A����ʎ��s - 
	if (tutorialFlag && !tutorialEndFlag) {
		tutorialEndFlag = tutorial.All();
	}

	// - �|�[�Y��ʎ��s�֐� -
	if(pauseFlag){
		PauseWindow(pauseFlag);
		// �G���^�[���͎��A�R���e�B�j���[�ȊO��I���̏ꍇ�A�V�[���؂�ւ������{
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
			tempScene = PauseWindow(pauseFlag);

			// ���[�U�[���R���e�B�j���[��I�񂾏ꍇ
			if (tempScene == SCENE_NON_CHANGE) {
				// �|�[�Y��ʏI�� 
				pauseFlag = false;
			}
		}
		else {
			tempScene = SCENE_NON_CHANGE;
		}
	}
	
	return tempScene;

}

void SceneManagerCh1::SoundAll() {
	// --- �T�E���h�̊Ǘ� ---
	// -- ���������� --
	if (!soundInit) {
		hb_stageSound = LoadSoundMem("sound/BGM/Ch1_ARPCHAIN.mp3");
		hb_bossSound = LoadSoundMem("sound/BGM/CH1_BossBGM.mp3");
		hs_playerRedShot01 = LoadSoundMem("sound/RedEffect/RedBullet001_MachinGun.mp3");
		hs_playerBlueShot01 = LoadSoundMem("sound/BlueEffect/BlueBullet001_Ray.mp3");
		hs_playerYellowShot01 = LoadSoundMem("sound/YellowEffect/YellowBullet001_Gus.mp3");
		hs_playerDead = LoadSoundMem("sound/Player/PlayerDeadSE.mp3");
		hs_playerTransform = LoadSoundMem("sound/Player/PlayerTransform.wav");
		hs_playerDamaged = LoadSoundMem("sound/Player/PlayerDamaged.mp3");
		hs_playerBombShot = LoadSoundMem("sound/Player/PlayerBombShot.mp3");
		hs_playerBombExplode = LoadSoundMem("sound/Player/PlayerBombExplode.mp3");
		hs_playerRefrect = LoadSoundMem("sound/Player/PlayerRefrect.mp3");
		hs_playerFlying = LoadSoundMem("sound/Player/PlayerFlying.mp3");
		hs_playerShiftedFlying = LoadSoundMem("sound/Player/PlayerShiftedFlying.mp3");
		hs_enemyDead = LoadSoundMem("sound/EnemyEffect/EnemyDead.mp3");
		hs_enemyDamaged = LoadSoundMem("sound/EnemyEffect/EnemyDamaged.mp3");
		hs_itemGot = LoadSoundMem("sound/Player/PlayerItemGot.mp3");
		hs_enemySho01 = LoadSoundMem("sound/EnemyEffect/EnemyShot001.wav");
		// ���j���[���
		hs_choiceSE = LoadSoundMem("sound/Title/TitleCursorSE.mp3");
		hs_decisionSE = LoadSoundMem("sound/Title/TitleDecisionSE.mp3");
		hs_enemyReflect = LoadSoundMem("sound/EnemyEffect/EnemyReflect.mp3");

		PlaySoundMem(hb_stageSound, DX_PLAYTYPE_LOOP);

		// - �����������I�� -
		soundInit = true;
	}

	// -- ���y�ύX --
	if (bossExistFlag) {
		// �{�X�o�����ABGM�ύX
		if (!bossExistInit) {
			StopSoundMem(hb_stageSound);
			PlaySoundMem(hb_bossSound, DX_PLAYTYPE_LOOP);

			bossExistInit = true;
		}
	}

	// -- ���ʉ��Đ� --
	
	// - ���@�V���b�g���ʉ��Ǘ� -
	int tempShotType = player->GetShotFlag();
	switch (tempShotType)
	{
	case RED:
		PlaySoundMem(hs_playerRedShot01, DX_PLAYTYPE_BACK);
		break;

	case BLUE:
		PlaySoundMem(hs_playerBlueShot01, DX_PLAYTYPE_BACK);
		break;

	case YELLOW:
		PlaySoundMem(hs_playerYellowShot01, DX_PLAYTYPE_BACK);
		break;

	default:
		// �������Ȃ�
		break;
	}
	// - ���@�j����ʉ��@or �_���[�W���ʉ� -
	if (player->GetDeadFlag()) {
		// ���@�j�󉹍Đ�
		PlaySoundMem(hs_playerDead, DX_PLAYTYPE_BACK);
	}
	else if (player->GetDamageSoundFlag()) {
		// ���@�_���[�W��(���G���ԊO�������Ă��)
		if (player->GetLife()) {
			PlaySoundMem(hs_playerDamaged, DX_PLAYTYPE_BACK);
			fs_playerDamaged = false;
		}
	}
	
	// - ���@�ό`��
	if (player->GetTranformFlag()) {
		PlaySoundMem(hs_playerTransform, DX_PLAYTYPE_BACK);
	}

	// - ���@�{�����ˉ� -
	if (player->GetBombShotSoundFlag()) {
		fs_playerBombShot = true;
	}
	if (fs_playerBombShot) {
		PlaySoundMem(hs_playerBombShot, DX_PLAYTYPE_BACK);
		fs_playerBombShot = false;
	}

	// - ���@�{�����j�� -
	if (player->GetBombExplodeSoundFlag()) {
		fs_playerBombExplode = true;
	}
	if (fs_playerBombExplode) {
		PlaySoundMem(hs_playerBombExplode, DX_PLAYTYPE_BACK);
		fs_playerBombExplode = false;
	}

	// - ���@�m�[�_���[�W�� -
	if (player->GetRefrectFlag()) {
		fs_playerRefrect = true;
	}
	if (fs_playerRefrect) {
		PlaySoundMem(hs_playerRefrect, DX_PLAYTYPE_BACK);
		fs_playerRefrect = false;
	}

	// - ���@��s���� -
	if (!player->GetDeadFlag()) {
		// �v���C���[�������Ă�΍Đ�
		if (!CheckSoundMem(hs_playerFlying)) {
			PlaySoundMem(hs_playerFlying, DX_PLAYTYPE_LOOP);
		}
		if (player->GetShiftedFlying()) {
			// ������s���͌��ʉ��ύX
			if (CheckSoundMem(hs_playerFlying)) {
				StopSoundMem(hs_playerFlying);
			}
			if (!CheckSoundMem(hs_playerShiftedFlying)) {
				PlaySoundMem(hs_playerShiftedFlying, DX_PLAYTYPE_LOOP);
			}
		}
		else {
			StopSoundMem(hs_playerShiftedFlying);
		}
	}

	// - �G�_���[�W�� - 
	if (fs_enemyDamaged) {
		PlaySoundMem(hs_enemyDamaged, DX_PLAYTYPE_BACK);
		fs_enemyDamaged = false;
	}

	// - �G�m�[�_���[�W��
	if (fs_enemyReflect) {
		PlaySoundMem(hs_enemyReflect, DX_PLAYTYPE_BACK);
		fs_enemyReflect = false;
	}

	// - �G���C��
	if (!gameOverFlag && !pauseFlag && !resultFlag) {
		for (int i = 0; i < enemySpawnCount; i++) {
			if (enemy[i] != NULL) {
				if (enemy[i]->GetShotFlag()) {
					fs_enemyShot01 = true;
					break;
				}
			}
		}
	}
	if (fs_enemyShot01) {
		PlaySoundMem(hs_enemySho01, DX_PLAYTYPE_BACK);
		fs_enemyShot01 = false;
	}

	// - �G���S�� -
	if (fs_enemyDead) {
		PlaySoundMem(hs_enemyDead, DX_PLAYTYPE_BACK);
		fs_enemyDead = false;
	}

	// - �A�C�e���擾�� - 
	if (fs_itemGot) {
		PlaySoundMem(hs_itemGot, DX_PLAYTYPE_BACK);
		fs_itemGot = false;
	}

	// - ���j���[���(�I����)
	if (fs_choiceSE) {
		PlaySoundMem(hs_choiceSE, DX_PLAYTYPE_BACK);
		fs_choiceSE = false;
	}

	// ���j���[���(���艹)
	if (fs_decisionSE) {
		PlaySoundMem(hs_decisionSE, DX_PLAYTYPE_BACK);
		fs_decisionSE = false;
	}


}

void SceneManagerCh1::CollisionAll() {
	// --- �����蔻������� ---
	
	// -- �ꎞ�ϐ� --
	double t_enemyX, t_enemyY;
	double t_eShotX, t_eShotY;
	double t_playerX, t_playerY;
	double t_pShotX, t_pShotY;
	double t_itemX, t_itemY;
	bool t_damageFlag = false;
	double t_bombX, t_bombY;

	// -- �v���C���[�̒e�Ƃ̓����蔻�� --
	for (int i = 0; i < MACHINE_NUM; i++) {
		for (int k = 0; k < PSHOT_NUM; k++) {
			// ���˒��̒e�𔻕ʂ��Ĕ�����{
			if (player->GetShotPosition(i, k, &t_pShotX, &t_pShotY)) {
				// �G�N���X�̃|�C���g��NULL�łȂ��@����
				// deadFlag ��False
				for (int n = 0; n < enemySpawnCount; n++) {
					if (enemy[n] != NULL && !enemy[n]->GetDeadFlag()) {
						// �G�̈ʒu�擾
						enemy[n]->GetPosition(&t_enemyX, &t_enemyY);
						// - �����蔻�� - 
						// �v���C���[�̒e�̎�ނɉ����ē����蔻��ω�
						int tempCollision;
						tempCollision = player->GetBulletCollision();

						if (CircleCollision(tempCollision, enemy[n]->GetCollision(), t_pShotX, t_enemyX, t_pShotY, t_enemyY)) {
							// �������Ă���ΓG��DamageFlag�𗧂Ă�
							// �_���[�W�v�Z�𑦍��Ɏ��{
							enemy[n]->SetDamage(player->GetShotElement(i, k), player->GetShotDamage(i, k));
							enemy[n]->DamageCalc();
							// ���Ă��v���C���[�̒e�̃t���O��߂�(�ђʒe�͂̂���)
							if (!player->GetPenetrationFlag()) {
								player->SetShotFlag(i, k, false);
							}


							// ���ʉ��t���O
							if (enemy[n]->GetDamageFlag()) {
								fs_enemyDamaged = true;
							}
							if (enemy[n]->GetRefrectFlag()) {
								fs_enemyReflect = true;
							}

						}

					}
				}
			}
		}
	}

	// -- �v���C���[�̃{���Ƃ̓����蔻�� --
	if (player->GetBombExplodeFlag()) {
		// ���e�������������̂ݎ��s
		player->GetBombPosition(&t_bombX, &t_bombY);
		// �o�������������̓G�ɂ̂ݓ����蔻��v�Z
		for (int i = 0; i < enemySpawnCount; i++) {
			if (enemy[i] != NULL && !enemy[i]->GetDeadFlag()) {
				enemy[i]->GetPosition(&t_enemyX, &t_enemyY);	// �G�̈ʒu���擾
				// �����蔻��v�Z
				if (CircleCollision(player->GetBombCollision(), enemy[i]->GetCollision(), t_bombX, t_enemyX, t_bombY, t_bombY)) {
					// �������Ă���ΓG��DamageFlag�𗧂Ă�B�{���͖������U��
					// �_���[�W�v�Z�𑦍��Ɏ��{
					enemy[i]->SetDamage(NON_ELEMENT, player->GetBombDamage());
					enemy[i]->DamageCalc();
					
				}

			}
		}

	}


	// -- �G�̒e�Ƒ���L�����Ƃ̓����蔻��
	// �v���C���[�������Ă��鎞����
	if (player->GetLife()) {
		// ���G���ԂłȂ�������
		if(!player->GetDamageInvalidFlag()){
			if (!player->GetDamageFlag()) {
				// ���ɔ�e����Ŗ�����Έȉ����{
				player->GetPosition(&t_playerX, &t_playerY);
				for (int i = 0; i < enemySpawnCount; i++) {
					// ���ݏo�����̓G�̐��������[�v
					if (enemy[i] != NULL) {
						for (int k = 0; k < ENEMY_SNUM; k++) {
							// �e�t���O true : �t�B�[���h��ɑ��݂���e�݂̂œ����蔻����{
							if (enemy[i]->GetShotPosition(k, &t_eShotX, &t_eShotY)) {
								// �����蔻����{(�G�̒e��Collision�T�C�Y���Q��)
								int tempCollision = enemy[i]->GetShotCollision(k);

								// �O���C�Y����

								// �_���[�W����
								if (CircleCollision(player->GetCollision(), tempCollision, t_playerX, t_eShotX, t_playerY, t_eShotY)) {
									// �ꎞ�I�ȃ_���[�W�t���O on
									t_damageFlag = true;
								}


								// �_���[�W����Ftrue���̏����F
								if (t_damageFlag) {
									// ��e�t���O�Z�b�g�A�e�̑����A�_���[�W�ʂ��Z�b�g
									player->SetDamage(enemy[i]->GetShotElement(k), enemy[i]->GetShotDamage(k));
									// �e�̏���
									enemy[i]->SetShotFlag(k, false);
									// �ꎞ�I�ȃt���O��߂�
									t_damageFlag = false;
								}
							}
						}
					}
				}
			}
		}

	}
	
	// -- �A�C�e���ƃv���C���[�̓����蔻�� -- 
	for (int i = 0; i < ITEM_NUM; i++) {
		// - �t���O���L���ȃA�C�e���̂ݎQ�� - 
		if (item[i]->GetFlag()) {
			item[i]->GetPosition(&t_itemX, &t_itemY);
			// �v���C���[�ƃA�C�e���̓����蔻��
			player->GetPosition(&t_playerX, &t_playerY);
			if (CircleCollision(player->GetCollision(), ITEM_COLLISION, t_playerX, t_itemX, t_playerY, t_itemY)) {
				// �A�C�e���̌��ʂ��v���C���[�ɗ^����
				player->SetItem(item[i]->GetType());
				// �A�C�e�����Ńt���O�𗧂Ă�
				item[i]->Delete();
				// �A�C�e���擾���Z�b�g
				fs_itemGot = true;

			}
		}
	}

}


bool SceneManagerCh1::CircleCollision(double cir1, double cir2, double cirX1, double cirX2, double cirY1, double cirY2) {
	// --- �~�`�����蔻�� ---
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

void SceneManagerCh1::GetPlayerPosition() {
	// --- �v���C���[�̈ʒu���擾 ---
	player->GetPosition(&this->playerX, &this->playerY);
}

void SceneManagerCh1::Time(float delta_time) {
	// --- ���Ԃ̊Ǘ� ---  
	totalTime += delta_time;	// �o�ߎ���
	if (gameOverFlag) {
		// - �Q�[���I�[�o�[�E�B���h�E�\���f�B���C�J�E���^ -
		gameOverWindowTime -= delta_time;
		if (gameOverWindowTime <= 0) {
			gameOverWindowTime = 0;
		} 
	}
	else {
		gameOverWindowTime = 0;
	}

	if (resultFlag) {
		// - ���U���g��ʕ\���f�B���C�J�E���^ -
		resultWindowTime -= delta_time;
		if (resultWindowTime <= 0) {
			resultWindowTime = 0;
		}
	}
	else {
		resultWindowTime = 0;
	}
}

void SceneManagerCh1::ScoreUpdate() {
	// --- �X�R�A�{�[�h�̏��X�V�F�v���C���[ ---
	score->SetScore(LIFE, player->GetLifeNum());
	score->SetScore(BOMB, player->GetBombNum());
	score->SetScore(HP, player->GetHpNum());
	score->SetScore(POWER, player->GetPowerNum());
	score->SetScore(SPEED, player->GetSpeedNum());
	score->SetScore(DEFENSE, player->GetDefenseNum());
	score->SetScore(ELEMENT, player->GetElement());
}

int SceneManagerCh1::PauseWindow(bool pauseFlag) {
	// --- �{�[�Y��ʕ\������ʑJ�ڃt���O(sceneDefine.h�Q��)��Ԃ� ---
	if (pauseFlag) {
		// -- �|�[�Y��ʕ`�� ---
		if (!pauseDrawInit) {
			// - �`�揉������ -
			pauseBackGh = LoadGraph("graphics/Pause/PauseShade_1_1024_768.png");
			pauseTitleLogoGh = LoadGraph("graphics/Pause/PauseLogo.png");
			LoadDivGraph("graphics/Pause/PauseSelection_8_225_488.png", 8, 1, 8, 225, 488 / 8, pauseSelectLogoGh);
			guideGh = LoadGraph("graphics/Pause/OptionGuide.png");
			pauseHelpLogo = LoadGraph("graphics/Pause/Help.png");
			controlGuideGh = LoadGraph("graphics/Pause/Controls.png");

			pauseDrawInit = true;
		}
		// - �w�i�\��(��ʂ��Â�����) -
		DrawGraph(0, 0, pauseBackGh, true);
		DrawRotaGraph(100, 90, 0.7, 0, pauseTitleLogoGh, true, false);
		// - ����K�C�h�̕\�� -
		DrawRotaGraph(130, 200, 0.7, 0, guideGh, true, false);
		// - �I�����̕\�� -
		for (int i = 0; i < 4; i++) {
			DrawRotaGraph(WIDTH / 2, 120 + i * 75, 0.7, 0, pauseSelectLogoGh[i], true, false);
		}
		// - �w���v�K�C�h�̕\��
		DrawRotaGraph(150, 275, 0.7, 0, pauseHelpLogo, true, false);
		// - H���͎��A ������@�����摜�\��
		if (pauseHelpFlag) {
			// ������@�K�C�h�̕\��
			DrawRotaGraph(WIDTH/2, HEIGHT/2, 1, 0, controlGuideGh, true, false);
			if (tnl::Input::IsKeyDownTrigger(eKeys::KB_ESCAPE)) {
				pauseHelpFlag = false;
			}
		}
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_H)) {
			pauseHelpFlag = true;
		} 
		


		// -- ���S�Z���N�g --
		if (!pauseHelpFlag) {
			if (tnl::Input::IsKeyDownTrigger(eKeys::KB_DOWN)) {
				pauseSelectLogo++;
				fs_choiceSE = true;
				if (pauseSelectLogo > P_TITLE_SELECT) {
					pauseSelectLogo = P_CONTINUE_SELECT;
				}
			}
			else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_UP)) {
				pauseSelectLogo--;
				fs_choiceSE = true;
				if (pauseSelectLogo < P_CONTINUE_SELECT) {
					pauseSelectLogo = P_TITLE_SELECT;
				}
			}
		}

		// - �I�𒆂̃��S�\��
		DrawRotaGraph(WIDTH / 2, 120 + 75 * (pauseSelectLogo), 0.7, 0,  pauseSelectLogoGh[pauseSelectLogo + 4], true, false);



		// -- �G���^�[���͂Ō���SE�𗬂� --
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
			fs_decisionSE = true;
		}

		// -- ���S�Z���N�g�ɉ����đI��������(sceneDefine.h�ƑΉ��t��) -- 
		if (!pauseHelpFlag) {
			switch (pauseSelectLogo)
			{
			case P_CONTINUE_SELECT:
				return SCENE_NON_CHANGE;

				break;

			case P_OPTION_SELECT:
				return SCENE_OPTION;

				break;

			case P_CHAPTER_SELECT:
				return SCENE_CH_SELECT;

			case P_TITLE_SELECT:
				return SCENE_TITLE;

			default:
				return SCENE_NON_CHANGE;

				break;
			}
		}
		else {
			return SCENE_NON_CHANGE;
		}

	}
	else {
		return SCENE_NON_CHANGE;
	}

}

int SceneManagerCh1::GameOverWindow(bool gameOverFlag) {
	// --- �Q�[���I�[�o�[��ʕ\������ʑJ�ڃt���O(sceneDefine.h�Q��)��Ԃ� ---
	if (gameOverFlag) {
		// -- �Q�[���I�[�o�[��ʕ`�� --
		if (!gameOverDrawInit) {
			// - �`�揉������ - 
			gameOverBackGh = LoadGraph("graphics/Pause/PauseShade_1_1024_768.png");		// �|�[�Y��ʔw�i���p
			gameOverTitle = LoadGraph("graphics/GameOver/Title.png");					// �Q�[���I�[�o�[��ʃ^�C�g��
			LoadDivGraph("graphics/GameOver/SelectLogo.png", 6, 1, 6, 378, 342 / 6, gameOverLogoGh);

			gameOverDrawInit = true;
		}
		// - �w�i�\�� (��ʂ��Â�����) -
		DrawGraph(0, 0, gameOverBackGh, true);
		DrawRotaGraph((WIDTH - MENU_WIDTH) / 2, 150, 1.0, 0, gameOverTitle, true, false);
		DrawGraph((WIDTH - MENU_WIDTH) / 2 - 140, 250, gameOverLogoGh[0], true);
		DrawGraph((WIDTH - MENU_WIDTH) / 2 - 140, 300, gameOverLogoGh[1], true);
		DrawGraph((WIDTH - MENU_WIDTH) / 2 - 140, 350, gameOverLogoGh[2], true);

		// - �I�𒆂̃��S�\��
		DrawGraph((WIDTH - MENU_WIDTH) / 2 - 140, 250 + 50 * (gameOverSelectLogo), gameOverLogoGh[gameOverSelectLogo + 3], true);

		// -- ���S�Z���N�g --
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_DOWN)) {
			gameOverSelectLogo++;
			fs_choiceSE = true;
			if (gameOverSelectLogo > TITLE_SELECT) {
				gameOverSelectLogo = CONTINUE_SELECT;
			}
			
		}
		else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_UP)) {
			gameOverSelectLogo--;
			fs_choiceSE = true;
			if (gameOverSelectLogo < CONTINUE_SELECT) {
				gameOverSelectLogo = TITLE_SELECT;
			}
		}

		// -- �G���^�[���͂Ō���SE�𗬂� --
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
			fs_decisionSE = true;
		}

		// -- ���S�Z���N�g�ɉ����đI��������(sceneDefine.h�ƑΉ��t��) -- 
		switch (gameOverSelectLogo)
		{
		case CONTINUE_SELECT:
			return SCENE_NON_CHANGE;

			break;

		case RETRY:
			return SCENE_CH_1;
		
			break;

		case TITLE_SELECT:
			return SCENE_TITLE;

		default:
			return SCENE_NON_CHANGE;

			break;
		}

	
	}
	else {
		return SCENE_NON_CHANGE;
	}

}

int SceneManagerCh1::ResultWindow(bool resultFlag) {
	// --- ���U���g��ʂ�\������ ---
	if (resultFlag) {
		if (!resultInit) {
			// -- �`�揉������ --
			resultBackGh = LoadGraph("graphics/Pause/PauseShade_1_1024_768.png");		// �|�[�Y��ʔw�i���p
			resultTitleGh = LoadGraph("graphics/Result/ResultTitle.png");		
			resultLogo1Gh = LoadGraph("graphics/Result/ResultLogo01.png");
			resultLogo2Gh = LoadGraph("graphics/Result/ResultLogo02.png");
			// �����̉摜�擾
			LoadDivGraph("graphics/Score/ScoreNumber_10_300_38.png", 10, 10, 1, 300 / 10, 38, numGh);
			// �����N�̉摜�擾
			LoadDivGraph("graphics/Result/ResultRank_5_475_95.png", 5, 5, 1, 475 / 5, 95, rankGh);

			resultInit = true;
		}
		// - �w�i�\��(��ʂ��Â�����) -
		DrawGraph(0, 0, resultBackGh, true);
		DrawRotaGraph((WIDTH - MENU_WIDTH) / 2, 150, 1.0, 0, resultTitleGh, true, false);
		DrawGraph((WIDTH - MENU_WIDTH) / 2 -240, 250, resultLogo1Gh, true);
		DrawGraph((WIDTH - MENU_WIDTH) / 2 - 300, 450, resultLogo2Gh, true);
		// �X�R�A�̕\��
		int tempScore = score->GetScore(CURRENT_SCORE);
		char buf[20];
		int num;
		num = sprintf(buf, "%d", tempScore);
		for (int i = 0; i < num; i++) {
			DrawRotaGraph((WIDTH - MENU_WIDTH) / 2 + 100 + i * 25, 287, 1.5, 0, numGh[(buf[i] - '0')], true, false);
		}
		// �����N�̕\��
		int tempRank;
		// - �`���v�^�[�ԍ��ɉ����ĕω� - //
		// Ch. 0
		if (chapterNum == 0) {
			if (tempScore >= 5000) {
				tempRank = 0;
			}
			else if (5000 > tempScore && tempScore >= 4500) {
				tempRank = 1;
			}
			else if (4500 > tempScore && tempScore >= 3500) {
				tempRank = 2;
			}
			else if (3500 > tempScore && tempScore >= 2500) {
				tempRank = 3;
			}
			else {
				tempRank = 4;
			}

		}
		// Ch. 1
		else if (chapterNum == 1) {

			if (tempScore >= 20000) {
				tempRank = 0;
			}
			else if (20000 > tempScore && tempScore >= 18000) {
				tempRank = 1;
			}
			else if (18000 > tempScore && tempScore >= 15000) {
				tempRank = 2;
			}
			else if (15000 > tempScore && tempScore >= 10000) {
				tempRank = 3;
			}
			else {
				tempRank = 4;
			}
		}
		else {
			if (tempScore >= 20000) {
				tempRank = 0;
			}
			else if (20000 > tempScore && tempScore >= 18000) {
				tempRank = 1;
			}
			else if (18000 > tempScore && tempScore >= 15000) {
				tempRank = 2;
			}
			else if (15000 > tempScore && tempScore >= 10000) {
				tempRank = 3;
			}
			else {
				tempRank = 4;
			}
		}

		DrawRotaGraph((WIDTH - MENU_WIDTH) / 2 + 100, 378, 1.0, 0, rankGh[tempRank], true, false);

		return SCENE_CH_SELECT;

	}
	else {
		return SCENE_NON_CHANGE;
	}
}

bool SceneManagerCh1::BossExistFlag() {
	// --- �{�X����ʏ�ɑ��݂��邩�ǂ����𔻒� ---
	for (int i = 0; i < enemySpawnCount; i++) {
		if (enemy[i] != NULL) {
			bool temp = enemy[i]->GetBossExist();
			if (temp) {
				return true;
			}
		}
	}
	return false;
}

bool SceneManagerCh1::BossDefeatFlag() {
	// --- �{�X�����j�������Atrue��Ԃ� ---
	for (int i = 0; i < enemySpawnCount; i++) {
		if (enemy[i] != NULL) {
			bool temp = enemy[i]->GetBossDead();
			if (temp) {
				return true;
			}
		}
	}
	return false;
}

void SceneManagerCh1::SetVolume(int bgmVolume, int seVolume) {
	// --- BGM&SE�̉��ʂ��Z�b�g ---
	this->bgmVolume = bgmVolume;
	this->seVolume = seVolume;
	// -- �e�n���h���ɉ��ʂ�K�������� --
	int tempBGMVol, tempSEVol;
	tempBGMVol = (int)255 * (float)bgmVolume / 100.0;
	tempSEVol = (int)255 * (float)seVolume / 100.0;
	// - BGM -
	ChangeVolumeSoundMem(tempBGMVol, hb_stageSound);
	ChangeVolumeSoundMem(tempBGMVol, hb_bossSound);
	// - SE -
	ChangeVolumeSoundMem(tempSEVol, hs_playerRedShot01);
	ChangeVolumeSoundMem(tempSEVol, hs_playerBlueShot01);
	ChangeVolumeSoundMem(tempSEVol, hs_playerBlueShot01);
	ChangeVolumeSoundMem(tempSEVol, hs_playerYellowShot01);
	ChangeVolumeSoundMem(tempSEVol, hs_playerDead);
	ChangeVolumeSoundMem(tempSEVol, hs_playerTransform);
	ChangeVolumeSoundMem(tempSEVol, hs_playerDamaged);
	ChangeVolumeSoundMem(tempSEVol, hs_playerBombShot);
	ChangeVolumeSoundMem(tempSEVol, hs_playerBombExplode);
	ChangeVolumeSoundMem(tempSEVol, hs_playerRefrect);
	ChangeVolumeSoundMem(tempSEVol, hs_playerFlying);
	ChangeVolumeSoundMem(tempSEVol, hs_playerShiftedFlying);
	ChangeVolumeSoundMem(tempSEVol, hs_enemyDead);
	ChangeVolumeSoundMem(tempSEVol, hs_enemyDamaged);
	ChangeVolumeSoundMem(tempSEVol, hs_itemGot);
	ChangeVolumeSoundMem(tempSEVol, hs_enemySho01);
	ChangeVolumeSoundMem(tempSEVol, hs_enemyReflect);
	// ���j���[���
	ChangeVolumeSoundMem(tempSEVol, hs_choiceSE);
	ChangeVolumeSoundMem(tempSEVol, hs_decisionSE);

}