#!/usr/bin/env python

import os.path
import re
import subprocess

# Extract the version from README
readme_path = os.path.realpath(os.path.join(os.path.dirname(__file__), '..', 'README.md'))
with open(readme_path, 'r') as fp:
    version = re.search(r'Latest version: ([.\d+]+)', fp.read()).groups()[0]
    print('#define KERNEL_VERSION "%s"' % version)

# Parse the last commit hash
last_commit = subprocess.check_output('git rev-parse HEAD'.split()).strip()
print('#define _GIT_LAST_COMMIT "%s"' % last_commit)
