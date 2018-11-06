#include "popup_window.h"
#include "ui_popup_window.h"
#include <QMessageBox>
#include "adddialog.h"
#include <QFile>
#include <QTextStream>

popup_window::popup_window(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::popup_window)
{
 //   connect(this, SIGNAL(textEdited(QString)),
 //           AddDialog, SLOT(setText(QString)));
 //   connect(AddDialog, SIGNAL(textEdited(QString)),
 //           this, SLOT(setText(QString)));
    ui->setupUi(this);
}

popup_window::~popup_window()
{
    delete ui;
}

void popup_window::on_Show_clicked()
{
    //QString cate = adddialog.AddDialog::returnVal();
    QMessageBox::information(this,tr("Info"),tr(""));

}

void popup_window::on_Add_clicked()
{
    AddDialog adddialog;
    adddialog.setModal(true);
    adddialog.exec();
    QString cate = adddialog.AddDialog::returnVal();
    if(cate!=0){
        ui->textEdit->append(cate);
    }
    //QMessageBox::information(this,tr("Info"),tr(cate));

}
