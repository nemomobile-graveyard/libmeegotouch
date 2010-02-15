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

#include <QCoreApplication>
#include <QString>
#include <QStringList>
#include <QProcess>
#include <QCoreApplication>
#include <QHash>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QRegExp>

namespace
{
    const QString command("qdbusxml2cpp");
    QString upperCamelServiceName;
    QString lowerServiceName;
    QString xmlFilename;
    QString newXmlFilename;
    QString chainTag;
    QString asyncTag;
    QString upperCamelProxyName;
    QString proxyCppFilename;
    QString proxyHeaderFilename;
    QString newProxyHeaderFilename;
    QString wrapperHeaderFilename;
    QString wrapperCppFilename;
    QString wrapperHeaderGuard;
    QString upperCamelAdaptorName;
    QString adaptorCppFilename;
    QString newAdaptorCppFilename;
    QString newAdaptorHeaderFilename;
    QString adaptorHeaderFilename;
    QString allSignals;
    QString allConnectSignals;
    QHash<int, QStringList> doc;
    QString docTag;
    QString cwd(".");
    QString pid;
    bool needsDuiApplication;
    QString hideThisWindowCode("\n\
        // hide this window\n\
        {\n\
            // Tell the window to not to be shown in the switcher\n\
            Atom skipTaskbarAtom = XInternAtom(QX11Info::display(), \"_NET_WM_STATE_SKIP_TASKBAR\", False);\n\
\n\
            Atom netWmStateAtom = XInternAtom(QX11Info::display(), \"_NET_WM_STATE\", False);\n\
            QVector<Atom> atoms;\n\
            atoms.append(skipTaskbarAtom);\n\
            XChangeProperty(QX11Info::display(), windowId, netWmStateAtom, XA_ATOM, 32, PropModeReplace, (unsigned char *)atoms.data(), atoms.count());\n\
            XSync(QX11Info::display(), False);\n\
        }\n");
};

void removeNewXmlFile()
{
    QFile newXmlFile(newXmlFilename);
    newXmlFile.remove();
}

void chomp(QString in)
{
    in.remove(QRegExp("\n$"));
}

