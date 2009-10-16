/**
 * This is a calculation class which calculates the radial average of a 
 * 2D detector.
 * 
 * This works as follows:
 *  - On setup the distance between the center and the four edges of the data are calculated. 
 *    The largest of these is the range of the radial average. 
 *  - From this range the binning will be calculated and stored in squaredDist. The bin size 
 *    will be the x pixel size.
 *  - For each data point: 
 *  -- the distance will be calculated and the right bin for this distance found
 *  -- The counts for this point will be added to counts and noBins for this bin 
 *     will be incremented.
 *  - What we really want is then counts divided by the number of contributing 
 *    bins in noBins     
 * Internally we calculate with squared distances, saves on Math.sqrt calls.
 *  
 * copyright: GPL
 * 
 * Mark Koennecke, April 2008
 */
package ch.psi.num.mountaingum.sans;

import java.util.Arrays;

public class RadialAverage {
	protected int xcenter, ycenter, xdim, ydim;
    protected int nBins;
	
	protected double squaredDist[], countsSum[];
	protected int noBins[];
	
	protected double distances[], counts[];
	
	protected double xpixel, ypixel;
	
	public RadialAverage(){
		nBins = 10;
		squaredDist = new double[nBins];
		countsSum = new double[nBins];
		noBins = new int[nBins];
		distances = new double[nBins];
		counts = new double[nBins];
		xcenter = -1;
		ycenter = -1;
		xpixel = ypixel = 1.;
	}
	
	public void setCenter(int x, int y){
		xcenter = x;
		ycenter = y;
	}
	public void setPixelSize(double x, double y){
		xpixel = x;
		ypixel = y;
	}
	
	public void calculate(double data[], int xdim, int ydim) throws Exception{
		if(xdim*ydim > data.length){
			throw new Exception("dimension to data mismatch");
		}
		setup(xdim,ydim);
		processData(data);
	}

	private void processData(double[] data) {
		int i, j, idx, rowOffset;
		double diff, binWidth;
		
		/*
		 * the grand loop
		 */
		binWidth = squaredDist[1] - squaredDist[0];
		for(i = 0; i < xdim; i++){
			rowOffset = i*ydim;
			for(j = 0; j < ydim; j++){
				diff = centerDist(i,j);
				//idx = Arrays.binarySearch(squaredDist,diff);
				idx = (int)Math.floor(diff/binWidth);
				if(idx < 0 || idx >= squaredDist.length){
					System.out.println("Look at this: internal error which should not happen, idx = " +
						idx + ", max = " + squaredDist.length);
					continue;
				}
				noBins[idx]++;
				countsSum[idx] += data[rowOffset+j];
			}
		}
		
		/* 
		 * normalise the final result
		 */
		for(i = 0; i < nBins; i++){
			if(noBins[i] > 0){
				counts[i] = countsSum[i]/noBins[i];
			} else {
				counts[i] = .0;
			}
		}
	}

	private void setup(int xdim2, int ydim2) {
		double range = -Double.MAX_VALUE, dist, binWidth;
		int i;
		
		xdim = xdim2;
		ydim = ydim2;
		if(xcenter < 0){
			xcenter = xdim/2;
			ycenter = ydim/2;
		}

		if((dist = centerDist(0,0)) > range){
			range = dist;
		}
		if((dist = centerDist(0,ydim)) > range){
			range = dist;
		}
		if((dist = centerDist(xdim,ydim)) > range){
			range = dist;
		}
		if((dist = centerDist(xdim,0)) > range){
			range = dist;
		}
		
		nBins = (int)Math.floor(range/(xpixel*xpixel*ypixel*ypixel)) + 1;
		binWidth = range/((double)nBins);
		nBins++;
		
		noBins = new int[nBins];
		squaredDist = new double[nBins];
		countsSum = new double[nBins];
		distances = new double[nBins];
		counts = new double[nBins];
		
		for(i = 0; i < nBins; i++){
			squaredDist[i] = i*binWidth;
			distances[i] = Math.sqrt(squaredDist[i]);
			countsSum[i] = .0;
			noBins[i] = 0;
		}
	}

	private double centerDist(int i, int j) {
		double xdiff, ydiff;
		xdiff = ((double)i - xcenter)*xpixel;
		ydiff = ((double)j - ycenter)*ypixel;
		return xdiff*xdiff + ydiff*ydiff;
	}

	public double[] getCounts() {
		return counts;
	}

	public double[] getDistances() {
		return distances;
	}
	public double[] getQ(double lambda, double distance) 
		throws Exception{
		double result[] = new double[distances.length];
		int i;
		double fac;
		
		if( lambda < .1 || distance < .1){
			throw new Exception("RadialAverage cannot calculate Q, bad lambda or distance");
		}
		fac = (Math.PI*4.)/lambda;
		for(i = 0; i < result.length; i++){
			result[i] = fac*Math.sin(Math.atan(distances[i]/distance));
		}
		return result;
	}
}
