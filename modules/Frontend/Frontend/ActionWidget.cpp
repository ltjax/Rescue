#include "ActionWidget.hpp"
#include "ui_Action.h"

ActionWidget::ActionWidget(QWidget* parent)
: QWidget(parent)
, mUi(std::make_unique<Ui::Action>())
{
	mUi->setupUi(this);
}

ActionWidget::~ActionWidget() = default;
