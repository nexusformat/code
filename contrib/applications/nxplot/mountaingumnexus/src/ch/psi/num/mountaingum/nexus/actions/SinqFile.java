/**
 * This is a tiny utility class for finding the filename of a SINQ data 
 * file. It first tries to locate the file in the local data area and 
 * if this fails, at sinqdata. May be this gets extended to download the file 
 * via http in a later stage. As this may be used elsewhere, this goes into 
 * a little utility class.
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, August 2009
 */
package ch.psi.num.mountaingum.nexus.actions;

import java.io.File;
import java.text.NumberFormat;

public class SinqFile {
	public static String getSinqFile(String instrument, int year, int numor, String extension){
		StringBuffer  file, path;
		String yy, numString, hundred;
		File f;
		
		/**
		 * build bits and pieces
		 */
		NumberFormat nf = NumberFormat.getInstance();
		nf.setGroupingUsed(false);
		nf.setMinimumIntegerDigits(4);
		yy = nf.format(year);
		nf.setMinimumIntegerDigits(6);
		numString = nf.format(numor);
		nf.setMinimumIntegerDigits(3);
		hundred = nf.format(numor/1000);
		
		
		/**
		 * build file name
		 */
		file = new StringBuffer(instrument.toLowerCase());
		file.append(yy);
		file.append('n');
		file.append(numString);
		file.append(extension);
		//System.out.println(file.toString());
		
		/**
		 * test local
		 */
		path = new StringBuffer("/home/");
		path.append(instrument);
		path.append("/data/");
		path.append(yy);
		path.append('/');
		path.append(hundred);
		path.append('/');
		path.append(file.toString());
		//System.out.println(path.toString());
		
		f = new File(path.toString());
		if(f.canRead()){
			return path.toString();
		}

		/**
		 * test sinqdata
		 */
		path = new StringBuffer("/afs/psi.ch/project/sinqdata/");
		path.append(yy);
		path.append('/');
		path.append(instrument);
		path.append('/');
		path.append(hundred);
		path.append('/');
		path.append(file.toString());
		//System.out.println(path.toString());
		
		f = new File(path.toString());
		if(f.canRead()){
			return path.toString();
		}
		return null;
	}
}
