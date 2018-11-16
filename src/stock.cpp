#include <string>
#include <stdlib.h>
#include <QtWidgets>
#include <QtNetwork>
#include <QDebug>
#include "stock.h"

Stock::Stock(QWidget *parent) : QGroupBox(tr("Your Stocks"), parent) {
    auto *layout = new QHBoxLayout;
    stockSearchBar = new QLineEdit;
    searchedStockPrice = new QLabel;

    layout->addWidget(stockSearchBar);
    layout->addWidget(searchedStockPrice, QSizePolicy::Maximum);
    setLayout(layout);

    stockSearchBar->setPlaceholderText(tr("Search stock price..."));
    searchedStockPrice->setText(tr("Stock price"));

    manager = new QNetworkAccessManager;

    connect(stockSearchBar, &QLineEdit::returnPressed,
            this, &Stock::onSearch);
    connect(manager, &QNetworkAccessManager::finished,
            this, &Stock::onFindStockResponse);
}

void Stock::findStock(const std::string &stockName) {
    std::string priceEndpoint("https://api.iextrading.com/1.0/stock/");
    std::string url;
    url = priceEndpoint + stockName + "/price";
    QSslConfiguration SslConfig = QSslConfiguration::defaultConfiguration();
    SslConfig.setProtocol(QSsl::TlsV1SslV3);
    request.setSslConfiguration(SslConfig);
    request.setUrl(QUrl(url.c_str()));

    manager->get(request);
}

void Stock::onSearch() {
    searchedStockPrice->setText(tr("..."));
    findStock(stockSearchBar->text().toStdString());
}

void Stock::onFindStockResponse(QNetworkReply *reply) {
    QString response = tr(reply->readAll());
    qDebug() << response << endl;
    if (response == tr("Unknown symbol")) {
        QString errorMessage("Stock not found");
        searchedStockPrice->setText(errorMessage);
        emit stockNotFound();
    } else {
        double price(response.toDouble());
        searchedStockPrice->setText(QString::number(price));
        emit stockFound();
    }
}
