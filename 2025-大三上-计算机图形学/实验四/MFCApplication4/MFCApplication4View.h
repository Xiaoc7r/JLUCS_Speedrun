
// MFCApplication4View.h: CMFCApplication4View 类的接口
//

#pragma once


class CMFCApplication4View : public CView
{
protected: // 仅从序列化创建
	CMFCApplication4View() noexcept;
	DECLARE_DYNCREATE(CMFCApplication4View)

// 特性
public:
	CMFCApplication4Doc* GetDocument() const;

// 操作
public:
	BOOL m_LBDown;
	BOOL m_RBDown;
	CPoint m_LTPoint;
	CPoint m_RBPoint;//LT point and RB point of the window

	CPoint m_Polygon[512];
	CPoint m_Polygon1[512];
	int	   m_Num_Ply;
	int	   m_Num_Ply1;
	CPoint m_ClipedPly[512];
	int	   m_NumCliped_Ply;

public:
	void	UpdateIndex(int& Index, int Length);
	int		CutByLeft(CPoint* source, int sLength, CPoint* destin);
	int		CutByRight(CPoint* source, int sLength, CPoint* destin);
	int 	CutByBottom(CPoint* source, int sLength, CPoint* destin);
	int 	CutByTop(CPoint* source, int sLength, CPoint* destin);
	void Sutherland_Hodgman();

public:
	void ShowLineSeg(CPoint BPoint, CPoint EPoint);
	void CorrectWindow();
	void ClearScreen();

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
	virtual ~CMFCApplication4View();
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
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // MFCApplication4View.cpp 中的调试版本
inline CMFCApplication4Doc* CMFCApplication4View::GetDocument() const
   { return reinterpret_cast<CMFCApplication4Doc*>(m_pDocument); }
#endif

