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

#include "textentrypage.h"
#include <DuiLayout>
#include <DuiLocale>
#include <DuiLabel>
#include <DuiTextEdit>
#include <DuiButton>
#include <DuiCompleter>
#include <DuiSceneManager>
#include <QList>
#include <QStringListModel>
#include <QDebug>
#include <layout/duigridlayoutpolicy.h>

namespace
{
    //! the maximum and minimum width for labels
    const int MaxminLabelWidth = 225;
    const int MiniminTextEntryWidth = 225;
}

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



class CustomDirectIMWidget : public DuiLabel
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
    : DuiLabel(parent)
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
    if (sceneManager())
        sceneManager()->requestSoftwareInputPanel(this);
    //% "Press any key"
    this->setText(qtTrId("xx_textentry_press_any_key"));
}

void CustomDirectIMWidget::focusOutEvent(QFocusEvent *event)
{
    Q_UNUSED(event);
    if (sceneManager())
        sceneManager()->closeSoftwareInputPanel();
    //% "Click here"
    this->setText(qtTrId("xx_textentry_click_here"));
}

QVariant CustomDirectIMWidget::inputMethodQuery(Qt::InputMethodQuery query) const
{
    switch ((int) query) {
    case Dui::ImModeQuery:
        return QVariant(Dui::InputMethodModeDirect);
    case Dui::InputEnabledQuery:
        return QVariant(true);
    default:
        return DuiLabel::inputMethodQuery(query);
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

TextEntryPage::TextEntryPage()
    : TemplatePage(),
      m_autoCapitalisation(true),
      m_errorCorrection(true),
      m_completer(new DuiCompleter)
{
    gid = TemplatePage::UserInput;
}


TextEntryPage::~TextEntryPage()
{
    delete m_completer;
}

QString TextEntryPage::timedemoTitle()
{
    return "TextEntry";
}

void TextEntryPage::createContent()
{
    DuiApplicationPage::createContent();
    DuiWidget *panel = centralWidget();
    DuiGridLayoutPolicy *layoutPolicy = TextEntryPage::createAndSetupGridPolicy(panel);
    int row = 0;

    // free text line
    Entries << new DuiTextEdit(DuiTextEditModel::SingleLine, "", centralWidget());

    label0 = new DuiLabel(centralWidget());
    label0->setWordWrap(true);
    label0->setMinimumWidth(MaxminLabelWidth);
    label0->setMaximumWidth(MaxminLabelWidth);

    layoutPolicy->addItem(label0, row, 0);
    layoutPolicy->addItem(Entries.at(row), row, 1);
    row++;

    // password line
    Entries << new DuiTextEdit(DuiTextEditModel::SingleLine, "", centralWidget());

    label1 = new DuiLabel(centralWidget());
    label1->setWordWrap(true);
    label1->setMinimumWidth(MaxminLabelWidth);
    label1->setMaximumWidth(MaxminLabelWidth);

    Entries.at(row)->setEchoMode(DuiTextEditModel::Password);
    layoutPolicy->addItem(label1, row, 0);
    layoutPolicy->addItem(Entries.at(row), row, 1);
    row++;

    // no echo line
    Entries << new DuiTextEdit(DuiTextEditModel::SingleLine, "", centralWidget());

    labelNoEcho = new DuiLabel(centralWidget());
    labelNoEcho->setWordWrap(true);
    labelNoEcho->setMinimumWidth(MaxminLabelWidth);
    labelNoEcho->setMaximumWidth(MaxminLabelWidth);

    Entries.at(row)->setEchoMode(DuiTextEditModel::NoEcho);
    layoutPolicy->addItem(labelNoEcho, row, 0);
    layoutPolicy->addItem(Entries.at(row), row, 1);
    row++;

    // passwordEchoOnEdit
    Entries << new DuiTextEdit(DuiTextEditModel::SingleLine, "", centralWidget());

    labelEchoOnEdit = new DuiLabel(centralWidget());
    labelEchoOnEdit->setWordWrap(true);
    labelEchoOnEdit->setMinimumWidth(MaxminLabelWidth);
    labelEchoOnEdit->setMaximumWidth(MaxminLabelWidth);

    Entries.at(row)->setEchoMode(DuiTextEditModel::PasswordEchoOnEdit);
    layoutPolicy->addItem(labelEchoOnEdit, row, 0);
    layoutPolicy->addItem(Entries.at(row), row, 1);
    row++;

    // number content type
    Entries << new DuiTextEdit(DuiTextEditModel::SingleLine, "", centralWidget());

    label2 = new DuiLabel(centralWidget());
    label2->setWordWrap(true);
    label2->setMinimumWidth(MaxminLabelWidth);
    label2->setMaximumWidth(MaxminLabelWidth);

    Entries.at(row)->setContentType(Dui::NumberContentType);
    layoutPolicy->addItem(label2, row, 0);
    layoutPolicy->addItem(Entries.at(row), row, 1);
    row++;

    // phone number content type
    Entries << new DuiTextEdit(DuiTextEditModel::SingleLine, "", centralWidget());

    label3 = new DuiLabel(centralWidget());
    label3->setWordWrap(true);
    label3->setMinimumWidth(MaxminLabelWidth);
    label3->setMaximumWidth(MaxminLabelWidth);

    Entries.at(row)->setContentType(Dui::PhoneNumberContentType);
    layoutPolicy->addItem(label3, row, 0);
    layoutPolicy->addItem(Entries.at(row), row, 1);
    row++;

    // email content type
    Entries << new DuiTextEdit(DuiTextEditModel::SingleLine, "", centralWidget());

    label4 = new DuiLabel(centralWidget());
    label4->setWordWrap(true);
    label4->setMinimumWidth(MaxminLabelWidth);
    label4->setMaximumWidth(MaxminLabelWidth);

    Entries.at(row)->setContentType(Dui::EmailContentType);
    layoutPolicy->addItem(label4, row, 0);
    layoutPolicy->addItem(Entries.at(row), row, 1);
    row++;

    // url content type
    Entries << new DuiTextEdit(DuiTextEditModel::SingleLine, "", centralWidget());

    label5 = new DuiLabel(centralWidget());
    label5->setWordWrap(true);
    label5->setMinimumWidth(MaxminLabelWidth);
    label5->setMaximumWidth(MaxminLabelWidth);

    Entries.at(row)->setContentType(Dui::UrlContentType);
    layoutPolicy->addItem(label5, row, 0);
    layoutPolicy->addItem(Entries.at(row), row, 1);
    row++;

    // multiline
    Entries << new DuiTextEdit(DuiTextEditModel::MultiLine, "", centralWidget());

    label6 = new DuiLabel(centralWidget());
    label6->setWordWrap(true);
    label6->setMinimumWidth(MaxminLabelWidth);
    label6->setMaximumWidth(MaxminLabelWidth);

    label6->setAlignment(Qt::AlignTop);
    layoutPolicy->addItem(label6, row, 0);
    layoutPolicy->addItem(Entries.at(row), row, 1);
    row++;

    // autoselection line
    Entries << new DuiTextEdit(DuiTextEditModel::SingleLine, "", centralWidget());
    Entries.at(row)->setAutoSelectionEnabled(true);

    label7 = new DuiLabel(centralWidget());
    label7->setWordWrap(true);
    label7->setMinimumWidth(MaxminLabelWidth);
    label7->setMaximumWidth(MaxminLabelWidth);
    label7->setAlignment(Qt::AlignTop);
    layoutPolicy->addItem(label7, row, 0);
    layoutPolicy->addItem(Entries.at(row), row, 1);
    row++;

    // completion
    Entries << new DuiTextEdit(DuiTextEditModel::MultiLine, "", centralWidget());
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
    m_completer->setCandidateSourceModel(model);
    m_completer->setCharactersToTrim(QString("<>"));
    m_completer->setAcceptMultipleEntries(true);
    Entries.at(row)->setCompleter(m_completer);

    label8 = new DuiLabel(centralWidget());
    label8->setWordWrap(true);
    label8->setMinimumWidth(MaxminLabelWidth);
    label8->setMaximumWidth(MaxminLabelWidth);
    label8->setAlignment(Qt::AlignTop);
    layoutPolicy->addItem(label8, row, 0);
    layoutPolicy->addItem(Entries.at(row), row, 1);
    row++;

    //direct input custom widget.
    directIMWidget = new CustomDirectIMWidget(centralWidget());
    labelDirectIM = new DuiLabel(centralWidget());;
    labelDirectIM->setWordWrap(true);
    labelDirectIM->setMinimumWidth(MaxminLabelWidth);
    labelDirectIM->setMaximumWidth(MaxminLabelWidth);
    labelDirectIM->setAlignment(Qt::AlignTop);
    layoutPolicy->addItem(labelDirectIM, row, 0);
    layoutPolicy->addItem(directIMWidget, row, 1);
    row++;

    // Auto capitalisation button (Toggle)
    button1 = new DuiButton(panel);
    button1->setObjectName("switchAutoCaps");

    button1->setCheckable(true);
    button1->setChecked(true);
    layoutPolicy->addItem(button1, row, 0, 1, 2);
    row++;

    // error correction button (Toggle)
    button2 = new DuiButton(panel);
    button2->setObjectName("switchErrorCorrection");

    button2->setCheckable(true);
    button2->setChecked(true);
    layoutPolicy->addItem(button2, row, 0, 1, 2);
    row++;

    connect(button1, SIGNAL(toggled(bool)), this, SLOT(changeAutoCapitalisation(bool)));
    connect(button2, SIGNAL(toggled(bool)), this, SLOT(changeErrorCorrection(bool)));

    //add an empty lable here, then the lower textenties won't be covered by vkb
    labelHeader1 = new DuiLabel(centralWidget());
    labelHeader1->setMinimumHeight(250);
    layoutPolicy->addItem(labelHeader1, row, 0);
    row++;

    layoutPolicy->setColumnMinimumWidth(1, MiniminTextEntryWidth);

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
    //% "Enter text here"
    Entries.at(0)->setPrompt(qtTrId("xx_tooltip_entry0"));

    //% "This tooltip is quite long and will be clipped on the display"
    Entries.at(1)->setPrompt(qtTrId("xx_tooltip_entry1"));

    //% "Provides email address suggestions"
    Entries.at(6)->setPrompt(qtTrId("xx_tooltip_entry8"));

    //% "This is\nmultiline\ntooltip"
    Entries.at(8)->setPrompt(qtTrId("xx_tooltip_entry6"));

    //% "Free text:"
    label0->setText(qtTrId("xx_textentry_free_text"));
    //% "Free text(masked):"
    label1->setText(qtTrId("xx_textentry_free_text_masked"));
    //% "Number text:"
    label2->setText(qtTrId("xx_textentry_number_text"));
    //% "Phone number:"
    label3->setText(qtTrId("xx_textentry_phone_number"));
    //% "Email text:"
    label4->setText(qtTrId("xx_textentry_email_text"));
    //% "Url text:"
    label5->setText(qtTrId("xx_textentry_url_text"));
    //% "Multiline:"
    label6->setText(qtTrId("xx_textentry_multiline"));
    //% "Singleline (selectall):"
    label7->setText(qtTrId("xx_textentry_singleselectall"));
    //% "Email suggestion text:"
    label8->setText(qtTrId("xx_textentry_completion_text"));

    //% "No Echo mode:"
    labelNoEcho->setText(qtTrId("xx_textentry_noecho"));

    //% "Echo on edit:"
    labelEchoOnEdit->setText(qtTrId("xx_textentry_echoonedit"));

    //% "Direct Input Mode:"
    labelDirectIM->setText(qtTrId("xx_textentry_direct_input_mode"));

    //% "Auto capitalisation"
    button1->setText(qtTrId("xx_auto_capitalisation"));
    //% "Error correction"
    button2->setText(qtTrId("xx_error_correction"));

    changeAutoCapitalisation(m_autoCapitalisation);
    changeErrorCorrection(m_errorCorrection);
}

DuiGridLayoutPolicy *TextEntryPage::createAndSetupGridPolicy(DuiWidget *panel)
{
    DuiLayout *layout = new DuiLayout(panel);
    panel->setLayout(layout);
    DuiGridLayoutPolicy *policy = new DuiGridLayoutPolicy(layout);
    policy->setContentsMargins(20, 0, 20, 0);
    return policy;
}


void TextEntryPage::changeAutoCapitalisation(bool val)
{
    m_autoCapitalisation = val;

    DuiTextEdit *entry = NULL;

    foreach(entry, Entries) {
        entry->setInputMethodAutoCapitalizationEnabled(val);
    }

    if (val)
        //% "Auto Capitalisation is active"
        Entries.at(4)->setPrompt(qtTrId("xx_auto_true_entry0"));
    else
        //% "No Auto Capitalisation"
        Entries.at(4)->setPrompt(qtTrId("xx_auto_false_entry0"));
}


void TextEntryPage::changeErrorCorrection(bool val)
{
    m_errorCorrection = val;

    DuiTextEdit *entry = NULL;
    foreach(entry, Entries) {
        if (!entry->completer())
            entry->setInputMethodCorrectionEnabled(val);
    }
}
