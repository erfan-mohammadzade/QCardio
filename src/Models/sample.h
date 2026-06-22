#ifndef SAMPLE_H
#define SAMPLE_H

#include <QByteArray>
#include <QDataStream>

// Define the count of leads (adjust based on your needs)
#ifndef SAVED_LEAD_COUNT
#define SAVED_LEAD_COUNT 3  // Change this to match your requirements
#endif

#include <QByteArray>
#include <QDataStream>
#include <QVector>

struct Sample
{
    qint16 leads[SAVED_LEAD_COUNT];
    qint16 status = 0;
    quint8 batteryLevel = 255;
    quint8 leadFailed = 0;

    inline void clear()
    {
        for (quint8 lIdx = 0; lIdx < SAVED_LEAD_COUNT; lIdx++)
        {
            leads[lIdx] = 0;
        }
        batteryLevel = 255;
        leadFailed = 0;
        status = 0;
    }

    // inline void toByteArray(QByteArray& bt) const
    // {
    //     bt.append(reinterpret_cast<const char*>(this), sizeof(*this));
    // }

    // inline void fromByteArray(const QByteArray& bt, quint64 sIdx)
    // {
    //     char* ptr = reinterpret_cast<char*>(this);
    //     for (quint8 i = 0; i < sizeof(*this); i++)
    //     {
    //         *(ptr++) = bt.at(i + sIdx);
    //     }
    // }

    friend QDataStream &operator<<(QDataStream &out, const Sample &myStruct) {
        for (int i = 0; i < SAVED_LEAD_COUNT; i++)
        {
            out << myStruct.leads[i];
        }
        out << myStruct.status << myStruct.batteryLevel << myStruct.leadFailed;
        return out;
    }

    friend QDataStream &operator>>(QDataStream &in, Sample &myStruct) {
        for (int i = 0; i < SAVED_LEAD_COUNT; i++)
        {
            in >> myStruct.leads[i];
        }
        in >> myStruct.status >> myStruct.batteryLevel >> myStruct.leadFailed;
        return in;
    }
};

struct HeaderInfo {
    int numberOfSignals;
    int samplingFrequency; // 360 Hz
    int numberOfSamples;
    QVector<Sample> data;

    friend QDataStream &operator>>(QDataStream &in, HeaderInfo &myStruct)
    {
        in >> myStruct.numberOfSamples;
        in >> myStruct.numberOfSignals;
        in >> myStruct.samplingFrequency;

        // Resize the vector before reading
        myStruct.data.resize(myStruct.numberOfSamples);

        // Read each Sample individually
        for(int i = 0; i < myStruct.numberOfSamples; i++)
            in >> myStruct.data[i];  // Index the vector element

        return in;
    }

    friend QDataStream &operator<<(QDataStream &out, const HeaderInfo &myStruct)
    {
        out << myStruct.numberOfSamples;
        out << myStruct.numberOfSignals;
        out << myStruct.samplingFrequency;

        // Write each Sample individually
        for(int i = 0; i < myStruct.numberOfSamples; i++)
            out << myStruct.data[i];  // Index the vector element

        return out;
    }
};

#endif // SAMPLE_H
