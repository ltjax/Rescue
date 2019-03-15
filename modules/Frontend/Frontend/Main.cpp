#include "Main.hpp"
#include "Action.hpp"
#include "RescueMainWindow.hpp"
#include "State.hpp"
#include "event_bus.hpp"
#include "state_observer.hpp"
#include "store.hpp"
#include "subscribe_event_list.hpp"
#include <QtCore/QCommandLineParser>
#include <QtWidgets/QApplication>
#include <memory>
#include <stdexcept>

#define RESCUE_VERSION "0.0.1"

int run(int argc, char** argv)
{
  using namespace Rescue;

  // Create basic application objects
  auto bus = std::make_shared<ushiro::event_bus>();
  auto observerManager = std::make_shared<ushiro::state_observation_manager<State>>();
  ushiro::store<State> store;

  // Notify about updates whenever store changes
  store.change_handler = [&](auto const& from, auto const& to) { observerManager->message_changed(from, to); };

  // Bind all events acting on the store
  auto event_list_subscription = ushiro::subscribe_event_list(*bus, store);

  QApplication app(argc, argv);
  QCoreApplication::setOrganizationName("ltjax");
  QCoreApplication::setApplicationName("Rescue");
  QCoreApplication::setApplicationVersion(RESCUE_VERSION);
  QCommandLineParser parser;
  parser.setApplicationDescription(QCoreApplication::applicationName());
  parser.addHelpOption();
  parser.addVersionOption();

  parser.process(app);

  RescueMainWindow mainWindow(bus, ushiro::state_observer<State>(observerManager));

  // Send out initial notification
  observerManager->message_all(store.state);

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
