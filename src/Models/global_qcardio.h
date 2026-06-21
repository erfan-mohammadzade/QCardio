#ifndef GLOBAL_QCARDIO_H
#define GLOBAL_QCARDIO_H

#include <QString>
#include <QDir>
#include <QMessageBox>
#include <QFile>

enum DirectoryValidationFlags {
    Exists = 0x01,
    Readable = 0x02,
    Writable = 0x04,
    Executable = 0x08,
    IsDirectory = 0x10,
    NotEmpty = 0x20,
    IsAbsolute = 0x40
};

struct MIT_BIH_ECGData
{
    int totalSample = 0;
    QVector<QVector<qreal>> nsigs;
    void clear()
    {
        for(QVector<qreal> sig: nsigs)
        {
            sig.clear();
            sig.squeeze();
        }
        nsigs.clear();
    }
};

struct SignalViewParameters
{
    QString signalFilePath;
    QString DatabasePath;
    int targetFs = 178;
    float gain = 1.0;
    float offset = 0.0;
};

#endif // GLOBAL_QCARDIO_H
