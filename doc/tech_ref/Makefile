IMAGES=tofnpd.jpg tofnpd.gif

DOC_SRC=NeXus_definitions.docbook terminology.docbook methods.docbook base_classes.docbook tofnpd.docbook tofnref.docbook monoref.docbook tofdgs.docbook monotas.docbook

all: docs

docs: NeXus_definitions.pdf

NeXus_definitions.pdf: $(DOC_SRC) $(IMAGES)
	docbook2pdf NeXus_definitions.docbook

NeXus_definitions.txt: $(DOC_SRC)
	docbook2txt NeXus_definitions.docboook
