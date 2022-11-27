#include "Player.h"

Player::Player() {
	// --- コンストラクタ ---
	// プレイヤー
	x = 500;
	y = 650;
	life = true;
	machineType = MACHINE_RED;	// 初期マシン：Red
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
		// グラフィックハンドラ：draw()で初期化
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
	tr_MaxTime = 0.3;			// 変形時間上限
	tr_countTime = 0;
	tr_animFlag = false;	
	tr_seFlag = false;
	// 画面処理パラメータ 
	drawInit = false;

	// -- 機体のパラメータ格納 -- 
	// Red機体
	machine[0].gh = 0;			// Draw()で後に初期化 (DxLib_Init()後実施のため)
	machine[0].width = 0;		// Draw()で後に初期化
	machine[0].height = 0;		// Draw()で後に初期化
	machine[0].element = RED;
	machine[0].hp = 0;			// プレイヤーはマシン構造体のhpを参照しない
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
	machine[0].score = 0;		// (敵機用 Playerでは使用しない)
	// Blue機体
	machine[1].gh = 0;			// Draw()で後に初期化
	machine[1].width = 0;		// Draw()で後に初期化
	machine[1].height = 0;		// Draw()で後に初期化
	machine[1].element = BLUE;
	machine[1].hp = 0;			// プレイヤーはマシン構造体のhpを参照しない
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
	machine[1].score = 0;		// (敵機用 Playerでは使用しない)
	// Yellow機体
	machine[2].gh = 0;			// Draw()で後に初期化
	machine[2].width = 0;		// Draw()で後に初期化
	machine[2].height = 0;		// Draw()で後に初期化
	machine[2].element = YELLOW;
	machine[2].mass = 0.3;
	machine[2].hp = 0;			// プレイヤーはマシン構造体のhpを参照しない
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
	machine[2].score = 0;		// (敵機用 Playerでは使用しない)
	// 速度制御パラメータ
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

	// -- 装備系のパラメータ --
	// - 弾のパラメータ初期化 -
	for (int i = 0; i < MACHINE_NUM; i++){
		for (int k = 0; k < PSHOT_NUM; k++) {

			shot[i][k].gh[SHOT_FRAME_MAX] = { 0 };			// Draw()で後に初期化
			shot[i][k].flag = false;
			shot[i][k].x = 0;
			shot[i][k].y = 0;
			shot[i][k].width = 0;		// Draw()で後に初期化
			shot[i][k].heigh = 0;		// Draw()で後に初期化
			shot[i][k].rad = 0;
			shot[i][k].speed = 0;
			shot[i][k].time = 0;
			shot[i][k].animFrame = 0;

			// 弾の種類に応じて分岐
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
	// 時間 
	totalTime = 0;
	reloadingTime = 0;
	damageInvalidTime = 0;
	damageAnimCounter = 0;
	respawnTime = 0;
	// フラグ
	damageFlag = false;
	damageInvalidFlag = false;
	deadFlag = false;
	respawnFlag = false;
	redShotFlag = false;		// 効果音再生フラグ
	blueShotFlag = false;
	yellowShotFlag = false;
	deadEffectFlag = false;
	deadEffectInitFlag = false;
	drawBomb = false;
	fs_damage = false;
	fs_refrect = false;
	hs_shifted = false;
	penetrationFlag = false;
	// ダメージ計算用 -
	damagedElement = 0;		// 被弾時の属性
	damagedNum = 0;			// 被弾ダメージ
	// 消滅エフェクトクラス
	//  -> All関数内でインスタンス化する. 暫定的にnullpointerを与える
	deadeffect = nullptr;

}

Player::~Player() {
	// --- デストラクタ ---
	if (deadeffect != NULL) {
		delete deadeffect;
	}
}

void Player::All(float delta_time) {
	// --- プレイヤーの操作を外部から呼び出し ---
	// -- nan エラー回避: ポーズ画面時 --
	if (!delta_time == 0) {
		// 照準の位置の取得
		GetMousePoint(&aimX, &aimY);
		// - 速度制御 - 
		Move(delta_time);
		// - 射撃 -
		Shot(delta_time);
	}
	// - 死亡時のフラグ
	if (deadFlag) {
		
		// 死亡エフェクトフラグ有効化
		deadEffectInitFlag = true;
		deadEffectFlag = true;

		deadFlag = false;

	} 
	
	// - 死亡エフェクト実行
	if (deadEffectFlag) {
		if (deadEffectInitFlag) {
			deadeffect = new DeadEffect(1, deadPosX, deadPosY, velX, velY, 1.0, 10.0, 2.5);
			deadEffectInitFlag = false;
		}
		if (deadeffect->All(delta_time)) {
			deadEffectFlag = false;
		}
	}
	// - ダメージ計算 - 
	DamageCalc();
	// - アイテムによるステータス変化があれば、機体に反映 -
	if (GetStateChange()) {
		StateUp();
	}

	// - 描画 -
	Draw();
	// - 時間＆フラグ管理 -
	TimeAndFlag(delta_time);

}


void Player::Move(float delta_time) {
	// --- プレイヤーの動作 ---
	// フラグリセット
	tr_seFlag = false;

	// *** 生きてる時の処理 ****
	if (life) {
		// -- マシンの選択(1, 2, 3 key) -- 
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
		// -- マシンの選択(e key)
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_E)) {
			machineType++;
			if (machineType > MACHINE_YELLOW) {
				machineType = MACHINE_RED;
			}
			tr_seFlag = true;
		}

		// -- マシンの選択(c key)
		if (tnl::Input::IsKeyDownTrigger(eKeys::KB_C)) {
			machineType--;
			if (machineType < MACHINE_RED) {
				machineType = MACHINE_YELLOW;
			}
			tr_seFlag = true;
		}

		// -- 移動制限 --
		// 水平方向
		if (x > (WIDTH - MENU_WIDTH)) {
			x = WIDTH - MENU_WIDTH;
		}
		else if (x < MARGINE) {
			x = MARGINE;
		}
		// 垂直方向
		if (y > HEIGHT - MARGINE) {
			y = HEIGHT - MARGINE;
		}
		else if (y < MARGINE) {
			y = MARGINE;
		}

		// -- プレイヤーの速度制御 --
		hs_shifted = false;		// シフトで高速飛行中かどうかのフラグ
		// - Shift入力時は速度＆加速度上限をアップさせる
		if (tnl::Input::IsKeyDown(eKeys::KB_LSHIFT)) {
			hs_shifted = true;		// シフトで高速飛行中かどうかのフラグ
			velMax = machine[machineType].refVelMaxShifted;
			accMax = machine[machineType].refAccMaxShifted;
		}
		else {
			velMax = machine[machineType].refVelMaxNormal;
			accMax = machine[machineType].refAccMaxNormal;
		}

		// - 速度目標値をWASD or 方向キーから得る - 
		// - 垂直動作 -
		if ((tnl::Input::IsKeyDown(eKeys::KB_W) && tnl::Input::IsKeyDown(eKeys::KB_S)) ||
			(tnl::Input::IsKeyDown(eKeys::KB_UP) && tnl::Input::IsKeyDown(eKeys::KB_DOWN))) {
			// 同時押しを防止
			refVelY = 0;

		}
		else if (tnl::Input::IsKeyDown(eKeys::KB_W) || tnl::Input::IsKeyDown(eKeys::KB_UP)) {
			refVelY = -1 * velMax;
		}
		else if (tnl::Input::IsKeyDown(eKeys::KB_S) || tnl::Input::IsKeyDown(eKeys::KB_DOWN)) {
			refVelY = velMax;
		}
		else {
			// 無操作時の処理
			refVelY = 0;
		}

		// - 水平動作 -
		if ((tnl::Input::IsKeyDown(eKeys::KB_D) && tnl::Input::IsKeyDown(eKeys::KB_A)) ||
			(tnl::Input::IsKeyDown(eKeys::KB_RIGHT) && tnl::Input::IsKeyDown(eKeys::KB_LEFT))) {
			// 同時押しを防止
			refVelX = 0;
		}
		else if (tnl::Input::IsKeyDown(eKeys::KB_D) || tnl::Input::IsKeyDown(eKeys::KB_RIGHT)) {
			refVelX = velMax;
		}
		else if (tnl::Input::IsKeyDown(eKeys::KB_A) || tnl::Input::IsKeyDown(eKeys::KB_LEFT)) {
			refVelX = -1 * velMax;
		}
		else {
			// 無操作時の処理
			refVelX = 0;
		}


		if (delta_time == 0) {

		}
		// - 速度追従させる(制御器)：Ux(F(N)) - 
		float errorX = (refVelX - velX);	// 速度誤差計算
		float errorY = (refVelY - velY);
		// 制御入力生成
		integralX += (errorX + preErrorX) / 2 * delta_time;
		float ux = machine[machineType].kp * errorX +	// 制御入力計算X
			machine[machineType].ki * integralX +
			machine[machineType].kd * (errorX - preErrorX) / delta_time;
		integralY += (errorY + preErrorY) / 2 * delta_time;
		float uy = machine[machineType].kp * errorY +	// 制御入力計算Y
			machine[machineType].ki * integralY +
			machine[machineType].kd * (errorY - preErrorY) / delta_time;

		// - 操作対象へ指令値を反映 - 
		// 加速度
		accX += (ux / machine[machineType].mass) * delta_time;	// A = F/Mを作用
		accY += (uy / machine[machineType].mass) * delta_time;
		// 加速度の大きさを制限
		float accNorm = hypot(accX, accY);
		if (accNorm >= accMax) {
			accX = (accX / accNorm) * accMax;
			accY = (accY / accNorm) * accMax;
		}

		// 速度
		velX += accX;
		velY += accY;
		// 速度の大きさを制限
		float velNorm = hypot(velX, velY);
		if (velNorm >= velMax) {
			velX = (velX / velNorm) * velMax;
			velY = (velY / velNorm) * velMax;
		}

		// 位置
		x += velX;
		y += velY;

		// - 制御情報を格納 -
		preErrorX = errorX;
		preErrorY = errorY;

		// -- ブーストチャージの実装 --
		

		// -- プレイヤーの向き制御 --
		// マウスの向きに応じてプレイヤーの角度を更新する.
		int mouseX, mouseY;
		GetMousePoint(&mouseX, &mouseY);
		rad = atan2((double)mouseY - y, (double)mouseX - x);

		// -- 変形モーションの作成 --
		// - 選択中のマシンと以前選択していたマシンの比較＆フラグ有効化
		if (machineType != pre_machineType) {
			pre_machineType = machineType;
			tr_animFlag = true;		//	フラグ無効化はTimeAndFlag()で実施
		}


	} else {


	// *** 死亡している時の処理 ****
	// -- リスポーン位置に移動する
	accX = 0;
	accY = 0;
	velX = 0;
	velY = 0;
	x = (WIDTH - MENU_WIDTH) / 2;
	y = HEIGHT + MARGINE + 10;
	rad = 0;
	// -- 残機があれば、リスポーンフラグを立てる
	if (lifeNum > 0) {
		respawnFlag = true;
	}
	// リスポーンフラグの確認
	else if (respawnFlag) {
		respawnFlag = false;
	}
	// - リスポーン目標位置まで移動
	double length = (HEIGHT + MARGINE + 10) - PLAYER_RESPAWN_POS_Y;
	double temp = HEIGHT - length * sin((tnl::PI / 2) * (respawnTime / PLAYER_RESPAWN_TIME));
	x = (WIDTH - MENU_WIDTH) / 2;
	y = temp;

	}
	
}

