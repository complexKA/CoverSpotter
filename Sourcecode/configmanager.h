#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include "totalGlobal.h"

#include <QSettings>
#include <QImage>
#include <QDir>


class ConfigManager {

public:

    // Access to the single instance (singleton)
    static ConfigManager& instance() {

        static ConfigManager _instance;
        return _instance;

    }

    // --- Variables used during runtime ---
    bool       bCurrentValid = false;
    QString    sCurrentFilename;
    QString    sCurrentImageName;
    QImage     imgCurrentCover;
    int        iCurrentCoverw  = 0, iCurrentCoverh  = 0;
    int        iCurrentWindoww = 0, iCurrentWindowh = 0;
    int64_t    i64CurrentFilesize = 0;

    // --- Save values ---
    QByteArray xGeometry;
    QByteArray xFileDialogGeo;
    QString    sLastOpenDir;

    double     dScaleAtLoad     = 1.0;
    bool       bResetWposSize   = false;

    int        iMode            = CS_PORTRAITMODE;
    int        iStartCounter    = 0;

    int        iTheme           = RADIOBUTTON_1;       // Appearance section
    double     dUIscalingFactor = 1.0;



    // Help function: Trim QString
    QString __max( QString sString, int iMax )  {

            if ( sString.length() > iMax )  sString = sString.left( iMax );
            return sString;

    }


    ////////////////////////////////////////////////
    ///
    ///   Load all values from the INI file
    ///
    ////////////////////////////////////////////////

    void load()  {

        QSettings s( __getSettingsPath(), QSettings::IniFormat );

        // Forces Qt to reload the INI file from the hard drive
        s.sync();

        // Window geometry and last opened directory
        s.beginGroup( "Geometry" );

            xGeometry        = s.value( "MainWindow" ).toByteArray();
            xFileDialogGeo   = s.value( "FileDialog" ).toByteArray();

        s.endGroup();

        // Last section
        s.beginGroup( "Last" );

            sLastOpenDir     = s.value( "OpenDir", QDir::homePath() ).toString();    sLastOpenDir = __max( sLastOpenDir, 255 );
            iMode            = s.value( "Mode",    CS_PORTRAITMODE  ).toInt();

        s.endGroup();

        // Counter section
        s.beginGroup( "Counter" );

            iStartCounter    = s.value( "StartCounter", 0 ).toInt();  iStartCounter++;

        s.endGroup();

        // General section
        //s.beginGroup( "General" );

            bResetWposSize   = s.value( "ResetWposSize",   false         ).toBool();

        //s.endGroup();

        // Appearance section
        s.beginGroup( "Appearance" );

            iTheme           = s.value( "Theme",           RADIOBUTTON_1 ).toInt();
            dUIscalingFactor = s.value( "UIscalingFactor", 1.0           ).toDouble();

        s.endGroup();


        // Miscellaneous
        dScaleAtLoad = dUIscalingFactor;        // Remember the scaling factor at the start

    }



    ////////////////////////////////////////////////
    ///
    ///   Save all values to the INI file
    ///
    ////////////////////////////////////////////////

    void save()  {

        QSettings s( __getSettingsPath(), QSettings::IniFormat );
        s.clear();

        // Synchronize first (loads changes from other instances into memory)
        s.sync();

        // Window geometry and last opened directory
        s.beginGroup( "Geometry" );

            s.setValue( "MainWindow",      xGeometry        );
            s.setValue( "FileDialog",      xFileDialogGeo   );

        s.endGroup();

        // Last section
        s.beginGroup( "Last" );

            s.setValue( "OpenDir",         sLastOpenDir     );         // The last directory opened in "from File"
            s.setValue( "Mode",            iMode            );         // Portrait or landscape

        s.endGroup();

        // Counter section
        s.beginGroup( "Counter" );

            s.setValue( "StartCounter",    iStartCounter    );

        s.endGroup();

        // General section
        //s.beginGroup( "General" );

            if ( dScaleAtLoad != dUIscalingFactor )  s.setValue( "ResetWposSize", true );
            else  s.setValue( "ResetWposSize", false );

        //s.endGroup();

        // Appearance section
        s.beginGroup( "Appearance" );

            s.setValue( "Theme",           iTheme           );        // Radio buttons
            s.setValue( "UIscalingFactor", dUIscalingFactor );        // UI Scaling Factor

        s.endGroup();


        s.sync();       // Force sync to ensure that the file is written immediately

    }

private:
    ConfigManager() { load(); }     // Private constructor is loaded on the first call

    // Prevent copying
    ConfigManager( const ConfigManager& ) = delete;
    void operator=( const ConfigManager& ) = delete;
};

#endif

