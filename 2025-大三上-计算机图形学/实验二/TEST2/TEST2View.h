
// TEST2View.h: CTEST2View 类的接口
//

#pragma once


class CTEST2View : public CView
{
protected: // 仅从序列化创建
	CTEST2View() noexcept;
	DECLARE_DYNCREATE(CTEST2View)

// 特性
public:
	CTEST2Doc* GetDocument() const;
	int m_nCount;
	CArray <CPoint, CPoint>ptArray;
	CPoint m_Newpoint;
	
	

// 操作
public:


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
	virtual ~CTEST2View();
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
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // TEST2View.cpp 中的调试版本
inline CTEST2Doc* CTEST2View::GetDocument() const
   { return reinterpret_cast<CTEST2Doc*>(m_pDocument); }
#endif

