#include "cwfdb.h"

Cwfdb::Cwfdb(QObject *parent)
    : QObject{parent}
{

}

Cwfdb::~Cwfdb()
{
    clearVec();
}

MIT_BIH_ECGData Cwfdb::getStructData() const
{
    return m_strData;
}

void Cwfdb::clearVec()
{
    if(signalInfo)
        delete signalInfo;
    if(sampleVector)
        delete sampleVector;
}

void Cwfdb::readData(const SignalViewParameters &params)
{
    clearVec();
    setwfdb(const_cast<char*>(params.DatabasePath.toStdString().c_str()));

    int numberOfSignals = 0;
    std::vector<WFDB_Siginfo> siginfo;
    // WFDB_Siginfo vahid[WFDB_MAXSIG];
    WFDB_Frequency target_fs = params.targetFs;
    setifreq(target_fs);
    numberOfSignals = isigopen(const_cast<char*>(params.signalFilePath.toStdString().c_str()), NULL, 0);

    if (numberOfSignals > 0) {
        siginfo.resize(numberOfSignals);

        if (isigopen(const_cast<char*>(params.signalFilePath.toStdString().c_str()), siginfo.data(), numberOfSignals) != numberOfSignals) {
            qDebug() << "Failed to read header";
            return;
        }

        long totalSamples = siginfo[0].nsamp;

        // Create a 2D vector to store all samples
        std::vector<std::vector<WFDB_Sample>> allData(
            numberOfSignals,
            std::vector<WFDB_Sample>(totalSamples)
            );

        // Read all samples
        for (long sampleIndex = 0; sampleIndex < totalSamples; ++sampleIndex) {
            WFDB_Sample* signalData = new WFDB_Sample[numberOfSignals];

            if (getvec(signalData) < 0) {
                qDebug() << "Error reading sample at index" << sampleIndex;
                delete[] signalData;
                break;
            }

            // CORRECTED: Use signalIndex to access signalData
            for (int signalIndex = 0; signalIndex < numberOfSignals; ++signalIndex) {
                allData[signalIndex][sampleIndex] = signalData[signalIndex];
            }

            delete[] signalData;
        }

        // QVector<Sample> sampleData

        // Now you can access all data
        QVector<qreal> nsig;
        QVector<QVector<qreal>> sigList;
        for (int signalIndex = 0; signalIndex < numberOfSignals; ++signalIndex) {
            nsig.clear();
            for (long sampleIndex = 0; sampleIndex < totalSamples; ++sampleIndex) {
                nsig.append((allData[signalIndex][sampleIndex]*params.gain)+params.offset);
            }
            sigList.append(nsig);
        }
        m_strData.totalSample = totalSamples;
        m_strData.nsigs = sigList;
    }
}


