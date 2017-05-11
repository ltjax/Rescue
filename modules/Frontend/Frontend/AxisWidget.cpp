#include "AxisWidget.hpp"
#include "ui_Axis.h"

AxisWidget::AxisWidget(QWidget * parent)
: mUi(std::make_unique<Ui::Axis>())
{
	mUi->setupUi(this);

	auto valueChanged = static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged);
	connect(mUi->m, valueChanged, [this](double value)
	{
		mUi->graphWidget->setCurve(mUi->graphWidget->getCurve().withM(value));
	});
	connect(mUi->k, valueChanged, [this](double value)
	{
		mUi->graphWidget->setCurve(mUi->graphWidget->getCurve().withK(value));
	});
	connect(mUi->c, valueChanged, [this](double value)
	{
		mUi->graphWidget->setCurve(mUi->graphWidget->getCurve().withC(value));
	});
	connect(mUi->b, valueChanged, [this](double value)
	{
		mUi->graphWidget->setCurve(mUi->graphWidget->getCurve().withB(value));
	});
}

AxisWidget::~AxisWidget() = default;
