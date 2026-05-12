#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "totalGlobal.h"
#include "configmanager.h"
#include "extras.h"
#include "csfileinfo.h"
#include "about.h"
#include "appsettings.h"

#include <QVBoxLayout>
#include <QFileDialog>
#include <QImage>
#include <QPixmap>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QMessageBox>
#include <QShortcut>
#include <QStandardItemModel>
#include <QTimer>


MainWindow::MainWindow( QWidget *parent )
    : QMainWindow( parent )
    , ui( new Ui::MainWindow )  {

    ui->setupUi( this );


    // Set window title
    setWindowTitle( "CoverSpotter" );

    // A central widget is required for QMainWindow
    QWidget *centralWidget = new QWidget( this );
    setCentralWidget( centralWidget );

    QVBoxLayout *layout = new QVBoxLayout( centralWidget );

    imageLabel = new QLabel( "Select or drop an MKV file" );
    imageLabel->setAlignment(Qt::AlignCenter);

    imageLabel->setMinimumSize( CS_MINW, CS_MINH );
    imageLabel->setMaximumSize( QWIDGETSIZE_MAX, QWIDGETSIZE_MAX );

    layout->addWidget( imageLabel );


    ////////////////////////////////
    /// Buttons

    QHBoxLayout *footerLayout = new QHBoxLayout();

    // Create a combo box instance
    cmboExtras = new QComboBox( this );

    // Add entries to the ComboBox
    cmboExtras->addItem( "Extras"         );
    cmboExtras->addItem( "File Info"      );
    cmboExtras->addItem( "Portrait Mode"  );
    cmboExtras->addItem( "Landscape Mode" );
    cmboExtras->addItem( "Settings"       );
    cmboExtras->addItem( "About"          );

  __updateExtraFileInfoSelectable( false );

    // Add to the layout (e.g., before the Info button)
    footerLayout->addWidget( cmboExtras, 25 );   cmboExtras->setMaximumWidth( 150 );     cmboExtras->setMinimumWidth( 150 );

    // Add the buttons to the horizontal layout
    btnOpen = new QPushButton( "Open MKV" );
    footerLayout->addWidget( btnOpen, 80 );
    layout->addLayout( footerLayout );

    // Connect Button "Open MKV"
    connect( btnOpen, &QPushButton::clicked, this, [this]()  {

            // Create an instance
            QFileDialog dialog( this, "Select MKV", CMI.sLastOpenDir, "Video (*.mkv)" );
            QString     sNewFile;

            // CHANGE MODE: Single existing file instead of a directory
            dialog.setFileMode( QFileDialog::ExistingFile );
            dialog.setOption( QFileDialog::DontResolveSymlinks, true );
            dialog.setOption( QFileDialog::DontUseNativeDialog );   // Important for saving geometry

            // Restore geometry
            if ( !CMI.xFileDialogGeo.isEmpty() )  dialog.restoreGeometry( CMI.xFileDialogGeo );
            else  dialog.resize( 800, 600 );

            if ( dialog.exec() == QDialog::Accepted )  {

                sNewFile = dialog.selectedFiles().first();

                // Save the path for next time in sLastOpenDir
                QFileInfo fi( sNewFile );
                CMI.sLastOpenDir = fi.absolutePath();

                // Save the geometry for the next call
                CMI.xFileDialogGeo = dialog.saveGeometry();
            }

            // Load and display cover
            if ( !sNewFile.isEmpty() )  {  if ( CMI.iMode == CS_LANDSCAPEMODE )  LoadLandscapeCoverFromMKV( sNewFile );
                                           else  LoadPortraitCoverFromMKV( sNewFile );
                                        }

    });

    // Connect Combobox "Extras"
    connect( cmboExtras, QOverload<int>::of(&QComboBox::activated), this, [this](int iIndex )  {

        cmboExtras->setCurrentIndex( COMBOBOX_0 );

        if ( iIndex == COMBOBOX_1 )  {  // File info
                                        CSfileinfo winCSfileinfo;
                                        winCSfileinfo.setModal( true );
                                        winCSfileinfo.exec();
                                     }

        if ( iIndex == COMBOBOX_2 )  {  // Portrait
                                        CMI.iMode = CS_PORTRAITMODE;
                                        if ( CMI.bCurrentValid == true )  LoadPortraitCoverFromMKV( CMI.sCurrentFilename );

                                        cmboExtras->setItemIcon( COMBOBOX_2, xActiveCheckIcon );
                                        cmboExtras->setItemIcon( COMBOBOX_3, QIcon() );     // Remove icon
                                     }

        if ( iIndex == COMBOBOX_3 )  {  // Landscape
                                        CMI.iMode = CS_LANDSCAPEMODE;
                                        if ( CMI.bCurrentValid == true )  LoadLandscapeCoverFromMKV( CMI.sCurrentFilename );

                                        cmboExtras->setItemIcon( COMBOBOX_3, xActiveCheckIcon );
                                        cmboExtras->setItemIcon( COMBOBOX_2, QIcon() );     // Remove icon
                                     }

        if ( iIndex == COMBOBOX_4 )  {  // Settings
                                        int iThemeOld = CMI.iTheme;

                                        // Open the Settings dialog
                                        AppSettings winAppSettings;
                                        winAppSettings.setModal( true );
                                        winAppSettings.exec();

                                        // You may need to reset the theme
                                        if ( CMI.iTheme != iThemeOld )  __setTheme( CMI.iTheme );

                                     }

        if ( iIndex == COMBOBOX_5 )  {  // About
                                        About winAbout;
                                        winAbout.setModal( true );
                                        winAbout.exec();
                                     }

    });


    ////////////////////////////////
    /// Keyboard shortcuts

    for( int i = 1; i <= 3; ++i )  {

            QShortcut *shortcut = new QShortcut( QKeySequence(Qt::ALT | (Qt::Key_0 + i) ), this );
            shortcut->setProperty( "sizeIndex", i );    // Merkt sich 1, 2 oder 3

            connect( shortcut, &QShortcut::activated, this, [this, shortcut]()  {

                    int iIndex = shortcut->property( "sizeIndex" ).toInt();
                  __changeWindowSize( iIndex );

    });
}


    ////////////////////////////////
    /// Additional settings for the window

    this->resize( 1, 1 );
    this->adjustSize();

  __setTheme( CMI.iTheme );
  __setGeomentry();



    // A flag indicates that the scaling has changed since the last launch: reset the window
    if ( CMI.bResetWposSize == true )  __resetMainWindowPosSize();

}


