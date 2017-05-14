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

AxisWidget::AxisWidget(std::shared_ptr<Axis> axis, QWidget * parent)
: mUi(std::make_unique<Ui::Axis>())
, mAxis(std::move(axis))
{
	mUi->setupUi(this);
	mUi->graphWidget->setCurve(mAxis->getCurve());

	using namespace std::placeholders;

	auto valueChanged = static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged);
	connect(mUi->m, valueChanged, directTo(&Curve::withM));
	connect(mUi->k, valueChanged, directTo(&Curve::withK));
	connect(mUi->c, valueChanged, directTo(&Curve::withC));
	connect(mUi->b, valueChanged, directTo(&Curve::withB));

	for (int i = 0; i < typeToString.size(); ++i)
		mUi->type->addItem(typeToString[i].second, QVariant(static_cast<int>(typeToString[i].first)));

	auto currentIndexChanged = static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged);
	connect(mUi->type, currentIndexChanged, [this](int current)
	{
		if (current < typeToString.size())
		{
			auto type = typeToString[current].first;
			modifyCurve(std::bind(&Curve::withType, _1, type));
		}			
	});
}

AxisWidget::~AxisWidget() = default;

void AxisWidget::modifyCurve(std::function<Curve(Curve)> modifier)
{
	auto newCurve = modifier(mAxis->getCurve());
	mUi->graphWidget->setCurve(newCurve);
	mAxis->setCurve(newCurve);
}
