#include "cexporter.h"

CExporter::CExporter(QObject *parent)
    : QObject{parent}
{}

void CExporter::exportDataInSample(const MIT_BIH_ECGData &data, const QString& path)
{
    QVector<Sample> dataVec;
    dataVec.reserve(data.totalSample);

    Sample sample;
    for(int i = 0; i < data.totalSample; i++)
    {
        sample.leads[0] = data.nsigs[0][i];
        sample.leads[1] = data.nsigs[1][i];
        dataVec << sample;
    }

    // Open file - specify a proper path
    QFile file(path + QDir::separator() + "output.bin");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        // Handle error
        qWarning() << "Failed to open file for writing";
        return;
    }

    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_5_15); // Use appropriate version

    // Write using const reference to avoid copying
    for(const Sample& s : dataVec)
        out << s;

    file.close();
}
