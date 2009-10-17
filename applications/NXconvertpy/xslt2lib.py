#!/usr/bin/env python
XSLT_PROC = "xsltproc"

def run(cmd, **kwargs):
  """Execute the supplied command and return the tuple (return value,
  stdout)"""
  shell = kwargs.get("shell", True)

  import subprocess as sub
  proc = sub.Popen(cmd, stdout=sub.PIPE, stderr=sub.STDOUT, shell=shell)
  (stdout, stderr) = proc.communicate()
  return_code = proc.wait()
  return (return_code, stdout)

def process(xml=None, xslt=None, output=None):
  if xml is None:
    raise Exception("xml file is None")
  if xslt is None:
    raise Exception("xslt file is None")
  command = "%s -o %s %s %s" % (XSLT_PROC, output, xslt, xml)
  print command
  (code, stdout) = run(command)
  print "RETURN:", code
  print "STDOUT:", stdout

if __name__ == "__main__":
  import os
  doc_root = "~/code/nexus-dfn/"
  xslt = os.path.join(doc_root, "xslt/nxdl2xsd.xsl")
  xml = os.path.join(doc_root, "base_classes/NXlog.nxdl.xml")
  process(xml, xslt, "deleteme")
