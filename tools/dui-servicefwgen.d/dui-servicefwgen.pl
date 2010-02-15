#! /usr/bin/perl -w
# This script is used to automatically generate the proxy source files,
# and the wrapper header file for an interface
use strict;

use vars qw(
  $upperCamelServiceName
  $lowerServiceName
  $xmlFile
  $newXmlFile
  $chainTag
  $asyncTag
  $upperCamelProxyName
  $proxyCppFile
  $proxyHeaderFile
  $newProxyHeaderFile
  $wrapperHeaderFile
  $wrapperCppFile
  $wrapperHeaderGuard
  $upperCamelAdaptorName
  $adaptorCppFile
  $newAdaptorCppFile
  $newAdaptorHeaderFile
  $adaptorHeaderFile
  $allSignals
  $allConnectSignals
  $needsDuiApplication
  @doc
  $docTag
);

sub processAdaptorHeaderFile
{
    my @signals = ();

    open( ADAPTOR,    "<$adaptorHeaderFile" )    || die( "Could not open $adaptorHeaderFile:$!" );
    open( NEWADAPTOR, ">$newAdaptorHeaderFile" ) || die( "Could not open $newAdaptorHeaderFile:$!" );

    while (<ADAPTOR>) {
        chomp();

        # remove doctag - can be more than one per line
        s/$docTag\d+//g;

        # add chaining code to NEWADAPTOR
        {
            if ( /Q_SIGNALS:/ ) {
                print NEWADAPTOR "    void goBack();\n";
                print NEWADAPTOR "\n";
                print NEWADAPTOR "private:\n";
                print NEWADAPTOR "    QString backServiceName;\n";
                print NEWADAPTOR "    int windowId;\n";
                print NEWADAPTOR "\n";
                print NEWADAPTOR "$_\n";
            } elsif ( /chainTask=\\"\w+\\"/ || /asyncTask=\\"\w+\\"/ ) { # process comment
                # remove asyncTask attribute
                if ( s/asyncTask=\\"\w+\\"\s*// ) {
                  $_ =~ s/\Q$asyncTag\E//g;
                }

                # remove asyncTask attribute
                my $isChainTask = 0;
                if ( s/chainTask=\\"(\w+)\\"\s*// ) {
                  $isChainTask = ($1 =~ /true/);
                  $_ =~ s/\Q$chainTag\E//g;
                }

                # remove extraneous spaces (inserted randomly by qdbusxml2cpp?)
                $_ =~ s/\s+(\/*>)/$1/;

                print NEWADAPTOR "$_\n";

                # this has to be printed after the method tag, above
                if ( $isChainTask ) {
                    print NEWADAPTOR "\"      <arg direction=\\\"in\\\" type=\\\"s\\\" name=\\\"backServiceName\\\"/>\\n\"\n";
                    print NEWADAPTOR "\"      <arg direction=\\\"in\\\" type=\\\"s\\\" name=\\\"windowTitle\\\"/>\\n\"\n";
                    print NEWADAPTOR "\"      <arg direction=\\\"in\\\" type=\\\"u\\\" name=\\\"windowId\\\"/>\\n\"\n";
                }
            } elsif ( /$chainTag\(/ || /$asyncTag\(/ ) { # add parameters to the chain methods
                if ( s/\Q$asyncTag\E//g ) {
                    $_ =~ s/(\w+)(\s+)(\w+)/Q_NOREPLY $1$2$3/; # put Q_NOREPLY before the first word on the line (which should be 'void', but perhaps not)

                    # make warning if type is not void
                    my $type = $1;
                    my $method = $3;
                    if ( $type ne "void" ) {
                        my $prototype = $_;

                        # clean up prototype and method name for warning message
                        $prototype =~ s/\Q$chainTag\E//g;
                        $prototype =~ s/^\s+//;
                        $method =~ s/\Q$chainTag\E//g;
                        print STDERR "WARNING: $prototype\n";
                        print STDERR "       : asyncTask=\"true\" specified for $method(), but type is $type\n";
                        print STDERR "       : return type should be void for async methods - did you really mean this?\n";
                        print STDERR "       : consider editing the xml to make the return type \'void\'\n";
                    }
                }

                if ( s/\Q$chainTag\E//g ) {
                    $_ =~ s/\(/\(const QString \&backServiceName, const QString \&windowTitle, const uint windowId, /;
                }

                print NEWADAPTOR "$_\n";
            } else {
                # remove extraneous spaces (inserted randomly by qdbusxml2cpp?)
                $_ =~ s/\s+(\/*>)/$1/;

                print NEWADAPTOR "$_\n";
            }
        }
    }

    close( NEWADAPTOR );
    close( ADAPTOR );

    # mv new adaptor header file (with chain parameters added)
    # to replace one produced by qdbusxml2cpp
    unlink( $adaptorHeaderFile );
    rename( $newAdaptorHeaderFile, $adaptorHeaderFile );

    # remove temporary file
    unlink( $newXmlFile );
}

sub processAdaptorCppFile
{
    my @signals = ();

    open( ADAPTOR,    "<$adaptorCppFile" )    || die( "Could not open $adaptorCppFile:$!" );
    open( NEWADAPTOR, ">$newAdaptorCppFile" ) || die( "Could not open $newAdaptorCppFile:$!" );

    my $inChainTask = 0;

    while (<ADAPTOR>) {
        chomp();

        # remove doctag - can be more than one per line
        s/$docTag\d+//g;

        # add chaining code to NEWADAPTOR
        {
            # always remove asyncTag, since we don't care about it in the cpp file
            $_ =~ s/\Q$asyncTag\E//g;

            if ( $needsDuiApplication && /#include <QtCore\/Q(Meta)?Object>/ ) {
                print NEWADAPTOR "\n";
                print NEWADAPTOR "#include <DuiApplication>\n";
                print NEWADAPTOR "#include <DuiApplicationPage>\n";
                print NEWADAPTOR "#include <DuiApplicationWindow>\n";
                print NEWADAPTOR "#include <DuiApplicationIfProxy>\n";
                print NEWADAPTOR "#include <DuiEscapeButtonPanel>\n";
                print NEWADAPTOR "#include <QX11Info>\n";
                print NEWADAPTOR "#include <X11/Xutil.h>\n";
                print NEWADAPTOR "#include <X11/Xlib.h>\n";
                print NEWADAPTOR "#include <X11/Xatom.h>\n";
                print NEWADAPTOR "#include <X11/Xmd.h>\n";
                print NEWADAPTOR "\n";
                print NEWADAPTOR "$_\n";
            } elsif ( $needsDuiApplication && /QDBusAbstractAdaptor\(parent\)/ ) {
                print NEWADAPTOR "$_,\n";
                print NEWADAPTOR "    backServiceName(),\n";
                print NEWADAPTOR "    windowId(-1)\n";
            } elsif ( s/\Q$newXmlFile\E/$xmlFile/g ) {
                print NEWADAPTOR "$_\n";
            } elsif ( $inChainTask ) {
                $_ =~ s/\Q$chainTag\E//;
                if ( /^{$/ ) { #}
                    print NEWADAPTOR "$_\n";
                    print NEWADAPTOR "    this->windowId = windowId;\n";
                    print NEWADAPTOR "    this->backServiceName = backServiceName;\n";
                    print NEWADAPTOR "\n";
                } elsif ( /return/ || /^}$/ ) { # match end of function - need to add the connect *before* the return, if there is one
                    print NEWADAPTOR "\n";
                    print NEWADAPTOR "    DuiApplication::instance()->activeWindow()->setWindowTitle( windowTitle );\n";
                    print NEWADAPTOR "    DuiApplicationWindow *appWindow = DuiApplication::activeApplicationWindow();\n";
                    print NEWADAPTOR "    if (appWindow != 0) {\n";
                    print NEWADAPTOR "        appWindow->currentPage()->setEscapeButtonMode( DuiEscapeButtonPanelModel::BackMode );\n";
                    print NEWADAPTOR "        // connect to the back button - assumes the above 'showImage' opens a\n";
                    print NEWADAPTOR "        // new window and so the window referred to below is already the top one\n";
                    print NEWADAPTOR "        connect(appWindow->currentPage(), SIGNAL(backButtonClicked()),\n";
                    print NEWADAPTOR "                this, SLOT(goBack()));\n";
                    print NEWADAPTOR "    }\n";
                    print NEWADAPTOR "\n";
                    print NEWADAPTOR "    // update the X server\n";
                    print NEWADAPTOR "    {\n";
                    print NEWADAPTOR "        XPropertyEvent p;\n";
                    print NEWADAPTOR "        p.send_event = True;\n";
                    print NEWADAPTOR "        p.display = QX11Info::display();\n";
                    print NEWADAPTOR "        p.type   = PropertyNotify;\n";
                    print NEWADAPTOR "        p.window = RootWindow(p.display, 0);\n";
                    print NEWADAPTOR "        p.atom   = XInternAtom(p.display, \"_NET_CLIENT_LIST\", False);\n";
                    print NEWADAPTOR "        p.state  = PropertyNewValue;\n";
                    print NEWADAPTOR "        p.time   = CurrentTime;\n";
                    print NEWADAPTOR "        XSendEvent(p.display, p.window, False, PropertyChangeMask,\n";
                    print NEWADAPTOR "                (XEvent*)&p);\n";
                    print NEWADAPTOR "\n";
                    print NEWADAPTOR "        XSync(QX11Info::display(), False);\n";
                    print NEWADAPTOR "    }\n";
                    print NEWADAPTOR "\n";
                    print NEWADAPTOR "$_\n";
                    $inChainTask = 0;
                } else {
                    print NEWADAPTOR "$_\n";
                }
            } elsif ( s/\Q$chainTag\E// ) {
                # remove CHAINTASK
                $_ =~ s/\(/\(const QString \&backServiceName, const QString \&windowTitle, const uint windowId, /;
                print NEWADAPTOR "$_\n";
                $inChainTask = 1;
            } else {
                print NEWADAPTOR "$_\n";
            }
        }
    }

    print NEWADAPTOR <<EOF;
void ${upperCamelAdaptorName}::goBack()
{
    qDebug() << __PRETTY_FUNCTION__;

    bool backServiceRegistered = ( windowId != -1 );
    if ( backServiceRegistered ) {
        DuiApplicationIfProxy duiApplicationIfProxy(backServiceName, this);

        if (duiApplicationIfProxy.connection().isConnected()) {
            qDebug() << "Launching " << backServiceName;
            duiApplicationIfProxy.launch();
        } else {
            qWarning() << "Could not launch" << backServiceName;
            qWarning() << "DBus not connected?";
        }

        // unhide the chain parent's window
        {
            // Tell the window to not to be shown in the switcher
            XDeleteProperty(QX11Info::display(), windowId, XInternAtom(QX11Info::display(), "_NET_WM_STATE", False));
            XSync(QX11Info::display(), False);
        }

        qWarning() << "quitting - bye bye";
        QTimer::singleShot( 0, QApplication::instance(), SLOT( quit() ) );
    } else {
        qWarning() << "backService is not registered: not going back";
    }
}
EOF

    close( NEWADAPTOR );
    close( ADAPTOR );

    # mv new adaptor header file (with chain parameters added)
    # to replace one produced by qdbusxml2cpp
    unlink( $adaptorCppFile );
    rename( $newAdaptorCppFile, $adaptorCppFile );

    # remove temporary file
    unlink( $newXmlFile );
}

sub processProxyHeaderFile
{
  my @signals = ();

  open( PROXY,    "<$proxyHeaderFile" )    || die( "Could not open $proxyHeaderFile:$!" );
  open( NEWPROXY, ">$newProxyHeaderFile" ) || die( "Could not open $newProxyHeaderFile:$!" );
  open( IFH,      ">$wrapperHeaderFile" )  || die( "Could not open $wrapperHeaderFile:$!" );
  open( IFC,      ">$wrapperCppFile" )     || die( "Could not open $wrapperCppFile:$!" );

  print IFC topBitC();
  print IFH topBitH();

  my $inSignalSection = 0;
  my $inChainTask = 0;

  while (<PROXY>) {
    chomp();

    # add documentation and remove doc tags here
    # note that middle bit is added here too, so it's not just about doc
    if ( /^class/ ) {
        if ( defined( $doc[ 0 ] ) ) {
            my $thisDoc = $doc[ 0 ];
            # adjust indentation
            if ( $thisDoc =~ /^(\s+)/ ) {
                my $noSpaces = length( $1 );
                $thisDoc =~ s/^\s{$noSpaces}//mg;
            }
            print IFH $thisDoc;
        }
        print IFH middleBitH();
    } else {
        for ( my $docTagNo=1; $docTagNo<@doc; ++$docTagNo ) {
            if ( s/$docTag$docTagNo// && !/return/ ) {
                my $thisDoc = $doc[ $docTagNo ];
                # adjust indentation
                if ( $thisDoc =~ /^(\s+)/ ) {
                    my $noSpaces = length( $1 );
                    my $correctIndentation=' 'x4;
                    $thisDoc =~ s/^\s{$noSpaces}/$correctIndentation/mg;
                }
                print IFH "\n";
                print IFH $thisDoc;
            }
        }
    }
    s/$docTag\d+//g;

    # add chaining code to NEWPROXY
    {
      # always remove asyncTag, since we don't care about it in the cpp file
      $_ =~ s/\Q$asyncTag\E//g;

      if ( $needsDuiApplication && /#include <QtCore\/Q(Meta)?Object>/ ) {
        print NEWPROXY "#include <DuiApplication>\n";
        print NEWPROXY "#include <DuiApplicationWindow>\n";
        print NEWPROXY "#include <DuiApplicationIfProxy>\n";
        print NEWPROXY "#include <DuiComponentData>\n";
        print NEWPROXY "#include <QX11Info>\n";
        print NEWPROXY "#include <X11/Xutil.h>\n";
        print NEWPROXY "#include <X11/Xlib.h>\n";
        print NEWPROXY "#include <X11/Xatom.h>\n";
        print NEWPROXY "#include <X11/Xmd.h>\n";
        print NEWPROXY "\n";
        print NEWPROXY "$_\n";
      } elsif ( $needsDuiApplication && s/\Q$newXmlFile\E/$xmlFile/g ) {
        print NEWPROXY "$_\n";
      } elsif ( $inChainTask ) {
        if ( /QList<QVariant>/ ) {
          print NEWPROXY "        Qt::HANDLE windowId       = DuiApplication::instance()->activeWindow()->winId();\n";
          print NEWPROXY "        QString windowTitle       = DuiApplication::instance()->activeWindow()->windowTitle();\n";
          print NEWPROXY "        QString goBackServiceName = DuiComponentData::instance()->serviceName();\n";
          print NEWPROXY "\n";
          print NEWPROXY "$_\n";
          print NEWPROXY "        argumentList << qVariantFromValue(goBackServiceName);\n";
          print NEWPROXY "        argumentList << qVariantFromValue(windowTitle);\n";
          print NEWPROXY "        argumentList << qVariantFromValue((uint)windowId);\n";
        } elsif ( /return/ ) {

          print NEWPROXY <<EOF;

        // hide this window
        {
            // Tell the window to not to be shown in the switcher
            Atom skipTaskbarAtom = XInternAtom(QX11Info::display(), "_NET_WM_STATE_SKIP_TASKBAR", False);

            Atom netWmStateAtom = XInternAtom(QX11Info::display(), "_NET_WM_STATE", False);
            QVector<Atom> atoms;
            atoms.append(skipTaskbarAtom);
            XChangeProperty(QX11Info::display(), windowId, netWmStateAtom, XA_ATOM, 32, PropModeReplace, (unsigned char *)atoms.data(), atoms.count());
            XSync(QX11Info::display(), False);
        }

EOF
          $_ =~ s/\Q$chainTag\E//;
          print NEWPROXY "$_\n";
          $inChainTask = 0;
        } else {
          print NEWPROXY "$_\n";
        }
      } elsif ( s/\Q$chainTag\E// ) {
        print NEWPROXY "$_\n";
        $inChainTask = 1;
      } else {
        print NEWPROXY "$_\n";
      }
    }

    if ( $inSignalSection ) { #{
      my $atEndOfSignalSection = /^};$/;
      if ( $atEndOfSignalSection ) {
        $inSignalSection = 0;
      } else {
        push @signals, $_;
      }
    } else {
      if ( /Q_SIGNALS:/ ) {
        $inSignalSection = 1;
      } elsif ( /inline\s+QDBusPendingReply<([^>]*)>\s*(\w+)\(([^)]*)\)/ ) {
        my $returnType = $1;
        my $methodName = $2;
        my $parameters = $3;

        $returnType = "void" if ( $returnType eq "" );

        my @paramNames = getParamNames( $parameters );

        print IFH "    $returnType $methodName( $parameters );\n";

        print IFC "$returnType ${upperCamelServiceName}::$methodName( $parameters )\n";
        print IFC "{\n";
        if ( $returnType eq "void" ) {
          print IFC "    static_cast<${upperCamelServiceName}Proxy*>(interfaceProxy)->${methodName}( ".join( ", ", @paramNames )." );\n";
        } else {
          print IFC "    return qobject_cast<${upperCamelServiceName}Proxy*>(interfaceProxy)->${methodName}( ".join( ", ", @paramNames )." ).value();\n";
        }
        print IFC "}\n\n";
      }
    }
  }

  $allSignals = join( "\n", @signals );

  # process signals, removing void and param names
  my @connectSignals = ();
  foreach ( @signals ) {
    my ($signalName, $signature) = ($_ =~ m/^\s+void\s+(\w+)\(([^)]*)\);$/);

    my @typesOnly = ();
    my @paramsAndTypes = split( ',', $signature );
    foreach ( @paramsAndTypes ) {
      # remove last word
      /(.*)\b\w+$/;
      push @typesOnly, $1;
    }
    my $joinedTypes = join( ',', @typesOnly );

    push @connectSignals, <<EOF;
             connect( interfaceProxy, SIGNAL( $signalName( $joinedTypes ) ),
                      this, SIGNAL( $signalName( $joinedTypes ) ) );
EOF
  }

  $allConnectSignals = join( "\n", @connectSignals );

  print IFC botBitC();
  print IFH botBitH();

  close( IFC );
  close( IFH );
  close( NEWPROXY );
  close( PROXY );

  # mv new proxy header file (with chain parameters added)
  # to replace one produced by qdbusxml2cpp
  unlink( $proxyHeaderFile );
  rename( $newProxyHeaderFile, $proxyHeaderFile );

  # remove temporary file
  unlink( $newXmlFile );
}

# add a tag to the name of chain and async methods in the xml file
# so that we can easily tell which methods are to be chained/asynched after they are processed by qdbusxml2cpp
sub preprocessXML()
{
    my $inDoc = "outOfDoc";
    my $docTagNo = -1;
    open( XML, "<$xmlFile" ) || die( "Could not open $xmlFile:$!" );
    open( NEWXML, ">$newXmlFile" ) || die( "Could not open $newXmlFile:$!" );
    while (<XML>) {

        # tag tasks
        if ( /chainTask=\"true\"/ ) {
            s/name=\"([^\"]+)\"/name=\"\Q$1$chainTag\E\"/;
            $needsDuiApplication = 1;
        }
        if ( /asyncTask=\"true\"/ ) {
            s/name=\"([^\"]+)\"/name=\"\Q$1$asyncTag\E\"/;
            $needsDuiApplication = 1;
        }

        # tag the interfaces and methods
        if ( /<interface/ || /<method/ ) {
            ++$docTagNo;
            $doc[ $docTagNo ] = "";
            s/name=\"([^\"]+)\"/name=\"$1$docTag$docTagNo\"/;
        }

        if ( /<doc>/ ) {
            # record entry of doc blocks
            $inDoc = "openDoc";
        } elsif ( /<\/doc>/ ) {
            # record exit of doc blocks
            $inDoc = "closeDoc";
        }

        if ( $inDoc eq "insideDoc" ) {
            # record doc lines
            $doc[ $docTagNo ] .= $_;
        } elsif ( $inDoc eq "openDoc" ) {
            $inDoc = "insideDoc";
        } elsif ( $inDoc eq "closeDoc" ) {
            $inDoc = "outsideDoc";
        } else {
            print NEWXML $_;
        }
    }
    close( NEWXML );
    close( XML );
}

sub getParamNames()
{
    my ( $parameters ) = @_;
    my @retVal = ();

    my @typesAndNames = split( /,/, $parameters );

    foreach my $typeAndName ( @typesAndNames ) {
        my @bits = split( /\W+/, $typeAndName );
        my $name = $bits[-1];

        push @retVal, $name;
    }

    return @retVal;
}

sub topBitH
{
    my $commandLine = join( " ", $0, @ARGV );

    return <<EOF;
#ifndef $wrapperHeaderGuard
#define $wrapperHeaderGuard

/*
 * automatically generated with the command line :
 * $commandLine
 */

#include <QObject>

#include <duiservicefwbaseif.h>
#include <$proxyHeaderFile>

EOF
}

sub middleBitH
{
    return <<EOF;
class $upperCamelServiceName : public DuiServiceFwBaseIf
{
Q_OBJECT

public:
EOF
}

sub topBitC
{
    my $commandLine = join( " ", $0, @ARGV );

    return <<EOF;
/*
 * automatically generated with the command line :
 * $commandLine
 */

#include "$wrapperHeaderFile"

EOF
}

sub botBitH
{
    return <<EOF;

public:
    /*!
     * \@brief Constructs a base interface
     * \@param preferredService, define the preferred service provider. Leave
       empty if no preferred provider. In most cases, this should be left
       empty.
     * \@param parent Parent object
     */

    $upperCamelServiceName( const QString& preferredService = "", QObject* parent = 0 );

    /*!
     * \@brief Set the service name
     * \@param service Name of the desired service
     */

    void setService(const QString & service);
Q_SIGNALS:
$allSignals

};
#endif
EOF
}

sub botBitC
{
    return <<EOF;

${upperCamelServiceName}::$upperCamelServiceName( const QString& preferredService, QObject* parent )
    : DuiServiceFwBaseIf( ${upperCamelProxyName}::staticInterfaceName(), parent )
{
    // Resolve the provider service name
    service = resolveServiceName( interface, preferredService );

    bool serviceNameInvalid = service.contains( " " ); // "not provided" - when the service wouldn't run
    if ( serviceNameInvalid ) {
        service.clear();
    }

    if (!service.isEmpty()) {
        // Construct the D-Bus proxy
        interfaceProxy = new $upperCamelProxyName( service, "/", QDBusConnection::sessionBus(), this );
        // allConnectSignals go here (empty block if none)
$allConnectSignals
    }
}

/*!
 * \@brief Set the service name
 * \@param service Name of the desired service
 */

void ${upperCamelServiceName}::setService(const QString & service)
{
    if (service.isEmpty()) return;

    this->service = service;

    if ( interfaceProxy ) {
        delete interfaceProxy;
        interfaceProxy = 0;
    }
    interfaceProxy = new $upperCamelProxyName( service, "/", QDBusConnection::sessionBus(), this );
    {
$allConnectSignals
    }
}
EOF
}

sub usage()
{
    print STDERR "usage: $0 [-a|-p] interfaceName\n";
    print STDERR "       -a            generate adaptor files\n";
    print STDERR "       -p            generate proxy files\n";
    print STDERR "       -h            this help\n";
    exit(1);
}

sub main()
{
  my $interfaceName="";

  if ( @ARGV == 0 ) {
      usage();
  }

  my %opts = (
      "a" => 0,
      "p" => 0,
      );

  for my $arg (@ARGV) {
    if ( $arg eq "-h" ) {
        usage();
    } elsif ( $arg eq "-a" ) {
      $opts{ "a" } = 1;
      if ( $opts{ "p" } ) {
        print STDERR "both -p and -a supplied\n";
        print STDERR "disabling proxy generation\n";
        $opts{ "p" } = 0;
      }
    } elsif ( $arg eq "-p" ) {
      $opts{ "p" } = 1;
      if ( $opts{ "a" } ) {
        print STDERR "both -p and -a supplied\n";
        print STDERR "disabling adaptor generation\n";
        $opts{ "a" } = 0;
      }
    } else {
      my @bits = split( "/", $arg );

      $interfaceName = pop @bits;
      my $interfacePath = join( "/", @bits );
      chdir( $interfacePath )
	      if ( defined( $interfacePath ) && ($interfacePath ne "") );
    }
  }

  if ( ! $opts{ "a" } && ! $opts{ "p" } ) {
      print STDERR "neither -p or -a specified\n";
      print STDERR "assuming -p\n";
      $opts{ "p" } = 1;
  }

  if ( $interfaceName eq "" ) {
      usage();
  }

  # make different upper/lower case versions from the Upper Camel version 
  $upperCamelServiceName=(split(/\./, $interfaceName))[-1];
  $lowerServiceName     =lc( $upperCamelServiceName );

  $xmlFile              = "$interfaceName.xml";
  $newXmlFile           = "$interfaceName-$$.xml";

  # tag to add to chained methods in the xml file
  my $randomNumber = int( rand( 1000000000 ) );
  $chainTag = "CHAINTASK$randomNumber";
  $asyncTag = "ASYNCTASK$randomNumber";
  $docTag   = "DOCTAG$randomNumber";

  $upperCamelProxyName   = "";
  $proxyCppFile          = "";
  $proxyHeaderFile       = "";
  $newProxyHeaderFile    = "";
  $wrapperHeaderFile     = "";
  $wrapperCppFile        = "";
  $wrapperHeaderGuard    = "";
  $upperCamelAdaptorName = "";
  $adaptorCppFile        = "";
  $newAdaptorCppFile     = "";
  $newAdaptorHeaderFile  = "";
  $needsDuiApplication   = 0;

  preprocessXML();

  if ( $opts{ "p" } ) {
    $upperCamelProxyName  = "${upperCamelServiceName}Proxy";
    my $proxyBase         = "${lowerServiceName}proxy";
    $proxyCppFile         = "${proxyBase}.cpp";
    $proxyHeaderFile      = "${proxyBase}.h";
    $newProxyHeaderFile   = "${proxyBase}-$$.h";
    $wrapperHeaderFile    = "${lowerServiceName}.h";
    $wrapperCppFile       = "${lowerServiceName}.cpp";
    ( $wrapperHeaderGuard = uc($wrapperHeaderFile) ) =~ s/\./_/;

    system( "qdbusxml2cpp -c $upperCamelProxyName -p $proxyBase $newXmlFile" );
    processProxyHeaderFile();
  } else {
    $upperCamelAdaptorName = "${upperCamelServiceName}Adaptor";
    my $adaptorBase          = "${lowerServiceName}adaptor";
    $adaptorCppFile        = "${adaptorBase}.cpp";
    $adaptorHeaderFile     = "${adaptorBase}.h";
    $newAdaptorCppFile     = "${adaptorBase}-$$.cpp";
    $newAdaptorHeaderFile  = "${adaptorBase}-$$.h";

    system( "qdbusxml2cpp -c $upperCamelAdaptorName -a $proxyBase $newXmlFile" );
    processAdaptorCppFile();
    processAdaptorHeaderFile();
  }
}

main();

exit(0);
