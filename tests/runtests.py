#!/usr/bin/python

import sys
import os
import time
import signal
import subprocess
import commands as cmd
import datetime
import time 
from _xmlplus.dom.NodeFilter import NodeFilter
from _xmlplus.dom import ext
from _xmlplus.dom import minidom
from datetime import datetime

def build_results(result, buffer):
    # Builds a results.xml by parsing xml logs in QTestLib xml format and 
    # combining their output into check xml format.


    
    # Then add those to the doc structure
    
    #testElement.setAttributeNS(checkns,"result","Success")

    #x = 0 
    #for line  in buffer.split("\n"):
    #    print "LINE " + str(x) + ": " + line
    #    x=x+1
    
    tmpdoc = minidom.parseString(buffer)

    e_testcase = tmpdoc.getElementsByTagName("TestCase")
    title =  e_testcase[0].attributes.item(0).nodeValue

    suiteElement = doc.createElementNS(checkns,"suite")
    titleElement = doc.createElementNS(checkns,"title")


    node = doc.createTextNode(title)
    titleElement.appendChild(node)

    rootElement.appendChild(suiteElement)
    suiteElement.appendChild(titleElement)


    suites = tmpdoc.getElementsByTagName("TestFunction")

    for elem in tmpdoc.getElementsByTagName("TestFunction"):
        testElement = doc.createElementNS(checkns,"test")
    
        pathElement = doc.createElementNS(checkns,"path")
        fnElement = doc.createElementNS(checkns,"fn")
        idElement = doc.createElementNS(checkns,"id")
        descriptionElement = doc.createElementNS(checkns,"description")
        messageElement = doc.createElementNS(checkns,"message")

        node = doc.createTextNode( elem.attributes.item(0).nodeValue )
        idElement.appendChild(node)

        node = doc.createTextNode(".")
        pathElement.appendChild(node)

        testElement.appendChild(pathElement)
        testElement.appendChild(fnElement)
        testElement.appendChild(idElement)
        testElement.appendChild(descriptionElement)
        testElement.appendChild(messageElement)
    
        suiteElement.appendChild(testElement)
        
        incident = elem.getElementsByTagName("Incident")
        if incident != None:
            incident_type = incident[0].getAttribute("type")
            incident_file = incident[0].getAttribute("file")
            incident_row  = incident[0].getAttribute("line")

            if (incident_type == "pass"):
                testElement.setAttributeNS(checkns,"result","success")
            elif (incident_type == "fail"):            
                testElement.setAttributeNS(checkns,"result","failure")
            else:
                testElement.setAttributeNS(checkns,"result","error")

            if incident_file != None and incident_file != "":
                node = doc.createTextNode(incident_file + ":" + str(incident_row))
            else:
                node = doc.createTextNode("n/a:0")
            
            fnElement.appendChild(node)


            description = incident[0].getElementsByTagName("DataTag")
            if description != None and description.length>0 and description[0].firstChild != None:
                node = doc.createTextNode(str(description[0].firstChild.data)+" ")
                descriptionElement.appendChild(node)

            description = incident[0].getElementsByTagName("Description")
            if description != None and description.length>0 and description[0].firstChild != None:
                node = doc.createTextNode(str(description[0].firstChild.data)+" ")
                descriptionElement.appendChild(node)

    #for binary in binariestorun:
    #    filename = "/tmp/" + binary + ".log"
    #    file = open(filename,"r")
    #    buffer = file.read()
    #    print binary
    
    return
    
def init_env(binariestorun):

    global doc
    global checkns 
    global rootElement 
    global datetimeElement 
    global durationElement
    global startTime

    doc = minidom.Document()
    doc = minidom.Document()
    checkns = "http://check.sourceforge.net/ns"
    rootElement = doc.createElementNS(checkns, "testsuites")
    datetimeElement = doc.createElementNS(checkns,"datetime")

    doc.appendChild(rootElement)
    rootElement.appendChild(datetimeElement)
    node = doc.createTextNode(datetime.today().strftime("%m.%d.%Y %H:%M:%S"))
    datetimeElement.appendChild(node)
    durationElement = doc.createElementNS(checkns,"duration")
    
    # If the binaries require different environments, which are not 
    # mutually compatible, init_env and deinit_env need to be called 
    # between running the binaries. Another option is to run the whole 
    # script several times with different arguments. 

    processes = []

    # Return a list of system processes that need to be closed after the
    # tests are ran.

    startTime = time.clock()
    return processes

def deinit_env(binariestorun, processes):

    endTime = time.clock()
    time.sleep(1)

    print "stop the daemonized processes", processes
    for process in processes:
        os.kill(process, signal.SIGTERM)
        os.waitpid(process, 0)

    rootElement.appendChild(durationElement)
    node = doc.createTextNode(str ( endTime - startTime) )
    durationElement.appendChild(node)

    file_object = open("/tmp/result.xml", "w")
    ext.PrettyPrint(doc, file_object)
    file_object.close()

def init_maps():

    # The test cases for test binaries. This might be automatically
    # generated?

    test_gprs = [ "test_connect" ]

    # Map the binary name keys to test case values ...
    casedict = { "test_gprs" : test_gprs }

    # ... and the other way around. There might be multiple binaries 
    # linked to one test case, so we need to have test binary lists for 
    # the test cases. Binarydict has test case names as keys, lists of 
    # corresponding binaries as values. 

    binarydict = {}

    for key in casedict.keys():
        for test in casedict[key]:
            if binarydict.has_key(test):
                binarylist = binarydict[test]
                binarylist.append(key)
            else:
                binarydict[test] = [key]

    return casedict, binarydict

def main():

    # 1. Initialize the mappings between the test cases and test 
    # binaries. This is needed if the test system wants to only specify 
    # test cases to be run. 
    
    casedict, binarydict = init_maps()

    # FIXME: temporary
    # The testmode variable changes the input handling behavior. If the 
    # variable is True, the input parameters are test case names. It 
    # the parameter is False, the input parameters are test binary 
    # names. 

    testmode = False
    # testmode = True

    binariestorun = []

    if testmode:
        # if input is test cases, see which binaries need to be run
        print "test cases: ", sys.argv[1:]
        for testcase in sys.argv[1:]:
            binarylist = binarydict[testcase]
            for binaryname in binarylist:
                if binariestorun.count(binaryname) == 0:
                    binariestorun.append(binaryname)
    
    else:
        # else if input is binary names, do nothing
        binariestorun = sys.argv[1:]


    # 2. Initialize the testing environment according to the binaries 
    # that are to be run. All system settings that differ from the 
    # basic boot-up state need to be defined here. 

    processes = init_env(binariestorun)

    # 3. Run the test binaries. It is assumed (for now) that they are 
    # in the same directory as the test script. 

    dirname = os.path.dirname(sys.argv[0])
    print "test binaries to run: ", binariestorun
    for binary in binariestorun:
        command = dirname + "/" + binary  + " -xml"
        # print command
        (status, output) =  cmd.getstatusoutput(command)
        cmdpipe = subprocess.Popen(command, shell=True, stdout=subprocess.PIPE, stderr=None, universal_newlines=True)
        status = cmdpipe.wait()
        output = cmdpipe.stdout.read()
        build_results(status,output)
        
    # 5. Clean up the testing environment.

    deinit_env(binariestorun, processes)

if __name__ == "__main__":
    sys.exit(main())

