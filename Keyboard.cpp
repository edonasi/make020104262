#include "DxLib.h"
//�L�[�{�[�h�̃\�[�X�t�@�C��
#include "Keyboard.h"

//�O���[�o���ϐ�
KEYBOARD keyboard;

//�֐�

/// <summary>
/// �L�[���͏�Ԃ��X�V����
/// </summary>
void AllKeyUpdate(void) 
{
	//���O�̃L�[���͂�����Ă���
	for (int i = 0; i < KEY_KIND_MAX; i++) 
	{
		keyboard.oldKeyState[i] = keyboard.allKeyState[i];
	}

	//���ׂẴL�[�̓��͏�Ԃ𓾂�
	GetHitKeyStateAll(keyboard.tmpKeyState);

	//������Ă���L�[�̎��Ԃ��X�V����
	for (int i = 0; i < KEY_KIND_MAX; i++) 
	{
	//�L�[�R�[�h�������Ă���Ƃ�
		if (keyboard.tmpKeyState[i] != 0)
		{
			//�����Ă��鎞�Ԃ����Z
			keyboard.allKeyState[i]++;
		}
		else 
		{
			//�����Ă��鎞�Ԃ�0�ɖ߂�
			keyboard.allKeyState[i] = 0;
		}
	}

	return;
}

/// <summary>
/// �L�[�������Ă��邩�L�[�R�[�h�Ŕ��f����
/// </summary>
/// <param name="keyInput">�L�[�R�[�h</param>
/// <returns>�����Ă���Ȃ�true</returns>
bool KeyDown(int keyInput)
{
	if (keyboard.allKeyState[keyInput] != 0) 
	{
		//������
		return true;
	}
	else 
	{
		//�����ĂȂ�
		return false;
	}
}

/// <summary>
/// �L�[���������u�Ԃ��L�[�R�[�h�Ŕ��f����
/// </summary>
/// <param name="keyInput">�L�[�R�[�h</param>
/// <returns>�����Ă���Ȃ�true</returns>
bool Key1Down(int keyInput)
{
	if (keyboard.allKeyState[keyInput] == 1)
	{
		//������
		return true;
	}
	else
	{
		//�����ĂȂ�
		return false;
	}
}

/// <summary>
/// �L�[���グ�Ă��邩�L�[�R�[�h�Ŕ��f����
/// </summary>
/// <param name="keyInput">�L�[�R�[�h</param>
/// <returns>�グ�Ă���Ȃ�true</returns>
bool KeyUp(int keyInput)
{
	//�ȑO�͉�����
	if (keyboard.oldKeyState[keyInput] != 0		
		//���݂͉����ĂȂ�
		&& keyboard.allKeyState[keyInput] == 0)	
	{
		//�グ��
		return true;
	}
	else
	{
		//�グ�Ă��Ȃ�
		return false;
	}
}

bool KeyUpping(int keyInput) 
{
	//�ȑO�͉�����
	if (keyboard.allKeyState[keyInput] ==0)
	{
		//�グ��
		return true;
	}
	else
	{
		//�グ�Ă��Ȃ�
		return false;
	}
}

/// <summary>
/// �L�[���N���b�N������
/// </summary>
/// <param name="keyInput">�L�[�R�[�h</param>
/// <returns>�N���b�N������truetrue</returns>
bool KeyClick(int keyInput)
{
	//�ȑO�͉�����
	if (keyboard.oldKeyState[keyInput] != 0		
		//���݂͉����ĂȂ�
		&& keyboard.allKeyState[keyInput] == 0)	
	{
		//�N���b�N����
		return true;
	}
	else
	{
		//�N���b�N���ĂȂ�
		return false;
	}
}

/// <summary>
/// �L�[�����������Ă��邩
/// </summary>
/// <param name="keyInput">�L�[�R�[�h</param>
/// <param name="milliTime">�L�[�R�[�h�����������Ă���~���b</param>
/// <returns></returns>
bool KeyDownKeep(int keyInput, int milliTime)
{
	//1�b = 1000�~���b
	float milliSec = 1000.0f;

	//���������Ă��鎞�Ԃ̓~���b���~FPS�l
	//��F1500�~���b���� / 1000�~�� = 1.5�b�~60FPS = 90(keyTime)
	int updateTime = (milliTime / milliSec) * 60;

	if (keyboard.allKeyState[keyInput] > updateTime)
	{
		//���������Ă���
		return true;
	}
	else 
	{
		//���������Ă��Ȃ�
		return false;
	}
}