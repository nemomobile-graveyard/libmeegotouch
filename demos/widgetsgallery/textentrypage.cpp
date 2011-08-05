/***************************************************************************
**
** Copyright (C) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#include "textentrypage.h"
#include "headeredtextedit.h"
#include <MLayout>
#include <MLocale>
#include <MLabel>
#include <MInputMethodState>
#include <MButton>
#include <MCompleter>
#include <MSceneManager>
#include <MRichTextEdit>
#include <MLinearLayoutPolicy>
#include <MWidgetStyle>
#include <MPannableViewport>

#include <QList>
#include <QStringListModel>
#include <QDebug>
#include <QKeyEvent>
#include <QGraphicsSceneMouseEvent>
#include <QTextDocument>
#include <QTextCursor>
#include <QAbstractTextDocumentLayout>
#include <QTextDocumentFragment>
#include <QTextLayout>
#include <QTextBlock>
#include <QGraphicsLinearLayout>

class CompleterTestModel : public QAbstractTableModel
{
public :
    CompleterTestModel(const QStringList &, const QStringList &, QObject *parent = 0);
    ~CompleterTestModel();

    void setData(const QStringList &, const QStringList &);

    //! \reimp
    virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    virtual QModelIndex parent(const QModelIndex &index) const;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    //! \reimp_end

private:
    QStringList column1, column2;
};

CompleterTestModel::CompleterTestModel(const QStringList &list1, const QStringList &list2,
                                       QObject *parent)
    : QAbstractTableModel(parent),
      column1(list1),
      column2(list2)
{
}

CompleterTestModel::~CompleterTestModel()
{
}

void CompleterTestModel::setData(const QStringList &list1, const QStringList &list2)
{
    column1 = list1;
    column2 = list2;
    reset();
}

QModelIndex CompleterTestModel::index(int row, int column, const QModelIndex &parent) const
{
    return hasIndex(row, column, parent) ? createIndex(row, column, 0) : QModelIndex();
}

QModelIndex CompleterTestModel::parent(const QModelIndex & /* index */) const
{
    return QModelIndex();
}

int CompleterTestModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 2;
}

int CompleterTestModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    int maxRow = qMax(column1.count(), column2.count());
    return maxRow;
}

QVariant CompleterTestModel::data(const QModelIndex &index, int role) const
{
    Q_UNUSED(role);
    int row = index.row();
    int column = index.column();
    if (role != Qt::DisplayRole)
        return QVariant();
    QString value;
    if (row < rowCount() && row >= 0 && column < 2 && column >= 0) {
        if (column == 0 && row < column1.count())
            value = column1[row];
        else if (column == 1 && row < column2.count())
            value = column2[row];
    }
    return QVariant(value);
}



class CustomDirectIMWidget : public MLabel
{
public:
    CustomDirectIMWidget(QGraphicsItem *parent = 0);
    ~CustomDirectIMWidget();

    //! \reimp
    virtual void focusInEvent(QFocusEvent *event);
    virtual void focusOutEvent(QFocusEvent *event);
    virtual QVariant inputMethodQuery(Qt::InputMethodQuery query) const;
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void keyReleaseEvent(QKeyEvent *event);
    //! \reimp_end
};


CustomDirectIMWidget::CustomDirectIMWidget(QGraphicsItem *parent)
    : MLabel(parent)
{
    setFocusPolicy(Qt::ClickFocus);
    setFlag(QGraphicsItem::ItemAcceptsInputMethod, true);
    this->setWordWrap(true);
    //% "Click here"
    this->setText(qtTrId("xx_textentry_click_here"));
}

CustomDirectIMWidget::~CustomDirectIMWidget()
{
}

void CustomDirectIMWidget::focusInEvent(QFocusEvent *event)
{
    Q_UNUSED(event);

    MInputMethodState::requestSoftwareInputPanel();
    //% "Press any key"
    this->setText(qtTrId("xx_textentry_press_any_key"));
}

void CustomDirectIMWidget::focusOutEvent(QFocusEvent *event)
{
    Q_UNUSED(event);

    MInputMethodState::closeSoftwareInputPanel();
    //% "Click here"
    this->setText(qtTrId("xx_textentry_click_here"));
}

