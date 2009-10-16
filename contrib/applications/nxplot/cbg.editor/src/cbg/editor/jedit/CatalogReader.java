package cbg.editor.jedit;

import java.io.IOException;
import java.net.URL;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

import org.dom4j.Document;
import org.dom4j.DocumentException;
import org.dom4j.Element;
import org.dom4j.io.SAXReader;

import cbg.editor.ColoringEditorTools;
import cbg.editor.EditorPlugin;

public class CatalogReader {

	public CatalogReader() {
		super();
	}
	
	public Mode[] read(String filename) {
		URL mode = null;
		try {
			return readModes(ColoringEditorTools.getFile(filename));
		} catch (Exception e) {
			EditorPlugin.log("Error reading catalog file " + mode.getFile(), e);
			e.printStackTrace();
			return new Mode[0];
		}
	}
	
	public Mode[] readModes(URL modeFile) throws DocumentException, IOException {
		SAXReader reader = new SAXReader();
		Document doc = null;
		doc = reader.read(modeFile);		
		Element root = doc.getRootElement();
		List modeE = root.elements("MODE");
		List modes = new ArrayList(50);
		for (Iterator iter = modeE.iterator(); iter.hasNext();) {
			Element modeElement = (Element) iter.next();
			modes.add(newMode(modeElement));
		}
		return (Mode[]) modes.toArray(new Mode[modes.size()]);
	}

	private Mode newMode(Element modeElement) {
		return Mode.newMode(modeElement.attributeValue("NAME"),
			modeElement.attributeValue("FILE"),
			modeElement.attributeValue("FILE_NAME_GLOB"),
			modeElement.attributeValue("FIRST_LINE_GLOB"));
	}

}
