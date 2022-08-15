#pragma once

class Tutorial
{
private:
	int tutorialGh[5];
	int tutorialEndFlag;
	int count;
	bool drawInit;
	bool soundInit;

private:
	void Draw();
	void Move();

public:
	Tutorial();
	bool All();
};