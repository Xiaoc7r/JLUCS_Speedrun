
// MFCApplication4View.cpp: CMFCApplication4View 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "MFCApplication4.h"
#endif

#include "MFCApplication4Doc.h"
#include "MFCApplication4View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCApplication4View

IMPLEMENT_DYNCREATE(CMFCApplication4View, CView)

BEGIN_MESSAGE_MAP(CMFCApplication4View, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMFCApplication4View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// CMFCApplication4View 构造/析构

CMFCApplication4View::CMFCApplication4View() noexcept
{
	// TODO: 在此处添加构造代码
	m_LBDown = FALSE;
	m_RBDown = FALSE;
	m_Num_Ply = 0;
	m_Num_Ply1 = 0;
	m_LTPoint = m_RBPoint = CPoint(0, 0);

	m_Num_Ply = 0;
	m_NumCliped_Ply = 0;

}

CMFCApplication4View::~CMFCApplication4View()
{
}

BOOL CMFCApplication4View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CMFCApplication4View 绘图

void CMFCApplication4View::OnDraw(CDC* /*pDC*/)
{
	CMFCApplication4Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CMFCApplication4View 打印


void CMFCApplication4View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMFCApplication4View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CMFCApplication4View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CMFCApplication4View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CMFCApplication4View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMFCApplication4View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CMFCApplication4View 诊断

#ifdef _DEBUG
void CMFCApplication4View::AssertValid() const
{
	CView::AssertValid();
}

void CMFCApplication4View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFCApplication4Doc* CMFCApplication4View::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCApplication4Doc)));
	return (CMFCApplication4Doc*)m_pDocument;
}
#endif //_DEBUG


// CMFCApplication4View 消息处理程序

void CMFCApplication4View::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	ClearScreen();
	m_Num_Ply = 0;
	m_Num_Ply1 = 0;
	m_LBDown = TRUE;
	m_LTPoint = point;

	CView::OnLButtonDown(nFlags, point);
}

void CMFCApplication4View::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_LBDown = FALSE;
	CorrectWindow();//

	CView::OnLButtonUp(nFlags, point);
}

void CMFCApplication4View::OnMButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_Num_Ply > 0)
	{
		ShowLineSeg(m_Polygon[m_Num_Ply - 1], m_Polygon[m_Num_Ply]);
		ShowLineSeg(m_Polygon[0], m_Polygon[m_Num_Ply]);
	}
	else
	{
		//MessageBox("错误，退出!");
		exit(0);
	}
	Sutherland_Hodgman();

	ClearScreen();
	CClientDC dc(this);

	dc.Rectangle(m_LTPoint.x, m_LTPoint.y, m_RBPoint.x, m_RBPoint.y);
	if (m_Num_Ply > 0) {
		dc.MoveTo(m_Polygon[m_Num_Ply - 1]);

		dc.LineTo(m_Polygon[0]);
	}
	dc.SelectStockObject(GRAY_BRUSH);
	dc.Polygon(m_Polygon1, m_Num_Ply1);
	dc.SelectStockObject(BLACK_BRUSH);
	dc.Polygon(m_Polygon, m_Num_Ply);
	//for(int i = 1; i < m_Num_Ply; i++)
	//{
		//dc.MoveTo(m_Polygon[i - 1]);
		//dc.LineTo(m_Polygon[i]);
	//}
	m_Num_Ply = 0;
	m_Num_Ply1 = 0;
	CView::OnRButtonDblClk(nFlags, point);

	CView::OnMButtonDown(nFlags, point);
}

void CMFCApplication4View::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_Polygon[m_Num_Ply] = point;
	m_Polygon1[m_Num_Ply1] = point;
	if (m_Num_Ply > 0)
		ShowLineSeg(m_Polygon[m_Num_Ply - 1], m_Polygon[m_Num_Ply]);
	m_Num_Ply++;
	m_Num_Ply1++;

	CView::OnRButtonDown(nFlags, point);
}

void CMFCApplication4View::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CClientDC dc(this);
	if (m_LBDown)
	{
		ClearScreen();
		m_RBPoint = point;
		dc.Rectangle(m_LTPoint.x, m_LTPoint.y, m_RBPoint.x, m_RBPoint.y);
	}

	CView::OnMouseMove(nFlags, point);
}

void CMFCApplication4View::ClearScreen()
{

	CClientDC dc(this);
	CRect window;
	GetClientRect(window);
	dc.SelectStockObject(WHITE_PEN);
	dc.SelectStockObject(WHITE_BRUSH);
	dc.Rectangle(window);

}

void CMFCApplication4View::CorrectWindow()
{
	int x, y;
	if (m_LTPoint.x > m_RBPoint.x)
	{
		x = m_LTPoint.x;
		m_LTPoint.x = m_RBPoint.x;
		m_RBPoint.x = x;
	}
	if (m_LTPoint.y < m_RBPoint.y)
	{
		y = m_LTPoint.y;
		m_LTPoint.y = m_RBPoint.y;
		m_RBPoint.y = y;
	}
}

void CMFCApplication4View::ShowLineSeg(CPoint BPoint, CPoint EPoint)
{
	CClientDC dc(this);
	dc.SetROP2(R2_NOT);
	dc.MoveTo(BPoint);
	dc.LineTo(EPoint);
}

void CMFCApplication4View::Sutherland_Hodgman()
{
	m_NumCliped_Ply = CutByLeft(m_Polygon, m_Num_Ply, m_ClipedPly);
	m_Num_Ply = CutByRight(m_ClipedPly, m_NumCliped_Ply, m_Polygon);
	m_NumCliped_Ply = CutByBottom(m_Polygon, m_Num_Ply, m_ClipedPly);
	m_Num_Ply = CutByTop(m_ClipedPly, m_NumCliped_Ply, m_Polygon);
}