// Destructor
MainWindow::~MainWindow()
{
    delete ui;
}



// ------------------------------------------------ Override ------------------------------------------------------------------



// Override closeEvent() to save the window position
void MainWindow::closeEvent( QCloseEvent *xEvent )  {

    // Save data in the INI file
    CMI.xGeometry = saveGeometry();
    CMI.save();

    // Open Original Destructor
    QMainWindow::closeEvent( xEvent );
}


// Override DragEnterEvent: Without this, drops will not be accepted
void MainWindow::dragEnterEvent( QDragEnterEvent *xEvent ) {

    if ( xEvent->mimeData()->hasUrls() )  xEvent->acceptProposedAction();

}

// The Actual Process of Letting Go (Overwritten)
void MainWindow::dropEvent( QDropEvent *xEvent ) {

    const QMimeData *mimeData = xEvent->mimeData();      if ( !mimeData->hasUrls() ) return;
    QList<QUrl> urlList = mimeData->urls();              if ( urlList.isEmpty() )  return;

    // On Linux, toLocalFile() removes the "file://" prefix
    QString sNewFile = urlList.at(0).toLocalFile();

    // Load and display cover
    if ( CMI.iMode == CS_LANDSCAPEMODE )  LoadLandscapeCoverFromMKV( sNewFile );
    else  LoadPortraitCoverFromMKV( sNewFile );

    // Bring the window to the foreground and focus on it
    this->activateWindow();
    this->raise();      // Ensures that it remains on top of other windows

}


