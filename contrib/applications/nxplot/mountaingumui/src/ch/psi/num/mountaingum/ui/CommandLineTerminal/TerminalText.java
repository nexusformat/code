/**
 * This is the text display for the Mountaingum terminal. It is based on code 
 * written by Tony Lam for Gumtree but has undergone significant changes 
 * in order to live in the mountaingum environment.
 * 
 *  copyright: GPL
 *  
 *  Mark Koennecke, December 2007
 */
package ch.psi.num.mountaingum.ui.CommandLineTerminal;

import java.util.Iterator;
import java.util.List;

import org.eclipse.swt.SWT;
import org.eclipse.swt.custom.StyleRange;
import org.eclipse.swt.custom.StyledText;
import org.eclipse.swt.custom.StyledTextContent;
import org.eclipse.swt.graphics.Color;
import org.eclipse.swt.graphics.Font;
import org.eclipse.swt.graphics.FontData;
import org.eclipse.swt.widgets.Composite;

import ch.psi.num.mountaingum.sys.TerminalListener;

public class TerminalText extends StyledText implements TerminalListener {
	static final int MAXTEXT = 262144; // 256 KB
	
	private Color blue;

	private Color black;

	private Color red;
	
	private Color orange;
	
	private int wrapSize;

	private Font uff;
	
	public TerminalText(Composite parent, int style) {
		super(parent, style);
		blue = new Color(getDisplay(), 0, 0, 255);
		black = new Color(getDisplay(), 0, 0, 0);
		red = new Color(getDisplay(), 255, 0, 0);
		orange = new Color(getDisplay(), 255, 140, 0);
		wrapSize = 256;
		uff = null;
		FontData fd = new FontData("MiscFixed",12,SWT.BOLD);
		if(fd != null){
			uff = new Font(parent.getDisplay(),fd);
			if(uff != null){
				setFont(uff);
			}
		}
		setTextLimit(MAXTEXT);
	}

	public void appendInputText(String text) {
		append("\n");
		StyleRange styleRange = new StyleRange();
		styleRange.start = getCharCount();
		styleRange.length = 1;
		styleRange.fontStyle = SWT.BOLD;
		append(">> ");
		setStyleRange(styleRange);
		styleRange = new StyleRange();
		styleRange.start = getCharCount();
		styleRange.length = text.length() + 1;
		styleRange.fontStyle = SWT.BOLD;
		styleRange.foreground = blue;
		append(text + "\n");
		setStyleRange(styleRange);
		autoScroll();
	}

	public void appendOutputText(String text, int style) {
		
		String multi[] = text.split("\n");
		if(multi.length > 1){
			for(int i = 0; i < multi.length; i++){
				appendOutputText(multi[i],style);
			}
			return;
		}
		
		int wrapNewLineCount = 0;
		Color fg;
		switch(style){
		case OutputStyle.ERROR:
			fg = red;
			break;
		case OutputStyle.WARNING:
			fg = orange;
			break;
		default:
			fg = black;
			break;
		}
		
		for(int i = 0; i < text.length(); i += wrapSize) {
			StyleRange styleRange = new StyleRange();
			if(i != 0) {
				append("-> ");
			}
			styleRange.start = getCharCount();
			if((i + wrapSize) >= text.length()) {
				append(text.substring(i, text.length()) + "\n");
				styleRange.length = text.length() - i + 1;
				styleRange.foreground = fg;
				setStyleRange(styleRange);
			} else {
				append(text.substring(i, i + wrapSize) + "\n");
				styleRange.length = wrapSize + 1;
				styleRange.foreground = fg;
				setStyleRange(styleRange);
			}
			autoScroll();
			wrapNewLineCount++;
		}
	}

	private void autoScroll() {
		StyledTextContent doc = getContent();
		int docLength = doc.getCharCount();
		if (docLength > 0) {
			setCaretOffset(docLength);
			showSelection();
		}
	}

	public void dispose() {
		blue.dispose();
		black.dispose();
		red.dispose();
		orange.dispose();
		if(uff != null){
			uff.dispose();
		}
		super.dispose();
	}

	public void appendLine(String text){
		if(text.indexOf("ERROR") >= 0){
			appendOutputText(text,OutputStyle.ERROR);
		} else if(text.indexOf("WARNING") >= 0){
			appendOutputText(text,OutputStyle.WARNING);
		} else {
			appendOutputText(text,OutputStyle.NORMAL);
		}
	}
	public void handleOutput(List lines) {
		final List lineList = lines;
		if(isDisposed()){
			return;
		}
		getDisplay().asyncExec(new Runnable() {
			public void run() {
				String text;
				Object o;
				Iterator it = lineList.iterator();
				while(it.hasNext()){
					o = it.next();
					if(o instanceof StringBuffer){
						text = ((StringBuffer)o).toString();
						appendLine(text);
					} else if(o instanceof String){
						text = (String)o;
						appendLine(text);				
					}
				}
			}
		});
	}	
}
