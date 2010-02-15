/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of libdui.
**
** If you have questions regarding the use of this file, please contact
** Nokia at directui@nokia.com.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/

/****************************************************************************
**
** This file was initially based on a part of a Qt Solutions component.
**
** Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
**
** Contact:  Qt Software Information (qt-info@nokia.com)
**
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Solutions Commercial License Agreement provided
** with the Software or, alternatively, in accordance with the terms
** contained in a written agreement between you and Nokia.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain
** additional rights. These rights are described in the Nokia Qt LGPL
** Exception version 1.0, included in the file LGPL_EXCEPTION.txt in this
** package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** Please note Third Party Software included with Qt Solutions may impose
** additional restrictions and it is the user's responsibility to ensure
** that they have met the licensing requirements of the GPL, LGPL, or Qt
** Solutions Commercial license and the relevant license of the Third
** Party Software they are using.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
**
****************************************************************************/

#include <QtGui>
#include "../../src/core/duiexport.h"
#include <DuiApplication>
#include <DuiImageWidget>
#include <DuiScene>
#include <DuiLayout>
#include <DuiLinearLayoutPolicy>
#include <DuiGridLayoutPolicy>
#include <DuiFlowLayoutPolicy>
#include <QGraphicsWidget>
//#include <DuiFreestyleLayoutPolicy>
#include <DuiLabel>
#include <DuiButton>
#include <DuiButtonGroup>
#include <QRectF>
#include <DuiLocale>
#include <DuiGConfItem>

#define USE_INNER_FORM

class AddItemButton;

DuiScene *s_scene;
QList<DuiAbstractLayoutPolicy *> s_policies;
DuiFlowLayoutPolicy *s_flowLayoutPolicy;
DuiLinearLayoutPolicy *s_innerPolicy;
AddItemButton *s_addItemButton;

/*!
 * \class AnimatedlayoutRetranslator
 * \brief AnimatedlayoutRetranslator is a test class to test translation on the fly
 *
 * This is just a simple test which will be changed soon.
 *
 */
class AnimatedlayoutRetranslator : public QObject
{
    Q_OBJECT

public slots:
    void animatedlayoutRetranslate() {
        qDebug() << __PRETTY_FUNCTION__;
        DuiGConfItem languageItem("/Dui/i18n/Language");
        QString language = languageItem.value().toString();
        DuiLocale locale(language);
        QString catalog = "animatedlayout";
        locale.installTrCatalog(catalog);
        DuiLocale::setDefault(locale);

        // tell the scene and its items about the language change
        QList<QGraphicsItem *> items = s_scene->items();

        foreach(QGraphicsItem * item, items) {
            // call setLayoutDirection_helper() for all top-level items
            if (!item->parentItem())
                this->setLayoutDirection_helper(static_cast<QGraphicsWidget *>(item));

            if (item->isWidget()) {
                QGraphicsWidget *widget = static_cast<QGraphicsWidget *>(item);

                QEvent ev(QEvent::LanguageChange);
                qApp->sendEvent(widget, &ev);
                QEvent evlayout(QEvent::LanguageChange);
                qApp->sendEvent(widget, &evlayout);
            }
        }
    }

private:
    void setLayoutDirection_helper(QGraphicsItem *item) {
        if (item->isWidget()) {
            QGraphicsWidget *widget = static_cast<QGraphicsWidget *>(item);
            Qt::LayoutDirection direction = qApp->layoutDirection();
            // if the direction has not changed or has been specified
            // directly, do not update.
            if (((direction == Qt::RightToLeft) == widget->testAttribute(Qt::WA_RightToLeft))
                    || widget->testAttribute(Qt::WA_SetLayoutDirection))
                return;
            widget->setAttribute(Qt::WA_RightToLeft, (direction == Qt::RightToLeft));
        }
        // Propagate this change to all children.
        const int childItemsSize = item->childItems().size();
        for (int i = 0; i < childItemsSize; ++i) {
            QGraphicsItem *childItem = item->childItems().at(i);
            setLayoutDirection_helper(childItem);
        }
        if (item->isWidget()) {
            // Send the notification event to this widget item.
            QEvent e(QEvent::LayoutDirectionChange);
            QApplication::sendEvent(static_cast<QGraphicsWidget *>(item), &e);
        }
    }
};

