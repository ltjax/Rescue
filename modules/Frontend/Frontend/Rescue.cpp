#include "Rescue.hpp"
#include "ui_Rescue.h"
#include "ActionWidget.hpp"
#include <QFileDialog>
#include "Domain/LoadSave.hpp"
#include <QMessageBox>

Rescue::Rescue()
: mUi(std::make_unique<Ui::MainWindow>())
{
	mUi->setupUi(this);

	connect(mUi->actionAdd_Action, &QAction::triggered, [this]()
	{
		onAddAction();
	});

	mAreaLayout = new QVBoxLayout();
	mAreaLayout->addStretch();
	mUi->actionArea->setLayout(mAreaLayout);

	connect(mUi->actionOpen, &QAction::triggered, [this] {onFileOpen(); });
	connect(mUi->actionNew, &QAction::triggered, [this] {onFileNew(); });
	connect(mUi->actionSave, &QAction::triggered, [this] {onFileSave(); });
	connect(mUi->actionSaveAs, &QAction::triggered, [this] {onFileSaveAs(); });
	//mUi->actionArea->setStyleSheet("background-color:white;");

	mGroup = std::make_shared<Group>();
}

Rescue::~Rescue() = default;

void Rescue::onAddAction()
{
	auto action = std::make_shared<Action>();
	mGroup->addAction(action);
	addActionWidget(action);
}

ActionWidget* Rescue::addActionWidget(std::shared_ptr<Action> const& action)
{
	ActionWidget* widget = new ActionWidget(action, mUi->actionArea);
	mAreaLayout->insertWidget(0, widget);
	return widget;
}

void Rescue::onFileSave()
{
}

void Rescue::onFileSaveAs()
{
	auto filename = QFileDialog::getSaveFileName(this, "Save");
	if (filename.isEmpty())
		return;

	if (mGroup == nullptr)
		return;
	
	catchAll([&]
	{
		LoadSave::saveTo(filename.toStdString(), mGroup);
	});
}

void Rescue::onFileOpen()
{
	auto filename = QFileDialog::getOpenFileName(this, "Open");
	if (filename.isEmpty())
		return;

	catchAll([&]
	{
		mGroup = LoadSave::loadFrom(filename.toStdString());
	});

	for (auto action : mGroup->getActionList())
	{
		auto actionWidget = addActionWidget(action);
		for (auto axis : action->getAxisList())
		{
			actionWidget->addAxisWidget(axis);
		}
	}
}

void Rescue::onFileNew()
{
}

void Rescue::catchAll(std::function<void()> rhs)
{
	try
	{
		rhs();
	}
	catch (std::exception const& e)
	{
		QMessageBox::critical(this, "Error", e.what());
	}
	catch (...)
	{
		QMessageBox::critical(this, "Error", "Unknown error");
	}
}
