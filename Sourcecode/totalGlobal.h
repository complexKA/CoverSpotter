#ifndef TOTALGLOBAL_H
#define TOTALGLOBAL_H



#include <QString>


static constexpr int OFF              = 0;

static constexpr int RADIOBUTTON_0    = 0;
static constexpr int RADIOBUTTON_1    = 1;
static constexpr int RADIOBUTTON_2    = 2;
static constexpr int RADIOBUTTON_3    = 3;

static constexpr int COMBOBOX_0       = 0;
static constexpr int COMBOBOX_1       = 1;
static constexpr int COMBOBOX_2       = 2;
static constexpr int COMBOBOX_3       = 3;
static constexpr int COMBOBOX_4       = 4;
static constexpr int COMBOBOX_5       = 5;

static constexpr int CS_PORTRAITMODE  = 1;
static constexpr int CS_LANDSCAPEMODE = 2;

static constexpr int CS_MINW          = 300;
static constexpr int CS_MINH          = 400;

static const QString LINE_EDIT_CSS    = "QLineEdit { padding: 4px 6px 5px 6px; font-size:10pt; }";
static const QString TEXTBROWSER_CSS  = "QTextBrowser { font: 9pt 'Sans Serif'; background-color: transparent; border: none; margin-left: -3px; }";



#define CMI ConfigManager::instance()


extern QString __getSettingsPath();

#endif
