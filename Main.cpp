//�w�b�_�t�@�C���̓ǂݍ���
#include "DxLib.h";//�K���K�v

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

enum NowColorState
{
	black,
	red,
	green,
	blue,
	magenta,
	cyan,
	yellow,
	white
};
NowColorState ChangeColorState(NowColorState nowColor);


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

	//�l�p�̈ʒu
	int xPos = 0;
	int yPos = 0;

	//�~�̈ʒu(���S�ʒu)
	int xcPos = 0;
	int ycPos = 0;
	int xcHalf = GAME_WIDTH / 2;
	int ycHalf = GAME_HEIGHT / 2;

	//��������
	int speedAbs = 8;
	int xSpeed = speedAbs;
	int ySpeed = speedAbs;
	int xcSpeed = speedAbs;
	int ycSpeed = speedAbs;

	//�l�p�̑傫��
	//��
	int width = 32;
	//����
	int height = 32;

	//�~�̑傫��
	int radius = 100;

	//�l�p�̐F
	NowColorState nowColorState = red;
	int maxColor = 255;
	int minColor = 0;
	//�F�̍\����
	struct Color
	{
		int red;
		int green;
		int blue;
	};
	//�F�̍\���̂�ݒ�
	Color nowColor;
	Color blackColor;
	Color redColor;
	Color greenColor;
	Color blueColor;
	Color magentaColor;
	Color cyanColor;
	Color yellowColor;
	Color whiteColor;
	//�F��������
	//black
	blackColor.red = minColor;
	blackColor.green = minColor; 
	blackColor.blue = minColor;
	//red
	redColor.red = maxColor;
	redColor.green = minColor;
	redColor.blue = minColor;
	//green
	greenColor.red = minColor;
	greenColor.green = maxColor;
	greenColor.blue = minColor;
	//blue
	blueColor.red = minColor;
	blueColor.green = minColor;
	blueColor.blue = maxColor;
	


	while (1)
	{
		//��ʂ���������
		if (ClearDrawScreen() != 0) { break; }
		

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

		/*
		//�F��ݒ肷��
		switch (nowColorState)
		{
		case black:
			nowColor = blackColor;
			break;

		case red:
			nowColor = redColor;
			break;

		case green:
			nowColor = greenColor;
			break;

		case blue:
			nowColor = blueColor;
			break;

		case magenta:
			nowColor = magentaColor;
			break;

		case cyan:
			nowColor = cyanColor;
			break;

		case yellow:
			nowColor = yellowColor;
			break;

		case white:
			nowColor = whiteColor;
			break;
		}
		*/

		//�l�p��`��
		//DrawBox(xPos, yPos, xPos+width, yPos+height, GetColor(nowColor.red, nowColor.green, nowColor.blue), TRUE);
		DrawCircle(xcPos, ycPos, radius, GetColor(0, 0, 0), FALSE,5);
		//���W�ړ��v�Z
		xPos += xSpeed;
		yPos += ySpeed;
		xcPos += xcSpeed;
		ycPos += ycSpeed;

		//�l�p
		//x- �Ȃ甽�Ε����Ɉړ�������
		if (xPos > GAME_WIDTH - width)
		{
			xSpeed = -speedAbs;
			//�F�̏�Ԃ�ύX
			nowColorState = ChangeColorState(nowColorState);
		}
		else if (xPos < 0)
		{
			xSpeed = speedAbs;
			//�F�̏�Ԃ�ύX
			nowColorState = ChangeColorState(nowColorState);
		}

		if (yPos > GAME_HEIGHT - height)
		{
			ySpeed = -speedAbs;
			//�F�̏�Ԃ�ύX
			nowColorState = ChangeColorState(nowColorState);
		}
		else if (yPos < 0)
		{
			ySpeed = speedAbs;
			//�F�̏�Ԃ�ύX
			nowColorState = ChangeColorState(nowColorState);
		}

		//�~
		if (xcPos + radius > GAME_WIDTH)
		{
			xcSpeed = -speedAbs;
		}
		else if (xcPos - radius < 0)
		{
			xcSpeed = speedAbs;
		}

		
		if (ycPos + radius > GAME_HEIGHT)
		{
			ycSpeed = -speedAbs;
		}
		else if (ycPos - radius < 0)
		{
			ycSpeed = speedAbs;
		}
		
		//�_�u���o�b�t�@�����O�����摜��`��
		ScreenFlip();
	}

	// �c�w���C�u�����g�p�̏I����������(return 0�Ń\�t�g���I������)
	DxLib_End();				

	// �\�t�g�̏I��
	return 0;				 
}


//�F�̏�Ԃ�ύX
NowColorState ChangeColorState(NowColorState nowColor) 
{
	int nowColorNum = (int)nowColor;

	nowColorNum++;

	if (nowColorNum > white)
	{
		nowColorNum = (int)black;
	}

	nowColor = (NowColorState)nowColorNum;

	return nowColor;
}