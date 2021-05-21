//�w�b�_�t�@�C���̓ǂݍ���
#include "DxLib.h"//�K���K�v
#include "Fps.h"
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

//�񋓌^
//�Q�[���̃V�[��
enum GAME_SCENE 
{
	GAME_SCENE_TITLE,
	GAME_SCENE_PLAY,
	GAME_SCENE_END,
	GAME_SCENE_CHANGE
};

//�V�[�����Ǘ�����ϐ�
//���݂̃Q�[���̃V�[��
GAME_SCENE GameScene;
//�O��̃Q�[���̃V�[��
GAME_SCENE GameSceneOld;
//���̃Q�[���̃V�[��
GAME_SCENE GameSceneNext;

//��ʂ̐؂�ւ�
//�t�F�[�h�A�E�g���Ă��邩
bool isFadeOut = false;
//�t�F�[�h�C�����Ă��邩
bool isFadeIn = false;

//�؂�ւ��~���b
int fadeTimeMill = 2000;
//�~���b���t���[���b�ɕϊ�
int fadeTimeMax = fadeTimeMill / 1000 * 60;

int fadeCnt = 0;

//�t�F�[�h�A�E�g
//�����l
int fadeOutCntInit = 0;
//�t�F�[�h�A�E�g�̃J�E���^
int fadeOutCnt = fadeOutCntInit;
//�t�F�[�h�A�E�g�̃J�E���^MAX
int fadeOutCntMax = fadeTimeMax;

//�t�F�[�h�C��
//�����l
int fadeInCntInit = fadeTimeMax;
//�t�F�[�h�A�E�g�̃J�E���^
int fadeInCnt = fadeInCntInit;
//�t�F�[�h�A�E�g�̃J�E���^
int fadeInCntMax = fadeTimeMax;

//��Alt+Shift+���h���b�O=��`�I��
// //�v���g�^�C�v�錾
//��ʂ�؂�ւ��鏈��
void ChangeScene(GAME_SCENE scene);
//�^�C�g�����
void Title();
//�^�C�g����� ����
void TitleProc();
//�^�C�g����� �`��
void TitleDraw();
//�v���C���
void Play();
//�v���C��� ����
void PlayProc();
//�v���C��� �`��
void PlayDraw();
//�G���h���
void End();
//�G���h��� ����
void EndProc();
//�G���h��� �`��
void EndDraw();
//�؂�ւ����
void Change();
//�؂�ւ���� ����
void ChangeProc();
//�؂�ւ���� �`��
void ChangeDraw();


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

	//�ŏ��̃V�[���̓^�C�g����ʂ���
	GameScene = GAME_SCENE_TITLE;

	while (1)
	{		
		//��ʂ���������
		if (ClearDrawScreen() != 0) { break; }
		//�L�[���͏�Ԃ��X�V����
		AllKeyUpdate();

		//FPS�l�̍X�V��AllKeyUpdate�Ȃǂ̈�A�̏������I����ɏ���
		FpsUpdate();
		
		//���b�Z�[�W���󂯎�葱����A�~�ȂǂŃE�B���h�E�������Ƃ�
		if (ProcessMessage() != 0) 
		{
			//�������[�v�𔲂���
			break;
		}

		//ESC�L�[�ŋ����I��
		if (KeyClick(KEY_INPUT_ESCAPE)) { break; }

		//�ȑO�̃V�[�����擾
		if (GameScene != GAME_SCENE_CHANGE) 
		{
			GameSceneOld = GameScene;
		}

		//��switch tab��enum��� tab��switch_on�̒���enum�̕ϐ������� enter
		switch (GameScene)
		{
			//�^�C�g�����
		case GAME_SCENE_TITLE:
			Title();
			break;

			//�v���C���
		case GAME_SCENE_PLAY:
			Play();
			break;

			//�G���h���
		case GAME_SCENE_END:
			End();
			break;

			//�؂�ւ����
		case GAME_SCENE_CHANGE:
			Change();
			break;
		default:
			break;
		}

		//�V�[����؂�ւ���
		if (GameSceneOld != GameScene)
		{
			//���݂̃V�[�����؂�ւ���ʂłȂ��Ƃ�
			if (GameScene!=GAME_SCENE_CHANGE) 
			{
				GameSceneNext = GameScene;		//���̃V�[����ۑ�
				GameScene = GAME_SCENE_CHANGE;	//��ʐ؂�ւ��V�[���ɕς���
			}
		}

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

		//FPS�l��`��
		FpsDraw(0,100,true);

		//FPS�l��҂�
		FpsWait();

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
		speed++;
		return speed;
	}
	else 
	{
		return max;
	}
}

/// <summary>
/// �V�[����؂�ւ���
/// </summary>
/// <param name="scene">�؂�ւ���̃V�[��</param>
void ChangeScene(GAME_SCENE scene)
{
	GameScene = scene;				//��ʐ؂�ւ�
	isFadeIn = false;				//�t�F�[�h�C���ɂ��Ȃ�
	isFadeOut = true;				//�t�F�[�h�A�E�g����	
}

