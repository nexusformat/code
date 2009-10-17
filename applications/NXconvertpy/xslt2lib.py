#!/usr/bin/env python
NXCONVERT = "nxconvert"
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

def nxconvert(definition=None, input=None, output=None, verbose=0):
  command = "%s -d %s %s" % (NXCONVERT, input, output)
  if verbose > 1:
    print command
  (code, stdout) = run(command)
  if verbose > 1:
    print stdout
  if code != 0:
    raise RuntimeError(NXCONVERT + " returned " + code)
  

def process(xml=None, xslt=None, output=None, verbose=0):
  if xml is None:
    raise Exception("xml file is None")
  if xslt is None:
    raise Exception("xslt file is None")
  command = "%s -o %s %s %s" % (XSLT_PROC, output, xslt, xml)
  if verbose > 1:
    print command
  (code, stdout) = run(command)
  if verbose > 1:
    print stdout
  if code != 0:
    raise RuntimeError(XSLT_PROC + " returned " + code)

if __name__ == "__main__":
  import os
  doc_root = "~/code/nexus-dfn/"

  xslt = os.path.join(doc_root, "xslt/nxdl2xsd.xsl")
  xml = os.path.join(doc_root, "base_classes/NXlog.nxdl.xml")
  process(xml, xslt, "transformednxdl")

  nexus = os.path.join(doc_root, "test/NXmonopd.hdf")
  nxconvert("NXmonopd", nexus, "reducednexus")