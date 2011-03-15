#ifndef MEDITORTOOLBARARROWSTYLE_H
#define MEDITORTOOLBARARROWSTYLE_H

#include <MWidgetStyle>

/*!
    \brief This defines style for the small arrow in MEditorToolbar.
*/
class M_VIEWS_EXPORT MEditorToolbarArrowStyle : public MWidgetStyle
{
    Q_OBJECT
    M_STYLE(MEditorToolbarArrowStyle)
};

class M_VIEWS_EXPORT MEditorToolbarArrowStyleContainer : public MWidgetStyleContainer
{
    M_STYLE_CONTAINER(MEditorToolbarArrowStyle)

    //! Mode for arrow pointing up
    M_STYLE_MODE(Up)

    //! Mode for arrow pointing down
    M_STYLE_MODE(Down)
};

#endif // MEDITORTOOLBARARROWSTYLE_H
