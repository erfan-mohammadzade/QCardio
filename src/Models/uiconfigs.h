#ifndef UICONFIGS_H
#define UICONFIGS_H
#include <QString>
class UIConfigs
{
public:
    UIConfigs();
    QString dataBasePath() const;
    void setDataBasePath(const QString &newDataBasePath);

    QString dataBaseName() const;
    void setDataBaseName(const QString &newDataBaseName);

    int lineEditSignal1Index() const;
    void setLineEditSignal1Index(int newLineEditSignal1Index);

    int lineEditSignal2Index() const;
    void setLineEditSignal2Index(int newLineEditSignal2Index);

    int lineEditSignal3Index() const;
    void setLineEditSignal3Index(int newLineEditSignal3Index);

    bool exportRC7() const;
    void setExportRC7(bool newExportRC7);

    bool exportRaw() const;
    void setExportRaw(bool newExportRaw);

    float gainSignal() const;
    void setGainSignal(float newGainSignal);

    int signalOfset() const;
    void setSignalOfset(int newSignalOfset);

    int targetFreq() const;
    void setTargetFreq(int newTargetFreq);

    bool exportAll() const;
    void setExportAll(bool newExportAll);

private:
    QString m_dataBasePath;
    QString m_dataBaseName;
    int m_lineEditSignal1Index = 0;
    int m_lineEditSignal2Index = 0;
    int m_lineEditSignal3Index = 0;
    bool m_exportRC7 = false;
    bool m_exportRaw = false;
    bool m_exportAll = false;
    float m_gainSignal = 1.0;
    int m_signalOfset = 0;
    int m_targetFreq = 170;
};

#endif // UICONFIGS_H
