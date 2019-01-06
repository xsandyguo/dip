#pragma once

#include "DlgBase.h"

class DlgRepair : public DialogBase
{
// Construction
public:
	long m_ncs;

	DlgRepair(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DlgRepair)
	enum { IDD = IDD_REPAIR };
	CComboBox	m_cs;
	float	m_radius;
	int		m_iterations;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgRepair)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DlgRepair)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
