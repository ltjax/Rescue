#include "RescueMainWindow.hpp"
#include "ActionWidget.hpp"
#include "Domain/LoadSave.hpp"
#include "ui_Rescue.h"
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>
#include <QtCore/QSettings>
#include <QtCore/QStandardPaths>
#include <memory>
#include <QtWidgets/QAction>

using namespace Rescue;

namespace
{

auto const UTILITY_DEFINITION_FILE_FILTER = "Utility Definition XML (*.xml)";
}

RescueMainWindow::RescueMainWindow(Ptr<ushiro::event_bus> bus, ushiro::state_observer<State> observer)
: mUi(std::make_unique<Ui::MainWindow>())
, mBus(std::move(bus))
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

#ifdef TODO
    observer.observe([](State const& state) {return state.group;},
      [this](Rescue::Group const& group) {syncWidgets(group);});
#endif

    mGroup = std::make_shared<Rescue::Group>();
}

RescueMainWindow::~RescueMainWindow() = default;

void RescueMainWindow::onAddAction()
{
    mBus->dispatch<Events::AddAction>(mUuidGenerator());
}

ActionWidget* RescueMainWindow::addActionWidget(std::shared_ptr<Rescue::Action> const& action)
{
    ActionWidget* widget = new ActionWidget(action, mUi->actionArea);
    mAreaLayout->insertWidget(0, widget);
    mActionWidgetList.push_back(widget);
    return widget;
}

void RescueMainWindow::clearActionWidgets()
{
    for (auto widget : mActionWidgetList)
    {
        delete widget;
    }
    mActionWidgetList.clear();
}

void RescueMainWindow::onFileSave()
{
    if (mCurrentFilename.isEmpty())
    {
        onFileSaveAs();
        return;
    }

    saveTo(mCurrentFilename);
}

void RescueMainWindow::onFileSaveAs()
{
    auto path = getFilePath();
    auto filename = QFileDialog::getSaveFileName(this, "Save", path, UTILITY_DEFINITION_FILE_FILTER);
    if (filename.isEmpty())
        return;

    saveTo(filename);
    setCurrentFilename(filename);
}

void RescueMainWindow::onFileOpen()
{
    auto path = getFilePath();
    auto filename = QFileDialog::getOpenFileName(this, "Open", path, UTILITY_DEFINITION_FILE_FILTER);
    if (filename.isEmpty())
        return;

#ifdef TODO
    catchAll([&] { mGroup = Rescue::LoadSave::loadFrom(filename.toStdString()); });
    setCurrentFilename(filename);
    syncWidgets();
#endif
}

void RescueMainWindow::saveTo(QString filename)
{
#ifdef TODO
    catchAll([&] { Rescue::LoadSave::saveTo(filename.toStdString(), mGroup); });
#endif
}

void RescueMainWindow::setCurrentFilename(QString filename)
{
    QSettings settings;
    settings.setValue("lastFile", filename);
    mCurrentFilename = filename;
    this->setWindowTitle(QString("Rescue (%1)").arg(filename));
}

QString RescueMainWindow::getFilePath() const
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

void RescueMainWindow::syncWidgets(Rescue::Group const& group)
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

void RescueMainWindow::onFileNew()
{
    mGroup = std::make_shared<Rescue::Group>();

#ifdef TODO
    syncWidgets();
#endif
}

void RescueMainWindow::catchAll(std::function<void()> rhs)
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