// The window is being updated; any cover image that has been loaded will need to be adjusted
void MainWindow::resizeEvent( QResizeEvent* event )  {

    QMainWindow::resizeEvent( event );      // Call the base class

    if ( CMI.bCurrentValid == true )  {

        // Resize the image to match the current label size each time it is resized
        imageLabel->setPixmap( QPixmap::fromImage(CMI.imgCurrentCover).scaled(

            imageLabel->size(),
            Qt::KeepAspectRatio,
            Qt::SmoothTransformation

        ));

    }

}




// -------------------------------------------------- Private functions -------------------------------------------------------



extern "C" {
#include <libavformat/avformat.h>
}

// Load the cover file (cover.jpg or cover.png) from an MKV file
bool MainWindow::__getCoverFromMKV( const QString sNewFile, const QString sFN1, const QString sFN2 )  {

    AVFormatContext *pFormatCtx = nullptr;
    QImage imgCover;
    bool bFound = false;

    // Open File
    if ( avformat_open_input(&pFormatCtx, sNewFile.toLocal8Bit().data(), nullptr, nullptr) == 0 )  {

        // Read stream information (IMPORTANT for correct scheduling flags)
        if ( avformat_find_stream_info(pFormatCtx, nullptr) >= 0 )  {

            for(unsigned int i = 0; i < pFormatCtx->nb_streams; i++ )  {

                if ( pFormatCtx->streams[i]->disposition & AV_DISPOSITION_ATTACHED_PIC )  {

                    AVDictionaryEntry *tag = av_dict_get( pFormatCtx->streams[i]->metadata, "filename", nullptr, 0 );

                    if ( tag )  {

                        QString sImageName = QString::fromUtf8( tag->value ).toLower();

                        if ( sImageName == sFN1 || sImageName == sFN2 )  {

                            // ------------------- Collect data -------------------------
                            CMI.bCurrentValid      = true;
                            CMI.sCurrentFilename   = sNewFile;
                            CMI.sCurrentImageName  = sImageName;

                            AVPacket &pkt = pFormatCtx->streams[i]->attached_pic;
                            imgCover.loadFromData( pkt.data, pkt.size );

                            CMI.i64CurrentFilesize = pkt.size;
                            CMI.imgCurrentCover    = imgCover;
                            CMI.iCurrentCoverw     = imgCover.width();
                            CMI.iCurrentCoverh     = imgCover.height();
                            CMI.iCurrentWindoww    = this->width();
                            CMI.iCurrentWindowh    = this->height();
                            // -------------------------------------------
                          __updateExtraFileInfoSelectable( true );

                            bFound = true;
                            break;      // End the loop, but don't return yet!
                        }
                    }
                }
            }
        }

        // ALWAYS close when pFormatCtx has been initialized
        avformat_close_input( &pFormatCtx );
    }

    // If found, stop the function here
    if ( bFound )  return true;


  __showErrorMessage( QString("%1 or %2 not found").arg(sFN1).arg(sFN2) );
    return false;

/*

    // The file does not contain a cover; reset all ads
    setWindowTitle( "CoverSpotter" );

    if ( CMI.sCurrentFilename.isEmpty() )  imageLabel->setText( "No file loaded" );
    else  imageLabel->setText( QString("%1 or %2 not found").arg(sFN1).arg(sFN2) );

    CMI.bCurrentValid      = false;
    CMI.imgCurrentCover    = QImage();       // Leeren
    CMI.sCurrentFilename.clear();
    CMI.sCurrentImageName.clear();
    CMI.iCurrentCoverw     = 0;     CMI.iCurrentCoverh  = 0;
    CMI.iCurrentWindoww    = 0;     CMI.iCurrentWindowh = 0;
    CMI.i64CurrentFilesize = 0;

  __updateExtraFileInfoSelectable( false );

    imageLabel->setFixedSize( CS_MINW, CS_MINH );
    this->resize( 1, 1 );
    this->adjustSize();
    imageLabel->setMinimumSize( CS_MINW, CS_MINH );
    imageLabel->setMaximumSize( QWIDGETSIZE_MAX, QWIDGETSIZE_MAX );

*/

}



