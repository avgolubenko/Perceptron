#ifndef CREATEDIALOG_H
#define CREATEDIALOG_H

#include <QDialog>

namespace Ui {
class CreateDialog;
}

class CreateDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreateDialog(QWidget *parent = 0);
    ~CreateDialog();

private:
    Ui::CreateDialog *ui;
};

#endif // CREATEDIALOG_H
