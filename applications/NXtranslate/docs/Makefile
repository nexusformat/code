DOC_SRC=NXtranslate.docbook overview.docbook translation.docbook \
        retrievers.docbook

all: docs

docs: NXtranslate.pdf

NXtranslate.pdf: $(DOC_SRC)
	docbook2pdf NXtranslate.docbook

NXtranslate.txt: $(DOC_SRC)
	docbook2txt NXtranslate.docboook
