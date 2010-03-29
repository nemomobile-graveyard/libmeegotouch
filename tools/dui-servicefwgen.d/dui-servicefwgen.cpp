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

#include <QDomDocument>

class Worker {
public:
    Worker();

    void setInterfaceName( const QString& val );
    void setAllSignals( const QString& val );
    void setAllConnectSignalCommands( const QString& val );

    void setCreateAdaptor( bool val );
    void setCreateProxy( bool val );

    void setClassDoc( const QString& doc );
    void setMethodDoc( int id, const QString& doc );

    void setNeedsDuiApplication( bool val );

    QString upperCamelServiceName();
    QString lowerServiceName();
    QString xmlFileName();
    QString newXmlFileName();
    QString chainTag();
    QString asyncTag();
    QString upperCamelProxyName();

    QString proxyHeaderFileName();
    QString proxyCppFileName();

    QString newProxyHeaderFileName();
    QString newProxyCppFileName();

    QString wrapperHeaderFileName();
    QString wrapperCppFileName();

    QString wrapperHeaderGuard();
    QString upperCamelAdaptorName();
    QString adaptorCppFileName();
    QString newAdaptorCppFileName();
    QString newAdaptorHeaderFileName();
    QString adaptorHeaderFileName();
    QString docTag();
    QString proxyBase();
    QString adaptorBase();

    QString hideThisWindowCode();

    QString topBitH();
    QString middleBitH();
    QString botBitH();

    QString topBitC();
    QString botBitC();

    QString duiHeaders();

    bool createAdaptor();
    bool createProxy();

    QString classDoc();
    QString methodDoc( int id );

    QString mangledClassDoc();
    QString mangledMethodDoc( int id );

    bool needsDuiApplication();

    void preprocessXML();
    void createConnectSignalCommands( const QStringList& ifSignals );

    void removeDocTag( QString &line );

private:
    QString m_interfaceName;

    QString m_upperCamelServiceName;
    QString m_lowerServiceName;
    QString m_xmlFileName;
    QString m_newXmlFileName;
    QString m_chainTag;
    QString m_asyncTag;
    QString m_upperCamelProxyName;
    QString m_proxyCppFileName;
    QString m_proxyHeaderFileName;
    QString m_newProxyHeaderFileName;
    QString m_newProxyCppFileName;
    QString m_wrapperHeaderFileName;
    QString m_wrapperCppFileName;
    QString m_wrapperHeaderGuard;
    QString m_upperCamelAdaptorName;
    QString m_adaptorCppFileName;
    QString m_newAdaptorCppFileName;
    QString m_newAdaptorHeaderFileName;
    QString m_adaptorHeaderFileName;
    QString m_docTag;
    QString m_proxyBase;
    QString m_adaptorBase;

    QString m_allSignals;
    QString m_allConnectSignalCommands;

    bool m_createAdaptor;
    bool m_createProxy;

    QString m_classDoc;
    QHash<int, QString> m_methodDocs;

    bool m_needsDuiApplication;
};

Worker::Worker()
{
    m_chainTag = "CHAINTASK";
    m_asyncTag = "ASYNCTASK";
    m_docTag   = "DOCTAG";

    m_createAdaptor = false;
    m_createProxy = false;

    m_needsDuiApplication = false;
}

void Worker::setInterfaceName( const QString& interfaceName )
{
    QString pid = QString::number(QCoreApplication::applicationPid());

    m_interfaceName = interfaceName;

    m_upperCamelServiceName = interfaceName.split(".").last();
    m_lowerServiceName      = m_upperCamelServiceName.toLower();

    m_xmlFileName           = interfaceName + ".xml";
    m_newXmlFileName        = interfaceName + "-" + pid + ".xml";

    // if -p
    m_upperCamelProxyName      = m_upperCamelServiceName + "Proxy";
    m_proxyBase                = m_lowerServiceName + "proxy";
    m_proxyCppFileName         = m_proxyBase + ".cpp";
    m_proxyHeaderFileName      = m_proxyBase + ".h";
    m_newProxyHeaderFileName   = m_proxyBase + "-" + pid + ".h";
    m_newProxyCppFileName      = m_proxyBase + "-" + pid + ".cpp";
    m_wrapperHeaderFileName    = m_lowerServiceName + ".h";
    m_wrapperCppFileName       = m_lowerServiceName + ".cpp";
    m_wrapperHeaderGuard       = m_wrapperHeaderFileName.toUpper().replace(".", "_");

    // if -a
    m_upperCamelAdaptorName     = m_upperCamelServiceName + "Adaptor";
    m_adaptorBase               = m_lowerServiceName + "adaptor";
    m_adaptorCppFileName        = m_adaptorBase + ".cpp";
    m_adaptorHeaderFileName     = m_adaptorBase + ".h";
    m_newAdaptorCppFileName     = m_adaptorBase + "-" + pid + ".cpp";
    m_newAdaptorHeaderFileName  = m_adaptorBase + "-" + pid + ".h";
}

