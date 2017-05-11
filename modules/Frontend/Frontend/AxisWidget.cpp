#include "AxisWidget.hpp"
#include "ui_Axis.h"

AxisWidget::AxisWidget(QWidget * parent)
: mUi(std::make_unique<Ui::Axis>())
{
	mUi->setupUi(this);
}

AxisWidget::~AxisWidget() = default;
