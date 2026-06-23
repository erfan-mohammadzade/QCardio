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

            if(data.selectedLead[j] > 2)
                sample.leads[j] = 0;
            else
                sample.leads[data.selectedLead[j]] = data.nsigs[j][i];
        }
        dataVec.append(sample);
    }

    // Generate filename with timestamp
    QString filePath = path + QDir::separator() + QString("%1_%2.bin").arg(data.dbName, data.filename);

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

bool CExporter::exportDataInRC7(const MIT_BIH_ECGData &data, const QString &path)
{
    if (data.nsigs.isEmpty())
    {
        qDebug() << "No sample data to save";
        return false;
    }

    const int numLeads = qMin(3, data.nsigs.size());
    if (numLeads == 0) {
        qWarning() << "No leads available for export";
        return false;
    }

    QString filePath = path + QDir::separator() + QString("%1_%2.rc7").arg(data.dbName, data.filename);

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        qWarning() << "Failed to open file for writing:" << filePath;
        return false;
    }

    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_5_15);

    // 1. Write header/magic number
    out << quint32(0x53414D50); // "SAMP"
    out << quint32(0x00000001); // Version

    // 2. Write metadata
    out << data.dbName;
    out << data.totalSample;
    out << data.sampling;
    out << data.recordDate;

    // 3. Write sample count
    quint32 sampleCount = data.totalSample;
    out << sampleCount;

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

            if(data.selectedLead[j] > 2)
                sample.leads[j] = 0;
            else
                sample.leads[data.selectedLead[j]] = data.nsigs[j][i];
        }
        dataVec.append(sample);
    }

    // 4. Write all samples in binary format
    for (const Sample& sample : dataVec) {
        out << sample.leads[0];
        out << sample.leads[1];
        out << sample.leads[2];
        out << sample.status;
        out << sample.leadFailed;
        out << sample.batteryLevel;
    }

    // 5. Write ZERO for diary events
    out << quint32(0); // No diary events

    // 6. Write ZERO for paced events
    out << quint32(0); // No paced events

    file.close();
    qDebug() << "Sample data saved successfully:" << filePath;
    qDebug() << "Samples saved:" << sampleCount;

    return true;
}
