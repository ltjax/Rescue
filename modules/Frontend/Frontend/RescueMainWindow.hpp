#pragma once
#include "Domain/Group.hpp"
#include "Model.hpp"
#include "view_binder.hpp"
#include <QBoxLayout>
#include <QMainWindow>
#include <functional>
#include <memory>

namespace Ui
{
class MainWindow;
}

class ActionWidget;
class RescueMainWindow : public QMainWindow, public view::component<Action, Model>
{
public:
    explicit RescueMainWindow(view::component<Action, Model> connection);
    ~RescueMainWindow();

    void onAddAction();

    ActionWidget* addActionWidget(std::shared_ptr<Rescue::Action> const& action);
    void clearActionWidgets();

    void onFileSave();
    void onFileSaveAs();
    void onFileOpen();
    void onFileNew();

private:
    void saveTo(QString filename);
    void setCurrentFilename(QString filename);
    QString getFilePath() const;
    void syncWidgets(Rescue::PtrList<Rescue::Action> const& actionList);
    void catchAll(std::function<void()> rhs);
    std::unique_ptr<Ui::MainWindow> mUi;
    QBoxLayout* mAreaLayout = nullptr;
    std::vector<ActionWidget*> mActionWidgetList;
    QString mCurrentFilename;
};