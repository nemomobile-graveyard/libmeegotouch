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
#include <QDir>

#include <QDomDocument>

class Worker
{
public:
    Worker();

    void setInterfaceName( const QString& val );
    void setInterfaceVersion( const QString& version );
    void setQtSfwDirectory( const QString& directory );
    void setAllSignals( const QString& val );
    void setAllConnectSignalCommands( const QString& val );
    void setXmlFileName( const QString& val );

    void setCreateQtSfw( bool val );
    void setCreateAdaptor( bool val );
    void setCreateProxy( bool val );

    void setClassDoc( const QString& doc );
    void setMethodDoc( int id, const QString& doc );

    void setNeedsMApplication( bool val );

    void setArguments( const int argc, const char * const argv[] );

    void setNameSpace( const QString &nameSpace );

    void setServiceName( const QString &serviceName );

    void setServiceDescription( const QString &serviceDescription );

    bool oldUsage() { return m_oldUsage; };
    void setOldUsage( bool oldUsage ) { m_oldUsage = oldUsage; };

    QString applicationName();
    QString generatedByComment();
    QString upperCamelInterfaceName();
    QString lowerInterfaceName();
    QString upperInterfaceName();
    QString xmlFileName();
    QString newXmlFileName();
    QString chainTag();
    QString asyncTag();
    QString upperCamelProxyName();
    QString nameSpace();
    QString interfaceName();
    QString interfaceVersion();
    QString qtSfwDirectory();
    QString interfaceDescription();
    QString serviceName();
    QString serviceXmlFileName();
    QString lowerServiceName();
    QString upperServiceName();
    QString upperCamelServiceName();
    QString servicePluginHeaderFileName();
    QString servicePluginCppFileName();
    QString serviceProjectFileName();
    //QString servicePluginProxyCppFileName();
    //QString servicePluginProxyHFileName();
    QString serviceDescription();
    QString qtSfwPluginPath();

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
    QRegExp qdbusxml2cppRegExp();

    QString topBitH();
    QString topBitHforQtSfw();
    QString middleBitH();
    QString middleBitHforQtSfw();
    QString botBitH();
    QString botBitHforQtSfw();

    QString topBitC();
    QString topBitCforQtSfw();
    QString botBitC();
    QString botBitCforQtSfw();

    QString mHeaders();

    bool createQtSfw();
    bool createAdaptor();
    bool createProxy();

    QString classDoc();
    QString methodDoc( int id );

    QString mangledClassDoc();
    QString mangledMethodDoc( int id );

    bool needsMApplication();
    bool hasNameSpace();

    void preprocessXML();
    void createConnectSignalCommands( const QStringList& ifSignals, bool forQtSfw=false );

    void removeDocTag( QString &line );

private:
    QString m_interfaceName;
    QString m_interfaceVersion;
    QString m_qtSfwDirectory;

    QString m_upperCamelInterfaceName;
    QString m_lowerInterfaceName;
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
    QString m_nameSpace;
    QString m_serviceName;
    QString m_serviceDescription;

    QString m_allSignals;
    QString m_allConnectSignalCommands;

    QStringList m_arguments;

    bool m_createQtSfw;
    bool m_createAdaptor;
    bool m_createProxy;

    QString m_classDoc;
    QHash<int, QString> m_methodDocs;

    bool m_needsMApplication;

    bool m_oldUsage;
};

Worker::Worker()
{
    m_chainTag = "CHAINTASK";
    m_asyncTag = "ASYNCTASK";
    m_docTag   = "DOCTAG";

    m_createQtSfw = false;
    m_createAdaptor = false;
    m_createProxy = false;

    m_needsMApplication = false;

    m_oldUsage = true;
}

bool Worker::hasNameSpace()
{
    return !m_nameSpace.isEmpty();
}

QString Worker::nameSpace()
{
    return m_nameSpace;
}

void Worker::setNameSpace( const QString &nameSpace )
{
    m_nameSpace = nameSpace;
}

QString Worker::serviceName()
{
    return m_serviceName;
}

QString Worker::servicePluginCppFileName()
{
    QString retVal = m_serviceName+"plugin.cpp";
    return retVal;
}

QString Worker::serviceProjectFileName()
{
    QString retVal = qtSfwDirectory()+".pro";

    return retVal;
}

QString Worker::servicePluginHeaderFileName()
{
    QString retVal = m_serviceName+"plugin.h";
    return retVal;
}

QString Worker::serviceXmlFileName()
{
    QString retVal = m_serviceName+".xml";
    return retVal;
}

QString Worker::upperCamelServiceName()
{
    QString retVal = m_serviceName.split(".").last();
    return retVal;
}

QString Worker::upperServiceName()
{
    QString retVal = m_serviceName.toUpper();
    return retVal;
}

QString Worker::lowerServiceName()
{
    QString retVal = m_serviceName.toLower();
    return retVal;
}

void Worker::setServiceName( const QString &serviceName )
{
    m_serviceName = serviceName;
}

QString Worker::serviceDescription()
{
    return m_serviceDescription;
}

QString Worker::qtSfwPluginPath()
{
    return QString( "/usr/lib/maemo-meegotouch-services" );
}

void Worker::setServiceDescription( const QString &serviceDescription )
{
    m_serviceDescription = serviceDescription;
}

void Worker::setArguments( const int argc, const char * const argv[] )
{
  for ( int argIndex=0; argIndex<argc; ++argIndex ) {
    m_arguments << argv[ argIndex ];
  }
}

QString Worker::interfaceVersion()
{
    return m_interfaceVersion;
}

QString Worker::qtSfwDirectory()
{
    QString retVal = m_qtSfwDirectory;

    if ( retVal.isEmpty() ) {
        retVal = "qtsfw";
    }

    return retVal;
}

QString Worker::interfaceDescription()
{
    QString retVal = classDoc();

    QStringList lines = retVal.split( "\n" );
    lines = lines.filter( "\"brief\"" );

    if ( lines.count() > 0 ) {
        retVal = lines.at(0);

        {
            QRegExp xmlCode( "<[^>]*>" );
            QRegExp beginSpaces( "^\\s*" );
            QRegExp endSpaces( "'\\s*$" );
            retVal = retVal.remove( xmlCode );
            retVal = retVal.remove( beginSpaces );
            retVal = retVal.remove( endSpaces );
        }
    }

    return retVal;
}

QString Worker::interfaceName()
{
    return m_interfaceName;
}

