#include "AxisWidget.hpp"
#include "ui_Axis.h"
#include <unordered_map>

namespace {


std::vector<std::pair<Curve::FunctionType, QString>> const typeToString = {
	{ Curve::FunctionType::Linear, "Linear" },
	{ Curve::FunctionType::Polynomial, "Polynomial" },
	{ Curve::FunctionType::Logistic, "Logistic" }
};

}

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

	for (int i = 0; i < typeToString.size(); ++i)
		mUi->type->addItem(typeToString[i].second, QVariant(static_cast<int>(typeToString[i].first)));

	auto currentIndexChanged = static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged);
	connect(mUi->type, currentIndexChanged, [this](int current)
	{
		if (current < typeToString.size())
			mUi->graphWidget->setCurve(mUi->graphWidget->getCurve().withType(typeToString[current].first));
			
	});
}

AxisWidget::~AxisWidget() = default;
