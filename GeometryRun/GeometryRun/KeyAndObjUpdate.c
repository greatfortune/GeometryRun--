#include "KeyAndObjUpdate.h"

Status GetWinMaxMinXY()
{
	// ==========================================================================================
	// 获取窗口四条边的坐标，当窗口发生移动或缩放，以下值会发生变化
	// ==========================================================================================
	winMaxX = AEGfxGetWinMaxX();
	winMaxY = AEGfxGetWinMaxY();
	winMinX = AEGfxGetWinMinX();
	winMinY = AEGfxGetWinMinY();
	return OK;
}

Status KeyUpdate()
{
	// 状态切换
	if (KeyPressed[KeyR] == TRUE)
	{
		Next = GS_Restart;
		return FLAG_IMPORTANTKEY;
	}
	if (KeyPressed[KeyESC] == TRUE)
	{
		Next = GS_Quit;
		return FLAG_IMPORTANTKEY;
	}
	if (KeyPressed[Key1] == TRUE)
	{
		Next = GS_L1;
		return FLAG_IMPORTANTKEY;
	}
	if (KeyPressed[Key2] == TRUE)
	{
		Next = GS_L2;
		return FLAG_IMPORTANTKEY;
	}

	// 画面播放完毕/点击鼠标左键 进入Level1
	if (Current == GS_L0)
	{
		KeyPressed[KeyLButton] = GetKeyState(VK_LBUTTON);
		if (KeyPressed[KeyLButton] < 0 || xcurrentFrame>11)
		{
			Next = GS_L1;
			return FLAG_IMPORTANTKEY;
		}
	}
	return OK;
}

//更新对象
Status ObjUpdate(insNode* pinsNode)
{
	baseNode *pBaseNode;
	GameObjList *pL;
	for (pBaseNode = theBaseList->head->next; pBaseNode != theBaseList->tail; pBaseNode = pBaseNode->next)
	{
		pL = pBaseNode->gameobj_list;
		switch (pBaseNode->gameobj_base.type)
		{
			case OTYPE_PLAYER:
				ListTraverse(pL, PlayerUpdate);
				break;
			case OTYPE_BACKGROUND:
				ListTraverse(pL, BackGroundUpdate);
				break;
			case OTYPE_PLATFORM:
				ListTraverse(pL, PlatformUpdate);
				break;
			case OTYPE_MONSTER:
				ListTraverse(pL, MonsterUpdate);
				break;
			case OTYPE_BLOCK:
				ListTraverse(pL, BlockUpdate);
				break;
			case OTYPE_BULLET:
				ListTraverse(pL, BulletUpdate);
				break;
			case OTYPE_BOSS2:
				ListTraverse(pL, Boss2Update);
				break;
			default:
			break;
		}
	}
}

//对象碰撞检测
static Status Visit_CollisionDetectAnother(insNode* pinsNode)
{
	GameObj* pInstOther = &(pinsNode->gameobj);
	// 跳过非活动对象和主对象自身
	if (pInstOther->flag == FLAG_INACTIVE || (pInstOther->pObject->type == pInstForCollisionDetect->pObject->type))
		return OK;
	switch (pInstForCollisionDetect->pObject->type)
	{
		case OTYPE_PLAYER:
			PlayerCollision(pinsNode);
			break;
		case OTYPE_BULLET:
			BulletCollision(pinsNode);
			break;
		
		default:
			break;
	}

	return OK;
}

Status Visit_CollisionDetect(insNode* pinsNode)
{
	pInstForCollisionDetect = &(pinsNode->gameobj);

	// 不处理非活动对象
	if (pInstForCollisionDetect->flag == FLAG_INACTIVE)
		return OK;

	// Player 与 background / block 的碰撞检测
	BaseListTraverse(theBaseList, Visit_CollisionDetectAnother);
	return OK;
}
 
Status Visit_Matrix2DCount(insNode* pinsNode)
{
	GameObj* pInst = &(pinsNode->gameobj);
	Matrix2D		 trans, rot, scale;

	// 不处理非活动对象
	if ((pInst->flag & FLAG_ACTIVE) == 0)
		return OK;

	// 缩放矩阵
	if (pInst->pObject->type == OTYPE_PLAYER)
		Matrix2DScale(&scale, pInst->scale, pInst->scale * pInst->properties->value);
	else
		Matrix2DScale(&scale, pInst->scale, pInst->scale);
	// 旋转矩阵
	Matrix2DRotDeg(&rot, pInst->dirCurr);
	// 平移矩阵
	Matrix2DTranslate(&trans, pInst->posCurr.x, pInst->posCurr.y);
	// 以正确的顺序连乘以上3个矩阵形成2维变换矩阵
	Matrix2DConcat(&(pInst->transform), &trans, &rot);
	Matrix2DConcat(&(pInst->transform), &(pInst->transform), &scale);
	return OK;
}