void Worker::setInterfaceName( const QString& interfaceName )
{
    QString pid = QString::number(QCoreApplication::applicationPid());

    m_interfaceName = interfaceName;

    m_upperCamelInterfaceName = interfaceName.split(".").last();
    m_lowerInterfaceName      = m_upperCamelInterfaceName.toLower();

    m_xmlFileName           = interfaceName + ".xml";
    m_newXmlFileName        = interfaceName + "-" + pid + ".xml";

    // if -p
    m_upperCamelProxyName      = m_upperCamelInterfaceName + "Proxy";
    m_proxyBase                = m_lowerInterfaceName + "proxy";
    m_proxyCppFileName         = m_proxyBase + ".cpp";
    m_proxyHeaderFileName      = m_proxyBase + ".h";
    m_newProxyHeaderFileName   = m_proxyBase + "-" + pid + ".h";
    m_newProxyCppFileName      = m_proxyBase + "-" + pid + ".cpp";
    m_wrapperHeaderFileName    = m_lowerInterfaceName + ".h";
    m_wrapperCppFileName       = m_lowerInterfaceName + ".cpp";
    m_wrapperHeaderGuard       = m_wrapperHeaderFileName.toUpper().replace(".", "_");

    // if -a
    m_upperCamelAdaptorName     = m_upperCamelInterfaceName + "Adaptor";
    m_adaptorBase               = m_lowerInterfaceName + "adaptor";
    m_adaptorCppFileName        = m_adaptorBase + ".cpp";
    m_adaptorHeaderFileName     = m_adaptorBase + ".h";
    m_newAdaptorCppFileName     = m_adaptorBase + "-" + pid + ".cpp";
    m_newAdaptorHeaderFileName  = m_adaptorBase + "-" + pid + ".h";
}

void Worker::setQtSfwDirectory( const QString& directory )
{
    m_qtSfwDirectory = directory;
}

void Worker::setInterfaceVersion( const QString& interfaceVersion )
{
    m_interfaceVersion = interfaceVersion;
}

void Worker::setAllSignals( const QString& val )
{
    m_allSignals = val;
}

void Worker::setAllConnectSignalCommands( const QString& val )
{
    m_allConnectSignalCommands = val;
}

