/**
 * This is the command line terminal to an instrument control system for 
 * mountaingum. It is based on code written by Tony Lam for GumTree. 
 * However, the code has been heavily reworked to remove the connection
 * pane and to integrate with the SICSIO I/O system. 
 * 
 * copyright: GPL
 * 
 * Mark Koennecke, December 2007
 */
package ch.psi.num.mountaingum.ui.CommandLineTerminal;

import java.util.ArrayList;
import java.util.List;

import org.eclipse.jface.bindings.keys.KeyStroke;
import org.eclipse.jface.bindings.keys.ParseException;
import org.eclipse.jface.fieldassist.ContentProposalAdapter;
import org.eclipse.jface.fieldassist.IContentProposal;
import org.eclipse.jface.fieldassist.IContentProposalListener;
import org.eclipse.jface.fieldassist.TextContentAdapter;
import org.eclipse.swt.SWT;
import org.eclipse.swt.events.FocusAdapter;
import org.eclipse.swt.events.FocusEvent;
import org.eclipse.swt.events.KeyAdapter;
import org.eclipse.swt.events.KeyEvent;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.events.SelectionListener;
import org.eclipse.swt.graphics.Color;
import org.eclipse.swt.layout.FillLayout;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Group;
import org.eclipse.swt.widgets.Label;
import org.eclipse.swt.widgets.Text;
import org.eclipse.ui.PlatformUI;
import org.eclipse.ui.part.ViewPart;

import ch.psi.num.mountaingum.sys.SysRegistry;
import ch.psi.num.mountaingum.sys.TerminalInterface;

public class CommandLineTerminal extends ViewPart implements SelectionListener {
	public static String ID = "mountaingumui.CommandLineTerminal";
	private TerminalText textDisplay;

	private Text textInput;

	private Group testDisplayGroup;

	private int readLineIndex;

	private boolean proposalAccepted;

	private boolean inputFocusLost;

	private boolean canCommit;

	private FilteredContentProposalProvider provider;

	private List histories;

	private Color red;
	
	private TerminalInterface terminal;
	
	public CommandLineTerminal() {
		super();
		readLineIndex = -1;
		proposalAccepted = false;
		inputFocusLost = false;
		textDisplay = null;
		red = null;
	}

