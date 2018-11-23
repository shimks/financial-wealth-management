#include <string>
#include <vector>
#include <stdlib.h>
#include <QtWidgets>
#include <QtNetwork>
#include <QtSql>
#include <QtCharts>
#include <QDebug>
#include "stock.h"

Stock::Stock(QWidget *parent) : QGroupBox(tr("Your Stocks"), parent) {
    auto *mainLayout = new QVBoxLayout;
    auto *stockSearchLayout = new QHBoxLayout;
    auto *stockTableLayout = new QHBoxLayout;
    auto *stockButtonGroup = new QVBoxLayout;

    stockSearchManager = new QNetworkAccessManager;
    currentlyOwnedStockSearchManager = new QNetworkAccessManager;
    stockChartManager = new QNetworkAccessManager;

    columnNames = QStringList({"Name", "Quantity", "Purchase Price", "Price", "Total Value"});
    stockSearchBar = new QLineEdit;
    searchedStockPrice = new QLabel;
    addStockBtn = new QPushButton("Add", this);
    editStockBtn = new QPushButton("Edit", this);
    delStockBtn = new QPushButton("Delete", this);
    totalStock = new QLabel;
    createStockTable();
    createStockChart();
    calculateStockTotal();

    mainLayout->addLayout(stockSearchLayout);
    mainLayout->addLayout(stockTableLayout);
    mainLayout->addWidget(totalStock);
    mainLayout->addWidget(stockChartView);
    stockSearchLayout->addWidget(stockSearchBar);
    stockSearchLayout->addWidget(searchedStockPrice, QSizePolicy::Maximum);
    stockTableLayout->addWidget(stockTree, QSizePolicy::Maximum);
    stockTableLayout->addLayout(stockButtonGroup);
    stockButtonGroup->addWidget(addStockBtn);
    stockButtonGroup->addWidget(editStockBtn);
    stockButtonGroup->addWidget(delStockBtn);
    setLayout(mainLayout);


    stockSearchBar->setPlaceholderText(tr("Search stock price..."));
    searchedStockPrice->setText(tr("Stock price"));

    connect(stockSearchBar, &QLineEdit::returnPressed,
            this, &Stock::onSearch);
    connect(stockSearchManager, &QNetworkAccessManager::finished,
            this, &Stock::onFindStockResponse);
    connect(currentlyOwnedStockSearchManager, &QNetworkAccessManager::finished,
            this, &Stock::onMyStockNetworkResponse);
    connect(stockChartManager, &QNetworkAccessManager::finished,
            this, &Stock::onStockChartResponse);
    connect(addStockBtn, &QPushButton::clicked,
            this, &Stock::onAddStock);
    connect(editStockBtn, &QPushButton::clicked,
            this, &Stock::onEditStock);
    connect(delStockBtn, &QPushButton::clicked,
            this, &Stock::onDelStock);
    connect(stockTree, &QTreeWidget::currentItemChanged,
            this, &Stock::buildStockChart);
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

void Stock::onMyStockNetworkResponse(QNetworkReply *reply) {
    QString response = tr(reply->readAll());
    // NOTE: no error checking is done here atm
    double quantity = stockTree->currentItem()->data(1, Qt::DisplayRole).toDouble();
    stockTree->currentItem()->setText(3, response); // set to currentPrice
    stockTree->currentItem()->setText(4, QString::number(response.toDouble() * quantity));
    // not very robust; the total can only be updated after a network call
    calculateStockTotal();
}

void Stock::onStockChartResponse(QNetworkReply *reply) {
    QByteArray response = reply->readAll();
    qDebug() << response << endl;
    if (tr(response) != tr("Unknown symbol")) {
        QJsonDocument doc = QJsonDocument::fromJson(response);
        QJsonArray array = doc.array();
        double max(10);
        double min(array.first().toObject()["close"].toDouble());
        stockLine->clear();
        for (QJsonValueRef obj : array) {
            QJsonObject json = obj.toObject();
            QString date = json["date"].toString();
            double price = json["close"].toDouble();

            QDateTime dateTime;
            QStringList dateValues = date.split(tr("-"));
            dateTime.setDate(QDate(dateValues[0].toInt(), dateValues[1].toInt(), dateValues[2].toInt()));

            max = (max > price) ? max : price;
            min = (min < price) ? min : price;

            qDebug() << dateTime.toMSecsSinceEpoch() << " " << price << endl;
            stockLine->append(dateTime.toMSecsSinceEpoch(), price);
        }
        QStringList firstDayValue = array.first().toObject()["date"].toString().split("-");
        QStringList lastDayValue = array.last().toObject()["date"].toString().split("-");
        axisX->setMax(QDateTime(QDate(lastDayValue[0].toInt(), lastDayValue[1].toInt(), lastDayValue[2].toInt())));
        axisX->setMin(QDateTime(QDate(firstDayValue[0].toInt(), firstDayValue[1].toInt(), firstDayValue[2].toInt())));
        axisY->setMax(max);
        axisY->setMin(min);
        stockChart->setTitle(tr("Closing values of ") + stockTree->currentItem()->data(0, Qt::DisplayRole).toString()
                             + " over the last month");
        stockChartView->show();
    }
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
        onMyStockNetworkResponse(res);
    }
}

void Stock::createStockChart() {
    stockChart = new QChart;
    stockChartView = new QChartView;
    stockLine = new QLineSeries;

    stockChart->addSeries(stockLine);
    stockChart->legend()->hide();

    axisX = new QDateTimeAxis;
    axisX->setTickCount(10);
    axisX->setFormat("MMM d");
    axisX->setTitleText("Date");
    stockChart->setAxisX(axisX, stockLine);

    axisY = new QValueAxis;
    axisY->setLabelFormat("%f");
    axisY->setTitleText("Closing price");
    stockChart->setAxisY(axisY, stockLine);

    stockChart->setAnimationOptions(QChart::AllAnimations);
    stockChartView->setChart(stockChart);
    stockChartView->setRenderHint(QPainter::Antialiasing);

    stockChartView->hide();

    if (stockTree->currentItem() != nullptr) {
        buildStockChart(stockTree->currentItem(), nullptr);
    }
}

