//FPS�\�[�X�t�@�C��

//�w�b�_�t�@�C���̓ǂݍ���
#include "DxLib.h"
#include "Fps.h"

//�O���[�o���ϐ�
FPS fps;	//fps�̊Ǘ�
float divDeltaTime = 1000000.0f;	//deltaTime�����鐔(1000000.0f)

//�֐�

/// <summary>
/// Fps�l�𑪒肵�A�l���X�V����
/// </summary>
/// <param name=""></param>
void FpsUpdate(void) 
{
	//��ԍŏ��̏���
	if (fps.IsInitFlag == false) 
	{
		//����J�n�������ʕb�P�ʂŎ擾
		fps.StartTime = GetNowHiPerformanceCount();	//Window���N�����Ă���o�߂�������(�}�C�N���b)

		//�ŏ��̏����I��
		fps.IsInitFlag = true;
	}

	//���݂̎������ʕb�P�ʂŎ擾
	fps.NowTime = GetNowHiPerformanceCount();

	//�O��擾�������Ԃ���̌o�ߎ��Ԃ�b��(����)�ɕϊ����Ă���Z�b�g
	fps.DeltaTime = (fps.NowTime - fps.OldTime) / divDeltaTime;

	//����擾�������Ԃ�ۑ�
	fps.OldTime = fps.NowTime;

	//1�t���[���ځ`FPS�ݒ�l�܂ł̓J�E���g�A�b�v
	if (fps.Count < fps.SampleRote) 
	{
		//�J�E���^�𑝂₷
		fps.Count++;
	}
	else	//FPS�ݒ�l�t���[���ŕ���FPS���v�Z
	{
		//���݂̎�������0�t���[��mw�̎��Ԃ��Ђ�FPS�̐��l�Ŋ���
		fps.DrawValue =
			divDeltaTime / ((fps.NowTime - fps.StartTime) / static_cast<float>(fps.SampleRote));
	
		//����J�n�������}�C�N���b�P�ʂŎ擾
		fps.StartTime = GetNowHiPerformanceCount();	//Windows���N�����Ă���o�߂�������

		//�J�E���^������
		fps.Count = 1;
	}

	return;
}

/// <summary>
/// �f�o�b�N�p�̕���FPS�l��`�悷��
/// </summary>
/// <param name="draw_x">�`�悷��x�ʒu</param>
/// <param name="draw_y">�`�悷��y�ʒu</param>
/// <param name="isBlackColor">�`��F�����ɂ��邩</param>
void FpsDraw(int draw_x,int draw_y,bool isBlackColor) 
{
	//�f�o�b�N���[�h�Ȃ�`�悷��
	if (GAME_DEBUG) 
	{
		//�`��F(�����͔�)
		int drawColor = 255;
		//�����ō��ɂ��閽�߂��o����`��F�����ɂ���
		if (isBlackColor)
		{
			drawColor = 0;
		}

		//�������`��
		DrawFormatString(draw_x, draw_y, GetColor(drawColor, drawColor, drawColor),
			"FPS : %.1f", fps.DrawValue);

	}
	
	return;
}

/// <summary>
/// FPS�ŏ�����҂�
/// </summary>
void FpsWait() 
{
	//���݂̎���-�ŏ��̎����ŁA���݂������Ă��鎞�����擾����
	LONGLONG resultTime = fps.NowTime - fps.StartTime;

	//�҂ׂ��~���b��(1�b/FPS�l�����݂̃t���[����)����A���݂������Ă��鎞��������
	LONGLONG waitTime = divDeltaTime / fps.Value * fps.Count - resultTime;

	//�~���b�ɕϊ������銄�鐔
	int millTime = 1000.0f;
	//�}�C�N���b����~���b�ɕϊ�
	waitTime /= millTime;

	//����������������A������҂�
	if (waitTime > 0) 
	{
		WaitTimer(waitTime);	//�����~���b�҂�(��WindowProc�Œ�~����̂œr���ŃC�x���g���N�����Ƃ����̏������s��)
	}

	//����������OFF�ɂ��Ă��邩
	if (GetWaitVSyncFlag() == false) 
	{
		//FPS���ő�l�ł͂Ȃ��Ƃ�
		if (fps.Value < GAME_FPS_MAX) 
		{
			//1�b����FPS�l�����A�҂��Ԃ��������Ƃ��́A������FPS�l���グ�Ă��悢
			//�҂���10�~�� <= 1�b/60FPS=16.6666�~���@�������������ł���
			if (waitTime > 0
				&& waitTime <= millTime / fps.Value)
			{
				fps.Value++;
			}
			else 
			{
				//FPS�l���ǂ����Ă��炸�A�x���Ƃ��́AFPS�l��������
				if (fps.Value > GAME_FPS_MIN) 
				{
					fps.Value--;
				}
			}
		}
	}

	return;
}