QVariant CustomDirectIMWidget::inputMethodQuery(Qt::InputMethodQuery query) const
{
    switch ((int) query) {
    case M::ImModeQuery:
        return QVariant(M::InputMethodModeDirect);
    default:
        return MLabel::inputMethodQuery(query);
    }
}

void CustomDirectIMWidget::keyPressEvent(QKeyEvent *event)
{
    QString label;
    //% "<i>Key press</i> key:"
    label = qtTrId("xx_textentry_keypress");
    label += QString("%1, ").arg(event->key(), 0, 16);
    //% "text:"
    label += qtTrId("xx_textentry_text");
    label += event->text();
    this->setText(label);
}

void CustomDirectIMWidget::keyReleaseEvent(QKeyEvent *event)
{
    QString label;
    label = this->text();
    label += '\n';
    //% "<i>Key release</i> key:"
    label += qtTrId("xx_textentry_keyrelease");
    label += QString("%1, ").arg(event->key(), 0, 16);
    //% "text:"
    label += qtTrId("xx_textentry_text");
    label += event->text();
    this->setText(label);
}

CustomTextEdit::CustomTextEdit(const QString &text, QGraphicsItem *parent)
    : MTextEdit(MTextEditModel::MultiLine, text, parent)
{
    QObject::connect(this, SIGNAL(textChanged()), this, SLOT(changeLabel()));
    QObject::connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(changeButton()));
    QObject::connect(this, SIGNAL(gainedFocus(Qt::FocusReason)), this, SLOT(changeLabel()));
    QObject::connect(this, SIGNAL(gainedFocus(Qt::FocusReason)), this, SLOT(changeButton()));
    setInputMethodCorrectionEnabled(false);
}

void CustomTextEdit::changeLabel()
{
    QString textCount = QString("%1").arg(text().length());
    MInputMethodState::instance()->setToolbarItemAttribute(model()->toolbarId(),
                                                           "labelcount",
                                                           "text", QVariant(textCount));
}

void CustomTextEdit::changeButton()
{
    MInputMethodState::instance()->setToolbarItemAttribute(model()->toolbarId(),
                                                           "buttonbold",
                                                           "pressed",
                                                           QVariant(document()->defaultFont().bold()));
    MInputMethodState::instance()->setToolbarItemAttribute(model()->toolbarId(),
                                                           "buttonunderline",
                                                           "pressed",
                                                           QVariant(document()->defaultFont().underline()));
}

bool CustomTextEdit::event(QEvent *event)
{
    if (QEvent::KeyPress == event->type()) {
        QKeyEvent *k = static_cast<QKeyEvent *>(event);
        if (Qt::Key_B == k->key() && (k->modifiers() & Qt::MetaModifier)) {
            QFont font= document()->defaultFont();
            font.setBold(!font.bold());
            document()->setDefaultFont(font);
            update();
            return true;
        } else if (Qt::Key_U == k->key() && (k->modifiers() & Qt::MetaModifier)) {
            QFont font= document()->defaultFont();
            font.setUnderline(!font.underline());
            document()->setDefaultFont(font);
            update();
            return true;
        }
    }
    return MTextEdit::event(event);
}

TextEntryPage::TextEntryPage()
    : TemplatePage(TemplatePage::UserInput), pageTitleLabel(),
      label0(), label1(), label2(), label3(), label4(), label5(),
      label6(), label7(), label8(), label9(), label10(),
      labelNoEcho(), labelEchoOnEdit(), labelDirectIM(),
      labelCustomToolbar1(), labelCustomToolbar2(), uneditableTextEditLabel(),
      labelHeader1(),
       button1(), button2(),
       labels(),
       Entries(),
      m_autoCapitalisation(true),
      m_errorCorrection(true),
      m_completer(new MCompleter)
{
    m_completer->setObjectName("m_completer");
}


TextEntryPage::~TextEntryPage()
{
}

QString TextEntryPage::timedemoTitle()
{
    return "TextEntry";
}

