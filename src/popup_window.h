#ifndef POPUP_WINDOW_H
#define POPUP_WINDOW_H

#include <QMainWindow>

namespace Ui {
class popup_window;
}

class popup_window : public QMainWindow
{
    Q_OBJECT

public:
    explicit popup_window(QWidget *parent = nullptr);
    ~popup_window();

private slots:
    void on_Show_clicked();

    void on_Add_clicked();

private:
    Ui::popup_window *ui;
};

#endif // POPUP_WINDOW_H
