#ifndef CEXPORTER_H
#define CEXPORTER_H

#include <QObject>
#include <Models/sample.h>
#include <Models/global_qcardio.h>
#include <QFileDialog>
class CExporter : public QObject
{
    Q_OBJECT
public:
    explicit CExporter(QObject *parent = nullptr);
    void exportDataInSample(const MIT_BIH_ECGData& data, const QString &path);
    bool exportDataInRC7(const MIT_BIH_ECGData& data, const QString &path);

signals:
};

#endif // CEXPORTER_H