void TextEntryPage::createContent()
{
    MApplicationPage::createContent();
    pannableViewport()->setAcceptGesturesFromAnyDirection(true);
    setStyleName(inv("CommonApplicationPage"));
    QGraphicsWidget *panel = centralWidget();
    MLinearLayoutPolicy *layoutPolicy = TextEntryPage::createAndSetupLinearPolicy(panel);

    pageTitleLabel = new MLabel(centralWidget());
    pageTitleLabel->setObjectName("pageTitleLabel");
    pageTitleLabel->setStyleName(inv("CommonApplicationHeader"));
    layoutPolicy->addItem(pageTitleLabel);

    int row = 0;

    // free text line
    Entries << new MTextEdit(MTextEditModel::SingleLine, "", centralWidget());
    Entries.at(row)->setStyleName(inv("CommonSingleInputFieldLabeled"));
    Entries.at(row)->setObjectName("freeTextTextEdit");
    label0 = new MLabel(centralWidget());
    label0->setObjectName("label0");
    label0->setStyleName(inv("CommonFieldLabel"));
    label0->setWordWrap(true);
    label0->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    layoutPolicy->addItem(label0);
    layoutPolicy->addItem(Entries.at(row));
    row++;

    // password line
    Entries << new MTextEdit(MTextEditModel::SingleLine, "", centralWidget());
    Entries.at(row)->setObjectName("passwordTextEdit");
    Entries.at(row)->setStyleName(inv("CommonSingleInputFieldLabeled"));

    label1 = new MLabel(centralWidget());
    label1->setObjectName("label1");
    label1->setStyleName(inv("CommonFieldLabel"));
    label1->setWordWrap(true);
    label1->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    Entries.at(row)->setEchoMode(MTextEditModel::Password);
    layoutPolicy->addItem(label1);
    layoutPolicy->addItem(Entries.at(row));
    row++;

    // no echo line
    Entries << new MTextEdit(MTextEditModel::SingleLine, "", centralWidget());
    Entries.at(row)->setObjectName("noEchoTextEdit");
    Entries.at(row)->setStyleName(inv("CommonSingleInputFieldLabeled"));

    labelNoEcho = new MLabel(centralWidget());
    labelNoEcho->setObjectName("labelNoEcho");
    labelNoEcho->setStyleName(inv("CommonFieldLabel"));
    labelNoEcho->setWordWrap(true);
    labelNoEcho->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    Entries.at(row)->setEchoMode(MTextEditModel::NoEcho);
    layoutPolicy->addItem(labelNoEcho);
    layoutPolicy->addItem(Entries.at(row));
    row++;

    // passwordEchoOnEdit
    Entries << new MTextEdit(MTextEditModel::SingleLine, "", centralWidget());
    Entries.at(row)->setObjectName("passwordEchoOnEditTextEdit");
    Entries.at(row)->setStyleName(inv("CommonSingleInputFieldLabeled"));

    labelEchoOnEdit = new MLabel(centralWidget());
    labelEchoOnEdit->setObjectName("labelEchoOnEdit");
    labelEchoOnEdit->setStyleName(inv("CommonFieldLabel"));
    labelEchoOnEdit->setWordWrap(true);
    labelEchoOnEdit->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    Entries.at(row)->setEchoMode(MTextEditModel::PasswordEchoOnEdit);
    layoutPolicy->addItem(labelEchoOnEdit);
    layoutPolicy->addItem(Entries.at(row));
    row++;

    // number content type
    Entries << new MTextEdit(MTextEditModel::SingleLine, "", centralWidget());
    Entries.at(row)->setObjectName("numberTextEdit");
    Entries.at(row)->setStyleName(inv("CommonSingleInputFieldLabeled"));

    label2 = new MLabel(centralWidget());
    label2->setObjectName("label2");
    label2->setStyleName(inv("CommonFieldLabel"));
    label2->setWordWrap(true);
    label2->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    Entries.at(row)->setContentType(M::NumberContentType);
    layoutPolicy->addItem(label2);
    layoutPolicy->addItem(Entries.at(row));
    row++;

    // phone number content type
    Entries << new MTextEdit(MTextEditModel::SingleLine, "", centralWidget());
    Entries.at(row)->setObjectName("phoneTextEdit");
    Entries.at(row)->setStyleName(inv("CommonSingleInputFieldLabeled"));

    label3 = new MLabel(centralWidget());
    label3->setObjectName("label3");
    label3->setStyleName(inv("CommonFieldLabel"));
    label3->setWordWrap(true);
    label3->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    Entries.at(row)->setContentType(M::PhoneNumberContentType);
    layoutPolicy->addItem(label3);
    layoutPolicy->addItem(Entries.at(row));
    row++;

    // email content type
    Entries << new MTextEdit(MTextEditModel::SingleLine, "", centralWidget());
    Entries.at(row)->setObjectName("emailTextEdit");
    Entries.at(row)->setStyleName(inv("CommonSingleInputFieldLabeled"));

    label4 = new MLabel(centralWidget());
    label4->setObjectName("label4");
    label4->setStyleName(inv("CommonFieldLabel"));
    label4->setWordWrap(true);
    label4->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    Entries.at(row)->setContentType(M::EmailContentType);
    layoutPolicy->addItem(label4);
    layoutPolicy->addItem(Entries.at(row));
    row++;

    // url content type
    Entries << new MTextEdit(MTextEditModel::SingleLine, "", centralWidget());
    Entries.at(row)->setObjectName("urlTextEdit");
    Entries.at(row)->setStyleName(inv("CommonSingleInputFieldLabeled"));

    label5 = new MLabel(centralWidget());
    label5->setObjectName("label5");
    label5->setStyleName(inv("CommonFieldLabel"));
    label5->setWordWrap(true);
    label5->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    Entries.at(row)->setContentType(M::UrlContentType);
    layoutPolicy->addItem(label5);
    layoutPolicy->addItem(Entries.at(row));
    row++;

    // multiline
    Entries << new MTextEdit(MTextEditModel::MultiLine, "", centralWidget());
    Entries.at(row)->setObjectName("multilineTextEdit");
    Entries.at(row)->setStyleName(inv("CommonSingleInputFieldLabeled"));

    label6 = new MLabel(centralWidget());
    label6->setObjectName("label6");
    label6->setStyleName(inv("CommonFieldLabel"));
    label6->setWordWrap(true);
    label6->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    label6->setAlignment(Qt::AlignTop);
    layoutPolicy->addItem(label6);
    layoutPolicy->addItem(Entries.at(row));
    row++;

    // autoselection line
    Entries << new MTextEdit(MTextEditModel::SingleLine, "", centralWidget());
    Entries.at(row)->setObjectName("autoselectionTextEdit");
    Entries.at(row)->setStyleName(inv("CommonSingleInputFieldLabeled"));
    Entries.at(row)->setAutoSelectionEnabled(true);

    label7 = new MLabel(centralWidget());
    label7->setObjectName("label7");
    label7->setStyleName(inv("CommonFieldLabel"));
    label7->setWordWrap(true);
    label7->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    label7->setAlignment(Qt::AlignTop);
    layoutPolicy->addItem(label7);
    layoutPolicy->addItem(Entries.at(row));
    row++;

    // completion
    Entries << new MTextEdit(MTextEditModel::MultiLine, "", centralWidget());
    Entries.at(row)->setObjectName("completionTextEdit");
    Entries.at(row)->setStyleName(inv("CommonSingleInputFieldLabeled"));
    QStringList list;
    list << "Betty Brey <Betty.C.Brey@example.com>"
         << "Anne Rodriguez <Anne.E.Rodriguez@example.com>"
         << "Elizabeth Hutchings <Elizabeth.T.Hutchings@example.com>"
         << "Robert Hornbuckle <Robert.C.Hornbuckle@example.com>"
         << "Willie Logan <Willie.B.Logan@example.com>"
         << "Marcia Pineda <Marcia.J.Pineda@example.com>"
         << "Jessica Schmitt <Jessica.L.Schmitt@example.com>"
         << "Lynda Wan <Lynda.W.Wan@example.com>"
         << "Alphonso Stevens <Alphonso.I.Stevens@example.com>"
         << "Patricia Murphy <Patricia.K.Murphy@example.com>"
         << "Jeff Terry <Jeff.M.Terry@example.com>"
         << "Richard Orlando <Richard.B.Orlando@example.com>"
         << "Eva Quackenbush <Eva.D.Quackenbush@example.com>"
         << "Tim Parker <Tim.J.Parker@example.com>"
         << "Deborah Sanchez <Deborah.E.Sanchez@example.com>"
         << "Carol Reimer <Carol.J.Reimer@example.com>"
         << "Susan Amaya <Susan.H.Amaya@example.com>"
         << "Shelia Rudd <Shelia.E.Rudd@example.com>"
         << "Elizabeth Livingston <Elizabeth.B.Livingston@example.com>"
         << "Renee Heard <Renee.N.Heard@example.com>"
         << "Megan Gagne <Megan.R.Gagne@example.com>"
         << "Katherine Enos <Katherine.B.Enos@example.com>"
         << "Gary Hawkins <Gary.J.Hawkins@example.com>"
         << "Bianca Sparks <Bianca.M.Sparks@example.com>"
         << "Cleta Richardson <Cleta.E.Richardson@example.com>"
         << "Latoya Lawrence <Latoya.E.Lawrence@example.com>"
         << "Ethel Allen <Ethel.S.Allen@example.com>"
         << "Maria Austin <Maria.D.Austin@example.com>"
         << "John Smith <john.smith@example.com>"
         << "Jane Smith <jane.smith@example.com>"
         << "Doug Barker <Doug.J.Barker@example.com>"
         << "Sandra Cross <Sandra.J.Cross@example.com>"
         << "Debra Roberts <Debra.D.Roberts@example.com>"
         << "Mei Copeland <Mei.D.Copeland@example.com>"
         << "Raymond Slack <Raymond.A.Slack@example.com>"
         << "Martin Vidal <Martin.M.Vidal@example.com>"
         << "Patricia Rymer <Patricia.R.Rymer@example.com>"
         << "Maria Gilbreath <Maria.G.Gilbreath@example.com>"
         << "Mary Ramos <Mary.C.Ramos@example.com>"
         << "Michael Haller <Michael.K.Haller@example.com>"
         << "Randall Hood <Randall.J.Hood@example.com>"
         << "Bruce Lindsey <Bruce.D.Lindsey@example.com>"
         << "Heidi Martin <Heidi.S.Martin@example.com>"
         << "Helen Kennedy <Helen.E.Kennedy@example.com>"
         << "Margaret Worsham <Margaret.R.Worsham@example.com>"
         << "Sun Singleton <Sun.R.Singleton@example.com>"
         << "Alberto Prince <Alberto.A.Prince@example.com>"
         << "Norman Weiland <Norman.B.Weiland@example.com>"
         << "Carolyn Delia <Carolyn.G.Delia@example.com>"
         << "Lakesha Acosta <Lakesha.G.Acosta@example.com>";

    QStringList cs, c1, c2;

    foreach(const QString & s, list) {
        cs = s.split('<');
        if (cs.count() >= 2) {
            c1 << cs[0].trimmed();
            c2 << QString('<' + cs[1]);
        }
    }

    CompleterTestModel *model = new CompleterTestModel(c1, c2, this);
    model->setObjectName("completerTestModel");
    m_completer->setCandidateSourceModel(model);
    m_completer->setCharactersToTrim(QString("<>"));
    m_completer->setCharactersToTrimForCompletionPrefix(QString(" "));
    m_completer->setAcceptMultipleEntries(true);
    Entries.at(row)->setCompleter(m_completer.data());

    label8 = new MLabel(centralWidget());
    label8->setObjectName("label8");
    label8->setStyleName(inv("CommonFieldLabel"));
    label8->setWordWrap(true);
    label8->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    label8->setAlignment(Qt::AlignTop);
    layoutPolicy->addItem(label8);
    layoutPolicy->addItem(Entries.at(row));
    row++;

    // rich text entry
    Entries << new MRichTextEdit(MTextEditModel::SingleLine, "", centralWidget());
    Entries.at(row)->setObjectName("richTextEdit");
    Entries.at(row)->setStyleName(inv("CommonSingleInputFieldLabeled"));

    label9 = new MLabel(centralWidget());
    label9->setObjectName("label9");
    label9->setStyleName(inv("CommonFieldLabel"));
    label9->setWordWrap(true);
    label9->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    label9->setAlignment(Qt::AlignTop);
    layoutPolicy->addItem(label9);
    layoutPolicy->addItem(Entries.at(row));
    row++;

    // multiline rich text entry
    Entries << new MRichTextEdit(MTextEditModel::MultiLine, "", centralWidget());
    Entries.at(row)->setObjectName("multilineRichTextEdit");
    Entries.at(row)->setStyleName(inv("CommonSingleInputFieldLabeled"));

    label10 = new MLabel(centralWidget());
    label10->setObjectName("label10");
    label10->setStyleName(inv("CommonFieldLabel"));
    label10->setWordWrap(true);
    label10->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    label10->setAlignment(Qt::AlignTop);
    layoutPolicy->addItem(label10);
    layoutPolicy->addItem(Entries.at(row));
    row++;

    CustomTextEdit *customTextEdit1 = new CustomTextEdit("", centralWidget());
    customTextEdit1->setObjectName("customTextEdit1");
    customTextEdit1->setStyleName(inv("CommonSingleInputFieldLabeled"));
    customTextEdit1->attachToolbar("/usr/share/widgetsgallery/imtoolbar/exampletoolbar1.xml");
    //% "Example custom toolbar 1"
    customTextEdit1->setPrompt(qtTrId("xx_tooltip_customtoolbar1"));
    labelCustomToolbar1 = new MLabel(centralWidget());
    labelCustomToolbar1->setObjectName("labelCustomToolbar1");
    labelCustomToolbar1->setStyleName(inv("CommonFieldLabel"));
    labelCustomToolbar1->setWordWrap(true);
    labelCustomToolbar1->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    labelCustomToolbar1->setAlignment(Qt::AlignTop);
    layoutPolicy->addItem(labelCustomToolbar1);
    layoutPolicy->addItem(customTextEdit1);
    Entries << customTextEdit1;
    row++;

    CustomTextEdit *customTextEdit2 = new CustomTextEdit("", centralWidget());
    customTextEdit2->setObjectName("customTextEdit2");
    customTextEdit2->setStyleName(inv("CommonSingleInputFieldLabeled"));
    customTextEdit2->attachToolbar("/usr/share/widgetsgallery/imtoolbar/exampletoolbar2.xml");
    //% "Example custom toolbar 2"
    customTextEdit2->setPrompt(qtTrId("xx_tooltip_customtoolbar2"));
    labelCustomToolbar2 = new MLabel(centralWidget());
    labelCustomToolbar2->setObjectName("labelCustomToolbar2");
    labelCustomToolbar2->setStyleName(inv("CommonFieldLabel"));
    labelCustomToolbar2->setWordWrap(true);
    labelCustomToolbar2->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    labelCustomToolbar2->setAlignment(Qt::AlignTop);
    layoutPolicy->addItem(labelCustomToolbar2);
    layoutPolicy->addItem(customTextEdit2);
    Entries << customTextEdit2;
    row++;

    // Email field showing error highlighting
    // Trivial demo showing how error highlighting can be used in
    // applications
    errorHighlightingTextEdit = new MTextEdit(MTextEditModel::SingleLine,
                                                     "", centralWidget());
    errorHighlightingTextEdit->setObjectName("errorHighlightingTextEdit");
    errorHighlightingTextEdit->setStyleName(inv("CommonSingleInputFieldLabeled"));
    errorHighlightingTextEdit->setContentType(M::EmailContentType);
    //% "Valid input: 'user@domain.toplevel'"
    errorHighlightingTextEdit->setPrompt(qtTrId("xx_error_highlighting"));
    connect(errorHighlightingTextEdit, SIGNAL(lostFocus(Qt::FocusReason)),
            this, SLOT(checkEmailValidity()));

    labelErrorHighlighting = new MLabel(centralWidget());
    labelErrorHighlighting->setObjectName("labelErrorHighlighting");
    labelErrorHighlighting->setStyleName(inv("CommonFieldLabel"));
    labelErrorHighlighting->setWordWrap(true);
    labelErrorHighlighting->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    layoutPolicy->addItem(labelErrorHighlighting);
    layoutPolicy->addItem(errorHighlightingTextEdit);
    Entries << errorHighlightingTextEdit;
    row++;

    //direct input custom widget.
    QGraphicsWidget *directIMPanel = new QGraphicsWidget(centralWidget());
    directIMPanel->setObjectName("directIMPanel");
    QGraphicsLinearLayout *directIMLayout = new QGraphicsLinearLayout(Qt::Horizontal, directIMPanel);
    directIMLayout->setContentsMargins(0, 0, 0, 0);
    directIMLayout->setSpacing(0);
    directIMWidget = new CustomDirectIMWidget(directIMPanel);
    directIMWidget->setObjectName("directIMWidget");
    directIMWidget->setStyleName(inv("CommonEditorInputFieldValue"));
    directIMWidget->setObjectName("directIMWidget");
    labelDirectIM = new MLabel(directIMPanel);
    labelDirectIM->setStyleName(inv("CommonEditorInputFieldLabel"));
    labelDirectIM->setTextElide(true);
    labelDirectIM->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    labelDirectIM->setAlignment(Qt::AlignTop);
    directIMLayout->addItem(labelDirectIM);
    directIMLayout->addItem(directIMWidget);

    layoutPolicy->addItem(directIMPanel);
    row++;

    HeaderedTextEdit *uneditableTextEdit = new HeaderedTextEdit(centralWidget());
    uneditableTextEdit->setObjectName("uneditableTextEdit");
    uneditableTextEdit->setStyleName(inv("CommonEditorInputFieldMultiline"));
    uneditableTextEdit->setHeaderText("Recipient:");
    uneditableTextEdit->setContentType(M::FreeTextContentType);
    uneditableTextEdit->setCompleter(m_completer.data());
    uneditableTextEditLabel = new MLabel(centralWidget());
    uneditableTextEditLabel->setObjectName("uneditableTextEditLabel");
    uneditableTextEditLabel->setStyleName(inv("CommonFieldLabel"));
    uneditableTextEditLabel->setTextElide(true);
    layoutPolicy->addItem(uneditableTextEditLabel);
    layoutPolicy->addItem(uneditableTextEdit);
    Entries << uneditableTextEdit;
    row++;

    // read only field
    Entries << new MTextEdit(MTextEditModel::SingleLine, "", centralWidget());
    Entries.last()->setStyleName(inv("CommonSingleInputFieldLabeled"));
    Entries.last()->setReadOnly(true);
    readOnlyEntryIndex = Entries.size() - 1;

    labelReadOnly = new MLabel(centralWidget());
    labelReadOnly->setStyleName(inv("CommonFieldLabel"));
    labelReadOnly->setWordWrap(true);
    labelReadOnly->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    labelReadOnly->setAlignment(Qt::AlignTop);
    layoutPolicy->addItem(labelReadOnly);
    layoutPolicy->addItem(Entries.last());
    row++;

    // Auto capitalisation button (Toggle)
    button1 = new MButton(panel);
    button1->setObjectName("button1");
    button1->setStyleName(inv("CommonSingleButton"));
    button1->setObjectName("switchAutoCaps");

    button1->setCheckable(true);
    button1->setChecked(true);
    layoutPolicy->addItem(button1, Qt::AlignCenter);
    row++;

    // error correction button (Toggle)
    button2 = new MButton(panel);
    button2->setObjectName("button2");
    button2->setStyleName(inv("CommonSingleButton"));
    button2->setObjectName("switchErrorCorrection");

    button2->setCheckable(true);
    button2->setChecked(true);
    layoutPolicy->addItem(button2, Qt::AlignCenter);
    row++;

    connect(button1, SIGNAL(toggled(bool)), this, SLOT(changeAutoCapitalisation(bool)));
    connect(button2, SIGNAL(toggled(bool)), this, SLOT(changeErrorCorrection(bool)));

    // switch on Auto Capitalistaion and error correction
    changeAutoCapitalisation(true);
    changeErrorCorrection(true);

    retranslateUi();
}

