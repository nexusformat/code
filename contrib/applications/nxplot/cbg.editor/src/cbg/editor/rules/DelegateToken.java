package cbg.editor.rules;

import cbg.editor.jedit.Rule;
import cbg.editor.jedit.Type;

public class DelegateToken extends CToken {
	protected Rule delegate;
	protected String end;
	protected boolean consumed;	
	public DelegateToken(Type type, Rule delegate, String end) {
		super(type);
		this.delegate = delegate;
		this.end = end;
		consumed = false;
	}
	public Object getData() {
		return delegate.getName() + super.getData();
	}

	public String getEnd() {
		return end;
	}
}
