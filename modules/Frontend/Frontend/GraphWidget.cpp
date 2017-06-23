#include "GraphWidget.hpp"
#include <QPainter>
#include <QPaintEvent>

GraphWidget::GraphWidget(QWidget* parent)
{
    setMouseTracking(true);
}

void GraphWidget::setCurve(Curve curve)
{
	mCurve = std::move(curve);
	update();
}

Curve const & GraphWidget::getCurve() const
{
	return mCurve;
}

void GraphWidget::mouseMoveEvent(QMouseEvent* e)
{
    mCurrentX = e->pos().x() / static_cast<float>(rect().width());
    update();
}

void GraphWidget::leaveEvent(QEvent* e)
{
    mCurrentX.reset();
    update();
}

void GraphWidget::paintEvent(QPaintEvent* e)
{
	QPainter painter(this);
	painter.fillRect(e->rect(), Qt::white);
	
	auto rect = this->rect();
	auto valueFor = [&](int pixel)
	{
		float ry = mCurve.evaluateFor((pixel + 0.5f) / rect.width());
		return (1.f - ry) * rect.height();
	};

	int lastY = valueFor(0);
	for (int x = 1; x < rect.width(); ++x)
	{
		int y = valueFor(x);
		painter.drawLine(x - 1, lastY, x, y);
		lastY = y;		
	}

    if (mCurrentX)
    {
        int px = *mCurrentX * rect.width();
        int py = valueFor(px);
        painter.drawEllipse({ px, py }, 5, 5);
        auto text = QString("%1 -> %2").arg(*mCurrentX, 4, 'f', 2).arg(mCurve.evaluateFor(*mCurrentX), 4, 'f', 2);
        painter.drawText(rect, Qt::AlignTop|Qt::AlignLeft, text);
    }
}