void Player::Draw() {
	// --- 描画処理 --- 
	
	// -- 初期化処理 -- 
	if (!drawInit) {

		// - 機体の画像初期化 - 
		machine[0].gh = LoadGraph("graphics/RedPlaneFront00.png");		// Red機体
		machine[1].gh = LoadGraph("graphics/BluePlaneFront00.png");		// Blue機体
		machine[2].gh = LoadGraph("graphics/YellowPlaneFront00.png");	// Yellow機体
		for (int i = 0; i < MACHINE_NUM; i++) {
			// 画像サイズを構造体に格納
			GetGraphSize(machine[i].gh, &machine[i].width, &machine[i].height);
		}
		// - 照準の処理
		aimGh = LoadGraph("graphics/Aim.png");
		// - ボムの処理 - 
		// bombアイテム画像を流用
		LoadDivGraph("graphics/Item/BombItem_3_861_284.png", 3, 3, 1, 861 / 3, 284, bomb.gh);
		LoadDivGraph("graphics/Effect/DeadEffect001_10_560_48.png", 10, 10, 1, 568 / 10, 48, bomb.effectGh);

		// - 変形モーション画像読み込み
		LoadDivGraph("graphics/QuantumCore_12_120_10.png", 12, 12, 1, 120 / 12, 10, transformGh, true, false);
		// - 弾の処理 - 
		int tempRed[RED_BULLET_FRAME_MAX];
		int tempBlue[BLUE_BULLET_FRAME_MAX];
		int tempYellow[YELLOW_BULLET_FRAME_MAX];

		LoadDivGraph("graphics/Red/RedBullet002_3_48_16.png", 3, 3, 1, 48 / 3, 16, tempRed);
		LoadDivGraph("graphics/Blue/BlueBullet002_6_60_21.png", 6, 6, 1, 60 / 6, 21, tempBlue);
		LoadDivGraph("graphics/Yellow/YellowBullet001_4_64_26.png", 4, 4, 1, 64 / 4, 26, tempYellow);

		int w, h;
		for (int i = 0; i < MACHINE_NUM; i++) {
			// 機体の種類に応じて処理分岐
			switch (i)
			{
			case RED:
				// 画像のサイズを取得
				GetGraphSize(tempRed[0], &w, &h);
				// グラフィックハンドル＆画像サイズを弾構造体へ渡す
				for (int k = 0; k < PSHOT_NUM; k++) {
					for (int n = 0; n < RED_BULLET_FRAME_MAX; n++) {
						shot[i][k].gh[n] = tempRed[n];
					}
					shot[i][k].width = w;
					shot[i][k].heigh = h;
				}
				break;

			case BLUE:
				// 画像のサイズを取得
				GetGraphSize(tempBlue[0], &w, &h);
				// グラフィックハンドル＆画像サイズを弾構造体へ渡す
				for (int k = 0; k < PSHOT_NUM; k++) {
					for (int n = 0; n < BLUE_BULLET_FRAME_MAX; n++) {
						shot[i][k].gh[n] = tempBlue[n];
					}
					shot[i][k].width = w;
					shot[i][k].heigh = h;
				}
				break;

			case YELLOW:
				// 画像のサイズを取得
				GetGraphSize(tempYellow[0], &w, &h);
				// グラフィックハンドル＆画像サイズを弾構造体へ渡す
				for (int k = 0; k < PSHOT_NUM; k++) {
					for (int n = 0; n < YELLOW_BULLET_FRAME_MAX; n++) {
						shot[i][k].gh[n] = tempYellow[n];
					}
					shot[i][k].width = w;
					shot[i][k].heigh = h;
				}
				break;

			default:
				OutputDebugString("エラー：自機弾の画像初期化処理、マシン数を確認して下さい");
				break;
			}
		}

		drawInit = true;
	}

	// -- 描画処理 -- 

	// - 弾丸の描画 - 
	for (int i = 0; i < MACHINE_NUM; i++) {
		for (int k = 0; k < PSHOT_NUM; k++) {
			if (shot[i][k].flag) {
				switch (i)
				{
				case RED:
					// 弾アニメーション再生
					if (shot[i][k].time >= RED_BULLET_ANIM_SPAN) {
						shot[i][k].time = 0;
						shot[i][k].animFrame++;
						shot[i][k].animFrame %= RED_BULLET_FRAME_MAX;
					}
					DrawRotaGraph(shot[i][k].x,	shot[i][k].y, 1.3, shot[i][k].rad + tnl::PI / 2, 
									shot[i][k].gh[shot[i][k].animFrame], true, false);
					break;

				case BLUE:
					// 弾アニメーション再生
					if (shot[i][k].time >= BLUE_BULLET_ANIM_SPAN) {
						shot[i][k].time = 0;
						shot[i][k].animFrame++;
						shot[i][k].animFrame %= BLUE_BULLET_FRAME_MAX;
					}
					DrawRotaGraph(shot[i][k].x, shot[i][k].y, 1.8, shot[i][k].rad + tnl::PI / 2,
						shot[i][k].gh[shot[i][k].animFrame], true, false);

					break;

				case YELLOW:
					// 弾アニメーション再生
					if (shot[i][k].time >= YELLOW_BULLET_ANIM_SPAN) {
						shot[i][k].time = 0;
						shot[i][k].animFrame++;
						shot[i][k].animFrame %= YELLOW_BULLET_FRAME_MAX;
					}
					DrawRotaGraph(shot[i][k].x, shot[i][k].y, 1.3, shot[i][k].rad + tnl::PI / 2,
						shot[i][k].gh[shot[i][k].animFrame], true, false);

					break;

				default:
					OutputDebugString("エラー：自機弾のアニメーション処理");
					break;
				}

			}
		}
	}

	// - ボムの描画 -
	if (bomb.bombFlag) {
		// ボム移動時の処理
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
		// ボム爆破後の処理
		if (bomb.bomb_explodeAnimTime >= 0.1) {
			bomb.bomb_explodeAnimTime = 0;
			bomb.bomb_explodeFlame++;
			if (bomb.bomb_explodeFlame >= 9) {
				// ボム爆破アニメーションはループしない
				bomb.bomb_explodeFlame = 9;
				drawBomb = false;
			}

			DrawRotaGraph(bomb.x, bomb.y, 7.0, bomb.rad + tnl::PI / 2, bomb.effectGh[bomb.bomb_explodeFlame], true, false);
		}

	}


	// - プレイヤーの描画 -
	if (life) {
		// * プレイヤーが生きているとき * 
		
		// 変形アニメ画像ハンドラ変更
		if (tr_animTime >= 0.05) {
			tr_animTime = 0;
			tr_animFlame++;
			tr_animFlame %= 12;
		}

		if (!damageInvalidFlag) {
			// 無敵時間で無い時のプレイヤー描画
			if (!tr_animFlag) {
				DrawRotaGraph(x, y, 1.0, rad + tnl::PI / 2, machine[machineType].gh, true, false);
			}
			else {
				// 変形アニメーション再生
				DrawRotaGraph(x, y, 4.0, rad + tnl::PI / 2, transformGh[tr_animFlame], true, false);
			}
		}
		else {
			// 無敵時間中のプレイヤーの描画
			// 無敵時間の経過時間割合を受け取る
			int temp = (int)(100 * ((float)DAMAGE_INVALID_TIME_MAX - damageInvalidTime) / (float)DAMAGE_INVALID_TIME_MAX);
			if (temp % 10 < 5){
				if (!tr_animFlag) {
					// プレイヤーを薄く表示 
					SetDrawBlendMode(DX_BLENDMODE_ALPHA, 140);
					DrawRotaGraph(x, y, 1.0, rad + tnl::PI / 2, machine[machineType].gh, true, false);
					SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
				}
				else {
					// 変形状態のプレイヤーを薄く表示
					SetDrawBlendMode(DX_BLENDMODE_ALPHA, 140);
					DrawRotaGraph(x, y, 4.0, rad + tnl::PI / 2, transformGh[tr_animFlame], true, false);
					SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
				}
			}
			else {
				if (!tr_animFlag) {
					// プレイヤーを更に薄く表示
					SetDrawBlendMode(DX_BLENDMODE_ALPHA, 80);
					DrawRotaGraph(x, y, 1.0, rad + tnl::PI / 2, machine[machineType].gh, true, false);
					SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
				}
				else {
					// 変形状態のプレイヤーを更に薄く表示
					SetDrawBlendMode(DX_BLENDMODE_ALPHA, 80);
					DrawRotaGraph(x, y, 4.0, rad + tnl::PI / 2, transformGh[tr_animFlame], true, false);
					SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

				}
			}		
		}
		// 照準の描画
		DrawRotaGraph(aimX, aimY, 0.8, rad + tnl::PI / 2, aimGh, true, true);

	} else if (respawnFlag) {
		// * リスポーン中 *
		if ((int)(totalTime) % 2 == 0) {
			// 偶数秒の時
			// プレイヤーを薄く表示 
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 140);
			DrawRotaGraph(x, y, 1.0, rad, machine[machineType].gh, true, false);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
		else {
			// 奇数秒の時
			// プレイヤーを更に薄く表示
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 80);
			DrawRotaGraph(x, y, 1.0, rad, machine[machineType].gh, true, false);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}

	}

}

