#include "Enemy.h"
#include "define.h"
#include "../dxlib_ext/dxlib_ext.h"

Enemy::Enemy(
	float time,		// 出現時間を格納
	double x,		// スポーン位置x
	double y,		// スポーン位置y
	int type,		// 敵の種類
	int stype,		// 弾の種類
	int s_pattern,	// ショットパターン
	int item,		// アイテム
	int m_pattern,	// 行動パターン
	float in_time,	// 出現時間
	float start_fire_time,	// 発砲開始時間
	float end_fire_time,		// 発砲終了時間
	float out_time,	// 帰還時間
	double outX,	// 帰還座標X
	double outY,		// 帰還座標Y	
	int moveK[ENEMY_MOVE_K]		// 行動パターン係数格納
	) {
	// --- コンストラクタ ---
	// - 敵の基本的なパラメータ - 
	this->totalTime = time;
	this->x = x;
	this->y = y;
	gh = 0;				// Draw()で後に初期化(DxLib_Init()後実施のため)
	ghs[3] = {0};		// Draw()で後に初期化
	width = 0;			// Draw()で後に初期化
	height = 0;			// Draw()で後に初期化
	rad = 0;	
	preRad = 0;
	refX = 0;
	refY = 0;
	// - 画像処理パラメータ - 
	drawInit = false;
	// - 敵の属性等 - 
	this->type = type;
	this->stype = stype;
	this->m_pattern = m_pattern;
	this->s_pattern = s_pattern;
	this->item = item;
	// - 時間：出現、射撃開始, 射撃停止、帰還時間 - 
	this->in_time = in_time;
	this->start_fire_time = start_fire_time;
	this->end_fire_time = end_fire_time;
	this->out_time = out_time;	
	damageAnimTime = 0;
	// - 帰還座標 - 
	this->outX = outX;
	this->outY = outY;
	// ボスフラグ(ボスキャラであればtrue)
	isBoss = false;
	isBossDead = false;
	isBossDeadInit = false;
	// - マシンパラメータ: typeに応じて参照するマシンパラメータ変化 - 
	GetMachine(type);
	// - 弾丸構造体の初期化: stypeに応じて参照する弾丸データ変化 -
	GetShot(stype);
	shotFlag = false;
	hormingFlag = false;
	waveShotFlag = false;
	waveShotRad = 0;
	waveShotCount = 0;
	// 敵の行動パターン係数　格納構造体 初期化
	for (int i = 0; i < ENEMY_MOVE_K; i++) {
		this->moveK[i] = moveK[i];
	}
	// 位置制御用パラメータ
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
	// 時間
	reloadingTime = 0;
	// フラグ
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
	// ダメージ計算用
	damagedElement = 99;	// 適当な数字を入れておく
	damagedNum = 0;
	fs_reflect = false;
	fs_damage = false;
	// 外部の情報を格納
	targetX = 0;
	targetY = 0;

	// 死亡エフェクトクラス
	// -> 本クラス死亡時にインスタンス化.暫定的にnullpointerを与える
	deadEffect = nullptr;

	//// ** デバッグ用
	//// インスタンス時に帰投時間を超えている場合、本インスタンスを消去するフラグを立てる
	//if (totalTime >= out_time -10 ) {
	//	endFlag = true;
	//}
	//if (totalTime <= in_time) {
	//	endFlag = true;
	//}

}

Enemy::~Enemy() {
	// --- デストラクタ ---
	if (deadEffect != NULL) {
		delete deadEffect;
	}
}

bool Enemy::All(float delta_time, double targetX, double targetY) {
	// --- 敵の操作を外部から呼び出し ---

	// - ターゲット(主にプレイヤー座標)を取得
	SetPosition(targetX, targetY);
	// - 目標位置refX, refY 更新
	MoveControl(m_pattern);
	// -- nanエラー回避：ポーズ画面時 --
	if (!delta_time == 0) {
		// - 目標位置に向かって動作
		Move(refX, refY, delta_time);
		// - 射撃 - 
		Shot(delta_time);
	}
	// - ダメージ計算 -
	// SceneManagerから即座に呼び出すよう変更
	// DamageCalc();
	
	// - 死亡時, 一度だけ実行される処理記述 -
	if (deadFlag) {
		if (!deadInitFlag) {
			// 死亡エフェクトクラスインスタンス化
			deadEffect = new DeadEffect(2, x, y, velX, velY, 0.8, 3.0, 2.0);
			// 死亡時、1フレームだけ有効なフラグを立てる
			deadMomentFlag = true;
			deadInitFlag = true;
		}
		else {
			// 死亡時、1フレームだけ有効なフラグを折る
			deadMomentFlag = false;
		}

		// 死亡エフェクトクラス実行＆再生が済んだら(trueを返す)削除
		deadEffectEndFlag = deadEffect->All(delta_time);
	}

	// - 描画 -
	Draw();
	// - 時間管理 - 
	Time(delta_time);

	// 帰投時間を超えている＆画面外に移動した時、本インスタンスを消去するフラグを立てる
	if (totalTime >= out_time && CheckOutOfWindow() && !shootingFlag) {
		endFlag = true;
	}

	// - インスタンス消去フラグを返す(shot()内で管理)
	return endFlag;
}

