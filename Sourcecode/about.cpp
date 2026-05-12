#include "about.h"
#include "ui_about.h"
#include "mitlicense.h"
#include "totalGlobal.h"

#include <QMessageBox>
#include <QTextFrame>



About::About( QWidget *parent )
    : QDialog( parent )
    , ui( new Ui::About )  {

    ui->setupUi( this );


    // Insert logo from the resource
    QPixmap picLogo( ":/Images/CoverSpotter-Logo.png" );
    ui->label_Logo->setPixmap( picLogo.scaled(45,45,Qt::KeepAspectRatio, Qt::SmoothTransformation) );
    ui->label_Logo->setStyleSheet( "margin: 6px 0 0 1px;" );

    // Leave a little space to the right of the scrollbar (tip)
    QTextFrameFormat xFormat = ui->textbrowser_about1->document()->rootFrame()->frameFormat();
    xFormat.setRightMargin( 15 );
    ui->textbrowser_about1->document()->rootFrame()->setFrameFormat( xFormat );

    // Format the "About" text in all tabs
    ui->textbrowser_about1->setStyleSheet( TEXTBROWSER_CSS );
    ui->textbrowser_about2->setStyleSheet( TEXTBROWSER_CSS );
    ui->textbrowser_about3->setStyleSheet( TEXTBROWSER_CSS );

    // Make sure the first tab opens first
    ui->tabWidget->setCurrentIndex( 0 );

    // Links should open in a new browser window
    ui->textbrowser_about1->setOpenExternalLinks( true );
    ui->textbrowser_about2->setOpenExternalLinks( true );
    ui->textbrowser_about3->setOpenExternalLinks( true );

    // Sets the fixed flags: window type, title, system menu, and only the close button
    // Locks the size to the current values (also disables dragging by the corners)
    this->setWindowFlags( Qt::Window | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint );
    this->setFixedSize( this->width(), this->height() );

}

// Destruktor
About::~About()
{
    delete ui;
}

// About Qt
void About::on_pb_aboutQt_clicked()
{
    QMessageBox::aboutQt( this, "Über Qt" );
}

// MIT License
void About::on_ob_MITlicense_clicked()
{
    MITlicense winMITlicence( this );
    winMITlicence.setModal( true );
    winMITlicence.exec();
}

