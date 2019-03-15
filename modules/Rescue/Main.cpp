#include "Main.hpp"
#include "Action.hpp"
#include "LoadSave.hpp"
#include "RescueMainWindow.hpp"
#include "State.hpp"
#include "event_bus.hpp"
#include "state_observer.hpp"
#include "store.hpp"
#include "subscribe_event_list.hpp"
#include "LoadSaveService.hpp"
#include <QtCore/QCommandLineParser>
#include <QtWidgets/QApplication>
#include <iostream>
#include <memory>
#include <stdexcept>

#define RESCUE_VERSION "0.0.1"

int run(int argc, char** argv)
{
  QApplication app(argc, argv);
  QCoreApplication::setOrganizationName("ltjax");
  QCoreApplication::setApplicationName("Rescue");
  QCoreApplication::setApplicationVersion(RESCUE_VERSION);
  QCommandLineParser parser;
  parser.setApplicationDescription(QCoreApplication::applicationName());
  parser.addHelpOption();
  parser.addVersionOption();

  parser.process(app);

  using namespace Rescue;

  // Create basic application objects
  auto bus = std::make_shared<ushiro::event_bus>();
  ushiro::store<State> store;
  auto observerManager = std::make_shared<ushiro::state_observation_manager<State>>([&store]() -> State const& {return store.state;});

  // Notify about updates whenever store changes
  store.change_handler = [&](auto const& from, auto const& to) { observerManager->message_changed(from, to); };

  store.error_handler = [](std::exception const& e) { std::cerr << "Store error: " << e.what() << std::endl; };

  // Bind all events acting on the store
  auto eventListSubscription = ushiro::subscribe_event_list(*bus, store);
  LoadSaveService loadSaveService(bus, store);

  RescueMainWindow mainWindow(bus, ushiro::state_observer<State>(observerManager));

  mainWindow.show();
  return app.exec();
}

int main(int argc, char** argv)
{
  try
  {
    run(argc, argv);
  }
  catch (std::exception const& /*e*/)
  {
    return -1;
  }
  catch (...)
  {
    return -1;
  }
  return 0;
}
