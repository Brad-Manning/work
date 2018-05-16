#!/usr/bin/env bash

sed -i "
    /<CORSIKA>/,/<\/CORSIKA>/ s@<file>.*</file>@<file>new_branch_name</file>@;
    /<CORSIKA>/,/<\/CORSIKA>/ s@<saveLocation>.*</saveLocation>@<saveLocation>new_branch_label</saveLocation>@
" "CORSIKAParameters.xml"
