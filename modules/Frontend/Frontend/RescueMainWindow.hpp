#pragma once
#include "Domain/Group.hpp"
#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QMainWindow>
#include <functional>
#include <memory>

namespace Ui
{
class MainWindow;
}

class ActionWidget;
class RescueMainWindow : public QMainWindow
{
public:
    RescueMainWindow();
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
    void syncWidgets();
    void catchAll(std::function<void()> rhs);
    std::unique_ptr<Ui::MainWindow> mUi;
    QBoxLayout* mAreaLayout = nullptr;
    std::vector<ActionWidget*> mActionWidgetList;
    QString mCurrentFilename;

    std::shared_ptr<Rescue::Group> mGroup;
};