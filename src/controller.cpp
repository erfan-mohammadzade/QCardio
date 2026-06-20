#include "controller.h"

Controller::Controller(QObject *parent)
    : QObject{parent}
{
    m_mainWindow.show();
    connect(&m_mainWindow, &MainWindow::sigReadDataRequested,
            this, &Controller::sltOpenRecord);
    connect(&m_mainWindow, &MainWindow::sigExportRequested,
            this, &Controller::sltExportRequested);

    m_cwfdb = new Cwfdb(this);
    m_csignalView = new CSignalView(this);
    m_cexporter = new CExporter(this);
    m_mainWindow.setSignalWidget(m_csignalView->windgetList());
}

void Controller::sltOpenRecord(const SignalViewParameters& params)
{
    m_cwfdb->readData(params);
    m_csignalView->setData(m_cwfdb->getStructData());
}

void Controller::sltExportRequested(const QString &path)
{
    m_cexporter->exportDataInSample(m_cwfdb->getStructData(), path);
}
