#include "Rescue.hpp"
#include "ui_Rescue.h"

Rescue::Rescue()
: mUi(std::make_unique<Ui::MainWindow>())
{
	mUi->setupUi(this);

	connect(mUi->actionAdd_Action, &QAction::triggered, [this]()
	{
		OnAddAction();
	});
}

Rescue::~Rescue() = default;

void Rescue::OnAddAction()
{
}
