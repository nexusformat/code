/**
  * NXlink is a replacement for the structure holding the data necessary
  * for doing a link in the NeXus-API. Consequently it is primitive.
  *
  * Mark Koennecke, October 2000
  * 
  * updated for Napi-2.0 with HDF-5
  * Mark Koennecke, August 2001
  *
  * copyright: see accomanying COPYRIGHT file 
  */
package neutron.nexus;

public class NXlink {
    public int tag, ref;
    public String ref5, refd;
   
    public NXlink(){
       ref5 = new String("");
       refd = new String("");
    }
}