class ContainerWidget : public QGraphicsWidget
{
public:
    ContainerWidget(QGraphicsItem *parent = 0)
        : QGraphicsWidget(parent)
    {}
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) {
        Q_UNUSED(option);
        Q_UNUSED(widget);
        QLinearGradient linearGrad(QPointF(0, 0), QPointF(size().width() / 5, size().height() / 5));
        linearGrad.setColorAt(0, QColor(0, 0, 0, 255));
        linearGrad.setColorAt(1, QColor(0, 0, 0, 0));
        painter->setPen(Qt::NoPen);
        painter->setBrush(linearGrad);
        painter->drawRoundedRect(0, 0, size().width() / 2, size().height() / 2, 5, 5);

        QLinearGradient linearGrad2(QPointF(size().width(), 0), QPointF(size().width() * 4 / 5, size().height() / 5));
        linearGrad2.setColorAt(0, QColor(0, 0, 0, 255));
        linearGrad2.setColorAt(1, QColor(0, 0, 0, 0));
        painter->setBrush(linearGrad2);
        painter->drawRoundedRect(size().width() / 2, 0, size().width() / 2, size().height() / 2, 5, 5);
    }
};
class Button : public QGraphicsWidget
{
    Q_OBJECT
public:
    Button(const QPixmap &pixmap, QGraphicsItem *parent = 0)
        : QGraphicsWidget(parent), _pix(pixmap) {
        setAcceptHoverEvents(true);
        setCacheMode(DeviceCoordinateCache);
    }

    QRectF boundingRect() const {
        return QRectF(-65, -65, 130, 130);
    }

    QPainterPath shape() const {
        QPainterPath path;
        path.addEllipse(boundingRect());
        return path;
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *) {
        bool down = option->state & QStyle::State_Sunken;
        QRectF r = boundingRect();
        QLinearGradient grad(r.topLeft(), r.bottomRight());
        grad.setColorAt(down ? 1 : 0, option->state & QStyle::State_MouseOver ? Qt::white : Qt::lightGray);
        grad.setColorAt(down ? 0 : 1, Qt::darkGray);
        painter->setPen(Qt::darkGray);
        painter->setBrush(grad);
        painter->drawEllipse(r);
        QLinearGradient grad2(r.topLeft(), r.bottomRight());
        grad.setColorAt(down ? 1 : 0, Qt::darkGray);
        grad.setColorAt(down ? 0 : 1, Qt::lightGray);
        painter->setPen(Qt::NoPen);
        painter->setBrush(grad);
        if (down)
            painter->translate(2, 2);
        painter->drawEllipse(r.adjusted(5, 5, -5, -5));
        painter->drawPixmap(-_pix.width() / 2, -_pix.height() / 2, _pix);
    }

signals:
    void pressed();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *) {
        emit pressed();
        update();
    }

    void mouseReleaseEvent(QGraphicsSceneMouseEvent *) {
        update();
    }

private:
    QPixmap _pix;
};

class AnimatedlayoutLabel : public DuiLabel
{
    Q_OBJECT
public:
    AnimatedlayoutLabel(const QString &messageId, DuiWidget *parent = 0)
        : DuiLabel(parent), _messageId(messageId) {
        this->setText(qtTrId(_messageId.toUtf8().constData()));
    }
protected:
    virtual void retranslateUi() {
        this->setText(qtTrId(_messageId.toUtf8().constData()));
    }
private:
    QString _messageId;
};

class ChangePolicyObjectNameButton : public DuiButton
{
    Q_OBJECT
public:
    ChangePolicyObjectNameButton(const QString &messageId, const QString &objectName, DuiWidget *parent = 0)
        : DuiButton(parent), _objectName(objectName), _messageId(messageId) {
        retranslateUi();
    }
protected:
    virtual void retranslateUi() {
        this->setText(qtTrId(_messageId.toUtf8().constData()));
    }

