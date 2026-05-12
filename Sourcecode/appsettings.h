#ifndef APPSETTINGS_H
#define APPSETTINGS_H

#include <QButtonGroup>
#include <QDialog>


namespace Ui {
class AppSettings;
}

class AppSettings : public QDialog
{
    Q_OBJECT

public:
    explicit AppSettings( QWidget *parent = nullptr );
    ~AppSettings();

private slots:
    void on_pb_Save_clicked();

private:
    Ui::AppSettings *ui;

    QButtonGroup *pThemeRBgrp;

};

#endif // APPSETTINGS_H
