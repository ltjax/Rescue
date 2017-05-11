#include "Rescue.hpp"
#include "ui_Rescue.h"
#include "ActionWidget.hpp"

Rescue::Rescue()
: mUi(std::make_unique<Ui::MainWindow>())
{
	mUi->setupUi(this);

	connect(mUi->actionAdd_Action, &QAction::triggered, [this]()
	{
		OnAddAction();
	});

	mAreaLayout = new QVBoxLayout();
	mAreaLayout->addStretch();
	mUi->actionArea->setLayout(mAreaLayout);

	//mUi->actionArea->setStyleSheet("background-color:white;");
}

Rescue::~Rescue() = default;

void Rescue::OnAddAction()
{
	ActionWidget* widget = new ActionWidget(mUi->actionArea);
	mAreaLayout->insertWidget(0, widget);
}
