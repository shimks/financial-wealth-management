#ifndef POPUP_WINDOW_H
#define POPUP_WINDOW_H

#include <QMainWindow>
#include <QTreeWidget>
#include <QList>
#include <QStringList>


namespace Ui {
class popup_window;
}

class popup_window : public QMainWindow
{
    Q_OBJECT

public:
    explicit popup_window(QWidget *parent = nullptr);
    ~popup_window();
    void AddRoot(QString Category, QString Amount);
    void EditRoot(int Index, QString Category, QString Amount);


private slots:
    void on_Show_clicked();

    void on_Add_clicked();


    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::popup_window *ui;
    QStringList cateList;
    QList<int> amountList;
};

#endif // POPUP_WINDOW_H
