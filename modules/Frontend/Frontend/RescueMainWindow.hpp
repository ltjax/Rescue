#pragma once
#include "Domain/Group.hpp"
#include "State.hpp"
#include "Vocabulary.hpp"
#include "state_observer.hpp"
#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QMainWindow>
#include <event_bus.hpp>
#include <functional>
#include <memory>
#include <boost/uuid/random_generator.hpp>

namespace Ui
{
class MainWindow;
}

namespace Rescue
{

class ActionWidget;
class RescueMainWindow : public QMainWindow
{
public:
    RescueMainWindow(Ptr<ushiro::event_bus> bus, ushiro::state_observer<State> observer);
    ~RescueMainWindow() final;

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
    void syncWidgets(Rescue::Group const& group);
    void catchAll(std::function<void()> rhs);
    std::unique_ptr<Ui::MainWindow> mUi;
    Ptr<ushiro::event_bus> mBus;
    QBoxLayout* mAreaLayout = nullptr;
    std::vector<ActionWidget*> mActionWidgetList;
    QString mCurrentFilename;

    std::shared_ptr<Rescue::Group> mGroup;
    boost::uuids::random_generator mUuidGenerator;
};
} // namespace Rescue