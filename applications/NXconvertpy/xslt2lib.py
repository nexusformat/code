#!/usr/bin/env python
NXCONVERT = "nxconvert"
XSLTPROC = "xsltproc"

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
    raise RuntimeError(NXCONVERT + " returned " + str(code))
  

def process(xml=None, xslt=None, output=None, verbose=0):
  if xml is None:
    raise Exception("xml file is None")
  if xslt is None:
    raise Exception("xslt file is None")
  command = "%s -o %s %s %s" % (XSLTPROC, output, xslt, xml)
  if verbose > 1:
    print command
  (code, stdout) = run(command)
  if verbose > 1:
    print stdout
  if code != 0:
    raise RuntimeError(XSLTPROC + " returned " + str(code))

def schematron2xslt(schematron=None, xslt=None, verbose=0):
  xslt1="iso_dsdl_include.xsl"
  xslt2="iso_abstract_expand.xsl"
  xslt3="iso_svrl_for_xsltn.xsl"

  (path, name) = os.path.split(schematron)
  schematron1 = name + ".step1"
  schematron2 = name + ".step2"

  process(schematron, xslt1, schematron1, verbose)
  process(schematron1, xslt2, schematron2, verbose)
  process(schematron2, xslt3, xslt, verbose)
  
  "xslt -stylesheet theSchema.xsl  myDocument.xml > myResult.xml"

if __name__ == "__main__":
  import os
  doc_root = "~/code/nexus-dfn/"
  VERBOSE = 2

  nexus = os.path.join(doc_root, "test/NXmonopd.hdf")
  reduced = "reducednexus"
  nxconvert("NXmonopd", nexus, reduced, VERBOSE)

  schematron = os.path.join(doc_root, "test/schematron.sch")
  schematron2xslt(schematron, "schema.xslt", VERBOSE)
