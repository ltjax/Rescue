#include "GraphWidget.hpp"
#include <QtGui/QPaintEvent>
#include <QtGui/QPainter>

using namespace Rescue;

GraphWidget::GraphWidget(QWidget* parent)
{
    setMouseTracking(true);
}

void GraphWidget::setRangedCurve(RangedCurve curve)
{
    mCurve = std::move(curve);
    update();
}

RangedCurve const& GraphWidget::getRangedCurve() const
{
    return mCurve;
}

void GraphWidget::mouseMoveEvent(QMouseEvent* e)
{
    mMouseInput = e->pos().x() / static_cast<float>(rect().width());
    update();
}

void GraphWidget::leaveEvent(QEvent* e)
{
    mMouseInput.reset();
    update();
}

void GraphWidget::paintEvent(QPaintEvent* e)
{
    QPainter painter(this);
    painter.fillRect(e->rect(), Qt::white);

    auto rect = this->rect();
    auto valueFor = [&](int pixel) {
        float ry = mCurve.getCurve().evaluateFor((pixel + 0.5f) / rect.width());
        return (1.f - ry) * rect.height();
    };

    int lastY = valueFor(0);
    for (int x = 1; x < rect.width(); ++x)
    {
        int y = valueFor(x);
        painter.drawLine(x - 1, lastY, x, y);
        lastY = y;
    }

    if (mMouseInput)
    {
        auto x = *mMouseInput;
        int px = x * rect.width();
        int py = valueFor(px);
        painter.drawEllipse({ px, py }, 5, 5);

        float rangedX = x * (mCurve.getMax() - mCurve.getMin()) + mCurve.getMin();
        auto text =
          QString("%1 -> %2").arg(rangedX, 4, 'f', 2).arg(mCurve.getCurve().evaluateFor(x), 4, 'f', 2);
        painter.drawText(rect, Qt::AlignTop | Qt::AlignLeft, text);
    }

    if (mSimulatedInput)
    {
        float x = *mSimulatedInput;
        float normalized = (*mSimulatedInput - mCurve.getMin()) / (mCurve.getMax() - mCurve.getMin());
        int px = normalized * rect.width();
        int py = valueFor(px);
        painter.drawEllipse({ px, py }, 5, 5);

        auto text =
          QString("%1 -> %2").arg(x, 4, 'f', 2).arg(mCurve.getCurve().evaluateFor(normalized), 4, 'f', 2);
        painter.drawText(rect, Qt::AlignTop | Qt::AlignRight, text);
    }
}

void GraphWidget::setSimulatedInput(GraphWidget::Optional<float> input)
{
    mSimulatedInput = std::move(input);
    update();
}
