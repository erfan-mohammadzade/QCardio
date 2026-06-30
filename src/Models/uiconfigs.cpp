#include "uiconfigs.h"

UIConfigs::UIConfigs() {}

QString UIConfigs::dataBasePath() const
{
    return m_dataBasePath;
}

void UIConfigs::setDataBasePath(const QString &newDataBasePath)
{
    m_dataBasePath = newDataBasePath;
}

QString UIConfigs::dataBaseName() const
{
    return m_dataBaseName;
}

void UIConfigs::setDataBaseName(const QString &newDataBaseName)
{
    m_dataBaseName = newDataBaseName;
}

int UIConfigs::lineEditSignal1Index() const
{
    return m_lineEditSignal1Index;
}

void UIConfigs::setLineEditSignal1Index(int newLineEditSignal1Index)
{
    m_lineEditSignal1Index = newLineEditSignal1Index;
}

int UIConfigs::lineEditSignal2Index() const
{
    return m_lineEditSignal2Index;
}

void UIConfigs::setLineEditSignal2Index(int newLineEditSignal2Index)
{
    m_lineEditSignal2Index = newLineEditSignal2Index;
}

int UIConfigs::lineEditSignal3Index() const
{
    return m_lineEditSignal3Index;
}

void UIConfigs::setLineEditSignal3Index(int newLineEditSignal3Index)
{
    m_lineEditSignal3Index = newLineEditSignal3Index;
}

bool UIConfigs::exportRC7() const
{
    return m_exportRC7;
}

void UIConfigs::setExportRC7(bool newExportRC7)
{
    m_exportRC7 = newExportRC7;
}

bool UIConfigs::exportRaw() const
{
    return m_exportRaw;
}

void UIConfigs::setExportRaw(bool newExportRaw)
{
    m_exportRaw = newExportRaw;
}

float UIConfigs::gainSignal() const
{
    return m_gainSignal;
}

void UIConfigs::setGainSignal(float newGainSignal)
{
    m_gainSignal = newGainSignal;
}

int UIConfigs::signalOfset() const
{
    return m_signalOfset;
}

void UIConfigs::setSignalOfset(int newSignalOfset)
{
    m_signalOfset = newSignalOfset;
}

int UIConfigs::targetFreq() const
{
    return m_targetFreq;
}

void UIConfigs::setTargetFreq(int newTargetFreq)
{
    m_targetFreq = newTargetFreq;
}

bool UIConfigs::exportAll() const
{
    return m_exportAll;
}

void UIConfigs::setExportAll(bool newExportAll)
{
    m_exportAll = newExportAll;
}