/// <summary>
/// �^�C�g�����
/// </summary>
void Title() 
{
	//����
	TitleProc();
	//�`��
	TitleDraw();
}



//����
void TitleProc() 
{
	//�Q�[����ʂɐ؂�ւ��
	if (KeyClick(KEY_INPUT_RETURN)) 
	{
		//���̃V�[���̏������������ōs���Ɗy

		//�v���C��ʂɐ؂�ւ�
		ChangeScene(GAME_SCENE_PLAY);
	}
	

	return;
}

//�`��
void TitleDraw() 
{
	const char* string = "Title";
	DrawString(0, 50, string, GetColor(0, 0, 0));
	//�����C���[�T�O�͉��ɏ������������Q�[����ʂł͏�̃��C���[�ɕ\�������(photoshop��illustrator�Ƌt)
	return;
}

//�v���C���
void Play() 
{
	//����
	PlayProc();
	//�`��
	PlayDraw();
}

//����
void PlayProc() 
{
	//�Q�[����ʂɐ؂�ւ��
	if (KeyClick(KEY_INPUT_RETURN))
	{
		//���̃V�[���̏������������ōs���Ɗy

		//�v���C��ʂɐ؂�ւ�
		ChangeScene(GAME_SCENE_END);
	}

	return;
}

//�`��
void PlayDraw() 
{
	const char* string = "Play";
	DrawString(0, 50, string, GetColor(0, 0, 0));

	return;
}

//�G���h���
void End() 
{
	//����
	EndProc();
	//�`��
	EndDraw();
}

//����
void EndProc()
{
	//�Q�[����ʂɐ؂�ւ��
	if (KeyClick(KEY_INPUT_RETURN))
	{
		//���̃V�[���̏������������ōs���Ɗy

		//�v���C��ʂɐ؂�ւ�
		ChangeScene(GAME_SCENE_TITLE);
	}

	return;
}

//�`��
void EndDraw()
{
	const char* string = "End";
	DrawString(0, 50, string, GetColor(0, 0, 0));

	return;
}

//�؂�ւ����
void Change() 
{
	DrawString(0, 0, "�ǂݍ���",GetColor(0,0,0));

	//����
	ChangeProc();
	//�`��
	ChangeDraw();
}

//����
void ChangeProc()
{
	//�t�F�[�h�C��
	if (isFadeIn) 
	{
		if (fadeInCnt > fadeInCntMax) 
		{
			//�J�E���^�����炷
			fadeInCnt--;
		}
		else //�t�F�[�h�C���������I�������Ƃ�
		{
			//�J�E���^��������
			fadeInCnt = fadeInCntInit;
			isFadeIn = false;
		}
	}

	//�t�F�[�h�A�E�g
	if (isFadeOut) 
	{
		if (fadeOutCnt < fadeOutCntMax) 
		{
		//�J�E���^�𑝂₷
			fadeOutCnt++;
		}
		else 
		{
		//�J�E���^������
			fadeOutCnt = fadeOutCntInit;;
			isFadeOut = false;
		}
	}

	//�؂�ւ������I����
	if (isFadeIn == false && isFadeOut == false) 
	{
	//�t�F�[�h�C�����Ă��Ȃ��A���t�F�[�h�A�E�g�����Ă��Ȃ�
		//���̃V�[���ɐ؂�ւ�
		GameScene = GameSceneNext;
		//�ȑO�̃V�[���̍X�V
		GameSceneOld = GameScene;
	}

	return;
}

//�`��
void ChangeDraw()
{
	switch (GameSceneOld)
	{
	case GAME_SCENE_TITLE:
		TitleDraw();	//�^�C�g����ʂ̕`��
		break;
	case GAME_SCENE_PLAY:
		PlayDraw();		//�v���C�@��ʂ̕`��
		break;
	case GAME_SCENE_END:
		EndDraw();		//�G���h�@��ʂ̕`��
		break;
	default:
		break;
	}

	////0~255�܂�
	//SetDrawBlendMode(DX_BLENDMODE_ALPHA, 127);
	////�������̍���ʂ�`��
	//DrawBox(0, 0, GAME_WIDTH, GAME_HEIGHT, GetColor(0, 0, 0), true);
	////�u�����h���[�h���I��点��
	//SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//�t�F�[�h�C��
	if (isFadeIn) 
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)fadeInCnt / fadeInCntMax) * 255);
	}

	//�t�F�[�h�A�E�g
	if (isFadeOut) 
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)fadeOutCnt / fadeOutCntMax) * 255);
	}

	DrawBox(0, 0, GAME_WIDTH, GAME_HEIGHT, GetColor(0, 0, 0), true);
	
	//�l�p��`��
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawString(0, 0, "�؂�ւ�", GetColor(0, 0, 0));

	return;
}
