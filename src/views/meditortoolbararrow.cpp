#include "meditortoolbararrow.h"

MEditorToolbarArrow::MEditorToolbarArrow(QGraphicsItem *parentItem)
    : MStylableWidget(parentItem),
      arrowDirection(ArrowDown)
{
}

MEditorToolbarArrow::ArrowDirection MEditorToolbarArrow::direction()
{
    return arrowDirection;
}

void MEditorToolbarArrow::setDirection(ArrowDirection direction)
{
    arrowDirection = direction;

    switch (direction) {
    case ArrowUp:
        style().setModeUp();
        break;
    case ArrowDown:
        style().setModeDown();
        break;
    }

    // Update new size.
    applyStyle();
}
