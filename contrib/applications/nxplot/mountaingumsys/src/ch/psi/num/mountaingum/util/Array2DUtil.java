/**
 * This is a utility class for defining some array manipulation methods. This will be 
 * used to rotate, mirror or otherwise manipulate 2D arrays in order to create an image 
 * which satisfies the instrument scientists. Please note that I usally work with 1D 
 * arrays which in reality are 2D arrays. 
 * 
 * copyright: see file COPYRIGHT
 * 
 * Mark Koennecke, March 2007
 */
package ch.psi.num.mountaingum.util;

public class Array2DUtil {
	/*
	 * rotate the array by 90 degree which is synonymous with swapping 
	 * the dimensions.
	 * @param src The source array with dims oriXdim, oriYDim
	 * @param target The target array where the data will be oriYDim, oriXdim
	 * @param oriXdim the xsize of src
	 * @param oriYdim the ysize of src.
	 */
	public void rotate90(double src[], double target[], int oriXdim, int oriYdim)
		throws Exception {
		int indSrc, indTarget, x, y;
		
		if(src.length != target.length){
			throw new Exception("Data size mismatch");
		}
		
		//System.out.println("Dimensions, x, y: " + oriXdim + ", " + oriYdim);
		for(y = 0; y < oriYdim; y++){
			for(x = 0; x < oriXdim; x++){
				indSrc = calcIndex(x,y,oriXdim, oriYdim);
				indTarget = calcIndex(y,x,oriYdim, oriXdim);
				//System.out.println("Mapping " + indSrc + " to " + indTarget);
				target[indTarget] = src[indSrc];
			}
		}
	}
	/*
	 * mirorY the dimensions. Puts y0 on top ymax to 0
	 * @param src The source array with dims oriXdim, oriYDim
	 * @param target The target array where the data will be mirrored
	 * @param oriXdim the xsize of src
	 * @param oriYdim the ysize of src.
	 */
	public void mirrorY(double src[], double target[], int oriXdim, int oriYdim)
		throws Exception {
		int indSrc, indTarget, x, y;
		
		if(src.length != target.length){
			throw new Exception("Data size mismatch");
		}
		
		for(y = 0; y < oriYdim; y++){
			for(x = 0; x < oriXdim; x++){
				indSrc = calcIndex(x,y,oriXdim, oriYdim);
				indTarget = calcIndex(x,oriYdim - y,oriXdim, oriYdim);
				target[indTarget] = src[indSrc];
			}
		}
	}
	/*
	 * mirorX the dimensions. Puts x0 to xmax and vice versa 
	 * @param src The source array with dims oriXdim, oriYDim
	 * @param target The target array where the data will be mirrored
	 * @param oriXdim the xsize of src
	 * @param oriYdim the ysize of src.
	 */
	public void mirrorX(double src[], double target[], int oriXdim, int oriYdim)
		throws Exception {
		int indSrc, indTarget, x, y;
		
		if(src.length != target.length){
			throw new Exception("Data size mismatch");
		}
		
		for(y = 0; y < oriYdim; y++){
			for(x = 0; x < oriXdim; x++){
				indSrc = calcIndex(x,y,oriXdim, oriYdim);
				indTarget = calcIndex(oriXdim - x -1,y,oriXdim, oriYdim);
				target[indTarget] = src[indSrc];
			}
		}
	}
	/*
	 * rotate the array by 90 degree which is synonymous with swapping 
	 * the dimensions.
	 * @param src The source array with dims oriXdim, oriYDim
	 * @param target The target array where the data will be oriYDim, oriXdim
	 * @param oriXdim the xsize of src
	 * @param oriYdim the ysize of src.
	 */
	public void rotate90(int src[], int target[], int oriXdim, int oriYdim)
		throws Exception {
		int indSrc, indTarget, x, y;
		
		if(src.length != target.length){
			throw new Exception("Data size mismatch");
		}
		
		//System.out.println("Dimensions, x, y: " + oriXdim + ", " + oriYdim);
		for(y = 0; y < oriYdim; y++){
			for(x = 0; x < oriXdim; x++){
				indSrc = calcIndex(x,y,oriXdim, oriYdim);
				indTarget = calcIndex(y,x,oriYdim, oriXdim);
				//System.out.println("Mapping " + indSrc + " to " + indTarget);
				target[indTarget] = src[indSrc];
			}
		}
	}
	/*
	 * mirorY the dimensions. Puts y0 on top ymax to 0
	 * @param src The source array with dims oriXdim, oriYDim
	 * @param target The target array where the data will be mirrored
	 * @param oriXdim the xsize of src
	 * @param oriYdim the ysize of src.
	 */
	public void mirrorY(int src[], int target[], int oriXdim, int oriYdim)
		throws Exception {
		int indSrc, indTarget, x, y;
		
		if(src.length != target.length){
			throw new Exception("Data size mismatch");
		}
		
		for(y = 0; y < oriYdim; y++){
			for(x = 0; x < oriXdim; x++){
				indSrc = calcIndex(x,y,oriXdim, oriYdim);
				indTarget = calcIndex(x,oriYdim - y,oriXdim, oriYdim);
				target[indTarget] = src[indSrc];
			}
		}
	}
	/*
	 * mirorX the dimensions. Puts x0 to xmax and vice versa 
	 * @param src The source array with dims oriXdim, oriYDim
	 * @param target The target array where the data will be mirrored
	 * @param oriXdim the xsize of src
	 * @param oriYdim the ysize of src.
	 */
	public void mirrorX(int src[], int target[], int oriXdim, int oriYdim)
		throws Exception {
		int indSrc, indTarget, x, y;
		
		if(src.length != target.length){
			throw new Exception("Data size mismatch");
		}
		
		for(y = 0; y < oriYdim; y++){
			for(x = 0; x < oriXdim; x++){
				indSrc = calcIndex(x,y,oriXdim, oriYdim);
				indTarget = calcIndex(oriXdim - x -1,y,oriXdim, oriYdim);
				target[indTarget] = src[indSrc];
			}
		}
	}
	/**
	 * make a projection on the x axis
	 * @param srcData The data to project from
	 * @param xdim x dimension
	 * @param ydim y dimension
	 * @param xstart x index to start projection
	 * @param xend y index to end projection
	 * @param ystart y index to starting summing
	 * @param yend y index of end of summing
	 */
	public double[] projectX(double srcData[], double projData[],
			int xdim, int ydim, int xstart, int xend, 
			int ystart, int yend){
		int i, j, k, idx;
		
		int length = xend - xstart;
		for(i = xstart, k = 0; i < xend; i++, k++){
			projData[k] = .0;
			for(j = ystart; j < yend; j++){
				idx = calcIndex(i,j,xdim,ydim);
				projData[k] += srcData[idx];
			}
		}
		return projData;
	}
	/**
	 * make a projection on the Y axis
	 * @param srcData The data to project from
	 * @param xdim x dimension
	 * @param ydim y dimension
	 * @param xstart x index to start summing
	 * @param xend y index to end summing
	 * @param ystart y index to starting projecting
	 * @param yend y index to end projecting
	 */
	public double[] projectY(double srcData[], double projData[],
			int xdim, int ydim, int xstart, int xend, 
			int ystart, int yend){
		int i, j, k, idx;
		
		int length = yend - ystart;
		for(i = ystart, k = 0; i < yend; i++, k++){
			projData[k] = .0;
			for(j = xstart; j < xend; j++){
				idx = calcIndex(j,i,xdim,ydim);
				projData[k] += srcData[idx];
			}
		}
		return projData;
	}
	
	/**
	 * calculate the index of a data item in the array
	 * @param x The x coordinate
	 * @param y  The y coordinate
	 * @param xdim The x size
	 * @param ydim The y size
	 * @return An index into the array.
	 */
	private int calcIndex(int x, int y, int xdim, int ydim){
		int address = 0;
		
		address = y * xdim + x;
		return address;
	}
}
