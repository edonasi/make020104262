//�w�b�_�t�@�C���̓ǂݍ���
#include "DxLib.h";//�K���K�v

//�}�N����`
#define GAME_TITLE "�Q�[���^�C�g��"	//�Q�[���^�C�g��
#define GAME_WIDTH 1280				//�Q�[���̉���
#define GAME_HEIGHT 720				//�Q�[���̏c��
#define GAME_COLOR 32				//�Q�[���̉𑜓x
#define BACK_GROUND_COLOR 255,255,255	//�Q�[���̔w�i�F
#define GAME_ICON_ID 333	//�Q�[����ICON��ID

// �v���O������ WinMain ����n�܂�܂�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//�E�B���h�E���[�h�ɐݒ�
	ChangeWindowMode(TRUE);
	//�Q�[���^�C�g���̐ݒ�
	SetMainWindowText(GAME_TITLE);
	//�Q�[���E�B���h�E��Ԑݒ�
	SetGraphMode(GAME_WIDTH, GAME_HEIGHT, GAME_COLOR);
	//�w�i�F�̐ݒ�
	SetBackgroundColor(BACK_GROUND_COLOR);
	//�Q�[����ICON��ݒ�
	SetWindowIconID(GAME_ICON_ID);

	if (DxLib_Init() == -1)		// �c�w���C�u��������������
	{
		return -1;			// �G���[���N�����璼���ɏI��
	}

	while (1)
	{
		//�L�[�������ꂽ�Ƃ�
		if (CheckHitKeyAll() != 0)
		{
			//�������[�v�𔲂���
			break;
		}

		//���b�Z�[�W���󂯎�葱����A�~�ȂǂŃE�B���h�E�������Ƃ�
		if (ProcessMessage() != 0) 
		{
			//�������[�v�𔲂���
			break;
		}
	}

	// �c�w���C�u�����g�p�̏I����������(return 0�Ń\�t�g���I������)
	DxLib_End();				

	// �\�t�g�̏I��
	return 0;				 
}