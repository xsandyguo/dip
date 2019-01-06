#pragma once

#include "DlgBase.h"

class CImageDocument;
/////////////////////////////////////////////////////////////////////////////
// DlgFFT dialog
class DlgFFT : public DialogBase
{
// Construction
public:
	CImageDocument* pDocReal;
	CImageDocument* pDocImag;

	long bForceFFT;
	long bMagnitude;
	long bInverse;

	DlgFFT(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DlgFFT)
	enum { IDD = IDD_FFT };
	CButton	m_cinverse;
	CButton	m_cmagnitude;
	CButton	m_cforcefft;
	CComboBox	m_imag;
	CComboBox	m_real;
	CxTargetButton	m_canc;
	CxTargetButton	m_ok;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgFFT)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DlgFFT)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
