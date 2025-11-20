#simple:
#	javac Java2D.java
#	jar cvmf MANIFEST.MF Java2D.jar *.class
#	mkdir -p bin
#	cat stub.sh Java2D.jar > bin/Java2D_default && chmod +x bin/Java2D_default
#	rm *.class
#	rm Java2D.jar
#
#rects:
#	javac Java2D_rects.java
#	jar cvmf MANIFEST_RECTS.MF Java2D_rects.jar *.class
#	mkdir -p bin
#	cat stub.sh Java2D_rects.jar > bin/Java2D_rects && chmod +x bin/Java2D_rects
#	rm *.class
#	rm Java2D_rects.jar
#
#image:
#	javac Java2D_image.java
#	jar cvmf MANIFEST_IMAGE.MF Java2D_image.jar *.class
#	mkdir -p bin
#	cat stub.sh Java2D_image.jar > bin/Java2D_image && chmod +x bin/Java2D_image
#	rm *.class
#	rm Java2D_image.jar

LWJGL_HOME ?= $(HOME)/.local/share/lwjgl3
LWJGL_JARS := $(wildcard $(LWJGL_HOME)/jars/*.jar)
LWJGL_CP := $(shell echo $(LWJGL_JARS) | tr ' ' ':')

JAVAFLAGS = -cp .:$(LWJGL_CP)

custom:
	javac $(JAVAFLAGS) lwjgl3_custom.java

	# No manifest needed anymore
	jar cf lwjgl3_custom.jar *.class

	mkdir -p bin
	cat stub.sh lwjgl3_custom.jar > bin/lwjgl3_custom
	chmod +x bin/lwjgl3_custom

	rm *.class
	rm lwjgl3_custom.jar

all: custom

#all: simple rects image custom

clean:
	rm -rf bin
