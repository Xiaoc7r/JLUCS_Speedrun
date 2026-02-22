// MFCApplication3View.h: CMFCApplication3View 类的接口
//

#pragma once

const int N = 3;
const int npoints = 100;

class POINT2D
{
public:
	double x, y;
};

class CMFCApplication3View : public CView
{
protected: // 仅从序列化创建
	CMFCApplication3View() noexcept;
	DECLARE_DYNCREATE(CMFCApplication3View)

	// 特性
public:
	CMFCApplication3Doc* GetDocument() const;

	// 操作
public:
	// Bezier曲线函数
	void new_split_Bezier(CDC* pDC, POINT2D P[]);
	double maxdistance(POINT2D p[]);
	void bez_to_points(POINT2D P[], int n, CPoint pts[], int npoints);
	CPoint decas(POINT2D P[], int n, double t);

	// Hermite曲线函数
	void DrawHermite(CDC* pDC);

	// B样条曲线函数
	void DrawBSpline(CDC* pDC);
	double BSplineBasis(int i, int k, double t, double knot[]);

	// 绘制控制多边形
	void DrawControlPolygon(CDC* pDC, int count);

	int m_nCount;
	CPoint m_Newpoint;
	POINT2D P[N + 1];
	CPoint bezier[npoints + 1];
	int nType;  // 0=Bezier Decas, 1=Bezier Split, 2=Hermite, 3=BSpline

	// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

	// 实现
public:
	virtual ~CMFCApplication3View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBezierDecas();
	afx_msg void OnBezierSplit();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnHermite();
	afx_msg void OnBspline();
};

#ifndef _DEBUG  // MFCApplication3View.cpp 中的调试版本
inline CMFCApplication3Doc* CMFCApplication3View::GetDocument() const
{
	return reinterpret_cast<CMFCApplication3Doc*>(m_pDocument);
}
#endif
