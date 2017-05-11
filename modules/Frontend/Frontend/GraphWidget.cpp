#include "GraphWidget.hpp"
#include <QPainter>
#include <QPaintEvent>

GraphWidget::GraphWidget(QWidget* parent)
{
}

void GraphWidget::setAxis(std::shared_ptr<Axis const> axis)
{
	mAxis = std::move(axis);
	update();
}

void GraphWidget::paintEvent(QPaintEvent* e)
{
	QPainter painter(this);
	painter.fillRect(e->rect(), Qt::white);

	if (mAxis == nullptr)
		return;

	auto rect = this->rect();
	auto valueFor = [&](int pixel)
	{
		float ry = mAxis->EvaluateFor((pixel + 0.5f) / rect.width());
		return (1.f - ry) * rect.height();
	};

	int lastY = valueFor(0);
	for (int x = 1; x < rect.width(); ++x)
	{
		int y = valueFor(x);
		painter.drawLine(x - 1, lastY, x, y);
		lastY = y;		
	}
}
