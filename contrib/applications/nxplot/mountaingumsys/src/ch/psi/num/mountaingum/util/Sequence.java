/**
 * This is an interface for managing a sequence of edit items, like in 
 * a text editor. It is inspired by the paper:
 * 
 * Charles Crowley: Data Structures for Text Sequences
 * 
 * I have a gut feeling that I need this interface though the implementation for 
 * Mountaingum will be based on a primitive linked list.
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, January 2008
 */
package ch.psi.num.mountaingum.util;

public interface Sequence {
	/**
	 * is the sequence empty?
	 * @return true or flase
	 */
	public boolean empty();
	/**
	 * insert o at position
	 * @param position The position where to insert the item
	 * @param o The item to insert
	 */
	public void insert(int position, Object o);
	/**
	 * insert a sequence 
	 * @param position the position where to start inserting the sequence
	 * @param s The sequence to insert
	 */
	public void insert(int position, Sequence s);
	/**
	 * delete the item at pos
	 * @param pos The index of the item to delete.
	 */
	public void delete(int pos);
	/**
	 * delete several items
	 * @param start
	 * @param end
	 */
	public void delete(int start, int end);
	/**
	 * return the item at pos
	 * @param pos The index into the sequence
	 * @return The element at pos or null.
	 */
	public Object itemAt(int pos);
	/**
	 * return the length of the sequence
	 * @return The length
	 */
	public int length();
}
