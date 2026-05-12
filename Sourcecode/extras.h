#ifndef EXTRAS_H
#define EXTRAS_H

#include <QDialog>

class MainWindow; // Forward Declaration


namespace Ui {
class Extras;
}

class Extras : public QDialog
{
    Q_OBJECT

public:
    explicit Extras( MainWindow *pMainWindow, QWidget *parent = nullptr );
    ~Extras();

private slots:
    void on_pb_Info_clicked();
    void on_pb_About_clicked();

    void on_pb_Portrait_clicked();
    void on_pb_Landscape_clicked();

    void on_pb_Quit_clicked();

private:
    Ui::Extras *ui;
    MainWindow *pMainWindow;
};

#endif // EXTRAS_H
