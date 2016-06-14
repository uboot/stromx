# -*- coding: utf-8 -*-

import argparse
import os
import re
import shutil
import zipfile

import stromx
import stromx.runtime

def extractArchive(filename, root):
    path = os.path.join(root, f)
    try:
        with zipfile.ZipFile(path, 'r') as archive:
            dirname = "{0}.extracted".format(filename)
            dirpath = os.path.join(root, dirname)
            os.mkdir(dirpath)
            archive.extractall(dirpath)
            print "Created {0}".format(dirpath)
    except zipfile.BadZipfile:
        print "Failed to open {0}".format(path)

def compressArchive(dirname, root):
    filename = dirname[:-len(".extracted")]
    filePath = os.path.join(root, filename)
    dirPath = os.path.join(root, dirname)
    with zipfile.ZipFile(filePath, 'w') as archive:
        for member in os.listdir(dirPath):
            memberPath = os.path.join(root, dirname, member)
            archive.write(memberPath, member)
    shutil.rmtree(dirPath)
    print "Created {0}".format(filePath)
    
def readArchiveData(zipInput, filename, mode):
    zipInput.initialize("", filename)
    data = None
    try:
        zipInput.openFile(mode)
        data = zipInput.file().read()
    except stromx.runtime.FileAccessFailed:
        pass
    return data
    
def writeArchiveData(zipOutput, basename, extension, mode, data):
    if not data:
        return
    zipOutput.initialize(basename)
    zipOutput.openFile(extension, mode)
    zipOutput.file().write(data)
    
def repackageArchive(filename, root):
    filePath = os.path.join(root, filename)
    TEXT_MODE_IN = stromx.runtime.InputProvider.OpenMode.TEXT
    BINARY_MODE_IN = stromx.runtime.InputProvider.OpenMode.BINARY
    TEXT_MODE_OUT = stromx.runtime.OutputProvider.OpenMode.TEXT
    BINARY_MODE_OUT = stromx.runtime.OutputProvider.OpenMode.BINARY
    try:
        zipInput = stromx.runtime.ZipFileInput(filePath)
        reader = stromx.runtime.XmlReader()
        stream = reader.readStream(zipInput, "stream.xml", factory)
        zipInput.initialize("", "views.json")
        viewData = readArchiveData(zipInput, "views.json", TEXT_MODE_IN)
        studioData = readArchiveData(zipInput, "stream.studio", BINARY_MODE_IN)
        studioGeometryData = readArchiveData(zipInput, "stream.studio.geometry",
                                             BINARY_MODE_IN)
        zipInput.close()
        
        writer = stromx.runtime.XmlWriter()
        zipOutput = stromx.runtime.ZipFileOutput(filePath)
        writer.writeStream(zipOutput, 'stream', stream)
        writeArchiveData(zipOutput, 'views', '.json', TEXT_MODE_OUT, viewData)
        writeArchiveData(zipOutput, 'stream', '.studio', BINARY_MODE_OUT,
                         studioData)
        writeArchiveData(zipOutput, 'stream.studio', '.geometry', 
                         BINARY_MODE_OUT, studioGeometryData)
        zipOutput.close()
        print "Repackaged {0}".format(filePath)
        
    except stromx.runtime.Exception as e:
        print "Failed to repackage {0}: {1}".format(filePath, e)

parser = argparse.ArgumentParser()
parser.add_argument('directory', type=str, nargs=1)
parser.add_argument('--packages', metavar='NAME', nargs='+', default=[],
                    help='extra packages basename, e.g. cvimgproc')
group = parser.add_mutually_exclusive_group(required = True)
group.add_argument('--extract', action='store_true')
group.add_argument('--compress', action='store_true')
group.add_argument('--repackage', action='store_true')

args = parser.parse_args()

if args.repackage:
    factory = stromx.runtime.Factory()
    stromx.register("libstromx_cvsupport.so", factory)
    stromx.register("libstromx_runtime.so", factory)
    stromx.register("libstromx_test.so", factory)
    for name in args.packages:
        stromx.register("libstromx_{0}.so".format(name), factory)

for root, dirs, files in os.walk(args.directory[0]):
    if args.compress:
        for d in dirs:
            if re.match(r".+\.(zip|stromx).extracted", d):
                compressArchive(d, root)
    else:
        for f in files:
            if re.match(r".+\.(zip|stromx)", f):
                if args.extract:
                    extractArchive(f, root)
                elif args.repackage:
                    repackageArchive(f, root)
                else:
                    assert(False)