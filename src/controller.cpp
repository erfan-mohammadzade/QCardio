#include "controller.h"

Controller::Controller(QObject *parent)
    : QObject{parent}
{
    m_mainWindow.show();
    connect(&m_mainWindow, &MainWindow::sigReadDataRequested,
            this, &Controller::sltOpenRecord);
    connect(&m_mainWindow, &MainWindow::sigExportRequested,
            this, &Controller::sltExportRequested);
    connect(this, &Controller::sigReadDataProcessEnd, this,[=]()
            {
                m_csignalView->setData(m_cwfdb->getStructData());
            });
    connect(this, &Controller::sigExportProcessEnd, this,[=]()
            {
                QMessageBox::information(nullptr, "Export", "Export Process Completed");
            });

    m_cwfdb = new Cwfdb(this);
    m_csignalView = new CSignalView(this);
    m_cexporter = new CExporter(this);
    m_mainWindow.setSignalWidget(m_csignalView->windgetList());
}

void Controller::sltOpenRecord(const SignalViewParameters& params)
{
    QtConcurrent::run(QThreadPool::globalInstance(),[=] {
        m_cwfdb->readData(params);
        Q_EMIT sigReadDataProcessEnd();
    });
}

void Controller::sltExportRequested(const QString &path)
{
    QtConcurrent::run(QThreadPool::globalInstance(),[=] {
        // m_cexporter->exportDataInSample(m_cwfdb->getStructData(), path);
        m_cexporter->exportDataInRC7(m_cwfdb->getStructData(), path);
        Q_EMIT sigExportProcessEnd();
    });
}

