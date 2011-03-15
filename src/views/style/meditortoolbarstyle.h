#ifndef MEDITORTOOLBARSTYLE_H
#define MEDITORTOOLBARSTYLE_H

#include <MWidgetStyle>

/*!
    \brief This defines style for editor toolbar which can be either cut/copy/paste
           or rich text editor toolbar.
*/
class M_VIEWS_EXPORT MEditorToolbarStyle : public MWidgetStyle
{
    Q_OBJECT
    M_STYLE(MEditorToolbarStyle)
};

class M_VIEWS_EXPORT MEditorToolbarStyleContainer : public MWidgetStyleContainer
{
    M_STYLE_CONTAINER(MEditorToolbarStyle)

    //! Mode for editor toolbar under cursor, arrow pointing up.
    M_STYLE_MODE(EditorUnderCursor)
};

#endif // MEDITORTOOLBARSTYLE_H
