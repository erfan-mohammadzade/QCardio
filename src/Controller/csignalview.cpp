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
    m_windgetList.append(m_signalViewWidget1);
    m_windgetList.append(m_signalViewWidget2);
}

QList<SignalViewWidget *> CSignalView::windgetList() const
{
    return m_windgetList;
}

void CSignalView::setData(const MIT_BIH_ECGData &data)
{
    if(data.nsigs.empty()) return;
    m_signalViewWidget1->setData(data.nsigs[0]);
    m_signalViewWidget2->setData(data.nsigs[1]);
}
