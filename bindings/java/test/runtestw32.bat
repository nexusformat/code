@echo off
REM------------------------------------------------------------------------
REM batch file for running the test program for the Java NeXus API
REM
REM Version for Windows32
REM NOTE: The HDF libraries must be in the path!
REM 
REM Mark Koennecke, October 2000
REM ------------------------------------------------------------------------
set JL=-Dneutron.nexus.JNEXUSLIB=..\jnexus\bin\win32\jnexus.dll
java -classpath C:\jdk1.1.5\lib\classes.zip;..\jnexus.jar;. %JL% TestJapi
 