void Worker::setAllSignals( const QString& val )
{
    m_allSignals = val;
}

void Worker::setAllConnectSignalCommands( const QString& val )
{
    m_allConnectSignalCommands = val;
}

void Worker::setCreateAdaptor( bool val )
{
    m_createAdaptor = val;
}

void Worker::setCreateProxy( bool val )
{
    m_createProxy = val;
}

void Worker::setClassDoc( const QString& doc )
{
    m_classDoc = doc;
}

void Worker::setMethodDoc( int id, const QString& doc )
{
    m_methodDocs[ id ] = doc;
}

void Worker::setNeedsDuiApplication( bool val )
{
    m_needsDuiApplication = val;
}


QString Worker::upperCamelServiceName()
{
    return m_upperCamelServiceName;
}

QString Worker::lowerServiceName()
{
    return m_lowerServiceName;
}

QString Worker::xmlFileName()
{
    return m_xmlFileName;
}

QString Worker::newXmlFileName()
{
    return m_newXmlFileName;
}

QString Worker::chainTag()
{
    return m_chainTag;
}

QString Worker::asyncTag()
{
    return m_asyncTag;
}

QString Worker::upperCamelProxyName()
{
    return m_upperCamelProxyName;
}

QString Worker::proxyCppFileName()
{
    return m_proxyCppFileName;
}

QString Worker::proxyHeaderFileName()
{
    return m_proxyHeaderFileName;
}

QString Worker::newProxyHeaderFileName()
{
    return m_newProxyHeaderFileName;
}

QString Worker::newProxyCppFileName()
{
    return m_newProxyCppFileName;
}

QString Worker::wrapperHeaderFileName()
{
    return m_wrapperHeaderFileName;
}

QString Worker::wrapperCppFileName()
{
    return m_wrapperCppFileName;
}

QString Worker::wrapperHeaderGuard()
{
    return m_wrapperHeaderGuard;
}

QString Worker::upperCamelAdaptorName()
{
    return m_upperCamelAdaptorName;
}

QString Worker::adaptorCppFileName()
{
    return m_adaptorCppFileName;
}

QString Worker::newAdaptorCppFileName()
{
    return m_newAdaptorCppFileName;
}

QString Worker::newAdaptorHeaderFileName()
{
    return m_newAdaptorHeaderFileName;
}

QString Worker::adaptorHeaderFileName()
{
    return m_adaptorHeaderFileName;
}

QString Worker::docTag()
{
    return m_docTag;
}

QString Worker::proxyBase()
{
    return m_proxyBase;
}

QString Worker::adaptorBase()
{
    return m_adaptorBase;
}

bool Worker::createAdaptor()
{
    return m_createAdaptor;
}

bool Worker::createProxy()
{
    return m_createProxy;
}

bool Worker::needsDuiApplication()
{
    return m_needsDuiApplication;
}

QString Worker::classDoc()
{
    return m_classDoc;
}

QString Worker::methodDoc( int id )
{
    return m_methodDocs[ id ];
}

QString getDoxygenFromXml( const QString& xml, int indentCount )
{
    // get doc tokens from xml
    QDomDocument doc;

    QString errorStr;
    int errorLine;
    int errorColumn;

    QHash<const QString, QString> xmlTokens;

    doc.setContent(xml, true, &errorStr, &errorLine, &errorColumn);

    QDomNode node = doc.documentElement();
    QDomElement el = node.toElement();
    el = el.firstChildElement( "arg" );

    while ( ! el.isNull() )
    {
        xmlTokens.insert( el.attribute( "tag" ), el.text() );
        el = el.nextSiblingElement( "arg" );
    }

    QStringList lines;

    QString indent;
    indent.fill( ' ', indentCount );


    QString result;
    QTextStream s( &result );

    QStringList doxTokens;
    doxTokens << "class" << "brief" << "details" << "state" << "ingroup";

    s << "\n" << indent << "/**\n";

    bool hasDoc = false;

    foreach( const QString dT, doxTokens )
    {
        if ( !xmlTokens.value( dT ).isEmpty() )
        {
            hasDoc = true;

            QString longLine = "@" + dT + " " + xmlTokens.value( dT );

            while ( !longLine.isEmpty() )
            {
                int pos = longLine.indexOf( ' ', 60 - indentCount );

                if ( pos == -1 )
                {
                    pos = longLine.length();
                }

                QString line = longLine.left( pos );
                longLine = longLine.mid( pos );
                s << indent << " * " << line << "\n";
            }
        }
    }

    s << indent << " */\n";

    if ( hasDoc )
    {
        return result;
    }
    else
    {
        return "";
    }
}

QString Worker::mangledClassDoc()
{
    return getDoxygenFromXml( m_classDoc, 0 );
}

