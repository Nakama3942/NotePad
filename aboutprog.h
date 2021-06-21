#ifndef ABOUTPROG_H
#define ABOUTPROG_H

#include <QWidget>

namespace Ui {
class aboutProg;
}

class aboutProg : public QWidget
{
    Q_OBJECT

public:
    explicit aboutProg(QWidget *parent = nullptr);
    ~aboutProg();

private slots:
    void on_buttClose_clicked();

private:
    Ui::aboutProg *ui;
};

#endif // ABOUTPROG_H
