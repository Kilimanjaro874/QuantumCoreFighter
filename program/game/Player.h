#pragma once
#include "../../program/dxlib_ext/dxlib_ext.h"	// DxLib等参照
#include "define.h"
#include "DeadEffect.h"

class Player {

private:
	// --- プレイヤー自身のパラメータ --- 
	// -- 基本情報 --
	double x, y;		// x, y座標
	bool life;			// 生存フラグ
	int machineType;	// 選択中のマシン
	int pre_machineType;	// 以前選択していたマシン
	float rad;			// プレイヤーの向き
	int hp;				// プレイヤーのHP(マシンに関係無く共有)
	int lifeNum;		// プレイヤー残機
	double deadPosX;	// プレイヤー死亡座標
	double deadPosY;	// 
	// -- 照準 --
	int aimX;		// 照準座標
	int aimY;
	int aimGh;			// 標準グラフィックハンドラ
	// -- ステータスアップ --
	int additionalPower;		// 攻撃力ボーナス
	int additionalSpeed;		// スピードボーナス
	int additionalDefense;		// 防御力ボーナス
	int pre_addPower;			// 1フレーム前の攻撃力ボーナス(フラグ立てに使用 )
	int pre_addSpeed;			// 1フレーム前のスピードボーナス
	int pre_addDefense;			// 1フレーム前の防御力ボーナス
	// -- ボム --
	int bombNum;				// ボム数
	Bomb bomb;					// ボム構造体
	bool fs_bombShot;			// ボムショット音フラグ
	bool fs_bombExplode;		// ボム爆破音フラグ
	//  -- 変形モーション --
	int transformGh[12];		// 変形モーショングラフィックハンドラ
	int tr_animFlame;			// 変形モーションフレーム
	float tr_animTime;			// 変形アニメーション時間間隔
	float tr_MaxTime;			// 変形アニメーション最大時間
	float tr_countTime;			// 変形時間格納
	bool tr_animFlag;			// 変形アニメーション実行フラグ
	bool tr_seFlag;				// 変形SEフラグ

	// - 画面処理パラメータ - 
	bool drawInit;		// 描画初期処理フラグ

	// - 速度制御用パラメータ[機体毎に設ける]
	Machine machine[MACHINE_NUM];
	float velX;			// 水平方向速度
	float velY;			// 垂直方向速度
	float accX;			// 水平方向加速度
	float accY;			// 垂直方向加速度
	float refVelY;		// 垂直方向速度目標値
	float refVelX;		// 水平方向速度目標値
	float velMax;		// 速度上限値
	float accMax;		// 加速度上限値
	float integralX;	// 制御誤差積算値X
	float integralY;	// 制御誤差積算値Y
	float preErrorX;	// 1フレーム前の速度誤差格納
	float preErrorY;	// 1フレーム前の速度誤差格納

	// - 装備系のパラメータ
	Shot shot[MACHINE_NUM][PSHOT_NUM];		// 各機体毎に弾を管理する
	int shotCount;		// 弾発射カウント

	// - 時間 - 
	float totalTime;			// 経過時間格納
	float reloadingTime;		// 次弾発射までの時間間隔管理
	float damageInvalidTime;	// ダメージ無効化時間
	float damageAnimCounter;	// ダメージアニメーション再生用カウンタ
	float respawnTime;			// リスポーン時間
	// - フラグ -
	bool damageFlag;		// 被ダメージフラグ
	bool damageInvalidFlag;	// 無敵フラグ
	bool deadFlag;			// 死亡フラグ
	bool respawnFlag;		// リスポーン中のフラグ
	bool redShotFlag;		// ショットフラグ
	bool blueShotFlag;
	bool yellowShotFlag;	
	bool deadEffectFlag;	// 死亡エフェクト実施
	bool deadEffectInitFlag;	// 消滅エフェクトクラスインスタンスフラグ
	bool drawBomb;			// 爆破エフェクト描画フラグ
	bool fs_damage;			// ダメージ効果音フラグ
	bool fs_refrect;		// 無ダメージ効果音フラグ
	bool hs_shifted;		// シフトで高速飛行中かどうかのフラグ
	bool penetrationFlag;	// 貫通弾を使用中かどうかのフラグ

	// - ダメージ計算用 -
	int damagedElement;		// 被弾時の属性
	int damagedNum;			// 被弾ダメージ

	// - 消滅エフェクトクラス
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