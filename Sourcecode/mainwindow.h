#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QComboBox>


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow( QWidget *parent = nullptr );
    ~MainWindow();

    void LoadPortraitCoverFromMKV( const QString sNewFile  )  {  if ( __getCoverFromMKV(sNewFile, "cover.jpg", "cover.png") == true )  __showCover();
                                                              }

    void LoadLandscapeCoverFromMKV( const QString sNewFile )  {  if ( __getCoverFromMKV(sNewFile, "cover_land.jpg", "cover_land.png") == true )  __showCover();
                                                              }

protected:
    void closeEvent( QCloseEvent * ) override;
    void dragEnterEvent( QDragEnterEvent * ) override;
    void dropEvent( QDropEvent * ) override;
    void resizeEvent( QResizeEvent * ) override;

private:
    Ui::MainWindow *ui;

    QLabel      *imageLabel;
    QPushButton *btnOpen;
    QComboBox   *cmboExtras;
    QIcon       xActiveCheckIcon;

    bool      __getCoverFromMKV( const QString, const QString, const QString );
    void      __showCover();
    void      __setTheme( const int );
    void      __changeWindowSize( const int );
    void      __resetMainWindowPosSize();
    void      __setGeomentry();
    void      __updateExtraFileInfoSelectable( const bool );
    void      __showErrorMessage( const QString );

};
#endif // MAINWINDOW_H
