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

#include "mainwindow.h"

#include <QPushButton>
#include <QColorDialog>
#include <QErrorMessage>
#include <QFileDialog>
#include <QFontDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QPageSetupDialog>
#include <QPrintPreviewDialog>
#include <QProgressDialog>
#include <QWizard>

MainWindow::MainWindow() : QMainWindow(NULL)
{
    setupUi(this);

    connect(normalNmDialogButton, SIGNAL(clicked()), SLOT(showNormalNmDialog()));
    connect(fullScreenNmDialogButton, SIGNAL(clicked()), SLOT(showFullScreenNmDialog()));
    connect(hugeNmDialogButton, SIGNAL(clicked()), SLOT(showHugeNmDialog()));

    connect(normalMDialogButton, SIGNAL(clicked()), SLOT(showNormalMDialog()));
    connect(fullScreenMDialogButton, SIGNAL(clicked()), SLOT(showFullScreenMDialog()));
    connect(HugeMDialogButton, SIGNAL(clicked()), SLOT(showHugeMDialog()));


    connect(colorDialogButton, SIGNAL(clicked()), SLOT(showColorDialog()));
    connect(errorMessageButton, SIGNAL(clicked()), SLOT(showErrorMessage()));
    connect(fileDialogButton, SIGNAL(clicked()), SLOT(showFileDialog()));
    connect(fontDialogButton, SIGNAL(clicked()), SLOT(showFontDialog()));
    connect(inputDialogButton, SIGNAL(clicked()), SLOT(showInputDialog()));
    connect(messageBoxButton, SIGNAL(clicked()), SLOT(showMessageBox()));
    connect(pageSetupDialogButton, SIGNAL(clicked()), SLOT(showPageSetupDialog()));
    connect(printPreviewDialogButton, SIGNAL(clicked()), SLOT(showPrintPreviewDialog()));
    connect(progressDialogButton, SIGNAL(clicked()), SLOT(showProgressDialog()));
    connect(wizardButton, SIGNAL(clicked()), SLOT(showWizard()));
}

MainWindow::~MainWindow()
{

}

void MainWindow::showColorDialog()
{
    QColor testColor = QColorDialog::getColor(Qt::red,
                       this, "Color Dialog",
                       QColorDialog::ShowAlphaChannel);
}

void MainWindow::showErrorMessage()
{
    QErrorMessage dialog(this);
    dialog.showMessage("Errare humanum est, sed in errare perseverare diabolicum.");
    dialog.exec();
}

void MainWindow::showFileDialog()
{
    QString testFileName = QFileDialog::getOpenFileName(this,
                           "Please choose a file name for testing",
                           "/");
}

void MainWindow::showFontDialog()
{
    bool ok = false;
    QFont testFont = QFontDialog::getFont(&ok, QFont("Times", 12),
                                          this,
                                          "Please choose a font for testing.");
}

void MainWindow::showInputDialog()
{
    QString testText = QInputDialog::getText(this,
                       "Please enter a string for testing.",
                       "This is a test");
}

void MainWindow::showMessageBox()
{
    QMessageBox::warning(this,
                         "Don't Panic! This is a test!",
                         "Error 422001: I'm sorry, M. I'm afraid I can't do that!");
}

void MainWindow::showPageSetupDialog()
{
    QPageSetupDialog dialog(this);
    dialog.exec();
}

void MainWindow::showPrintPreviewDialog()
{
    QPrintPreviewDialog dialog(this);
    dialog.exec();
}

void MainWindow::showProgressDialog()
{
    QProgressDialog dialog(this);
    dialog.exec();
}

void MainWindow::showWizard()
{
    QWizard dialog(this);
    dialog.exec();
}

void MainWindow::showNormalNmDialog()
{
    QDialog dialog(this);
    dialog.setModal(false);
    dialog.exec();
}

void MainWindow::showFullScreenNmDialog()
{
    QDialog dialog(this);
    dialog.showFullScreen();
    dialog.setModal(false);
    dialog.exec();
}

void MainWindow::showHugeNmDialog()
{
    QDialog dialog(this);
    dialog.setFixedWidth(2048);
    dialog.setFixedHeight(2048);
    dialog.setModal(false);
    dialog.exec();
}

void MainWindow::showNormalMDialog()
{
    QDialog dialog(this);
    dialog.exec();
}

void MainWindow::showFullScreenMDialog()
{
    QDialog dialog(this);
    dialog.showFullScreen();
    dialog.exec();
}

void MainWindow::showHugeMDialog()
{
    QDialog dialog(this);
    dialog.setFixedWidth(2048);
    dialog.setFixedHeight(2048);
    dialog.exec();
}
