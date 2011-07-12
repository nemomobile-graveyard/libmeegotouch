
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
    void testBannerModelDatainViews();
    void testBannerDateFormats();
    void testBannerPixmapAndIcon();
    void testBannerMouseEvents();

    void testInstantiateBannerView();
    void testMouseMoveEvent();
    void testDrawForeground();
    void testDrawContents();

    void testPrivateCreateGrid();
    void testPrivateLayoutShortEventBanner();
    void testPrivateLayoutInformationBanner();
    void testPrivateLayoutSystemBanner();
    void testPrivateLayoutFullEventBanner();
    void testPrivateLayoutLockScreenEventBanner();
    void testPrivateLayoutPrivateEventBanner();
    void testPrivateManageOpacities();

private:

    void testLayoutSetup();
    void testSystemBannerLayoutItemsExist();
    bool findItemInGridLayout(QGraphicsGridLayout *layout,
                              QGraphicsLayoutItem *item);
    void testLayoutTimeStamp();


    MBanner *m_banner;
    MBannerView *m_subject;
};

#endif
