#include "AxisWidget.hpp"
#include "ui_Axis.h"
#include <unordered_map>

using namespace Rescue;

namespace
{

std::vector<std::pair<Curve::FunctionType, QString>> const typeToString = {
    { Curve::FunctionType::Linear, "Linear" },
    { Curve::FunctionType::Polynomial, "Polynomial" },
    { Curve::FunctionType::Logistic, "Logistic" }
};
}

AxisWidget::AxisWidget(std::shared_ptr<Axis> axis, QWidget* parent)
: mUi(std::make_unique<Ui::Axis>())
, mAxis(std::move(axis))
{
    mUi->setupUi(this);
    mUi->graphWidget->setCurve(mAxis->getCurve());
    mUi->input->setText(mAxis->getInput().c_str());

    using namespace std::placeholders;

    auto valueChanged = static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged);
    connect(mUi->m, valueChanged, directTo(&Curve::withM));
    mUi->m->setValue(mAxis->getCurve().m());
    connect(mUi->k, valueChanged, directTo(&Curve::withK));
    mUi->k->setValue(mAxis->getCurve().k());
    connect(mUi->c, valueChanged, directTo(&Curve::withC));
    mUi->c->setValue(mAxis->getCurve().c());
    connect(mUi->b, valueChanged, directTo(&Curve::withB));
    mUi->b->setValue(mAxis->getCurve().b());

    for (std::size_t i = 0; i < typeToString.size(); ++i)
        mUi->type->addItem(typeToString[i].second, QVariant(static_cast<int>(typeToString[i].first)));

    auto currentIndexChanged = static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged);
    connect(mUi->type, currentIndexChanged, [this](int current) {
        if (current < static_cast<int>(typeToString.size()))
        {
            auto type = typeToString[current].first;
            modifyCurve(std::bind(&Curve::withType, _1, type));
        }
    });

    connect(mUi->input, &QLineEdit::textEdited, [this](QString text) { mAxis->setInput(text.toStdString()); });

    connect(mUi->min, valueChanged, [this](double rhs) { mAxis->setMin(rhs); });
    mUi->min->setValue(mAxis->getMin());

    connect(mUi->max, valueChanged, [this](double rhs) { mAxis->setMax(rhs); });
    mUi->max->setValue(mAxis->getMax());
}

AxisWidget::~AxisWidget() = default;

void AxisWidget::modifyCurve(std::function<Curve(Curve)> modifier)
{
    auto newCurve = modifier(mAxis->getCurve());
    mUi->graphWidget->setCurve(newCurve);
    mAxis->setCurve(newCurve);
}
