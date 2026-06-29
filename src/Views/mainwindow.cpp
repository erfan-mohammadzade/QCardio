#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QCheckBox>
#include <QComboBox>
#include <QSlider>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    updateRecordList(ui->lineEditPath->text());
    loadStyle();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButtonSetPath_clicked()
{
    QString path = QFileDialog::getExistingDirectory(nullptr,
                                                     "Select Database Path",
                                                     QDir::homePath());
    updateRecordList(path);
    ui->lineEditPath->setText(path);
}


void MainWindow::on_pushButtonRead_clicked()
{
    enableUIBtn(false);

    QString path = ui->lineEditPath->text();
    if(DirectoryValidator::validateDirectory(path))
    {
        SignalViewParameters params = readSignalSetting();
        Q_EMIT sigReadDataRequested(params);
    }
    else
        QMessageBox::critical(nullptr, "Database Path", "Path is not readable");
}

void MainWindow::setupSignal(QGridLayout *mainLayout, SignalViewWidget* signalView)
{
    signalView->setMinimumHeight(200);
    mainLayout->addWidget(signalView);
}

ExprotSetting::ExportMethod MainWindow::getExportMethod()
{
    if(ui->radioButtonRec7->isChecked())
        return ExprotSetting::ExportMethod::RC7;
    else if(ui->radioButtonRawSample->isChecked())
        return ExprotSetting::ExportMethod::RawSample;
}

SignalViewParameters MainWindow::readSignalSetting()
{
    SignalViewParameters params;
    params.dbPath = ui->lineEditPath->text();
    params.signalFilePath = m_heaFilesWithPath.at(m_listItemIdx);
    params.targetFs = ui->spinBoxTargetFreq->value();
    params.gain = ui->doubleSpinBoxGain->value();
    params.offset = ui->spinBoxOffset->value();
    params.selectedLead[0] = ui->comboBoxSignal1->currentIndex();
    params.selectedLead[1] = ui->comboBoxSignal2->currentIndex();
    params.selectedLead[2] = ui->comboBoxSignal3->currentIndex();
    params.dbName = ui->comboBoxDatabaseName->currentText();
    return params;
}

void MainWindow::setSignalWidget(const QList<SignalViewWidget*> widgetList)
{
    setupSignal(ui->gridLayoutLead1, widgetList[0]);
    setupSignal(ui->gridLayoutLead2, widgetList[1]);
    setupSignal(ui->gridLayoutLead2, widgetList[2]);
}

void MainWindow::updateRecordList(const QString& recordDirectory)
{
    if(!DirectoryValidator::validateDirectory(recordDirectory))
        return;

    QDir dir(recordDirectory);
    m_heaFilesWithPath.clear();
    m_headerFilePath.clear();
    ui->listWidgetItems->clear();
    QFileInfoList infoList = dir.entryInfoList();
    for (const QFileInfo& info : std::as_const(infoList)) {
        if (info.suffix() == "hea") {
            m_heaFilesWithPath << info.fileName().split(".").first();
            m_headerFilePath << info.filePath();
        }
    }
    ui->listWidgetItems->addItems(m_heaFilesWithPath);
}

void MainWindow::loadStyle()
{
    QFile styleFile(":/Res/style/style.qss");
    styleFile.open(QFile::ReadOnly);
    QString style = QLatin1String(styleFile.readAll());
    setStyleSheet(style);
}

void MainWindow::enableUIBtn(const bool& isEnable)
{
    ui->pushButtonExport->setEnabled(isEnable);
    ui->pushButtonRead->setEnabled(isEnable);
    ui->pushButtonUpdate->setEnabled(isEnable);
}

void MainWindow::on_listWidgetItems_currentRowChanged(int currentRow)
{
    // Handle invalid selection
    if (currentRow < 0 || currentRow >= m_headerFilePath.size()) {
        ui->textEditSignalInfo->clear();
        m_listItemIdx = -1;
        return;
    }

    m_listItemIdx = currentRow;

    QFile file(m_headerFilePath.at(currentRow));

    if (!file.open(QIODevice::ReadOnly)) {
        // Handle error - show message or clear text
        ui->textEditSignalInfo->clear();
        ui->textEditSignalInfo->setText("Error: Could not open file");
        return;
    }

    QByteArray data = file.readAll();
    file.close();

    ui->textEditSignalInfo->clear();
    ui->textEditSignalInfo->setText(data);
}

void MainWindow::on_pushButtonExport_clicked()
{
    QString path = QFileDialog::getExistingDirectory(nullptr, "Select Directory To Export", QDir::homePath());
    if(DirectoryValidator::validateDirectory(path))
    {
        enableUIBtn(false);
        ExprotSetting expSetting;
        expSetting.method = getExportMethod();
        expSetting.params = readSignalSetting();
        expSetting.outputPath = path;
        expSetting.pathList = m_heaFilesWithPath;
        expSetting.compressingRequsted = ui->checkBoxCompression->isChecked();
        if(!ui->checkBoxExportAllData->isChecked())
            Q_EMIT sigExportRequested(path);
        else
        {
            Q_EMIT sigExportAllRequested(expSetting);
        }
    }
    else
        QMessageBox::critical(nullptr, "Dir Validation", "Dir is invalid");

}


void MainWindow::on_pushButtonUpdate_clicked()
{
    on_pushButtonRead_clicked();
}

