#include "MainWindow.hpp"
#include "ActionWidget.hpp"
#include "LoadSave.hpp"
#include "ui_Rescue.h"
#include "InputPanelListWidget.hpp"
#include "OutputListWidget.hpp"
#include <QtCore/QSettings>
#include <QtCore/QStandardPaths>
#include <QtWidgets/QAction>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>
#include <memory>

using namespace Rescue;

namespace
{

auto const UTILITY_DEFINITION_FILE_FILTER = "Utility Definition XML (*.xml)";
}

MainWindow::MainWindow(Ptr<ushiro::event_bus> bus, ushiro::state_observer<State> observer)
: mUi(std::make_unique<Ui::MainWindow>())
, mBus(std::move(bus))
, mObserver(std::move(observer))
{
  mUi->setupUi(this);

  connect(mUi->actionAdd_Action, &QAction::triggered, [this]() { onAddAction(); });

  mAreaLayout = new QVBoxLayout();
  mAreaLayout->addStretch();
  mUi->actionArea->setLayout(mAreaLayout);

  connect(mUi->actionOpen, &QAction::triggered, [this] { onFileOpen(); });
  connect(mUi->actionNew, &QAction::triggered, [this] { onFileNew(); });
  connect(mUi->actionSave, &QAction::triggered, [this] { onFileSave(); });
  connect(mUi->actionSaveAs, &QAction::triggered, [this] { onFileSaveAs(); });
  connect(mUi->actionAdd_Input, &QAction::triggered, [this] { onAddInput(); });

  mUi->inputScrollArea->setWidget(new InputPanelListWidget(mBus, mObserver, this));
  mUi->outputScrollArea->setWidget(new OutputListWidget(mObserver, this));

  mObserver.observe(
    [](State const& state) {
      // Get the actions;
      return std::tie(state.group);
    },
    [this](Rescue::Group const& group) { syncWidgets(group); });
}

MainWindow::~MainWindow() = default;

void MainWindow::onAddAction()
{
  mBus->dispatch<Events::AddAction>(createId());
}

void MainWindow::onFileSave()
{
  if (mCurrentFilename.isEmpty())
  {
    onFileSaveAs();
    return;
  }

  saveTo(mCurrentFilename);
}

void MainWindow::onFileSaveAs()
{
  auto path = getFilePath();
  auto filename = QFileDialog::getSaveFileName(this, "Save", path, UTILITY_DEFINITION_FILE_FILTER);
  if (filename.isEmpty())
    return;

  saveTo(filename);
  setCurrentFilename(filename);
}

void MainWindow::onFileOpen()
{
  auto path = getFilePath();
  auto filename = QFileDialog::getOpenFileName(this, "Open", path, UTILITY_DEFINITION_FILE_FILTER);
  if (filename.isEmpty())
    return;

  catchAll([&] { mBus->dispatch<Events::LoadFrom>(filename.toStdString()); });
  setCurrentFilename(filename);
}

void MainWindow::saveTo(QString filename)
{
  catchAll([&] { mBus->dispatch<Events::SaveTo>(filename.toStdString()); });
}

void MainWindow::setCurrentFilename(QString filename)
{
  QSettings settings;
  settings.setValue("lastFile", filename);
  mCurrentFilename = filename;
  this->setWindowTitle(QString("Rescue (%1)").arg(filename));
}

QString MainWindow::getFilePath() const
{
  QSettings settings;
  auto result = settings.value("lastFile").toString();
  if (!result.isEmpty())
  {
    result = QFileInfo(result).path();
  }
  else
  {
    result = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
  }

  return result;
}

void MainWindow::syncWidgets(Rescue::Group const& group)
{
  auto insert = [this](auto const& item, auto index) {
    auto widget = new ActionWidget(mBus, mObserver, item->id, mUi->actionArea);
    mAreaLayout->insertWidget(index, widget);
    return widget;
  };

  mActionWidgetList.update(group, insert);
}

void MainWindow::onFileNew()
{
  mBus->dispatch<Events::NewFile>();
}

void MainWindow::catchAll(std::function<void()> rhs)
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

void MainWindow::onAddInput()
{
  mBus->dispatch<Events::CreateActionInput>(createId());
}
