#include "cexporter.h"

CExporter::CExporter(QObject *parent)
    : QObject{parent}
{}

void CExporter::exportDataInSample(const MIT_BIH_ECGData &data, const QString& path)
{
    // Validate input
    if (data.totalSample <= 0 || data.nsigs.isEmpty()) {
        qWarning() << "Invalid data: no samples or leads";
        return;
    }

    // Determine how many leads to export (max 3)
    const int numLeads = qMin(3, data.nsigs.size());
    if (numLeads == 0) {
        qWarning() << "No leads available for export";
        return;
    }

    // Validate all lead vectors have the expected size
    for (int i = 0; i < numLeads; ++i) {
        if (data.nsigs[i].size() < data.totalSample) {
            qWarning() << "Lead" << i << "has insufficient samples";
            return;
        }
    }

    QVector<Sample> dataVec;
    dataVec.reserve(data.totalSample);

    for (int i = 0; i < data.totalSample; ++i) {
        Sample sample; // Creates a new sample each iteration
        // Initialize all leads to 0 (optional, but good practice)
        sample.leads[0] = 0;
        sample.leads[1] = 0;
        sample.leads[2] = 0;

        // Fill the sample with data from each lead
        for (int j = 0; j < numLeads; ++j) {
            sample.leads[j] = data.nsigs[j][i];
        }
        dataVec.append(sample);
    }

    // Generate filename with timestamp
    QString datestr = QDateTime::currentDateTime().toString("yyyyMMdd_hhmm");
    QString filePath = path + QDir::separator() + QString("ECG_Export_%1.bin").arg(datestr);

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        qWarning() << "Failed to open file for writing:" << filePath;
        return;
    }

    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_5_15); // Keep your original version

    // Write data
    for (const Sample& s : dataVec) {
        out << s;
    }

    file.close();

    qDebug() << "Exported" << dataVec.size() << "samples to" << filePath;
}
