#include "ActionWidget.hpp"
#include "ui_Action.h"
#include "AxisWidget.hpp"

ActionWidget::ActionWidget(QWidget* parent)
: QWidget(parent)
, mUi(std::make_unique<Ui::Action>())
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
	auto widget = new AxisWidget(mUi->axisArea);
	mAreaLayout->insertWidget(0, widget);
}
