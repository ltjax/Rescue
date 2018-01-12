#include "Main.hpp"
#include "Domain/Action.hpp"
#include "Model.hpp"
#include "RescueMainWindow.hpp"
#include "rtti_reducer.hpp"
#include "view_binder.hpp"
#include <QApplication>
#include <QCommandLineParser>
#include <lager/event_loop/manual.hpp>
#include <lager/store.hpp>
#include <mpark/variant.hpp>
#include <stdexcept>
#include <unordered_map>

#define RESCUE_VERSION "0.0.1"

int Run(int argc, char** argv)
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

    auto Binder = std::make_shared<view::binder<Model>>();
    auto Store = lager::make_store<Action>(Model{}, Reducer{}, std::ref(*Binder), lager::with_manual_event_loop{});

    RescueMainWindow mainWindow(
        view::component<Action, Model>([&Store](Action action) { Store.dispatch(action); }, Binder));
    mainWindow.show();
    return app.exec();
}

int main(int argc, char** argv)
{
    try
    {
        Run(argc, argv);
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
