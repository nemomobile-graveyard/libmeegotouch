Some short instructions and notes on how to write performance benchmarks.

1) Use pt_duibutton as a template - TODO:make a base class for common parts.
2) Change file and class names as appropriate
3) In the createView() method, change the cases of the switch statement to set the 'currentView' to a new instance of each view's class
4) In the fetchNewRow() method, the header file, and the paintPerformance_data() method, change the columns/variables to what you want to use as variables to measure performance
5) In the createController() method, create the controller and assign the properties appropriately
6) In the createPaintDevice() method, you generally want to set the width and height of the target pixmap to the width being rendered, but sometimes not since things may be rendered outside this area (eg labels)
7) In the paintPerformance() method, you generally only need a single call to paint() but some view's paint methods have different parameters, in which case you'll need a switch (eg in the duislider).
