#ifndef STOCK_H
#define STOCK_H

#include <string>
#include <vector>
#include <QtWidgets>
#include <QtNetwork/QNetworkAccessManager>

class Stock : public QGroupBox {
    Q_OBJECT
public:
    Stock(QWidget *parent = nullptr);

    QLineEdit *stockSearchBar;
    QLabel *searchedStockPrice;
    QTreeWidget *stockTree;

    QPushButton *addStockBtn;

    void findStock(const std::string &stockName);

signals:
    void stockFound();
    void stockNotFound();

private:
    QStringList columnNames;
    QNetworkAccessManager *stockSearchManager;
    QNetworkAccessManager *currentlyOwnedStockSearchManager;

    void createStockTable();
    QNetworkRequest setupNetworkRequest(const std::string &stockName);

private slots:
    void onSearch();
    void onFindStockResponse(QNetworkReply*);
    void onAddStock();
    void onAddStockNetworkResponse(QNetworkReply*);
};

class StockDialog : public QDialog {
    Q_OBJECT
public:
    StockDialog(QWidget *parent = nullptr);

    QLineEdit *name;
    QLineEdit *quantity;
    QLineEdit *purchasePrice;

    QStringList returnDialogValues();
};

#endif // STOCK_H
