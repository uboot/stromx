# -*- coding: utf-8 -*-

import argparse
import os
import re
import shutil
import zipfile

parser = argparse.ArgumentParser()
parser.add_argument('directory', type=str, nargs=1)
group = parser.add_mutually_exclusive_group(required = True)
group.add_argument('--extract', action='store_true')
group.add_argument('--compress', action='store_true')

args = parser.parse_args()

for root, dirs, files in os.walk(args.directory[0]):
    if args.extract:
        for f in files:
            if re.match(r".+\.(zip|stromx)", f):
                path = os.path.join(root, f)
                try:
                    with zipfile.ZipFile(path, 'r') as archive:
                        dirname = "{0}.extracted".format(f)
                        dirpath = os.path.join(root, dirname)
                        os.mkdir(dirpath)
                        archive.extractall(dirpath)
                        print "Created {0}".format(dirpath)
                except zipfile.BadZipfile:
                    print "Failed to open {0}".format(path)
    else:
        for d in dirs:
            if re.match(r".+\.(zip|stromx).extracted", d):
                f = d[:-len(".extracted")]
                filePath = os.path.join(root, f)
                dirPath = os.path.join(root, d)
                with zipfile.ZipFile(filePath, 'w') as archive:
                    for member in os.listdir(dirPath):
                        memberPath = os.path.join(root, d, member)
                        archive.write(memberPath, member)
                shutil.rmtree(dirPath)
                print "Created {0}".format(filePath)