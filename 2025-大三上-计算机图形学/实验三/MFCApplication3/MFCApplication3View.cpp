// MFCApplication3View.cpp: CMFCApplication3View 类的实现
//

#include "pch.h"
#include "framework.h"
#ifndef SHARED_HANDLERS
#include "MFCApplication3.h"
#endif

#include "MFCApplication3Doc.h"
#include "MFCApplication3View.h"

#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define PI 3.1415926
#define DEGREE (((double)PI) / 180)

// CMFCApplication3View

IMPLEMENT_DYNCREATE(CMFCApplication3View, CView)

BEGIN_MESSAGE_MAP(CMFCApplication3View, CView)
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMFCApplication3View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_KEYDOWN()
	// 变换类型菜单
	ON_COMMAND(ID_TRANSLATION, &CMFCApplication3View::OnTranslation)
	ON_COMMAND(ID_ROTATION, &CMFCApplication3View::OnRotation)
	ON_COMMAND(ID_SCALE, &CMFCApplication3View::OnScale)
	// 投影类型菜单
	ON_COMMAND(ID_PROJ_PERSPECTIVE, &CMFCApplication3View::OnProjPerspective)
	ON_COMMAND(ID_PROJ_OBLIQUE, &CMFCApplication3View::OnProjOblique)
END_MESSAGE_MAP()

// CMFCApplication3View 构造/析构

CMFCApplication3View::CMFCApplication3View() noexcept
{
	m_nType = 0;      // 默认平移模式
	m_nProjType = 0;  // 默认透视投影
}

CMFCApplication3View::~CMFCApplication3View()
{
}

BOOL CMFCApplication3View::PreCreateWindow(CREATESTRUCT& cs)
{
	return CView::PreCreateWindow(cs);
}

// CMFCApplication3View 绘图

void CMFCApplication3View::OnDraw(CDC* /*pDC*/)
{
	drawCube(&cube);
}

void CMFCApplication3View::drawCube(Cube* cube)
{
	int i, j;
	POINT3 cent = cube->viewPoint;
	POINT3* verts = cube->vertices;
	POINT pts[8];
	CClientDC dc(this);

	// 计算所有顶点的2D投影坐标
	for (i = 0; i < 8; ++i) {
		if (m_nProjType == PROJ_PERSPECTIVE) {
			pts[i] = POINT3::perspectiveProjection1P2D(verts[i], cent);
		}
		else {
			pts[i] = POINT3::obliqueProjection2D(verts[i]);
		}
	}

	// 立方体的12条边（顶点索引对）
	int edges[12][2] = {
		{0,1}, {1,2}, {2,3}, {3,0},  // 底面4条边
		{4,5}, {5,6}, {6,7}, {7,4},  // 顶面4条边
		{0,4}, {1,5}, {2,6}, {3,7}   // 连接边4条
	};

	// 画12条边
	CPen pen(PS_SOLID, 2, RGB(0, 0, 0));  // 黑色线条
	CPen* pOldPen = dc.SelectObject(&pen);

	for (i = 0; i < 12; ++i) {
		dc.MoveTo(pts[edges[i][0]]);
		dc.LineTo(pts[edges[i][1]]);
	}

	dc.SelectObject(pOldPen);
}

// CMFCApplication3View 打印

void CMFCApplication3View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMFCApplication3View::OnPreparePrinting(CPrintInfo* pInfo)
{
	return DoPreparePrinting(pInfo);
}

void CMFCApplication3View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

void CMFCApplication3View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

void CMFCApplication3View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMFCApplication3View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}

// CMFCApplication3View 诊断

#ifdef _DEBUG
void CMFCApplication3View::AssertValid() const
{
	CView::AssertValid();
}

void CMFCApplication3View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFCApplication3Doc* CMFCApplication3View::GetDocument() const
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCApplication3Doc)));
	return (CMFCApplication3Doc*)m_pDocument;
}
#endif //_DEBUG



void CMFCApplication3View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// 平移模式
	if (m_nType == TRANSFORM_TRANSLATE) {
		if (nChar == VK_UP)    cube.translate(Cube::X, -10);
		if (nChar == VK_DOWN)  cube.translate(Cube::X, 10);
		if (nChar == VK_LEFT)  cube.translate(Cube::Y, -10);
		if (nChar == VK_RIGHT) cube.translate(Cube::Y, 10);
		if (nChar == VK_HOME)  cube.translate(Cube::Z, -10);
		if (nChar == VK_END)   cube.translate(Cube::Z, 10);
	}
	// 旋转模式
	else if (m_nType == TRANSFORM_ROTATE) {
		if (nChar == VK_UP)    cube.rotate(Cube::X, -DEGREE * 10);
		if (nChar == VK_DOWN)  cube.rotate(Cube::X, DEGREE * 10);
		if (nChar == VK_LEFT)  cube.rotate(Cube::Y, DEGREE * 10);
		if (nChar == VK_RIGHT) cube.rotate(Cube::Y, -DEGREE * 10);
		if (nChar == VK_HOME)  cube.rotate(Cube::Z, DEGREE * 10);
		if (nChar == VK_END)   cube.rotate(Cube::Z, -DEGREE * 10);
	}
	// 比例模式
	else if (m_nType == TRANSFORM_SCALE) {
		if (nChar == VK_UP)    cube.scale(1.1);   // 放大
		if (nChar == VK_DOWN)  cube.scale(0.9);   // 缩小
	}

	this->Invalidate(true);
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CMFCApplication3View::OnTranslation()
{
	m_nType = TRANSFORM_TRANSLATE;
	AfxMessageBox(_T("已切换到平移模式\n上下左右键平移，Home/End键前后移动"));
}

void CMFCApplication3View::OnRotation()
{
	m_nType = TRANSFORM_ROTATE;
	AfxMessageBox(_T("已切换到旋转模式\n上下键绕X轴，左右键绕Y轴，Home/End键绕Z轴"));
}

void CMFCApplication3View::OnScale()
{
	m_nType = TRANSFORM_SCALE;
	AfxMessageBox(_T("已切换到比例模式\n上键放大，下键缩小"));
}


void CMFCApplication3View::OnProjPerspective()
{
	m_nProjType = PROJ_PERSPECTIVE;
	AfxMessageBox(_T("已切换到透视投影"));
	this->Invalidate(true);
}

void CMFCApplication3View::OnProjOblique()
{
	m_nProjType = PROJ_OBLIQUE;
	AfxMessageBox(_T("已切换到斜二测投影"));
	this->Invalidate(true);
}
