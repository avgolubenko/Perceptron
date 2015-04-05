#ifndef CREATEDIALOG_H
#define CREATEDIALOG_H

#include <QDialog>
#include "ui_create-dialog.h"

class CreateModelDialog : public QDialog, public Ui::CreateModelDialog
{
    Q_OBJECT

public:
    explicit CreateModelDialog(QWidget *parent = 0);
private slots:
    void on_layersNumSpinBox_valueChanged(int i);
};

#endif // CREATEDIALOG_H
