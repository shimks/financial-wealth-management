#ifndef STOCK_H
#define STOCK_H

#include <string>
#include <vector>
#include <QtWidgets>
#include <QtNetwork/QNetworkAccessManager>
#include <QtCharts>

class Stock : public QGroupBox {
    Q_OBJECT
public:
    Stock(QWidget *parent = nullptr);

    QLineEdit *stockSearchBar;
    QLabel *searchedStockPrice;
    QTreeWidget *stockTree;
    QChart *stockChart;
    QChartView *stockChartView;
    QLineSeries *stockLine;
    QDateTimeAxis *axisX;
    QValueAxis *axisY;

    QPushButton *addStockBtn;
    QPushButton *editStockBtn;
    QPushButton *delStockBtn;

    QLabel *totalStock;

    void findStock(const std::string &stockName);

signals:
    void stockFound();
    void stockNotFound();

private:
    QStringList columnNames;
    QNetworkAccessManager *stockSearchManager;
    QNetworkAccessManager *currentlyOwnedStockSearchManager;
    QNetworkAccessManager *stockChartManager;

    void createStockTable();
    void createStockChart();
    QNetworkRequest setupNetworkRequest(const std::string &stockName);
    QString getIdOfItem(const QTreeWidgetItem*);

private slots:
    void onSearch();
    void onFindStockResponse(QNetworkReply*);
    void onAddStock();
    void onMyStockNetworkResponse(QNetworkReply*);
    void onEditStock();
    void onDelStock();
    void buildStockChart(QTreeWidgetItem*, QTreeWidgetItem*);
    void onStockChartResponse(QNetworkReply*);
    void calculateStockTotal();
};

class StockDialog : public QDialog {
    Q_OBJECT
public:
    StockDialog(Stock *parent = nullptr, bool isEdit = false);

    QLineEdit *name;
    QLineEdit *quantity;
    QLineEdit *purchasePrice;

    QStringList returnDialogValues();
};

#endif // STOCK_H
