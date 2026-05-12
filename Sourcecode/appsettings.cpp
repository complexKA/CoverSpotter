#include "appsettings.h"
#include "ui_appsettings.h"
#include "configmanager.h"
#include <unistd.h>



AppSettings::AppSettings( QWidget *parent )
    : QDialog( parent )
    , ui( new Ui::AppSettings )  {

    ui->setupUi( this );



    ////////////////////////////////////////////
    /// Preparing for the dialogue

    // Define a group of radio buttons for Theme
    pThemeRBgrp = new QButtonGroup( this );
    pThemeRBgrp->addButton( ui->rb_WhiteTheme,        RADIOBUTTON_0 );
    pThemeRBgrp->addButton( ui->rb_DarkTheme,         RADIOBUTTON_1 );


    ////////////////////////////////////////////
    /// Restore controls from the backup

    // Appearance section
    int iSavedIndex_Theme = CMI.iTheme;
    if ( auto *pBtn = pThemeRBgrp->button(iSavedIndex_Theme) )  pBtn->setChecked( true );

    ui->dsb_UIscalingFactor->setValue( CMI.dUIscalingFactor );


}

// Destruktor
AppSettings::~AppSettings()
{
    delete ui;
}



// -------------------------------------- Response to the push buttons ----------------------------------------------



// Save settings
void AppSettings::on_pb_Save_clicked()
{
    // Appearance section
    CMI.iTheme           = pThemeRBgrp->checkedId();
    CMI.dUIscalingFactor = ui->dsb_UIscalingFactor->value();

    // Close the window
    close();
}

