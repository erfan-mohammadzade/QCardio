#ifndef SIGNALVIEWWIDGET_H
#define SIGNALVIEWWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QVector>
#include <QTimer>
#include <QResizeEvent>
#include <QColor>
#include <QFont>
#include <cmath>

class SignalViewWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SignalViewWidget(QWidget *parent = nullptr);
    ~SignalViewWidget();

    // Configuration methods
    void setSignalColor(const QColor &color);
    void setGridColor(const QColor &color);
    void setBackgroundColor(const QColor &color);
    void setSignalName(const QString &name);
    void setTimeWindow(qreal seconds);
    void setAmplitudeRange(qreal min, qreal max);
    void setSampleRate(int samplesPerSecond);
    void setShowGrid(bool show);
    void setShowLabels(bool show);
    void setLineWidth(int width);

    // Data methods
    void addDataPoint(qreal value);
    void addDataPoints(const QVector<qreal> &values);
    void clearData();
    void setData(const QVector<qreal> &data);

    // ECG specific
    void setECGMode(bool enabled);
    void setHeartRate(int bpm);

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    // Data buffer
    QVector<qreal> m_data;
    int m_maxDataPoints;

    // Configuration
    QColor m_signalColor;
    QColor m_gridColor;
    QColor m_backgroundColor;
    QString m_signalName;

    qreal m_timeWindow;        // Seconds to display
    qreal m_amplitudeMin;
    qreal m_amplitudeMax;
    int m_sampleRate;          // Samples per second
    bool m_showGrid;
    bool m_showLabels;
    int m_lineWidth;
    bool m_ecgMode;
    int m_heartRate;
    int m_visibleTimeWindow = 20;

    // View control
    qreal m_timeOffset;        // For scrolling/panning
    qreal m_zoomLevel;
    QPoint m_lastMousePos;
    bool m_isPanning;

    // Helper methods
    void updateMaxDataPoints();
    QPointF dataToPixel(qreal time, qreal value) const;
    QPair<qreal, qreal> pixelToData(const QPoint &pixel) const;
    void drawGrid(QPainter &painter);
    void drawSignal(QPainter &painter);
    void drawLabels(QPainter &painter);
};

#endif // SIGNALVIEWWIDGET_H