// Show uploaded cover
void MainWindow::__showCover( void )  {

    if ( CMI.bCurrentValid == false )  return;


    QPixmap pixmap = QPixmap::fromImage( CMI.imgCurrentCover );

    // Safety check: Maximum width
    int iMax = 800;  if ( CMI.iMode == CS_LANDSCAPEMODE )  iMax = 1067;
    if ( pixmap.width() > iMax )  pixmap = pixmap.scaledToWidth( iMax, Qt::SmoothTransformation );

    // Remove the FixedSize setting after the error occurs
    imageLabel->setMinimumSize( CS_MINW, CS_MINH );
    imageLabel->setMaximumSize( QWIDGETSIZE_MAX, QWIDGETSIZE_MAX );

    // Calculate how much space the layout takes up
    int iMarginL, iMarginT, iMarginR, iMarginB;
    centralWidget()->layout()->getContentsMargins( &iMarginL, &iMarginT, &iMarginR, &iMarginB );

    // Make the window large enough to fit the image and margins
    // (Any buttons that extend vertically must also be taken into account here)
    int iTargetWidth  = (pixmap.width() + iMarginL + iMarginR) - 2;
    int iTargetHeight = pixmap.height() + iMarginT + iMarginB + btnOpen->height();     // if btn is available

    this->resize( iTargetWidth, iTargetHeight );

    // Check the label now—it should now be exactly pixmap.width() pixels wide
    imageLabel->setPixmap( pixmap.scaled(

           imageLabel->size(),
           Qt::KeepAspectRatio,
           Qt::SmoothTransformation

    ));
    imageLabel->setMinimumSize( CS_MINW, CS_MINH );

    // Show title in file name
    QFileInfo xFileInfo( CMI.sCurrentFilename );
    setWindowTitle( xFileInfo.fileName() + " – CoverSpotter" );

    // Remember changed window dimensions
    CMI.iCurrentWindoww    = this->width();
    CMI.iCurrentWindowh    = this->height();

}