QString Worker::mangledMethodDoc( int id )
{
    return getDoxygenFromXml( m_methodDocs[ id ], 4 );
}

QString Worker::hideThisWindowCode()
{
    return
"\n\
        // hide this window\n\
        {\n\
            // Tell the window to not to be shown in the switcher\n\
            Atom skipTaskbarAtom = XInternAtom(QX11Info::display(), \"_NET_WM_STATE_SKIP_TASKBAR\", False);\n\
\n\
            Atom netWmStateAtom = XInternAtom(QX11Info::display(), \"_NET_WM_STATE\", False);\n\
            QVector<Atom> atoms;\n\
            atoms.append(skipTaskbarAtom);\n\
            XChangeProperty(QX11Info::display(), windowId, netWmStateAtom, XA_ATOM, 32,\n\
                            PropModeReplace, (unsigned char *)atoms.data(), atoms.count());\n\
            XSync(QX11Info::display(), False);\n\
        }\n\
";
}


QString Worker::topBitH()
{
    QString commandLine(QCoreApplication::arguments().join(" "));

    return
"#ifndef " + wrapperHeaderGuard() + "\n\
#define " + wrapperHeaderGuard() + "\n\
\n\
/*\n\
 * automatically generated with the command line :\n\
 * " + commandLine + "\n\
 */\n\
\n\
#include <QObject>\n\
\n\
#include <duiservicefwbaseif.h>\n";
}


QString Worker::middleBitH()
{
    return
"class " + upperCamelServiceName() + " : public DuiServiceFwBaseIf\n\
{\n\
Q_OBJECT\n\
\n\
public:\n";
}


QString Worker::botBitH()
{
    return
"\n\
public:\n\
    /*!\n\
     * @brief Constructs a base interface\n\
     * @param preferredService, define the preferred service provider. Leave\n\
     * empty if no preferred provider. In most cases, this should be left\n\
     * empty.\n\
     * @param parent Parent object\n\
     */\n\
\n\
    " + upperCamelServiceName() + "( const QString& preferredService = \"\", QObject* parent = 0 );\n\
\n\
    /*!\n\
     * @brief Set the service name\n\
     * @param service Name of the desired service\n\
     */\n\
\n\
    void setService(const QString & service);\n\
Q_SIGNALS:\n\
" + m_allSignals + "\n\
};\n\
#endif\n\
";
}


QString Worker::topBitC()
{
    QString commandLine(QCoreApplication::arguments().join(" "));

    return
"/*\n\
 * automatically generated with the command line :\n\
 * " + commandLine + "\n\
 */\n\
\n\
#include \"" + wrapperHeaderFileName() + "\"\n\
#include \"" + proxyHeaderFileName() + "\"\n";
}