QString botBitH()
{
    return QString("\
\n\
public:\n\
    /*!\n\
     * @brief Constructs a base interface\n\
     * @param preferredService, define the preferred service provider. Leave\n\
       empty if no preferred provider. In most cases, this should be left\n\
       empty.\n\
     * @param parent Parent object\n\
     */\n\
\n\
    " + upperCamelServiceName + "( const QString& preferredService = \"\", QObject* parent = 0 );\n\
\n\
    /*!\n\
     * @brief Set the service name\n\
     * @param service Name of the desired service\n\
     */\n\
\n\
    void setService(const QString & service);\n\
Q_SIGNALS:\n\
" + allSignals + "\n\
\n\
};\n\
#endif");
               }

                   QString botBitC()
               {
                   return QString("\
                   " + upperCamelServiceName + "::" + upperCamelServiceName + "( const QString& preferredService, QObject* parent )\n\
               : DuiServiceFwBaseIf( " + upperCamelProxyName + "::staticInterfaceName(), parent )\n\
                   {\n\
                    // Resolve the provider service name\n\
                    service = resolveServiceName( interface, preferredService );\n\
                    \n\
                    bool serviceNameInvalid = service.contains( \" \" ); // \"not provided\" - when the service wouldn't run\n\
    if ( serviceNameInvalid ) {\n\
        service.clear();\n\
    }\n\
\n\
    if (!service.isEmpty()) {\n\
        // Construct the D-Bus proxy\n\
        interfaceProxy = new " + upperCamelProxyName + "( service, \"/\", QDBusConnection::sessionBus(), this );\n\
        // allConnectSignals go here (empty block if none)\n\
" + allConnectSignals + "\n\
    }\n\
}\n\
\n\
void " + upperCamelServiceName + "::setService(const QString & service)\n\
{\n\
    if (service.isEmpty()) return;\n\
\n\
    this->service = service;\n\
\n\
    if ( interfaceProxy ) {\n\
        delete interfaceProxy;\n\
        interfaceProxy = 0;\n\
    }\n\
    interfaceProxy = new " + upperCamelProxyName + "( service, \"/\", QDBusConnection::sessionBus(), this );\n\
    {\n\
" + allConnectSignals + "\n\
    }\n\
}");
                   }

                   QString middleBitH()
    {
        return QString("\
class " + upperCamelServiceName + " : public DuiServiceFwBaseIf\n\
{\n\
Q_OBJECT\n\
\n\
public:");
    }

    QString topBitH()
    {
        QString commandLine(QCoreApplication::arguments().join(" "));

        return QString("\
#ifndef " + wrapperHeaderGuard + "\n\
               #define " + wrapperHeaderGuard + "\n\
\n\
/*\n\
 * automatically generated with the command line :\n\
 * " + commandLine + "\n\
 */\n\
\n\
#include <QObject>\n\
\n\
#include <duiservicefwbaseif.h>\n\
#include <" + proxyHeaderFilename + ">\n");
                   }

                       QString topBitC()
                       {
                       QString commandLine(QCoreApplication::arguments().join(" "));

                       return QString("\
                       /*\n\
                                                                      * automatically generated with the command line :\n\
                                                                      * " + commandLine + "\n\
                                                                      */\n\
                       \n\
#include \"" + wrapperHeaderFilename + "\"\n");
                   }

                   QStringList getParamNames(QString parameters)
    {
        QStringList retVal;

        QStringList typesAndNames = parameters.split(",");

        for (int typeAndNameIndex = 0; typeAndNameIndex < typesAndNames.size(); ++typeAndNameIndex) {
            QString typeAndName(typesAndNames[ typeAndNameIndex ]);
            QStringList bits = typeAndName.split(QRegExp("\\W+"));
            QString name = bits.last();

            retVal.append(name);
        }

        return retVal;
    }

    void processAdaptorCppFile()
    {
        QStringList ifSignals;

        QFile adaptorCppFile(adaptorCppFilename);
        if (!adaptorCppFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qCritical("Could not open %s", qPrintable(adaptorCppFilename));
            exit(-1);
        }
        QTextStream adaptorCppFileStream(&adaptorCppFile);

        QFile newAdaptorCppFile(newAdaptorCppFilename);
        if (!newAdaptorCppFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            qCritical("Could not open %s", qPrintable(adaptorCppFilename));
            exit(-1);
        }
        QTextStream newAdaptorCppStream(&newAdaptorCppFile);

        bool inChainTask = false;
        bool needGoBack = false;

        while (!adaptorCppFileStream.atEnd()) {
            QString line = adaptorCppFileStream.readLine();
            chomp(line);

            // remove doctag - can be more than one per line
            line.remove(QRegExp(docTag + "\\d+"));

            // add chaining code to NEWADAPTOR
            {
                // always remove asyncTag, since we don't care about it in the cpp file
                line.remove(asyncTag);

                if (needsDuiApplication && line.contains(QRegExp("#include <QtCore/Q(Meta)?Object>"))) {
                    newAdaptorCppStream << endl;
                    newAdaptorCppStream << "#include <DuiApplication>" << endl;
                    newAdaptorCppStream << "#include <DuiApplicationPage>" << endl;
                    newAdaptorCppStream << "#include <DuiApplicationWindow>" << endl;
                    newAdaptorCppStream << "#include <DuiApplicationIfProxy>" << endl;
                    newAdaptorCppStream << "#include <DuiEscapeButtonPanel>" << endl;
                    newAdaptorCppStream << "#include <QX11Info>" << endl;
                    newAdaptorCppStream << "#include <X11/Xutil.h>" << endl;
                    newAdaptorCppStream << "#include <X11/Xlib.h>" << endl;
                    newAdaptorCppStream << "#include <X11/Xatom.h>" << endl;
                    newAdaptorCppStream << "#include <X11/Xmd.h>" << endl;
                    newAdaptorCppStream << endl;
                    newAdaptorCppStream << line << endl;
                } else if (needsDuiApplication && line.contains("QDBusAbstractAdaptor(parent)")) {
                    newAdaptorCppStream << line + "," << endl;
                    newAdaptorCppStream << "    backServiceName()," << endl;
                    newAdaptorCppStream << "    windowId(-1)" << endl;
                } else if (line.contains(newXmlFilename)) {
                    line.replace(newXmlFilename, xmlFilename);
                    newAdaptorCppStream << line << endl;
                } else if (inChainTask) {
                    line.remove(chainTag);
                    if (line == "{") {
                        newAdaptorCppStream << line << endl;
                        newAdaptorCppStream << "    this->windowId = windowId;" << endl;
                        newAdaptorCppStream << "    this->backServiceName = backServiceName;" << endl;
                        newAdaptorCppStream << endl;
                    } else if (line.contains("return") || line == "}") {     // match end of function - need to add the connect *before* the return, if there is one
                        newAdaptorCppStream << endl;
                        newAdaptorCppStream << "    DuiApplication::instance()->activeWindow()->setWindowTitle( windowTitle );" << endl;
                        newAdaptorCppStream << "    DuiApplicationWindow *appWindow = DuiApplication::activeApplicationWindow();" << endl;
                        newAdaptorCppStream << "    if (appWindow != 0) {" << endl;
                        newAdaptorCppStream << "        appWindow->currentPage()->setEscapeButtonMode( DuiEscapeButtonPanelModel::BackMode );" << endl;
                        newAdaptorCppStream << "        // connect to the back button - assumes the above 'showImage' opens a" << endl;
                        newAdaptorCppStream << "        // new window and so the window referred to below is already the top one" << endl;
                        newAdaptorCppStream << "        connect(appWindow->currentPage(), SIGNAL(backButtonClicked())," << endl;
                        newAdaptorCppStream << "                this, SLOT(goBack()));" << endl;
                        newAdaptorCppStream << "    }" << endl;
                        newAdaptorCppStream << endl;
                        newAdaptorCppStream << "    // update the X server" << endl;
                        newAdaptorCppStream << "    {" << endl;
                        newAdaptorCppStream << "        XPropertyEvent p;" << endl;
                        newAdaptorCppStream << "        p.send_event = True;" << endl;
                        newAdaptorCppStream << "        p.display = QX11Info::display();" << endl;
                        newAdaptorCppStream << "        p.type   = PropertyNotify;" << endl;
                        newAdaptorCppStream << "        p.window = RootWindow(p.display, 0);" << endl;
                        newAdaptorCppStream << "        p.atom   = XInternAtom(p.display, \"_NET_CLIENT_LIST\", False);" << endl;
                        newAdaptorCppStream << "        p.state  = PropertyNewValue;" << endl;
                        newAdaptorCppStream << "        p.time   = CurrentTime;" << endl;
                        newAdaptorCppStream << "        XSendEvent(p.display, p.window, False, PropertyChangeMask," << endl;
                        newAdaptorCppStream << "                (XEvent*)&p);" << endl;
                        newAdaptorCppStream << endl;
                        newAdaptorCppStream << "        XSync(QX11Info::display(), False);" << endl;
                        newAdaptorCppStream << "    }" << endl;
                        newAdaptorCppStream << endl;
                        newAdaptorCppStream << line << endl;
                        inChainTask = false;
                    } else {
                        newAdaptorCppStream << line << endl;
                    }
                } else if (line.contains(chainTag)) {
                    line.remove(chainTag);
                    // remove CHAINTASK
                    line.replace("(", "(const QString &backServiceName, const QString &windowTitle, const uint windowId, ");
                    newAdaptorCppStream << line << endl;
                    inChainTask = true;
                    needGoBack  = true;
                } else {
                    newAdaptorCppStream << line << endl;
                }
            }
        }

        if (needGoBack) {
            newAdaptorCppStream << "\
void " + upperCamelAdaptorName + "::goBack()\n\
{\n\
    qDebug() << __PRETTY_FUNCTION__;\n\
\n\
    bool backServiceRegistered = ( windowId != -1 );\n\
    if ( backServiceRegistered ) {\n\
        DuiApplicationIfProxy duiApplicationIfProxy(backServiceName, this);\n\
\n\
        if (duiApplicationIfProxy.connection().isConnected()) {\n\
            qDebug() << \"Launching \" << backServiceName;\n\
            duiApplicationIfProxy.launch();\n\
        } else {\n\
            qWarning() << \"Could not launch\" << backServiceName;\n\
            qWarning() << \"DBus not connected?\";\n\
        }\n\
\n\
        // unhide the chain parent's window\n\
        {\n\
            // Tell the window to not to be shown in the switcher\n\
            XDeleteProperty(QX11Info::display(), windowId, XInternAtom(QX11Info::display(), \"_NET_WM_STATE\", False));\n\
            XSync(QX11Info::display(), False);\n\
        }\n\
\n\
        qWarning() << \"quitting - bye bye\";\n\
        QTimer::singleShot( 0, QApplication::instance(), SLOT( quit() ) );\n\
    } else {\n\
        qWarning() << \"backService is not registered: not going back\";\n\
    }\n\
}" << endl;
        }

        newAdaptorCppFile.close();
        adaptorCppFile.close();

        // mv new adaptor header file (with chain parameters added)
        // to replace one produced by qdbusxml2cpp
        adaptorCppFile.remove();
        newAdaptorCppFile.rename(adaptorCppFilename);

        removeNewXmlFile();
    }

    void processAdaptorHeaderFile()
    {
        QStringList ifSignals;

        QFile adaptorHeaderFile(adaptorHeaderFilename);
        if (!adaptorHeaderFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qCritical("Could not open %s", qPrintable(adaptorHeaderFilename));
            exit(-1);
        }
        QTextStream adaptorHeaderFileStream(&adaptorHeaderFile);

        QFile newAdaptorHeaderFile(newAdaptorHeaderFilename);
        if (!newAdaptorHeaderFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            qCritical("Could not open %s", qPrintable(adaptorHeaderFilename));
            exit(-1);
        }
        QTextStream newAdaptorHeaderStream(&newAdaptorHeaderFile);

        bool hasChains = false;

        while (!adaptorHeaderFileStream.atEnd()) {
            QString line = adaptorHeaderFileStream.readLine();
            chomp(line);

            // remove doctag - can be more than one per line
            line.remove(QRegExp(docTag + "\\d+"));

            // add chaining code to NEWADAPTOR
            {
                if (line.contains("Q_SIGNALS:")) {
                    if (hasChains) {
                        newAdaptorHeaderStream << "    void goBack();" << endl;
                        newAdaptorHeaderStream << "" << endl;
                        newAdaptorHeaderStream << "private:" << endl;
                        newAdaptorHeaderStream << "    QString backServiceName;" << endl;
                        newAdaptorHeaderStream << "    int windowId;" << endl;
                        newAdaptorHeaderStream << "" << endl;
                    }
                    newAdaptorHeaderStream << line << endl;
                } else if (line.contains(QRegExp("chainTask=\\\\\"\\w+\\\\\"")) || line.contains(QRegExp("asyncTask=\\\\\"\\w+\\\\\""))) {           // process comment - remember the backslashes are in the source too, hence so many of them
                    // remove asyncTask attribute
                    {
                        QRegExp matchThis("asyncTask=\\\\\"\\w+\\\\\"\\s*");
                        if (line.contains(matchThis)) {
                            line.remove(matchThis);
                            line.remove(asyncTag);
                        }
                    }

                    // remove asyncTask attribute
                    bool isChainTask = false;
                    {
                        QRegExp matchThis("chainTask=\\\\\"(\\w+)\\\\\"\\s*");
                        if (line.contains(matchThis)) {
                            line.remove(matchThis);
                            isChainTask = matchThis.cap(1).contains("true");
                            line.remove(chainTag);
                        }
                    }

                    // remove extraneous spaces (inserted randomly by qdbusxml2cpp?)
                    line.replace(QRegExp("\\s+(/*>)"), "\\1");

                    newAdaptorHeaderStream << line << endl;

                    // this has to be printed after the method tag, above
                    if (isChainTask) {
                        newAdaptorHeaderStream << "\"      <arg direction=\\\"in\\\" type=\\\"s\\\" name=\\\"backServiceName\\\"/>\\n\"" << endl;
                        newAdaptorHeaderStream << "\"      <arg direction=\\\"in\\\" type=\\\"s\\\" name=\\\"windowTitle\\\"/>\\n\"" << endl;
                        newAdaptorHeaderStream << "\"      <arg direction=\\\"in\\\" type=\\\"u\\\" name=\\\"windowId\\\"/>\\n\"" << endl;
                    }
                } else if (line.contains(chainTag + "(") || line.contains(asyncTag + "(")) {   // add parameters to the chain methods
                    if (line.contains(asyncTag)) {
                        line.remove(asyncTag);
                        QRegExp matchThis("(\\w+)(\\s+)(\\w+)\\(");
                        line.replace(matchThis, "Q_NOREPLY \\1\\2\\3(");   // put Q_NOREPLY before the first word on the line (which should be 'void', but perhaps not)

                        // make warning if type is not void
                        line.contains(matchThis);
                        QString type = matchThis.cap(1);
                        QString method = matchThis.cap(3);
                        if (type != "void") {
                            QString prototype(line);

                            // clean up prototype and method name for warning message
                            prototype.remove(chainTag);
                            prototype.remove(QRegExp("^\\s+"));
                            method.remove(chainTag);
                            qDebug() <<  "WARNING: " + prototype;
                            qDebug() <<  "       : asyncTask=\"true\" specified for " + method + "(), but type is " + type;
                            qDebug() <<  "       : return type should be void for async methods - did you really mean this?";
                            qDebug() <<  "       : consider editing the xml to make the return type \'void\'";
                        }
                    }

                    if (line.contains(chainTag)) {
                        hasChains = true;
                        line.remove(chainTag);
                        line.replace("(", "(const QString &backServiceName, const QString &windowTitle, const uint windowId, ");
                    }

                    newAdaptorHeaderStream << line << endl;
                } else {
                    // remove extraneous spaces (inserted randomly by qdbusxml2cpp?)
                    line.replace(QRegExp("\\s+(/*>)"), "\\1");

                    newAdaptorHeaderStream << line << endl;
                }
            }
        }

        newAdaptorHeaderFile.close();
        adaptorHeaderFile.close();

        // mv new adaptor header file (with chain parameters added)
        // to replace one produced by qdbusxml2cpp
        adaptorHeaderFile.remove();
        newAdaptorHeaderFile.rename(adaptorHeaderFilename);

        // remove temporary file
        removeNewXmlFile();
    }

    void processProxyHeaderFile()
    {
        QStringList ifSignals;

        QFile proxyHeaderFile(proxyHeaderFilename);
        if (!proxyHeaderFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qCritical("Could not open %s", qPrintable(proxyHeaderFilename));
            exit(-1);
        }
        QTextStream proxyHeaderStream(&proxyHeaderFile);

        QFile newProxyHeaderFile(newProxyHeaderFilename);
        if (!newProxyHeaderFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            qCritical("Could not open %s", qPrintable(newProxyHeaderFilename));
            exit(-1);
        }
        QTextStream newProxyHeaderStream(&newProxyHeaderFile);

        QFile wrapperHeaderFile(wrapperHeaderFilename);
        if (!wrapperHeaderFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            qCritical("Could not open %s", qPrintable(wrapperHeaderFilename));
            exit(-1);
        }
        QTextStream wrapperHeaderStream(&wrapperHeaderFile);

        QFile wrapperCppFile(wrapperCppFilename);
        if (!wrapperCppFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            qCritical("Could not open %s", qPrintable(wrapperCppFilename));
            exit(-1);
        }
        QTextStream wrapperCppStream(&wrapperCppFile);

        wrapperCppStream    << topBitC() << endl;
        wrapperHeaderStream << topBitH() << endl;

        bool inSignalSection = false;
        bool inChainTask     = false;

        while (!proxyHeaderStream.atEnd()) {
            QString line = proxyHeaderStream.readLine();
            chomp(line);

            // add documentation and remove doc tags here
            // note that middle bit is added here too, so it's not just about doc
            if (line.contains(QRegExp("^class"))) {
                if (!doc[ 0 ].isEmpty()) {
                    QStringList thisDoc(doc[ 0 ]);

                    // adjust indentation
                    QRegExp matchThis("^(\\s+)");
                    if (thisDoc.join("\n").contains(matchThis)) {
                        thisDoc.replaceInStrings(QRegExp("^" + matchThis.cap(1)), "");
                    }
                    wrapperHeaderStream << thisDoc.join("\n") << endl;
                }
                wrapperHeaderStream << middleBitH() << endl;
            } else {
                for (int docTagNo = 1; docTagNo < doc.size(); ++docTagNo) {
                    QString fullDocTag = docTag + QString::number(docTagNo);
                    if (line.contains(fullDocTag)) {
                        line.remove(fullDocTag);
                        if (!line.contains("return")) {
                            QStringList thisDoc = doc[ docTagNo ];

                            if (!thisDoc.isEmpty()) {
                                // adjust indentation
                                QRegExp matchThis("^(\\s+)");
                                if (thisDoc.join("\n").contains(matchThis)) {
                                    QString correctIndentation(4, ' ');
                                    thisDoc.replaceInStrings(QRegExp("^" + matchThis.cap(1)), correctIndentation);
                                }
                                wrapperHeaderStream << endl;
                                wrapperHeaderStream << thisDoc.join("\n") << endl;
                            }
                        }
                    }
                }
            }
            line.remove(QRegExp(docTag + "\\d+"));

            // add chaining code to NEWPROXY
            {
                // always remove asyncTag, since we don't care about it in the cpp file
                line.remove(asyncTag);

                if (needsDuiApplication && line.contains(QRegExp("#include <QtCore/Q(Meta)?Object>"))) {
                    newProxyHeaderStream << "#include <DuiApplication>" << endl;
                    newProxyHeaderStream << "#include <DuiApplicationWindow>" << endl;
                    newProxyHeaderStream << "#include <DuiApplicationIfProxy>" << endl;
                    newProxyHeaderStream << "#include <DuiComponentData>" << endl;
                    newProxyHeaderStream << "#include <QX11Info>" << endl;
                    newProxyHeaderStream << "#include <X11/Xutil.h>" << endl;
                    newProxyHeaderStream << "#include <X11/Xlib.h>" << endl;
                    newProxyHeaderStream << "#include <X11/Xatom.h>" << endl;
                    newProxyHeaderStream << "#include <X11/Xmd.h>" << endl;
                    newProxyHeaderStream << endl;
                    newProxyHeaderStream << line << endl;
                } else if (needsDuiApplication && line.contains(newXmlFilename)) {
                    line.replace(newXmlFilename, xmlFilename);
                    newProxyHeaderStream << line << endl;
                } else if (inChainTask) {
                    if (line.contains("QList<QVariant>")) {
                        newProxyHeaderStream << "        Qt::HANDLE windowId       = DuiApplication::instance()->activeWindow()->winId();" << endl;
                        newProxyHeaderStream << "        QString windowTitle       = DuiApplication::instance()->activeWindow()->windowTitle();" << endl;
                        newProxyHeaderStream << "        QString goBackServiceName = DuiComponentData::instance()->serviceName();" << endl;
                        newProxyHeaderStream << "" << endl;
                        newProxyHeaderStream << line << endl;
                        newProxyHeaderStream << "        argumentList << qVariantFromValue(goBackServiceName);" << endl;
                        newProxyHeaderStream << "        argumentList << qVariantFromValue(windowTitle);" << endl;
                        newProxyHeaderStream << "        argumentList << qVariantFromValue((uint)windowId);" << endl;
                    } else if (line.contains("return")) {

                        newProxyHeaderStream << hideThisWindowCode << endl;

                        line.remove(chainTag);
                        newProxyHeaderStream << line << endl;
                        inChainTask = false;
                    } else {
                        newProxyHeaderStream << line << endl;
                    }
                } else if (line.contains(chainTag)) {
                    line.remove(chainTag);
                    newProxyHeaderStream << line << endl;
                    inChainTask = true;
                } else {
                    newProxyHeaderStream << line << endl;
                }
            }

            if (inSignalSection) {
                bool atEndOfSignalSection = (line == "};");
                if (atEndOfSignalSection) {
                    inSignalSection = false;
                } else {
                    ifSignals.append(line);
                }
            } else {
                if (line.contains("Q_SIGNALS:")) {
                    inSignalSection = true;
                } else {
                    QRegExp matchThis("inline\\s+QDBusPendingReply<([^>]*)>\\s*(\\w+)\\(([^)]*)\\)");
                    if (line.contains(matchThis)) {
                        QString returnType = matchThis.cap(1);
                        QString methodName = matchThis.cap(2);
                        QString parameters = matchThis.cap(3);

                        if (returnType.isEmpty()) {
                            returnType = "void";
                        }

                        QStringList paramNames = getParamNames(parameters);

                        wrapperHeaderStream << "    " + returnType + " " + methodName + "( " + parameters + " );" << endl;

                        wrapperCppStream << returnType + " " + upperCamelServiceName + "::" + methodName + "( " + parameters + " )" << endl;
                        wrapperCppStream << "{" << endl;
                        if (returnType == "void") {
                            wrapperCppStream << "    static_cast<" + upperCamelServiceName + "Proxy*>(interfaceProxy)->" + methodName + "( " + paramNames.join(", ") + " );" << endl;
                        } else {
                            wrapperCppStream << "    return qobject_cast<" + upperCamelServiceName + "Proxy*>(interfaceProxy)->" + methodName + "( " + paramNames.join(", ") + " ).value();" << endl;
                        }
                        wrapperCppStream << "}\n" << endl;
                    }
                }
            }
        }

        allSignals = ifSignals.join("\n");

        // process signals, removing void and param names
        QStringList connectSignals;
        for (int thisSignalIndex = 0; thisSignalIndex < ifSignals.size(); ++thisSignalIndex) {
            QString thisSignal = ifSignals.at(thisSignalIndex);

            QString signalName;
            QString signature;
            QRegExp matchThis("^\\s+void\\s+(\\w+)\\(([^)]*)\\);$");
            if (thisSignal.contains(matchThis)) {
                signalName = matchThis.cap(1);
                signature  = matchThis.cap(2);
            }

            QStringList typesOnly;
            QStringList paramsAndTypes = signature.split(',');
            for (int thisParamAndTypeIndex = 0; thisParamAndTypeIndex < paramsAndTypes.size(); ++thisParamAndTypeIndex) {
                QString thisParamsAndType = paramsAndTypes.at(thisParamAndTypeIndex);
                // remove last word
                QRegExp matchThis("(.*)\\b\\w+$");
                if (thisParamsAndType.contains(matchThis)) {
                    typesOnly.append(matchThis.cap(1));
                }
            }
            QString joinedTypes(typesOnly.join(","));

            connectSignals.append("\
    connect( interfaceProxy, SIGNAL( " + signalName + "( " + joinedTypes + " ) ),\
             this, SIGNAL( " + signalName + "( " + joinedTypes + " ) ) );\
    ");
        }

        allConnectSignals = connectSignals.join("\n");

        wrapperCppStream << botBitC() << endl;
        wrapperHeaderStream << botBitH() << endl;

        // mv new proxy header file (with chain parameters added)
        // to replace one produced by qdbusxml2cpp
        proxyHeaderFile.remove();
        newProxyHeaderFile.rename(proxyHeaderFilename);

        // remove temporary file
        removeNewXmlFile();
    }

