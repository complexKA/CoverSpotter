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
