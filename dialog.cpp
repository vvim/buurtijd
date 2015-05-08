#include "dialog.h"
#include "ui_dialog.h"

#define TRANSACTIE_id 0
//#define TRANSACTIE_timestamp
#define TRANSACTIE_ontvanger 1
#define TRANSACTIE_gever 2
#define TRANSACTIE_buren 3
#define TRANSACTIE_taak 4
#define TRANSACTIE_datum 5
#define TRANSACTIE_opmerking 6
#define TRANSACTIE_ingegeven_door 7
#define TRANSACTIE_categorie 8

#define DOSSIER_datum 0
#define DOSSIER_inhoud 1

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    usersModel = new QStandardItemModel(0, 2);
    dossiersModel = NULL;
    ontvangenModel = NULL;
    gegevenModel = NULL;

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
    ui->buurtijdOverzichtLabel->clear();

    qDebug() << "index is nu:" << gebruikers_id << gebruikers_naam;

    delete dossiersModel;
    delete gegevenModel;
    delete ontvangenModel;
    QStringList dossiers_labels;
    dossiers_labels << "Datum" << "Tekst";

    dossiersModel = new QStandardItemModel(0, dossiers_labels.count());
    ontvangenModel = new QStandardItemModel(0, 8);
    gegevenModel = new QStandardItemModel(0, 8);


    double ontvangenBuren = 0;
    double gegevenBuren = 0;


      /**

          interessant: aparte classe Participant
            Participant::init (met alle queries)
            Participant::getOntvangenModel()
            Participant::getOntvangenAantalBuren()
            Participant::getOntvangenAantalTransacties()
          **/

    QSqlQuery query_ontvanger;
    query_ontvanger.prepare("SELECT * FROM transactie WHERE ontvanger = :arg1 ORDER BY datum");
    query_ontvanger.bindValue(":arg1", gebruikers_id);

    if(!query_ontvanger.exec())
    {
        qDebug() << "er ging iets mis bij het opzoeken van de ontvangen Buren voor deelnemer" << gebruikers_id << query_ontvanger.lastError();
        exit(0);
    }

    while(query_ontvanger.next())
    {
        // filling in the model:
        ontvangenModel->insertRow(0);
        ontvangenModel->setData(ontvangenModel->index(0,TRANSACTIE_id), query_ontvanger.value(0).toInt()); // id
                            // 1 is timestamp en dat hebben we niet nodig
        ontvangenModel->setData(ontvangenModel->index(0,TRANSACTIE_ontvanger), query_ontvanger.value(2).toInt()); // ontvanger
        ontvangenModel->setData(ontvangenModel->index(0,TRANSACTIE_gever), query_ontvanger.value(3).toInt()); // gever
        ontvangenModel->setData(ontvangenModel->index(0,TRANSACTIE_buren), query_ontvanger.value(4).toDouble()); // aantal_buren
        ontvangenModel->setData(ontvangenModel->index(0,TRANSACTIE_taak), query_ontvanger.value(5).toString()); // taak
        ontvangenModel->setData(ontvangenModel->index(0,TRANSACTIE_datum), query_ontvanger.value(6).toDate()); // datum van de uitwisseling
        ontvangenModel->setData(ontvangenModel->index(0,TRANSACTIE_opmerking), query_ontvanger.value(7).toString()); // opmerkingen?
        ontvangenModel->setData(ontvangenModel->index(0,TRANSACTIE_categorie), query_ontvanger.value(8).toInt()); // categorie

        qDebug() << "<vvim> TODO: categorie zou een INT moeten worden";

        ontvangenBuren += query_ontvanger.value(4).toDouble();
    }

    QSqlQuery query_gever;
    query_gever.prepare("SELECT * FROM transactie WHERE gever = :arg1 ORDER BY datum");
    query_gever.bindValue(":arg1", gebruikers_id);

    if(!query_gever.exec())
    {
        qDebug() << "er ging iets mis bij het opzoeken van de gegeven Buren voor deelnemer" << gebruikers_id << query_gever.lastError();
        exit(0);
    }

    while(query_gever.next())
    {
        // filling in the model:
        ontvangenModel->insertRow(0);
        ontvangenModel->setData(gegevenModel->index(0,TRANSACTIE_id), query_gever.value(0).toInt()); // id
                            // 1 is timestamp en dat hebben we niet nodig
        ontvangenModel->setData(gegevenModel->index(0,TRANSACTIE_ontvanger), query_gever.value(2).toInt()); // ontvanger
        ontvangenModel->setData(gegevenModel->index(0,TRANSACTIE_gever), query_gever.value(3).toInt()); // gever
        ontvangenModel->setData(gegevenModel->index(0,TRANSACTIE_buren), query_gever.value(4).toDouble()); // aantal_buren
        ontvangenModel->setData(gegevenModel->index(0,TRANSACTIE_taak), query_gever.value(5).toString()); // taak
        ontvangenModel->setData(gegevenModel->index(0,TRANSACTIE_datum), query_gever.value(6).toDate()); // datum van de uitwisseling
        ontvangenModel->setData(gegevenModel->index(0,TRANSACTIE_opmerking), query_gever.value(7).toString()); // opmerkingen?
        ontvangenModel->setData(gegevenModel->index(0,TRANSACTIE_categorie), query_gever.value(8).toInt()); // categorie

        qDebug() << "<vvim> TODO: categorie zou een INT moeten worden";

        gegevenBuren += query_gever.value(4).toDouble();
    }

    QSqlQuery query_dossier;
    query_dossier.prepare("SELECT * FROM dossier WHERE van_gebruiker = :arg1 ORDER BY dossier.datum");
    query_dossier.bindValue(":arg1", gebruikers_id);

    if(!query_dossier.exec())
    {
        qDebug() << "er ging iets mis bij het opzoeken van de dossiers voor deelnemer" << gebruikers_id << query_dossier.lastError();
        exit(0);
    }

    while(query_dossier.next())
    {
        // filling in the model:
        dossiersModel->insertRow(0);
        dossiersModel->setData(dossiersModel->index(0,DOSSIER_datum), query_dossier.value(2).toDate()); // datum
        dossiersModel->setData(dossiersModel->index(0,DOSSIER_inhoud), query_dossier.value(4).toString()); // inhoud

        qDebug() << " - data set:" << dossiersModel->data(dossiersModel->index(0,DOSSIER_inhoud)).toString() << " hetzelfde als " << query_dossier.value(4).toString();
        qDebug() << " - van datum:" << dossiersModel->data(dossiersModel->index(0,DOSSIER_datum)).toDate().toString();
    }


    for(int i = 0; i < dossiers_labels.count(); i++)
    {
       dossiersModel->setHeaderData(i,Qt::Horizontal, /* QObject::tr( */ dossiers_labels[i] /*)*/ );  // why does 'tr()' not work? -> QString& instead of QString...
    }

    ui->allDossiersTreeView->setModel(dossiersModel);
    ui->allDossiersTreeView->hideColumn(DOSSIER_inhoud);
    ui->allDossiersTreeView->sortByColumn(DOSSIER_datum); // zal niet werken: proxyfilter nodig!!

    ui->buurtijdOverzichtLabel->setText(QString("%1 β ontvangen en %2 β gegeven").arg(ontvangenBuren).arg(gegevenBuren));
}