int CMFCApplication4View::CutByBottom(CPoint* source, int sLength, CPoint* destin)
{
	int yBottom = m_RBPoint.y;

	int pre_Index = sLength - 1;
	int cur_Index = 0;
	CPoint S = source[pre_Index];
	CPoint P = source[cur_Index];
	int   newIndex = 0;

	BOOL Flag_Outside = FALSE;


	for (int i = 0; i < sLength; i++)
	{
		if (S.y < yBottom)
			Flag_Outside = TRUE;
		if (P.y >= yBottom)
		{
			if (Flag_Outside)
			{
				Flag_Outside = FALSE;
				destin[newIndex].y = yBottom;
				destin[newIndex].x = S.x + (double)(yBottom - S.y) / (double)(P.y - S.y) * (P.x - S.x);
				newIndex++;
			}
			destin[newIndex] = P;
			newIndex++;
		}
		else
		{
			if (!Flag_Outside)
			{
				Flag_Outside = TRUE;
				destin[newIndex].y = yBottom;
				destin[newIndex].x = S.x + (double)(yBottom - S.y) / (double)(P.y - S.y) * (P.x - S.x);
				newIndex++;
			}
		}

		S = P;
		UpdateIndex(pre_Index, sLength);
		UpdateIndex(cur_Index, sLength);
		P = source[cur_Index];
	}

	return newIndex;
}

int CMFCApplication4View::CutByTop(CPoint* source, int sLength, CPoint* destin)
{
	int yTop = m_LTPoint.y;

	int pre_Index = sLength - 1;
	int cur_Index = 0;
	CPoint S = source[pre_Index];
	CPoint P = source[cur_Index];
	int   newIndex = 0;

	BOOL Flag_Outside = FALSE;


	for (int i = 0; i < sLength; i++)
	{
		if (S.y > yTop)
			Flag_Outside = TRUE;
		if (P.y <= yTop)
		{
			if (Flag_Outside)
			{
				Flag_Outside = FALSE;
				destin[newIndex].y = yTop;
				destin[newIndex].x = S.x + (double)(yTop - S.y) / (double)(P.y - S.y) * (P.x - S.x);
				newIndex++;
			}
			destin[newIndex] = P;
			newIndex++;
		}
		else
		{
			if (!Flag_Outside)
			{
				Flag_Outside = TRUE;
				destin[newIndex].y = yTop;
				destin[newIndex].x = S.x + (double)(yTop - S.y) / (double)(P.y - S.y) * (P.x - S.x);
				newIndex++;
			}
		}

		S = P;
		UpdateIndex(pre_Index, sLength);
		UpdateIndex(cur_Index, sLength);
		P = source[cur_Index];
	}

	return newIndex;
}

int CMFCApplication4View::CutByLeft(CPoint* source, int sLength, CPoint* destin)
{
	int xLeft = m_LTPoint.x;

	int pre_Index = m_Num_Ply - 1;
	int cur_Index = 0;
	CPoint S = source[pre_Index];
	CPoint P = source[cur_Index];
	int   newIndex = 0;

	BOOL Flag_Outside = FALSE;


	for (int i = 0; i < sLength; i++)
	{
		if (S.x < xLeft)
			Flag_Outside = TRUE;
		if (P.x >= xLeft)
		{
			if (Flag_Outside)
			{
				Flag_Outside = FALSE;
				destin[newIndex].x = xLeft;
				destin[newIndex].y = S.y + (double)(xLeft - S.x) / (double)(P.x - S.x) * (P.y - S.y);
				newIndex++;
			}
			destin[newIndex] = P;
			newIndex++;
		}
		else
		{
			if (!Flag_Outside)
			{
				Flag_Outside = TRUE;
				destin[newIndex].x = xLeft;
				destin[newIndex].y = S.y + (double)(xLeft - S.x) / (double)(P.x - S.x) * (P.y - S.y);
				newIndex++;
			}
		}

		S = P;
		UpdateIndex(pre_Index, sLength);
		UpdateIndex(cur_Index, sLength);
		P = source[cur_Index];
	}

	return newIndex;
}

int CMFCApplication4View::CutByRight(CPoint* source, int sLength, CPoint* destin)
{
	int xRight = m_RBPoint.x;

	int pre_Index = sLength - 1;
	int cur_Index = 0;
	CPoint S = source[pre_Index];
	CPoint P = source[cur_Index];
	int   newIndex = 0;

	BOOL Flag_Outside = FALSE;


	for (int i = 0; i < sLength; i++)
	{
		if (S.x > xRight)
			Flag_Outside = TRUE;
		if (P.x <= xRight)
		{
			if (Flag_Outside)
			{
				Flag_Outside = FALSE;
				destin[newIndex].x = xRight;
				destin[newIndex].y = S.y + (double)(xRight - S.x) / (double)(P.x - S.x) * (P.y - S.y);
				newIndex++;
			}
			destin[newIndex] = P;
			newIndex++;
		}
		else
		{
			if (!Flag_Outside)
			{
				Flag_Outside = TRUE;
				destin[newIndex].x = xRight;
				destin[newIndex].y = S.y + (double)(xRight - S.x) / (double)(P.x - S.x) * (P.y - S.y);
				newIndex++;
			}
		}

		S = P;
		UpdateIndex(pre_Index, sLength);
		UpdateIndex(cur_Index, sLength);
		P = source[cur_Index];
	}

	return newIndex;
}

void CMFCApplication4View::UpdateIndex(int& Index, int Length)
{
	Index++;
	Index = Index % Length;
}

