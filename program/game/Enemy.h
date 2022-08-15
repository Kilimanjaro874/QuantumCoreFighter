#pragma once
#include "define.h"
#include "DeadEffect.h"

class Enemy
{
private:
	// - 敵の基本的なパラメータ - 
	double x, y;		// x, y座標
	int gh;				// グラフィックハンドラ(一枚絵)
	int ghs[3];			// グラフィックハンドラ(3枚絵)
	int width, height;	// 横幅, 立幅
	float rad;			// 向き
	double refX, refY;	// 到達目標位置(本座標を目指して移動する)
	float preRad;		// 1フレーム前の向き

	// - 画面処理パラメータ - 
	bool drawInit;		// 描画初期処理フラグ

	// - 敵の属性等 -
	int type;			// 種類
	int stype;			// 弾の種類
	int s_pattern;		// ショットパターン
	int item;			// ドロップアイテム
	int m_pattern;		// 移動パターン
	// - 時間 -
	float in_time;			// 出現時間
	float start_fire_time;	// 発砲開始時間
	float end_fire_time;	// 発砲終了時間
	float out_time;			// 帰還時間
	float damageAnimTime;	// ダメージアニメ用カウンタ
	// - 帰還座標 -
	double outX;
	double outY;
	// - マシンパラメータ(読込み専用) - 
	Machine machine;

	// - 弾丸等 -
	EShot shot[ENEMY_SNUM];
	bool shotFlag;			// 弾が撃てるようになったかのフラグ
	bool hormingFlag;		// ホーミング弾にする場合のフラグ
	bool waveShotFlag;		// ウェーブショットする場合のフラグ
	float waveShotRad;		// 
	int waveShotCount;		// 

	// - 敵の行動パターン係数 格納構造体 - 
	int moveK[ENEMY_MOVE_K];

	// - 位置制御用パラメータ -
	float velX;			// 水平方向速度
	float velY;			// 垂直方向速度
	float accX;			// 水平方向加速度
	float accY;			// 垂直方向加速度
	float refPosX;		// 垂直方向位置目標値
	float refPosY;		// 水平方向位置目標値
	float velMax;		// 速度上限値
	float accMax;		// 加速度上限値
	float preErrorX;	// 1フレーム前の位置誤差格納X
	float preErrorY;	// 1フレーム前の位置誤差格納Y
	float integralX;	// 誤差積算値X
	float integralY;	// 誤差積分値Y
	
	// - 時間 -
	float totalTime;		// 経過時間格納
	float reloadingTime;	// 次弾発射までの時間間隔管理
	
	// - フラグ -
	bool damageFlag;		// ダメージフラグ
	bool deadFlag;			// 敵消滅フラグ
	bool deadInitFlag;		// 死亡時、一度だけ実行される関数のフラグ
	bool endFlag;			// 敵クラス消滅フラグ
	bool damageEffectFlag;	// 被ダメージ時のエフェクトフラグ;
	bool deadMomentFlag;	// 破壊フラグ(破壊時の1フレームのみ有効化)
	bool deadEffectEndFlag;	// 死亡エフェクトの再生が終了したらtrue;
	bool damageAnimFlag;	// ダメージアニメフラグ
	bool fs_shotFlag;		// 発砲したかのフラグ
	bool shootingFlag;		// 画面に発射中の弾が存在する時true;		

	// - ダメージ計算用 -
	int damagedElement;	// 被弾時の属性
	int damagedNum;		// 被弾ダメージ
	bool fs_reflect;	// ノーダメージフラグ
	bool fs_damage;		// ダメージフラグ
	// - 外部の情報を格納 -
	double targetX, targetY;	// 標的(主にプレイヤー)の位置を格納

	// - ボスフラグ -
	bool isBoss;			// ボスであるか
	bool isBossDead;		// ボス死亡フラグ
	bool isBossDeadInit;	// ボス死亡初期処理フラグ
	//- 死亡エフェクトクラス
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
		float time,		// 出現時間を格納
		double x,		// スポーン位置x
		double y,		// スポーン位置y
		int type,		// 敵の種類
		int stype,		// 弾の種類
		int s_pattern,	// ショットパターン
		int item,		// アイテム
		int m_pattern,	// 行動パターン
		float in_time,	// 出現時間
		float start_fire_time,		// 発砲開始時間
		float end_fire_time,		// 発砲終了時間
		float out_time,				// 帰還時間
		double outX,	// 帰還座標X
		double outY,	// 帰還座標Y	
		int moveK[ENEMY_MOVE_K]		// 行動パターン係数格納
		);
	~Enemy();
};