void Dialog::on_allDossiersTreeView_clicked(const QModelIndex &index)
{
    qDebug() << "geklikt op rij nummer" << index.row() << dossiersModel->data(dossiersModel->index(index.row(),DOSSIER_datum)).toDate().toString();
    qDebug() << "dat is dan tekst:" << dossiersModel->data(dossiersModel->index(index.row(),DOSSIER_inhoud)).toString();
    ui->dossierTextBrowser->setText(dossiersModel->data(dossiersModel->index(index.row(),DOSSIER_inhoud)).toString());
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
        dossiersModel->setData(dossiersModel->index(row,DOSSIER_datum), addDossier.getDate()); // datum
        dossiersModel->setData(dossiersModel->index(row,DOSSIER_inhoud), addDossier.getContentHTML()); // inhoud
        QModelIndex index = dossiersModel->index(row,DOSSIER_datum);
        ui->allDossiersTreeView->setCurrentIndex(index);
        ui->dossierTextBrowser->setText(dossiersModel->data(dossiersModel->index(index.row(),DOSSIER_inhoud)).toString());
        ui->allDossiersTreeView->sortByColumn(DOSSIER_datum); // zal niet werken: proxyfilter nodig!!
    }
}

void Dialog::on_deleteButton_clicked()
{

}