// add a tag to the name of chain and async methods in the xml file
// so that we can easily tell which methods are to be chained/asynched after they are processed by qdbusxml2cpp
    void preprocessXML()
    {
        QString inDoc("outOfDoc");
        int docTagNo = -1;
        QFile xmlFile(xmlFilename);
        if (!xmlFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qCritical("Could not open %s", qPrintable(xmlFilename));
            exit(-1);
        }
        QTextStream xmlStream(&xmlFile);

        QFile newXmlFile(newXmlFilename);
        if (!newXmlFile.open(QFile::WriteOnly | QFile::Truncate)) {
            qCritical("Could not open %s", qPrintable(newXmlFilename));
            exit(-1);
        }
        QTextStream newXmlStream(&newXmlFile);

        while (!xmlStream.atEnd()) {
            QString line = xmlStream.readLine();

            // tag tasks
            if (line.contains("chainTask=\"true\"")) {
                // add chaintag
                QRegExp matchThis("name=\"([^\"]+)\"");
                line.replace(matchThis, "name=\"\\1" + chainTag + "\"");
                needsDuiApplication = true;
            }
            if (line.contains("asyncTask=\"true\"")) {
                // add asynctag
                QRegExp matchThis("name=\"([^\"]+)\"");
                line.replace(matchThis, "name=\"\\1" + asyncTag + "\"");
                needsDuiApplication = true;
            }

            // tag the interfaces and methods
            if (line.contains("<interface") || line.contains("<method")) {
                ++docTagNo;
                doc[ docTagNo ] = QStringList();
                QRegExp matchThis("name=\"([^\"]+)\"");
                line.replace(matchThis, "name=\"\\1" + docTag + QString::number(docTagNo) + "\"");
            }

            if (line.contains("<doc>")) {
                // record entry of doc blocks
                inDoc = "openDoc";
            } else if (line.contains("</doc>")) {
                // record exit of doc blocks
                inDoc = "closeDoc";
            }

            if (inDoc == "insideDoc") {
                // record doc lines
                doc[ docTagNo ] << line;
            } else if (inDoc == "openDoc") {
                inDoc = "insideDoc";
            } else if (inDoc == "closeDoc") {
                inDoc = "outsideDoc";
            } else {
                newXmlStream << line << endl;
            }
        }

        newXmlStream.flush();
        xmlStream.flush();
    }

    void runQDBusXml2Cpp(const QStringList &params)
    {
        QProcess qdbusxml2cpp;
        qdbusxml2cpp.setWorkingDirectory(cwd);

        qdbusxml2cpp.start(command, params);
        if (!qdbusxml2cpp.waitForStarted())
            qCritical() << qdbusxml2cpp.error();
        if (!qdbusxml2cpp.waitForFinished())
            qCritical() << "did not finish";
    }

    void usage()
    {
        qDebug() << "usage: $0 [-a|-p] interfaceName";
        qDebug() << "       -a            generate adaptor files";
        qDebug() << "       -p            generate proxy files";
        qDebug() << "       -h            this help";
        exit(1);
    }

    int main(int argc, char *argv[])
    {
        QCoreApplication *app = new QCoreApplication(argc, argv);
        Q_UNUSED(app);

        QString interfaceName("");
        pid = QString::number(QCoreApplication::applicationPid());

        if (argc == 1) {
            usage();
        }

        QHash<QString, bool> opts;
        opts[ "a" ] = false;
        opts[ "p" ] = false;

        //foreach (QString arg, QStringList( argv ) ) {
        for (int argIndex = 1; argIndex < argc; ++argIndex) {
            QString arg(argv[ argIndex ]);

            if (arg == "-h") {
                usage();
            } else if (arg == "-a") {
                opts[ "a" ] = true;
                if (opts[ "p" ]) {
                    qDebug() << "both -p and -a supplied";
                    qDebug() << "disabling proxy generation";
                    opts[ "p" ] = 0;
                }
            } else if (arg == "-p") {
                opts[ "p" ] = true;
                if (opts[ "a" ]) {
                    qDebug() << "both -p and -a supplied";
                    qDebug() << "disabling adaptor generation";
                    opts[ "a" ] = false;
                }
            } else {
                QStringList bits = arg.split("/");

                interfaceName = bits.takeLast();
                QString interfacePath = bits.join("/");
                if (!interfacePath.isEmpty()) {
                    cwd = interfacePath;
                }
            }
        }

        if (! opts[ "a" ] && ! opts[ "p" ]) {
            qDebug() << "neither -p or -a specified";
            qDebug() << "assuming -p";
            opts[ "p" ] = true;
        }

        if (interfaceName.isEmpty()) {
            usage();
        }

        // make different upper/lower case versions from the Upper Camel version
        upperCamelServiceName = interfaceName.split(".").last();
        lowerServiceName      = upperCamelServiceName.toLower();

        xmlFilename           = interfaceName + ".xml";
        newXmlFilename        = interfaceName + "-" + pid + ".xml";

        // tag to add to chained methods in the xml file
        chainTag = "CHAINTASK";
        asyncTag = "ASYNCTASK";
        docTag   = "DOCTAG";

        upperCamelProxyName       = "";
        proxyCppFilename          = "";
        proxyHeaderFilename       = "";
        newProxyHeaderFilename    = "";
        wrapperHeaderFilename     = "";
        wrapperCppFilename        = "";
        wrapperHeaderGuard        = "";
        upperCamelAdaptorName     = "";
        adaptorCppFilename        = "";
        newAdaptorCppFilename     = "";
        newAdaptorHeaderFilename  = "";
        needsDuiApplication       = false;

        preprocessXML();

        if (opts[ "p" ]) {
            upperCamelProxyName      = upperCamelServiceName + "Proxy";
            QString proxyBase        = lowerServiceName + "proxy";
            proxyCppFilename         = proxyBase + ".cpp";
            proxyHeaderFilename      = proxyBase + ".h";
            newProxyHeaderFilename   = proxyBase + "-" + pid + ".h";
            wrapperHeaderFilename    = lowerServiceName + ".h";
            wrapperCppFilename       = lowerServiceName + ".cpp";
            wrapperHeaderGuard       = wrapperHeaderFilename.toUpper().replace(".", "_");

            runQDBusXml2Cpp(QStringList() << "-c" << upperCamelProxyName << "-p" << proxyBase << newXmlFilename);
            processProxyHeaderFile();
        } else {
            upperCamelAdaptorName     = upperCamelServiceName + "Adaptor";
            QString adaptorBase       = lowerServiceName + "adaptor";
            adaptorCppFilename        = adaptorBase + ".cpp";
            adaptorHeaderFilename     = adaptorBase + ".h";
            newAdaptorCppFilename     = adaptorBase + "-" + pid + ".cpp";
            newAdaptorHeaderFilename  = adaptorBase + "-" + pid + ".h";

            runQDBusXml2Cpp(QStringList() << "-c" << upperCamelAdaptorName << "-a" << adaptorBase << newXmlFilename);
            processAdaptorCppFile();
            processAdaptorHeaderFile();
        }
    }