void Enemy::Move(double refX, double refY, float delta_time){
	// --- 敵の動作 --- 

	// -- 位置制御 -- 
	this->refPosX = refX;		// 目標位置X
	this->refPosY = refY;		// 目標位置Y

	// - 位置追従させる(制御器) - 
	// 速度＆加速度上限値参照
	velMax = machine.refVelMaxNormal;
	accMax = machine.refAccMaxNormal;
	float errorX = (refPosX - x);	// 位置誤差計算
	float errorY = (refPosY - y);	
	// 制御入力生成
	integralX += (errorX + preErrorX) / 2 * delta_time;
	float ux = machine.kp * errorX +
		machine.ki * integralX +
		machine.kd * (errorX - preErrorX) / delta_time;
	integralY += (errorY + preErrorY) / 2 * delta_time;
	float uy = machine.kp * errorY +
		machine.ki * integralY +
		machine.kd * (errorY - preErrorY) / delta_time;


	// - 制御対象へ指令値を反映 - 
	// 加速度
	accX += (ux / machine.mass) * delta_time;
	accY += (uy / machine.mass) * delta_time;
	// 加速度制限
	float accNorm = hypot(accX, accY);
	if (accNorm >= accMax) {
		accX = (accX / accNorm) * accMax;
		accY = (accY / accNorm) * accMax;
	}
	// 速度
	velX += accX;
	velY += accY;
	// 速度制限
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


	// -- 姿勢制御 --
	// 1フレーム前のradを格納(主に偏差射撃参考のため)
	preRad = rad;

	// ターゲットの方を向く
	rad = atan2(targetY - y, targetX - x);

}

