package org.nexusformat;

import java.awt.Color;
import java.awt.Component;

import javax.swing.Icon;
import javax.swing.ImageIcon;
import javax.swing.JLabel;
import javax.swing.JTree;
import javax.swing.tree.DefaultMutableTreeNode;
import javax.swing.tree.TreeCellRenderer;

import org.xml.sax.Attributes;

public class NXvalidateGuiTree {

	public static class ITag implements IconAndTipCarrier {
		private String name;

		private String data;

		private Attributes attr;

		private Icon icon;

		private String tipText;

		public ITag(String n, Attributes a) {
			name = n;
			attr = a;
			for (int i = 0; i < attr.getLength(); i++) {
				String aname = attr.getQName(i);
				String value = attr.getValue(i);
				if (aname.equals("icon")) {
					tipText = value;
					icon = new ImageIcon(value);
					break;
				}
			}
		}

		public String getName() {
			return name;
		}

		public Attributes getAttributes() {
			return attr;
		}

		public void setData(String d) {
			data = d;
		}

		public String getData() {
			return data;
		}

		public String getToolTipText() {
			return tipText;
		}

		public Icon getIcon() {
			return icon;
		}

		public void addData(String d) {
			if (data == null) {
				setData(d);
			} else {
				data += d;
			}
		}

		public String getAttributesAsString() {
			StringBuffer buf = new StringBuffer(256);
			for (int i = 0; i < attr.getLength(); i++) {
				buf.append(attr.getQName(i));
				buf.append("=\"");
				buf.append(attr.getValue(i));
				buf.append("\"");
			}
			return buf.toString();
		}

		public String toString() {
			String a = getAttributesAsString();
			return name + ": " + a + (data == null ? "" : " (" + data + ")");
		}
	}

}

interface IconAndTipCarrier {
	public Icon getIcon();

	public String getToolTipText();
}

class IconAndTipRenderer extends JLabel implements TreeCellRenderer {
	/**
	 * 
	 */
	private static final long serialVersionUID = -6749045036022861743L;

	Color backColor = new Color(0xFF, 0xCC, 0xFF);

	Icon openIcon, closedIcon, leafIcon;

	String tipText = "";

	public IconAndTipRenderer(Icon open, Icon closed, Icon leaf) {
		openIcon = open;
		closedIcon = closed;
		leafIcon = leaf;
		setBackground(backColor);
		setForeground(Color.black);
	}

	public Component getTreeCellRendererComponent(JTree tree, Object value,
			boolean selected, boolean expanded, boolean leaf, int row,
			boolean hasFocus) {
		setText(value.toString());
		if (selected) {
			setOpaque(true);
		} else {
			setOpaque(false);
		}

		IconAndTipCarrier itc = null;
		if (value instanceof DefaultMutableTreeNode) {
			Object uo = ((DefaultMutableTreeNode) value).getUserObject();
			if (uo instanceof IconAndTipCarrier) {
				itc = (IconAndTipCarrier) uo;
			}
		} else if (value instanceof IconAndTipCarrier) {
			itc = (IconAndTipCarrier) value;
		}
		if ((itc != null) && (itc.getIcon() != null)) {
			setIcon(itc.getIcon());
			tipText = itc.getToolTipText();
		} else {
			tipText = " ";
			if (expanded) {
				setIcon(openIcon);
			} else if (leaf) {
				setIcon(leafIcon);
			} else {
				setIcon(closedIcon);
			}
		}
		return this;
	}

	public String getToolTipText() {
		return tipText;
	}
}
