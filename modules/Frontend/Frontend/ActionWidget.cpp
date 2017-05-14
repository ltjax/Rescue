#include "ActionWidget.hpp"
#include "ui_Action.h"
#include "AxisWidget.hpp"

ActionWidget::ActionWidget(std::shared_ptr<Action> action, QWidget* parent)
: QWidget(parent)
, mUi(std::make_unique<Ui::Action>())
, mAction(std::move(action))
{
	mUi->setupUi(this);
	mUi->addAxis->setDefaultAction(mUi->actionAdd_Axis);
	connect(mUi->actionAdd_Axis, &QAction::triggered, [this]()
	{
		OnAddAxis();
	});
	mAreaLayout = new QHBoxLayout();
	mAreaLayout->addStretch();
	mUi->axisArea->setLayout(mAreaLayout);
}

ActionWidget::~ActionWidget() = default;

void ActionWidget::OnAddAxis()
{
	auto axis = std::make_shared<Axis>("", Curve());
	mAction->addAxis(axis);
	auto widget = new AxisWidget(axis, mUi->axisArea);
	mAreaLayout->insertWidget(0, widget);
}
