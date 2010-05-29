#ifndef MYDIALOG_H
#define MYDIALOG_H

#include <MDialog>

class MTextEdit;

class MyDialog : public MDialog
{
    Q_OBJECT
public:
    MyDialog();

private Q_SLOTS:
    void focusTextEntry();

private:
    MTextEdit *textEdit;
};

#endif // MYDIALOG_H
