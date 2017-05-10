#include "Rescue.hpp"
#include "ui_Rescue.h"
#include "ActionWidget.hpp"
#include <QBoxLayout>

Rescue::Rescue()
: mUi(std::make_unique<Ui::MainWindow>())
{
	mUi->setupUi(this);

	connect(mUi->actionAdd_Action, &QAction::triggered, [this]()
	{
		OnAddAction();
	});

	mUi->actionArea->setLayout(new QVBoxLayout());
}

Rescue::~Rescue() = default;

void Rescue::OnAddAction()
{
	ActionWidget* widget = new ActionWidget(mUi->actionArea);
	widget->show();
	mUi->actionArea->layout()->addWidget(widget);
}