void Worker::setCreateQtSfw( bool val )
{
    m_createQtSfw = val;
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

void Worker::setNeedsMApplication( bool val )
{
    m_needsMApplication = val;
}

QString Worker::applicationName()
{
  return m_arguments[0];
}

QString Worker::generatedByComment()
{
  QStringList lines;

  lines
    << " * automatically generated with the command line :"
    << " * "+m_arguments.join( " " );

  return lines.join( "\n" );
}

QString Worker::upperCamelInterfaceName()
{
    return m_upperCamelInterfaceName;
}

QString Worker::lowerInterfaceName()
{
    return m_lowerInterfaceName;
}

QString Worker::upperInterfaceName()
{
    QString retVal = m_lowerInterfaceName.toUpper();
    return retVal;
}

void Worker::setXmlFileName( const QString& xmlFileName )
{
    m_xmlFileName = xmlFileName;
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

QRegExp Worker::qdbusxml2cppRegExp()
{
    return QRegExp( "qdbusxml2cpp(?: version \\d+\\.\\d+)*" );
}

bool Worker::createQtSfw()
{
    return m_createQtSfw;
}

bool Worker::createAdaptor()
{
    return m_createAdaptor;
}

bool Worker::createProxy()
{
    return m_createProxy;
}

bool Worker::needsMApplication()
{
    return m_needsMApplication;
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

    QHash<const QString, QStringList> xmlTokens;

    doc.setContent(xml, true, &errorStr, &errorLine, &errorColumn);

    QDomNode node = doc.documentElement();
    QDomElement el = node.toElement();
    el = el.firstChildElement( "arg" );

    while ( ! el.isNull() ) {
        QString attribute = el.attribute( "tag" );
        QString text          = el.text();
        xmlTokens[ attribute ] << text;
        el = el.nextSiblingElement( "arg" );
    }

    QStringList lines;

    QString indent;
    indent.fill( ' ', indentCount );


    QString result;
    QTextStream s( &result );

    QStringList doxTokens;
    doxTokens << "class" << "brief" << "param" << "return" << "details" << "state" << "ingroup";

    s << "\n" << indent << "/**\n";

    bool hasDoc = false;

    foreach( const QString dT, doxTokens ) {

        if ( !xmlTokens.value( dT ).isEmpty() ) {
            hasDoc = true;

            foreach( const QString xT, xmlTokens[dT]) {
                QString longLine = "@" + dT + " " + xT;

                while ( !longLine.isEmpty() ) {
                    int pos = longLine.indexOf( ' ', 60 - indentCount );

                    if ( pos == -1 ) {
                        pos = longLine.length();
                    }

                    QString line = longLine.left( pos );
                    longLine = longLine.mid( pos );
                    s << indent << " * " << line << "\n";
                }
            }
        }
    }

    s << indent << " */\n";

    if ( hasDoc ) {
        return result;
    } else {
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

QString Worker::topBitHforQtSfw()
{
    return
"#ifndef " + wrapperHeaderGuard() + "\n\
#define " + wrapperHeaderGuard() + "\n\
\n\
/*\n"
  + generatedByComment() + "\n\
 */\n\
\n\
#include <QObject>\n\
#include <QVariantList>\n\
#include <QString>\n\
#include <QStringList>\n\
\n\
class " + upperCamelInterfaceName() + "Proxy;\n\
";
}

QString Worker::topBitH()
{
    return
"#ifndef " + wrapperHeaderGuard() + "\n\
#define " + wrapperHeaderGuard() + "\n\
\n\
/*\n"
  + generatedByComment() + "\n\
 */\n\
\n\
#include <QObject>\n\
\n\
#include <mservicefwbaseif.h>\n";
}

QString Worker::middleBitHforQtSfw()
{
    return
"class " + upperCamelInterfaceName() + " : public QObject\n\
{\n\
Q_OBJECT\n\
\n\
public:\n";
}

QString Worker::middleBitH()
{
    return
"class " + upperCamelInterfaceName() + " : public MServiceFwBaseIf\n\
{\n\
Q_OBJECT\n\
\n\
public:\n";
}

QString Worker::botBitHforQtSfw()
{
    QString retVal =
"\n\
public:\n\
    " + upperCamelInterfaceName() + "( QObject* parent = 0 );\n\
\n\
Q_SIGNALS:\n\
" + m_allSignals + "\n\
private:\n\
    " + upperCamelInterfaceName() + "Proxy *interfaceProxy;\n\
};\n";

    if (hasNameSpace()) {
        retVal += "\n}; // namespace\n";
    }

    retVal += "#endif\n";

  return retVal;
}

QString Worker::botBitH()
{
    QString retVal =
"\n\
public:\n\
    /*!\n\
     * @brief Constructs a base interface\n\
     * @param preferredService the preferred service provider. Leave\n\
     * empty if no preferred provider. In most cases, this should be left\n\
     * empty.\n\
     * @param parent Parent object\n\
     */\n\
\n\
    " + upperCamelInterfaceName() + "( const QString& preferredService = \"\", QObject* parent = 0 );\n\
\n\
    /*!\n\
     * @brief Set the service name\n\
     * @param service Name of the desired service\n\
     */\n\
\n\
    void setService(const QString & service);\n\
Q_SIGNALS:\n\
" + m_allSignals + "\n\
};\n";

    if (hasNameSpace()) {
        retVal += "\n}; // namespace\n";
    }

    retVal += "#endif\n";

  return retVal;
}

QString Worker::topBitCforQtSfw()
{
    return topBitC();
}

QString Worker::topBitC()
{
    QString retVal;
    QString commandLine(QCoreApplication::arguments().join(" "));

    retVal =
"/*\n\
 * automatically generated with the command line :\n\
 * " + commandLine + "\n\
 */\n\
\n\
#include \"" + wrapperHeaderFileName() + "\"\n\
#include \"" + proxyHeaderFileName() + "\"\n";

    if ( hasNameSpace() ) {
        retVal += "\nnamespace " + nameSpace() + " {\n";
    }

    return retVal;
}

QString Worker::botBitCforQtSfw()
{
    QString retVal;

    retVal =
"" + upperCamelInterfaceName() + "::" + upperCamelInterfaceName() + "( QObject* parent )\n\
{\n\
    interfaceProxy = new " + upperCamelInterfaceName() + "Proxy( \"" + serviceName() + "\", \"/\", QDBusConnection::sessionBus(), this );\n\
\n\
"+m_allConnectSignalCommands + "\n\
}\n";

    if (hasNameSpace()) {
        retVal += "\n}; // namespace";
    }

    return retVal;
}

QString Worker::botBitC()
{
    QString retVal;

    retVal =
"" + upperCamelInterfaceName() + "::" + upperCamelInterfaceName() + "( const QString& preferredService, QObject* parent )\n\
    : MServiceFwBaseIf( " + upperCamelProxyName() + "::staticInterfaceName(), parent )\n\
{\n\
    // Resolve the provider service name\n\
    QString service = resolveServiceName( interfaceName(), preferredService );\n\
\n\
    bool serviceNameInvalid = service.contains( \" \" ); // \"not provided\" - when the service wouldn't run\n\
    if ( serviceNameInvalid ) {\n\
        service.clear();\n\
    }\n\
    setServiceName( service );\n\
\n\
    if (!service.isEmpty()) {\n\
        // Construct the D-Bus proxy\n\
        setInterfaceProxy( new " + upperCamelProxyName() + "( service, \"/\", QDBusConnection::sessionBus(), this ));\n\
        // allConnectSignals go here (empty block if none)\n\
" + m_allConnectSignalCommands + "\n\
    }\n\
}\n\
\n\
void " + upperCamelInterfaceName() + "::setService(const QString & service)\n\
{\n\
    if (service.isEmpty()) return;\n\
\n\
    setServiceName( service );\n\
    setInterfaceProxy( new " + upperCamelProxyName() + "( service, \"/\", QDBusConnection::sessionBus(), this ));\n\
    {\n\
" + m_allConnectSignalCommands + "\n\
    }\n\
}\n";

    if (hasNameSpace()) {
        retVal += "\n}; // namespace";
    }

    return retVal;
}

QString Worker::mHeaders()
{
    return
"\
#include <Qt> // for Qt::HANDLE\n\
#include <MApplication>\n\
#include <MWindow>\
";
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

    if (!doc.setContent(&inFile, true, &errorStr, &errorLine, &errorColumn)) {
        qWarning( "doc.setContent failed" );
        return;
    }

    // here we go from <node> to <interface>,
    // then we save and strip the <doc> childElement, if it exists,
    // then we walk over the methods,
    // and save the doc for them,
    // and mangle the doctag, chaintasktag and asynctag strings into the method names

    QDomNode node = doc.documentElement();
    if ( node.nodeName() != "node" ) {
        qCritical( "Error: this is not a valid interface. exiting" );
        exit( -1 );
    }

    QDomElement el = node.toElement();

    el = el.firstChildElement( "interface" );

    // try to save and drop <doc> child of <interface>
    if ( !el.firstChildElement( "doc" ).isNull() ) {
        QString string;
        QTextStream stream( &string );
        el.firstChildElement( "doc" ).save( stream, 4 );

        setClassDoc( string );

        // mangle doc tag into class name
        el.setAttribute( "name", el.attribute( "name" ) + docTag() );

        el.removeChild( el.firstChildElement( "doc" ) );
    }

    // try to save and drop <namespace> child of <interface>
    if ( !el.firstChildElement( "annotation" ).isNull() ) {
        const QDomElement &annotation = el.firstChildElement( "annotation" );
        if ( annotation.attribute( "name" ) == "namespace" ) {
            setNameSpace( annotation.attribute( "value" ).split(".").last() );
        }
    }

    QDomElement start = el;

    // walk over the methods and signals
    foreach( const QString methSig, QStringList() << "signal" << "method"  ) {
        el = start.firstChildElement( methSig );

        while ( ! el.isNull() ) {
            // now handle chainTask and asyncTask for this method
            if ( el.attribute( "chainTask" ) == "true" ) {
                el.setAttribute( "name", el.attribute( "name" ) + chainTag() );
                setNeedsMApplication( true );
            }

            if ( el.attribute( "asyncTask" ) == "true" ) {
                el.setAttribute( "name", el.attribute( "name" ) + asyncTag() );
                setNeedsMApplication( true );
            }

            // handle doc
            if ( ! el.firstChildElement( "doc" ).isNull() ) {
                QString name = el.attribute( "name" );
                QString string;
                QTextStream stream( &string );
                el.firstChildElement( "doc" ).save( stream, 4 );

                ++docTagNo;
                setMethodDoc( docTagNo, string );

                // mangle doc tag into class name
                el.setAttribute( "name", name + docTag() + QString::number(docTagNo) );

                el.removeChild( el.firstChildElement( "doc" ) );
            }


            el = el.nextSiblingElement( methSig );
        }

    }

    outFile.write( qPrintable( doc.toString( 4 ) ) );
}


void Worker::createConnectSignalCommands( const QStringList& ifSignals, bool forQtSfw )
{
    // process signals, removing void and param names
    QStringList connectSignals;
    for (int thisSignalIndex = 0; thisSignalIndex < ifSignals.size(); ++thisSignalIndex) {
        QString thisSignal = ifSignals.at(thisSignalIndex);

        // need the last line to chop off the documentation
        thisSignal = thisSignal.split( "\n" ).last();

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

        if ( forQtSfw ) {
            connectSignals.append(
"\
    connect( interfaceProxy, SIGNAL( " + signalName + "( " + joinedTypes + " ) ),\n\
             this, SIGNAL( " + signalName + "( " + joinedTypes + " ) ) );\n\
");
        } else {
            connectSignals.append(
"\
    connect( interfaceProxy(), SIGNAL( " + signalName + "( " + joinedTypes + " ) ),\n\
             this, SIGNAL( " + signalName + "( " + joinedTypes + " ) ) );\n\
");
        }

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

QString stripOffDefaultValue(const QString &parameters)
{
    QString retVal;

    QString myParameters = parameters;

    bool startsWithSpace =  myParameters.startsWith(" ");
    if ( startsWithSpace ) {
        myParameters.replace( QRegExp( "^ " ), "" );
    }

    bool endsWithSpace = myParameters.endsWith(" ");
    if ( endsWithSpace ) {
        myParameters.replace( QRegExp( " $" ), "" );
    }

    QStringList typesAndNamesWithoutDefaults;
    QStringList typesAndNames = myParameters.split(QRegExp("\\s*,\\s*"));

    for (int typeAndNameIndex = 0; typeAndNameIndex < typesAndNames.size(); ++typeAndNameIndex) {
        QString typeAndName(typesAndNames[ typeAndNameIndex ]);

        // strip off default value for parameter, if there is one
        if ( typeAndName.contains( "=" ) ) {
            typeAndName = typeAndName.split("=").first();
        }

        typesAndNamesWithoutDefaults << typeAndName;
    }

    retVal = typesAndNamesWithoutDefaults.join(", ");

    if (startsWithSpace) {
        retVal = " "+retVal;
    }
    if (endsWithSpace) {
        retVal = retVal+" ";
    }

    return retVal;
}

QStringList getParamNames(const QString &parameters)
{
    QStringList retVal;

    QStringList typesAndNames = parameters.split(",");

    for (int typeAndNameIndex = 0; typeAndNameIndex < typesAndNames.size(); ++typeAndNameIndex) {
        QString typeAndName(typesAndNames[ typeAndNameIndex ]);

        typeAndName = stripOffDefaultValue( typeAndName );

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
    QString previousLine;

    bool inChainTask      = false;

    while (!inS.atEnd()) {
        QString line = inS.readLine();
        line.remove(QRegExp("\n$"));

        // remove doctag - can be more than one per line
        w.removeDocTag( line );

        line.replace( w.qdbusxml2cppRegExp(), w.applicationName() );
        line.replace( w.newXmlFileName(), w.xmlFileName() );

        if (w.hasNameSpace() && previousLine == "#include <QtCore/QVariant>" ) {
                // this is the blank line after the last #include class declarations, before the beginning of the main class definition
                outS << endl;
                outS << "namespace " << w.nameSpace() << " {" << endl;
        }

        // add chaining code to NEWADAPTOR
        {
            // always remove asyncTag, since we don't care about it in the cpp file
            line.remove(w.asyncTag());

            if (w.needsMApplication() && line.contains(QRegExp("#include <QtCore/Q(Meta)?Object>"))) {
                outS <<
                    "\n"                        \
                    "#include <MComponentData>\n" \
                    "#include <MDebug>\n"                    \
                    "\n"                                     \
                    + line + "\n";
            } else if (inChainTask) {
                line.remove(w.chainTag());
                if (line == "{") {
                    outS << line << endl
                         << "    MComponentData::ChainData thisData( _windowId, _taskTitle );" << endl
                         << "    MComponentData::pushChainData( thisData );" << endl
                         << endl;
                } else if (line.contains("return") || line == "}") {
                    // match end of function - need to add the connect *before* the return, if there is one
                    inChainTask = false;
                    outS << line << endl;
                } else {
                    outS << line << endl;
                }
            } else if (line.contains(w.chainTag())) {
                line.remove(w.chainTag());
                QString parameterString = "(const uint _windowId, const QString _taskTitle";

                bool methodHasParameters = !line.contains( QRegExp( "\\(\\s*\\)" ));
                if ( methodHasParameters ) {
                    parameterString += ", ";
                }
                line.replace("(", parameterString);

                outS << line << endl;
                inChainTask = true;
            } else if (line.contains("This file was generated by")) {
              outS << w.generatedByComment() << endl;
            } else if (line.contains( "Command line was:")) {
              // do nothing - the replacement for this line is output by the above
            } else {
                outS << line << endl;
            }
        }

        previousLine = line;
    }

    if (w.hasNameSpace()) {
        outS << endl;
        outS << "}; // namespace" << endl;
    }

    newAdaptorCppFile.close();
    adaptorCppFile.close();

    // move new adaptor header file (with chain parameters added)
    // to replace one produced by qdbusxml2cpp
    adaptorCppFile.remove();
    newAdaptorCppFile.rename(w.adaptorCppFileName());
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

    QString previousLine;

    while (!adaptorHeaderFileStream.atEnd()) {
        QString line = adaptorHeaderFileStream.readLine();
        line.remove(QRegExp("\n$"));

        // remove doctag - can be more than one per line
        w.removeDocTag( line );

        line.replace( w.qdbusxml2cppRegExp(), w.applicationName() );

        // add chaining code to NEWADAPTOR
        {
            if (w.hasNameSpace()) {
                if ( previousLine.startsWith( "class" ) && previousLine.endsWith( ";" )
                    && line.isEmpty() ) {
                    // this is the blank line after the forward class declarations, before the beginning of the main class definition
                    newAdaptorHeaderStream << line << endl;
                    newAdaptorHeaderStream << "namespace " << w.nameSpace() << " {" << endl;
                } else if (line == "#endif") {
                    newAdaptorHeaderStream << "}; // namespace" << endl << endl;
                }
            }

            if (line.contains(QRegExp("chainTask=\\\\\"\\w+\\\\\"")) || line.contains(QRegExp("asyncTask=\\\\\"\\w+\\\\\""))) {           // process comment - remember the backslashes are in the source too, hence so many of them
                // remove asyncTask attribute
                {
                    QRegExp matchThis("asyncTask=\\\\\"\\w+\\\\\"\\s*");
                    if (line.contains(matchThis)) {
                        line.remove(matchThis);
                        line.remove(w.asyncTag());
                    }
                }

                // remove asyncTask attribute
                bool chainTaskHasNoParameters = false;
                bool isChainTask = false;
                {
                    QRegExp matchThis("chainTask=\\\\\"(\\w+)\\\\\"\\s*");
                    if (line.contains(matchThis)) {
                        line.remove(matchThis);
                        isChainTask = matchThis.cap(1).contains("true");
                        line.remove(w.chainTag());
                        if ( line.contains( "/>") ) {
                            chainTaskHasNoParameters = true;
                            line.replace( "/>", ">" ); // if the method has no args, it'll have a />, but we will add args, so we don't want this
                        } else {
                            chainTaskHasNoParameters = false;
                        }
                    }
                }

                // remove extraneous spaces (inserted randomly by qdbusxml2cpp?)
                line.replace(QRegExp("\\s+(/*>)"), "\\1");

                newAdaptorHeaderStream << line << endl;

                // this has to be printed after the method tag, above
                if (isChainTask) {
                    newAdaptorHeaderStream
                        << "\"      <arg direction=\\\"in\\\" type=\\\"u\\\" name=\\\"_windowId\\\"/>\\n\"" << endl
                        << "\"      <arg direction=\\\"in\\\" type=\\\"s\\\" name=\\\"_taskTitle\\\"/>\\n\"" << endl;
                    if ( chainTaskHasNoParameters ) {
                        newAdaptorHeaderStream
                            << "\"    </method>\\n\"" << endl;
                    }
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
                    line.remove(w.chainTag());

                    QString parameterString = "(const uint _windowId, const QString _taskTitle";

                    bool methodHasParameters = !line.contains( QRegExp( "\\(\\s*\\)" ));
                    if ( methodHasParameters ) {
                        parameterString += ", ";
                    }
                    line.replace("(", parameterString);
                }

                newAdaptorHeaderStream << line << endl;
            } else if (line.contains("This file was generated by")) {
              newAdaptorHeaderStream << w.generatedByComment() << endl;
            } else if (line.contains( "Command line was:")) {
              // do nothing - the replacement for this line is output by the above
            } else {
                // remove extraneous spaces (inserted randomly by qdbusxml2cpp?)
                line.replace(QRegExp("\\s+(/*>)"), "\\1");

                newAdaptorHeaderStream << line << endl;
            }
        }

        previousLine = line;
    }

    newAdaptorHeaderFile.close();
    adaptorHeaderFile.close();

    // move new adaptor header file (with chain parameters added)
    // to replace one produced by qdbusxml2cpp
    adaptorHeaderFile.remove();
    newAdaptorHeaderFile.rename(w.adaptorHeaderFileName());
}

// this method generates the code that is needed for chaining of tasks
void doChainTaskHandling( QString &line, bool& inChainTask, QTextStream& newProxyHeaderStream, bool forQtSfw )
{
    if (inChainTask) {
        if (line.contains("QList<QVariant> argumentList;"))
        {
            if ( forQtSfw ) {
                newProxyHeaderStream << line + "\n\
        argumentList << qVariantFromValue(_parentWindowId);\n\
        argumentList << qVariantFromValue(_taskTitle);\n";
            } else {
                newProxyHeaderStream << "\
        Qt::HANDLE windowId = 0;\n\
\n\
        if ( MApplication::instance() )\n\
        {\n\
            MWindow *win = MApplication::instance()->activeWindow();\n\
            if ( win ) {\n\
                windowId = win->effectiveWinId();\n\
            }\n\
        }\n\
        else\n\
        {\n\
            QWidget *widget = qApp->activeWindow();\n\
            if ( widget ) {\n\
                windowId = widget->effectiveWinId();\n\
            }\n\
        }\n\
\n\
" + line + "\n\
        argumentList << qVariantFromValue((uint)windowId);\n\
        argumentList << qVariantFromValue(_taskTitle);\n";
            }
        }
        else if (line.contains("return")) {
            line.remove(w.chainTag());
            newProxyHeaderStream << line << endl;
            inChainTask = false;
        }
        else {
            newProxyHeaderStream << line << endl;
        }
    } else if (line.contains(w.chainTag())) {
        line.remove(w.chainTag());
        QString outputThisLine = line;

        bool hasNoParams = line.endsWith( "()" );

        if ( forQtSfw ) {
            if ( hasNoParams ) {
                outputThisLine.replace( ")", "const uint _parentWindowId, const QString &_taskTitle)" );
            } else {
                outputThisLine.replace( ")", ", const uint _parentWindowId, const QString &_taskTitle)" );
            }
        } else {
            if ( hasNoParams ) {
                outputThisLine.replace( ")", "const QString &_taskTitle)" );
            } else {
                outputThisLine.replace( ")", ", const QString &_taskTitle)" );
            }
        }
        newProxyHeaderStream << outputThisLine << "\n";
        inChainTask = true;
    } else {
        line.remove( w.chainTag() );

        newProxyHeaderStream << line << "\n";
    }
}


void processProxyHeaderFile( bool forQtSfw=false )
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
    QString previousLine;

    if ( forQtSfw ) {
        wrapperCppStream    << w.topBitCforQtSfw() << endl;
        wrapperHeaderStream << w.topBitHforQtSfw() << endl;
    } else {
        wrapperCppStream    << w.topBitC() << endl;
        wrapperHeaderStream << w.topBitH() << endl;
    }

    if ( w.hasNameSpace() ) {
        wrapperHeaderStream << "namespace " << w.nameSpace() << " {" << endl;
        wrapperHeaderStream << endl;
    }

    QStringList ifSignals;

    bool inSignalSection = false;
    bool inChainTask     = false;

    while ( ! proxyHeaderStream.atEnd() ) {
        QString line = proxyHeaderStream.readLine();

        line.replace( w.qdbusxml2cppRegExp(), w.applicationName() );

        if (w.hasNameSpace()) {
            if ( previousLine.contains(QRegExp("#include <QtDBus/QtDBus>")) ) {
                // this is the blank line after the forward class declarations, before the beginning of the main class definition
                newProxyHeaderStream << line << endl;
                newProxyHeaderStream << "namespace " << w.nameSpace() << " {" << endl;
            }
            else if (line == "#endif" && proxyHeaderStream.atEnd() ) {
                newProxyHeaderStream << endl;
                newProxyHeaderStream << "}; // namespace" << endl;
                newProxyHeaderStream << endl;
            }
        }

        if ( w.hasNameSpace() ) {
            line.replace( "typedef ::", "typedef ::"+w.nameSpace()+"::" );
        }

        // add documentation and remove doc tags here
        // note that middle bit is added here too, so it's not just about doc
        if (line.contains(QRegExp("^class"))) {
            if ( forQtSfw ) {
                // don't bother with documentation in the qtsfw header file
                wrapperHeaderStream << w.middleBitHforQtSfw();
            } else {
                wrapperHeaderStream << w.mangledClassDoc();
                wrapperHeaderStream << w.middleBitH();
            }
        } else if (line.contains( "Command line was:")) {
            // do nothing - the replacement for this line is output by the above
            line.chop(1); // remove '\n'
        } else if ( line.contains( w.docTag() ) && !inSignalSection ) {
            // we have to handle doxygen doc here.
            // we have three cases:
            // - class documentation
            // - method documentation
            // - a docTag, that can be ignored, because
            //   it appears somewhere in a method implementation

            if ( ! line.contains( "return" ) ) {
                // method docs
                QRegExp rx( w.docTag() + "(\\d+)" );

                // do the match
                if ( rx.indexIn( line ) != -1) {
                    int id = rx.cap( 1 ).toInt();

                    if ( id > 0 && !forQtSfw ) {
                        wrapperHeaderStream << w.mangledMethodDoc( id );
                    }
                }
            }
        }

        line.remove(w.asyncTag());

        if (w.needsMApplication()) {
            if (line.contains(QRegExp("#include <QtDBus/QtDBus>")) && !forQtSfw ) {
                line += QString( "\n\n" ) += w.mHeaders();
            } else if (line.contains(w.newXmlFileName())) {
                line.replace(w.newXmlFileName(), w.xmlFileName());
                line += "\n";
            }
        }

        int docId=0;
        if (inSignalSection) {
            // method docs
            QRegExp rx( w.docTag() + "(\\d+)" );

            // do the match
            if ( rx.indexIn( line ) != -1) {
                docId = rx.cap( 1 ).toInt();
            }
        }

        // remove docTag with possible number suffix
        w.removeDocTag( line );

        // add chaining code to NEWPROXY
        doChainTaskHandling( line, inChainTask, newProxyHeaderStream, forQtSfw );

        if (inSignalSection) {
            bool atEndOfSignalSection = (line == "};");
            if (atEndOfSignalSection) {
                inSignalSection = false;
            } else {
                QString lineWithDoc = "";

                if ( docId > 0 ) {
                    lineWithDoc = w.mangledMethodDoc( docId );
                }

                lineWithDoc += line;

                ifSignals.append(lineWithDoc);
            }
        } else {

            if (line.contains("Q_SIGNALS:")) {
                inSignalSection = true;
            } else {
                QRegExp methodPrototype("inline\\s+QDBusPendingReply<([^>]*)>\\s*(\\w+)\\(([^)]*)\\)");

                if (line.contains(methodPrototype)) {
                    QString returnType = methodPrototype.cap(1);
                    QString methodName = methodPrototype.cap(2);
                    QString parameters = methodPrototype.cap(3);

                    if (returnType.isEmpty()) {
                        returnType = "void";
                    }

                    if ( inChainTask ) {
                        QStringList paramNames = getParamNames(parameters);

                        QString paramString;
                        if ( parameters.isEmpty() ) {
                            paramString = "";
                        } else {
                            paramString = " "+parameters+" ";
                        }

                        if ( forQtSfw ) {
                            wrapperHeaderStream <<
                                "    Q_INVOKABLE " + returnType + " " + methodName + "(" + paramString + ");" << endl;
                        } else {
                            wrapperHeaderStream <<
                                "    " + returnType + " " + methodName + "(" + paramString + ");" << endl;
                        }

                        wrapperCppStream <<
                            returnType + " " + w.upperCamelInterfaceName() + "::" + methodName + "(" + paramString + ")" << endl;

                        wrapperCppStream << "{" << endl;

                        if ( forQtSfw ) {
                            if ( parameters.isEmpty() ) {
                                paramString = " 0, QString() ";
                            } else {
                                paramString = " " + paramNames.join(", ") + ", 0, QString() ";
                            }
                        } else {
                            if ( parameters.isEmpty() ) {
                                paramString = " QString() ";
                            } else {
                                paramString = " " + paramNames.join(", ") + ", QString() ";
                            }
                        }

                        if (returnType == "void") {
                            wrapperCppStream <<
    "    " + methodName + "(" + paramString + ");" << endl;
                        } else {
                            wrapperCppStream <<
    "    return " + methodName + "(" + paramString + ");" << endl;
                        }

                        wrapperCppStream << "}\n" << endl;

                        if ( !parameters.isEmpty() ) {
                            parameters += ", ";
                        }

                        if ( forQtSfw ) {
                            parameters += "const uint _parentWindowId, const QString &_taskTitle";
                        } else {
                            parameters += "const QString &_taskTitle";
                        }
                    }

                    QStringList paramNames = getParamNames(parameters);

                    QString paramString;
                    if ( parameters.isEmpty() ) {
                        paramString = "";
                    } else {
                        paramString = " "+parameters+" ";
                    }

                    if ( forQtSfw ) {
                        wrapperHeaderStream <<
                            "    Q_INVOKABLE " + returnType + " " + methodName + "(" + paramString + ");" << endl;
                    } else {
                        wrapperHeaderStream <<
                            "    " + returnType + " " + methodName + "(" + paramString + ");" << endl;
                    }

                    wrapperCppStream <<
                        returnType + " " + w.upperCamelInterfaceName() + "::" + methodName + "(" + paramString + ")" << endl;

                    wrapperCppStream << "{" << endl;

                    if ( parameters.isEmpty() ) {
                        paramString = "";
                    } else {
                        paramString = " " + paramNames.join(", ") + " ";
                    }

                    if (returnType == "void") {
                        if ( forQtSfw ) {
                            wrapperCppStream <<
                                "    interfaceProxy->" + methodName + "(" + paramString + ");" << endl;
                        } else {
                            wrapperCppStream <<
                                "    static_cast<" + w.upperCamelInterfaceName() + "Proxy*>(interfaceProxy())->" + methodName + "(" + paramString + ");" << endl;
                        }
                    } else {
                        if ( forQtSfw ) {
                            wrapperCppStream <<
                                "    return interfaceProxy->" + methodName + "(" + paramString + ").value();" << endl;
                        } else {
                            wrapperCppStream <<
                                "    return qobject_cast<" + w.upperCamelInterfaceName() + "Proxy*>(interfaceProxy())->" + methodName + "(" + paramString + ").value();" << endl;
                        }
                    }

                    wrapperCppStream << "}\n" << endl;
                }
            }
        }

        previousLine = line;
    } // ! proxyHeaderStream.atEnd()


    w.setAllSignals( ifSignals.join("\n") );
    w.createConnectSignalCommands( ifSignals, forQtSfw );

    if ( forQtSfw ) {
        wrapperCppStream << w.botBitCforQtSfw() << endl;
        wrapperHeaderStream << w.botBitHforQtSfw() << endl;
    } else {
        wrapperCppStream << w.botBitC() << endl;
        wrapperHeaderStream << w.botBitH() << endl;
    }

    // mv new proxy header file (with chain parameters added)
    // to replace one produced by qdbusxml2cpp
    proxyHeaderFile.remove();
    newProxyHeaderFile.rename(w.proxyHeaderFileName());
}

void processProxyCppFile( bool forQtSfw=false )
{
    Q_UNUSED( forQtSfw );
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

    while ( ! inS.atEnd() ) {
        QString line = inS.readLine();

        w.removeDocTag( line );

        line.remove( w.chainTag() );

        line.remove( w.asyncTag() );

        line.replace( "qdbusxml2cpp", w.applicationName() );
        line.replace( w.newXmlFileName(), w.xmlFileName() );

        if (line.contains("This file was generated by")) {
          outS << w.generatedByComment() << endl;
        } else if (w.hasNameSpace() && line.startsWith( "#include") ) {
          outS << line << endl;
          outS << endl;
          outS << "namespace " << w.nameSpace() << " {" << endl;
        } else if (line.contains( "Command line was:")) {
          // do nothing - the replacement for this line is output by the above
        } else {
          outS << line << "\n";
        }
    }

    if ( w.hasNameSpace() ) {
      outS << "}; // namespace" << endl;
    }

    inFile.remove();
    outFile.rename(w.proxyCppFileName());
}

void generateQSFWXmlFile()
{
    QString contents( "\
<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n\
<service>\n\
    <name>@@SERVICENAME@@</name>\n\
    <filepath>@@QTSFWPLUGINPATH@@/lib@@SERVICENAME@@.so</filepath>\n\
    <description>@@SERVICEDESCRIPTION@@</description>\n\
    <interface>\n\
        <name>@@INTERFACENAME@@</name>\n\
        <version>@@INTERFACEVERSION@@</version>\n\
        <description>@@INTERFACEDESCRIPTION@@</description>\n\
        <capabilities></capabilities>\n\
    </interface>\n\
</service>\
" );

    contents.replace( "@@SERVICENAME@@", w.serviceName() );
    contents.replace( "@@QTSFWPLUGINPATH@@", w.qtSfwPluginPath() );
    contents.replace( "@@SERVICEDESCRIPTION@@", w.serviceDescription() );
    contents.replace( "@@INTERFACENAME@@", w.interfaceName() );
    contents.replace( "@@INTERFACEVERSION@@", w.interfaceVersion() );
    contents.replace( "@@INTERFACEDESCRIPTION@@", w.interfaceDescription() );

    QFile outFile(w.serviceXmlFileName());
    if (!outFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qCritical("Could not open %s", qPrintable( outFile.fileName() ) );
        exit(-1);
    }
    QTextStream outS(&outFile);

    outS << contents;
}

void generateQSFWPluginCpp()
{
    QString contents( "\
#include <qserviceinterfacedescriptor.h>\n\
#include <qabstractsecuritysession.h>\n\
#include <qservicecontext.h>\n\
\n\
#include \"@@SERVICEPLUGINHEADERFILENAME@@\"\n\
#include \"@@WRAPPERHEADERFILENAME@@\"\n\
\n\
QObject* @@UPPERCAMELSERVICENAME@@Plugin::createInstance(const QServiceInterfaceDescriptor& descriptor, QServiceContext* context, QAbstractSecuritySession* session)\n\
{\n\
    Q_UNUSED(context);\n\
    Q_UNUSED(session);\n\
\n\
    if (descriptor.interfaceName() == \"@@INTERFACENAME@@\")\n\
        return new @@UPPERCAMELINTERFACENAME@@();\n\
    else\n\
        return 0;\n\
}\n\
\n\
Q_EXPORT_PLUGIN2(@@SERVICENAME@@, @@UPPERCAMELSERVICENAME@@Plugin)\n\
"
            );

    contents.replace( "@@INTERFACENAME@@", w.interfaceName() );
    contents.replace( "@@SERVICENAME@@", w.serviceName() );
    //contents.replace( "@@LOWERINTERFACENAME@@", w.lowerInterfaceName() );
    //contents.replace( "@@LOWERSERVICENAME@@", w.lowerServiceName() );
    contents.replace( "@@UPPERCAMELSERVICENAME@@", w.upperCamelServiceName() );
    contents.replace( "@@UPPERCAMELINTERFACENAME@@", w.upperCamelInterfaceName() );
    contents.replace( "@@SERVICEPLUGINHEADERFILENAME@@", w.servicePluginHeaderFileName() );
    contents.replace( "@@WRAPPERHEADERFILENAME@@", w.wrapperHeaderFileName() );

    QFile outFile( w.servicePluginCppFileName() );
    if (!outFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qCritical("Could not open %s", qPrintable( outFile.fileName() ) );
        exit(-1);
    }
    QTextStream outS(&outFile);

    outS << contents;
}

void generateQSFWPluginH()
{
    QString contents( "\
#ifndef @@UPPERSERVICENAME@@PLUGIN_H\n\
#define @@UPPERSERVICENAME@@PLUGIN_H\n\
\n\
#include <QObject>\n\
\n\
#include <qserviceplugininterface.h>\n\
\n\
QTM_USE_NAMESPACE\n\
\n\
class @@UPPERCAMELSERVICENAME@@Plugin : public QObject,\n\
    public QServicePluginInterface\n\
{\n\
Q_OBJECT\n\
    Q_INTERFACES(QtMobility::QServicePluginInterface)\n\
public:\n\
    QObject* createInstance(const QServiceInterfaceDescriptor& descriptor,\n\
            QServiceContext* context,\n\
            QAbstractSecuritySession* session);\n\
    };\n\
\n\
#endif\n\
"
            );

    contents.replace( "@@UPPERSERVICENAME@@", w.upperServiceName().replace( ".", "_" ) );
    contents.replace( "@@UPPERCAMELSERVICENAME@@", w.upperCamelServiceName() );

    QFile outFile( w.servicePluginHeaderFileName() );
    if (!outFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qCritical("Could not open %s", qPrintable( outFile.fileName() ));
        exit(-1);
    }
    QTextStream outS(&outFile);

    outS << contents;
}

#ifdef COMMENTEDOUT
void generateQSFWProxyCpp()
{
    QString contents( "\
#include \"@@LOWERINTERFACENAME@@proxy.h\"\n\
\n\
/*\n\
 * Implementation of interface class @@UPPERCAMELINTERFACENAME@@Proxy\n\
 */\n\
\n\
@@UPPERCAMELINTERFACENAME@@Proxy::@@UPPERCAMELINTERFACENAME@@Proxy(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent)\n\
    : QDBusAbstractInterface(service, path, staticInterfaceName(), connection, parent)\n\
{\n\
}\n\
\n\
@@UPPERCAMELINTERFACENAME@@Proxy::~@@UPPERCAMELINTERFACENAME@@Proxy()\n\
{\n\
}\n\
" );
    contents.replace( "@@LOWERINTERFACENAME@@", w.lowerInterfaceName() );
    contents.replace( "@@UPPERCAMELINTERFACENAME@@", w.upperCamelInterfaceName() );

    QFile outFile( w.servicePluginProxyCppFileName() );
    if (!outFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qCritical("Could not open %s", qPrintable( outFile.fileName() ));
        exit(-1);
    }
    QTextStream outS(&outFile);

    outS << contents;
}
#endif // 0

void generateQSFWProjectFile()
{
    QString contents( "\
TEMPLATE=lib\n\
CONFIG *= \\\n\
    plugin \\\n\
    mobility \\\n\
\n\
QT *= dbus\n\
\n\
TARGET = @@TARGET@@\n\
\n\
MOBILITY = serviceframework\n\
\n\
SOURCES *= \\\n\
    @@SOURCES@@ \\\n\
\n\
HEADERS *= \\\n\
    @@HEADERS@@ \\\n\
\n\
target.path = @@QTSFWPLUGINPATH@@\n\
\n\
xml.path  = @@QTSFWPLUGINPATH@@\n\
xml.files = @@SERVICEXMLFILENAME@@\n\
\n\
INSTALLS *= \\\n\
    xml\\\n\
    target\\\n\
" );

    QStringList sources;
    sources
        << w.servicePluginCppFileName()
        << w.wrapperCppFileName()
        << w.proxyCppFileName();

    QStringList headers;
    headers
        << w.servicePluginHeaderFileName()
        << w.wrapperHeaderFileName()
        << w.proxyHeaderFileName();

    contents.replace( "@@TARGET@@", w.serviceName() );
    contents.replace( "@@SOURCES@@", sources.join( " \\\n    " ) );
    contents.replace( "@@HEADERS@@", headers.join( " \\\n    " ) );
    contents.replace( "@@QTSFWPLUGINPATH@@", w.qtSfwPluginPath() );
    contents.replace( "@@SERVICEXMLFILENAME@@", w.serviceXmlFileName() );

    QFile outFile( w.serviceProjectFileName() );
    if (!outFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qCritical("Could not open %s", qPrintable( outFile.fileName() ));
        exit(-1);
    }
    QTextStream outS(&outFile);

    outS << contents;
}


void generateQSFWProject()
{
    generateQSFWPluginCpp();
    generateQSFWPluginH();
    generateQSFWProjectFile();
}

void runQDBusXml2Cpp(const QStringList &params)
{
    QProcess qdbusxml2cpp;
    qdbusxml2cpp.setWorkingDirectory(cwd);

    qdbusxml2cpp.start(command, params);
    if (!qdbusxml2cpp.waitForStarted()) {
        qCritical() << qdbusxml2cpp.error();
    }

    if (!qdbusxml2cpp.waitForFinished()) {
        qCritical() << "m-servicefwgen: " << command << "did not finish";
    }
}

void usage()
{
    qDebug() << "";
    qDebug() << "old usage: m-servicefwgen [-p interfaceName |-a interfaceName | -q interfaceName] -s serviceName -d serviceDescription -v serviceVersion";
    qDebug() << "";
    qDebug() << "new usage: m-servicefwgen [-p interfaceName |-a interfaceName | -q interfaceName] -s serviceName -d serviceDescription -v serviceVersion xmlFile";
    qDebug() << "";
    qDebug() << "       -a            generate adaptor files";
    qDebug() << "       -p            generate proxy files";
    qDebug() << "       -q            generate Qt SFW proxy files";
    qDebug() << "       -v            interface version for the Qt SFW service xml file";
    qDebug() << "       -s            service name for the Qt SFW service xml file";
    qDebug() << "       -d            service description for the Qt SFW service xml file";
    qDebug() << "       -D            directory for Qt SFW code (default:qtsfw)";
    qDebug() << "       -h            this help";
    qDebug() << "       xmlFile       path to the xml file for the interface";
    qDebug() << "";
    qDebug() << "Note: the old usage is deprecated. The reason for that is that we are guessing";
    qDebug() << "where we can find the xml file from the given interface names.";
    qDebug() << "Then with the old usage the program changed the directory to";
    qDebug() << "the path that was guessed. And then it created the adaptor,";
    qDebug() << "proxy or interface files in that directory.";
    qDebug() << "";
    qDebug() << "With the new usage, the program does not cd to somewhere any more";
    qDebug() << "at all. This way you can generate the needed files in the directory";
    qDebug() << "where you want to have them. Also the usage is quite similar to";
    qDebug() << "how the qdbusxml2cpp program from Qt is used.";
    qDebug() << "";
    qDebug() << "Example:";
    qDebug() << "    m-servicefwgen -a com.nokia.maemo.meegotouch.CameraInterface \\";
    qDebug() << "        /usr/share/dbus-1/interfaces/com.nokia.maemo.meegotouch.CameraInterface.xml";
    qDebug() << "";
    exit(1);
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    Q_UNUSED(app);
    w.setArguments( argc, argv );

    QString interfaceName("");

    QString xmlFilePath;

    if (argc == 1) {
        usage();
    }

    //foreach (QString arg, QStringList( argv ) ) {
    for (int argIndex = 1; argIndex < argc; ++argIndex) {
        QString arg(argv[ argIndex ]);

        if (arg == "-h") {
            usage();
        }
        else if (arg == "-a") {
            w.setCreateAdaptor( true );

            argIndex++;
            arg = argv[ argIndex ];

            QStringList bits = arg.split("/");

            interfaceName = bits.takeLast();
            QString interfacePath = bits.join("/");
            if (!interfacePath.isEmpty()) {
                cwd = interfacePath;
            }

            if ( w.createProxy() ) {
                qDebug() << "both -p and -a supplied";
                qDebug() << "disabling proxy generation";
                w.setCreateProxy( false );
            }
            else if ( w.createQtSfw() ) {
                qDebug() << "both -a and -q supplied - confused, exiting";
                exit(-1);
            }
        }
        else if (arg == "-p") {
            w.setCreateProxy( true );

            argIndex++;
            arg = argv[ argIndex ];

            QStringList bits = arg.split("/");

            interfaceName = bits.takeLast();
            QString interfacePath = bits.join("/");
            if (!interfacePath.isEmpty()) {
                cwd = interfacePath;
            }

            if ( w.createAdaptor() ) {
                qDebug() << "both -p and -a supplied";
                qDebug() << "disabling adaptor generation";
                w.setCreateAdaptor( false );
            }
            else if ( w.createQtSfw() ) {
                qDebug() << "both -p and -q supplied - confused, exiting";
                exit(-1);
            }
        }
        else if (arg == "-q") {
            w.setCreateQtSfw( true );

            argIndex++;
            arg = argv[ argIndex ];

            QStringList bits = arg.split("/");

            interfaceName = bits.takeLast();
            QString interfacePath = bits.join("/");
            if (!interfacePath.isEmpty()) {
                cwd = interfacePath;
            }

            if ( w.createAdaptor() ) {
                qDebug() << "both -q and -a supplied";
                qDebug() << "disabling adaptor generation";
                w.setCreateAdaptor( false );
            }
            else if ( w.createProxy() ) {
                qDebug() << "both -p and -q supplied - confused, exiting";
                exit(-1);
            }
        }
        else if (arg == "-s") {
            if ( !w.createQtSfw() ) {
                qDebug() << "please specify \'-q\' before \'-s <servicefilename>\'";
                QCoreApplication::quit();
            }

            argIndex++;
            arg = argv[ argIndex ];

            w.setServiceName( arg );
        }
        else if (arg == "-d") {
            if ( !w.createQtSfw() ) {
                qDebug() << "please specify \'-q\' before \'-d \"<Service description>\"\'";
                usage();
            }

            argIndex++;
            arg = argv[ argIndex ];

            w.setServiceDescription( arg );
        }
        else if (arg == "-v") {
            if ( !w.createQtSfw() ) {
                qDebug() << "please specify \'-q\' before \'-v \"<interface version>\"\'";
                usage();
            }

            argIndex++;
            arg = argv[ argIndex ];

            w.setInterfaceVersion( arg );
        }
        else if (arg == "-D") {
            if ( !w.createQtSfw() ) {
                qDebug() << "please specify \'-q\' before \'-D \"<Qt SFW directory>\"\'";
                usage();
            }

            argIndex++;
            arg = argv[ argIndex ];

            w.setQtSfwDirectory( arg );
        }
        else
        {
            // if we get a parameter here, we got the path of the xml file
            // to use as interface description, similar to how
            // qdbusxml2cpp handles it

            // check if file exists
            QFileInfo fi( arg );

            if ( ! fi.exists() )
            {
                qDebug() << "error: xml file " << arg << " does not exist.";
                usage();
            }

            xmlFilePath = arg;
            w.setOldUsage( false );
        }

    }

    if ( (! w.createAdaptor()) && (! w.createProxy()) && !w.createQtSfw() ) {
        qDebug() << "neither -p, -a or -q specified";
        usage();
    }

    if (interfaceName.isEmpty()) {
        usage();
    }

    w.setInterfaceName( interfaceName );

    if ( ! w.oldUsage() )
    {
        // update xml file path after all other values were
        // calculated from setInterfaceName()

        w.setXmlFileName( xmlFilePath );
    }

    w.preprocessXML();

    if ( w.createProxy() ) {
        runQDBusXml2Cpp(QStringList()
                        << "-c"
                        << w.upperCamelProxyName()
                        << "-p"
                        << w.proxyBase()
                        << w.newXmlFileName());

        processProxyHeaderFile();
        processProxyCppFile();

        // remove temporary file
        removeNewXmlFile();
    } else
    if ( w.createAdaptor() ) {
        runQDBusXml2Cpp(QStringList()
                        << "-c"
                        << w.upperCamelAdaptorName()
                        << "-a"
                        << w.adaptorBase()
                        << w.newXmlFileName());

        processAdaptorCppFile();
        processAdaptorHeaderFile();

        // remove temporary file
        removeNewXmlFile();
    } else {
        // create qtsfw

        // make qtsfw project directory
        QString qtSfwDirectory = w.qtSfwDirectory();
        if ( !QFile::exists( qtSfwDirectory ) ) {
            QDir workingDir;
            bool succeeded = workingDir.mkdir( qtSfwDirectory );
            if ( !succeeded ) {
                qDebug() << "Could not create directory" << qtSfwDirectory;
                exit(-1);
            }
        }

        // move new xml file into qtsfw directory
        {
            bool success = QFile::rename( w.newXmlFileName(), qtSfwDirectory+"/"+w.newXmlFileName() );
            if ( !success ) {
                qDebug() << "Could not move newXmlFile" << w.newXmlFileName();
                exit(-1);
            }
        }

        // chdir into the project directory
        QDir::setCurrent( qtSfwDirectory );

        // make standard proxy files
        runQDBusXml2Cpp(QStringList()
                        << "-c"
                        << w.upperCamelProxyName()
                        << "-p"
                        << w.proxyBase()
                        << w.newXmlFileName());
        bool forQtSfw = true;
        processProxyHeaderFile( forQtSfw );
        processProxyCppFile( forQtSfw );

        generateQSFWXmlFile();
        generateQSFWProject();

        // remove temporary file
        removeNewXmlFile();
    }

    return 0;
}
