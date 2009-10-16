/*
 * ArrayTools.java
 * Copyright (c) 2004, KTH NADA.
 *
 * This file is part of SweSum^2 (see http://swesum.nada.kth.se/),
 * and is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, see
 * http://opensource.org/licenses/gpl-license.php
 *
 * Martin Hassel, 2004-feb-01
 * http://www.nada.kth.se/~xmartin/
 *
 */

//package moj.util;
package ch.psi.num.mountaingum.util;

/**
 * A small utility class that contains methods for converting between strings 
 * and arrays, as well as increasing and decreasing the size of arrays.
 * 
 * @author Martin Hassel
 * @version 2004-Oct-18
 */

public final class ArrayTools {
	
	//// String to Array methods ////
	
	/**
	 * Converts a string of numbers to a byte array. Observe that a byte
	 * ranges from -127 to 127
	 *
	 * @param str String of numbers to convert
	 * @param fieldSeparator String that separates each number
	 * @return an array of bytes
	 */
	public static byte[] splitBytes(String str, String fieldSeparator){
		String[] fields = str.split(fieldSeparator);
		byte[] tmp = new byte[fields.length];
		for(int i = 0; i < tmp.length; i++)
			tmp[i] = Byte.parseByte(fields[i]);
		return tmp;
	}
	
	/**
	 * Converts a string of numbers to a short array
	 *
	 * @param str String of numbers to convert
	 * @param fieldSeparator String that separates each number
	 * @return an array of shorts
	 */
	public static short[] splitShorts(String str, String fieldSeparator){
		String[] fields = str.split(fieldSeparator);
		short[] tmp = new short[fields.length];
		for(int i = 0; i < tmp.length; i++)
			tmp[i] = Short.parseShort(fields[i]);
		return tmp;
	}
	
	/**
	 * Converts a string of numbers to an integer array
	 *
	 * @param str String of numbers to convert
	 * @param fieldSeparator String that separates each number 
	 * @return an array of integers
	 */
	public static int[] splitInts(String str, String fieldSeparator){
		String[] fields = str.split(fieldSeparator);
		int[] tmp = new int[fields.length];
		for(int i = 0; i < tmp.length; i++)
			tmp[i] = Integer.parseInt(fields[i]);
		return tmp;
	}
	
	/**
	 * Converts a string of numbers to a long array
	 *
	 * @param str String of numbers to convert
	 * @param fieldSeparator String that separates each number
	 * @return an array of shorts
	 */
	public static long[] splitLongs(String str, String fieldSeparator){
		String[] fields = str.split(fieldSeparator);
		long[] tmp = new long[fields.length];
		for(int i = 0; i < tmp.length; i++)
			tmp[i] = Long.parseLong(fields[i]);
		return tmp;
	}
	
	/**
	 * Convert a string of reals to a float array.
	 *
	 * @param str String of numbers to convert
	 * @param fieldSeparator String that separates each number
	 * @return an array of floats
	 */
	public static float[] splitFloats(String str, String fieldSeparator){
		String[] fields = str.split(fieldSeparator);
		float[] tmp = new float[fields.length];
		for(int i = 0; i < tmp.length; i++)
			tmp[i] = Float.parseFloat(fields[i]);
		return tmp;
	}
	
	/**
	 * Convert a string of reals to a double array.
	 *
	 * @param str String of numbers to convert
	 * @param fieldSeparator String that separates each number
	 * @return an array of doubles
	 */
	public static double[] splitDoubles(String str, String fieldSeparator){
		String[] fields = str.split(fieldSeparator);
		double[] tmp = new double[fields.length];
		for(int i = 0; i < tmp.length; i++)
			tmp[i] = Double.parseDouble(fields[i]);
		return tmp;
	}
	
	//// Re-sizing methods ////
	
	/**
	 * Resizes an array of bytes. If the new size
	 * is smaller only newSize elements will be copied.
	 *
	 * @param a the original array
	 * @param newSize the size of the new array
	 * @return a new array with the original elements in a copied to it
	 */
	public static byte[] setSize(byte[] a, int newSize) {
		if(newSize < 0) throw new IllegalArgumentException("Negative array size");
		byte[] v = new byte[newSize];
		System.arraycopy(a,0,v,0,newSize > a.length ? a.length : newSize);
		return v;
	}
	
