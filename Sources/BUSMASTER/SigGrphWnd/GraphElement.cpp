/*********************************************************************
  Project       :  Auto-SAT_Tools
  FileName      :  GraphElement.cpp
  Description   :  Implementation file for CGraphElement class
  $Log:   X:/Archive/Sources/SigGrphWnd/GraphElement.cpv  $

  Author(s)     :  Raja N
  Date Created  :  01.12.2004
  Modified By   :
  Copyright (c) 2011, Robert Bosch Engineering and Business Solutions.  All rights reserved
 *********************************************************************/
// For standard Headers
#include "SigGrphWnd_stdafx.h"
// For CGraphElement class declaration
#include "GraphElement.h"

/*******************************************************************************
 Function Name  : CGraphElement
 Description    : Standard default constructor
 Member of      : CGraphElement
 Functionality  : This will initialise local variables
 Author(s)      : Raja N
 Date Created   : 01.12.2004
 Modifications  :
*******************************************************************************/
CGraphElement::CGraphElement()
{
    // Message ID
    m_nMsgID = -1;
    // Message Name
    m_strMsgName = "";
    // Format - Standard or Extended
    m_nFrameFormat = -1;
    // Element Name
    m_omStrElementName = "";
    // Value Type
    m_nValueType = -1;
    // Graph Type
    m_nLineType = 0; // For Solid
    // Default Color
    m_nLineColor = RED_COLOR;
    // Sample point type
    m_nPointType = 0; // None
    // Sample point color
    m_nPointColor = 0; // Black
    // Visible
    m_bVisible = TRUE;
    // Enabled
    m_bEnabled = TRUE;
}

/*******************************************************************************
 Function Name  : CGraphElement
 Description    : Standard Destructor
 Member of      : CGraphElement
 Functionality  : -
 Author(s)      : Raja N
 Date Created   : 01.12.2004
*******************************************************************************/
CGraphElement::~CGraphElement()
{

}

BYTE* CGraphElement::pbyGetConfigData(BYTE* pbyTrgData)
{
    BYTE* pbyTemp = pbyTrgData;
    if (pbyTemp != nullptr)
    {
        // Save properties one after another
        // Message ID
        COPY_DATA(pbyTemp, &m_nMsgID, sizeof(int));
        // Message Name
        char MsgName[MAX_PATH] = {'\0'};
        strcpy_s(MsgName, MAX_PATH, m_strMsgName.GetBuffer(MAX_PATH));
        COPY_DATA(pbyTemp, MsgName, (sizeof(char) * MAX_PATH));
        // Frame Format - Standard
        COPY_DATA(pbyTemp, &m_nFrameFormat, sizeof(short));
        // Element Name String
        char acName[MAX_PATH] = {'\0'};
        strcpy_s(acName, MAX_PATH, m_omStrElementName.GetBuffer(MAX_PATH));
        COPY_DATA(pbyTemp, acName, (sizeof(char) * MAX_PATH));
        // Type of the element val
        COPY_DATA(pbyTemp, &m_nValueType, sizeof(int));
        // Line type of the elemen
        COPY_DATA(pbyTemp, &m_nLineType, sizeof(int));
        // Line Color of the eleme
        COPY_DATA(pbyTemp, &m_nLineColor, sizeof(int));
        // Sample point symbol typ
        COPY_DATA(pbyTemp, &m_nPointType, sizeof(int));
        // Sample point symbol col
        COPY_DATA(pbyTemp, &m_nPointColor, sizeof(int));
        // Visible or Not
        COPY_DATA(pbyTemp, &m_bVisible, sizeof(BOOL));
        // Enabled or not
        COPY_DATA(pbyTemp, &m_bEnabled, sizeof(BOOL));
    }
    return pbyTemp;
}

BYTE* CGraphElement::pbySetConfigData(BYTE* pbyTrgData)
{
    BYTE* pbyTemp = pbyTrgData;
    if (pbyTemp != nullptr)
    {
        // Save properties one after another
        // Message ID
        COPY_DATA_2(&m_nMsgID, pbyTemp, sizeof(int));
        //Message Name
        char MsgName[MAX_PATH] = {'\0'};
        COPY_DATA_2(MsgName, pbyTemp, (sizeof(char) * MAX_PATH));
        m_strMsgName.Format("%s", MsgName);
        // Frame Format - Standard
        COPY_DATA_2(&m_nFrameFormat, pbyTemp, sizeof(short));
        // Element Name String
        char acName[MAX_PATH] = {'\0'};
        COPY_DATA_2(acName, pbyTemp, (sizeof(char) * MAX_PATH));
        m_omStrElementName.Format("%s", acName);
        // Type of the element val
        COPY_DATA_2(&m_nValueType, pbyTemp, sizeof(int));
        // Line type of the elemen
        COPY_DATA_2(&m_nLineType, pbyTemp, sizeof(int));
        // Line Color of the eleme
        COPY_DATA_2(&m_nLineColor, pbyTemp, sizeof(int));
        // Sample point symbol typ
        COPY_DATA_2(&m_nPointType, pbyTemp, sizeof(int));
        // Sample point symbol col
        COPY_DATA_2(&m_nPointColor, pbyTemp, sizeof(int));
        // Visible or Not
        COPY_DATA_2(&m_bVisible, pbyTemp, sizeof(BOOL));
        // Enabled or not
        COPY_DATA_2(&m_bEnabled, pbyTemp, sizeof(BOOL));
    }
    return pbyTemp;
}
/*******************************************************************************
 Function Name  : nSerialize
 Input(s)       : omArchive - CArchive class. Serialisation stream
 Output         : -
 Functionality  : This function will serialise this class using CArchive stream
 Member of      : CGraphElement
 Author(s)      : Raja N
 Date Created   : 01.12.2004
 Modifications  :
*******************************************************************************/
int CGraphElement::nSerialize(CArchive& omArch)
{
    int nReturn = 0;
    // If it is storing
    try
    {
        if( omArch.IsStoring() )
        {
            // Save properties one after another
            // Message ID
            omArch << m_nMsgID  ;
            // Frame Format - Standard
            omArch << m_nFrameFormat;
            // Element Name String
            omArch << m_omStrElementName;
            // Type of the element val
            omArch << m_nValueType;
            // Line type of the elemen
            omArch << m_nLineType;
            // Line Color of the eleme
            omArch << m_nLineColor;
            // Sample point symbol typ
            omArch << m_nPointType;
            // Sample point symbol col
            omArch << m_nPointColor;
            // Visible or Not
            omArch << m_bVisible;
            // Enabled or not
            omArch << m_bEnabled;
        }
        // This is for loading
        else
        {
            // Load properties one after another
            omArch >> m_nMsgID;
            // Frame Format - Standard
            omArch >> m_nFrameFormat;
            // Element Name String
            omArch >> m_omStrElementName;
            // Type of the element val
            omArch >> m_nValueType;
            // Line type of the elemen
            omArch >> m_nLineType;
            // Line Color of the eleme
            omArch >> m_nLineColor;
            // Sample point symbol typ
            omArch >> m_nPointType;
            // Sample point symbol col
            omArch >> m_nPointColor;
            // Visible or Not
            omArch >> m_bVisible;
            // Enabled or not
            omArch >> m_bEnabled;
        }
    }
    catch(CArchiveException* poArchExcep)
    {
        // Get the Exception code and delete the dynamic object
        nReturn = poArchExcep->m_cause;
        poArchExcep->Delete();
    }
    // Return the result
    return nReturn;
}
