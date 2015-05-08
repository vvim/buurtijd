#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    usersModel = new QStandardItemModel(0, 2);
    dossiersModel = NULL;

    QSqlQuery query;
    query.prepare("SELECT * FROM deelnemer ORDER by naam DESC");
    if(!query.exec())
    {
        qDebug() << "er ging iets mis bij het opzoeken van deelnemers" << query.lastError();
        exit(0);
    }

    QStringList usersList;
    while(query.next())
    {
        usersModel->insertRow(0);
        usersModel->setData(usersModel->index(0,0), query.value(2).toString()); // naam
        usersModel->setData(usersModel->index(0,1), query.value(0).toInt()); // id
    }

    ui->gebruikersComboBox->setModel(usersModel);
}

Dialog::~Dialog()
{
    delete ui;
    delete usersModel;
    delete dossiersModel;
}

//use a textbrowser: https://www.youtube.com/watch?v=MvD_hFuLfzc


/*

        QSqlQuery query;
        query.prepare("SELECT naam, straat, nr, bus, postcode, plaats, land, openingsuren, contactpersoon, telefoonnummer1, extra_informatie"
                       " FROM   ophaalpunten WHERE id = :id ");
        query.bindValue(":id",marker->ophaalpunt.ophaalpunt_id);
        if((query.exec()) && (query.next()))
        {
            TransportationListDocumentWriter::Ophaalpunt doc_ophaalpunt;
            doc_ophaalpunt.counter = counter;
            doc_ophaalpunt.kaart_nr = kaart_nr;
            doc_ophaalpunt.arrivaltime = startTimeEdit->time().addSecs(seconds_needed_to_complete_transport);
            doc_ophaalpunt.naam = query.value(0).toString();

*/

void Dialog::on_gebruikersComboBox_currentIndexChanged(int index)
{
    QModelIndex index_naam = usersModel->index(index,0);
    QModelIndex index_id = usersModel->index(index,1);
    QString gebruikers_naam = usersModel->data(index_naam).toString();
    int gebruikers_id = usersModel->data(index_id).toInt();

    // clear TextBrowser:
    ui->dossierTextBrowser->clear();

    qDebug() << "index is nu:" << gebruikers_id << gebruikers_naam;

    delete dossiersModel;
    QStringList labels;
    labels << "Datum" << "Tekst";

    dossiersModel = new QStandardItemModel(0, labels.count());

    QSqlQuery query;
    query.prepare("SELECT * FROM dossier WHERE van_gebruiker = :arg1 ORDER BY dossier.datum");
    query.bindValue(":arg1", gebruikers_id);

    if(!query.exec())
    {
        qDebug() << "er ging iets mis bij het opzoeken van de dossiers voor deelnemer" << gebruikers_id << query.lastError();
        exit(0);
    }

    while(query.next())
    {
        // filling in the model:
        dossiersModel->insertRow(0);
        dossiersModel->setData(dossiersModel->index(0,0), query.value(2).toDate()); // datum
        dossiersModel->setData(dossiersModel->index(0,1), query.value(4).toString()); // inhoud

        qDebug() << " - data set:" << dossiersModel->data(dossiersModel->index(0,1)).toString() << " hetzelfde als " << query.value(4).toString();
        qDebug() << " - van datum:" << dossiersModel->data(dossiersModel->index(0,0)).toDate().toString();
    }


    for(int i = 0; i < labels.count(); i++)
    {
       dossiersModel->setHeaderData(i,Qt::Horizontal, /* QObject::tr( */ labels[i] /*)*/ );  // why does 'tr()' not work? -> QString& instead of QString...
    }

    ui->allDossiersTreeView->setModel(dossiersModel);
    // !! hide text in TreeView
    ui->allDossiersTreeView->hideColumn(1);
    // !! sort by date
    ui->allDossiersTreeView->sortByColumn(0);
}

void Dialog::on_allDossiersTreeView_clicked(const QModelIndex &index)
{
    qDebug() << "geklikt op rij nummer" << index.row() << dossiersModel->data(dossiersModel->index(index.row(),0)).toDate().toString();
    qDebug() << "dat is dan tekst:" << dossiersModel->data(dossiersModel->index(index.row(),1)).toString();
    ui->dossierTextBrowser->setText(dossiersModel->data(dossiersModel->index(index.row(),1)).toString());
}

void Dialog::on_addButton_clicked()
{
    QModelIndex index_naam = usersModel->index(ui->gebruikersComboBox->currentIndex(),0);
    QString gebruikers_naam = usersModel->data(index_naam).toString();

    AddDossierDialog addDossier(gebruikers_naam);
    if(addDossier.exec())
    {
        int row = dossiersModel->rowCount();
        dossiersModel->insertRow(row);
        dossiersModel->setData(dossiersModel->index(row,0), addDossier.getDate()); // datum
        dossiersModel->setData(dossiersModel->index(row,1), addDossier.getContentHTML()); // inhoud
        QModelIndex index = dossiersModel->index(row,0);
        ui->allDossiersTreeView->setCurrentIndex(index);
        ui->dossierTextBrowser->setText(dossiersModel->data(dossiersModel->index(index.row(),1)).toString());
        // !! sort by date
        ui->allDossiersTreeView->sortByColumn(0);
    }
}

void Dialog::on_deleteButton_clicked()
{

}