	/**
	 * Resizes an array of chars. If the new size
	 * is smaller only newSize elements will be copied.
	 *
	 * @param a the original array
	 * @param newSize the size of the new array
	 * @return a new array with the original elements in a copied to it
	 */
	public static char[] resize(char[] a, int newSize) {
		if(newSize < 0) throw new IllegalArgumentException("Negative array size");
		char[] v = new char[newSize];
		System.arraycopy(a,0,v,0,newSize > a.length ? a.length : newSize);
		return v;
	}
	
	/**
	 * Resizes an array of shors. If the new size
	 * is smaller only newSize elements will be copied.
	 *
	 * @param a the original array
	 * @param newSize the size of the new array
	 * @return a new array with the original elements in a copied to it
	 */
	public static short[] resize(short[] a, int newSize) {
		if(newSize < 0) throw new IllegalArgumentException("Negative array size");
		short[] v = new short[newSize];
		System.arraycopy(a,0,v,0,newSize > a.length ? a.length : newSize);
		return v;
	}
	
	/**
	 * Resizes an array of ints. If the new size
	 * is smaller only newSize elements will be copied.
	 *
	 * @param a the original array
	 * @param newSize the size of the new array
	 * @return a new array with the original elements in a copied to it
	 */
	public static int[] resize(int[] a, int newSize) {
		if(newSize < 0) throw new IllegalArgumentException("Negative array size");
		int[] v = new int[newSize];
		System.arraycopy(a,0,v,0,newSize > a.length ? a.length : newSize);
		return v;
	}
	
	/**
	 * Resizes an array of longs. If the new size
	 * is smaller only newSize elements will be copied.
	 *
	 * @param a the original array
	 * @param newSize the size of the new array
	 * @return a new array with the original elements in a copied to it
	 */
	public static long[] resize(long[] a, int newSize) {
		if(newSize < 0) throw new IllegalArgumentException("Negative array size");
		long[] v = new long[newSize];
		System.arraycopy(a,0,v,0,newSize > a.length ? a.length : newSize);
		return v;
	}
	
	/**
	 * Resizes an array of floats. If the new size
	 * is smaller only newSize elements will be copied.
	 *
	 * @param a the original array
	 * @param newSize the size of the new array
	 * @return a new array with the original elements in a copied to it
	 */
	public static float[] resize(float[] a, int newSize) {
		if(newSize < 0) throw new IllegalArgumentException("Negative array size");
		float[] v = new float[newSize];
		System.arraycopy(a,0,v,0,newSize > a.length ? a.length : newSize);
		return v;
	}
	
	/**
	 * Resizes an array of doubles. If the new size
	 * is smaller only newSize elements will be copied.
	 *
	 * @param a the original array
	 * @param newSize the size of the new array
	 * @return a new array with the original elements in a copied to it
	 */
	public static double[] resize(double[] a, int newSize) {
		if(newSize < 0)
			throw new IllegalArgumentException("Negative array size");
		double[] v = new double[newSize];
		System.arraycopy(a,0,v,0,newSize > a.length ? a.length : newSize);
		return v;
	}
	
	/**
	 * Resizes an array of Objects. If the new size
	 * is smaller overflowing elements will be discarded.
	 *
	 * @param a the original array
	 * @param newSize the size of the new array
	 * @return a new array with the original elements in a copied to it
	 */
	public static Object[] resize(Object[] a, int newSize) {
		if(newSize < 0) throw new IllegalArgumentException("Negative array size");
		Class arrayClass = a.getClass().getComponentType();
		Object[] v = (Object[]) java.lang.reflect.Array.newInstance(arrayClass, newSize);
		System.arraycopy(a,0,v,0,newSize > a.length ? a.length : newSize);
		return v;
	}
	
	//// Array to String methods ////
	
	/**
	 * Converts a array of numbers into a String.
	 *
	 * @param array the array to convert
	 * @param fieldSeparator string that separates each element
	 * @return a String representation of the array
	 */
	public static String join(byte[] array, String fieldSeparator) {
		StringBuffer sb = new StringBuffer();
		for(int i = 0; i < array.length; i++)
			sb.append(array[i]+fieldSeparator);
		sb.setLength(sb.length() > 0 ? sb.length()-1 : 0);
		return sb.toString();
	}
	
