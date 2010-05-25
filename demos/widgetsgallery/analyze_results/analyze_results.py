# -*- coding: utf-8 -*-

import benchmark_results
from optparse import OptionParser
import sys
import os

try:
  import pylab
  pylab_available = True
except ImportError:
  pylab_available = False


def main() :
    options = parse_commandline_arguments()

    parser = benchmark_results.result_parser()
    parser.load(options.file, options.pages, options.benchmarks)

    if (not options.graphical) or options.textual :
        textual_output()

    if options.graphical :
        if not pylab_available :
            print "Pylab is not installed. Plotting is disabled."
            sys.exit(-1)
        if options.output_folder :
            if not os.path.isdir(options.output_folder) :
                os.makedirs(options.output_folder)

        generate_plots(options.output_folder)


def generate_plots(output_folder) :
    colors = ['b', 'g', 'r', 'c', 'm', 'y', 'b']
    characters = ['o', '^', '*']
    types = [col+char+'-' for char in characters for col in colors]

    for benchmark_name, results in benchmark_results.benchmarksByBenchmarkName.items() :
        i = 0
        pylab.clf()
        for page_name, result in results.items() :
            result.plot_fps(2, page_name, types[i % len(types)])
            i += 1

        pylab.xlabel('time (ms)')
        pylab.ylabel('frames per second')
        pylab.title(benchmark_name)
        pylab.grid(True)
        pylab.legend(loc=0)
        pylab.axis(xmin=0, ymin=0)

        if output_folder :
            pylab.savefig(os.path.join(output_folder, benchmark_name))
        else :
            pylab.show()


def textual_output() :
    fps_width = 4
    fps_unit_width = 3
    runtime_width = 5
    runtime_unit_width = 2
    whitespace = 0
    std_dev_width = 4
    benchmark_width = 3 + fps_width + fps_unit_width + whitespace + fps_width + fps_unit_width + whitespace + runtime_width + runtime_unit_width + whitespace + std_dev_width
    page_title_width = max(map(len, benchmark_results.benchmarksByPageName))

    print '{0:{1}}'.format('Page name', page_title_width),
    for name in benchmark_results.benchmark_names :
        print '| {0:{1}}'.format(name, max(benchmark_width, len(name))),
    print

    print '{0:{1}}'.format('', page_title_width),
    for name in benchmark_results.benchmarksByBenchmarkName :
        # mean fps
        print '| {0:{1}}'.format('mean', fps_width + fps_unit_width),
        # min fps
        print '{0:{1}}'.format('min', fps_width + fps_unit_width),
        # runtime
        print '{0:{1}}'.format('runtime', runtime_width + runtime_unit_width),
        # std dev
        print '{0:{1}}'.format('sdev', std_dev_width),
    print

    for page_name, results in benchmark_results.benchmarksByPageName.items() :
        print '{0:{1}}'.format(page_name, page_title_width),
        for benchmark_name in benchmark_results.benchmark_names :
            if results.has_key(benchmark_name) :
                result = results[benchmark_name]
                # mean fps
                print '| {0:>{1}.1f}fps'.format(result.fps_mean(), fps_width),
                # min fps
                print '{0:>{1}.1f}fps'.format(result.fps_min(), fps_width),
                # runtime
                print '{0:>{1}}ms'.format(result.runtime(), runtime_width),
                # std dev
                print '{0:>{1}.1f}'.format(result.fps_std_dev(), std_dev_width),
            else :
                print '|',
                print '{0:^{1}}'.format('n/a', 2 * fps_width + 2 * fps_unit_width + runtime_width + runtime_unit_width + std_dev_width + 3),
        print

def parse_commandline_arguments() :
    parser = OptionParser()
    parser.add_option('-f', '--file', dest='file', action='store', type='string', help='The file containing the timestamps')
    parser.add_option('-t', '--textual', dest='textual', action='store_true', help='Enable textual output')
    parser.add_option('-g', '--graphical', dest='graphical', action='store_true', help='Enable graphical output')
    parser.add_option('-o', '--output-folder', dest='output_folder', type='string', help='Folder to save graphical output into')
    parser.add_option('-p', '--pages', dest='pages', action='store', type='string', help='Select a subset of pages to evaluate. If this option is not set all pages will be used.')
    parser.add_option('-b', '--benchmarks', dest='benchmarks', action='store', type='string', help='Select a subset of benchmarks to evaluate. If this option is not set all benchmarks will be used.')

    (options, args) = parser.parse_args()
    if options.pages :
        options.pages = options.pages.split(',')
    if options.benchmarks :
        options.benchmarks = options.benchmarks.split(',')

    if not (options.file) :
        parser.print_help()
        sys.exit(-1)

    return options

if __name__ == '__main__' :
    main()
