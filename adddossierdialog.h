#ifndef ADDDOSSIERDIALOG_H
#define ADDDOSSIERDIALOG_H

#include <QDialog>
#include <QDate>

namespace Ui {
class AddDossierDialog;
}

class AddDossierDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit AddDossierDialog(QString user, QWidget *parent = 0);
    ~AddDossierDialog();

    QDate getDate();
    QString getContent();
    QString getContentHTML();
    
private:
    Ui::AddDossierDialog *ui;
};

#endif // ADDDOSSIERDIALOG_H
