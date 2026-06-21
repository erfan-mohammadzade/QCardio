#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include "Views/mainwindow.h"
#include "./Controller/cwfdb.h"
#include "./Controller/csignalview.h"
#include "./Controller/cexporter.h"
#include <QGuiApplication>
class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(QObject *parent = nullptr);
    MainWindow m_mainWindow;
    Cwfdb *m_cwfdb = nullptr;
    CSignalView* m_csignalView = nullptr;
    CExporter* m_cexporter = nullptr;

signals:

public Q_SLOTS:
    void sltOpenRecord(const SignalViewParameters& params);
    void sltExportRequested(const QString& path);
};

#endif // CONTROLLER_H
