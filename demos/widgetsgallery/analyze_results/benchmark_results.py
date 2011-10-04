#!/usr/bin/env python
# -*- coding: utf-8 -*-
#/***************************************************************************
#**
#** Copyright (C) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies).
#** All rights reserved.
#** Contact: Nokia Corporation (directui@nokia.com)
#**
#** This file is part of libmeegotouch.
#**
#** If you have questions regarding the use of this file, please contact
#** Nokia at directui@nokia.com.
#**
#** This library is free software; you can redistribute it and/or
#** modify it under the terms of the GNU Lesser General Public
#** License version 2.1 as published by the Free Software Foundation
#** and appearing in the file LICENSE.LGPL included in the packaging
#** of this file.
#**
#****************************************************************************/


import xml.dom.minidom
import numpy

try:
  import pylab
except ImportError:
  pass

class benchmark_result :
    def __init__(self, page, benchmark, timestamps) :
        self.page = page
        self.benchmark = benchmark
        self.timestamps = timestamps

    def fps(self, start, window_size) :
        return (window_size - 1) * 1000. / (self.timestamps[start + window_size - 1] - self.timestamps[start])

    def fps_list(self, window_size=2) :
        result = []
        for i in range(len(self.timestamps) - window_size + 1) :
            result.append(self.fps(i, window_size))
        return result

    def plot_fps(self, window_size, label, style) :
        pylab.plot(self.timestamps[:-window_size+1], self.fps_list(window_size), style, label=label)

    def fps_mean(self) :
        if (self.runtime() > 0) :
            return 1000. * (len(self.timestamps) - 1) / self.runtime()
        else :
            return 0

    def fps_std_dev(self) :
        return numpy.std(self.fps_list(2))

    def fps_median(self) :
        return numpy.median(self.fps_list(2))

    def fps_min(self) :
        fps_list = self.fps_list(2)
        if not fps_list :
            return 0
        else :
            return min(fps_list)

    def runtime(self) :
        if len(self.timestamps) < 2 :
            return 0
        else :
            return self.timestamps[-1] - self.timestamps[0]

    def __str__(self) :
        return '{0} : {1} : {2}'.format(self.page, self.benchmark, self.timestamps)

# not a proper class, but...
class result_parser :
    def load(self, file, pages, benchmarks) :
        self.pages = pages
        self.benchmarks = benchmarks
        dom = xml.dom.minidom.parse(file)

        timedemo = dom.getElementsByTagName('timedemo')[0]
        pages = timedemo.getElementsByTagName('page')
        for page in pages :
            self.__handlePage(page)

    def __handlePage(self, page) :
        page_name = page.getAttribute('id')
        if self.pages and not page_name in self.pages :
            return
        benchmarks = page.getElementsByTagName('benchmark')
        for benchmark in benchmarks :
            self.__handleBenchmark(benchmark, page_name)

    def __handleBenchmark(self, benchmark, page_name) :
        benchmark_name = benchmark.getAttribute('id')
        if self.benchmarks and not (result.benchmark in self.benchmarks) :
            return

        frameItem = benchmark.getElementsByTagName('frames')[0].firstChild
        if frameItem :
            timestamps = map(int, frameItem.nodeValue.split(','))
        else :
            return

        if not benchmark_name in benchmark_names :
            benchmark_names.append(benchmark_name)
        result = benchmark_result(page_name, benchmark_name, timestamps)
        if not benchmarksByPageName.has_key(result.page) :
            benchmarksByPageName[result.page] = {}
        benchmarksByPageName[result.page][result.benchmark] = result
        if not benchmarksByBenchmarkName.has_key(result.benchmark) :
            benchmarksByBenchmarkName[result.benchmark] = {}
        benchmarksByBenchmarkName[result.benchmark][result.page] = result

benchmark_names = []
benchmarksByPageName = {}
benchmarksByBenchmarkName = {}
