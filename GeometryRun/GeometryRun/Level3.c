/**
 * Project:		GameStateManager
 * File Name:	Level3.c
 * Author:		���������
 * Date:		2017-4-22
 * Purpose:		�ؿ�3
 */
#include "Level3.h"

clock_t timeStart_level3;
const float bossOccurTime_level3 = 15.0f;

void Load3(void)
{
	// ��ʼ��ʱ��ϵͳ
	TimerIni(&timeStart_level3);

	printf("Level3: Load\n");
	theBaseList = NULL;
	// ���ó���
	SetConstants();
	// ��ʼ����Ϸ��������ʵ���б�
	InitialGameObjBaseList(&theBaseList);

	PlayerLoad();
	BlockLoad();
	BulletLoad();
	MonsterLoad();
	AIMonsterLoad();
	PlatformLoad();
	Boss2Load();
	BackGroundLoad();
	PauseLoad();
}

void Ini3(void)
{
	// ���õ�ǰ�ؿ���һЩ����ֵ
	float BlockVel = -400.0f;
	float MonsterVel = -400.0f;
	int bossHP = 30;

	printf("Level3: Ini\n");
	// ��ȡ��ǰ�ؿ�ʱ��
	timeStart_level3 = clock();

	PlayerStart();
	BlockStart(BlockVel);
	PlatformStart();
	BackGroundStart();
	MonsterStart(MonsterVel);
	AIMonsterStart(MonsterVel);
	BulletStart();
	Boss2Start(bossHP);
	PauseStart();

	// ����ʵ������
	pHero = CreateGameObj(OTYPE_PLAYER, SIZE_HERO, iniPosition_Player, zero, 0, theBaseList, 0, NULL);
	CreateGameObj(OTYPE_BACKGROUND, SIZE_BACKGROUND, iniPosition_Background, iniVelocity_Background, 0, theBaseList, 0, NULL);
	CreateGameObj(OTYPE_PLATFORM, SIZE_PLATFORM, iniPosition_Platform, iniVelocity_Platform, 0, theBaseList, 0, NULL);

	CreateObjInMap0(1.0f);
	CreateObjInMap7(9.0f);
	CreateBossInMap_Level2(bossOccurTime_level3);
}

void Update3(void)
{
	Vector2D iniPosition_Block;
	Vector2D iniVelocity_Block;

	GetWinMaxMinXY();

	// =========================
	// ��Ϸ�߼���Ӧ����
	// =========================
	KeyUpdate();

	if (endPause)
	{
		// ���¼�������ͣ�ӳٵ�ʱ��
		timeStart_level3 += pauseEndTime - pauseStartTime;
		endPause = FALSE;
		GameObjDelete(pPause);
		pauseCreated = FALSE;
	}

	if (isPaused)
	{
		if (!pauseCreated)
		{
			pPause = CreateGameObj(OTYPE_PAUSE, SIZE_PAUSE, iniPosition_Pause, zero, 0, theBaseList, 0, NULL);
			pauseCreated = TRUE;
		}
	}
	else
	{
		TimerUpdate(timeStart_level3);
		// ���¶���
		ObjUpdate();

		// ====================
		// ��ײ���
		// ====================
		BaseListTraverse(Visit_CollisionDetect);

	}
	// =====================================
	// �������ж����2D�任����
	// =====================================
	BaseListTraverse(Visit_Matrix2DCount);
}

void Draw3(void)
{
	// Ϊ��ʼ��������׼��
	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
	AEGfxSetBlendMode(AE_GFX_RM_TEXTURE);
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

	// ������ƶ����б��е����ж���
	BaseListTraverse(Visit_DrawObj);

}

void Free3(void)
{

	printf("Level3: free\n");
	// ʹ�ú���gameObjDestroyɾ���б��еĶ���
	BaseListTraverse(Visit_DestroyObj);
	TimerFree();
}

void Unload3(void)
{
	printf("Level3: Unload\n");
	DestroyGameObjBaseList(&theBaseList);
}