void Player::Shot(float delta_time) {
	// --- 弾丸の発射、軌道の計算 ---

	// 一時変数
	int num = 0;
	// -- 弾丸の発射(マシン毎に違う弾を使用) -- 
	// - 生きている時だけ射撃 -
	if (life) {
		// - 被ダメージ中(つまり無敵時間中)は発射しない
		if (!damageInvalidFlag) {
			// ** 通常弾の処理 * //
			if (tnl::Input::IsMouseDown(eMouse::LEFT) &&
				reloadingTime >= machine[machineType].reloadTime) {

				if (machineType == RED) {
					// * 強化ステータスに応じて発射する弾の変更を加える * //
					for (int i = 0; i < PSHOT_NUM; i++) {
						if (shot[machineType][i].flag == false) {

							// 1. additonalPower < 4の時のショット
							if (additionalPower < 3) {
								// 貫通弾無効
								penetrationFlag = false;
								shot[machineType][i].flag = true;
								shot[machineType][i].x = x;
								shot[machineType][i].y = y;
								shot[machineType][i].rad = rad;
								shot[machineType][i].speed = machine[machineType].bulletSpeed;
								shot[machineType][i].time = 0;

								break;
							}
							// 2. additionalPower >= 3の時のショット
							else if (additionalPower >= 3 && additionalPower < 6) {
								// 貫通弾無効
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
							// 3. additonalPower >= 6の時のショット
							else if (additionalPower >= 6) {
								// 貫通弾無効
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
							// 1. additonalPower < 4の時のショット
							if (additionalPower < 3) {
								// 貫通弾無効
								penetrationFlag = false;
								shot[machineType][i].flag = true;
								shot[machineType][i].x = x;
								shot[machineType][i].y = y;
								shot[machineType][i].rad = rad;
								shot[machineType][i].speed = machine[machineType].bulletSpeed;
								shot[machineType][i].time = 0;

								break;
							}
							// 2. additonalPower >= 3の時のショット
							if (additionalPower >= 3 && additionalPower < 6) {
								// 貫通弾有効化
								penetrationFlag = true;
								shot[machineType][i].flag = true;
								shot[machineType][i].x = x;
								shot[machineType][i].y = y;
								shot[machineType][i].rad = rad;
								shot[machineType][i].speed = machine[machineType].bulletSpeed;
								shot[machineType][i].time = 0;

								break;
							}
							// 3. additonalPower >= 6の時のショット
							if (additionalPower >= 6) {
								// 貫通弾有効化
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

							// 1. additonalPower < 4の時のショット
							if (additionalPower < 3) {
								// 貫通弾無効
								penetrationFlag = false;
								shot[machineType][i].flag = true;
								shot[machineType][i].x = x;
								shot[machineType][i].y = y;
								shot[machineType][i].rad = rad;
								shot[machineType][i].speed = machine[machineType].bulletSpeed;
								shot[machineType][i].time = 0;

								break;
							}

							// 2. additonalPower >= 3の時のショット
							// 30°内の角度で4発の散弾発射、一発は正面に発射する
							if (additionalPower >= 3 && additionalPower < 6) {
								// 貫通弾無効
								penetrationFlag = false;
								// 乱数取得
								if (num == 0) {
									srand((unsigned int)time(0));
									// 一発は正面に発射
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

							// 3. additonalPower >= 6の時のショット
							if (additionalPower >= 6) {
								// 貫通弾無効
								penetrationFlag = false;
								// 乱数取得
								if (num == 0) {
									srand((unsigned int)time(0));
									// 一発は正面に発射
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
						
				// - リロードタイムのリセット -
				reloadingTime = 0;

				// - ショット効果音再生フラグを立てる -
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
			// * ボムの処理 * //
			// ボム爆破フラグを折る
			bomb.explodeFlag = false;
			// ボムショット音フラグを折る
			fs_bombShot = false;
			// ボムは画面に一つだけ表示させる
			if (tnl::Input::IsMouseDown(eMouse::RIGHT) && bombNum > 0 && bomb.bombFlag == false && bomb.explodeFlag == false && drawBomb == false) {
				// ボムショット音フラグを立てる
				fs_bombShot = true;
				// 初期化処理
				bomb.bomb_animTime = 0;
				bomb.bomb_animFlame = 0;
				bomb.bomb_explodeAnimTime = 0;
				bomb.bomb_explodeFlame = 0;
				
				bombNum--;
				if (bombNum < 0) {
					bombNum = 0;
				}
				// 標的までの角度を取得
				bomb.rad = rad;
				// プレイヤーの位置を格納
				bomb.x = x;
				bomb.y = y;
				// マウスカーソル位置を目標位置として格納
				int mouseX, mouseY;
				GetMousePoint(&mouseX, &mouseY);
				bomb.targetX = mouseX;
				bomb.targetY = mouseY;
				// プレイヤーの向きに応じてボム初速成分計算
				bomb.velX = bomb.velMagunitude * delta_time * cos(bomb.rad);
				bomb.velY = bomb.velMagunitude * delta_time * sin(bomb.rad);
				// 発射フラグ有効化
				bomb.bombFlag = true;

			}
		}
	}

	// -- フラグの立った全ての弾丸の位置更新 -- 
	// - マシン全機の発射弾丸を処理 - 
	for (int i = 0; i < MACHINE_NUM; i++) {
		for (int k = 0; k < PSHOT_NUM; k++) {
			
			if(shot[i][k].flag) {

				shot[i][k].x += shot[i][k].speed * delta_time * cos(shot[i][k].rad);
				shot[i][k].y += shot[i][k].speed * delta_time * sin(shot[i][k].rad);
				shot[i][k].time += delta_time;
			}

			// 画面外に弾がはみ出したら、フラグを戻す
			if((shot[i][k].x > WIDTH - MENU_WIDTH) ||
				(shot[i][k].x < MARGINE) ||
				(shot[i][k].y > HEIGHT - MARGINE) ||
				(shot[i][k].y < MARGINE)) {

				shot[i][k].flag = false;
			}

		}
	}

	// - ボム発射後の処理実施 - 
	// ボム爆破音フラグを折る
	fs_bombExplode = false;
	if (bomb.bombFlag) {
		// ボムが爆破するまで移動続ける
		bomb.x += bomb.velX;
		bomb.y += bomb.velY;
		bomb.bomb_animTime += delta_time;	// アニメーションカウンタ更新

		// - 画面外に弾がはみ出す or 目標位置に到達した場合、爆破フラグを有効化 -
		if ((bomb.x > WIDTH - MENU_WIDTH) ||
			(bomb.x < MARGINE) ||
			(bomb.y > HEIGHT - MARGINE) ||
			(bomb.y < MARGINE)) {
			bomb.bombFlag = false;
			bomb.explodeFlag = true;
			// ボム爆破音フラグを立てる
			fs_bombExplode = true;

		}
		else if (((double)bomb.targetX > bomb.x - 2.0 && (double)bomb.targetX < bomb.x + 2.0) ||
			((double)bomb.targetY > bomb.y - 2.0 && (double)bomb.targetY < bomb.y + 2.0)) {
			bomb.bombFlag = false;
			bomb.explodeFlag = true;
			// ボム爆破音フラグを立てる
			fs_bombExplode = true;
		}

	}

	if (drawBomb) {
		// 爆発後移動停止、アニメーションカウンタは更新
		bomb.bomb_explodeAnimTime += delta_time;
	}
	
	
}

void Player::TimeAndFlag(float delta_time) {
	// --- 時間＆フラグの管理 --- 
	totalTime += delta_time;		// 経過時間を更新
	reloadingTime += delta_time;	// 弾のリロード時間更新
	tr_animTime += delta_time;		// 変形アニメーション時間更新
	if (tr_animFlag) {
		// 変形が有効の際、変形時間更新
		tr_countTime += delta_time;
		if (tr_countTime >= tr_MaxTime) {
			tr_countTime = 0;
			tr_animFlag = false;
		}
	}

	// -- 無敵時間の管理 --
	if (damageInvalidFlag) {
		// - 一定時間無敵 -
		damageInvalidTime += delta_time;
		if (damageInvalidTime >= DAMAGE_INVALID_TIME_MAX) {
			// 無敵フラグ＆時間リセット
			damageInvalidFlag = false;
			damageInvalidTime = 0;
		}
	}

	// -- リスポーン時間の管理 --
	if (respawnFlag) {
		respawnTime += delta_time;
		if (respawnTime >= PLAYER_RESPAWN_TIME) {
			// - リスポーン -
			respawnTime = 0;
			respawnFlag = false;
			life = true;
			hp = PLAYER_INITIAL_HP;
			// - リスポーン後、即死防止のため無敵時間セット - 
			damageInvalidFlag = true;
		}
	}

}

void Player::DamageCalc() {
	// --- 被弾ダメージを計算 ---
	// ダメージの受け取りはSetDamage()で実施
	// ダメージ効果音フラグを折る
	fs_damage = false;
	fs_refrect = false;
	// ダメージを受けた属性に応じてダメージ変化
	int tempElement = machine[machineType].element;
	int calcType;

	if (life) {
		if (damageFlag) {
			// - じゃんけんアルゴリズムを参考 - 

			int temp = (tempElement - damagedElement + 3) % 3;
			if (temp == 0) {
				// あいこ
				calcType = 0;
			}
			else if (temp == 1) {
				// 勝ち(効果はいまいち)
				calcType = 1;
			}
			else {
				// 負け(弱点を突かれた)
				calcType = 2;
			}

			// - ジャンケンの結果に応じてダメージ計算 - 
			switch (calcType)
			{
			case 0:
				// あいこ
				if (machine[machineType].defence - damagedNum <= 0) {
					hp += (int)machine[machineType].defence - damagedNum;
					damageInvalidFlag = true;
					fs_damage = true;
				} else {
					// ノーダメージ
					fs_refrect = true;
				}
				break;

			case 1:
				// かち(ダメージ1/2)
				if (machine[machineType].defence - damagedNum / 2 <= 0) {
					hp += (int)machine[machineType].defence - damagedNum / 2;
					damageInvalidFlag = true;
					fs_damage = true;
				} else {
					// ノーダメージ
					fs_refrect = true;
				}
				break;
			case 2:
				// 負け(ダメージ2倍)
				if (machine[machineType].defence - damagedNum * 2 <= 0) {
					hp += (int)machine[machineType].defence - damagedNum * 2;
					damageInvalidFlag = true;
					fs_damage = true;
				} else {
					// ノーダメージ
					fs_refrect = true;
				}
				break;
			}


			// - hpが0以下ならば、ライフを減らす -
			if (hp <= 0) {
				hp = 0;
				life = false;
				deadFlag = true;
				deadPosX = x;
				deadPosY = y;
				lifeNum--;
				// ステータスのリセット
				additionalPower = 0;
				additionalDefense = 0;
				additionalSpeed = 0;
				if (lifeNum <= 0) {
					lifeNum = 0;
				}

			}

		}
	}
	// - ダメージフラグを戻す - 
	damageFlag = false;

}

void Player::StateUp() {
	// --- アイテム取得効果による恩恵を機体に与える ---
	
	// -- 機体 ＆ 弾のパラメータ変更 -
	for (int i = 0; i < MACHINE_NUM; i++) {
		if (i == RED) {
			// 防御力UP
			machine[i].defence = RED_INITIAL_DEFENSE + additionalDefense;
			// スピードアップ
			machine[i].refAccMaxNormal = RED_REF_ACC_MAX_N + (float)additionalSpeed / 10.0;
			machine[i].refAccMaxShifted = RED_REF_ACC_MAX_S + (float)additionalSpeed / 10.0;
			machine[i].refAccMaxNormal = RED_REF_VEL_MAX_N + (float)additionalSpeed / 10.0;
			machine[i].refAccMaxShifted = RED_REF_VEL_MAX_S + (float)additionalSpeed / 10.0;
			// リロード速度UP
			machine[i].reloadTime = RED_RELOAD_TIME - (float)additionalSpeed / 50;
			// 弾速UP
			machine[i].bulletSpeed = RED_BULLET_SPEED + (float)additionalSpeed * 20;

		}
		else if (i == BLUE) {
			// 防御力UP
			machine[i].defence = BLUE_INITIAL_DEFENSE + additionalDefense;
			// スピードアップ
			machine[i].refAccMaxNormal = BLUE_REF_ACC_MAX_N + (float)additionalSpeed / 5.0;
			machine[i].refAccMaxShifted = BLUE_REF_ACC_MAX_S + (float)additionalSpeed / 5.0;
			machine[i].refAccMaxNormal = BLUE_REF_VEL_MAX_N + (float)additionalSpeed / 5.0;
			machine[i].refAccMaxShifted = BLUE_REF_VEL_MAX_S + (float)additionalSpeed / 5.0;
			// リロード速度UP
			machine[i].reloadTime = BLUE_RELOAD_TIME - (float)additionalSpeed / 50;
			// 弾速UP
			machine[i].bulletSpeed = BLUE_BULLET_SPEED + (float)additionalSpeed * 20;
		}
		else if (i == YELLOW) {
			// 防御力UP
			machine[i].defence = YELLOW_INITIAL_DEFENSE + additionalDefense;
			// スピードアップ
			machine[i].refAccMaxNormal = YELLOW_REF_ACC_MAX_N + (float)additionalSpeed / 5.0;
			machine[i].refAccMaxShifted = YELLOW_REF_ACC_MAX_S + (float)additionalSpeed / 5.0;
			machine[i].refAccMaxNormal = YELLOW_REF_VEL_MAX_N + (float)additionalSpeed / 5.0;
			machine[i].refAccMaxShifted = YELLOW_REF_VEL_MAX_S + (float)additionalSpeed / 5.0;
			// リロード速度UP
			machine[i].reloadTime = YELLOW_RELOAD_TIME - (float)additionalSpeed / 50;
			// 弾速UP
			machine[i].bulletSpeed = YELLOW_BULLET_SPEED + (float)additionalSpeed * 20;
		}


		for (int k = 0; k < PSHOT_NUM; k++) {
			if (i == RED) {
				// 攻撃力UP
				shot[i][k].damage = RED_INITIAL_BULLET_ATK + additionalPower;
			}
			else if (i == BLUE) {
				// 攻撃力UP
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
	// --- プレイヤーの位置を与える ---
	*x = this->x;
	*y = this->y;
}

bool Player::GetShotPosition(int mechNum, int index, double* x, double* y) {
	// --- プレイヤーの発射した弾の座標を参照コピー ---
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
	// --- 指定した弾の属性：Elementを返す
	return shot[mechNum][index].element;
}

int Player::GetShotDamage(int mechNum, int index) {
	// --- 指定した弾のダメージを返す ---
	return shot[mechNum][index].damage;
}

int Player::GetCollision() {
	// --- 機体の当たり判定半径：Collisionを返す ---
	// 画像サイズの横幅＆立幅の平均 / 2を返す
	int collision = (machine[machineType].width / 2 + this->machine[machineType].height / 2) / 2;
	return collision;
}

int Player::GetBulletCollision() {
	// -- 自機発射弾の当たり判定半径：Collisionを返す ---
	// 画像サイズの横幅＆立幅の平均 / 2を返す
	int collision = (shot[machineType][0].width / 2 + shot[machineType][0].heigh / 2) / 2;
	return collision;
}

bool Player::GetDamageSoundFlag() {
	// --- 被弾中の音のフラグを返す ---
	return fs_damage;
}

bool Player::GetRefrectFlag() {
	// --- ノーダメージ時のフラグを返す ---
	return fs_refrect;
}

bool Player::GetDamageFlag() {
	// --- 被弾中であるかのフラグを返す ---
	return damageFlag;
}

int Player::GetShotFlag() {
	// --- ショット効果音の再生フラグを返す ---
	// フラグが無ければ -1 を返す
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
	// --- プレイヤーが無敵状態であるかのフラグを返す ---
	return damageInvalidFlag;
}

bool Player::GetLife() {
	// --- プレイヤーの生存フラグを返す ---
	return life;
}

bool Player::GetDeadFlag() {
	// --- プレイヤーの破壊フラグを返す ---
	return deadFlag;
}

int Player::GetLifeNum() {
	// --- プレイヤーの残機を返す ---
	return lifeNum;
}

int Player::GetHpNum() {
	// --- プレイヤーのHPを返す ---
	return hp;
}

int Player::GetPowerNum() {
	// --- プレイヤーのボーナス攻撃力を返す ---
	return additionalPower;
}

int Player::GetSpeedNum() {
	// --- プレイヤーのボーナス速度を返す ---
	return additionalSpeed;
}

int Player::GetDefenseNum() {
	// --- プレイヤーのボーナス防御力を返す ---
	return additionalDefense;
}

int Player::GetElement() {
	// --- プレイヤーの選択中の属性を返す ---
	return machineType;
}

int Player::GetBombNum() {
	// --- プレイヤーの所持ボム数を返す ---
	return bombNum;
}

bool Player::GetBombFlag() {
	// --- ボム発射中かどうかを返す ---
	return bomb.bombFlag;
}

bool Player::GetBombExplodeFlag() {
	// --- ボムが爆発かどうかを返す ---
	return bomb.explodeFlag;
}

void Player::GetBombPosition(double* x, double* y) {
	// --- ボムの位置を返す ---
	*x = this->bomb.x;
	*y = this->bomb.y;
}

int Player::GetBombCollision() {
	// --- ボムのコリジョンを返す ---
	return bomb.colision;
}

int Player::GetBombDamage() {
	// --- ボムのダメージを返す ---
	return bomb.bomb_damage;
}

bool Player::GetBombShotSoundFlag() {
	// --- ボムショット音フラグを返す ---
	return fs_bombShot;
}

bool Player::GetBombExplodeSoundFlag() {
	// --- ボム爆破音フラグを返す ---
	return fs_bombExplode;
}

bool Player::GetStateChange() {
	// --- プレイヤーにステータス変化があった場合：trueを返す ---
	if (additionalPower != pre_addPower ||
		additionalSpeed != pre_addSpeed ||
		additionalDefense != pre_addDefense) {
		// 1フレーム前の情報を修正する
		pre_addPower = additionalPower;
		pre_addSpeed = additionalSpeed;
		pre_addDefense = additionalDefense;

		// -- ステータス変化に応じて弾の画像切り替え -- 
		if (additionalPower < 3) {
			
			// -- 赤機体の処理 --
			// 何もしない

			// -- 青機体の処理 --
			// - 弾の画像を切り替え -
			int tempGh[BLUE_BULLET_FRAME_MAX];// 一時的なグラフィックハンドラ
			int w, h;
			LoadDivGraph("graphics/Blue/BlueBullet002_6_60_21.png", 6, 6, 1, 60 / 6, 21, tempGh);
			GetGraphSize(tempGh[0], &w, &h);
			// グラフィックハンドル＆画像サイズを弾構造体へ渡す
			for (int i = 0; i < PSHOT_NUM; i++) {
				for (int k = 0; k < BLUE_BULLET_FRAME_MAX; k++) {
					shot[BLUE][i].gh[k] = tempGh[k];
				}
				shot[BLUE][i].width = w;
				shot[BLUE][i].heigh = h;
			}

			// -- 黄機体の処理 -- 
			// 何もしない


		}

		if (pre_addPower >= 3) {
			
			// -- 赤機体の処理 --
			// 何もしない

			// -- 青機体の処理 --
			// - 弾の画像を切り替え -
			int tempGh[BLUE_BULLET_FRAME_MAX];// 一時的なグラフィックハンドラ
			int w, h;
			LoadDivGraph("graphics/Blue/BlueBullet003_6_90_35.png", 6, 6, 1, 90/6, 35, tempGh);
			GetGraphSize(tempGh[0], &w, &h);
			// グラフィックハンドル＆画像サイズを弾構造体へ渡す
			for (int i = 0; i < PSHOT_NUM; i++) {
				for (int k = 0; k < BLUE_BULLET_FRAME_MAX; k++) {
					shot[BLUE][i].gh[k] = tempGh[k];
				}
				shot[BLUE][i].width = w;
				shot[BLUE][i].heigh = h;
			}
			
			// -- 黄機体の処理 --
			// 何もしない


		}
		if (pre_addPower >= 6) {
			
			// -- 赤機体の処理 --
			// 何もしない

			// -- 青機体の処理 --
			// - 弾の画像を切り替え -
			int tempGh[BLUE_BULLET_FRAME_MAX];// 一時的なグラフィックハンドラ
			int w, h;
			LoadDivGraph("graphics/Blue/BlueBullet004_6_120_56.png", 6, 6, 1, 120/6, 56, tempGh);
			GetGraphSize(tempGh[0], &w, &h);
			// グラフィックハンドル＆画像サイズを弾構造体へ渡す
			for (int i = 0; i < PSHOT_NUM; i++) {
				for (int k = 0; k < BLUE_BULLET_FRAME_MAX; k++) {
					shot[BLUE][i].gh[k] = tempGh[k];
				}
				shot[BLUE][i].width = w;
				shot[BLUE][i].heigh = h;
			}

			// -- 黄機体の処理 --
			// 何もしない

		}

		return true;
	}
	else {
		return false;
	}
}

bool Player::GetTranformFlag() {
	// --- プレイヤーの変形フラグを返す ---
	return tr_seFlag;
}

bool Player::GetShiftedFlying() {
	// --- シフトで高速飛行中かどうかを返す ---
	return hs_shifted;
}

bool Player::GetPenetrationFlag() {
	// --- プレイヤーが貫通弾使用中かどうかを返す ---
	return penetrationFlag;

}

void Player::SetShotFlag(int mechNum, int index, bool flag) {
	// --- 指定した弾のフラグを変更 ---
	// (主に弾を消去したい時に使用)
	shot[mechNum][index].flag = flag;
}

void Player::SetDamage(int element, int damage) {
	// --- 被弾時に呼び出し。弾の属性＆ダメージセット
	this->damagedElement = element;
	this->damagedNum = damage;
	damageFlag = true;
}

void Player::SetRespawn() {
	// --- プレイヤーをリスポーンさせる(コンティニュー選択)
	respawnFlag = true;
	lifeNum = PLAYER_INITIAL_LIFENUM;
}

void Player::SetItem(int type) {
	// --- アイテムの効果セット用関数 ---
	switch (type)
	{
	case ITEM_HP:
		// HP回復
		hp += ITEM_HP_RECOVERY_AMOUNT;
		if (hp >= PLAYER_MAX_HP) {
			hp = PLAYER_MAX_HP;
		}
		break;

	case ITEM_POWER:
		// 攻撃力アップ
		additionalPower += 1;
		if (additionalPower >= PLAYER_STATUS_UP_MAX) {
			additionalPower = PLAYER_STATUS_UP_MAX;
		}
		break;

	case ITEM_DEFENSE:
		// 防御力アップ
		additionalDefense += 1;
		if (additionalDefense >= PLAYER_STATUS_UP_MAX) {
			additionalDefense = PLAYER_STATUS_UP_MAX;
		}
		break;

	case ITEM_SPEED:
		// スピードアップ
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