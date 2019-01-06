/*
 * Copyright (c) 1997, 2019. All rights reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * This code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 only, as
 * published by the Free Software Foundation.
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * version 2 for more details (a copy is included in the LICENSE file that
 * accompanied this code).
 *
 * You should have received a copy of the GNU General Public License version
 * 2 along with this work; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Please contact xsandyguo@qq.com if you need additional information or have any
 * questions.
 *
 */

#pragma once

#include <afxwin.h>  
#include <base/basictypes.h>

class CToolElement : public CObject {
    DECLARE_DYNAMIC(CToolElement)

    friend class CToolBoxCtrl;

public:
    CToolElement(const CString& strTabName);
    CToolElement(const CString& strName, int category, UINT nIDIcon);
    virtual ~CToolElement();

    BOOL AddSubTool(CToolElement* pTool);
    BOOL IsToolTab() const { return is_tooltab_; }
    void Expand(BOOL bExpand = TRUE);
    BOOL IsExpanded() const { return is_expanded_; }
    BOOL IsEnabled() const { return is_enabled_; }
    BOOL IsVisible() const { return is_visible_; }
    void SetCategory(int category) { category_ = category; }
    int GetCategory() const { return category_; }
    void* GetTaggedData() const{ return tagged_data_;}
    void SetTaggedData(void* tagged_data){ tagged_data_ = tagged_data;}

    CToolElement* GetTool(int nClass) const;
    virtual BOOL IsSelected() const;
    virtual BOOL IsHovered() const;
    BOOL IsParentExpanded() const;
    int GetExpandedSubTools(BOOL bIncludeHidden = TRUE) const;
    BOOL RemoveSubTool(CToolElement*& pTool, BOOL bDelete = TRUE);
    CToolElement* HitTest(CPoint point);

    void Redraw();
    virtual void OnDrawExpandBox(CDC* pDC, CRect rectExpand);
    virtual void OnDrawIcon(CDC* pDC, CRect rect);
    virtual void OnDrawName(CDC* pDC, CRect rect);

protected:
    void Init();
    BOOL IsSubTool(CToolElement* pTool) const;
    void SetOwnerList(CToolBoxCtrl* pWndList);
    void Reposition(int& y);

protected:
    CString name_;//Tool Name
    int category_;//Tool Class
    BOOL    is_tooltab_;//Is ToolTab?
    HICON   icon_handle_;//Tool Icon
    CRect rect_;//Tool rectangle (in the ToolBox.list coordinates)
    BOOL is_enabled_;         // Is Tool enabled?
    BOOL is_visible_;       // Is property visible
    BOOL is_expanded_;        // Is ToolTab expanded (for ToolTab only)
    BOOL is_name_truncated_;
    void* tagged_data_;
    CToolBoxCtrl*     toolbox_list_;    // Pointer to the ToolBoxList window
    CToolElement*     parent_;     // Parent Tool (NULL for ToolTab)
    CList<CToolElement*, CToolElement*> subtool_list_; // Sub-tools list

private:
    DISALLOW_EVIL_CONSTRUCTORS(CToolElement);
};

struct SelectionCallback{
    SelectionCallback()
        :old_value(NULL), new_value(NULL), args(NULL), invoke(NULL){}
    bool has_changed() const { return old_value != new_value; }
    CToolElement* old_value;
    CToolElement* new_value;
    void* args;
    void(*invoke)(SelectionCallback*);
};

class CToolBoxCtrl : public CWnd {
    DECLARE_DYNAMIC(CToolBoxCtrl)

    friend class CToolElement;

public:
    CToolBoxCtrl();
    virtual ~CToolBoxCtrl();
    virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);

    int AddToolTab(CToolElement* pToolTab, BOOL bRedraw = TRUE, BOOL bAdjustLayout = TRUE);
    BOOL DeleteToolTab(CToolElement*& pTool, BOOL bRedraw = TRUE, BOOL bAdjustLayout = TRUE);
    void RemoveAll();
    int GetToolTabCount() const { return (int) tooltabs_list_.GetCount(); }
    void SetCallback(SelectionCallback* callback){callback_ = callback; }

    void SetCurSel(CToolElement* pTool, BOOL bRedraw = TRUE);
    void SetCurSel(int nClass,BOOL bRedraw=TRUE);
    CToolElement* GetCurSel() const { return selected_item_; }
    void ExpandAll(BOOL bExpand = TRUE);

    virtual void AdjustLayout();
    CToolElement* HitTest(CPoint pt) const;

    virtual int OnDrawTool(CDC* pDC, CToolElement* pTool) const;
    void SetCustomColors(COLORREF clrBackground, COLORREF clrSelected,
        COLORREF clrHover, COLORREF m_clrSelectedBorder,
        COLORREF clrToolTab);
    void GetCustomColors(COLORREF& clrBackground, COLORREF& clrSelected, 
        COLORREF& clrHover, COLORREF& m_clrSelectedBorder,
        COLORREF& clrToolTab);

protected:
    virtual void OnFillBackground(CDC* pDC, CRect rectClient);
    virtual void OnDrawList(CDC* pDC);
    HFONT SetCurrFont(CDC* pDC);
    void CreateToolTabFont();

    void ReposTools();
    void SetScrollSizes();
    int GetTotalItems(BOOL bIncludeHidden = TRUE) const;

protected:
    CToolTipCtrl        tooltip_ctrl_;       // Tooltip control
    CScrollBar          vertical_scrollbar_; // Vertical scroll bar

    CList<CToolElement*, CToolElement*> tooltabs_list_;         // List of ToolTab
    CToolElement*                       selected_item_;                // Current selection
    CToolElement*                       hover_item_;              //Hover over

    CRect rect_list_; // ToolBox area
    int tab_height_; //ToolTab height of the single row
    int tool_height_;          // Tool height of the single row
    int vertical_scroll_offset_;   // In rows
    int vertical_scroll_total_;
    int vertical_scroll_page_;

    HFONT regular_font_;    // ToolBox list regular font
    CFont tooltab_font_; // ToolTab font
    CBrush background_brush_;

    COLORREF background_color_;
    COLORREF selected_color_;
    COLORREF hover_color_;
    COLORREF selected_border_color_;
    COLORREF tooltab_color_;
    SelectionCallback* callback_;
    
    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnPaint();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnMouseLeave();
    afx_msg void OnDestroy();

private:
    DISALLOW_EVIL_CONSTRUCTORS(CToolBoxCtrl);
};


