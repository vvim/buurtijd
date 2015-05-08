#include "adddossierdialog.h"
#include "ui_adddossierdialog.h"

AddDossierDialog::AddDossierDialog(QString user, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddDossierDialog)
{

    ui->setupUi(this);
    ui->dateEdit->setDisplayFormat("dd MMM yyyy");
    ui->dateEdit->setLocale(QLocale::Dutch);
    ui->dateEdit->setCalendarPopup(true);  //zie http://stackoverflow.com/questions/7031962/qdateedit-calendar-popup
    ui->dateEdit->setDate(QDate::currentDate());

    setWindowTitle(tr("Dossier voor %1 toevoegen").arg(user));
}

AddDossierDialog::~AddDossierDialog()
{
    delete ui;
}

QDate AddDossierDialog::getDate()
{
    return ui->dateEdit->date();
}

QString AddDossierDialog::getContent()
{
    return ui->inhoudTextEdit->toPlainText();
}

QString AddDossierDialog::getContentHTML()
{
    return ui->inhoudTextEdit->toHtml();
}
