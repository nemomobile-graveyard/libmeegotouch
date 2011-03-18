#include <QtGlobal>
#include <QFileInfo>
#include <QDir>
#include <QProcess>
#include <QtDebug>


void runModelMoc(const QString& header, const QStringList& arguments)
{
    QFile inf(header);
    if(!inf.open(QFile::ReadOnly))
        qFatal("Could not open header file for reading");

    QTextStream in(&inf);

    QProcess p;
    p.setProcessChannelMode( QProcess::ForwardedChannels );
    p.start(QString(QT_MOC_PATH), arguments, QIODevice::WriteOnly);
    if(!p.waitForStarted()) {
        qFatal("mmoc: failed to run command '%s'", QT_MOC_PATH);
    }

    QString spaces = "\\s*";
    QString comma = ",";
    QString colon = ":";
    QString parenO = "\\(";
    QString parenC = "\\)";
    QString emptyParen = "\\(\\)";
    QString angleO = "<";
    QString angleC = ">";
    QString braceO = "\\{";
    QString nameSpace = "(?:\\w+::)";
    QString typeName = "\\w+";
    QString pointer = "\\*";
    QString templateName = "\\w+";
    QString plainParam = "(\\w+)";
    QString boolParam = "(true|false)";
    QString anyParam = "(.+)";

    QRegExp propertyRegExp(
            spaces+
            "M_MODEL_PROPERTY"+
            spaces+parenO+spaces+
                "("+
                    "(?:"+
                         nameSpace+"{,1}"+
                         typeName+
                         spaces+
                         pointer+"{,1}"+
                    ")"+
                    "|"+
                    "(?:"+
                         templateName+
                         angleO+
                             spaces+
                             typeName+
                             spaces+
                             pointer+"{,1}"+
                             spaces+
                         angleC+
                     ")"+
                  ")"+
                      spaces+comma+spaces+
                  plainParam+
                      spaces+comma+spaces+
                  plainParam+
                      spaces+comma+spaces+
                  plainParam+
                      spaces+comma+spaces+
                  anyParam+
              spaces+parenC+spaces );

    QRegExp propertyPtrRegExp(
        spaces+
        "M_MODEL_PTR_PROPERTY"+
        spaces+parenO+spaces+
            "("+
                "(?:"+
                    nameSpace+"{,1}"+
                    typeName+
                    spaces+
                    pointer+"{,1}"+
                    spaces+
                ")"+
                "|"+
                "(?:"+
                    templateName+
                    angleO+
                        spaces+
                        typeName+
                        spaces+
                        pointer+"{,1}"+
                        spaces+
                    angleC+
                ")"+
            ")"+
                spaces+comma+spaces+
            plainParam+
                spaces+comma+spaces+
            plainParam+
                spaces+comma+spaces+
            boolParam+
                spaces+comma+spaces+
            anyParam+
        spaces+parenC+spaces );

    QString line;
    while(true) {
        line = in.readLine();
        if(line.isNull()) {
            break;
        }

        line.replace(propertyRegExp, "    Q_PROPERTY(\\1 \\2 READ \\2 WRITE set\\3)");
        line.replace(propertyPtrRegExp, "    Q_PROPERTY(\\1 \\2 READ \\2 WRITE set\\3)");
        p.write(QString(line + "\n").toLatin1());
    }
    p.closeWriteChannel();

    if(!p.waitForFinished()) {
        qFatal("mmoc: failed to run command '%s'", QT_MOC_PATH);
    }

    if ( p.exitCode() != 0 ) {
        qFatal("mmoc: qt moc ended with error: %i", p.exitCode() );
    }
}

void runStyleMoc(const QString& header, const QStringList& arguments)
{
    QFile inf(header);
    if(!inf.open(QFile::ReadOnly))
        qFatal("Could not open header file for reading");

    QTextStream in(&inf);

    QProcess p;
    p.setProcessChannelMode( QProcess::ForwardedChannels );
    p.start(QString(QT_MOC_PATH), arguments);
    if(!p.waitForStarted()) {
        qFatal("mmoc: failed to run command '%s'", QT_MOC_PATH);
    }


    QRegExp attributeRegExp("\\s*M_STYLE_ATTRIBUTE\\s*\\(\\s*(\\w+\\:*\\w*)\\s*,\\s*(\\w+)\\s*,\\s*(\\w+)\\s*\\)\\s*");
    QRegExp attributePtrRegExp("\\s*M_STYLE_PTR_ATTRIBUTE\\s*\\(\\s*(\\w+\\:*\\w*\\s*\\*+)\\s*,\\s*(\\w+)\\s*,\\s*(\\w+)\\s*\\)\\s*");


    QString line;
    while(true) {
        line = in.readLine();
        if(line.isNull()) {
            break;
        }

        line.replace(attributeRegExp, "    Q_PROPERTY(\\1 \\2 READ \\2 WRITE set\\3)");
        line.replace(attributePtrRegExp, "    Q_PROPERTY(const \\1 \\2 READ \\2 WRITE set\\3)");
        p.write(QString(line + "\n").toLatin1());
    }
    p.closeWriteChannel();

    if(!p.waitForFinished()) {
        qFatal("mmoc: failed to run command '%s'", QT_MOC_PATH);
    }

    if ( p.exitCode() != 0 ) {
        qFatal("mmoc: qt moc ended with error: %i", p.exitCode() );
    }
}

void runQtMoc(const QStringList& arguments)
{
    QProcess p;
    p.setProcessChannelMode( QProcess::ForwardedChannels );
    p.start(QString(QT_MOC_PATH), arguments);

    if(!p.waitForStarted()) {
        qFatal("mmoc: failed to run command '%s'", QT_MOC_PATH);
    }

    if(!p.waitForFinished()) {
        qFatal("mmoc: failed to run command '%s'", QT_MOC_PATH);
    }

    if ( p.exitCode() != 0 ) {
        qFatal("mmoc: qt moc ended with error: %i", p.exitCode() );
    }
}

enum HeaderType {
    Model,
    Style
};

int main(int argc, const char *argv[])
{
    HeaderType type=Model;
    QStringList commandLineParameters;
    QString filename;

    for(int i=1; i<argc; ++i) {
        if(QString(argv[i]).endsWith("style.h")) {
            type = Style;
            filename = argv[i];
        } else if(QString(argv[i]).endsWith("model.h")) {
            type = Model;
            filename = argv[i];
        } else {
            commandLineParameters << QString(argv[i]);
        }
    }

    if(filename.isEmpty()) {
        runQtMoc(commandLineParameters);
    } else {
        commandLineParameters << "-f" + filename;
        if(type == Model) {
            runModelMoc(filename, commandLineParameters);
        } else if(type == Style) {
            runStyleMoc(filename, commandLineParameters);
        }
    }

    return 0; //success
}

