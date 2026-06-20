#include "SignalViewWidget.h"
#include <QMouseEvent>
#include <QWheelEvent>
#include <QPainterPath>
#include <QLinearGradient>
#include <QDebug>
#include <QRandomGenerator>

SignalViewWidget::SignalViewWidget(QWidget *parent)
    : QWidget(parent)
    , m_signalColor(Qt::red)
    , m_gridColor(Qt::gray)
    , m_backgroundColor(Qt::black)
    , m_signalName("ECG Signal")
    , m_timeWindow(10.0)
    , m_amplitudeMin(0)
    , m_amplitudeMax(2000)
    , m_sampleRate(100)
    , m_showGrid(true)
    , m_showLabels(true)
    , m_lineWidth(2)
    , m_ecgMode(true)
    , m_heartRate(72)
    , m_timeOffset(0)
    , m_zoomLevel(1.0)
    , m_isPanning(false)
{
    // Initialize data buffer
    m_maxDataPoints = m_sampleRate * m_timeWindow;
    m_data.reserve(m_maxDataPoints);

    // Set widget properties
    setMinimumSize(600, 300);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setMouseTracking(true);

    // Initialize with some data
    for (int i = 0; i < m_maxDataPoints; ++i) {
        m_data.append(0);
    }
}

SignalViewWidget::~SignalViewWidget()
{
}

void SignalViewWidget::setSignalColor(const QColor &color)
{
    m_signalColor = color;
    update();
}

void SignalViewWidget::setGridColor(const QColor &color)
{
    m_gridColor = color;
    update();
}

void SignalViewWidget::setBackgroundColor(const QColor &color)
{
    m_backgroundColor = color;
    update();
}

void SignalViewWidget::setSignalName(const QString &name)
{
    m_signalName = name;
    update();
}

void SignalViewWidget::setTimeWindow(qreal seconds)
{
    m_timeWindow = seconds;
    updateMaxDataPoints();
    update();
}

void SignalViewWidget::setAmplitudeRange(qreal min, qreal max)
{
    m_amplitudeMin = min;
    m_amplitudeMax = max;
    update();
}

void SignalViewWidget::setSampleRate(int samplesPerSecond)
{
    m_sampleRate = samplesPerSecond;
    updateMaxDataPoints();
    update();
}

void SignalViewWidget::setShowGrid(bool show)
{
    m_showGrid = show;
    update();
}

void SignalViewWidget::setShowLabels(bool show)
{
    m_showLabels = show;
    update();
}

void SignalViewWidget::setLineWidth(int width)
{
    m_lineWidth = width;
    update();
}

void SignalViewWidget::setECGMode(bool enabled)
{
    m_ecgMode = enabled;
    update();
}

void SignalViewWidget::setHeartRate(int bpm)
{
    m_heartRate = bpm;
    update();
}

void SignalViewWidget::addDataPoint(qreal value)
{
    m_data.push_back(value);
    if (m_data.size() > m_maxDataPoints) {
        m_data.removeFirst();
    }
    update();
}

void SignalViewWidget::addDataPoints(const QVector<qreal> &values)
{
    for (qreal value : values) {
        m_data.push_back(value);
        if (m_data.size() > m_maxDataPoints) {
            m_data.removeFirst();
        }
    }
    update();
}

void SignalViewWidget::clearData()
{
    m_data.clear();
    m_data.fill(0, m_maxDataPoints);
    update();
}

void SignalViewWidget::setData(const QVector<qreal> &data)
{
    m_data.clear();
    m_data = data;
    update();
}

// 1. Fix data mapping to use actual time range
void SignalViewWidget::drawSignal(QPainter &painter)
{
    if (m_data.isEmpty()) return;

    painter.setPen(QPen(m_signalColor, m_lineWidth));
    QPainterPath path;
    bool firstPoint = true;

    qreal timeStep = 1.0 / m_sampleRate;

    // Find the start index based on time offset
    int startIndex = qMax(0, static_cast<int>(m_timeOffset * m_sampleRate));
    int endIndex = qMin(m_data.size(), static_cast<int>((m_timeOffset + m_visibleTimeWindow) * m_sampleRate) + 1);

    for (int i = startIndex; i < endIndex; ++i) {
        qreal time = i * timeStep;
        qreal value = m_data[i];

        // Convert to pixel coordinates with offset
        QPointF pixel = dataToPixel(time, value);

        // Skip off-screen points
        if (pixel.x() < -10 || pixel.x() > width() + 10) {
            if (!firstPoint) {
                // End the path if we're going off screen
                painter.drawPath(path);
                path = QPainterPath();
                firstPoint = true;
            }
            continue;
        }

        if (firstPoint) {
            path.moveTo(pixel);
            firstPoint = false;
        } else {
            path.lineTo(pixel);
        }
    }

    painter.drawPath(path);
}

// 3. Enable disabled features in paintEvent
void SignalViewWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.fillRect(rect(), m_backgroundColor);

    if (m_showGrid) {
        drawGrid(painter);
    }

    drawSignal(painter);

    // if (m_showLabels) {
    //     drawLabels(painter);      // Uncomment
    // }
}

