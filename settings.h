#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <QAbstractButton>

namespace Ui {
class Settings;
}

class Settings : public QDialog
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = 0);
    ~Settings();

private slots:
    void on_engines_add_button_clicked();

    void on_engines_remove_button_clicked();

    void on_engines_up_button_clicked();

    void on_engines_down_button_clicked();

    void on_engines_default_button_clicked();

    void on_engines_table_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);

    void accept();

    void refresh_table();

    void swap_rows(int sourceRow, int destinationRow);

private:
    Ui::Settings *ui;
};

#endif // SETTINGS_H
