#include "mydialog.h"

#include <QGraphicsLinearLayout>
#include <MTextEdit>
#include <MLabel>

MyDialog::MyDialog()
    : MDialog("Dialogs are neat!", 0)
{
    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Vertical, centralWidget());
    MLabel *label = new MLabel("Don't you agree?");
    textEdit =  new MTextEdit(MTextEditModel::SingleLine);
    layout->addItem(label);
    layout->addItem(textEdit);

    addButton("Undoubtedly!");
    addButton("Meh...");

    connect(this, SIGNAL(appeared()), this, SLOT(focusTextEntry()));
}

void MyDialog::focusTextEntry()
{
    textEdit->setFocus();
}
