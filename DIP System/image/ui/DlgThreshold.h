#pragma once

#include "DlgBase.h"
class DlgThreshold : public DialogBase
{
// Construction
public:
	DlgThreshold(CWnd* pParent = NULL);   // standard constructor

	BYTE m_bPreserve;

// Dialog Data
	//{{AFX_DATA(DlgThreshold)
	enum { IDD = IDD_THRESHOLD };
	CButton	m_cPreserve;
	BYTE	m_level;
	BYTE	m_mean;
	long	m_thresh1;
	long	m_thresh2;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgThreshold)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DlgThreshold)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
