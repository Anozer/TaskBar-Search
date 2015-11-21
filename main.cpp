#include "taskbarmenu.h"
#include <QApplication>
#include <QWidgetAction>
#include <QMenu>
#include <QSystemTrayIcon>
#include <QIcon>
#include <QMessageBox>
#include <QFile>
#include <QSettings>
#include <QStandardPaths>
#include <QFileInfo>

int main(int argc, char *argv[])
{
    // Init app that do not quit without opened window
    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed(false);

    // Warn if the OS do not support system tray, then exit
    if(!QSystemTrayIcon::isSystemTrayAvailable())
    {
        QMessageBox errorMessage(QMessageBox::Critical,
                                 "System not supported",
                                 "The operating system do not provide any  system tray or task bar.\n\nThe application will now exit."
                                 );
        errorMessage.exec();
        return(0);
    }


    // Configre the app, essentialy for QSettings
    QCoreApplication::setOrganizationName("Sylvain-Mariel");
    QCoreApplication::setOrganizationDomain("sylvain-mariel.fr");
    QCoreApplication::setApplicationName("TaskBarSearch");


    // Init settings
#ifdef Q_OS_WIN
    // Special feature on Windows : load default settings if there is no settings.
    if(!QFile::exists(QCoreApplication::applicationDirPath()+"/settings.ini"))
    {
        QFile::copy(QCoreApplication::applicationDirPath()+"/default.ini", QCoreApplication::applicationDirPath()+"/settings.ini");
    }
    QSettings settings(QCoreApplication::applicationDirPath()+"/settings.ini", QSettings::IniFormat);

    // Speacial feature on Windows : overwrite auto startup setting depending on the file existance
    settings.setValue("autoStartup",
                      QFile::exists(
                          QStandardPaths::writableLocation(QStandardPaths::ApplicationsLocation)
                          + "/Startup/"
                          + QFileInfo( QCoreApplication::applicationFilePath() ).fileName()+".lnk"));
#else
    // Other OS than Windows : just create settings wherever the OS want
    QSettings settings;
#endif
    settings.sync();


    // Create a sub-element of a menu that contain the custom UI
    QWidgetAction *trayWidget = new QWidgetAction(0);
    trayWidget->setDefaultWidget(new TaskBarMenu());

    // Create a menu that contain the sub-element
    QMenu *trayMenu = new QMenu();
    trayMenu->addAction(trayWidget);

    // Create a tray icon that show the menu on a right click
    QSystemTrayIcon *trayIcon = new QSystemTrayIcon();
    trayIcon->setContextMenu(trayMenu);
    trayIcon->setIcon(QIcon(":/img/icons/icon.16.png"));
    trayIcon->show();


    return a.exec();
}
