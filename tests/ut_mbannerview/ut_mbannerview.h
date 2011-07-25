
#ifndef UT_MBANNERVIEW_H
#define UT_MBANNERVIEW_H

#include <QObject>

class MBanner;
class MBannerView;
class QGraphicsGridLayout;
class QGraphicsLayoutItem;

class Ut_MBannerView : public QObject
{
    Q_OBJECT

private slots:

    void initTestCase();
    void cleanupTestCase();

    void testInstantiateBannerView();
    void testBannerModelDatainViews();
    void testBannerPixmapAndIcon();
    void testBannerDateFormats();
    void testBannerMouseEvents();
    void testMouseMoveEvent();
    void testDrawForeground();
    void testDrawContents();
    void testBannerStyles();
    void testPrivateManageOpacities();

private:

    void testPrivateLayoutShortEventBanner();
    void testPrivateLayoutInformationBanner();
    void testPrivateLayoutSystemBanner();
    void testPrivateLayoutFullEventBanner();
    void testPrivateLayoutLockScreenEventBanner();
    void testPrivateLayoutPrivateEventBanner();

    void testLayoutSetup();
    void testGridItemsExist();
    void testTitleLabel();
    void testSubtitleLabel();
    void testPixmapOrIconGrid();
    void testPixmapOrIconLayout();
    bool findItemInGridLayout(QGraphicsGridLayout *layout,
                              QGraphicsLayoutItem *item);
    void testLayoutTimeStamp();

    MBanner *m_banner;
    MBannerView *m_subject;
};

#endif
