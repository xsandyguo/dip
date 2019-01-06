#pragma once

#include <afxwin.h>
#include <base/basictypes.h>

#include "xTargetButton.h"
#include "resource.h"

class DialogBase : public CDialog{
public:
    DialogBase(UINT nIDTemplate, CWnd* pParent = NULL);

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    CxTargetButton	m_ok;
    CxTargetButton	m_canc;

private:
    DISALLOW_COPY_AND_ASSIGN(DialogBase);
};