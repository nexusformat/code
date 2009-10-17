package ch.psi.num.mountaingum.ui.TreeViewer;

import java.io.InputStream;
import java.net.URL;
import java.util.HashMap;
import java.util.Map;

import javax.swing.ImageIcon;

import org.eclipse.jface.resource.ImageDescriptor;
import org.eclipse.swt.graphics.Image;
import org.eclipse.swt.graphics.ImageData;

import com.sun.org.apache.bcel.internal.util.Class2HTML;

import ch.psi.num.mountaingum.tree.TreeNode;

public class IconTreeLabelProvider extends TreeLabelProvider {

	Map<String, Image> classToIcon = new HashMap<String, Image>();
	
	@Override
	public Image getColumnImage(Object element, int columnIndex) {
		// TODO Auto-generated method stub
		 if (element instanceof TreeNode) {
			TreeNode node = (TreeNode) element;
			String type = node.getProperty("type");
			if (type != null) {
				if (classToIcon.containsKey(type)) 
					return classToIcon.get(type);
				else {
					URL iconUrl = getClass().getResource("icons/"+type+".gif");
					ImageDescriptor iconDes = ImageDescriptor.createFromURL(iconUrl);
					Image icon = iconDes.createImage();
					classToIcon.put(type, icon);
					return icon;
				}
			}
		}
		 
		 Image columnImage = super.getColumnImage(element, columnIndex);
		 return columnImage;
	}

}
