#pragma once

//�Q�[���S�̂̃w�b�_�t�@�C��
//�w�b�_�t�@�C���̓ǂݍ���
#include "DxLib.h"//�K���K�v

////�}�N����`]
//�Q�[���^�C�g��
#define GAME_TITLE "�Q�[���^�C�g��"	
//�Q�[���̉���
#define GAME_WIDTH 1280
//�Q�[���̏c��
#define GAME_HEIGHT 720				
//�Q�[���̉𑜓x
#define GAME_COLOR 32				
//�Q�[���̔w�i�F
#define BACK_GROUND_COLOR 200,200,200	
//�Q�[����ICON��ID
#define GAME_ICON_ID 333	
//�E�B���h�E�o�[�̎��(0 : �c���̒�����ύX�s��)(1 : �o�[���Ȃ�)
#define GAME_WINDOW_BAR 0
//�f�o�b�N���[�h
#define GAME_DEBUG true

//�񋓌^
//�Q�[���̃V�[��
enum GAME_SCENE
{
	GAME_SCENE_TITLE,
	GAME_SCENE_PLAY,
	GAME_SCENE_END,
	GAME_SCENE_CHANGE
};