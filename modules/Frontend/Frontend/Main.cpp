#include "Main.hpp"
#include "Domain/Action.hpp"
#include "Rescue.hpp"
#include <QApplication>
#include <QCommandLineParser>
#include <stdexcept>

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

    Rescue mainWindow;
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
