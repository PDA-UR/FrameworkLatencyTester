#!/bin/sh

MYSELF="$0"
LWJGL_HOME="$HOME/.local/share/lwjgl3"

CP=""

# Add regular jars
for j in "$LWJGL_HOME"/jars/*.jar; do
    CP="$CP:$j"
done

# Add native jars (IMPORTANT!)
for j in "$LWJGL_HOME"/natives/*.jar; do
    CP="$CP:$j"
done

# Add the application jar (self-contained)
CP="$CP:$MYSELF"

java=java
if test -n "$JAVA_HOME"; then
    java="$JAVA_HOME/bin/java"
fi

exec "$java" -cp "$CP" lwjgl3_custom "$@"
exit 1
