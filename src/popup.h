#ifndef POPUP_H
#define POPUP_H

#include <QDialog>

namespace Ui {
class popup;
}

class popup : public QDialog
{
    Q_OBJECT

public:
    explicit popup(QWidget *parent = nullptr);
    ~popup();
    //Return function
    QString returnAsset();
    QString returnValue();

private slots:
    void on_pushButton_clicked();

private:
    Ui::popup *ui;

    /*Global Varaibles*/
    QString assetName;
    QString assetValue;
};

#endif // POPUP_H