void TextEntryPage::retranslateUi()
{
    //% "Text Entries"
    setTitle(qtTrId("xx_textentry_title"));
    if (!isContentCreated())
        return;

    //% "Text Entries"
    pageTitleLabel->setText(qtTrId("xx_textentry_title"));
    //% "Enter text here"
    Entries.at(0)->setPrompt(qtTrId("xx_tooltip_entry0"));

    //% "This tooltip is quite long and will be clipped on the display"
    Entries.at(1)->setPrompt(qtTrId("xx_tooltip_entry1"));

    //% "Provides email address suggestions"
    Entries.at(10)->setPrompt(qtTrId("xx_tooltip_entry8"));

    //% "This is a multiline text entry"
    Entries.at(8)->setPrompt(qtTrId("xx_tooltip_entry6"));

    //% "Free text:"
    label0->setText(qtTrId("xx_textentry_free_text"));
    //% "Free text (masked):"
    label1->setText(qtTrId("xx_textentry_free_text_masked"));
    //% "Number text:"
    label2->setText(qtTrId("xx_textentry_number_text"));
    //% "Phone number:"
    label3->setText(qtTrId("xx_textentry_phone_number"));
    //% "Email text:"
    label4->setText(qtTrId("xx_textentry_email_text"));
    //% "URL text:"
    label5->setText(qtTrId("xx_textentry_url_text"));
    //% "Multiline:"
    label6->setText(qtTrId("xx_textentry_multiline"));
    //% "Singleline (selectall):"
    label7->setText(qtTrId("xx_textentry_singleselectall"));
    //% "Email suggestion text:"
    label8->setText(qtTrId("xx_textentry_completion_text"));
    //% "Email suggestions"
    m_completer->setCompletionTitle(qtTrId("xx_completion_title"));
    //% "Rich text:"
    label9->setText(qtTrId("xx_textentry_richtext_text"));
    //% "Rich text (multiline):"
    label10->setText(qtTrId("xx_textentry_richtext_text_multiline"));

    //% "No echo mode:"
    labelNoEcho->setText(qtTrId("xx_textentry_noecho"));

    //% "Echo on edit:"
    labelEchoOnEdit->setText(qtTrId("xx_textentry_echoonedit"));

    //% "Direct input mode:"
    labelDirectIM->setText(qtTrId("xx_textentry_direct_input_mode"));

    //% "Custom toolbar 1:"
    labelCustomToolbar1->setText(qtTrId("xx_textentry_custom_toolbar1"));
    //% "Custom toolbar 2:"
    labelCustomToolbar2->setText(qtTrId("xx_textentry_custom_toolbar2"));

    //% "Error highlighting:"
    labelErrorHighlighting->setText(qtTrId("xx_error_highlighting_label"));
    //% "Uneditable Text Edit:"
    uneditableTextEditLabel->setText(qtTrId("xx_textentry_uneditable_textedit"));

    //% "Read only field"
    labelReadOnly->setText(qtTrId("xx_textentry_readonly_label"));

    //% "This is a read only text field"
    Entries.at(readOnlyEntryIndex)->setText(qtTrId("xx_textentry_readonly"));

    //% "Auto capitalisation"
    button1->setText(qtTrId("xx_auto_capitalisation"));
    //% "Error correction"
    button2->setText(qtTrId("xx_error_correction"));

    changeAutoCapitalisation(m_autoCapitalisation);
    changeErrorCorrection(m_errorCorrection);
}

