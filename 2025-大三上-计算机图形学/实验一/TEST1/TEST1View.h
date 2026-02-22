
// TEST1View.h: CTEST1View 类的接口
//

#pragma once


class CTEST1View : public CView
{
protected: // 仅从序列化创建
	CTEST1View() noexcept;
	DECLARE_DYNCREATE(CTEST1View)

// 特性
public:
	CTEST1Doc* GetDocument() const;

// 操作
public:
	void DDALine(CDC* pDC, int x1, int y1, int x2, int y2);
	
		CPoint m_Oldpoint, m_Newpoint;
	BOOL m_pLMousedown;
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
	virtual ~CTEST1View();
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
//	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // TEST1View.cpp 中的调试版本
inline CTEST1Doc* CTEST1View::GetDocument() const
   { return reinterpret_cast<CTEST1Doc*>(m_pDocument); }
#endif

