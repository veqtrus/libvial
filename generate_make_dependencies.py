# Copyright (c) 2020 Pavlos Georgiou
#
# Permission is hereby granted, free of charge, to any person obtaining a copy of this
# software and associated documentation files (the "Software"), to deal in the Software
# without restriction, including without limitation the rights to use, copy, modify,
# merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
# permit persons to whom the Software is furnished to do so.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
# PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
# HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
# OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
# SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

import sys
import re

def header_dependencies(fname):
    deps = set()
    with open(fname) as fp:
        for line in fp:
            match = re.search('#\\s*include\\s*<(.+)>', line)
            if match:
                header = match.group(1)
                if header.startswith('vial/'):
                    deps.add('include/' + header)
    return deps

def main():
    deps = dict()
    first = True
    for fname in sys.argv:
        if first:
            first = False
            continue
        deps[fname] = header_dependencies(fname)
    while True:
        modified = False
        for fname, headers in deps.items():
            prev_len = len(headers)
            extra_deps = set()
            for header in headers:
                extra_deps |= header_dependencies(header)
            headers |= extra_deps
            modified = modified or len(headers) != prev_len
        if not modified:
            break
    for source, headers in deps.items():
        if len(headers) == 0:
            continue
        print(source[:-2] + '.o: ' + source + ' ' + ' '.join(headers))

if __name__ == '__main__':
    main()