// Darkmode
void MainWindow::__setTheme( const int iTheme )  {

    if ( iTheme == RADIOBUTTON_1 )  {

            //////////////////////////////
            /// DARKMODE

            QPalette darkPal;

            darkPal.setColor(QPalette::Window,          QColor(55, 55, 55));
            darkPal.setColor(QPalette::WindowText,      Qt::white);
            darkPal.setColor(QPalette::Base,            QColor(0,0,0));
            darkPal.setColor(QPalette::AlternateBase,   QColor(45, 45, 45));
            darkPal.setColor(QPalette::ToolTipBase,     Qt::white);
            darkPal.setColor(QPalette::ToolTipText,     Qt::white);
            darkPal.setColor(QPalette::Text,            Qt::white);
            darkPal.setColor(QPalette::Button,          QColor(45, 45, 45));
            darkPal.setColor(QPalette::ButtonText,      Qt::white);
            darkPal.setColor(QPalette::BrightText,      Qt::red);
            darkPal.setColor(QPalette::Highlight,       QColor(42, 130, 218));
            darkPal.setColor(QPalette::HighlightedText, Qt::black);

            // The color for normal links (e.g., light blue)
            darkPal.setColor(QPalette::Link, Qt::white);

            // The color for links that have already been visited (e.g., a lighter shade of purple)
            darkPal.setColor(QPalette::LinkVisited, Qt::white);

            // The color of the frame (often Mid or Dark)
            darkPal.setColor(QPalette::Mid, QColor(90, 90, 90));
            darkPal.setColor(QPalette::Light, QColor(160, 160, 160)); // Eine hellere Akzentkante

            // Shadow should remain dark to ensure the right contrast
            darkPal.setColor(QPalette::Shadow, Qt::black);

            // Text color for DISABLED elements (buttons, labels, etc.)
            darkPal.setColor(QPalette::Disabled, QPalette::WindowText, QColor(120, 120, 120));
            darkPal.setColor(QPalette::Disabled, QPalette::Text,       QColor(120, 120, 120));
            darkPal.setColor(QPalette::Disabled, QPalette::ButtonText, QColor(120, 120, 120));

            // Optional: Slightly darken the background of disabled input fields as well
            darkPal.setColor(QPalette::Disabled, QPalette::Base,       QColor(40, 40, 40));

            // Horizontal line
            darkPal.setColor(QPalette::Mid, QColor(67, 67, 67));
            darkPal.setColor(QPalette::Dark, QColor(60, 60, 60));
            darkPal.setColor(QPalette::Light, QColor(67, 67, 67));

            qApp->setPalette( darkPal );

    } else {

            //////////////////////////////
            /// WHITEMODE

            // Get a clean, fresh palette
            QPalette lightPal = QApplication::palette();

            // Explicitly reset the critical roles to "Hell"
            lightPal.setColor(QPalette::Window,          QColor(240, 240, 240));
            lightPal.setColor(QPalette::WindowText,      Qt::black);
            lightPal.setColor(QPalette::Base,            Qt::white);
            lightPal.setColor(QPalette::Text,            Qt::black);
            lightPal.setColor(QPalette::Button,          QColor(240, 240, 240));
            lightPal.setColor(QPalette::ButtonText,      Qt::black);
            lightPal.setColor(QPalette::Link,            Qt::blue);

            // Specifically for DISABLED elements on a light background (dark gray on light gray)
            QColor disabledGray(160, 160, 160);     // A classic "grayed-out"
            lightPal.setColor(QPalette::Disabled, QPalette::WindowText, disabledGray);
            lightPal.setColor(QPalette::Disabled, QPalette::Text,       disabledGray);
            lightPal.setColor(QPalette::Disabled, QPalette::ButtonText, disabledGray);

            // Optional: Slightly darken the background of disabled fields
            lightPal.setColor(QPalette::Disabled, QPalette::Base, QColor(225, 225, 225));

            qApp->setPalette( lightPal );

    }

    // Set tooltips based on the theme
    if ( iTheme == RADIOBUTTON_1 ) {

            // Dark mode: Dark background, light text
            qApp->setStyleSheet(
                "QToolTip { "
                "color: #ffffff; "
                "background-color: #353535; "
                "border: 1px solid #555555; "
                "}"
            );

    } else {

            // Light mode: Light background, dark text
            qApp->setStyleSheet(
                "QToolTip { "
                "color: #000000; "
                "background-color: #f0f0f0; "
                "border: 1px solid #ababab; "
                "}"
            );
    }

    // Set the selection for the current mode in the extra combo box—depending on the theme
    static QIcon xCheckIconDark(  ":/Images/CheckmarkDark.png" );
    static QIcon xCheckIconWhite( ":/Images/CheckmarkWhite.png" );

    xActiveCheckIcon = ( CMI.iTheme == RADIOBUTTON_1 ) ? xCheckIconDark : xCheckIconWhite;

    if ( CMI.iMode == CS_LANDSCAPEMODE )  {  cmboExtras->setItemIcon( COMBOBOX_3, xActiveCheckIcon );
                                             cmboExtras->setItemIcon( COMBOBOX_2, QIcon() );    // Remove icon
                                          }
    else  {  cmboExtras->setItemIcon( COMBOBOX_2, xActiveCheckIcon );
             cmboExtras->setItemIcon( COMBOBOX_3, QIcon() );    // Remove icon
          }

}



