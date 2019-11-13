// SignalDefinerDlg.cpp : implementation file
//

#include "SignalDefiner_stdafx.h"
#include "SignalDefiner.h"
#include "SignalDefinerDlg.h"
#include "SignalDefiner_Extern.h"

#define M_PI       3.14159265358979323846
#define MAX_SAMPLING_TIME_PERIOD 32767
#define MAX_FREQUENCY            125
const float SINE_COEFF = (8 / (M_PI* M_PI));

// CSignalDefinerDlg dialog

IMPLEMENT_DYNAMIC(CSignalDefinerDlg, CDialog)

CSignalDefinerDlg::CSignalDefinerDlg(CWnd* pParent /*=nullptr*/) :
    CDialog(CSignalDefinerDlg::IDD, pParent),
    m_fAmplitude(10),
    m_fFrequency(1),
    m_dblSamplingTimePeriod(125),
    m_nSelCycle(3),
    m_nSignalType(1),
    m_bAutoCorrect(true),
    m_poDMGraphCtrl(nullptr),
    m_strSignalName()
{
    AfxEnableControlContainer();
}

CSignalDefinerDlg::~CSignalDefinerDlg()
{
}

void CSignalDefinerDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_SIGNAL_AMPLITUDE, m_fAmplitude);
    DDX_Text(pDX, IDC_EDIT_SIGNAL_FREQUENCY, m_fFrequency);
    DDX_Text(pDX, IDC_EDIT_SIGNAL_SAMPLING_TIME, m_dblSamplingTimePeriod);
    DDV_MinMaxDouble(pDX, m_dblSamplingTimePeriod, 1, MAX_SAMPLING_TIME_PERIOD);
    DDV_MinMaxFloat(pDX, m_fFrequency,1, MAX_FREQUENCY);
    DDX_CBIndex(pDX, IDC_COMBO_CYCLES, m_nSelCycle);
    DDX_Control(pDX, IDC_COMBO_SIGNAL_TYPE, m_ctrSignalType);
    DDX_Control(pDX, IDC_COMBO_CYCLES, m_ctrNoOfCycles);
    DDX_Control(pDX, IDCB_AUTOCORRECT, m_btnAutoCorrect);
    DDX_CBIndex(pDX, IDC_COMBO_SIGNAL_TYPE, m_nSignalType);
}


