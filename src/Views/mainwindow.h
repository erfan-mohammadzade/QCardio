#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QVBoxLayout>
#include <Models/directoryvalidator.h>
#include <Views/signalviewwidget.h>
#include <QFileInfo>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setSignalWidget(const QList<SignalViewWidget *> widgetList);
    void updateRecordList(const QString &recordDirectory);


private slots:
    void on_pushButtonSetPath_clicked();

    void on_pushButtonRead_clicked();

    void on_listWidgetItems_currentRowChanged(int currentRow);

    void on_pushButtonExport_clicked();

private:
    Ui::MainWindow *ui;

    void setupSignal(QGridLayout* mainLayout, SignalViewWidget* signalWidget);
    int m_listItemIdx = 0;
    QStringList m_heaFilesWithPath;
    QStringList m_headerFilePath;

Q_SIGNALS:
    void sigReadDataRequested(const SignalViewParameters& params);
    void sigExportRequested(const QString& path);
};
#endif // MAINWINDOW_H
