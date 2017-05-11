#pragma once
#include <QWidget>
#include <memory>
#include "Domain/Axis.hpp"

class GraphWidget : public QWidget
{
public:
	GraphWidget(QWidget* parent);

	void setCurve(Curve curve);
	Curve const& getCurve() const;


	void paintEvent(QPaintEvent* e) override;
private:
	Curve mCurve;
};