BEGIN_MESSAGE_MAP(CSignalDefinerDlg, CDialog)
    ON_CBN_SELCHANGE(IDC_COMBO_SIGNAL_TYPE, OnCbnSelchangeComboSignalType)
    ON_CBN_SELCHANGE(IDC_COMBO_CYCLES, OnCbnSelchangeComboCycles)
    ON_EN_CHANGE(IDC_EDIT_SIGNAL_AMPLITUDE, OnEnChangeEditSignalAmplitude)
    ON_EN_CHANGE(IDC_EDIT_SIGNAL_FREQUENCY, OnEnChangeEditSignalFrequency)
    ON_EN_CHANGE(IDC_EDIT_SIGNAL_SAMPLING_TIME, OnEnChangeEditSignalSamplingTime)
    ON_EN_UPDATE(IDC_EDIT_SIGNAL_SAMPLING_TIME, OnEnUpdateEditSignalSamplingTime)
    ON_BN_CLICKED(IDOK, OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
    ON_BN_CLICKED(IDCB_AUTOCORRECT, OnBnClickedAutoCorrect)
    ON_WM_CLOSE()
    ON_CONTROL_RANGE(EN_KILLFOCUS, IDC_COMBO_SIGNAL_TYPE, IDCB_AUTOCORRECT, OnKillFocus)
END_MESSAGE_MAP()


BOOL CSignalDefinerDlg::OnInitDialog()
{
    CDialog::OnInitDialog();


    UpdateData(FALSE);
    m_ctrNoOfCycles.SetCurSel(m_nSelCycle);

    //Get the CWnd reference to the DMGraph ActiveX control
    m_poDMGraphCtrl = GetDlgItem(IDC_DMGRAPHCTRL);

    /*Set Graph properties*/
    if( m_poDMGraphCtrl->m_hWnd != nullptr )
    {
        LPUNKNOWN pUnk = m_poDMGraphCtrl->GetControlUnknown();
        IDMGraphCtrl* pDMGraphCtrl = nullptr;

        pUnk->QueryInterface(IID_IDMGraphCtrl, (void**) &pDMGraphCtrl);
        if (  pDMGraphCtrl ==nullptr )
        {
            return TRUE;
        }
        // Set Axis Color
        pDMGraphCtrl->put_AxisColor((OLE_COLOR)0xFF);
        // Set Plot Area Color
        pDMGraphCtrl->put_PlotAreaColor((OLE_COLOR)0x00);
        // Set Grid Color
        pDMGraphCtrl->put_GridColor((OLE_COLOR)0xC0C0C0);
        // Set Frame Style
        pDMGraphCtrl->put_FrameStyle((FrameStyle)1);
        // Set Frame Color
        pDMGraphCtrl->put_ControlFrameColor((OLE_COLOR)0x5500);

        // Set Grid Lines Count
        CComPtr<IDMGraphAxis> spAxisX;
        pDMGraphCtrl->get_Axis( HorizontalX, &spAxisX);
        spAxisX->put_GridNumber(10);

        CComPtr<IDMGraphAxis> spAxisY;
        pDMGraphCtrl->get_Axis( VerticalY, &spAxisY);
        spAxisY->put_GridNumber(5);
        if(m_bAutoCorrect == true)
        {
            m_btnAutoCorrect.SetCheck(BST_CHECKED);
        }
        else
        {
            m_btnAutoCorrect.SetCheck(BST_UNCHECKED);
        }

        if (nullptr != pDMGraphCtrl)
        {
            pDMGraphCtrl->Release();
            pDMGraphCtrl = nullptr;
        }
    }

    vGenerateWave();


    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

// CSignalDefinerDlg message handlers

void CSignalDefinerDlg::OnCbnSelchangeComboSignalType()
{
    vGenerateWave();
}

void CSignalDefinerDlg::OnCbnSelchangeComboCycles()
{
    vGenerateWave();
}

void CSignalDefinerDlg::OnEnChangeEditSignalAmplitude()
{
    vGenerateWave();
}

void CSignalDefinerDlg::OnEnChangeEditSignalFrequency()
{
    vGenerateWave();
}

void CSignalDefinerDlg::OnEnChangeEditSignalSamplingTime()
{
    vGenerateWave();
}
void CSignalDefinerDlg::OnEnUpdateEditSignalSamplingTime()
{
    CWnd* pEdit = GetDlgItem(IDC_EDIT_SIGNAL_SAMPLING_TIME);
    if(pEdit != nullptr)
    {
#ifdef _UNICODE
        CString strTime = L"";
        pEdit->GetWindowText(strTime);
        double dTime = _wtof(strTime);
#else
        CString strTime = "";
        pEdit->GetWindowText(strTime);
        double dTime = atof(strTime);
#endif
        if(dTime > 0)
        {
            m_dblSamplingTimePeriod = dTime;
        }
    }
    //UpdateData(FALSE);
}

void CSignalDefinerDlg::OnKillFocus(UINT nID)
{
    if ( IDC_EDIT_SIGNAL_SAMPLING_TIME == nID )
    {
        UpdateData(FALSE);
    }
}

void CSignalDefinerDlg::OnBnClickedOk()
{
    long lCount = 0;
    if(spElements != nullptr)
    {
        spElements->get_Count(&lCount);
        for (long lIdx = 0; lIdx < lCount; lIdx++)
        {
            spElements->Delete(lIdx);
            spElements = nullptr;
        }
    }
    OnOK();

    // If Sampling time period value is out of range
    // Setting focus back to Sampling Time Period
    if(m_dblSamplingTimePeriod < 1 || m_dblSamplingTimePeriod > MAX_SAMPLING_TIME_PERIOD)
    {
        (GetDlgItem(IDC_EDIT_SIGNAL_SAMPLING_TIME))->SetFocus();
    }
    else
    {
        (GetDlgItem(IDC_EDIT_SIGNAL_FREQUENCY))->SetFocus();
    }
}

void CSignalDefinerDlg::OnBnClickedAutoCorrect()
{
    if(m_btnAutoCorrect.GetCheck() == BST_UNCHECKED)
    {
        m_bAutoCorrect = false;
    }
    else
    {
        m_bAutoCorrect = true;
    }
    vGenerateWave();
}
/**
* \brief         Helper to convert degrees to radians
* \param[out]    -
* \param[in]     Degrees as double
* \return        Radians as double
* \authors       Arunkumar Karri
* \date          03.02.2012 Created
*/

double DegreesToRadians(double dblDegrees)
{
    return 2 * M_PI * dblDegrees / 360.0;
}

/**
* \brief         Helper function to get the Triangle values calculated
* \param[out]    -
* \param[in]     dblXSamplingPoint, fAmplitude, dblTimePeriod
* \return        double is the Y value calculated
* \authors       Arunkumar Karri
* \date          07.02.2012 Created
*/
double CalculateYatXForTriangleWave(double dblXSamplingPoint,float fAmplitude,double dblTimePeriod)
{
    double dblYResult = 0;

    if ( dblXSamplingPoint <= dblTimePeriod / 4 )
    {
        dblYResult = ((4 * fAmplitude) / dblTimePeriod) * dblXSamplingPoint;
    }
    else if ( dblTimePeriod / 4 < dblXSamplingPoint && dblXSamplingPoint <= dblTimePeriod / 2 )
    {
        dblYResult = fAmplitude - ((((4 * fAmplitude) / dblTimePeriod) * dblXSamplingPoint) - fAmplitude);
    }
    else if ( dblTimePeriod / 2 < dblXSamplingPoint && dblXSamplingPoint <= 0.75 * dblTimePeriod )
    {
        dblYResult = -(((4 * fAmplitude) / dblTimePeriod) * (dblXSamplingPoint - (dblTimePeriod / 2)));
    }
    else if ( 0.75 * dblTimePeriod < dblXSamplingPoint && dblXSamplingPoint <= dblTimePeriod )
    {
        dblYResult = -(fAmplitude - ((((4 * fAmplitude) / dblTimePeriod) * (dblXSamplingPoint - (dblTimePeriod / 2))) - fAmplitude));
    }
    return dblYResult;
}

/**
* \brief         Generates points based on selected signal type
* \param[out]    -
* \param[in]     Signal Type
* \return        void
* \authors       Arunkumar Karri
* \date          03.02.2012 Created
*/
void CSignalDefinerDlg::vGenerateWave()
{
    UpdateData();

    SIGNAL_TYPE enSignalType = (SIGNAL_TYPE)(m_ctrSignalType.GetCurSel());
    int nPointCount;
    double dblFrqStep, dblTimePeriod, dblSamplingPoint;;

    /*Setting the Frequency Resolution to nResolution*/
    dblFrqStep = m_dblSamplingTimePeriod;

    /* Calculate Time period per each cycle */
    dblTimePeriod = 1 / m_fFrequency;

    if(m_fFrequency == 0.0)
    {
        (GetDlgItem(IDC_EDIT_SIGNAL_FREQUENCY))->SetFocus();
        return;
    }
    //Calculate number of points to be plotted
    nPointCount = ((dblTimePeriod * 1000) + (dblFrqStep / 10)) * (m_nSelCycle+1);

    if(m_btnAutoCorrect.GetCheck() == BST_CHECKED )
    {
        if((nPointCount/(dblFrqStep*(m_nSelCycle+1))) < 8)          //the no. of points in a cycle should be more than 8 to plot a proper graph
        {
            dblFrqStep = nPointCount/(9* (m_nSelCycle+1)); //set the number of required points to 9
            if(dblFrqStep > MAX_SAMPLING_TIME_PERIOD)
            {
                (GetDlgItem(IDC_EDIT_SIGNAL_FREQUENCY))->SetFocus();
                return;
            }
            if(dblFrqStep < 1)
            {
                (GetDlgItem(IDC_EDIT_SIGNAL_FREQUENCY))->SetFocus();
                return;
            }
            m_dblSamplingTimePeriod = dblFrqStep;
            CString omSamplingPeriod;
            omSamplingPeriod.Format(TEXT("%d"), (int)m_dblSamplingTimePeriod);
            SetDlgItemText(IDC_EDIT_SIGNAL_SAMPLING_TIME, omSamplingPeriod);            //This command will call this function continously, hence the above logic.
            nPointCount = ((dblTimePeriod * 1000) + (dblFrqStep / 10)) * (m_nSelCycle+1);
        }
    }
    else if(m_btnAutoCorrect.GetCheck() == BST_UNCHECKED )
    {
        if(m_fFrequency > MAX_FREQUENCY)
        {
            (GetDlgItem(IDC_EDIT_SIGNAL_FREQUENCY))->SetFocus();
            return;
        }
        if(dblFrqStep == 0)
        {
            // If Sampling time period value is out of range
            // Setting focus back to Sampling Time Period
            (GetDlgItem(IDC_EDIT_SIGNAL_SAMPLING_TIME))->SetFocus();
            return;
        }
        if(dblFrqStep > MAX_SAMPLING_TIME_PERIOD)
        {
            // If Sampling time period value is out of range
            // Setting focus back to Sampling Time Period
            (GetDlgItem(IDC_EDIT_SIGNAL_SAMPLING_TIME))->SetFocus();
            return;
        }
    }
    /*For variant packing purpose*/
    CComVariant varrX, varrY;
    varrX.parray = SafeArrayCreateVector(VT_R8, 0, nPointCount);
    if(varrX.parray == nullptr)
    {
        return;
    }
    varrX.vt = VT_ARRAY|VT_R8;

    varrY.parray = SafeArrayCreateVector(VT_R8, 0, nPointCount);
    if(varrY.parray == nullptr)
    {
        return;
    }
    varrY.vt = VT_ARRAY|VT_R8;

    LONG lngCount = 0;

    if(dblFrqStep > 0)
    {
        /*Currently using the Peak to Peak Amplitude as 0 to 2*Amplitude
        instead of -Amplitude to +Amplitude*/
        for(double dblCounter=0; dblCounter<nPointCount; dblCounter+=dblFrqStep)
        {
            double dblX, dblY;
            dblCounter /= 1000;
            switch(enSignalType)
            {
                case SINE_WAVE:
                    dblX = dblCounter*1000;

                    dblY = m_fAmplitude +
                           m_fAmplitude * sin( DegreesToRadians(2 * 180 * m_fFrequency * dblCounter) );

                    break;
                case COS_WAVE:
                    dblX = dblCounter*1000;

                    dblY = m_fAmplitude +
                           m_fAmplitude * cos( DegreesToRadians(2 * 180 * m_fFrequency * dblCounter) );
                    break;
                case TRIANGULAR_WAVE:
                    dblSamplingPoint = dblCounter;
                    while ( dblSamplingPoint > dblTimePeriod )
                    {
                        dblSamplingPoint -= dblTimePeriod;
                    }
                    dblX = dblCounter*1000;
                    dblY = m_fAmplitude +
                           CalculateYatXForTriangleWave(dblSamplingPoint, m_fAmplitude, dblTimePeriod);

                    break;
                case SAWTOOTH_WAVE:
                    dblSamplingPoint = dblCounter;
                    while ( dblSamplingPoint > dblTimePeriod )
                    {
                        dblSamplingPoint -= dblTimePeriod;
                    }
                    dblX = dblCounter*1000;
                    dblY = m_fAmplitude +   (((2 * m_fAmplitude * dblSamplingPoint)/dblTimePeriod )-  m_fAmplitude);/* Sawtooth :((2A t /T) - A) */

                    break;
            }
            HRESULT hr;
            hr = SafeArrayPutElement(varrX.parray, &lngCount, &dblX);
            hr = SafeArrayPutElement(varrY.parray, &lngCount, &dblY);
            lngCount++;

            if ( (dblY >= 2* m_fAmplitude) && enSignalType == SAWTOOTH_WAVE )
            {
                dblY = 0;
                hr = SafeArrayPutElement(varrX.parray, &lngCount, &dblX);
                hr = SafeArrayPutElement(varrY.parray, &lngCount, &dblY);
                lngCount++;
            }
            dblCounter *= 1000;
        }
    }

    SetGraphData(&varrX, &varrY);
}

/**
* \brief         Plots the element points available onto the Graph
* \param[out]    -
* \param[in]     pvarrX, pvarrY; Takes the element points in the form of VARIANT array
* \return        void
* \authors       Arunkumar Karri
* \date          03.02.2012 Created
*/
void CSignalDefinerDlg::SetGraphData(VARIANT* pvarrX, VARIANT* pvarrY)
{
    CComBSTR bsName("Signal");

    IDMGraphCtrl* pDMGraphCtrl = nullptr;

    /*CComPtr<IDMGraphCollection> spElements;*/

    CComPtr<IDMGraphElement> spGraphElement;

    if( m_poDMGraphCtrl->m_hWnd != nullptr )
    {
        LPUNKNOWN pUnk = m_poDMGraphCtrl->GetControlUnknown();

        pUnk->QueryInterface(IID_IDMGraphCtrl, (void**) &pDMGraphCtrl);
        if (  pDMGraphCtrl ==nullptr )
        {
            return;
        }
    }

    HRESULT hr = pDMGraphCtrl->get_Elements(&spElements);

    //Set the signal name as caption
    pDMGraphCtrl->put_Caption(m_strSignalName);

    long i, nElementCount = 0;
    BOOL bReplace = FALSE;
    hr = spElements->get_Count(&nElementCount);
    for(i=0; i<nElementCount; i++)
    {
        CComBSTR bsElemName;
        CComPtr<IDispatch> spDispatch;

        hr = spElements->get_Item(i, &spDispatch);
        hr = spDispatch.QueryInterface(&spGraphElement);

        spGraphElement->get_Name(&bsElemName);
        if(_wcsicmp(bsElemName, bsName) == 0)
        {
            spGraphElement->Plot(*pvarrX, *pvarrY);
            pDMGraphCtrl->AutoRange();
            return;
        }
        else
        {
            spGraphElement = nullptr;
        }
    }
    if(bReplace == FALSE || spGraphElement == nullptr)
    {
        CComPtr<IDispatch> spDispatch;
        hr = spElements->Add(&spDispatch);
        spGraphElement = nullptr;
        hr = spDispatch.QueryInterface(&spGraphElement);
    }

    hr = spGraphElement->put_Name(bsName);

    hr = spGraphElement->put_PointSymbol( Dots );
    hr = spGraphElement->put_PointSize(2);
    hr = spGraphElement->Plot(*pvarrX, *pvarrY);


    hr = pDMGraphCtrl->AutoRange();

    if (nullptr != pDMGraphCtrl)
    {
        pDMGraphCtrl->Release();
        pDMGraphCtrl = nullptr;
    }
}
void CSignalDefinerDlg::OnClose()
{
    long lCount = 0;
    if(spElements != nullptr)
    {
        spElements->get_Count(&lCount);
        for (long lIdx = 0; lIdx < lCount; lIdx++)
        {
            spElements->Delete(lIdx);
            spElements = nullptr;
        }
    }
    CDialog::OnClose();
}
void CSignalDefinerDlg::OnBnClickedCancel()
{
    long lCount = 0;
    if(spElements != nullptr)
    {
        spElements->get_Count(&lCount);
        for (long lIdx = 0; lIdx < lCount; lIdx++)
        {
            spElements->Delete(lIdx);
            spElements = nullptr;
        }
    }
    CDialog::OnCancel();
}