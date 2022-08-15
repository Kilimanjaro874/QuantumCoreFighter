#pragma once
// -- 環境設定 -- 
#define WIDTH 1024			// ゲーム画面横幅
#define HEIGHT 768			// ゲーム画面縦幅
#define MENU_WIDTH 300		// メニュー画面横幅	 
#define MENU_HEIGHT 720		// メニュー画面縦幅	
#define SCROLL_SPEED 200	// 背景のスクロールスピード
// -- 背景 --
#define NAME_SIZE 50		// 背景名サイズ

// -- 定数定義 --
#define MARGINE 15		// プレイヤーの移動制限
#define TOTAL_CH 6		// チャプター総数

// -- 3すくみ定義 --
#define RED 0			// 赤属性
#define BLUE 1			// 青属性
#define YELLOW 2		// 黄属性
#define NON_ELEMENT 3	// 無属性

// -- BGM&SEボリューム --
#define BGM_INITIAL_VOLUME 50
#define SE_INITIAL_VOLUME 50

// -- プレイヤーのパラメータ等定義 -- 
#define PLAYER_INITIAL_LIFENUM 3		// プレイヤー初期残機数
#define PLAYER_INITIAL_HP 100			// プレイヤー初期体力
#define PLAYER_MAX_HP 100				// プレイヤー体力上限
#define PLAYER_RESPAWN_TIME 3			// プレイヤーリスポーンタイム
#define PLAYER_RESPAWN_POS_X (WIDTH - MENU_WIDTH) / 2	// プレイヤーリスポーン位置X
#define PLAYER_RESPAWN_POS_Y (HEIGHT - MARGINE - 70)
#define DAMAGE_INVALID_TIME_MAX 1.5		// 被ダメージ後の無敵時間(s)
#define RED_BULLET_COLLISION 3
#define BLUE_BULLET_COLLISION 4
#define YELLOW_BULLET_COLLISION 3
#define RED_BULLET_FRAME_MAX 3			// 赤機体弾画像フレーム数
#define BLUE_BULLET_FRAME_MAX 6			// 青機体弾画像フレーム数
#define YELLOW_BULLET_FRAME_MAX 4		// 黄機体弾画像フレーム数
#define RED_BULLET_ANIM_SPAN 0.1		// 赤機体弾アニメ切替時間
#define BLUE_BULLET_ANIM_SPAN 0.1		// 青機体弾アニメ切替時間
#define YELLOW_BULLET_ANIM_SPAN 0.1		// 黄機体弾アニメ切替時間
#define PLAYER_INITIAL_BOMBNUM 3		// プレイヤー初期ボム数
#define PLAYER_MAX_BOMBNUM 3			// プレイヤーボム数上限
#define PLAYER_STATUS_UP_MAX 8			// ステータスアップ上限値

// -- プレイヤーの初期ステータス..まずは書いておく --
// ステータス変化をもたらすため、初期の値を参照したい。なので定義。
#define RED_INITIAL_BULLET_ATK 18		// 赤機体通常弾初期攻撃力
#define BLUE_INITIAL_BULLET_ATK 45		// 青機体通常弾初期攻撃力
#define YELLOW_INITIAL_BULLET_ATK 65	// 黄機体通常弾初期攻撃力
#define RED_INITIAL_DEFENSE 15			// 赤機体初期防御力
#define BLUE_INITIAL_DEFENSE 20			// 青機体初期防御力
#define YELLOW_INITIAL_DEFENSE 12		// 黄機体初期防御力
#define RED_REF_ACC_MAX_N 0.3			// 赤機体初期通常加速度
#define RED_REF_ACC_MAX_S 0.6			// 赤機体初期シフト加速度
#define RED_REF_VEL_MAX_N 3.5			// 赤機体初期通常速度
#define RED_REF_VEL_MAX_S 5.5			// 赤機体初期シフト速度
#define BLUE_REF_ACC_MAX_N 0.2			// 青機体初期通常加速度
#define BLUE_REF_ACC_MAX_S 0.4			// 青機体初期シフト加速度
#define BLUE_REF_VEL_MAX_N 3.2			// 青機体初期通常速度
#define BLUE_REF_VEL_MAX_S 4.5			// 青機体初期シフト速度
#define YELLOW_REF_ACC_MAX_N 0.4		// 黄機体初期通常加速度
#define YELLOW_REF_ACC_MAX_S 0.7		// 黄機体初期シフト加速度
#define YELLOW_REF_VEL_MAX_N 4.8		// 黄機体初期通常速度
#define YELLOW_REF_VEL_MAX_S 7.5		// 黄機体初期シフト速度
#define RED_RELOAD_TIME 0.08			// 赤機体初期リロードタイム
#define BLUE_RELOAD_TIME 0.4			// 青機体初期リロードタイム
#define YELLOW_RELOAD_TIME 0.6			// 黄機体初期リロードタイム
#define RED_BULLET_SPEED 800			// 赤機体弾速初期値
#define BLUE_BULLET_SPEED 1800			// 青機体弾速初期値
#define YELLOW_BULLET_SPEED 600			// 黄機体弾速初期値

// -- スコアボードのパラメータ等定義 -- 
#define MENU_ORIGINE_X  WIDTH - MENU_WIDTH			// メニュー画面原点Ｘ
#define MENU_ORIGINE_Y  MARGINE					// メニュー画面原点Y
#define ELEMENT_X MENU_ORIGINE_X + MENU_WIDTH * 50/100			// エレメント表示位置X
#define ELEMENT_Y MENU_ORIGINE_Y + MENU_HEIGHT * 85/100		// エレメント表示位置Y

