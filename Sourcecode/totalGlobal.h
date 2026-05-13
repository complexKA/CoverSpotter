#ifndef TOTALGLOBAL_H
#define TOTALGLOBAL_H



#include <QString>


static constexpr int OFF               = 0;

static constexpr int RADIOBUTTON_0     = 0;
static constexpr int RADIOBUTTON_1     = 1;
static constexpr int RADIOBUTTON_2     = 2;
static constexpr int RADIOBUTTON_3     = 3;

static constexpr int COMBOBOX_0        = 0;
static constexpr int COMBOBOX_1        = 1;
static constexpr int COMBOBOX_2        = 2;
static constexpr int COMBOBOX_3        = 3;
static constexpr int COMBOBOX_4        = 4;
static constexpr int COMBOBOX_5        = 5;
static constexpr int COMBOBOX_6        = 6;
static constexpr int COMBOBOX_7        = 7;
static constexpr int COMBOBOX_8        = 8;

static constexpr int CS_PORTRAITMODE   = 1;
static constexpr int CS_LANDSCAPEMODE  = 2;

static constexpr int CS_MINW_PORTRAIT  = 300;
static constexpr int CS_MINH_PORTRAIT  = 400;

static constexpr int CS_MINW_LANDSCAPE = 534;
static constexpr int CS_MINH_LANDSCAPE = 300;

static const QString LINE_EDIT_CSS     = "QLineEdit { font-family: 'Segoe UI', Helvetica, Arial; padding: 3px 6px 3px 6px; font-size: 12pt; }";
static const QString TEXTBROWSER_CSS   = "QTextBrowser { font: 9pt 'Sans Serif'; background-color: transparent; border: none; margin-left: -3px; }";



#define CMI ConfigManager::instance()


extern QString __getSettingsPath();

extern int     __getCurrentMinW();
extern int     __getCurrentMinH();

#endif