void SignalViewWidget::updateMaxDataPoints()
{
    m_maxDataPoints = m_sampleRate * m_timeWindow;
    if (m_data.size() > m_maxDataPoints) {
        m_data.remove(0, m_data.size() - m_maxDataPoints);
    } else if (m_data.size() < m_maxDataPoints) {
        m_data.insert(0, m_maxDataPoints - m_data.size(), 0);
    }
}


QPointF SignalViewWidget::dataToPixel(qreal time, qreal value) const
{
    // Apply time offset to view a specific window of data
    qreal adjustedTime = time - m_timeOffset;

    // Only draw data within the visible window
    if (adjustedTime < 0 || adjustedTime > m_visibleTimeWindow) {
        return QPointF(-1000, -1000); // Off-screen
    }

    qreal pixelX = (adjustedTime / m_visibleTimeWindow) * width();
    qreal pixelY = height() - ((value - m_amplitudeMin) / (m_amplitudeMax - m_amplitudeMin)) * height();
    return QPointF(pixelX, pixelY);
}

QPair<qreal, qreal> SignalViewWidget::pixelToData(const QPoint &pixel) const
{
    qreal time = (pixel.x() / width()) * m_timeWindow;
    qreal value = m_amplitudeMin + (1.0 - pixel.y() / height()) * (m_amplitudeMax - m_amplitudeMin);
    return qMakePair(time, value);
}

void SignalViewWidget::drawGrid(QPainter &painter)
{
    painter.setPen(QPen(m_gridColor, 1, Qt::DotLine));

    // Vertical grid lines (time) - show from offset
    for (int i = 0; i <= m_visibleTimeWindow; ++i) {
        qreal relativeTime = (i / 10.0) * m_visibleTimeWindow;
        qreal absoluteTime = m_timeOffset + relativeTime;
        qreal x = (relativeTime / m_visibleTimeWindow) * width();
        painter.drawLine(x, 0, x, height());
    }

    // Horizontal grid lines (amplitude)
    for (int i = 0; i <= m_visibleTimeWindow; ++i) {
        qreal y = (i / 10.0) * height();
        painter.drawLine(0, y, width(), y);
    }
}

void SignalViewWidget::drawLabels(QPainter &painter)
{
    painter.setPen(QPen(Qt::white, 1));

    QFont font = painter.font();
    font.setPointSize(9);
    painter.setFont(font);

    // Title with time info
    painter.drawText(10, 25, m_signalName);
    painter.drawText(10, 45,
                     QString("Time: %1s - %2s").arg(m_timeOffset, 0, 'f', 1)
                         .arg(m_timeOffset + m_visibleTimeWindow, 0, 'f', 1));

    // Time labels with absolute times
    for (int i = 0; i <= 5; ++i) {
        qreal relativeTime = (i / 5.0) * m_visibleTimeWindow;
        qreal absoluteTime = m_timeOffset + relativeTime;
        qreal x = (relativeTime / m_visibleTimeWindow) * width();
        painter.drawText(x - 15, height() - 5, QString::number(absoluteTime, 'f', 1) + "s");
    }

    // Amplitude labels
    for (int i = -2; i <= 2; ++i) {
        if (i == 0) continue;
        qreal value = i * 0.5;
        qreal y = height() - ((value - m_amplitudeMin) / (m_amplitudeMax - m_amplitudeMin)) * height();
        if (y >= 0 && y <= height()) {
            painter.drawText(5, y + 3, QString::number(value, 'f', 1));
        }
    }

    // Show current values
    if (!m_data.isEmpty()) {
        qreal currentValue = m_data.last();
        qreal currentTime = m_data.size() / static_cast<qreal>(m_sampleRate);
        painter.drawText(width() - 150, 25,
                         QString("Value: %1 mV").arg(currentValue, 0, 'f', 2));
        painter.drawText(width() - 150, 45,
                         QString("HR: %1 BPM").arg(m_heartRate));
        painter.drawText(width() - 150, 65,
                         QString("Time: %1s").arg(currentTime, 0, 'f', 1));
    }
}

void SignalViewWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    update();
}

void SignalViewWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_isPanning = true;
        m_lastMousePos = event->pos();
        setCursor(Qt::ClosedHandCursor);
    }
    QWidget::mousePressEvent(event);
}

void SignalViewWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_isPanning = false;
        setCursor(Qt::ArrowCursor);
    }
    QWidget::mouseReleaseEvent(event);
}

void SignalViewWidget::wheelEvent(QWheelEvent *event)
{
    // Pan left/right with the wheel
    qreal delta = event->angleDelta().y();

    // Adjust the scroll speed based on current visible window
    qreal scrollSpeed = m_visibleTimeWindow * 0.1;

    if (delta > 0) {
        // Scroll left (move to older data)
        m_timeOffset -= scrollSpeed;
    } else if (delta < 0) {
        // Scroll right (move to newer data)
        m_timeOffset += scrollSpeed;
    }

    // Clamp time offset to valid range
    qreal maxTimeOffset = qMax(0.0, (m_data.size() / static_cast<qreal>(m_sampleRate)) - m_visibleTimeWindow);
    m_timeOffset = qBound(0.0, m_timeOffset, maxTimeOffset);

    // Update the display
    update();
    event->accept();
}