	public void createPartControl(Composite parent) {
		Composite composite = new Composite(parent, SWT.NONE);
		composite.setLayout(new GridLayout());

		testDisplayGroup = new Group(composite, SWT.NONE);
		testDisplayGroup.setText("Communication log");
		createTextDisplay();
		testDisplayGroup.setLayoutData(new GridData(SWT.FILL, SWT.FILL, true, true));

		Group textInputGroup = new Group(composite, SWT.NONE);
		textInputGroup.setText("Input");
		textInputGroup.setLayout(new FillLayout(SWT.VERTICAL));
		textInputGroup.setLayoutData(new GridData(SWT.FILL, SWT.CENTER, true, false));
		Label label = new Label(textInputGroup, SWT.NONE);
		label.setText("Press Ctrl+Space to trigger command history");
		textInput = new Text(textInputGroup,  SWT.BORDER | SWT.SINGLE);
		textInput.setEnabled(true);

		Button berta = new Button(composite,SWT.PUSH);
		berta.setText("Interrupt");
		berta.addSelectionListener(this);
	    if(red == null){
			red = new Color(parent.getDisplay(),255,0,0);
	    }
	    berta.setBackground(red);
	    berta.setLayoutData(new GridData(GridData.GRAB_HORIZONTAL|GridData.FILL_HORIZONTAL));
	
	    terminal = SysRegistry.getSystemAdapter().makeTerminalInterface();
	    terminal.setTerminalListener(textDisplay);
	    
		// Set content assistance
		provider = new FilteredContentProposalProvider();
		KeyStroke keyStroke = null;
		try {
			keyStroke = KeyStroke.getInstance("Ctrl+Space");
		} catch (ParseException e1) {
			e1.printStackTrace();
		}
		proposalAccepted = false;
		ContentProposalAdapter adapter = new ContentProposalAdapter(textInput,
				new TextContentAdapter(), provider, keyStroke, null);
		adapter.setProposalAcceptanceStyle(ContentProposalAdapter.PROPOSAL_REPLACE);

		adapter.addContentProposalListener(new IContentProposalListener() {
			public void proposalAccepted(IContentProposal proposal) {
				proposalAccepted = true;
				if(inputFocusLost) {
					canCommit = false;
					// add time delay to avoid fast key release that leads to commit
					new Thread(new Runnable() {
						public void run() {
							try {
								Thread.sleep(500);
							} catch (InterruptedException e) {
								Thread.currentThread().interrupt();
							} finally {
								canCommit = true;
							}
						}
					}).start();
				}
			}
		});


		textInput.addFocusListener(new FocusAdapter() {
			public void focusLost(FocusEvent e) {
				inputFocusLost = true;
			}
		});

		textInput.addKeyListener(new KeyAdapter() {
			public void keyReleased(KeyEvent e) {
				if (e.keyCode == SWT.KEYPAD_CR || e.keyCode == SWT.CR) {
					// proposal was selected without focusing into the popup,
					// so textInput key listener will be triggered
					// to prevent command is committed, a special flag is risen
					if (proposalAccepted) {
						if(!inputFocusLost) {
							// proposal accepted in text box
							proposalAccepted = false;
							return;
						} else {
							if(!canCommit) {
								// proposal accepted in pop up menu with enter
								return;
							}
						}
					}
					commitCommand(textInput.getText());
				} else if (e.keyCode == SWT.ARROW_UP) {
					if(getHistories().isEmpty())
						return;
					// Initialise
					if(readLineIndex == -1) {
						readLineIndex = getHistories().size() - 1;
					} else if(readLineIndex != 0) {
						// If it does not reach top command
						readLineIndex = readLineIndex - 1;
					}
					updateTextInput((String)getHistories().get(readLineIndex));
				} else if (e.keyCode == SWT.ARROW_DOWN) {
					if(getHistories().isEmpty() || readLineIndex == -1) {
						return;
					}
					// If it does not reach last command
					if(readLineIndex != getHistories().size() - 1) {
						readLineIndex = readLineIndex + 1;
					}
					updateTextInput((String)getHistories().get(readLineIndex));
				}
			}
		});
	}

	private void updateTextInput(String text) {
		textInput.setText(text);
		textInput.setSelection(text.length());
	}


	private void createTextDisplay() {
		testDisplayGroup.setLayout(new FillLayout());
		textDisplay = new TerminalText(testDisplayGroup, SWT.MULTI | SWT.V_SCROLL
	                | SWT.H_SCROLL | SWT.FULL_SELECTION | SWT.READ_ONLY);
		testDisplayGroup.layout(true);
	}

	public void dispose() {
		super.dispose();
		SysRegistry.getSystemAdapter().closeTerminalInterface(terminal);
	}

	private List  getHistories() {
		if(histories == null) {
			histories = new ArrayList();
		}
		return histories;
	}

	protected void commitCommand(String text) {
		try {
			getHistories().add(text);
			provider.addNewProposal(text);
			terminal.sendCommand(text);
			textInput.setText("");
			readLineIndex = -1;
			textDisplay.appendInputText(text);
		} catch (Exception e) {
			printError(e);
		}
	}
	private void printError(final Exception e) {
		PlatformUI.getWorkbench().getDisplay().asyncExec(new Runnable() {
			public void run() {
				textDisplay.append("Cannot connect to selected adapter.\n");
				textDisplay.append(e.getMessage() + '\n');
				textDisplay.append(e.getCause().toString());
			}
		});
	}

	public void setFocus() {
		// Nothing to do as of now, but the interface definition requires me
	}

	public void widgetDefaultSelected(SelectionEvent e) {
		// Nothing to do here, but the interface definition requires me
	}

	public void widgetSelected(SelectionEvent e) {
		terminal.interrupt();
	}

}
