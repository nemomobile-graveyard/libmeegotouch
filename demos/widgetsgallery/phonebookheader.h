/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of libmeegotouch.
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

#ifndef PHONEBOOKHEADER_H
#define PHONEBOOKHEADER_H

#include <MStylableWidget>

class MLabel;
class MLayout;

class PhoneBookHeader : public MStylableWidget
{
    Q_OBJECT
public:
    PhoneBookHeader(QGraphicsItem *parent=NULL);

    void setGroupTitle(const QString &title);
    void setGroupCount(int count);

protected:
    MLabel *groupTitleLabelWidget();
    MLabel *groupCountLabelWidget();
    
    void resizeEvent(QGraphicsSceneResizeEvent *event);
    MLayout *createLayout();

private:
    MLayout *layout;

    MLabel *groupTitleLabel;
    MLabel *groupCountLabel;
};

#endif // PHONEBOOKHEADER_H
