#include <QtGui/QApplication>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QTextCodec>
#include <QDebug>

#include "dialog.h"

bool connectToDatabase()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("dummytest");
    db.setUserName("dummy" );
    db.setPassword("xxxxxxxx");

    if( !db.open() )
    {
        QMessageBox::critical(0, qApp->tr("Cannot open database"),
            qApp->tr("Unable to establish a database connection.\n"
                     "This is the error recieved: \n\n").append(db.lastError().text()), QMessageBox::Cancel);
        qCritical( "Failed to connect to database" );
        return false;
    }

    qDebug( "Connected to database" );

    return true;
}

int main(int argc, char *argv[])
{
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    QApplication a(argc, argv);

    //connect to DB
    if (!connectToDatabase()) exit(0);

    Dialog w;
    w.show();
    
    int returnvalue = a.exec();
    qDebug() << "remove DB";
    QSqlDatabase::removeDatabase("buurtijd_test");


    return returnvalue;
}
