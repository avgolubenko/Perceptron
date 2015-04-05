#include "create-dialog.h"

CreateModelDialog::CreateModelDialog(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    layerGroupBox2->hide();
    layerGroupBox3->hide();
    // установка менеджером компоновки фиксированного размера диалогового окна
    layout()->setSizeConstraint(QLayout::SetFixedSize);
    //
    connect(layersNumSpinBox,SIGNAL(valueChanged(int)),this,SLOT(on_layersNumSpinBox_valueChanged(int)));
}

void CreateModelDialog::on_layersNumSpinBox_valueChanged(int i)
{
    layerGroupBox2->setVisible(i >= 2);
    layerGroupBox3->setVisible(i >= 3);
}
