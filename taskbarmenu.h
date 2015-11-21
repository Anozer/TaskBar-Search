#ifndef TASKBARMENU_H
#define TASKBARMENU_H

#include <QWidget>

namespace Ui {
class TaskBarMenu;
}

class TaskBarMenu : public QWidget
{
    Q_OBJECT

public:
    explicit TaskBarMenu(QWidget *parent = 0);
    ~TaskBarMenu();

private slots:
    void on_exit_button_clicked();

    void on_settings_button_clicked();

    void on_search_bar_returnPressed();

    void on_search_button_clicked();

    void refresh_engineList();

private:
    Ui::TaskBarMenu *ui;
};

#endif // TASKBARMENU_H
