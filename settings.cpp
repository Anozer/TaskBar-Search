#include "settings.h"
#include "ui_settings.h"
#include "searchengine.h"
#include <QSettings>
#include <QFile>
#include <QMessageBox>
#include <QStandardPaths>
#include <QFileInfo>

#include <QDebug>


// ---------------------------
// Settings window constructor
Settings::Settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings)
{  
    ui->setupUi(this);

    this->refresh_table();

#ifdef Q_OS_WIN
    // On Windows, get the startup checkbox state
    QSettings settings(QCoreApplication::applicationDirPath()+"/settings.ini", QSettings::IniFormat);
    ui->startup_checkbox->setChecked(settings.value("autoStartup").value<bool>());
#else
    // Hide special Windows function to non-windows OS
    ui->engines_default_button->setVisible(false);
    ui->startup_group->setVisible(false);
#endif
}


// --------------------------
// Settings window destructor
Settings::~Settings()
{
    delete ui;
}


// ------------------
// Add button clicked
void Settings::on_engines_add_button_clicked()
{
    // add a row with empty items when the button is clicked
    ui->engines_table->setRowCount(ui->engines_table->rowCount()+1);
    ui->engines_table->setItem(ui->engines_table->rowCount()-1,0,new QTableWidgetItem(""));
    ui->engines_table->setItem(ui->engines_table->rowCount()-1,1,new QTableWidgetItem(""));

    // if it's the first row, enable the remove button
    if(1 == ui->engines_table->rowCount())
    {
        ui->engines_remove_button->setEnabled(true);
    }

    // enable the "down" button because there is a new row below
    ui->engines_down_button->setEnabled(true);
}


// ---------------------
// Remove button clicked
void Settings::on_engines_remove_button_clicked()
{
    // remove the selected row when the button is clicked
    ui->engines_table->removeRow(ui->engines_table->currentRow());

    // if it's the last row, disable the remove button
    if(0 == ui->engines_table->rowCount())
    {
        ui->engines_remove_button->setEnabled(false);
    }

    // if the first row is removed, disable the up button
    if(0 == ui->engines_table->currentRow()) ui->engines_up_button->setEnabled(false);

    // if the last row is removed, disable the down button
    if(ui->engines_table->rowCount()-1 == ui->engines_table->currentRow())  ui->engines_down_button->setEnabled(false);
}


// -----------------
// Up button clicked
void Settings::on_engines_up_button_clicked()
{
    // swap the current row with the previous one
    int sourceRow = ui->engines_table->currentRow();
    int destinationRow = sourceRow-1;
    this->swap_rows(sourceRow, destinationRow);

    // Set focus to the new row
    ui->engines_table->setCurrentCell(destinationRow,ui->engines_table->currentColumn());
}


// -------------------
// Down button clicked

void Settings::on_engines_down_button_clicked()
{
    // swap the current row with the next one
    int sourceRow = ui->engines_table->currentRow();
    int destinationRow = sourceRow+1;
    this->swap_rows(sourceRow, destinationRow);

    // Set focus to the new row
    ui->engines_table->setCurrentCell(destinationRow,ui->engines_table->currentColumn());
}


// ------------------------
// Current cell has changed
void Settings::on_engines_table_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    // If the current row is the first, disable the "up" button, else, enable
    ui->engines_up_button->setEnabled((0 != currentRow));

    // If the current row is the last, disable the "down" button, else, enable
    if(ui->engines_table->rowCount() != (currentRow+1)) ui->engines_down_button->setEnabled(false);
}


// ----------------------------
// "Set default" button clicked
void Settings::on_engines_default_button_clicked()
{
#ifdef Q_OS_WIN
    // Special feature on Windows : reload default settings
    QFile::remove(QCoreApplication::applicationDirPath()+"/settings.ini");
    QFile::copy(QCoreApplication::applicationDirPath()+"/default.ini", QCoreApplication::applicationDirPath()+"/settings.ini");
    QSettings settings(QCoreApplication::applicationDirPath()+"/settings.ini", QSettings::IniFormat);
    settings.sync();
    // Refill the table with refreshed settins
    this->refresh_table();
#endif
}


