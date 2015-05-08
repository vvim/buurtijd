#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDate>
#include <QStringListModel>
#include <QStandardItemModel>
#include "adddossierdialog.h"

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
    
private slots:
    void on_gebruikersComboBox_currentIndexChanged(int index);

    void on_allDossiersTreeView_clicked(const QModelIndex &index);

    void on_addButton_clicked();

    void on_deleteButton_clicked();

private:
    Ui::Dialog *ui;
    QStandardItemModel *usersModel, *dossiersModel, *ontvangenModel, *gegevenModel;
};

#endif // DIALOG_H