void Enemy::Draw() {
	// --- 敵の描画 --- 

	// -- 初期化処理 -- 
	if (!drawInit) {
		
		// - 機体 - 
		// タイプに応じて各々の画像読み込み(追々、csv等から読み込みできるようにしたい)
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
		// 画像のサイズを取得
		GetGraphSize(ghs[machine.element], &width, &height);

		// - 弾 - 
		// 弾の種類に応じて取得画像変更(追々、csv等から読み込みできるようにしたい)

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

		// - グラフィックハンドラ格納＆機体の幅＆高さ格納
		for (int i = 0; i < ENEMY_SNUM; i++) {
			for (int k = 0; k < SHOT_FRAME_MAX; k++) {
				shot[i].gh[k] = temp[k];
			}
			shot[i].width = w;
			shot[i].height = h;
			
		}

		drawInit = true;
	}

	// -- 弾の描画 --
	for (int i = 0; i < ENEMY_SNUM; i++) {
		if (shot[i].flag) {
			// - 弾アニメ切替 -
			if (shot[i].time >= ENEMY_BULLET_ANIM_SPAN) {
				shot[i].time = 0;
				shot[i].animFrame++;
				shot[i].animFrame %= SHOT_FRAME_MAX;
			}
			DrawRotaGraph(shot[i].x, shot[i].y, 1.5, shot[i].rad + tnl::PI / 2, shot[i].gh[shot[i].animFrame], true, false);
		}
	}


	// -- 敵の描画 --
	// - 死亡していなければ描画 - 
	if (!deadFlag) {

		// ダメージ有：ダメージアニメを表示
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
			// - ダメージを受けていない時の描画
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
	// --- 時間の管理 ---
	totalTime += delta_time;
	reloadingTime += delta_time;

	// --　ダメージアニメーションのためのカウント --
	if (damageAnimFlag) {
		damageAnimTime += delta_time;
		if (damageAnimTime >= ENEMY_DAMAGE_ANIMTIME) {
			damageAnimTime = 0;
			damageAnimFlag = false;
		}
	}

}

void Enemy::Shot(float delta_time) {
	// --- 敵の射撃を管理 ---

	// 発砲フラグを折る
	fs_shotFlag = false;
	// 一時変数
	int num = 0;
	float deviationRad = 0;		// 偏差射撃角度計算用

	// 発射タイミングになったら、フラグを立てる
	if (start_fire_time <= totalTime) {
		shotFlag = true;
	}

	// フラグが立っている時だげ射撃
	if (shotFlag) {

		if (!deadFlag) {
			// リロードタイムに応じて射撃実施
			if (reloadingTime >= machine.reloadTime) {
				// 発砲フラグを立てる
				fs_shotFlag = true;
				switch (s_pattern)
				{
				// プレイヤーに向かって単純に射撃
				case 0:
					for (int i = 0; i < ENEMY_SNUM; i++) {
						// フラグが立ってない弾を探して、座標セット
						if (shot[i].flag == false) {
							shot[i].flag = true;
							shot[i].x = x;
							shot[i].y = y;
							shot[i].rad = rad;

							break;
						}
					}

					break;

				// プレイヤーに向かって散弾発射
				case 1:
					for (int i = 0; i < ENEMY_SNUM; i++) {
						// フラグが立ってない弾を探して、座標セット
						if (shot[i].flag == false) {
							// 乱数取得
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

				// プレイヤーに向かって30°のランダムな角度に発射
				case 2:
					for (int i = 0; i < ENEMY_SNUM; i++) {
						// フラグが立ってない弾を探して、座標セット
						if (shot[i].flag == false) {
							// 乱数取得
							
							shot[i].flag = true;
							shot[i].x = x;
							shot[i].y = y;
							shot[i].rad = rad - (60 * tnl::PI / 180) + ((rand() % 120) * tnl::PI / 180);
							
							break;
						}
					}

					break;
				
				// 偏差射撃のような射撃を行う
				// プレイヤーの移動方向に向けてショット
				case 3:
					for (int i = 0; i < ENEMY_SNUM; i++) {
						// フラグが立ってない弾を探して、座標セット
						if (shot[i].flag == false) {
							// 偏差角度を求める
							deviationRad = rad - preRad;
							shot[i].flag = true;
							shot[i].x = x;
							shot[i].y = y;
							// 偏差角度 
							//shot[i].rad = rad + deviationRad * (rand() % 30 * (tnl::PI/180));
							shot[i].rad = rad + deviationRad * (rand() % 40);
							
							break;
						}
					}

					break;

				// 3方向射撃を行う(15°ずつずらす)
				case 4:
					for (int i = 0; i < ENEMY_SNUM; i++) {
						// フラグが立ってない弾を探して、座標セット
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

				// ホーミング弾を打つ
				case 5:
					// ホーミング弾フラグを立てる
					hormingFlag = true;
					for (int i = 0; i < ENEMY_SNUM; i++) {
						// フラグが立ってない弾を探して、座標セット
						if (shot[i].flag == false) {
							shot[i].flag = true;
							shot[i].x = x;
							shot[i].y = y;
							shot[i].rad = rad;

							break;
						}
					}

					break;

				
				// 12方向に扇状射撃
				case 6:
					for (int i = 0; i < ENEMY_SNUM; i++) {
						// フラグが立ってない弾を探して、座標セット
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

				// 偏差射撃 + 3方向ショット
				case 7:
					deviationRad = rad - preRad;
					for (int i = 0; i < ENEMY_SNUM; i++) {
						// フラグが立ってない弾を探して、座標セット
						if (shot[i].flag == false) {
							// 偏差角度を求める
							if (num == 0) {
								deviationRad *= (rand() % 40);
								shot[i].flag = true;
								shot[i].x = x;
								shot[i].y = y;
								// 偏差角度 
								shot[i].rad = rad - (15 * tnl::PI / 180) + deviationRad;
							}

							if (num == 1) {
								shot[i].flag = true;
								shot[i].x = x;
								shot[i].y = y;
								// 偏差角度 
								shot[i].rad = rad + deviationRad;
							}

							if (num == 2) {
								shot[i].flag = true;
								shot[i].x = x;
								shot[i].y = y;
								// 偏差角度 
								shot[i].rad = rad + (15 * tnl::PI / 180) + deviationRad;
							}

							num++;

						}

						if( num >= 3){
							break;
						}

					}

				// ホーミング弾 + 同時7発を打つ
				case 8:
					// ホーミング弾フラグを立てる
					hormingFlag = true;
					for (int i = 0; i < ENEMY_SNUM; i++) {
						// フラグが立ってない弾を探して、座標セット
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

					// プレイヤーに向かってもっと沢山の散弾発射
				case 9:
					for (int i = 0; i < ENEMY_SNUM; i++) {
						// フラグが立ってない弾を探して、座標セット
						if (shot[i].flag == false) {
							// 乱数取得
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

				// リロードタイム再開
				reloadingTime = 0;
			}
		}

		// フラグが立っている弾の数をチェック
		int shotCount = 0;

		// フラグが立っている弾だけ、弾を移動させる
		for (int i = 0; i < ENEMY_SNUM; i++) {
			if (shot[i].flag) {
				// ホーミングフラグが立っていれば、ホーミング弾を打つ
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

				// 画面外に弾がはみ出したら、フラグを戻す
				if ((shot[i].x > WIDTH - MENU_WIDTH) ||
					(shot[i].x < MARGINE) ||
					(shot[i].y > HEIGHT - MARGINE) ||
					(shot[i].y < MARGINE)) {

					shot[i].flag = false;
				}

				// 本敵クラスが発射している弾をカウント
				shotCount++;
			}
		}

		// 発射中の弾が0であるかどうかのフラグを返す(All()で使用)
		if (shotCount == 0) {
			shootingFlag = false;
		}
		else {
			shootingFlag = true;
		}

		// 死亡フラグtrue, 発射中の弾が0ならば、本インスタンスを消去
		if (shotCount == 0 && deadFlag && deadEffectEndFlag) {
			endFlag = true;
		}

	}
}

void Enemy::DamageCalc() {
	// --- 被弾ダメージを計算：外部から呼び出し。 ---
	// ダメージの受け取りはSetDamageで実施
	// ダメージを受けた属性に応じてダメージ変化
	int tempElement = machine.element;
	int calcType;
	// 弾を弾く＆ダメージフラグを戻す
	fs_reflect = false;
	fs_damage = false;
	if (damageFlag) {
		// -- 無属性攻撃の場合の計算 -- 
		if (damagedElement == NON_ELEMENT) {
			if (machine.defence - damagedNum <= 0) {
				machine.hp += (int)machine.defence - damagedNum;
				damageEffectFlag = true;
				damageAnimFlag = true;
			}
			else {
				// ノーダメージ
				fs_reflect = true;
			}
		}
		else {
		// -- 属性攻撃の場合の計算
			// - じゃんけんアルゴリズムを参考 -
			int temp = (tempElement - damagedElement + 3) % 3;
			if (temp == 0) {
				// あいこ
				calcType = 0;
			}
			else if (temp == 1) {
				// 勝ち（効果はいまいち)
				calcType = 1;
			}
			else {
				// 負け (弱点を突かれた)
				calcType = 2;
			}

			// - じゃんけんの結果に応じてダメージ計算 -
			switch (calcType) {
			case 0:
				// あいこ
				if (machine.defence - damagedNum <= 0) {
					machine.hp += (int)machine.defence - damagedNum;
					damageEffectFlag = true;
					damageAnimFlag = true;
					fs_damage = true;
				}
				else {
					// ノーダメージ
					fs_reflect = true;
				}
				break;

			case 1:
				// かち(ダメージ 1/2)
				if (machine.defence - damagedNum / 2 <= 0) {
					// ダメージエフェクト有効化
					machine.hp += (int)machine.defence - damagedNum / 2;
					damageEffectFlag = true;
					damageAnimFlag = true;
					fs_damage = true;
				}
				else {
					// ノーダメージ
					fs_reflect = true;
				}
				break;
			case 2:
				// 負け(ダメージ2倍)
				if (machine.defence - damagedNum * 2 <= 0) {
					machine.hp += (int)machine.defence - damagedNum * 2;
					damageEffectFlag = true;
					damageAnimFlag = true;
					fs_damage = true;
				}
				else {
					// ノーダメージ
					fs_reflect = true;
				}
				break;
			}
		}

		// - ダメージフラグを戻す -//
		damageFlag = false;

		// - hpが0以下ならば、死亡フラグを立てる -
		if (machine.hp <= 0) {
			machine.hp = 0;
			deadFlag = true;
			// ボスキャラであった場合、ボスキャラ死亡フラグを立てる
			// (1フレームのみ有効。GetBossDead()内で初期化する)
			if (isBoss) {
				isBossDead = true;
			}
		}
	}
}

bool Enemy::CircleCollision(double cir1, double cir2, double cirX1, double cirX2, double cirY1, double cirY2) {
	// --- 円形当たり判定 (主に巡回完了確認等に使用 ---
	// cir1, cir2 : 各々の円の半径
	// cirNn : 各々の円の位置X,Y
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
	// --- 敵機が画面外に存在するかをチェック ---
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
	// --- 座標を外部に与える --- 
	*x = this->x;
	*y = this->y;
}

void Enemy::SetPosition(double x, double y) {
	// --- 標的(主にプレイヤー)の位置を格納 --- 
	this->targetX = x;
	this->targetY = y;
}

bool Enemy::GetDeadFlag() {
	// --- 破壊されたか否かのフラグを返す ---
	return deadFlag;
}

int Enemy::GetCollision() {
	// --- 機体の当たり判定半径：Collisionを返す ---
	// 画像サイズの横幅＆縦幅の平均 / 2 を返す
	int collision = (this->width / 2 + this->height / 2) / 2;
	return collision;
}

int Enemy::GetShotCollision(int index) {
	// --- 特定のインデックスの弾のCollisionを返す ---
	// 画像サイズの横幅＆縦幅の平均 / 2 を返す
	int collision = (this->shot[index].width / 2 + this->shot[index].height / 2) / 2;
	return collision;
}


bool Enemy::GetShotPosition(int index, double* x, double* y) {
	// --- 特定のインデックスの弾の位置を返す ---
	// フラグが true : フィールド上に存在する弾のみ位置を返す
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
	// --- 特定のインデックスの弾の属性を返す ---
	return shot[index].element;
}

int Enemy::GetShotDamage(int index) {
	// --- 指定した弾のダメージを返す ---
	return shot[index].damage;
}

bool Enemy::GetDeadMoment() {
	// --- 死亡効果音を鳴らすフラグ---
	return deadMomentFlag;
}

void Enemy::GetVelocity(double* vx, double* vy) {
	// --- 速度情報を参照渡し ---
	*vx = this->velX;
	*vy = this->velY;

}

bool Enemy::GetShotFlag() {
	// --- 発砲したかのフラグを返す --- 
	return fs_shotFlag;
}

int Enemy::GetScore(float totalTime) {
	// --- スコアを返す ---
	// 計算式：敵を早く倒す程、基本スコアより多い値を返す(最大30%)
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
	// --- 敵にセットしたアイテムを返す ---
	return item;
}

bool Enemy::GetBossExist() {
	// --- ボスフラグを返す ---
	return isBoss;
}

bool Enemy::GetBossDead() {
	// --- ボスキャラが死亡した瞬間(1フレーム)、trueを返す ---
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
	// --- 敵が弾を弾いたかどうかのフラグを返す ---
	return fs_reflect;
}

bool Enemy::GetDamageFlag() {
	// --- 敵がダメージを受けたかどうかのフラグを返す ---
	return fs_damage;
}

void Enemy::SetShotFlag(int index, bool flag) {
	// --- 指定した弾のフラグを変更 ---
	// (主に弾を消去したい時に使用)
	shot[index].flag = flag;
}

void Enemy::SetDamage(int element, int damage) {
	// --- 被弾時に呼び出し。弾の属性＆ダメージセット ---
	this->damagedElement = element;
	this->damagedNum = damage;
	damageFlag = true;
}

void Enemy::MoveControl(int m_pattern) {
	// --- 敵の行動(目標位置)を行動パターンに応じて更新する関数 ---
	// 一時変数
	int t_count = 0;
	int t_collision = 0;
	int t_point = 0;
	switch (m_pattern)
	{

	case 0:
		// -- 特攻 --
		// プレイヤーに向かって突進を続ける
		refX = this->targetX;
		refY = this->targetY;
		if (!isBoss) {
			if (out_time <= totalTime) {
				// 帰還時間になったら、帰還ポイントへ移動
				refX = outX;
				refY = outY;
			}
		}

		break;

	case 1:
		// -- 複数座標巡回(ループ無し) --
		t_point = moveK[0];		// 巡回するポイント数
		// k3:X1, k4: Y1, k5:X2, ...を目標地点とし、
		// 順繰りに巡行させる
		if (moveK[1] == 0) {
			// 初期化処理
			moveK[1] = 3;
		}

		// 巡行処理
		t_count = moveK[1];
		t_collision = moveK[2];
		refX = (double)moveK[t_count];
		refY = (double)moveK[t_count + 1];
		// 目標位置へ到達したかの判定
		if (CircleCollision(t_collision, t_collision, refX, x, refY, y)) {
			// 次の目標位置へ進む
			moveK[1] += 2;
			// 最終目標位置へ到着したら、待機
			if (moveK[1] >= 11) {
				moveK[1] = 11;
			}

		}
		else if (moveK[1] > 3 + t_point * 2) {
			// 巡回するポイント数上限
			moveK[1] = 3 + t_point * 2;
		}

		if (!isBoss) {
			if (out_time <= totalTime) {
				// 帰還時間になったら、帰還ポイントへ移動
				refX = outX;
				refY = outY;
			}
		}

		break;

	case 2: 
		// - 複数座標巡行(ループ有)
		// -- 複数座標巡回(ループ無し) --
		t_point = moveK[0];		// 巡回するポイント数
		// k3:X1, k4: Y1, k5:X2, ...を目標地点とし、
		// 順繰りに巡行させる
		if (moveK[1] == 0) {
			// 初期化処理
			moveK[1] = 3;
		}

		// 巡行処理
		t_count = moveK[1];
		t_collision = moveK[2];
		refX = (double)moveK[t_count];
		refY = (double)moveK[t_count + 1];
		// 目標位置へ到達したかの判定
		if (CircleCollision(t_collision, t_collision, refX, x, refY, y)) {
			// 次の目標位置へ進む
			moveK[1] += 2;
			// 最終目標位置へ到着したら、最初の座標に戻る(ループ実現)
			if (moveK[1] >= 11) {
				moveK[1] = 3;
			}

		}
		else if (moveK[1] > 3 + t_point * 2) {
			// 巡回するポイント数上限
			moveK[1] = 3 + t_point * 2;
		}

		if (!isBoss) {
			if (out_time <= totalTime) {
				// 帰還時間になったら、帰還ポイントへ移動
				refX = outX;
				refY = outY;
			}
		}

		break;
	}
}

void Enemy::SetOutTime(float outTime) {
	// --- 敵の帰投時間を変更する ---
	this->out_time = outTime;
}

void Enemy::GetMachine(int type) {
	// --- type に応じてコピーする敵機(machine)を取得 ---
	// ※ 画像の初期化はDraw()内で実施(DxLib_Init()後実施のため)
	// (追々、csv等から読み込みできるようにしたい)
	switch (type)
	{
		// 共通の処理
		machine = {
			.gh = 0, .width = 0, .height = 0
		};

	// -- RED機体(100 ~ 199) --
	case 100:
		// - 中量機, 位置決め精密, 
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
		// - 中量機, 位置決め精密, 
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
		// - 中量機, 位置決め精密, 
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
		// - 中量機, 位置決め精密, 
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
		isBoss = true;		// ボスキャラフラグを立てる
		break;

	case 110:
		// - 中量機, 位置決め精密, 
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
		isBoss = true;		// ボスキャラフラグを立てる
		break;

	case 111:
		// - Ch.1 ボス(1/3体：RED)
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
		isBoss = true;		// ボスキャラフラグを立てる
		break;

	// -- BLUE機体(200~299) --
	case 200:
		// - 中量機, 位置決め精密, 
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
		isBoss = true;		// ボスキャラフラグを立てる
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
		isBoss = true;		// ボスキャラフラグを立てる
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
		isBoss = true;		// ボスキャラフラグを立てる
		break;

		// -- YELLOW機体(300~399) --
	case 300:
		// - 中量機, 位置決め精密, 
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
		// - Ch.2 ボス(3/3体：Yellow)
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
		isBoss = true;		// ボスキャラフラグを立てる
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
		isBoss = true;		// ボスキャラフラグを立てる
		break;
	}
}

void Enemy::GetShot(int stype) {
	// --- stypeに応じてコピーする弾丸(Eshot)を取得 ---
	// ※ 画像の初期化はDraw()内で実施(DxLib_Init()後実施のため)
	// (追々、csv等から読み込みできるようにしたい)
	EShot tempEShot;
	// -- 共通の初期化実施 --
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
	// tempを全ての弾に適用
	for (int i = 0; i < ENEMY_SNUM; i++) {
		shot[i] = tempEShot;
	}

}