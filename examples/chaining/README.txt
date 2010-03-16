This example shows a basic chaining mechanism.

There are three entities involved in chaining.

1. The interface - chainingexampleinterface.
It defines what methods, signals, etc the interface consists of.
Applications wishing to call methods in the interface need to include the interface's header file, and link with the interface's library.

2. The interface user - chainingexampleapp1.
It is the application that uses the interface. It doesn't care what application actually implements the methods in the interface.

3. The interface provider/implementor - chainingexampleapp2.
It implements the methods/etc in the interface and so is launched when chainingexampleapp1 calls it's methods via the interface. When the user is done with this app, he/she presses the 'back' button, which calls the DuiApplicationService 'launch' method for app1, thereby raising it to the top, and exits.
