#pragma once

class Item
{
private:
	int type;						// �A�C�e���̎��
	bool existeFlag;				// �A�C�e���o���t���O
	bool drawInit;					// �`��n������
	static int hpItemGh[3];
	static int powerItemGh[3];
	static int defenseItemGh[3];
	static int speedItemGh[3];
	static int bombItemGh[3];
	double x, y;
	int animFrame;					// �A�j���[�V�����̃t���[��
	float animCounter;				// �A�j���ؑ֎��ԃJ�E���^
	float animChangeTime;			// �A�j���[�V�����ؑ֎���
	float totalTime;				// �����t���O�������Ă���̎���
	
private:
	void Move(float delta_time);
	void Draw();
	void Time(float delta_time);
public:
	Item();
	void All(float delta_time);
	void Delete();
	int GetType();
	bool GetFlag();
	void GetPosition(double* x, double* y);
	void SetFlag(double x, double y, int data);
};