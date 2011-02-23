#ifndef MEDITORTOOLBARARROW_H
#define MEDITORTOOLBARARROW_H

#include <MStylableWidget>
#include "meditortoolbararrowstyle.h"

//! \internal

class MEditorToolbarArrow : public MStylableWidget
{
    Q_OBJECT
public:

    enum ArrowDirection {
        ArrowUp,
        ArrowDown
    };

    MEditorToolbarArrow(QGraphicsItem *parentItem = 0);

    ArrowDirection direction();
    void setDirection(ArrowDirection direction);

private:
    ArrowDirection arrowDirection;
    M_STYLABLE_WIDGET(MEditorToolbarArrowStyle)
};

//! \internal_end

#endif // MEDITORTOOLBARARROW_H
