#pragma once

extern int scene;				// �V�[���؂�ւ��t���O(from game_main.c)
static bool screenFixedFlag = true;	// �Œ�E�B���h�E���[�h

#define SCENE_NON_CHANGE 0		// �V�[���؂�ւ��Ȃ�
#define END_GAME		-1		// �Q�[���I��
#define SCENE_OPENING	100		// �I�[�v�j���O
#define SCENE_TITLE		200		// �^�C�g�����
#define SCENE_OPTION	201		// �I�v�V�������
#define SCENE_CH_SELECT	300		// �`���v�^�[�Z���N�g
#define SCENE_CH_1		400		// �`���v�^�[1

// -- �v���g�^�C�v�錾 --
void sceneTitle(float delta_time);								// �^�C�g���֐�
void endSceneTitle();							// �^�C�g���I���֐�
void sceneOpening(float delta_time);			// �I�[�v�j���O�֐�
void endSceneOp();								// �I�[�v�j���O�I���֐�
void sceneChSelect();					// �`���v�^�[�Z���N�g�֐�
void endSceneChSelect();				// �`���v�^�[�Z���N�g�G���h�֐�
void scenePlayCh1(float delta_time);	// Ch1 �Q�[���v���C�֐�
void endScenePlayCh1();					// Ch1�@�Q�[���v���C�I���֐�

// -- �Q�[���I���� ���s����֐� -- 
void endGame();					