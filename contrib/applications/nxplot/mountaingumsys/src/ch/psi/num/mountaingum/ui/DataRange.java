/**
 * This is just a little class for holding the range of a data set.
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, August 2007
 */
package ch.psi.num.mountaingum.ui;

public class DataRange {
	public double min, max;
	
	public DataRange(){
		min = Double.MAX_VALUE;
		max =  Double.MIN_VALUE;
	}
	public void calculate(double d[]){
		min = Double.MAX_VALUE;
		max = -min;
		for(int i = 0; i < d.length; i++){
			if(d[i] < min){
				min = d[i];
			}
			if(d[i] > max){
				max = d[i];
			}
		}
	}
	public boolean isValid(){
		if(min == Double.MAX_VALUE || max == Double.MIN_VALUE){
			return false;
		} 
		return true;
	}
}
