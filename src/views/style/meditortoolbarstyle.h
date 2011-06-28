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

    /*! \brief Autohide timeout.
     * The delay (in milliseconds) after which the toolbar will be hidden, if no text is selected.
     */
    M_STYLE_ATTRIBUTE(int, hideTimeout, HideTimeout)

    /*! \brief Hide animation duration, in milliseconds.
     */
    M_STYLE_ATTRIBUTE(int, hideAnimationDuration, HideAnimationDuration)
};

class M_VIEWS_EXPORT MEditorToolbarStyleContainer : public MWidgetStyleContainer
{
    M_STYLE_CONTAINER(MEditorToolbarStyle)

    //! Mode for editor toolbar under cursor, arrow pointing up.
    M_STYLE_MODE(EditorUnderCursor)
};

#endif // MEDITORTOOLBARSTYLE_H
