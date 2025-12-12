OPENJFX="/usr/share/openjfx/lib"

simple:
	javac --module-path $(OPENJFX) --add-modules javafx.controls,javafx.fxml JavaFX.java
	jar cvmf MANIFEST.MF JavaFX.jar *.class
	mkdir -p bin
	cat stub.sh JavaFX.jar > bin/JavaFX_default && chmod +x bin/JavaFX_default
	rm *.class
	rm JavaFX.jar

rects:
	javac --module-path $(OPENJFX) --add-modules javafx.controls,javafx.fxml JavaFX_rects.java
	jar cvmf MANIFEST_RECTS.MF JavaFX_rects.jar *.class
	mkdir -p bin
	cat stub.sh JavaFX_rects.jar > bin/JavaFX_rects && chmod +x bin/JavaFX_rects
	rm *.class
	rm JavaFX_rects.jar

image:
	javac --module-path $(OPENJFX) --add-modules javafx.controls,javafx.fxml JavaFX_image.java
	jar cvmf MANIFEST_IMAGE.MF JavaFX_image.jar *.class
	mkdir -p bin
	cat stub.sh JavaFX_image.jar > bin/JavaFX_image && chmod +x bin/JavaFX_image
	rm *.class
	rm JavaFX_image.jar

custom:
	javac --module-path $(OPENJFX) --add-modules javafx.controls,javafx.fxml JavaFX_custom.java
	jar cvmf MANIFEST_CUSTOM.MF JavaFX_custom.jar *.class
	mkdir -p bin
	cat stub.sh JavaFX_custom.jar > bin/JavaFX_custom && chmod +x bin/JavaFX_custom
	rm *.class
	rm JavaFX_custom.jar

all: simple rects image custom

clean:
	rm -rf bin
