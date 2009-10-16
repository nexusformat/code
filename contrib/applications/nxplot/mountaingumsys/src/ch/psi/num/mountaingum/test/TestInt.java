/**
 * Testing the IntValue class.....
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, June 2007
 */
package ch.psi.num.mountaingum.test;
import ch.psi.num.mountaingum.tree.IntValue;
public class TestInt {
	public static void main(String argv[]){
		int dim[] = new int[1];
		dim[0] = 3;
		IntValue iv = new IntValue(1,dim);
		iv.setFromString("1 2 3");
		int d[] = iv.getData();
		d[0] = 5;
		d[1] = 6;
		d[2] = 7;
		iv.setData(d);
		System.out.println(iv.toString());
	}
}
