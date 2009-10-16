/**
 * This is content proposal provider for Mountaingum. It is based on 
 * code written by Tony Lam for Gumtree. But it has been repackaged for
 * Mountaingum and dependencies on Java 6 have been removed.
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, December 2007
 */
package ch.psi.num.mountaingum.ui.CommandLineTerminal;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;

import org.eclipse.jface.fieldassist.IContentProposal;
import org.eclipse.jface.fieldassist.IContentProposalProvider;

public class FilteredContentProposalProvider implements IContentProposalProvider {

	private Map proposalMap;

	public FilteredContentProposalProvider() {
		proposalMap = new TreeMap();
	}

	public IContentProposal[] getProposals(String content, int position) {
		if(content == null || content.length() == 0) {
			return (IContentProposal[])proposalMap.values().toArray(new IContentProposal[proposalMap.values().size()]);
		}
		List matchedProposals = new ArrayList();
		Iterator it  = proposalMap.entrySet().iterator();
		while(it.hasNext()){
			matchedProposals.add(proposalMap.get(it.next()));
		}
		return (IContentProposal[])matchedProposals.toArray(new IContentProposal[matchedProposals.size()]);
	}

	public void addNewProposal(final String content) {
		if(!proposalMap.containsKey(content)) {
			proposalMap.put(content, new IContentProposal() {
				public String getContent() {
					return content;
				}
				public int getCursorPosition() {
					return getContent().length();
				}
				public String getDescription() {
					return null;
				}
				public String getLabel() {
					return null;
				}
			});
		}
	}

}