    void mousePressEvent(QGraphicsSceneMouseEvent *event) {
        foreach(DuiAbstractLayoutPolicy * policy, s_policies) {
            policy->setObjectName(_objectName);
        }
        DuiButton::mousePressEvent(event);
    }
private:
    QString _objectName;
    QString _messageId;
};


class Image : public DuiImageWidget
{
public:
    explicit Image(const QString &imageName, DuiWidget *parent = 0)
        : DuiImageWidget(imageName, parent) {
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
    }
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) {
        DuiLayout *layout = dynamic_cast<DuiLayout *>(parentLayoutItem());
        if (layout) {
            if (event && event->button() == Qt::RightButton) {
                layout->removeItem(this);
                deleteLater();
            } else
                layout->animatedDeleteItem(this);
        }
    }
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
        //Paint a white background so that we completely paint the bounding box
        painter->setPen(Qt::NoPen);
        painter->setBrush(Qt::white);
        painter->drawRect(QRectF(0, 0, size().width(), size().height()));
        DuiImageWidget::paint(painter, option, widget);
    }
};

class AddItemButton : public DuiButton
{
public:
    AddItemButton(DuiWidget *parent = 0)
        : DuiButton("+", parent)
    {}
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) {
        Image *item = new Image("dui-logo-red");
        item->setObjectName("new");
        item->setVisible(false);

        s_flowLayoutPolicy->insertItem(10, item);
        DuiButton::mousePressEvent(event);
    }
};


class ChangePolicyButton : public Button
{
public:
    ChangePolicyButton(const QPixmap &pixmap, DuiAbstractLayoutPolicy *policy, QGraphicsItem *parent = 0)
        : Button(pixmap, parent), _policy(policy)
    {}
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) {
        _policy->activate();
        Button::mousePressEvent(event);
        DuiLinearLayoutPolicy *linearPolicy = dynamic_cast<DuiLinearLayoutPolicy *>(_policy);
        if (linearPolicy &&
                linearPolicy->orientation() == Qt::Vertical)
            s_innerPolicy->setOrientation(Qt::Horizontal);
        else
            s_innerPolicy->setOrientation(Qt::Vertical);
        s_addItemButton->setVisible(_policy == s_flowLayoutPolicy);
    }
private:
    DuiAbstractLayoutPolicy *_policy;
};

class View : public QGraphicsView
{
public:
    View(QGraphicsScene *scene) : QGraphicsView(scene) { }

protected:
    void resizeEvent(QResizeEvent *event) {
        QGraphicsView::resizeEvent(event);
        fitInView(sceneRect(), Qt::KeepAspectRatio);
    }
};


