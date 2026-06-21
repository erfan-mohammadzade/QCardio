#include "csignalview.h"

CSignalView::CSignalView(QObject *parent)
    : QObject{parent}
{
    createObj();
}

CSignalView::~CSignalView()
{
    for(QWidget *widget : m_windgetList)
        widget->deleteLater();
    m_windgetList.clear();
}

void CSignalView::createObj()
{
    m_windgetList.clear();
    m_signalViewWidget1 = new SignalViewWidget();
    m_signalViewWidget2 = new SignalViewWidget();
    m_signalViewWidget3 = new SignalViewWidget();
    m_windgetList.append(m_signalViewWidget1);
    m_windgetList.append(m_signalViewWidget2);
    m_windgetList.append(m_signalViewWidget3);
}

QList<SignalViewWidget *> CSignalView::windgetList() const
{
    return m_windgetList;
}

void CSignalView::setData(const MIT_BIH_ECGData &data)
{
    if(data.nsigs.empty()) return;

    if(data.nsigs.size() > 0)m_signalViewWidget1->setData(data.nsigs[0]);
    if(data.nsigs.size() > 1)m_signalViewWidget2->setData(data.nsigs[1]);
    if(data.nsigs.size() > 2)m_signalViewWidget3->setData(data.nsigs[2]);
}
