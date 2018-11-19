#include <string>
#include <vector>
#include <stdlib.h>
#include <QtWidgets>
#include <QtNetwork>
#include <QtSql>
#include <QDebug>
#include "stock.h"

Stock::Stock(QWidget *parent) : QGroupBox(tr("Your Stocks"), parent) {
    auto *mainLayout = new QVBoxLayout;
    auto *stockSearchLayout = new QHBoxLayout;
    auto *stockTableLayout = new QHBoxLayout;
    auto *stockButtonGroup = new QVBoxLayout;

    stockSearchManager = new QNetworkAccessManager;
    currentlyOwnedStockSearchManager = new QNetworkAccessManager;

    columnNames = QStringList({"Name", "Quantity", "Purchase Price", "Price"});
    stockSearchBar = new QLineEdit;
    searchedStockPrice = new QLabel;
    addStockBtn = new QPushButton("Add", this);
    createStockTable();

    mainLayout->addLayout(stockSearchLayout);
    mainLayout->addLayout(stockTableLayout);
    stockSearchLayout->addWidget(stockSearchBar);
    stockSearchLayout->addWidget(searchedStockPrice, QSizePolicy::Maximum);
    stockTableLayout->addWidget(stockTree, QSizePolicy::Maximum);
    stockTableLayout->addLayout(stockButtonGroup);
    stockButtonGroup->addWidget(addStockBtn);
    setLayout(mainLayout);


    stockSearchBar->setPlaceholderText(tr("Search stock price..."));
    searchedStockPrice->setText(tr("Stock price"));

    connect(stockSearchBar, &QLineEdit::returnPressed,
            this, &Stock::onSearch);
    connect(stockSearchManager, &QNetworkAccessManager::finished,
            this, &Stock::onFindStockResponse);
    connect(currentlyOwnedStockSearchManager, &QNetworkAccessManager::finished,
            this, &Stock::onAddStockNetworkResponse);
    connect(addStockBtn, &QPushButton::clicked,
            this, &Stock::onAddStock);
}

// finds the price for symbol put in the search bar
void Stock::findStock(const std::string &stockName) {
    QNetworkRequest req = setupNetworkRequest(stockName);

    stockSearchManager->get(req);
}

QNetworkRequest Stock::setupNetworkRequest(const std::string &stockName) {
    std::string priceEndpoint("https://api.iextrading.com/1.0/stock/");
    std::string url;
    url = priceEndpoint + stockName + "/price";
    QSslConfiguration SslConfig = QSslConfiguration::defaultConfiguration();
    SslConfig.setProtocol(QSsl::TlsV1SslV3);

    QNetworkRequest request;
    request.setSslConfiguration(SslConfig);
    request.setUrl(QUrl(url.c_str()));

    return request;
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

void Stock::onAddStockNetworkResponse(QNetworkReply *reply) {
    QString response = tr(reply->readAll());
    // NOTE: no error checking is done here atm
    stockTree->currentItem()->setText(columnNames.size() - 1, response);
}

void Stock::createStockTable() {
    stockTree = new QTreeWidget;
    stockTree->setColumnCount(columnNames.size());
    stockTree->setHeaderLabels(columnNames);
    // NOTE: this query is not very scalable atm
    QSqlQuery query("select name, quantity, purchasePrice from stock");
    qDebug() << query.lastError() << endl;
    while (query.next()) {
        // NOTE: again, not scalable
        auto newEntry = new QTreeWidgetItem(
                    QStringList({
                                    query.value(0).toString(),
                                    query.value(1).toString(),
                                    query.value(2).toString()})
                    );
        stockTree->addTopLevelItem(newEntry);
        stockTree->setCurrentItem(newEntry);

        qDebug() << "Making network request for stock price..." << endl;

        // remove all of the lines below in case internet access gets spotty
        // during presentation
        QNetworkRequest req = setupNetworkRequest(query.value(0).toString().toStdString());
        QNetworkReply* res = currentlyOwnedStockSearchManager->get(req);
        // turns network requests into synchronous calls
        // NOTE: this is very inefficient and should be refactored later on to remain asynchronous
        QEventLoop loop;
        connect(res, &QNetworkReply::finished, &loop, &QEventLoop::quit);

        qDebug() << "before loop" << endl;
        loop.exec();
        qDebug() << "after loop" << endl;
        onAddStockNetworkResponse(res);
    }
}

void Stock::onAddStock() {
    StockDialog dialog;
    dialog.exec();
    if (dialog.result() == QDialog::Accepted) {
        QStringList listOfValues(dialog.returnDialogValues());

        auto newEntry = new QTreeWidgetItem(listOfValues);
        stockTree->addTopLevelItem(newEntry);
        stockTree->setCurrentItem(newEntry);

        QSqlQuery query;
        // NOTE: this query is not very scalable atm
        QString insertQuery = tr("insert into stock(name, quantity, purchasePrice) values('")
                + listOfValues[0] + "', "
                + listOfValues[1] + ", "
                + listOfValues[2] + ")";
        query.exec("create table stock (id integer primary key, name varchar(20), quantity int, purchasePrice double)");
        query.exec(insertQuery);

        QNetworkRequest req = setupNetworkRequest(listOfValues[0].toStdString());
        currentlyOwnedStockSearchManager->get(req);
    }
}

StockDialog::StockDialog(QWidget *parent) : QDialog(parent) {
    auto layout = new QFormLayout;
    name = new QLineEdit;
    quantity = new QLineEdit;
    purchasePrice = new QLineEdit;
    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    layout->addRow(tr("Name"), name);
    layout->addRow(tr("Quantity"), quantity);
    layout->addRow(tr("Purchase Price"), purchasePrice);
    layout->addRow(buttonBox);

    connect(buttonBox, &QDialogButtonBox::accepted,
            this, &StockDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected,
            this, &StockDialog::reject);

    setLayout(layout);
}

QStringList StockDialog::returnDialogValues() {
    return QStringList({name->text(), purchasePrice->text(), quantity->text()});
}
