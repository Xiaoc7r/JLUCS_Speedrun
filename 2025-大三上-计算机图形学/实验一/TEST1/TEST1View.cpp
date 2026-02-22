
// TEST1View.cpp: CTEST1View 类的实现
#include "math.h"
#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "TEST1.h"
#endif

#include "TEST1Doc.h"
#include "TEST1View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTEST1View

IMPLEMENT_DYNCREATE(CTEST1View, CView)

BEGIN_MESSAGE_MAP(CTEST1View, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CTEST1View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
//	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

// CTEST1View 构造/析构

CTEST1View::CTEST1View() noexcept
{
	// TODO: 在此处添加构造代码
	m_pLMousedown = FALSE;

}

CTEST1View::~CTEST1View()
{
}

BOOL CTEST1View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CTEST1View 绘图

void CTEST1View::OnDraw(CDC* pDC/*pDC*/)
{
	//DDALine(pDC, 0, 0, 100, 100);
	// TODO: 在此处为本机数据添加绘制代码
	//pDC->MoveTo(0,0);
	//pDC->LineTo(100, 100);
	//pDC->Rectangle(0, 0, 100, 100);
	//pDC->Ellipse(0, 0, 100, 100);
}


// CTEST1View 打印


void CTEST1View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CTEST1View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CTEST1View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CTEST1View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CTEST1View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CTEST1View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CTEST1View 诊断

#ifdef _DEBUG
void CTEST1View::AssertValid() const
{
	CView::AssertValid();
}

void CTEST1View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTEST1Doc* CTEST1View::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTEST1Doc)));
	return (CTEST1Doc*)m_pDocument;
}
#endif //_DEBUG


// CTEST1View 消息处理程序
void CTEST1View::DDALine(CDC* pDC, int x1, int y1, int x2, int y2)
{
	double dx, dy,e, x, y;
	dx = x2 - x1;
	dy = y2 - y1;
	// 取dx和dy绝对值中的较大值作为循环次数e
	e = (fabs(dx) > fabs(dy)) ? fabs(dx) : fabs(dy);
	dx = dx / e;
	dy = dy / e;
	x = x1;
	y = y1;
	for (int i = 1; i <= e; i++)
	{
		pDC->SetPixel((int)(x + 0.5), (int)(y + 0.5), RGB(0, 0, 0));
		x += dx;
		y += dy;
	}
}


void CTEST1View::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_pLMousedown)
	{
		CClientDC dc(this);

		// 选入空画刷，实现空心效果
		CBrush* pOldBrush = (CBrush*)dc.SelectStockObject(NULL_BRUSH);
		// 创建红色画笔（椭圆用）
		CPen redPen(PS_SOLID, 2, RGB(255, 0, 0));
		CPen* pOldPen = dc.SelectObject(&redPen);

		// 关键修改：替换反色模式为R2_NOTXORPEN，支持画笔颜色
		dc.SetROP2(R2_NOTXORPEN);
		// 绘制红色空心椭圆
		dc.Ellipse(m_Newpoint.x, m_Newpoint.y, m_Oldpoint.x, m_Oldpoint.y);
		dc.Ellipse(m_Newpoint.x, m_Newpoint.y, point.x, point.y);

		int oldRadius = (int)sqrt(pow(m_Oldpoint.x - m_Newpoint.x, 2) + pow(m_Oldpoint.y - m_Newpoint.y, 2));
		CPoint oldRectStart(m_Newpoint.x - oldRadius, m_Newpoint.y - oldRadius);
		CPoint oldRectEnd(m_Newpoint.x + oldRadius, m_Newpoint.y + oldRadius);
		int newRadius = (int)sqrt(pow(point.x - m_Newpoint.x, 2) + pow(point.y - m_Newpoint.y, 2));
		CPoint newRectStart(m_Newpoint.x - newRadius, m_Newpoint.y - newRadius);
		CPoint newRectEnd(m_Newpoint.x + newRadius, m_Newpoint.y + newRadius);

		// 创建蓝色画笔（圆用）
		CPen bluePen(PS_SOLID, 2, RGB(0, 0, 255));
		dc.SelectObject(&bluePen);

		// 绘制蓝色空心圆
		dc.Ellipse(oldRectStart.x, oldRectStart.y, oldRectEnd.x, oldRectEnd.y);
		dc.Ellipse(newRectStart.x, newRectStart.y, newRectEnd.x, newRectEnd.y);

		// 恢复设备上下文资源
		dc.SelectObject(pOldPen);
		dc.SelectObject(pOldBrush);

		m_Oldpoint = point; //存放当前鼠标位置
	}
	CView::OnMouseMove(nFlags, point);
}

void CTEST1View::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_Oldpoint = point;//保存光标的当前位置
	m_Newpoint = point;//存放画线的起始位置
	m_pLMousedown = TRUE;//表示鼠标为按下的
	CView::OnLButtonDown(nFlags, point);
}

void CTEST1View::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_pLMousedown) {
		m_pLMousedown = FALSE;
	}
	CView::OnLButtonUp(nFlags, point);
}
