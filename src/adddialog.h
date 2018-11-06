#ifndef ADDDIALOG_H
#define ADDDIALOG_H

#include <QDialog>

namespace Ui {
class AddDialog;
}

class AddDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddDialog(QWidget *parent = nullptr);
    ~AddDialog();
    QString returnVal();

private slots:

    void on_pushButton_clicked();


    void on_pushButton_2_clicked();

private:
    Ui::AddDialog *ui;

    QString categroy;
};

#endif // ADDDIALOG_H
