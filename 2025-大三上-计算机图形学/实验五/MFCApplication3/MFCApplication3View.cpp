// MFCApplication3View.cpp: CMFCApplication3View 类的实现
//

#include "pch.h"
#include "framework.h"
#ifndef SHARED_HANDLERS
#include "MFCApplication3.h"
#endif

#include "MFCApplication3Doc.h"
#include "MFCApplication3View.h"
#include <cmath>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCApplication3View

IMPLEMENT_DYNCREATE(CMFCApplication3View, CView)

BEGIN_MESSAGE_MAP(CMFCApplication3View, CView)
    ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
    ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
    ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMFCApplication3View::OnFilePrintPreview)
    ON_WM_CONTEXTMENU()
    ON_WM_RBUTTONUP()
    ON_COMMAND(ID_BEZIER_DECAS, &CMFCApplication3View::OnBezierDecas)
    ON_COMMAND(ID_BEZIER_SPLIT, &CMFCApplication3View::OnBezierSplit)
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_MOUSEMOVE()
    ON_COMMAND(ID_HERMITE, &CMFCApplication3View::OnHermite)
    ON_COMMAND(ID_BSPLINE, &CMFCApplication3View::OnBspline)
END_MESSAGE_MAP()

// CMFCApplication3View 构造/析构

CMFCApplication3View::CMFCApplication3View() noexcept
{
    m_nCount = 0;
    nType = 0;  // 默认Bezier Decas
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
    CMFCApplication3Doc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    if (!pDoc)
        return;
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


// ===================== Bezier 曲线相关函数 =====================

void CMFCApplication3View::new_split_Bezier(CDC* pDC, POINT2D P[])
{
    POINT2D R[N + 1], Q[N + 1];
    int i, j;
    const double epsilon = 1;

    if (maxdistance(P) < epsilon)
    {
        pDC->MoveTo((int)P[0].x, (int)P[0].y);
        pDC->LineTo((int)P[N].x, (int)P[N].y);
    }
    else
    {
        for (i = 0; i <= N; i++)
            R[i] = P[i];
        for (i = 0; i <= N - 1; i++)
        {
            Q[i] = R[0];
            for (j = 0; j <= N - 1 - i; j++)
            {
                R[j].x = (R[j].x + R[j + 1].x) / 2;
                R[j].y = (R[j].y + R[j + 1].y) / 2;
            }
        }
        Q[N] = R[0];
        new_split_Bezier(pDC, Q);
        new_split_Bezier(pDC, R);
    }
}

double CMFCApplication3View::maxdistance(POINT2D p[])
{
    double s[N - 1], h[N - 1], hmax = 0;
    for (int i = 1; i <= N - 1; i++) {
        s[i - 1] = ((p[0].x - p[i].x) * (p[0].y + p[i].y) +
            (p[i].x - p[N].x) * (p[i].y + p[N].y) +
            (p[N].x - p[0].x) * (p[N].y + p[0].y));
    }
    double distance = sqrt((double)((p[0].x - p[N].x) * (p[0].x - p[N].x) + (p[0].y - p[N].y) * (p[0].y - p[N].y)));
    for (int i = 1; i <= N - 1; i++) {
        h[i - 1] = fabs(s[i - 1] / distance);
        if (h[i - 1] > hmax) hmax = h[i - 1];
    }
    return hmax;
}

void CMFCApplication3View::bez_to_points(POINT2D P[], int n, CPoint pts[], int npts)
{
    double t, delt;
    delt = 1.0 / (double)npts;
    t = 0.0;
    for (int i = 0; i <= npts; i++)
    {
        pts[i] = decas(P, n, t);
        t += delt;
    }
}

CPoint CMFCApplication3View::decas(POINT2D P[], int n, double t)
{
    int m, i;
    POINT2D* R, * Q;
    CPoint P0;
    R = new POINT2D[n + 1];
    Q = new POINT2D[n + 1];
    for (i = 0; i <= n; i++)
        R[i] = P[i];
    for (m = n; m > 0; m--)
    {
        for (i = 0; i <= m - 1; i++)
        {
            Q[i].x = R[i].x + t * (R[i + 1].x - R[i].x);
            Q[i].y = R[i].y + t * (R[i + 1].y - R[i].y);
        }
        for (i = 0; i <= m - 1; i++)
            R[i] = Q[i];
    }
    P0.x = (int)R[0].x;
    P0.y = (int)R[0].y;
    delete[] R;
    delete[] Q;
    return (P0);
}


// ===================== Hermite 曲线函数 =====================
// Hermite曲线需要4个点：P0(起点), P1(起点切向量终点), P2(终点切向量起点), P3(终点)
// 切向量 T0 = P1 - P0, T1 = P3 - P2

void CMFCApplication3View::DrawHermite(CDC* pDC)
{
    // P[0]=起点, P[1]=起点切向量终点, P[2]=终点切向量起点, P[3]=终点
    double x0 = P[0].x, y0 = P[0].y;
    double x1 = P[3].x, y1 = P[3].y;

    // 切向量
    double tx0 = (P[1].x - P[0].x) * 3;
    double ty0 = (P[1].y - P[0].y) * 3;
    double tx1 = (P[3].x - P[2].x) * 3;
    double ty1 = (P[3].y - P[2].y) * 3;

    // 绘制Hermite曲线（蓝色）
    CPen penBlue(PS_SOLID, 2, RGB(0, 0, 255));
    CPen* pOldPen = pDC->SelectObject(&penBlue);

    pDC->MoveTo((int)x0, (int)y0);
    for (int i = 1; i <= npoints; i++)
    {
        double t = (double)i / npoints;
        double t2 = t * t;
        double t3 = t2 * t;

        double h00 = 2 * t3 - 3 * t2 + 1;
        double h10 = t3 - 2 * t2 + t;
        double h01 = -2 * t3 + 3 * t2;
        double h11 = t3 - t2;

        double x = h00 * x0 + h10 * tx0 + h01 * x1 + h11 * tx1;
        double y = h00 * y0 + h10 * ty0 + h01 * y1 + h11 * ty1;

        pDC->LineTo((int)x, (int)y);
    }

    // 绘制起点切向量（红色箭头）
    CPen penRed(PS_SOLID, 2, RGB(255, 0, 0));
    pDC->SelectObject(&penRed);
    pDC->MoveTo((int)P[0].x, (int)P[0].y);
    pDC->LineTo((int)P[1].x, (int)P[1].y);

    // 绘制终点切向量（红色箭头）
    pDC->MoveTo((int)P[2].x, (int)P[2].y);
    pDC->LineTo((int)P[3].x, (int)P[3].y);

    // 绘制端点（小圆圈标记）
    CBrush brushGreen(RGB(0, 200, 0));
    CBrush* pOldBrush = pDC->SelectObject(&brushGreen);
    pDC->Ellipse((int)P[0].x - 5, (int)P[0].y - 5, (int)P[0].x + 5, (int)P[0].y + 5);
    pDC->Ellipse((int)P[3].x - 5, (int)P[3].y - 5, (int)P[3].x + 5, (int)P[3].y + 5);

    pDC->SelectObject(pOldPen);
    pDC->SelectObject(pOldBrush);
}



// ===================== B样条曲线函数 =====================
// 四阶(k=4)三次等距B样条

double CMFCApplication3View::BSplineBasis(int i, int k, double t, double knot[])
{
    if (k == 1)
    {
        if (t >= knot[i] && t < knot[i + 1])
            return 1.0;
        else
            return 0.0;
    }
    else
    {
        double d1 = knot[i + k - 1] - knot[i];
        double d2 = knot[i + k] - knot[i + 1];
        double c1 = 0.0, c2 = 0.0;

        if (d1 != 0)
            c1 = (t - knot[i]) / d1 * BSplineBasis(i, k - 1, t, knot);
        if (d2 != 0)
            c2 = (knot[i + k] - t) / d2 * BSplineBasis(i + 1, k - 1, t, knot);

        return c1 + c2;
    }
}

void CMFCApplication3View::DrawBSpline(CDC* pDC)
{
    int n = m_nCount - 1;  // 此时m_nCount已重置为0，所以用实际的点数
    // B样条至少需要4个控制点
    int count = 4;  // 这里固定用4个点（因为我们的输入逻辑）

    // 如果实际要支持更多点，需要修改输入逻辑
    // 当前实现：4个控制点的三次B样条

    int k = 4;  // 四阶
    int numKnots = count + k;  // 节点数 = 控制点数 + 阶数

    // 创建等距节点向量
    double* knot = new double[numKnots];
    for (int i = 0; i < numKnots; i++)
    {
        knot[i] = (double)i;
    }

    // 有效参数范围 [k-1, count]
    double tMin = knot[k - 1];
    double tMax = knot[count] - 0.0001;  // 稍微减小以避免边界问题

    // 绘制B样条曲线
    bool first = true;
    for (int j = 0; j <= npoints; j++)
    {
        double t = tMin + (tMax - tMin) * j / npoints;
        double x = 0.0, y = 0.0;

        for (int i = 0; i < count; i++)
        {
            double basis = BSplineBasis(i, k, t, knot);
            x += P[i].x * basis;
            y += P[i].y * basis;
        }

        if (first)
        {
            pDC->MoveTo((int)x, (int)y);
            first = false;
        }
        else
        {
            pDC->LineTo((int)x, (int)y);
        }
    }

    delete[] knot;
}


// ===================== 绘制控制多边形 =====================

void CMFCApplication3View::DrawControlPolygon(CDC* pDC, int count)
{
    CPen penDash(PS_DASH, 1, RGB(128, 128, 128));  // 灰色虚线
    CPen* pOldPen = pDC->SelectObject(&penDash);

    pDC->MoveTo((int)P[0].x, (int)P[0].y);
    for (int i = 1; i < count; i++)
    {
        pDC->LineTo((int)P[i].x, (int)P[i].y);
    }

    // 绘制控制点（小圆圈）
    CBrush brushRed(RGB(255, 0, 0));
    CBrush* pOldBrush = pDC->SelectObject(&brushRed);
    for (int i = 0; i < count; i++)
    {
        pDC->Ellipse((int)P[i].x - 4, (int)P[i].y - 4,
            (int)P[i].x + 4, (int)P[i].y + 4);
    }

    pDC->SelectObject(pOldPen);
    pDC->SelectObject(pOldBrush);
}


// ===================== 菜单响应函数 =====================

void CMFCApplication3View::OnBezierDecas()
{
    nType = 0;
    m_nCount = 0;  // 重置点计数
    Invalidate();  // 清屏
}

void CMFCApplication3View::OnBezierSplit()
{
    nType = 1;
    m_nCount = 0;
    Invalidate();
}

void CMFCApplication3View::OnHermite()
{
    nType = 2;
    m_nCount = 0;
    Invalidate();
}

void CMFCApplication3View::OnBspline()
{
    nType = 3;
    m_nCount = 0;
    Invalidate();
}


// ===================== 鼠标消息处理 =====================

void CMFCApplication3View::OnLButtonDown(UINT nFlags, CPoint point)
{
    P[m_nCount].x = point.x;
    P[m_nCount].y = point.y;
    m_Newpoint = point;
    m_nCount = (++m_nCount) % (N + 1);  // 4个点循环

    CView::OnLButtonDown(nFlags, point);
}

void CMFCApplication3View::OnLButtonUp(UINT nFlags, CPoint point)
{
    // 当输入完4个点后绘制曲线
    if (m_nCount == 0)
    {
        CClientDC dc(this);

        // 先绘制控制多边形
        DrawControlPolygon(&dc, N + 1);

        // 根据类型绘制对应曲线
        CPen penBlue(PS_SOLID, 2, RGB(0, 0, 255));  // 蓝色曲线
        CPen* pOldPen = dc.SelectObject(&penBlue);

        switch (nType) {
        case 0:  // Bezier - de Casteljau
            bez_to_points(P, N, bezier, npoints);
            dc.Polyline(bezier, npoints + 1);
            break;
        case 1:  // Bezier - Split
            new_split_Bezier(&dc, P);
            break;
        case 2:  // Hermite
            DrawHermite(&dc);
            break;
        case 3:  // B-Spline
            DrawBSpline(&dc);
            break;
        }

        dc.SelectObject(pOldPen);
    }

    CView::OnLButtonUp(nFlags, point);
}

void CMFCApplication3View::OnMouseMove(UINT nFlags, CPoint point)
{
    if (m_nCount > 0 && m_nCount <= N)
    {
        CClientDC dc(this);
        dc.SetROP2(R2_NOT);
        dc.MoveTo((int)P[m_nCount - 1].x, (int)P[m_nCount - 1].y);
        dc.LineTo(m_Newpoint);
        dc.MoveTo((int)P[m_nCount - 1].x, (int)P[m_nCount - 1].y);
        dc.LineTo(point);
        m_Newpoint = point;
    }

    CView::OnMouseMove(nFlags, point);
}