QString Worker::botBitC()
{
    return
"" + upperCamelServiceName() + "::" + upperCamelServiceName() + "( const QString& preferredService, QObject* parent )\n\
    : DuiServiceFwBaseIf( " + upperCamelProxyName() + "::staticInterfaceName(), parent )\n\
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
        interfaceProxy = new " + upperCamelProxyName() + "( service, \"/\", QDBusConnection::sessionBus(), this );\n\
        // allConnectSignals go here (empty block if none)\n\
" + m_allConnectSignalCommands + "\n\
    }\n\
}\n\
\n\
void " + upperCamelServiceName() + "::setService(const QString & service)\n\
{\n\
    if (service.isEmpty()) return;\n\
\n\
    this->service = service;\n\
\n\
    if ( interfaceProxy ) {\n\
        delete interfaceProxy;\n\
        interfaceProxy = 0;\n\
    }\n\
    interfaceProxy = new " + upperCamelProxyName() + "( service, \"/\", QDBusConnection::sessionBus(), this );\n\
    {\n\
" + m_allConnectSignalCommands + "\n\
    }\n\
}\n";
}


QString Worker::duiHeaders()
{
    return
"#include <DuiApplication>\n\
#include <DuiApplicationWindow>\n\
#include <DuiApplicationIfProxy>\n\
#include <DuiComponentData>\n\
#include <QX11Info>\n\
#include <X11/Xutil.h>\n\
#include <X11/Xlib.h>\n\
#include <X11/Xatom.h>\n\
#include <X11/Xmd.h>\n";
}

void Worker::preprocessXML()
{
    int docTagNo = 0;

    QFile inFile(xmlFileName());
    if (!inFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qCritical("Could not open %s", qPrintable(xmlFileName()));
        exit(-1);
    }

    QFile outFile(newXmlFileName());
    if (!outFile.open(QFile::WriteOnly | QFile::Truncate)) {
        qCritical("Could not open %s", qPrintable(newXmlFileName()));
        exit(-1);
    }

    QDomDocument doc;

    QString errorStr;
    int errorLine;
    int errorColumn;

    if (!doc.setContent(&inFile, true, &errorStr, &errorLine, &errorColumn))
    {
        qWarning( "doc.setContent failed" );
        return;
    }

    // here we go from <node> to <interface>,
    // then we save and strip the <doc> childElement, if it exists,
    // then we walk over the methods,
    // and save the doc for them,
    // and mangle the doctag, chaintasktag and asynctag strings into the method names

    QDomNode node = doc.documentElement();
    if ( node.nodeName() != "node" )
    {
        qCritical( "Error: this is not a valid interface. exiting" );
        exit( -1 );
    }

    QDomElement el = node.toElement();
    el = el.firstChildElement( "interface" );

    // try to save and drop <doc> child of <interface>
    if ( !el.firstChildElement( "doc" ).isNull() )
    {
        QString string;
        QTextStream stream( &string );
        el.firstChildElement( "doc" ).save( stream, 4 );

        setClassDoc( string );

        // mangle doc tag into class name
        el.setAttribute( "name", el.attribute( "name" ) + docTag() );

        el.removeChild( el.firstChildElement( "doc" ) );
    }

    // walk over the methods
    el = el.firstChildElement( "method" );

    while ( ! el.isNull() )
    {
        // now handle chainTask and asyncTask for this method
        if ( el.attribute( "chainTask" ) == "true" )
        {
            el.setAttribute( "name", el.attribute( "name" ) + chainTag() );
            setNeedsDuiApplication( true );
        }

        if ( el.attribute( "asyncTask" ) == "true" )
        {
            el.setAttribute( "name", el.attribute( "name" ) + asyncTag() );
            setNeedsDuiApplication( true );
        }

        // handle doc
        if ( ! el.firstChildElement( "doc" ).isNull() )
        {
            QString string;
            QTextStream stream( &string );
            el.firstChildElement( "doc" ).save( stream, 4 );

            ++docTagNo;
            setMethodDoc( docTagNo, string );

            // mangle doc tag into class name
            el.setAttribute( "name", el.attribute( "name" ) + docTag() + QString::number(docTagNo) );

            el.removeChild( el.firstChildElement( "doc" ) );
        }


        el = el.nextSiblingElement( "method" );
    }

    outFile.write( qPrintable( doc.toString( 4 ) ) );
}


void Worker::createConnectSignalCommands( const QStringList& ifSignals )
{
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

        connectSignals.append(
"    connect( interfaceProxy, SIGNAL( " + signalName + "( " + joinedTypes + " ) ),\n\
             this, SIGNAL( " + signalName + "( " + joinedTypes + " ) ) );\n");
    }

    setAllConnectSignalCommands( connectSignals.join( "" ) );
}

void Worker::removeDocTag( QString &line )
{
    line.remove( QRegExp( docTag() + "[0-9]*" ) );
}

namespace
{
    const QString command("qdbusxml2cpp");

    Worker w;

    QString cwd(".");
};


