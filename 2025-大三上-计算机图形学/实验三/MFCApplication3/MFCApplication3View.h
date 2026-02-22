// MFCApplication3View.h: CMFCApplication3View 类的接口
//

#pragma once
#include "POINT3.h"
#include "Cube.h"

// 投影类型枚举
enum ProjectionType {
	PROJ_PERSPECTIVE = 0,  // 透视投影
	PROJ_OBLIQUE = 1       // 斜二测投影
};

// 变换类型枚举
enum TransformType {
	TRANSFORM_TRANSLATE = 0,  // 平移
	TRANSFORM_ROTATE = 1,     // 旋转
	TRANSFORM_SCALE = 2       // 比例
};

class CMFCApplication3View : public CView
{
protected:
	CMFCApplication3View() noexcept;
	DECLARE_DYNCREATE(CMFCApplication3View)

	// 特性
public:
	CMFCApplication3Doc* GetDocument() const;

	// 操作
public:
	void drawCube(Cube* cube);

	int m_nType;              // 变换类型：0=平移, 1=旋转, 2=比例
	int m_nProjType;          // 投影类型：0=透视, 1=斜二测
	Cube cube;

	// 重写
public:
	virtual void OnDraw(CDC* pDC);
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
	// 键盘消息
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

	// 变换类型菜单
	afx_msg void OnTranslation();
	afx_msg void OnRotation();
	afx_msg void OnScale();

	// 投影类型菜单
	afx_msg void OnProjPerspective();
	afx_msg void OnProjOblique();

	// 设置对话框
	afx_msg void OnSetViewpoint();
};

#ifndef _DEBUG
inline CMFCApplication3Doc* CMFCApplication3View::GetDocument() const
{
	return reinterpret_cast<CMFCApplication3Doc*>(m_pDocument);
}
#endif
