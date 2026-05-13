#include "csfileinfo.h"
#include "ui_csfileinfo.h"
#include "configmanager.h"

#include <QFileInfo>
#include <QFontMetrics>
#include <QMessageBox>


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

    // Freeze the height completely (min and max are identical)
    this->setMinimumHeight( this->height() );
    this->setMaximumHeight( this->height() );

    // Keep the width of the dialog variable between 350 pixels and 1200 pixels
    this->setMinimumWidth( 350 );
    this->setMaximumWidth( 1200 );

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


    /////////////////////////////
    /// Adjust the dialog width to match the text width (up to 700 pixels)

    // QFontMetrics for precise pixel calculations in Qt 6
    QFontMetrics metrics(ui->le_FileName->font());

    // horizontalAdvance measures the actual width of the text in pixels
    int iTextWidth = metrics.horizontalAdvance( CMI.sCurrentFilename );

    // Limit the value to between 350 and 700
    int iFinalWidth = qBound( 350, iTextWidth, 700 );

    // Apply new dialog width
    this->resize( iFinalWidth, this->sizeHint().height() );

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

