#include "RescueMainWindow.hpp"
#include "ActionWidget.hpp"
#include "Actions.hpp"
#include "Domain/LoadSave.hpp"
#include "ui_Rescue.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
#include <QStandardPaths>

namespace
{

auto const UTILITY_DEFINITION_FILE_FILTER = "Utility Definition XML (*.xml)";
}

RescueMainWindow::RescueMainWindow(view::component<Action, Model> connection)
: view::component<Action, Model>(connection)
, mUi(std::make_unique<Ui::MainWindow>())
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
    // mUi->actionArea->setStyleSheet("background-color:white;");

    connect_view([](Model const& model) { return model.Group; },
                 [this](auto group) { syncWidgets(group->getActionList()); });
}

RescueMainWindow::~RescueMainWindow() = default;

void RescueMainWindow::onAddAction()
{
    dispatch(::addAction());
    // addActionWidget(action);
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
    // setCurrentFilename(filename);
}

void RescueMainWindow::onFileOpen()
{
    auto path = getFilePath();
    auto filename = QFileDialog::getOpenFileName(this, "Open", path, UTILITY_DEFINITION_FILE_FILTER);
    if (filename.isEmpty())
        return;

    catchAll([&] { dispatch(loadDocumentFrom(filename.toStdString())); });
    // setCurrentFilename(filename);
    // syncWidgets();
}

void RescueMainWindow::saveTo(QString filename)
{
    // catchAll([&] { Rescue::LoadSave::saveTo(filename.toStdString(), mGroup); });
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

void RescueMainWindow::syncWidgets(Rescue::PtrList<Rescue::Action> const& actionList)
{
    clearActionWidgets();

    for (auto action : actionList)
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
    dispatch(newDocument());
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
