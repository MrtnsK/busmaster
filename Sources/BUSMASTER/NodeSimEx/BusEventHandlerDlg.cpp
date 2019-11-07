// CBusEventHandlerDlg.cpp : implementation file
//

#include "NodeSimEx_stdafx.h"
#include "FunctionEditorDoc.h" //Accessing the CFunctionEditorDoc class pointer
#include "HashDefines.h"
#include "BusEventHandlerDlg.h"


// CBusEventHandlerDlg dialog

IMPLEMENT_DYNAMIC(CBusEventHandlerDlg, CDialog)

CBusEventHandlerDlg::CBusEventHandlerDlg(ETYPE_BUS eBus, CFunctionEditorDoc* pDoc, bool bEnablePreConnect, CWnd* pParent /*=nullptr*/, BOOL bIsDelete /*FALSE*/)
    : CDialog(CBusEventHandlerDlg::IDD, pParent)
    , m_bChkBusEventConnect(FALSE)
    ,m_bChkBusEventPreConnect(FALSE)
    ,m_bChkBusEventDisConnect(FALSE)
{
    m_pDoc = pDoc;
    m_bIsDelete = bIsDelete;
    m_eBus = eBus;
    m_bEnablePreConnect = bEnablePreConnect;
}

CBusEventHandlerDlg::~CBusEventHandlerDlg()
{
}

void CBusEventHandlerDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Check(pDX, IDC_CHKB_BUSEV_CONNECT_HANDLER, m_bChkBusEventConnect);
    DDX_Check(pDX, IDC_CHKB_BUSEV_DISCONNECT_HANDLER, m_bChkBusEventDisConnect);
}


BEGIN_MESSAGE_MAP(CBusEventHandlerDlg, CDialog)
    ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CBusEventHandlerDlg message handlers

BOOL CBusEventHandlerDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    CStringArray* pomStrArrayHandlerName = nullptr;
    CDialog::OnInitDialog();

    if (true == m_bEnablePreConnect)
    {
        GetDlgItem(IDC_CHKB_BUSEV_PRECONNECT_HANDLER)->ShowWindow(TRUE);
    }
    if( m_pDoc != nullptr )
    {
        CButton* pomButton       = nullptr;
        CString omStrHandlerName = "" ;
        CString omStrControl     = "" ;
        INT nCheck               = 0;
        // get the pointer to CStringArray of error handlers prototypes
        pomStrArrayHandlerName = m_pDoc->pomStrGetBusEventHandlerPrototypes();
        if(pomStrArrayHandlerName != nullptr )
        {
            // Check for the handlers already added.
            for(INT  i = 0; i<defBUSEV_HANDLER_NUMBER; i++)
            {
                for(INT j =0; j<pomStrArrayHandlerName->GetSize(); j++)
                {
                    pomButton = (CButton*)GetDlgItem(IDC_CHKB_BUSEV_CONNECT_HANDLER+i);
                    if(pomButton != nullptr )
                    {
                        omStrHandlerName = pomStrArrayHandlerName->GetAt(j);
                        pomButton->GetWindowText(omStrControl);
                        omStrControl.Replace(' ','_');
                        omStrControl.Replace('-','_');
                        omStrControl = "OnBus_"+ omStrControl;
                        // The find the control text in added handlers text
                        if( omStrHandlerName.Find(omStrControl) != -1 )
                        {
                            // If the dialog box is invoked for delete, check
                            // the box of already added handlers else check and
                            // disable
                            if(m_bIsDelete == TRUE )
                            {
                                pomButton->SetCheck(TRUE);
                            }
                            else
                            {
                                pomButton->SetCheck(TRUE);
                                pomButton->EnableWindow(FALSE);
                            }
                        }
                    }
                }
            }
        }
        // Disable all other check box corresponding to which the handlers
        // are not added and dialog box in invoked to delete the handlers
        if(m_bIsDelete == TRUE )
        {
            for(INT i = 0; i<defBUSEV_HANDLER_NUMBER; i++)
            {
                pomButton = (CButton*)GetDlgItem(IDC_CHKB_BUSEV_CONNECT_HANDLER+i);
                if(pomButton != nullptr )
                {
                    nCheck = pomButton->GetCheck();
                    if(nCheck == 0 )
                    {
                        pomButton->EnableWindow(FALSE);
                    }
                }
            }
            // Set the dialog caption text to indicate user is deleting
            // the handlers
            SetWindowText(defBUSEV_HANDLER_TEXT_DEL);
        }
        else
        {
            // Set the dialog caption text to indicate user is adding
            // the handlers
            SetWindowText(defBUSEV_HANDLER_TEXT_ADD);
        }
    }
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CBusEventHandlerDlg::OnBnClickedOk()
{
    CButton* pomButton       = nullptr;
    CString omStrHandlerName = "" ;
    CString omStrControl     = "" ;
    INT nCheck               = 0;
    BOOL bIsEnable           = TRUE;
    // Get the selected handlers either for deleteting or adding and
    // add the name in the array
    for(int j=0; j<defBUSEV_HANDLER_NUMBER; j++)
    {
        pomButton = (CButton*)GetDlgItem(IDC_CHKB_BUSEV_CONNECT_HANDLER + j);
        if(pomButton != nullptr )
        {
            nCheck      = pomButton->GetCheck();
            bIsEnable   = pomButton->IsWindowEnabled();
            pomButton->GetWindowText(omStrControl);
            omStrControl.TrimLeft();
            omStrControl.TrimRight();
            omStrControl.Replace(' ','_');
            omStrControl.Replace('-','_');
            omStrHandlerName = omStrControl;
            if(nCheck == 1 && bIsEnable != 0 )
            {
                m_omStrArrayBusEventHandler.Add(omStrHandlerName);
            }
        }
    }
    OnOK();
}
