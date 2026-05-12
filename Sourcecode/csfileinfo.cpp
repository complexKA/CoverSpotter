#include "csfileinfo.h"
#include "ui_csfileinfo.h"
#include "configmanager.h"

#include <QFileInfo>


CSfileinfo::CSfileinfo( QWidget *parent )
    : QDialog( parent )
    , ui(new Ui::CSfileinfo)
{
    ui->setupUi( this );


    // Insert logo from the resource
    QPixmap picLogo( ":/Images/CoverSpotter-Logo.png" );
    ui->label_Logo->setPixmap( picLogo.scaled(45,45,Qt::KeepAspectRatio, Qt::SmoothTransformation) );
    ui->label_Logo->setStyleSheet( "margin: 6px 0 0 1px;" );

    // Sets the fixed flags: window type, title, system menu, and only the close button
    // Locks the size to the current values (also disables dragging by the corners)
    this->setWindowFlags( Qt::Window | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint );
    this->setFixedSize( this->width(), this->height() );

    // If no cover image is loaded
    if ( CMI.bCurrentValid == false )  {  ui->le_FileName->setText(     "No file loaded" );
                                          ui->label_ImageName->setText( "No file loaded" );
                                          ui->label_Width->setText(     "No file loaded" );
                                          ui->label_Height->setText(    "No file loaded" );
                                          ui->label_Size->setText(      "No file loaded" );
                                       }
    else  {  // Insert data
             QFileInfo xFileInfo( CMI.sCurrentFilename );
             ui->le_FileName->setText(     xFileInfo.fileName() );

             ui->label_ImageName->setText( CMI.sCurrentImageName );
             ui->label_Width->setText(     QString::number(CMI.iCurrentCoverw) );
             ui->label_Height->setText(    QString::number(CMI.iCurrentCoverh) );
             ui->label_Size->setText(      QString("%1 KB").arg(CMI.i64CurrentFilesize / 1024) );
          }

    // Further adjustments
    ui->le_FileName->setStyleSheet( LINE_EDIT_CSS );

}

// Destruktor
CSfileinfo::~CSfileinfo()
{
    delete ui;
}


// -------------------------------------- Response to the push buttons ----------------------------------------------


// Click "OK"
void CSfileinfo::on_pb_Ok_clicked()
{
    close();
}