void Stock::onAddStock() {
    StockDialog dialog(this);
    dialog.exec();
    if (dialog.result() == QDialog::Accepted) {
        QStringList listOfValues(dialog.returnDialogValues());

        auto newEntry = new QTreeWidgetItem(listOfValues);
        stockTree->addTopLevelItem(newEntry);
        stockTree->setCurrentItem(newEntry);

        QSqlQuery query;
        // NOTE: this query is not very scalable atm
        QString addCmd = tr("INSERT INTO stock(name, quantity, purchasePrice) VALUES('")
                + listOfValues[0] + "', "
                + listOfValues[1] + ", "
                + listOfValues[2] + ")";
        query.exec("CREATE TABLE stock (id integer primary key, name varchar(20), quantity int, purchasePrice double)");
        query.exec(addCmd);

        QNetworkRequest req = setupNetworkRequest(listOfValues[0].toStdString());
        currentlyOwnedStockSearchManager->get(req);
    }
}

void Stock::onEditStock() {
    if (stockTree->currentItem() == nullptr) {
        QMessageBox::warning(this, tr("No Stock"), tr("No stocks exist!"));
    } else {
        StockDialog dialog(this, true);
        dialog.exec();
        if (dialog.result() == QDialog::Accepted) {
            QTreeWidgetItem *current = stockTree->currentItem();
            QString id = getIdOfItem(current);


            QStringList listOfValues(dialog.returnDialogValues());

            for (int i = 0; i < listOfValues.length(); i++) {
                stockTree->currentItem()->setData(i, Qt::DisplayRole, listOfValues[i]);
            }

            QSqlQuery query;
            // NOTE: this query is not very scalable atm
            QString insertCmd = tr("UPDATE stock SET ")
                    + "name = '" + listOfValues[0] + "', "
                    + "quantity = " + listOfValues[1] + ", "
                    + "purchasePrice = " + listOfValues[2] +
                    + " WHERE id = " + id;
            query.exec(insertCmd);
            qDebug() << insertCmd << endl;
            qDebug() << query.lastError() << endl;

            QNetworkRequest req = setupNetworkRequest(listOfValues[0].toStdString());
            currentlyOwnedStockSearchManager->get(req);
        }
    }
}

void Stock::onDelStock() {
    if (stockTree->currentItem() == nullptr) {
        QMessageBox::warning(this, tr("No Stock"), tr("No stocks exist!"));
    } else {
        QMessageBox::StandardButton reply = QMessageBox::question(this, tr("Deleting a Stock"),
                              "Are you sure you want to delete this stock?");
        if (reply == QMessageBox::Yes) {
            QTreeWidgetItem *current = stockTree->currentItem();
            QString id = getIdOfItem(current);

            delete current;

            QSqlQuery query;
            QString deleteCmd = tr("DELETE FROM stock WHERE id = ") + id;
            query.exec(deleteCmd);
            qDebug() << query.lastError() << endl;
        }
    }
}

void Stock::buildStockChart(QTreeWidgetItem *current, QTreeWidgetItem *prev) {
    if (current == nullptr) {
        return;
    }
    QString symbol = current->data(0, Qt::DisplayRole).toString();

    QString priceEndpoint("https://api.iextrading.com/1.0/stock/");
    QString url;
    url = priceEndpoint + symbol + "/chart";
    QSslConfiguration SslConfig = QSslConfiguration::defaultConfiguration();
    SslConfig.setProtocol(QSsl::TlsV1SslV3);

    QNetworkRequest request;
    request.setSslConfiguration(SslConfig);
    request.setUrl(QUrl(url));

    stockChartManager->get(request);
}

void Stock::calculateStockTotal() {
    QTreeWidgetItemIterator iter(stockTree);
    double total(0);
    while (*iter) {
        total += (*iter)->text(4).toDouble();
        ++iter;
    }
    totalStock->setText(tr("Total value of portfolio: ") + QString::number(total));
}

QString Stock::getIdOfItem(const QTreeWidgetItem *item) {
    QSqlQuery idQuery;
    QString getRowCommand = tr("SELECT id FROM stock WHERE (")
            + tr("name='") + item->data(0, Qt::DisplayRole).toString() + tr("' AND ")
            + tr("quantity=") + item->data(1, Qt::DisplayRole).toString() + tr(" AND ")
            + tr("purchasePrice=") + item->data(2, Qt::DisplayRole).toString() + tr(")");
    idQuery.exec(getRowCommand);
    qDebug() << idQuery.lastError() << endl;
    idQuery.next();
    return idQuery.value(0).toString();
}

StockDialog::StockDialog(Stock *parent, bool isEditing) : QDialog(parent) {
    auto layout = new QFormLayout;
    name = new QLineEdit;
    quantity = new QLineEdit;
    purchasePrice = new QLineEdit;
    if (isEditing) {
        name->setText(parent->stockTree->currentItem()->data(0, Qt::DisplayRole).toString());
        // weak way to have the value be readonly; might be programatically changeable
        name->setReadOnly(true);
        quantity->setText(parent->stockTree->currentItem()->data(1, Qt::DisplayRole).toString());
        purchasePrice->setText(parent->stockTree->currentItem()->data(2, Qt::DisplayRole).toString());
    }
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
    return QStringList({name->text(), quantity->text(), purchasePrice->text()});
}
