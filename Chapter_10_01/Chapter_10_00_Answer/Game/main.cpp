#include "stdafx.h"
#include "system/system.h"
#include "Game.h"

using namespace std;


///////////////////////////////////////////////////////////////////
// �E�B���h�E�v���O�����̃��C���֐��B
///////////////////////////////////////////////////////////////////
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	//�Q�[���̏������B
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, "Game");
	
	//�Q�[���N���X�̃C���X�^���X���`����B
	Game game;

	//�Q�[�����[�v�B
	while (DispatchWindowMessage() == true)
	{
		//�Q�[���̍X�V���������s�B
		game.Update();
		//�Q�[���̕`�揈�������s�B
		game.Render();
	}
}