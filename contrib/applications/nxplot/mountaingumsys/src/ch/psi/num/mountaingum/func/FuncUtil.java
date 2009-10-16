/**
 * a utility class which implements various functional strategies
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, June 2007
 */
package ch.psi.num.mountaingum.func;
import java.util.LinkedList;
import java.util.List;

public class FuncUtil {

	public static List map(IStream in, IFunc func){
		Object arg, out;
		List l = new LinkedList();
		
		while((arg = in.next()) != null){
			out = func.apply(arg);
			if(out != null){
				l.add(out);
			}
		}
		return l;
	}
}
