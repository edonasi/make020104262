//�w�b�_�t�@�C���̓ǂݍ���
#include "DxLib.h";//�K���K�v
#include "Keyboard.h"

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

//�����Ă���ԉ���
int AddSpeed(int speed);

// �v���O������ WinMain ����n�܂�܂�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//Log.txt���o�͂��Ȃ�(GitHub�ŊǗ����邽�߁A���ݕK�v�Ȃ�)
	SetOutApplicationLogValidFlag(FALSE);	
	//�E�B���h�E���[�h�ɐݒ�
	ChangeWindowMode(TRUE);
	//�Q�[���^�C�g���̐ݒ�
	SetMainWindowText(GAME_TITLE);
	//�Q�[���E�B���h�E�𑜓x�ݒ�
	SetGraphMode(GAME_WIDTH, GAME_HEIGHT, GAME_COLOR);
	//�E�B���h�E�̃T�C�Y��ݒ�
	SetWindowSize(GAME_WIDTH, GAME_HEIGHT);
	//�w�i�F�̐ݒ�
	SetBackgroundColor(BACK_GROUND_COLOR);
	//�Q�[����ICON��ݒ�
	SetWindowIconID(GAME_ICON_ID);
	//�E�B���h�E�o�[�̏��
	SetWindowStyleMode(GAME_WINDOW_BAR);
	//�E�B���h�E�̐���������L���ɂ���(�f�B�X�v���C�̔\�͂𐧌����āA�t���[�����[�g(fps)�����ɕۂ�)=FALSE�̓t���[�����[�g���Ǘ����Ȃ���΂Ȃ�Ȃ�
	SetWaitVSyncFlag(TRUE);
	//��A�N�e�B�u�ł����s����
	SetAlwaysRunFlag(TRUE);

	if (DxLib_Init() == -1)		// �c�w���C�u��������������
	{
		return -1;			// �G���[���N�����璼���ɏI��
	}
	//�_�u���o�b�t�@�����O�L����
	SetDrawScreen(DX_SCREEN_BACK);

	
	//�~�̒��S�_
	int x = GAME_WIDTH / 2;
	int y = GAME_HEIGHT / 2;
	//�~�̔��a
	int radius = 50;
	//�ړ��̑���
	int speed = 1;

	while (1)
	{		
		//��ʂ���������
		if (ClearDrawScreen() != 0) { break; }
		//�L�[���͏�Ԃ��X�V����
		AllKeyUpdate();
		
		//���b�Z�[�W���󂯎�葱����A�~�ȂǂŃE�B���h�E�������Ƃ�
		if (ProcessMessage() != 0) 
		{
			//�������[�v�𔲂���
			break;
		}

		//�X�s�[�h�𑬂�����
		/*if (Key1Down(KEY_INPUT_UP) || Key1Down(KEY_INPUT_DOWN)
			|| Key1Down(KEY_INPUT_RIGHT) || Key1Down(KEY_INPUT_LEFT))
		{
			speed++;
		}*/

		//���x�����Z�b�g
		if (KeyUpping(KEY_INPUT_UP) || KeyUpping(KEY_INPUT_DOWN)
			|| KeyUpping(KEY_INPUT_RIGHT) || KeyUpping(KEY_INPUT_LEFT))
		{
			if (speed > 0)
			{
				speed--;
			}
			else 
			{
				speed = 0;
			}
			
		}

		//�㉺�̍��W���v�Z
		if (KeyDown(KEY_INPUT_UP)) 
		{
			y -= speed;
			speed = AddSpeed(speed);
		}
		if (KeyDown(KEY_INPUT_DOWN)) 
		{
			y += speed;
			speed = AddSpeed(speed);
		}

		//���E�̍��W���v�Z
		if (KeyDown(KEY_INPUT_RIGHT)) 
		{
			x += speed;
			speed = AddSpeed(speed);
		}
		if (KeyDown(KEY_INPUT_LEFT)) 
		{
			x -= speed;
			speed = AddSpeed(speed);
		}

		DrawFormatString(0, 0, GetColor(0, 0, 0), "%d", speed);

		//�~�̕`��
		DrawCircle(x, y, radius, GetColor(0, 0, 0), TRUE);

		//�_�u���o�b�t�@�����O�����摜��`��
		ScreenFlip();
	}

	// �c�w���C�u�����g�p�̏I����������(return 0�Ń\�t�g���I������)
	DxLib_End();				

	// �\�t�g�̏I��
	return 0;				 
}

int AddSpeed(int speed) 
{
	//�ő呬�x
	int max = 100;

	if (speed < max)
	{
		return speed + 1;
	}
	else 
	{
		return max;
	}
}