int main(int argc, char **argv)
{
    DuiApplication app(argc, argv);

    AnimatedlayoutRetranslator animatedlayoutRetranslator;
    QObject::connect(&app, SIGNAL(localeSettingsChanged()), &animatedlayoutRetranslator, SLOT(animatedlayoutRetranslate()));

    QPixmap *bgPix = new QPixmap("images/background.png");

    s_scene = new DuiScene;
    int offset = 0;  //sets where the background image starts tiling from
    s_scene->setSceneRect(offset, offset, 864, 480);

    //We scale the background pixmap to be a bit bigger than the actual scene because
    //when its resized, the aspect ratio might not be exactly 1:1.  It's an ugly hack
    QGraphicsPixmapItem *background = new QGraphicsPixmapItem(bgPix->scaled(864 + 10, 480 + 10));
    s_scene->addItem(background);
    background->setZValue(-1);
    background->setPos(-5, -5);

    DuiLayout *layout = new DuiLayout;
    QGraphicsWidget *form = new ContainerWidget;
    form->setLayout(layout);
    s_scene->addItem(form);
    form->setGeometry(offset, offset, 864, 480);
    form->setPreferredWidth(864);
    form->setPreferredHeight(480);
    form->setMaximumWidth(864);
    form->setMaximumHeight(480);
    form->setMinimumWidth(864);
    form->setMinimumHeight(480);

    DuiGridLayoutPolicy *gridPolicy = new DuiGridLayoutPolicy(layout);
    DuiLinearLayoutPolicy *linearHorizontalPolicy = new DuiLinearLayoutPolicy(layout, Qt::Horizontal);
    DuiLinearLayoutPolicy *linearVerticalPolicy = new DuiLinearLayoutPolicy(layout, Qt::Vertical);
    s_flowLayoutPolicy = new DuiFlowLayoutPolicy(layout);
    DuiAbstractLayoutPolicy *emptyPolicy = new DuiLinearLayoutPolicy(layout, Qt::Horizontal);
    s_policies << linearHorizontalPolicy;
    s_policies << linearVerticalPolicy;
    s_policies << gridPolicy;
    s_policies << s_flowLayoutPolicy;
    s_policies << emptyPolicy;
//    DuiFreestyleLayoutPolicy *freestylePolicy = new DuiFreestyleLayoutPolicy(layout);

    DuiLayout *innerLayout = new DuiLayout;
#ifdef USE_INNER_FORM
    QGraphicsWidget *innerForm = new QGraphicsWidget;
    innerForm->setLayout(innerLayout);
    innerForm->setContentsMargins(0, 0, 0, 0);
    innerForm->setMaximumWidth(50);
    innerForm->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
#endif
    s_innerPolicy = new DuiLinearLayoutPolicy(innerLayout, Qt::Vertical);
    innerLayout->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    for (int i = 0; i < 4; ++i) {
        Image *item = new Image("dui-logo-blue");
        item->setObjectName("blue");
        item->setVisible(false);
        //add to active policy first
        s_innerPolicy->addItem(item);
    }

    int i = 0;
    for (int y = 0; y < 10; ++y) {
        for (int x = 0; x < 4; ++x, ++i) {
            Image *item = new Image("dui-logo");
            item->setZValue(i);
            item->setVisible(false);
            //add to active policy first
            if (y < 4) {
                gridPolicy->addItem(item, y, x);
                Q_ASSERT(item->scene());
                linearHorizontalPolicy->addItem(item);
                linearVerticalPolicy->addItem(item);
//              freestylePolicy->addItemAtGeometry(item, QRectF(100*x,100*y,130,130));
            }
            s_flowLayoutPolicy->addItem(item);
        }
        gridPolicy->setColumnMaximumWidth(y, 50);
    }

    //% "Hello"
    AnimatedlayoutLabel  *textlabel = new AnimatedlayoutLabel(QT_TRID_NOOP("xx_hello"));
    textlabel->setObjectName("myLabelMultiLength");
    textlabel->setTextElide(true);
    gridPolicy->addItem(textlabel, 4, 0);
    linearHorizontalPolicy->addItem(textlabel);
    linearVerticalPolicy->addItem(textlabel);
#ifdef USE_INNER_FORM
    s_flowLayoutPolicy->addItem(textlabel);
    gridPolicy->addItem(innerForm, 4, 1, 1, 1);
    linearHorizontalPolicy->addItem(innerForm);
    linearVerticalPolicy->addItem(innerForm);
    s_flowLayoutPolicy->addItem(innerForm);
#else
    s_flowLayoutPolicy->addItem(textlabel);
    gridPolicy->addItem(innerLayout, 4, 1, 1, 1);
    linearHorizontalPolicy->addItem(innerLayout);
    linearVerticalPolicy->addItem(innerLayout);
    s_flowLayoutPolicy->addItem(innerLayout);
#endif

    Q_ASSERT(dynamic_cast<QGraphicsWidget *>(s_innerPolicy->itemAt(0))->scene());

    // Buttons
    QGraphicsWidget *buttonParent = new QGraphicsWidget;
    QList<Button *> buttons;
    buttons << new ChangePolicyButton(QPixmap("images/horizontal.png").scaled(64, 64), linearHorizontalPolicy, buttonParent);
    buttons << new ChangePolicyButton(QPixmap("images/vertical.png").scaled(64, 64), linearVerticalPolicy, buttonParent);
    buttons << new ChangePolicyButton(QPixmap("images/tile.png").scaled(64, 64), gridPolicy, buttonParent);
    buttons << new ChangePolicyButton(QPixmap("images/flow.png").scaled(64, 64), s_flowLayoutPolicy, buttonParent);
    buttons << new ChangePolicyButton(QPixmap("").scaled(64, 64), emptyPolicy, buttonParent);
//    buttons << new ChangePolicyButton(QPixmap("images/flow.png").scaled(64,64), freestylePolicy, buttonParent);

    const int radius = 130;
    const int buttonsCount = buttons.count();
    for (int i = 0; i < buttonsCount; ++i) {
        buttons[i]->setPos(radius * sin(2 * 3.1415 * i / buttonsCount), radius * cos(2 * 3.1415 * i / buttonsCount));
    }

    s_scene->addItem(buttonParent);
    buttonParent->scale(0.75, 0.75);
    buttonParent->setPos(670 + offset, 340 + offset);
    buttonParent->setZValue(65);

    QGraphicsWidget *controlWidget = new QGraphicsWidget;
    controlWidget->setZValue(70);
    DuiLayout *changeObjectNameLayout = new DuiLayout(controlWidget);
    changeObjectNameLayout->setAnimation(NULL);
    DuiGridLayoutPolicy *changeObjectNameLayoutPolicy = new DuiGridLayoutPolicy(changeObjectNameLayout);
    s_scene->addItem(controlWidget);
    controlWidget->setGeometry(100, 320, 300, 20);
    DuiButtonGroup *changePolicyObjectNameButtonGroup = new DuiButtonGroup();
    changePolicyObjectNameButtonGroup->setExclusive(true);
    QList<ChangePolicyObjectNameButton *> name_buttons;
    //% "spacing+margins"
    name_buttons << new ChangePolicyObjectNameButton(QT_TRID_NOOP("xx_spacing_margins"), "spacing+margins");
    //% "margins"
    name_buttons << new ChangePolicyObjectNameButton(QT_TRID_NOOP("xx_margins"), "margins");
    //% "spacing"
    name_buttons << new ChangePolicyObjectNameButton(QT_TRID_NOOP("xx_spacing"), "spacing");
    //% "packed"
    name_buttons << new ChangePolicyObjectNameButton(QT_TRID_NOOP("xx_packed"), "packed");

    int buttonIndex = 0;
    //we have to set the first button to be checked before adding it due to a bug in the button group
    name_buttons[0]->setCheckable(true);
    name_buttons[0]->setChecked(true);
    foreach(ChangePolicyObjectNameButton * button, name_buttons) {
        button->setViewType(DuiButton::toggleType);
        button->setCheckable(true);
        changePolicyObjectNameButtonGroup->addButton(button);
        changeObjectNameLayoutPolicy->addItem(button, (buttonIndex / 2), (buttonIndex % 2));
        button->setObjectName("wgToggleButton");
        buttonIndex++;
    }
    name_buttons[0]->click();
    foreach(DuiAbstractLayoutPolicy * policy, s_policies) {
        policy->setObjectName("spacing+margins");
    }

    s_addItemButton = new AddItemButton();
    s_addItemButton->setObjectName("wgPushButton");
    s_addItemButton->setGeometry(QRectF(offset + 864 - s_addItemButton->minimumWidth(), offset + 480 - s_addItemButton->minimumHeight(), s_addItemButton->minimumWidth(), s_addItemButton->minimumHeight()));
    s_scene->addItem(s_addItemButton);
    s_addItemButton->setVisible(false);

    s_policies << linearHorizontalPolicy;
    s_policies << linearVerticalPolicy;
    s_policies << gridPolicy;
    s_policies << s_flowLayoutPolicy;
    s_policies << emptyPolicy;


    View *view = new View(s_scene);
    view->setWindowTitle("Animated Layouts");
    view->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    view->setCacheMode(QGraphicsView::CacheBackground);
    view->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    view->resize(864, 480);
    QTimer::singleShot(300, view, SLOT(invalidateScene()));
    QTimer::singleShot(1000, view, SLOT(invalidateScene()));
    view->show();
    return app.exec();
}

#include "animatedlayout.moc"
