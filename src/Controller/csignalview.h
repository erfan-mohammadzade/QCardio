#ifndef CSIGNALVIEW_H
#define CSIGNALVIEW_H

#include <QObject>
#include <Models/global_qcardio.h>
#include <Views/signalviewwidget.h>
class CSignalView : public QObject
{
    Q_OBJECT
public:
    explicit CSignalView(QObject *parent = nullptr);
    ~CSignalView();
    void createObj();
    QList<SignalViewWidget*> m_windgetList;
    SignalViewWidget *m_signalViewWidget1 = nullptr;
    SignalViewWidget *m_signalViewWidget2 = nullptr;
    QList<SignalViewWidget *> windgetList() const;

    void setData(const MIT_BIH_ECGData& data);

signals:
};

#endif // CSIGNALVIEW_H