// Respond to ALT+1/2/3
void MainWindow::__changeWindowSize( const int iIndex )  {

    if ( CMI.bCurrentValid == false )  return;

    if ( CMI.iMode == CS_PORTRAITMODE )
    switch( iIndex )  {  case 1  :  this->resize( CMI.iCurrentWindoww / 2,   CMI.iCurrentWindowh / 2 );    break;
                         case 2  :  this->resize( CMI.iCurrentWindoww,       CMI.iCurrentWindowh );        break;
                         case 3  :  this->resize( CMI.iCurrentWindoww * 1.5, CMI.iCurrentWindowh * 1.5 );  break;
                      }

    if ( CMI.iMode == CS_LANDSCAPEMODE )
    switch( iIndex )  {  case 1  :  this->resize( CMI.iCurrentWindoww / 1.5, CMI.iCurrentWindowh / 1.5 );  break;
                         case 2  :  this->resize( CMI.iCurrentWindoww,       CMI.iCurrentWindowh );        break;
                         case 3  :  this->resize( CMI.iCurrentWindoww * 1.5, CMI.iCurrentWindowh * 1.5 );  break;
                      }

}



// Resets the position and size of the main window to their default values
void MainWindow::__resetMainWindowPosSize( void )  {

    // Determine the primary display
    QScreen *xPrimaryScreen = QGuiApplication::primaryScreen();

    // Retrieve the dimensions of the main screen (without the taskbar)
    QRect rectScreenGeometry = xPrimaryScreen->availableGeometry();

    // Set the initial size of the window
    this->resize( CS_MINW, CS_MINH );

    // Move window
    this->move( rectScreenGeometry.left() + 100, rectScreenGeometry.top() + 100 );

}



// Set the window's dimensions and position if they were saved in CoverSpotter.ini
// Otherwise, set the default value
void MainWindow::__setGeomentry( void )  {

    // Set window size or default
    if ( !CMI.xGeometry.isEmpty() )  restoreGeometry( CMI.xGeometry );     // Restore saved dimensions and position
    else  __resetMainWindowPosSize();

    // IMPORTANT: After calling setWindowFlags, we must call show()
    // to make the window visible again
    this->show();

}



// Set the status of the "File Info" entry in the extra combo box
void MainWindow::__updateExtraFileInfoSelectable( const bool bValid )  {

    // Access the default model of the ComboBox
    QStandardItemModel* model = qobject_cast<QStandardItemModel*>( cmboExtras->model() );

    if ( model )  {

        QStandardItem *item = model->item( COMBOBOX_1 );
        if ( item )  {

            // Sets flags: If not valid, it is neither selectable nor active
            item->setFlags(bValid ? (Qt::ItemIsSelectable | Qt::ItemIsEnabled)
                                  : Qt::NoItemFlags);

            // Optional: Gray out the text color for better feedback
            item->setData(bValid ? QVariant() : QColor(Qt::gray), Qt::ForegroundRole);

        }
    }
}



// Display a message in white text on a red background in the center of the window for about 2 seconds
void MainWindow::__showErrorMessage( const QString sMessage ) {

    // Create a label if it doesn't already exist
    // 'this' ensures that it is positioned above the MainWindow
    QLabel *errorLabel = new QLabel( sMessage, this );

    // Styling: Roter Hintergrund, weiße Schrift
    errorLabel->setStyleSheet(

        "background-color: red; "
        "color: white; "
        "font-weight: bold; "
        "padding: 10px 20px;"

    );

    errorLabel->setAttribute( Qt::WA_DeleteOnClose );   // Release memory when closing
    errorLabel->setAlignment( Qt::AlignCenter );
    errorLabel->adjustSize();   // Adjust text size

    // Place in the center of the main window
    errorLabel->move( (width() - errorLabel->width()) / 2,
                      ((height() - errorLabel->height()) / 2) - 10
                    );

    errorLabel->show();

    // Timer: Close the label after 2000 ms (2 seconds)
    QTimer::singleShot( 2000, errorLabel, &QWidget::close );

}

