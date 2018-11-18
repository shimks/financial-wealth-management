#ifndef ADDDIALOG_H
#define ADDDIALOG_H

#include <QDialog>
#include <QtCore>
#include <QtGui>
#include <QTreeWidgetItem>
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
    QString returnAmo();

private slots:

    void on_pushButton_clicked();


    void on_pushButton_2_clicked();

private:
    Ui::AddDialog *ui;

    QString categroy;
    QString amount;
};

#endif // ADDDIALOG_H