// -- アイテムの情報 --
#define ITEM_NUM 30						// 画面上のアイテム数上限
#define ITEM_COLLISION 16				// アイテム取得範囲
#define ITEM_NONE 0						// アイテム無し
#define ITEM_HP 1						// 回復アイテム
#define ITEM_POWER 2					// パワーアップアイテム
#define ITEM_DEFENSE 3					// ディフェンスアップアイテム
#define ITEM_SPEED 4					// スピードアップアイテム
#define ITEM_BOMB 5						// ボム所持数アップアイテム

// - アイテムの効果 -
#define ITEM_HP_RECOVERY_AMOUNT 30		// アイテムでのHP回復量

// - ボム構造体 -
struct Bomb {
	int gh[3];				// グラフィックハンドラ
	int effectGh[10];		// 爆破エフェクトハンドラ
	double x;				// 座標
	double y;
	int targetX;			// ボム到達目標値
	int targetY;			// 
	float rad;				// ボム到達目標角度
	float velMagunitude;	// ボム初速
	float velX;				// ボム初速x成分
	float velY;				// ボム初速y成分
	float bomb_animTime;	// ボムアニメーション変更時間間隔
	int bomb_animFlame;		// ボム移動アニメーションフレーム
	float bomb_explodeAnimTime;		// ボムアニメーション変更時間間隔
	int bomb_explodeFlame;	// ボム爆破アニメーションフレーム
	bool bombFlag;			// ボム発動フラグ
	bool explodeFlag;		// ボム爆発フラグ
	int bomb_damage;		// ボムのダメージ
	int colision;			// ボムのコリジョン
};
#define BOMB_VEL_MAGNITUDE 300			// ボム発射初速度
#define BOMB_EXPLODE_COLLISION	150		// ボム爆破コライダー
#define BOMB_DAMAGE 900					// ボム攻撃力 

// -- 敵のパラメータ等定義 --
#define EFFECT_EDEADNUM	25				// 敵破壊エフェクトクラス定義数（画面上のエフェクト上限数)
#define ENEMY_BULLET_ANIM_SPAN 0.1		// 敵機体弾アニメ切替時間
#define ENEMY_DAMAGE_ANIMTIME 0.2		// 敵機被弾アニメ表示時間

// - 機体毎の動作を特徴づけるパラメータ構造体 - 
struct Machine {
	int gh;				// グラフィックハンドル
	int width;			// 機体の横幅
	int height;			// 機体の縦幅
	int element;		// 機体の属性
	int hp;				// 機体の体力
	int defence;		// 機体の防御力
	float mass;			// 機体質量
	float refAccMaxNormal;		// 通常飛行時の加速度上限
	float refAccMaxShifted;		// 高速飛行時の加速度上限
	float refVelMaxNormal;		// 通常飛行時の速度上限
	float refVelMaxShifted;		// 高速飛行時の速度上限
	float kp;					// 比例定数
	float ki;					// 積分定数
	float kd;					// 微分定数
	float reloadTime;			// 次弾発射までの時間
	float bulletSpeed;			// 弾丸の発射速度
	int score;					// (敵機用 : 撃破スコア)
};
#define MACHINE_NUM 3			// マシン数
#define MACHINE_RED	0			// マシンの種類：Red
#define MACHINE_BLUE 1			// マシンの種類：Blue
#define MACHINE_YELLOW 2		// マシンの種類：Yellow

// - 弾の構造体 - 
#define PSHOT_NUM 50	// 弾数制限
#define SHOT_FRAME_MAX 6	// 弾画像フレーム上限
struct Shot {
	int gh[SHOT_FRAME_MAX];				// グラフィックハンドラ
	bool flag;			// 弾が発射中かどうか
	double x;			// 座標
	double y;
	int width, heigh;	// 画像の幅と高さ
	float rad;			// 弾の発射角度
	float speed;		// 弾の速度
	int element;		// 弾の属性
	float damage;		// 弾のダメージ
	float time;			// スポーンしてからの経過時間
	int animFrame;		// 弾アニメーションのフレーム数
};


// -- 敵のパラメータ等定義 -- 
#define ENEMY_NUM 100	// 敵の総数
// - 敵の弾構造体 -
#define ENEMY_SNUM 100	// 敵の弾の所持数上限
struct EShot {
	int type;			// 弾の種類
	int gh[SHOT_FRAME_MAX];				// グラフィックハンドラ
	bool flag;			// 弾が発射中かどうか
	double x;			// 座標
	double y;
	int width, height;	// 画像の幅と高さ
	float rad;			// 弾の発射角度
	float speed;		// 弾のスピード
	int element;		// 弾の属性
	float damage;		// ダメージ
	float time;			// スポーンしてからの経過時間
	int animFrame;		// 弾アニメーションのフレーム数
};

// - 敵のデータ構造体(ファイルコピーのため)
#define ENEMY_MOVE_K 13	// 敵の行動パターン係数格納数上限
struct EnemyData {
	int x;		// スポーン位置x
	int y;		// スポーン位置y
	int type;		// 敵の種類
	int stype;		// 弾の種類
	int s_pattern;	// ショットパターン
	int item;		// アイテム
	int m_pattern;	// 行動パターン
	float in_time;	// 出現時間
	float start_fire_time;	// 発砲開始時間
	float end_fire_time;		// 発砲終了時間
	float out_time;	// 帰還時間
	int outX;		// 帰還座標X
	int outY;		// 帰還座標Y	
	int moveK[ENEMY_MOVE_K];		// 行動パターン係数格納
	int colision;					// ボムのコリジョン
};

// - 破壊エフェクト - 
#define DEAD_EFFECT_FRAME_MAX 10	// 破壊アニメーションのフレーム上限

