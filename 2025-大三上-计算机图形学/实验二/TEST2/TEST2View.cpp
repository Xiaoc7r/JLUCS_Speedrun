
// TEST2View.cpp: CTEST2View 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "TEST2.h"
#endif

#include "TEST2Doc.h"
#include "TEST2View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTEST2View

IMPLEMENT_DYNCREATE(CTEST2View, CView)

BEGIN_MESSAGE_MAP(CTEST2View, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CTEST2View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()

// CTEST2View 构造/析构

CTEST2View::CTEST2View() noexcept
{
	// TODO: 在此处添加构造代码
	ptArray.RemoveAll();
	m_nCount = 0;

}

CTEST2View::~CTEST2View()
{
}

BOOL CTEST2View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CTEST2View 绘图

void CTEST2View::OnDraw(CDC* /*pDC*/)
{
	CTEST2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CTEST2View 打印


void CTEST2View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CTEST2View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CTEST2View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CTEST2View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CTEST2View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CTEST2View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CTEST2View 诊断

#ifdef _DEBUG
void CTEST2View::AssertValid() const
{
	CView::AssertValid();
}

void CTEST2View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTEST2Doc* CTEST2View::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTEST2Doc)));
	return (CTEST2Doc*)m_pDocument;
}
#endif //_DEBUG


// CTEST2View 消息处理程序

void CTEST2View::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_nCount > 0)
	{
		CClientDC dc(this);
		dc.SetROP2(R2_NOT); //设置绘图模式，以屏幕颜色的相反色绘图
		//以重绘的方式擦除前一个OnMouseMove绘制的直线
		dc.MoveTo(ptArray[m_nCount - 1].x, ptArray[m_nCount - 1].y);
		dc.LineTo(m_Newpoint);
		dc.MoveTo(ptArray[m_nCount - 1].x, ptArray[m_nCount - 1].y); //重新位置到当前位置画一条直线
		dc.LineTo(point);
		m_Newpoint = point; //存放当前鼠标位置
	}
	CView::OnMouseMove(nFlags, point);
}

void CTEST2View::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	ptArray.Add(point);
	m_Newpoint = point;
	m_nCount++;
	CView::OnLButtonDown(nFlags, point);
}

void CTEST2View::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CView::OnLButtonUp(nFlags, point);
}

void CTEST2View::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CClientDC dc(this);
	CPoint* p;
	CBitmap bm;
	CBrush brush, * oldbrush;
	bm.LoadBitmapW(IDB_BITMAP1);
	brush.CreatePatternBrush(&bm);
	ptArray.Add(point);;
	m_Newpoint = point;
	m_nCount++;

	int sz = ptArray.GetSize();
	p = new CPoint[sz];
	for (int i = 0; i < sz; i++)
	{
		p[i] = ptArray.GetAt(i);
	}
	oldbrush = dc.SelectObject(&brush);
	CPen pen(PS_SOLID, 1, RGB(0, 0, 255)); // 创建蓝色画笔（RGB(0,0,255)为蓝色）
	CPen* oldPen = dc.SelectObject(&pen);  // 选入设备上下文
	dc.Polygon(p, sz);
	dc.SelectObject(oldPen);
	dc.SelectObject(oldbrush);
	ptArray.RemoveAll();
	m_nCount = 0;
	CView::OnRButtonDown(nFlags, point);
}
