
#include "BfMouseMoveStatic.h"


CBfMouseMoveStatic::CBfMouseMoveStatic(void)
{
}

CBfMouseMoveStatic::~CBfMouseMoveStatic(void)
{
}

void CBfMouseMoveStatic::OnMouseHover(CPoint point)
{
	// 发送消息
	if (m_MsgInfo.wParam > 0)
		m_pMsgCtrl->OnDirectUiWindowMsgProc(DWM_MOUSEHOVER, m_MsgInfo);
}

void CBfMouseMoveStatic::OnMouseLeave(CPoint point)
{
	// 发送消息
	if (m_MsgInfo.wParam > 0)
		m_pMsgCtrl->OnDirectUiWindowMsgProc(DWM_MOUSELEAVE, m_MsgInfo);
}
