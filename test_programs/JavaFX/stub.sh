#!/bin/sh
JAVA_OPTS="--module-path /usr/share/openjfx/lib --add-modules javafx.controls,javafx.fxml"
MYSELF=`which "$0" 2>/dev/null`
[ $? -gt 0 -a -f "$0" ] && MYSELF="./$0"
java=java
if test -n "$JAVA_HOME"; then
    java="$JAVA_HOME/bin/java"
fi
exec "$java" $JAVA_OPTS $java_args -jar $MYSELF "$@"
#exit 1 
exit 1
