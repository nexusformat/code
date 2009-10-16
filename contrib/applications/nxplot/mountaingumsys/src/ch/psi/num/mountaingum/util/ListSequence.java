/**
 * This is linked list implementation of the Sequence interface. This is inefficient
 * for text editing but as Maountaingum batch queues are not expected to hold more 
 * then a hundred entries, this is OK.
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, January 2008
 */
package ch.psi.num.mountaingum.util;

import java.util.LinkedList;

public class ListSequence implements Sequence {

	private LinkedList l;
	public ListSequence(){
		l = new LinkedList();
	}
	public void delete(int pos) {
		l.remove(pos);
	}
	public void delete(int start, int end) {
		for(int i = start; i < end; i++){
			l.remove(start);
		}
	}
	public boolean empty() {
		return l.isEmpty();
	}
	public void insert(int position, Object o) {
		l.add(position,o);
	}

	public void insert(int position, Sequence s) {
		for(int i = 0; i < s.length(); i++){
			l.add(position+i,s.itemAt(i));
		}
	}

	public Object itemAt(int pos) {
		return l.get(pos);
	}

	public int length() {
		return l.size();
	}
}
