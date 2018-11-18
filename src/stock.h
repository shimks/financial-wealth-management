#ifndef STOCK_H
#define STOCK_H

#include <string>
#include <QtWidgets>
#include <QtNetwork/QNetworkAccessManager>

class Stock : public QGroupBox {
    Q_OBJECT
public:
    Stock(QWidget *parent = nullptr);

    QLineEdit *stockSearchBar;
    QLabel *searchedStockPrice;

    void findStock(const std::string &stockName);

signals:
    void stockFound();
    void stockNotFound();

private:
    QNetworkAccessManager *manager;
    QNetworkRequest request;
    std::string ALPHA_VANTAGE_API_KEY;

private slots:
    void onSearch();
    void onFindStockResponse(QNetworkReply *reply);
};

#endif // STOCK_H
