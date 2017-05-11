#include "AxisWidget.hpp"
#include "ui_Axis.h"

AxisWidget::AxisWidget(QWidget * parent)
: mUi(std::make_unique<Ui::Axis>())
{
	mUi->setupUi(this);
	auto axis = std::make_shared<Axis>("", Axis::CurveType::Linear, 1.f, 1.f, 0.f, 0.f);
	mUi->graphWidget->setAxis(axis);

	auto valueChanged = static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged);
	connect(mUi->mSpinner, valueChanged, [this](double value)
	{
		auto axis = std::make_shared<Axis>("", Axis::CurveType::Linear, value, 1.f, 0.f, 0.f);
		mUi->graphWidget->setAxis(axis);
	});
}

AxisWidget::~AxisWidget() = default;
