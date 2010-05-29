#ifndef MYPAGE_H
#define MYPAGE_H

#include <MApplicationPage>

class MTextEdit;

class MyPage : public MApplicationPage
{
    Q_OBJECT
public:
    MyPage();

protected:
    virtual bool event(QEvent *e);

private:
    MTextEdit *textEdit;
};

#endif // MYPAGE_H
