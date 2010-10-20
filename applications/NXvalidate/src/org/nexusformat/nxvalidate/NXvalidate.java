/* NeXus - Neutron & X-ray Common Data Format
 *
 * NeXus file validation GUI tool.
 *
 * Copyright (C) 2010 Nexus Team
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * For further information, see <http://www.nexusformat.org/>
 *
 * NXvalidate.java
 *
 */
package org.nexusformat.nxvalidate;

import java.io.File;
import java.util.ArrayList;
import java.util.logging.Level;
import java.util.logging.Logger;

public class NXvalidate {

    static final String VERSION = "0.1 alpha";
    private ArrayList<File> files;
    private File schematronFile;
    private boolean keepTemp;
    private boolean convertNxs;
    private int verbose;
    private ArrayList<Report> reports;
    private File reduced = null;

    NXvalidate() {
        files = new ArrayList<File>();
        this.schematronFile = null;
        this.keepTemp = false;
        this.convertNxs = true;
        this.verbose = 0;
        this.reports = new ArrayList<Report>();
    }

    public ArrayList<Report> getReports() {
        return reports;
    }

    public ArrayList<File> getFilenames() {
        return files;
    }

    public void setFilenames(ArrayList<String> filenames) {
        this.files = files;
    }

    public void setNXSFile(File file) {
        files.add(file);
    }

    public File getSchematron() {
        return schematronFile;
    }

    public void setSchematron(File schematronFile) {
        this.schematronFile = schematronFile;
    }

    public boolean isKeepTemp() {
        return keepTemp;
    }

    public void setKeepTemp(boolean keepTemp) {
        this.keepTemp = keepTemp;
    }

    public boolean isConvertNxs() {
        return convertNxs;
    }

    public void setConvertNxs(boolean convertNxs) {
        this.convertNxs = convertNxs;
    }

    public int getVerbose() {
        return verbose;
    }

    public void setVerbose(int verbose) {
        this.verbose = verbose;
    }

    public static String getVersion() {
        return VERSION;
    }

    public File getReduced() {
        return reduced;
    }

    public File setReduced(File reduced) {
        return this.reduced = reduced;
    }

    void parseArgs(final String[] args) {
        // check that the help and version arguments aren't specified
        for (int i = 0; i < args.length; i++) {
            if (args[i].equals("-h") || args[i].equals("--help")) {
                this.printHelp(2);
                System.exit(0);
            }
            if (args[i].equals("--version")) {
                this.printVersion();
                System.exit(0);
            }
        }

        // go through the arguments for real
        for (int i = 0; i < args.length; i++) {
            if (args[i].equals("-v") || args[i].equals("--verbose")) {
                this.verbose += 1;
            } else if (args[i].equals("-k") || args[i].equals("--keep")) {
                this.keepTemp = true;
            } else if (args[i].equals("-d") || args[i].equals("--dfn")) {
                schematronFile = new File(args[i + 1]);
                i++;
            } else if (args[i].equals("--noconvert")) {
                this.convertNxs = false;
            } else {
                files.add(new File(args[i]));
            }
        }
       

        // confirm that the manditory arguments are there
        if (this.files.size() <= 0) {
            System.out.println("Must specify at least one nexus file");
            this.printHelp(0);
            System.exit(-1);
        }
        if (!schematronFile.exists()) {
            System.out.println("Must specify a schematron file");
            this.printHelp(0);
            System.exit(-1);
        }
    }

    

    void process() {
        if (this.verbose > 0) {
            System.out.println("Running NXvalidate (version:" + VERSION + ")");
        }
        int size = this.files.size();
        for (int i = 0; i < size; i++) {
            this.process(this.files.get(i));
        }
    }

    private static File toAbsFile(final String filename) {
        File file = new File(filename);
        return file;
    }

    private File process(final File file) throws Error {

        File result = null;

        if (convertNxs) {
            try {
                NXconvert converter = new NXconvert(file, keepTemp,null);
                reduced = converter.convert();
            } catch (Exception e) {
                Logger.getLogger(NXvalidate.class.getName()).log(Level.SEVERE,
                        "While converting \"" + file +
                        "\" to reduced xml format",e);
                throw new Error("While converting \"" + file +
                        "\" to reduced xml format", e);
            }
        }

        if (reduced != null && schematronFile !=null) {
            
            // create the validation setup
            NXschematron schematron = new NXschematron(file,reduced,
                    schematronFile, keepTemp);

            try {
                result = schematron.validate();
            } catch (Exception e) {
                Logger.getLogger(NXvalidate.class.getName()).log(Level.SEVERE,
                        "While creating validation report",e);
                throw new Error("While creating validation report", e);
            }

            // create the report
            Report report = null;
            try {
                report = new Report(reduced, result);
            } catch (Exception e) {
                Logger.getLogger(NXvalidate.class.getName()).log(Level.SEVERE,
                        "While generating the report object",e);
                throw new Error("While generating the report object", e);
            }

            // Add to vector of reports (one for each input file)
            reports.add(report);

            report.printTree();
            int numErrors = report.numErrors();
            if (numErrors > 0) {
                report.printReport();
            }

        }
        return result;
    }

    private void printVersion() {
        System.out.println("NXvalidate version " + VERSION);
    }

    private void printHelp(final int level) {
        System.out.println("usage: nxvalidate [options] <nxsfile>");
        if (level <= 0) {
            return;
        }

        System.out.println();
        System.out.println("Validate nexus files against the nexus definitions");
        this.printVersion();
        if (level <= 1) {
            return;
        }

        System.out.println();
        System.out.println("-h, --help    print this help information");
        System.out.println("-v, --verbose increase verbose printing");
        System.out.println("-d, --dfn     specify the definition file");
        System.out.println("-k, --keep    keep temporary files");
        System.out.println("--noconvert   do not reduce the nexus file");
    }

    public static void main(String[] args) {
        NXvalidate validate = new NXvalidate();
        validate.parseArgs(args);
        validate.process();
    }
}
