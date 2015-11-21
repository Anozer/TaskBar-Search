#include "taskbarmenu.h"
#include "ui_taskbarmenu.h"
#include "settings.h"
#include "searchengine.h"
#include <QSettings>
#include <QDesktopServices>
#include <QUrl>
#include <QTimer>


// ------------------------------
// TaskBarMenu window constructor
TaskBarMenu::TaskBarMenu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TaskBarMenu)
{
    // setup the ui ...
    ui->setupUi(this);

    // Populate the search engine list from settings
    this->refresh_engineList();

    QTimer::singleShot(0, ui->search_bar, SLOT(setFocus()));
}


// -----------------------------
// TaskBarMenu window destructor
TaskBarMenu::~TaskBarMenu()
{
    delete ui;
}


// --------------------
// Exit button clicked
void TaskBarMenu::on_exit_button_clicked()
{
    // Exit on user request
    delete ui;
    QApplication::exit(0);
}


// -----------------------
// Settings button clicked
void TaskBarMenu::on_settings_button_clicked()
{
    // open settings window and wait untill it is closed
    Settings* settings_window = new Settings(this);
    settings_window->exec();

    // when it is closed, refresh the search engine list
    this->refresh_engineList();
}


// --------------------------------------
// Search is requested by pressing return
void TaskBarMenu::on_search_bar_returnPressed()
{
    // return pressed is the sames as search button clicked
    this->on_search_button_clicked();
}


// ------------------------------------------
// Search is requested by clicking the button
void TaskBarMenu::on_search_button_clicked()
{
    // get the selected search engine
    SearchEngine engine = ui->engines_list->currentData().value<SearchEngine>();
    // Create the URL combining the engine preformated URL with the user text
    QUrl searchURL = QUrl(engine.url.replace("%s", ui->search_bar->text().trimmed()));
    // Open the URL
    QDesktopServices::openUrl(searchURL);
}


// -------------------------------------
// Refresh the engine list from settings
void TaskBarMenu::refresh_engineList()
{
    // Empty the list
    ui->engines_list->clear();

    // get all search engines from settings
#ifdef Q_OS_WIN
    QSettings settings(QCoreApplication::applicationDirPath()+"/settings.ini", QSettings::IniFormat);
#else
    QSettins settings;
#endif
    int engineNb = settings.beginReadArray("searchEngines");
    for (int i = 0; i < engineNb; ++i) {
        // reconstruct each engine
        settings.setArrayIndex(i);
        SearchEngine engine;
        engine.name = settings.value("name").toString();
        engine.url = settings.value("url").toString();

        // store each engine in the combobox as user data
        ui->engines_list->addItem(engine.name, QVariant::fromValue<SearchEngine>(engine));
    }
    settings.endArray();

    // disable the interface if there is no engines
    bool enableInterface = (engineNb > 0);
    ui->search_button->setEnabled(enableInterface);
    ui->search_bar->setEnabled(enableInterface);
    ui->engines_list->setEnabled(enableInterface);
}
