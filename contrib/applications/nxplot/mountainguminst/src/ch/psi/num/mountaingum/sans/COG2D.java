/**
 * This is a little class which calculates the center of gravity for 
 * a 2 dimensional data set.
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, April 2008
 */
package ch.psi.num.mountaingum.sans;

public class COG2D {
	public double x, y, cogTotal;
	
	public void calculate(double data[], int xdim, int ydim){
		calculate(data,xdim,ydim,0,xdim,0,ydim);
	}
	public void calculate(double data[], int xdim, int ydim,
			int startx, int endx, int starty, int endy){
		int i, j, rowOffset, count = 0;
		double cogTotal = .0, cogX = .0, cogY = .0;
		
		if(startx < 0){
			startx = 0;
		}
		if(starty < 0){
			starty = 1;
		}
		if(endx >= xdim){
			endx = xdim -1;
		}
		if(endy >= ydim){
			endy = ydim -1;
		}
		
		for(i = startx; i < endx; i++){
			rowOffset = i * ydim;
			for(j = starty; j < endy; j++){
				cogTotal += data[rowOffset +j];
				cogX += i* data[rowOffset +j];
				cogY += j* data[rowOffset +j];
				count++;
			}
		}
		this.cogTotal = cogTotal;
		if(cogTotal > .0){
			x = cogX/cogTotal;
			y = cogY/cogTotal;
		} else {
			x = xdim/2.;
			y = ydim/2.;
		}
	}
}
