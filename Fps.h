#pragma once

//�w�b�_�ǂݍ���
#include "Game.h"	//�Q�[���S�̂̃w�b�_�t�@�C��

//�}�N����`
#define GAME_FPS 60			//FPS�W��
#define GAME_FPS_MAX 240	//FPS�ő�l
#define GAME_FPS_MIN 60 	//FPS�ő�l

//FPS�\����
struct FPS
{
	bool IsInitFlag = false;	//�ŏ��̑���J�n�t���O
	LONGLONG StartTime = 0;		//�J�n�J�n����
	LONGLONG NowTime = 0;		//���݂̎���
	LONGLONG OldTime = 0;		//�ȑO�̎���

	int Value = GAME_FPS;		//FPS�̒l

	float DeltaTime = 0.000001;	//�o�ߎ���
	int Count = 1;				//���݂̃t���[����
	float DrawValue = 0.0f;		//�v�Z���ʂ�`��
	int SampleRote = GAME_FPS;	//���ς����T���v���l
};

//�v���g�^�C�v�錾
void FpsUpdate(void);	// Fps�l�𑪒肵�A�l���X�V����
void FpsDraw(int draw_x, int draw_y, bool isBlackColor);	//�f�o�b�N�p�̕���FPS�l��`�悷��
void FpsWait();			// FPS�ŏ�����҂