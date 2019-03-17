#pragma once
#include "State.hpp"
#include "Vocabulary.hpp"
#include "diffable_list.hpp"
#include "state_observer.hpp"
#include "CuteDiffableList.hpp"
#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QMainWindow>
#include <boost/uuid/random_generator.hpp>
#include <event_bus.hpp>
#include <functional>
#include <memory>

namespace Ui
{
class MainWindow;
}

namespace Rescue
{

class ActionWidget;
class MainWindow : public QMainWindow
{
public:
  MainWindow(Ptr<ushiro::event_bus> bus, ushiro::state_observer<State> observer);
  ~MainWindow() final;

  void onAddAction();
  void onAddInput();

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
  ushiro::state_observer<State> mObserver;
  QBoxLayout* mAreaLayout = nullptr;
  CuteDiffableList<ActionWidget> mActionWidgetList;
  QString mCurrentFilename;
};
} // namespace Rescue