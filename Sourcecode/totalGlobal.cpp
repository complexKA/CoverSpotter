#include "configmanager.h"

#include <QString>
#include <QStandardPaths>
#include <QDir>


// Helper function: Determine the path to the settings file (CoverSpotter.ini)
QString __getSettingsPath()  {

    // Determines the path to the INI file, usually: ~/.config/CoverSpotter/CoverSpotter.ini
    QString sPath = QStandardPaths::writableLocation( QStandardPaths::AppConfigLocation );

    // IMPORTANT: Create a folder if it does not already exist
    QDir().mkpath( sPath );

    // Return
    return sPath + "/CoverSpotter.ini";

}

// Helper function: Returns the current minimum width (portrait or landscape, depending on the mode)
int __getCurrentMinW()  {

    if ( CMI.iMode == CS_LANDSCAPEMODE )  return CS_MINW_LANDSCAPE;
    return CS_MINW_PORTRAIT;

}

// Helper function: Returns the current minimum height (portrait or landscape, depending on the mode)
int __getCurrentMinH()  {

    if ( CMI.iMode == CS_LANDSCAPEMODE )  return CS_MINH_LANDSCAPE;
    return CS_MINH_PORTRAIT;

}