void removeNewXmlFile()
{
    QFile newXmlFile(w.newXmlFileName());
    newXmlFile.remove();
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

    QFile adaptorCppFile(w.adaptorCppFileName());
    if (!adaptorCppFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qCritical("Could not open %s", qPrintable(w.adaptorCppFileName()));
        exit(-1);
    }
    QTextStream inS(&adaptorCppFile);

    QFile newAdaptorCppFile(w.newAdaptorCppFileName());
    if (!newAdaptorCppFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qCritical("Could not open %s", qPrintable(w.adaptorCppFileName()));
        exit(-1);
    }
    QTextStream outS(&newAdaptorCppFile);

    bool inChainTask = false;
    bool needGoBackMethod = false;

    while (!inS.atEnd()) {
        QString line = inS.readLine();
        line.remove(QRegExp("\n$"));

        // remove doctag - can be more than one per line
        w.removeDocTag( line );

        // add chaining code to NEWADAPTOR
        {
            // always remove asyncTag, since we don't care about it in the cpp file
            line.remove(w.asyncTag());

            if (w.needsDuiApplication() && line.contains(QRegExp("#include <QtCore/Q(Meta)?Object>"))) {
                outS <<
                    "\n"                        \
                    "#include <DuiApplication>\n"       \
                    "#include <DuiApplicationPage>\n"   \
                    "#include <DuiApplicationWindow>\n" \
                    "#include <DuiApplicationIfProxy>\n"        \
                    "#include <DuiEscapeButtonPanel>\n"         \
                    "#include <QX11Info>\n"                     \
                    "#include <X11/Xutil.h>\n"                  \
                    "#include <X11/Xlib.h>\n"                   \
                    "#include <X11/Xatom.h>\n"                  \
                    "#include <X11/Xmd.h>\n"                    \
                    "\n"
                    + line + "\n";
            } else if (w.needsDuiApplication() && line.contains("QDBusAbstractAdaptor(parent)")) {
                outS
                    << line + "," << endl
                    << "    backServiceName()," << endl
                    << "    windowId(-1)" << endl;
            } else if (line.contains(w.newXmlFileName())) {
                line.replace(w.newXmlFileName(), w.xmlFileName());
                outS << line << endl;
            } else if (inChainTask) {
                line.remove(w.chainTag());
                if (line == "{") {
                    outS << line << endl
                         << "    this->windowId = windowId;" << endl
                         << "    this->backServiceName = backServiceName;" << endl
                         << endl;
                } else if (line.contains("return") || line == "}") {
                    // match end of function - need to add the connect *before* the return, if there is one
                    outS <<
"\n"\
"    DuiApplication::instance()->activeWindow()->setWindowTitle( windowTitle );\n"\
"    DuiApplicationWindow *appWindow = DuiApplication::activeApplicationWindow();\n"\
"    if (appWindow != 0) {\n"\
"        appWindow->currentPage()->setEscapeMode( DuiApplicationPageModel::EscapeManualBack );\n"\
"        // connect to the back button - assumes the above 'showImage' opens a\n"\
"        // new window and so the window referred to below is already the top one\n"\
"        connect(appWindow->currentPage(), SIGNAL(backButtonClicked()),\n"\
"                this, SLOT(goBack()));\n"\
"    }\n"\
"\n"\
"    // update the X server\n"\
"    {\n"\
"        XPropertyEvent p;\n"\
"        p.send_event = True;\n"\
"        p.display = QX11Info::display();\n"\
"        p.type   = PropertyNotify;\n"\
"        p.window = RootWindow(p.display, 0);\n"\
"        p.atom   = XInternAtom(p.display, \"_NET_CLIENT_LIST\", False);\n"\
"        p.state  = PropertyNewValue;\n"\
"        p.time   = CurrentTime;\n"\
"        XSendEvent(p.display, p.window, False, PropertyChangeMask,\n"\
"                (XEvent*)&p);\n"\
"\n"\
"        XSync(QX11Info::display(), False);\n"\
"    }\n"\
"\n"
+ line + "\n";
                    inChainTask = false;
                } else {
                    outS << line << endl;
                }
            } else if (line.contains(w.chainTag())) {
                line.remove(w.chainTag());
                // remove CHAINTASK
                line.replace("(", "(const QString &backServiceName, const QString &windowTitle, const uint windowId, ");
                outS << line << endl;
                inChainTask = true;
                needGoBackMethod  = true;
            } else {
                outS << line << endl;
            }
        }
    }

    if (needGoBackMethod) {
        outS <<
"void " + w.upperCamelAdaptorName() + "::goBack()\n"\
"{\n"                                                   \
"    qDebug() << __PRETTY_FUNCTION__;\n"\
"\n"\
"    bool backServiceRegistered = ( windowId != -1 );\n"\
"    if ( backServiceRegistered ) {\n"\
"        DuiApplicationIfProxy duiApplicationIfProxy(backServiceName, this);\n"\
"\n"\
"        if (duiApplicationIfProxy.connection().isConnected()) {\n"\
"            qDebug() << \"Launching \" << backServiceName;\n"\
"            duiApplicationIfProxy.launch();\n"\
"        } else {\n"\
"            qWarning() << \"Could not launch\" << backServiceName;\n"\
"            qWarning() << \"DBus not connected?\";\n"\
"        }\n"\
"\n"\
"        // unhide the chain parent's window\n"\
"        {\n"\
"            // Tell the window to not to be shown in the switcher\n"\
"            XDeleteProperty(QX11Info::display(), windowId, XInternAtom(QX11Info::display(), \"_NET_WM_STATE\", False));\n"\
"            XSync(QX11Info::display(), False);\n"\
"        }\n"\
"\n"\
"        qWarning() << \"quitting - bye bye\";\n"\
"        QTimer::singleShot( 0, QApplication::instance(), SLOT( quit() ) );\n"\
"    } else {\n"\
"        qWarning() << \"backService is not registered: not going back\";\n"\
"    }\n"\
"}\n";
    }

    newAdaptorCppFile.close();
    adaptorCppFile.close();

    // move new adaptor header file (with chain parameters added)
    // to replace one produced by qdbusxml2cpp
    adaptorCppFile.remove();
    newAdaptorCppFile.rename(w.adaptorCppFileName());

    removeNewXmlFile();
}