// ---------------------------------------------------------------------
// accept slot rewrited (essentilly triggered when OK button is clicked)
void Settings::accept()
{
    // Verrifying datas
    for(int i=0; i<ui->engines_table->rowCount(); ++i)
    {
        // Check if names are empty
        if(ui->engines_table->item(i,0)->text().isEmpty())
        {
            QMessageBox message(QMessageBox::Critical,"Name is empty","The name of engine "+QString::number(i+1)+" is empty");
            message.exec();
            return;
        }
        // Check if URL are empty
        else if(ui->engines_table->item(i,1)->text().isEmpty())
        {
            QMessageBox message(QMessageBox::Critical,"URL is empty","The url of engine "+QString::number(i+1)+" is empty");
            message.exec();
            return;
        }
        // Check if URL contain %s
        else if(!ui->engines_table->item(i,1)->text().contains("%s"))
        {
            QMessageBox message(QMessageBox::Critical,"No request in URL","The url of engine "+QString::number(i+1)+" doesn not contain the %s keyword");
            message.exec();
            return;
        }
    }


    // Remove the previous settings wherever they are
#ifdef Q_OS_WIN
    QSettings settings(QCoreApplication::applicationDirPath()+"/settings.ini", QSettings::IniFormat);
#else
    QSettins settings;
#endif
    settings.beginGroup("searchEngines");
    settings.remove("");
    settings.endGroup();

    // Recreate the settings with the new engines from the table
    settings.beginWriteArray("searchEngines");
    for(int i=0; i<ui->engines_table->rowCount(); ++i)
    {
        settings.setArrayIndex(i);
        settings.setValue("name", ui->engines_table->item(i,0)->text());
        settings.setValue("url" , ui->engines_table->item(i,1)->text());
    }
    settings.endArray();

#ifdef Q_OS_WIN

    // If auto startup is checked
    settings.setValue("autoStartup",ui->startup_checkbox->isChecked());
    if(ui->startup_checkbox->isChecked())
    {
        // Place a shortcut of the app in the Startup folder
        QFile::link(QCoreApplication::applicationFilePath(),
                    QStandardPaths::writableLocation(QStandardPaths::ApplicationsLocation)
                    +"/Startup/"
                    +QFileInfo( QCoreApplication::applicationFilePath() ).fileName()+".lnk");
    }
    // If auto startup is not checked
    else
    {
        // Try to delete the Startup shotcut
        QFile::remove(QStandardPaths::writableLocation(QStandardPaths::ApplicationsLocation)
                      + "/Startup/"
                      + QFileInfo( QCoreApplication::applicationFilePath() ).fileName()+".lnk");
    }

#endif


    this->close();
}


// -------------------------------------
// Refresh the table datas from settings
void Settings::refresh_table()
{
    // get all search engines from settings
#ifdef Q_OS_WIN
    QSettings settings(QCoreApplication::applicationDirPath()+"/settings.ini", QSettings::IniFormat);
#else
    QSettins settings;
#endif
    int engineNb = settings.beginReadArray("searchEngines");
    ui->engines_table->setRowCount(engineNb);
    for (int i = 0; i < engineNb; ++i) {
        // display each engine in the table
        settings.setArrayIndex(i);
        QTableWidgetItem* engine_name = new QTableWidgetItem(settings.value("name").toString());
        QTableWidgetItem* engine_url  = new QTableWidgetItem(settings.value("url" ).toString());
        ui->engines_table->setItem(i,0,engine_name);
        ui->engines_table->setItem(i,1,engine_url);
    }
    settings.endArray();

    // Auto resize the table
    ui->engines_table->horizontalHeader()->setStretchLastSection(true);

    // if the table is empty
    if(0 == ui->engines_table->rowCount())
    {
        // disable the remve button
        ui->engines_remove_button->setEnabled(false);
    }
    else
    {
        // select the first row otherwise
        ui->engines_table->selectRow(0);
    }
}


// ----------------------------------------------
// Swap the two given rows by swapping each items
void Settings::swap_rows(int sourceRow, int destinationRow)
{
    QTableWidgetItem *sourceItem, *destinationItem;

    // for each item in rows (here 2 items (or 2 columns))
    for(int item_col=0; item_col<2; ++item_col)
    {
        // get pointers on source and destination items
        sourceItem = ui->engines_table->takeItem(sourceRow, item_col);
        destinationItem = ui->engines_table->takeItem(destinationRow, item_col);
        // set source item pointer into destination row
        ui->engines_table->setItem(destinationRow, item_col, sourceItem);
        // set destination item pointer into source row
        ui->engines_table->setItem(sourceRow, item_col, destinationItem);
    }
}