MLinearLayoutPolicy *TextEntryPage::createAndSetupLinearPolicy(QGraphicsWidget *panel)
{
    MLayout *layout = new MLayout(panel);
    layout->setContentsMargins(0, 0, 0, 0);
    panel->setLayout(layout);
    MLinearLayoutPolicy *policy = new MLinearLayoutPolicy(layout, Qt::Vertical);
    policy->setContentsMargins(0, 0, 0, 0);
    policy->setSpacing(0);
    return policy;
}


void TextEntryPage::changeAutoCapitalisation(bool val)
{
    m_autoCapitalisation = val;

    MTextEdit *entry = NULL;

    foreach(entry, Entries) {
        entry->setInputMethodAutoCapitalizationEnabled(val);
    }

    if (val)
        //% "Auto capitalisation is active"
        Entries.at(9)->setPrompt(qtTrId("xx_auto_true_entry0"));
    else
        //% "No auto capitalisation"
        Entries.at(9)->setPrompt(qtTrId("xx_auto_false_entry0"));
}


void TextEntryPage::changeErrorCorrection(bool val)
{
    m_errorCorrection = val;

    MTextEdit *entry = NULL;
    foreach(entry, Entries) {
        if (!entry->completer())
            entry->setInputMethodCorrectionEnabled(val);
    }
}

void TextEntryPage::checkEmailValidity()
{
    // For the purpose of this demo,
    // a valid email address has the form "username@domain.toplevel"
    QString text = errorHighlightingTextEdit->text();
    QRegExp regExp("[^@]+@[^@]+\\.[^@]+");
    bool validEmail = regExp.exactMatch(text);

    errorHighlightingTextEdit->setErrorHighlight(!validEmail);
}
