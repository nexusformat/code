   /**
     *                  A S C I I R e a d e r
     *
     * A class for reading SinQ ASCII data files. Data is entered in a 
     * dictionary on reading. From this dictionary data can be retrieved
     * using its keyword. There are a few system special dictionary entries:
     * NP is the number of points, Preset the preset and Mode the scan mode.
     * ScanVariables has as value a comma separated list of scan variables.
     * names is a comma separated list of all the variables recorded at each
     * scan point.
     *
     * Mark Koennecke, August 1998
     * 
     * Added getKeys() method
     * 
     * Mark Koennecke, September 2009
     */
package ch.psi.num.mountaingum.nexus.loader.sinqascii;

import java.util.Enumeration;
import java.util.Hashtable;
import java.util.Vector;
import java.util.StringTokenizer;
import java.io.InputStreamReader;
import java.io.BufferedReader;
import java.io.InputStream;
import java.io.IOException;

     public class ASCIIReader {

       /**
         * Variables 
       */
         protected BufferedReader bin;
         protected Hashtable datadict;
         private final static int debug = 0;
    
       /** 
         * A Constructor
       */
       public ASCIIReader(InputStream in)
       {
         bin = new BufferedReader(new InputStreamReader(in));
         datadict = new Hashtable();
       }

       /**
         * The Read method coordinates the work. It calls readHeader and
         * readData
       */
       public void read() throws IOException
       {
           readHeader();
           readData();
       }

       /**
         * readHader reads lines until it finds a string with ** DATA **.
         * If the line contains a = sign, the line is split there and
         * the two components added to the datadict as key value pair.
       */
       protected void readHeader() throws IOException
       {
          String line;
          StringTokenizer st;
          int iIndex;
          String key,value;

          line = bin.readLine();
          while(line.indexOf("*** DATA ***") < 0)
          {
              /* older TOPSI files do not have the = in all cases */
              line = fixTopsi(line);
              iIndex = line.indexOf('=');
              if(iIndex > 0)
              {
                 key = line.substring(0,iIndex);
                 key = key.trim();
                 value = line.substring(iIndex+1,line.length());
                 value = value.trim();
                 if(debug > 0)
                 {
                     System.out.println("Entering: " +
                       key + ", " +
                       value );
                 }
                 datadict.put(key,value);
             }
             line = bin.readLine();
          }
       }
       /**
         *
       */
       private String fixTopsi(String line)
       {
           int iIndex;
           String result;

           if(line.indexOf("Stardate:") > 0)
           {
             iIndex = line.indexOf(':');
             result = line.substring(0,iIndex) + "=" +
                      line.substring(iIndex+1, line.length()) ;
             return result;                
           }
           return line;
       }

       /**
         * readData reads the data header info, then the data and stores
         * them in the dictionary as Vectors.
       */
       protected void readData() throws IOException
       {
          int iIndex, i, i2;
          int iNP = 0, nData, iCount;
          String line, temp, snames;
          String names[];
          float data[][], f2[];
          Float f;

          /* get scan variables */
          line = bin.readLine();
          iIndex = line.indexOf("WARNING");
          if(iIndex >=0 )
	  {
            line = bin.readLine();
          }
          iIndex = line.indexOf(':');
          datadict.put("ScanVariables",
               line.substring(iIndex+1,line.length()));

          /* get np, mode & preset */
          line = bin.readLine();
          StringTokenizer st = new StringTokenizer(line,",",false);
          if(st.countTokens() < 3)
          {
              throw new IOException("Invalid data file format");
          }
          temp = st.nextToken();
          iIndex = temp.indexOf(' ');
          Integer ig = new Integer(0);
          try {
              ig = new Integer(temp.substring(0,iIndex));
              iNP = ig.intValue();
          } catch(Exception egolf) {
            throw new IOException("Invalid data file format"); 
          }
          if(debug > 0)
          {
              System.out.println("Found NP as: " + iNP);
          }
          datadict.put("NP",ig.toString());
          temp = st.nextToken();
          iIndex = temp.indexOf(':');
          datadict.put("ScanMode",
                      temp.substring(iIndex+1,temp.length()));
          temp = st.nextToken();
          iIndex = temp.indexOf(' ');
          datadict.put("Preset",
             temp.substring(iIndex+1,temp.length()));

          /* get data item names */
          line = bin.readLine();
          st = new StringTokenizer(line," ",false);          
          nData = st.countTokens();
          names = new String[nData-1]; /* do not get NP */
          st.nextToken(); /* skip NP */
          for(i = 0; i < nData-1; i++)
          {
              names[i] = st.nextToken();
          }
          data = new float[nData][iNP];

          /* after having set up, read the actual data */
          if(debug > 0)
          {
             System.out.println("Starting to read Scan data");
          }
          for(i = 0; i < iNP; i++)
          {
             line = bin.readLine();
             /* handle it gracefully, if the user aborted the scan */
             if(line.equals("END-OF-DATA"))
             {
                 iNP = i-1;
                 datadict.remove("NP");
                 ig = new Integer(iNP);
                 datadict.put("NP",ig.toString());
                 break;            
             }
             st = new StringTokenizer(line," ",false);
             /* skip NP as first column */
             st.nextToken();
             for(i2 = 0; i2 < nData -1; i2++)
             {
               temp = st.nextToken();
               try {
                   f = new Float(temp);
                   data[i2][i] = f.floatValue();
                   if(debug > 3)
                   {
                       System.out.println("assigning: " + f.floatValue() +
                                 "to " + i2 + "," +i);
                   }
                }catch(Exception rudolf) {
                  if(debug > 0) 
                  {
                      System.out.println("Failure to convert number from "
                                         +temp);
                      System.out.println(rudolf.getMessage());
                  }
                  data[i2][i] = (float)0.0;
                }
             }
          }
          if(debug > 0)
          {
              System.out.println("Read all scan points");
          }

          /* done reading, stow away */
          snames = new String();
          for(i = 0; i < nData-1; i++)
          {
              snames = snames.concat(names[i]);
              snames = snames.concat(",");
              f2 = new float[iNP];
              for(i2 = 0; i2 < iNP; i2++)
              {
                 f2[i2] = data[i][i2];
              }
              datadict.put(names[i],f2);
          }
          /* remove last komma in snames */
          iIndex = snames.lastIndexOf(',');
          snames = snames.substring(0,iIndex);
         
          datadict.put("names",snames);
       }

     /**
       * getValue gets the object requested as key from the dictionary.
     */
      public Object getValue(String key)
      {
          return datadict.get(key);
      } 

      public Enumeration getKeys(){
    	  return datadict.keys();
      }
      /**
        * a finalizer
      */
       protected void finalize() throws Throwable
       {
           if(bin != null)
           {
               bin.close();
               bin = null;
           }
           datadict = null;
       }
     } /* end of class definition */
