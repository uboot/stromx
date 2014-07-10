# -*- coding: utf-8 -*-

import stromx.runtime as sr

stream = sr.Stream()
stream.setName("My stream")
data = "dfdsfdsds\nkljöklöjkfldsjf"
factory = sr.Factory()

out = sr.ZipFileOutput("provider_test.zip")
#out = sr.DirectoryFileOutput(".")
out.initialize("filename")
out.openFile("txt", sr.OutputProvider.OpenMode.TEXT)
out.file().write(data)
sr.XmlWriter().writeStream(out, "stream", stream)
out.close()


inp = sr.ZipFileInput("provider_test.zip")
#inp = sr.DirectoryFileInput(".")
inp.initialize("", "filename.txt")
inp.openFile(sr.InputProvider.OpenMode.TEXT)
data = inp.file().read()

stream = sr.XmlReader().readStream(inp, "stream.xml", factory)
inp.close()

print stream.name()
print data