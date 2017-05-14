#include "Rescue.hpp"
#include "ui_Rescue.h"
#include "ActionWidget.hpp"
#include <QFileDialog>
#include "Domain/LoadSave.hpp"
#include <QMessageBox>
#include <QStandardPaths>

namespace
{

auto const UTILITY_DEFINITION_FILE_FILTER = "Utility Definition XML (*.xml)";
}

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
	mActionWidgetList.push_back(widget);
	return widget;
}

void Rescue::clearActionWidgets()
{
	for (auto widget : mActionWidgetList)
	{
		delete widget;
	}
	mActionWidgetList.clear();
}

void Rescue::onFileSave()
{
	if (mCurrentFilename.isEmpty())
	{
		onFileSaveAs();
		return;
	}

	saveTo(mCurrentFilename);
}

void Rescue::onFileSaveAs()
{
	auto path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
	auto filename = QFileDialog::getSaveFileName(this, "Save", path, UTILITY_DEFINITION_FILE_FILTER);
	if (filename.isEmpty())
		return;
	
	saveTo(filename);
	setCurrentFilename(filename);
}

void Rescue::onFileOpen()
{
	auto path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
	auto filename = QFileDialog::getOpenFileName(this, "Open", path, UTILITY_DEFINITION_FILE_FILTER);
	if (filename.isEmpty())
		return;

	catchAll([&]
	{
		mGroup = LoadSave::loadFrom(filename.toStdString());
	});
	setCurrentFilename(filename);
	syncWidgets();
}

void Rescue::saveTo(QString filename)
{
	catchAll([&]
	{
		LoadSave::saveTo(filename.toStdString(), mGroup);
	});
}

void Rescue::setCurrentFilename(QString filename)
{
	mCurrentFilename = filename;
	this->setWindowTitle(QString("Rescue (%1)").arg(filename));
}

void Rescue::syncWidgets()
{
	clearActionWidgets();

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
	mGroup = std::make_shared<Group>();
	syncWidgets();
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
