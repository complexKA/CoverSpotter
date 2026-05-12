#ifndef CSFILEINFO_H
#define CSFILEINFO_H

#include <QDialog>

namespace Ui {
class CSfileinfo;
}

class CSfileinfo : public QDialog
{
    Q_OBJECT

public:
    explicit CSfileinfo( QWidget *parent = nullptr );
    ~CSfileinfo();

private slots:
    void on_pb_Ok_clicked();

private:
    Ui::CSfileinfo *ui;
};

#endif // CSFILEINFO_H
