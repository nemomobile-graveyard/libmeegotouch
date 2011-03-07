
#ifndef UT_MBANNERVIEW_H
#define UT_MBANNERVIEW_H

#include <QObject>

class MBanner;
class MBannerView;
class MBannerViewPrivate;

class Ut_MBannerView : public QObject
{
    Q_OBJECT

private slots:

    void initTestCase();
    void cleanupTestCase();
    void testBannerModelDatainViews();
    void testBannerDateFormats();
    void testBannerPixmapAndIcon();
    void testBannerMouseEvents();

private:

    MBanner *m_banner;
    MBannerView *m_subject;
};

#endif