	/**
	 * Converts a array of numbers into a String.
	 *
	 * @param array the array to convert
	 * @param fieldSeparator string that separates each element
	 * @return a String representation of the array
	 */
	public static String join(short[] array, String fieldSeparator) {
		StringBuffer sb = new StringBuffer();
		for(int i = 0; i < array.length; i++)
			sb.append(array[i]+fieldSeparator);
		sb.setLength(sb.length() > 0 ? sb.length()-1 : 0);
		return sb.toString();
	}
	
	/**
	 * Converts a array of numbers into a String.
	 *
	 * @param array the array to convert
	 * @param fieldSeparator string that separates each element
	 * @return a String representation of the array
	 */
	public static String join(int[] array, String fieldSeparator) {
		StringBuffer sb = new StringBuffer();
		for(int i = 0; i < array.length; i++)
			sb.append(array[i]+fieldSeparator);
		sb.setLength(sb.length() > 0 ? sb.length()-1 : 0);
		return sb.toString();
	}
	
	/**
	 * Converts a array of numbers into a String.
	 *
	 * @param array the array to convert
	 * @param fieldSeparator string that separates each element
	 * @return a String representation of the array
	 */
	public static String join(long[] array, String fieldSeparator) {
		StringBuffer sb = new StringBuffer();
		for(int i = 0; i < array.length; i++)
			sb.append(array[i]+fieldSeparator);
		sb.setLength(sb.length() > 0 ? sb.length()-1 : 0);
		return sb.toString();
	}
	
	/**
	 * Converts a array of reals into a String.
	 *
	 * @param array the array to convert
	 * @param fieldSeparator string that separates each element
	 * @return a String representation of the array
	 */
	public static String join(float[] array, String fieldSeparator) {
		StringBuffer sb = new StringBuffer();
		for(int i = 0; i < array.length; i++)
			sb.append(array[i]+fieldSeparator);
		sb.setLength(sb.length() > 0 ? sb.length()-1 : 0);
		return sb.toString();
	}
	
	/**
	 * Converts a array of reals into a String.
	 *
	 * @param array the array to convert
	 * @param fieldSeparator string that separates each element
	 * @return a String representation of the array
	 */
	public static String join(double[] array, String fieldSeparator) {
		StringBuffer sb = new StringBuffer();
		for(int i = 0; i < array.length; i++)
			sb.append(array[i]+fieldSeparator);
		sb.setLength(sb.length() > 0 ? sb.length()-1 : 0);
		return sb.toString();
	}
	
	/**
	 * Converts an array of Objects into a String.
	 *
	 * @param array the array to convert
	 * @param fieldSeparator string that separates each element
	 * @return a String representation of the array
	 */
	public static String join(Object[] array, String fieldSeparator) {
		StringBuffer sb = new StringBuffer();
		for(int i = 0; i < array.length; i++)
			sb.append(array[i]+fieldSeparator);
		sb.setLength(sb.length() > 0 ? sb.length()-1 : 0);
		return sb.toString();
	}
	/**
	 * prints statistics on an array
	 * @param d The array to analyze
	 */
	public static void printStats(int d[]){
		int min = Integer.MAX_VALUE;
		long max = -min;
		int maxIndex = 0;
		int i;
		long sum = 0;
		
		for(i = 0; i < d.length; i++){
			sum += d[i];
			if(d[i] < min){
				min = d[i];
			}
			if(d[i] > max){
				max = d[i];
				maxIndex = i;
			}
		}
		System.out.println("Min, max, maxIndex, avg = " + min +", " +
				max +", " + maxIndex + ", " + (double)sum/d.length);
	}
	/**
	 * prints statistics on an array
	 * @param d The array to analyze
	 */
	public static void printStats(double d[]){
		double min = Integer.MAX_VALUE;
		double max = -min;
		int maxIndex = 0;
		int i;
		long sum = 0;
		
		for(i = 0; i < d.length; i++){
			sum += d[i];
			if(d[i] < min){
				min = d[i];
			}
			if(d[i] > max){
				max = d[i];
				maxIndex = i;
			}
		}
		System.out.println("Min, max, maxIndex, avg = " + min +", " +
				max +", " + maxIndex + ", " + (double)sum/d.length);
	}
}
