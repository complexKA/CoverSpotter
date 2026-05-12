#include "extras.h"
#include "mainwindow.h"
#include "ui_extras.h"
#include "csfileinfo.h"
#include "about.h"
#include "configmanager.h"


Extras::Extras( MainWindow *__pMainWindow, QWidget *parent )
    : QDialog( parent )
    , ui( new Ui::Extras )
{
    pMainWindow = __pMainWindow;
    ui->setupUi( this );

    // Sets the fixed flags: window type, title, system menu, and only the close button
    // Locks the size to the current values (also disables dragging by the corners)
    this->setWindowFlags( Qt::Window | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint );
    this->setFixedSize( this->width(), this->height() );
}

// Destruktor
Extras::~Extras()
{
    delete ui;
}

// Info
void Extras::on_pb_Info_clicked()
{
    close();
    CSfileinfo winCSfileinfo;
    winCSfileinfo.setModal( true );
    winCSfileinfo.exec();
}

// About
void Extras::on_pb_About_clicked()
{
    close();
    About winAbout;
    winAbout.setModal( true );
    winAbout.exec();
}

// Portrait
void Extras::on_pb_Portrait_clicked()
{
    close();
    pMainWindow->LoadPortraitCoverFromMKV( CMI.sCurrentFilename );
}

// Landscape
void Extras::on_pb_Landscape_clicked()
{
    close();
    pMainWindow->LoadLandscapeCoverFromMKV( CMI.sCurrentFilename );
}

// Quit
void Extras::on_pb_Quit_clicked()
{
    close();
}





