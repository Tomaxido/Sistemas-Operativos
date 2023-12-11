JAVAC = javac
JAVA = java
MAIN_CLASS = fork_find

all: $(MAIN_CLASS).class

$(MAIN_CLASS).class: $(MAIN_CLASS).java
	$(JAVAC) $<

run: $(MAIN_CLASS).class
	$(JAVA) $(MAIN_CLASS)
	rm -f *.class

.PHONY: all run clean