void processAdaptorHeaderFile()
{
    QStringList ifSignals;

    QFile adaptorHeaderFile(w.adaptorHeaderFileName());
    if (!adaptorHeaderFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qCritical("Could not open %s", qPrintable(w.adaptorHeaderFileName()));
        exit(-1);
    }
    QTextStream adaptorHeaderFileStream(&adaptorHeaderFile);

    QFile newAdaptorHeaderFile(w.newAdaptorHeaderFileName());
    if (!newAdaptorHeaderFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qCritical("Could not open %s", qPrintable(w.adaptorHeaderFileName()));
        exit(-1);
    }
    QTextStream newAdaptorHeaderStream(&newAdaptorHeaderFile);

    bool hasChains = false;

    while (!adaptorHeaderFileStream.atEnd()) {
        QString line = adaptorHeaderFileStream.readLine();
        line.remove(QRegExp("\n$"));

        // remove doctag - can be more than one per line
        w.removeDocTag( line );

        // add chaining code to NEWADAPTOR
        {
            if (line.contains("Q_SIGNALS:")) {
                if (hasChains) {
                    newAdaptorHeaderStream
                        << "    void goBack();" << endl
                        << "" << endl
                        << "private:" << endl
                        << "    QString backServiceName;" << endl
                        << "    int windowId;" << endl
                        << "" << endl;
                }
                newAdaptorHeaderStream << line << endl;
            } else if (line.contains(QRegExp("chainTask=\\\\\"\\w+\\\\\"")) || line.contains(QRegExp("asyncTask=\\\\\"\\w+\\\\\""))) {           // process comment - remember the backslashes are in the source too, hence so many of them
                // remove asyncTask attribute
                {
                    QRegExp matchThis("asyncTask=\\\\\"\\w+\\\\\"\\s*");
                    if (line.contains(matchThis)) {
                        line.remove(matchThis);
                        line.remove(w.asyncTag());
                    }
                }

                // remove asyncTask attribute
                bool isChainTask = false;
                {
                    QRegExp matchThis("chainTask=\\\\\"(\\w+)\\\\\"\\s*");
                    if (line.contains(matchThis)) {
                        line.remove(matchThis);
                        isChainTask = matchThis.cap(1).contains("true");
                        line.remove(w.chainTag());
                    }
                }

                // remove extraneous spaces (inserted randomly by qdbusxml2cpp?)
                line.replace(QRegExp("\\s+(/*>)"), "\\1");

                newAdaptorHeaderStream << line << endl;

                // this has to be printed after the method tag, above
                if (isChainTask) {
                    newAdaptorHeaderStream
                        << "\"      <arg direction=\\\"in\\\" type=\\\"s\\\" name=\\\"backServiceName\\\"/>\\n\"" << endl
                        << "\"      <arg direction=\\\"in\\\" type=\\\"s\\\" name=\\\"windowTitle\\\"/>\\n\"" << endl
                        << "\"      <arg direction=\\\"in\\\" type=\\\"u\\\" name=\\\"windowId\\\"/>\\n\"" << endl;
                }
            } else if (line.contains(w.chainTag() + "(") || line.contains(w.asyncTag() + "(")) {   // add parameters to the chain methods
                if (line.contains(w.asyncTag())) {
                    line.remove(w.asyncTag());
                    QRegExp matchThis("(\\w+)(\\s+)(\\w+)\\(");
                    line.replace(matchThis, "Q_NOREPLY \\1\\2\\3(");   // put Q_NOREPLY before the first word on the line (which should be 'void', but perhaps not)

                    // make warning if type is not void
                    line.contains(matchThis);
                    QString type = matchThis.cap(1);
                    QString method = matchThis.cap(3);
                    if (type != "void") {
                        QString prototype(line);

                        // clean up prototype and method name for warning message
                        prototype.remove(w.chainTag());
                        prototype.remove(QRegExp("^\\s+"));
                        method.remove(w.chainTag());
                        qDebug() <<  "WARNING: " + prototype;
                        qDebug() <<  "       : asyncTask=\"true\" specified for " + method + "(), but type is " + type;
                        qDebug() <<  "       : return type should be void for async methods - did you really mean this?";
                        qDebug() <<  "       : consider editing the xml to make the return type \'void\'";
                    }
                }

                if (line.contains(w.chainTag())) {
                    hasChains = true;
                    line.remove(w.chainTag());
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

    // move new adaptor header file (with chain parameters added)
    // to replace one produced by qdbusxml2cpp
    adaptorHeaderFile.remove();
    newAdaptorHeaderFile.rename(w.adaptorHeaderFileName());

    // remove temporary file
    removeNewXmlFile();
}


// this method generates the code that is needed for chaining of tasks
void doChainTaskHandling( QString line, bool& inChainTask, QTextStream& newProxyHeaderStream )
{
//     if (w.needsDuiApplication() && line.contains(QRegExp("#include <QtCore/Q(Meta)?Object>")))
//     {
//         newProxyHeaderStream << w.duiHeaders() << line << "\n";
//     }
    if (w.needsDuiApplication() && line.contains(QRegExp("#include <QtDBus/QtDBus>")))
    {
        newProxyHeaderStream << line << "\n\n" << w.duiHeaders();
    }
    else if (w.needsDuiApplication() && line.contains(w.newXmlFileName()))
    {
        line.replace(w.newXmlFileName(), w.xmlFileName());
        newProxyHeaderStream << line << "\n";
    }
    else if (inChainTask)
    {
        if (line.contains("QList<QVariant>"))
        {
            newProxyHeaderStream << "\
        Qt::HANDLE windowId       = DuiApplication::instance()->activeWindow()->winId();\n\
        QString windowTitle       = DuiApplication::instance()->activeWindow()->windowTitle();\n\
        QString goBackServiceName = DuiComponentData::instance()->serviceName();\n\
\n\
" + line + "\n\
        argumentList << qVariantFromValue(goBackServiceName);\n\
        argumentList << qVariantFromValue(windowTitle);\n\
        argumentList << qVariantFromValue((uint)windowId);\n";
        }
        else if (line.contains("return"))
        {
            newProxyHeaderStream << w.hideThisWindowCode() << endl;

            line.remove(w.chainTag());
            newProxyHeaderStream << line << "\n";
            inChainTask = false;
        }
        else
        {
            newProxyHeaderStream << line << "\n";
        }
    }
    else if (line.contains(w.chainTag()))
    {
        line.remove(w.chainTag());
        newProxyHeaderStream << line << "\n";
        inChainTask = true;
    }
    else
    {
        newProxyHeaderStream << line << "\n";
    }
}


void processProxyHeaderFile()
{
    // open proxy header file for reading,
    // open the other files for writing.

    /*
      here we walk over the proxy header file line by line,
      and for every line, we create the needed output in the
      other files, if needed for this line.

      if the line matches "class ...", we generate a doxygen
      documentation snippet, if needed.


     */

    QFile proxyHeaderFile(w.proxyHeaderFileName());
    if (!proxyHeaderFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qCritical("Could not open %s", qPrintable(w.proxyHeaderFileName()));
        exit(-1);
    }
    QTextStream proxyHeaderStream(&proxyHeaderFile);

    QFile newProxyHeaderFile(w.newProxyHeaderFileName());
    if (!newProxyHeaderFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qCritical("Could not open %s", qPrintable(w.newProxyHeaderFileName()));
        exit(-1);
    }
    QTextStream newProxyHeaderStream(&newProxyHeaderFile);

    QFile wrapperHeaderFile(w.wrapperHeaderFileName());
    if (!wrapperHeaderFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qCritical("Could not open %s", qPrintable(w.wrapperHeaderFileName()));
        exit(-1);
    }
    QTextStream wrapperHeaderStream(&wrapperHeaderFile);

    QFile wrapperCppFile(w.wrapperCppFileName());
    if (!wrapperCppFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qCritical("Could not open %s", qPrintable(w.wrapperCppFileName()));
        exit(-1);
    }
    QTextStream wrapperCppStream(&wrapperCppFile);

    wrapperCppStream    << w.topBitC() << endl;
    wrapperHeaderStream << w.topBitH() << endl;

    QStringList ifSignals;

    bool inSignalSection = false;
    bool inChainTask     = false;

    while ( ! proxyHeaderStream.atEnd() )
    {
        QString line = proxyHeaderStream.readLine();

        // add documentation and remove doc tags here
        // note that middle bit is added here too, so it's not just about doc
        if (line.contains(QRegExp("^class")))
        {
            wrapperHeaderStream << w.mangledClassDoc();
            wrapperHeaderStream << w.middleBitH();
        }
        else if ( line.contains( w.docTag() ) )
        {
            // we have to handle doxygen doc here.
            // we have three cases:
            // - class documentation
            // - method documentation
            // - a docTag, that can be ignored, because
            //   it appears somewhere in a method implementation

            if ( ! line.contains( "return" ) )
            {
                // method docs
                QRegExp rx( w.docTag() + "(\\d+)" );

                // do the match
                rx.indexIn( line );

                int id = rx.cap( 1 ).toInt();

                if ( id > 0 )
                {
                    wrapperHeaderStream << w.mangledMethodDoc( id );
                }
            }

            // remove docTag with possible number suffix
            w.removeDocTag( line );
        }

        line.remove(w.asyncTag());

        // add chaining code to NEWPROXY
        doChainTaskHandling( line, inChainTask, newProxyHeaderStream );

        line.remove( w.chainTag() );

        if (inSignalSection)
        {
            bool atEndOfSignalSection = (line == "};");
            if (atEndOfSignalSection)
            {
                inSignalSection = false;
            }
            else
            {
                ifSignals.append(line);
            }
        }
        else
        {
            if (line.contains("Q_SIGNALS:"))
            {
                inSignalSection = true;
            }
            else
            {
                QRegExp matchThis("inline\\s+QDBusPendingReply<([^>]*)>\\s*(\\w+)\\(([^)]*)\\)");
                if (line.contains(matchThis))
                {
                    QString returnType = matchThis.cap(1);
                    QString methodName = matchThis.cap(2);
                    QString parameters = matchThis.cap(3);

                    if (returnType.isEmpty())
                    {
                        returnType = "void";
                    }

                    QStringList paramNames = getParamNames(parameters);

                    wrapperHeaderStream <<
"    " + returnType + " " + methodName + "( " + parameters + " );" << endl;

                    wrapperCppStream <<
returnType + " " + w.upperCamelServiceName() + "::" + methodName + "( " + parameters + " )" << endl;

                    wrapperCppStream <<
"{" << endl;

                    if (returnType == "void")
                    {
                        wrapperCppStream <<
"    static_cast<" + w.upperCamelServiceName() + "Proxy*>(interfaceProxy)->" + methodName + "( " + paramNames.join(", ") + " );" << endl;
                    }
                    else
                    {
                        wrapperCppStream <<
"    return qobject_cast<" + w.upperCamelServiceName() + "Proxy*>(interfaceProxy)->" + methodName + "( " + paramNames.join(", ") + " ).value();" << endl;
                    }
                    wrapperCppStream << "}\n" << endl;
                }
            }
        }
    } // ! proxyHeaderStream.atEnd()


    w.setAllSignals( ifSignals.join("") );
    w.createConnectSignalCommands( ifSignals );

    wrapperCppStream << w.botBitC() << endl;
    wrapperHeaderStream << w.botBitH() << endl;

    // mv new proxy header file (with chain parameters added)
    // to replace one produced by qdbusxml2cpp
    proxyHeaderFile.remove();
    newProxyHeaderFile.rename(w.proxyHeaderFileName());

    // remove temporary file
    removeNewXmlFile();
}

void processProxyCppFile()
{
    // here we only read the cpp file,
    // and remove the tags line by line,
    // and copy the new file over the old one.

    QFile inFile(w.proxyCppFileName());
    if (!inFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qCritical("Could not open %s", qPrintable(w.proxyCppFileName()));
        exit(-1);
    }
    QTextStream inS(&inFile);

    QFile outFile(w.newProxyCppFileName());
    if (!outFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qCritical("Could not open %s", qPrintable(w.newProxyCppFileName()));
        exit(-1);
    }
    QTextStream outS(&outFile);

    while ( ! inS.atEnd() )
    {
        QString line = inS.readLine();

        if ( line.contains( w.docTag() ) )
        {
            w.removeDocTag( line );
        }

        if ( line.contains( w.chainTag() ) )
        {
            line.remove( w.chainTag() );
        }

        if ( line.contains( w.asyncTag() ) )
        {
            line.remove( w.asyncTag() );
        }

        if ( line.contains( w.newXmlFileName() ) )
        {
            line.replace(w.newXmlFileName(), w.xmlFileName());
        }

        outS << line << "\n";
    }

    inFile.remove();
    outFile.rename(w.proxyCppFileName());
}

void runQDBusXml2Cpp(const QStringList &params)
{
    QProcess qdbusxml2cpp;
    qdbusxml2cpp.setWorkingDirectory(cwd);

    qdbusxml2cpp.start(command, params);
    if (!qdbusxml2cpp.waitForStarted())
        qCritical() << qdbusxml2cpp.error();
    if (!qdbusxml2cpp.waitForFinished())
        qCritical() << "dui-servicefwgen: " << command << "did not finish";
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

    if (argc == 1) {
        usage();
    }

    //foreach (QString arg, QStringList( argv ) ) {
    for (int argIndex = 1; argIndex < argc; ++argIndex) {
        QString arg(argv[ argIndex ]);

        if (arg == "-h") {
            usage();
        } else if (arg == "-a") {
            w.setCreateAdaptor( true );
            if ( w.createProxy() ) {
                qDebug() << "both -p and -a supplied";
                qDebug() << "disabling proxy generation";
                w.setCreateProxy( false );
            }
        } else if (arg == "-p") {
            w.setCreateProxy( true );
            if ( w.createAdaptor() ) {
                qDebug() << "both -p and -a supplied";
                qDebug() << "disabling adaptor generation";
                w.setCreateAdaptor( false );
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

    if ( (! w.createAdaptor()) && (! w.createProxy()) ) {
        qDebug() << "neither -p or -a specified";
        qDebug() << "assuming -p";
        w.setCreateProxy( true );
    }

    if (interfaceName.isEmpty()) {
        usage();
    }

    w.setInterfaceName( interfaceName );
    w.preprocessXML();

    if ( w.createProxy() )
    {
        runQDBusXml2Cpp(QStringList()
                        << "-c"
                        << w.upperCamelProxyName()
                        << "-p"
                        << w.proxyBase()
                        << w.newXmlFileName());

        processProxyHeaderFile();
//        processProxyCppFile();
    }
    else
    {
        runQDBusXml2Cpp(QStringList()
                        << "-c"
                        << w.upperCamelAdaptorName()
                        << "-a"
                        << w.adaptorBase()
                        << w.newXmlFileName());

        processAdaptorCppFile();
        processAdaptorHeaderFile();
    }

    return 0;
}
