#pragma once
#include <QWidget>
#include <memory>
#include "Domain/Axis.hpp"

class GraphWidget : public QWidget
{
public:
	GraphWidget(QWidget* parent);

	void setAxis(std::shared_ptr<Axis const> axis);


	void paintEvent(QPaintEvent* e) override;
private:
	std::shared_ptr<Axis const> mAxis;
};
